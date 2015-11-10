#include "SapphirePrerequisites.h"
#include "SapphireFrustum.h"
#include "SapphireException.h"
#include "SapphireAxisAlignedBox.h"
#include "SapphireSphere.h"
#include "SapphirePlaneBoundedVolume.h"


namespace Sapphire
{

	//String Frustum::msMovableType = "Frustum";
	const Real Frustum::INFINITE_FAR_PLANE_ADJUST = 0.00001;
	//-----------------------------------------------------------------------
	Frustum::Frustum(const String& name) :
		mProjType(PT_PERSPECTIVE),
		mFOVy(Radian(Math::_PI / 4.0f)),
		mFarDist(100000.0f),
		mNearDist(100.0f),
		mAspect(1.33333333333333f),
		mOrthoHeight(1000),
		mFrustumOffset(Vector2::ZERO),
		mFocalLength(1.0f),
		mLastParentOrientation(Quaternion::IDENTITY),
		mLastParentPosition(Vector3::ZERO),
		mRecalcFrustum(true),
		mRecalcView(true),
		mRecalcFrustumPlanes(true),
		mRecalcWorldSpaceCorners(true),
		mRecalcVertexData(true),
		mCustomViewMatrix(false),
		mCustomProjMatrix(false),
		mFrustumExtentsManuallySet(false),
		mOrientationMode(OR_DEGREE_0),
		mReflect(false),
		//mLinkedReflectPlane(0),
		mObliqueDepthProjection(false)
		//mLinkedObliqueProjPlane(0)
	{
		// ��ʼ������
		//mMaterial = MaterialManager::getSingleton().getByName("BaseWhiteNoLighting");

		// �޸ĳ����Ա����
		//mVisible = false;
		//mParentNode = 0;
		//mName = name;

		//mLastLinkedReflectionPlane.normal = Vector3::ZERO;
		//mLastLinkedObliqueProjPlane.normal = Vector3::ZERO;

		updateView();
		updateFrustum();
	}


	Frustum::~Frustum()
	{
		// Do nothing
	}

	//-----------------------------------------------------------------------
	void Frustum::setFOVy(const Radian& fov)
	{
		mFOVy = fov;
		invalidateFrustum();   //��Ч����ǰ��׶�壬���и���
	}

	//-----------------------------------------------------------------------
	const Radian& Frustum::getFOVy(void) const
	{
		return mFOVy;
	}


	//-----------------------------------------------------------------------
	void Frustum::setFarClipDistance(Real farPlane)
	{
		mFarDist = farPlane;
		invalidateFrustum();
	}

	//-----------------------------------------------------------------------
	Real Frustum::getFarClipDistance(void) const
	{
		return mFarDist;
	}
	//-----------------------------------------------------------------------
	void Frustum::setNearClipDistance(Real nearPlane)
	{
		//������ƽ�治��С��=0
		if (nearPlane <= 0)
			//�׳��쳣
			SAPPHIRE_EXCEPT(Exception::ERR_INVALIDPARAMS, "Near clip distance must be greater than zero.",
			"Frustum::setNearClipDistance");
		mNearDist = nearPlane;
		invalidateFrustum();
	}
	//-----------------------------------------------------------------------
	Real Frustum::getNearClipDistance(void) const
	{
		return mNearDist;
	}

	//---------------------------------------------------------------------
	void Frustum::setFrustumOffset(const Vector2& offset)
	{
		mFrustumOffset = offset;
		invalidateFrustum();
	}
	//---------------------------------------------------------------------
	void Frustum::setFrustumOffset(Real horizontal, Real vertical)
	{
		setFrustumOffset(Vector2(horizontal, vertical));
	}
	//---------------------------------------------------------------------
	const Vector2& Frustum::getFrustumOffset() const
	{
		return mFrustumOffset;
	}
	//---------------------------------------------------------------------
	void Frustum::setFocalLength(Real focalLength)
	{
		if (focalLength <= 0)
		{
			//�׳��쳣
			SAPPHIRE_EXCEPT(Exception::ERR_INVALIDPARAMS,
				"Focal length must be greater than zero.",
				"Frustum::setFocalLength");
		}

		mFocalLength = focalLength;
		invalidateFrustum();
	}
	//---------------------------------------------------------------------
	Real Frustum::getFocalLength() const
	{
		return mFocalLength;
	}
	//-----------------------------------------------------------------------
	const Matrix4& Frustum::getProjectionMatrix(void) const
	{

		updateFrustum();

		return mProjMatrix;
	}
	//-----------------------------------------------------------------------
	const Matrix4& Frustum::getProjectionMatrixWithRSDepth(void) const
	{

		updateFrustum();

		return mProjMatrixRSDepth;
	}
	//-----------------------------------------------------------------------
	const Matrix4& Frustum::getProjectionMatrixRS(void) const
	{

		updateFrustum();

		return mProjMatrixRS;
	}
	//-----------------------------------------------------------------------
	const Matrix4& Frustum::getViewMatrix(void) const
	{
		updateView();

		return mViewMatrix;

	}
	//-----------------------------------------------------------------------
	const Plane* Frustum::getFrustumPlanes(void) const
	{
		// ʹ�κ�δ�����ĸ��������׶��ƽ�淢������
		updateFrustumPlanes();

		return mFrustumPlanes;
	}

	//-----------------------------------------------------------------------
	const Plane& Frustum::getFrustumPlane(unsigned short plane) const
	{
		// ʹ�κ�δ�����ĸ��������׶��ƽ�淢������
		updateFrustumPlanes();

		return mFrustumPlanes[plane];

	}


	//-----------------------------------------------------------------------
	bool Frustum::isVisible(const AxisAlignedBox& bound, FrustumPlane* culledBy) const
	{
		// AABB�����Ǹ��պ��ӣ����ɼ�
		if (bound.isNull()) return false;

		// AABB�������޴�
		if (bound.isInfinite()) return true;

		// ���б�Ҫ�ĸ���
		updateFrustumPlanes();

		// ��ȡAABB���ӵ�����
		Vector3 centre = bound.getCenter();
		// ��ȡ���Ӱ��С����
		Vector3 halfSize = bound.getHalfSize();

		// ������׶���ÿ��ƽ�棬��ÿ��ƽ�棬 ��������еĵ㶼�ڷ���
		// ������������󲻿ɼ�
		for (int plane = 0; plane < 6; ++plane)
		{
			// �����׶�����ޣ�����Զ��ƽ��
			if (plane == FRUSTUM_PLANE_FAR && mFarDist == 0)
				continue;

			Plane::Side side = mFrustumPlanes[plane].getSide(centre, halfSize);
			if (side == Plane::NEGATIVE_SIDE)
			{
				// ���еĽǶ��ڷ��棬��ô������֮��
				if (culledBy)
					*culledBy = (FrustumPlane)plane;
				return false;
			}

		}

		return true;
	}

	//-----------------------------------------------------------------------
	bool Frustum::isVisible(const Vector3& vert, FrustumPlane* culledBy) const
	{
		// ����׶����б�Ҫ�ĸ���
		updateFrustumPlanes();

		// ������׶���ÿ��ƽ�棬��ÿ��ƽ�棬 ��������еĵ㶼�ڷ���
		// ������������󲻿ɼ�
		for (int plane = 0; plane < 6; ++plane)
		{
			// �����׶�����ޣ�����Զ��ƽ��
			if (plane == FRUSTUM_PLANE_FAR && mFarDist == 0)
				continue;

			if (mFrustumPlanes[plane].getSide(vert) == Plane::NEGATIVE_SIDE)
			{
				// ���еĽǶ��ڷ��棬��ô������֮��
				if (culledBy)
					*culledBy = (FrustumPlane)plane;
				return false;
			}

		}

		return true;
	}


	//-----------------------------------------------------------------------
	bool Frustum::isVisible(const Sphere& sphere, FrustumPlane* culledBy) const
	{

		updateFrustumPlanes();

		// ������׶���ÿ��ƽ�棬��ÿ��ƽ�棬 ��������еĵ㶼�ڷ���
		// ������������󲻿ɼ�
		for (int plane = 0; plane < 6; ++plane)
		{
			// �����׶��ռ����ޣ�����
			if (plane == FRUSTUM_PLANE_FAR && mFarDist == 0)
				continue;

			// ����������ĵ�ƽ��ľ���Ϊ�������Ҵ�������뾶����ô������׶��֮��
			if (mFrustumPlanes[plane].getDistance(sphere.getCenter()) < -sphere.getRadius())
			{
				// ���еĽ��ڷ��棬��ô������Ұ
				if (culledBy)
					*culledBy = (FrustumPlane)plane;
				return false;
			}

		}

		return true;
	}

	//---------------------------------------------------------------------
	uint32 Frustum::getTypeFlags(void) const
	{
		//return SceneManager::FRUSTUM_TYPE_MASK;
		return 0;
	}


	//-----------------------------------------------------------------------
	void Frustum::calcProjectionParameters(Real& left, Real& right, Real& bottom, Real& top) const
	{
		if (mCustomProjMatrix)  //�Զ���ͶӰ����
		{
			// ת������ƽ��ǵ�����ռ���
			Matrix4 invProj = mProjMatrix.inverse();   //��͸��ͶӰ��������ù۲����
			Vector3 topLeft(-0.5f, 0.5f, 0.0f);  //���Ͻ�
			Vector3 bottomRight(0.5f, -0.5f, 0.0f); //���½�

			topLeft = invProj * topLeft;
			bottomRight = invProj * bottomRight;

			left = topLeft.x;
			top = topLeft.y;
			right = bottomRight.x;
			bottom = bottomRight.y;

		}
		else
		{
			//��׶���ֶ���չ����
			if (mFrustumExtentsManuallySet)
			{
				left = mLeft;
				right = mRight;
				top = mTop;
				bottom = mBottom;
			}
			// ����ͨ�õ�ͶӰ����
			else if (mProjType == PT_PERSPECTIVE) //͸��ͶӰ
			{
				Radian thetaY(mFOVy * 0.5f);  //thetaY= mFovy��һ��
				Real tanThetaY = Math::Tan(thetaY);
				Real tanThetaX = tanThetaY * mAspect;
				//����3d������ʾ����
				Real nearFocal = mNearDist / mFocalLength;
				Real nearOffsetX = mFrustumOffset.x * nearFocal;
				Real nearOffsetY = mFrustumOffset.y * nearFocal;
				Real half_w = tanThetaX * mNearDist;
				Real half_h = tanThetaY * mNearDist;

				left = -half_w + nearOffsetX;
				right = +half_w + nearOffsetX;
				bottom = -half_h + nearOffsetY;
				top = +half_h + nearOffsetY;

				mLeft = left;
				mRight = right;
				mTop = top;
				mBottom = bottom;
			}
			else
			{
				//����ͶӰ
				// ��֪������Ӧ����׶��ƫ�Ƶ�����ͶӰ������������Ч��
				Real half_w = getOrthoWindowWidth() * 0.5f;
				Real half_h = getOrthoWindowHeight() * 0.5f;

				left = -half_w;
				right = +half_w;
				bottom = -half_h;
				top = +half_h;

				mLeft = left;
				mRight = right;
				mTop = top;
				mBottom = bottom;
			}

		}
	}
	//-----------------------------------------------------------------------

	void Frustum::updateFrustumImpl(void) const
	{
		// ͨ�ü���
		Real left, right, bottom, top;

#if SAPPHIRE_NO_VIEWPORT_ORIENTATIONMODE == 0
		if (mOrientationMode != OR_PORTRAIT)
			calcProjectionParameters(bottom, top, left, right);
		else
#endif
			calcProjectionParameters(left, right, bottom, top);

		//�Զ���ͶӰ����
		if (!mCustomProjMatrix)
		{

			// ��Щ��������Ҫ����ͨ�õ�ͶӰ������ glFrustum��glOrtho����
			// ���˳������ֶ��Ż��� ������Щ����������ҽ���
			//���w�ĵ���
			Real inv_w = 1 / (right - left);
			//�߶�h�ĵ���
			Real inv_h = 1 / (top - bottom);
			//���d�ĵ���
			Real inv_d = 1 / (mFarDist - mNearDist);

			// �����׶������ı䣬���¼���
			if (mProjType == PT_PERSPECTIVE)  //͸��ͶӰ
			{
				// �������Ԫ��
				Real A = 2 * mNearDist * inv_w;
				Real B = 2 * mNearDist * inv_h;
				Real C = (right + left) * inv_w;
				Real D = (top + bottom) * inv_h;
				Real q, qn;
				if (mFarDist == 0)
				{
					// ����Զƽ��
					q = Frustum::INFINITE_FAR_PLANE_ADJUST - 1;
					qn = mNearDist * (Frustum::INFINITE_FAR_PLANE_ADJUST - 2);
				}
				else
				{
					q = -(mFarDist + mNearDist) * inv_d;
					qn = -2 * (mFarDist * mNearDist) * inv_d;
				}

				// NB: �ⴴ��'uniform'��͸��ͶӰ������ȷ�Χ[-1,1]�����ֹ���
				//
				// [ A   0   C   0  ]
				// [ 0   B   D   0  ]
				// [ 0   0   q   qn ]
				// [ 0   0   -1  0  ]
				//
				// A = 2 * near / (right - left)
				// B = 2 * near / (top - bottom)
				// C = (right + left) / (right - left)
				// D = (top + bottom) / (top - bottom)
				// q = - (far + near) / (far - near)
				// qn = - 2 * (far * near) / (far - near)

				mProjMatrix = Matrix4::ZERO;
				mProjMatrix[0][0] = A;
				mProjMatrix[0][2] = C;
				mProjMatrix[1][1] = B;
				mProjMatrix[1][2] = D;
				mProjMatrix[2][2] = q;
				mProjMatrix[2][3] = qn;
				mProjMatrix[3][2] = -1;

				if (mObliqueDepthProjection)  //���ͶӰ
				{
					// �任���ƽ��Ĺ۲�ռ�

					// ���ͨ������ͷ���һ���޳���׶��۲�������أ�����ʹ��getViewMatrix
					updateView();
					Plane plane = mViewMatrix * mObliqueProjPlane;

					//������пռ���ڼ���ƽ��Ľǵ�
					// �� (sgn(clipPlane.x), sgn(clipPlane.y), 1, 1) ����ͨ����ͶӰ����������任��������ռ�


					/* ͨ�ð汾
					Vector4 q = matrix.inverse() *
					Vector4(Math::Sign(plane.normal.x),
					Math::Sign(plane.normal.y), 1.0f, 1.0f);
					*/
					Vector4 qVec;
					qVec.x = (Math::Sign(plane.normal.x) + mProjMatrix[0][2]) / mProjMatrix[0][0];
					qVec.y = (Math::Sign(plane.normal.y) + mProjMatrix[1][2]) / mProjMatrix[1][1];
					qVec.z = -1;
					qVec.w = (1 + mProjMatrix[2][2]) / mProjMatrix[2][3];

					// ��������ƽ������
					Vector4 clipPlane4d(plane.normal.x, plane.normal.y, plane.normal.z, plane.d);
					Vector4 c = clipPlane4d * (2 / (clipPlane4d.dotProduct(qVec)));

					// �滻ͶӰ����ĵ�����
					mProjMatrix[2][0] = c.x;
					mProjMatrix[2][1] = c.y;
					mProjMatrix[2][2] = c.z + 1;
					mProjMatrix[2][3] = c.w;
				}
			}
			else if (mProjType == PT_ORTHOGRAPHIC)
			{
				Real A = 2 * inv_w;
				Real B = 2 * inv_h;
				Real C = -(right + left) * inv_w;
				Real D = -(top + bottom) * inv_h;
				Real q, qn;
				if (mFarDist == 0)
				{
					// ���ܳ�������Զƽ�棬Ҫ�����0
					q = -Frustum::INFINITE_FAR_PLANE_ADJUST / mNearDist;
					qn = -Frustum::INFINITE_FAR_PLANE_ADJUST - 1;
				}
				else
				{
					q = -2 * inv_d;
					qn = -(mFarDist + mNearDist)  * inv_d;
				}

				// NB: ����'uniform'����ͶӰ������ȷ�Χ[-1,1]�� ���ֹ淶
				//
				// [ A   0   0   C  ]
				// [ 0   B   0   D  ]
				// [ 0   0   q   qn ]
				// [ 0   0   0   1  ]
				//
				// A = 2 * / (right - left)
				// B = 2 * / (top - bottom)
				// C = - (right + left) / (right - left)
				// D = - (top + bottom) / (top - bottom)
				// q = - 2 / (far - near)
				// qn = - (far + near) / (far - near)

				mProjMatrix = Matrix4::ZERO;
				mProjMatrix[0][0] = A;
				mProjMatrix[0][3] = C;
				mProjMatrix[1][1] = B;
				mProjMatrix[1][3] = D;
				mProjMatrix[2][2] = q;
				mProjMatrix[2][3] = qn;
				mProjMatrix[3][3] = 1;
			}
		} // 

#if SAPPHIRE_NO_VIEWPORT_ORIENTATIONMODE == 0
		// ������ģʽ
		mProjMatrix = mProjMatrix * Quaternion(Degree(mOrientationMode * 90.f), Vector3::UNIT_Z);
#endif

		//RenderSystem* renderSystem = Root::getSingleton().getRenderSystem();
		//ָ��API
		//renderSystem->_convertProjectionMatrix(mProjMatrix, mProjMatrixRS);
		//ָ��GPU��̵�APIģʽ
		//renderSystem->_convertProjectionMatrix(mProjMatrix, mProjMatrixRSDepth, true);


		// ����AABB��ײ���ӣ����أ�
		// �����Ǵ�0��-z�� ���Χ��Զ��ƽ��
		// ��׶�����ֵ
		Real farDist = (mFarDist == 0) ? 100000 : mFarDist;
		// ������ƽ��ı߽�
		Vector3 min(left, bottom, -farDist);
		Vector3 max(right, top, 0);

		if (mCustomProjMatrix)
		{
			//��Щ�Զ����ͶӰ�����ܹ��ò����õ��෴����
			// ����ȷ��AABB�ǿ�ʼ����ȷ������
			Vector3 tmp = min;
			min.makeFloor(max);
			max.makeCeil(tmp);
		}

		if (mProjType == PT_PERSPECTIVE)
		{
			// �ϲ�Զ��ƽ��߽�
			Real radio = farDist / mNearDist;
			min.makeFloor(Vector3(left * radio, bottom * radio, -farDist));
			max.makeCeil(Vector3(right * radio, top * radio, 0));
		}
		mBoundingBox.setExtents(min, max);

		mRecalcFrustum = false;

		//�����׶�����ƽ�����
		mRecalcFrustumPlanes = true;
	}
	//----------------------------------------------------------------------
	void Frustum::updateFrustum(void) const
	{
		if (isFrustumOutOfDate())
		{
			updateFrustumImpl();
		}
	}

	//���¶�������
	void Frustum::updateVertexData(void) const
	{
		/*
		if (mRecalcVertexData)
		{
		if (mVertexData.vertexBufferBinding->getBufferCount() <= 0)
		{
		// ��ʼ������&��������
		mVertexData.vertexDeclaration->addElement(0, 0, VET_FLOAT3, VES_POSITION);
		mVertexData.vertexCount = 32;
		mVertexData.vertexStart = 0;
		mVertexData.vertexBufferBinding->setBinding(0,
		HardwareBufferManager::getSingleton().createVertexBuffer(
		sizeof(float) * 3, 32, HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY));
		}

		// Note: ���������������ܹ���ͨ��ͶӰ����������������������Զƽ�治���ݣ�����������ȻҪ��ͶӰ��������

		// ���������ƽ����Ľ�
		Real vpLeft, vpRight, vpBottom, vpTop;
		calcProjectionParameters(vpLeft, vpRight, vpBottom, vpTop);

		// ������Զ����������ƣ�0Ϊ����Զ
		Real farDist = (mFarDist == 0) ? 100000 : mFarDist;

		// ����ƽ��Ľ�
		Real radio = mProjType == PT_PERSPECTIVE ? farDist / mNearDist : 1;
		Real farLeft = vpLeft * radio;
		Real farRight = vpRight * radio;
		Real farBottom = vpBottom * radio;
		Real farTop = vpTop * radio;

		// ���㶥��λ��(�ֲ�)
		// 0 ��ԭ��
		// 1, 2, 3, 4 �ǽ�����ƽ������Ͻǿ�ʼ��˳ʱ�뷽��
		// 5, 6, 7, 8 ��Զ��ƽ�棬���Ͻǿ�ʼ��˳ʱ�뷽��
		HardwareVertexBufferSharedPtr vbuf = mVertexData.vertexBufferBinding->getBuffer(0);
		float* pFloat = static_cast<float*>(vbuf->lock(HardwareBuffer::HBL_DISCARD));

		// ������ƽ�� (�ǵ���׶����-z ������ ��������ϵ)
		*pFloat++ = vpLeft;  *pFloat++ = vpTop;    *pFloat++ = -mNearDist;
		*pFloat++ = vpRight; *pFloat++ = vpTop;    *pFloat++ = -mNearDist;

		*pFloat++ = vpRight; *pFloat++ = vpTop;    *pFloat++ = -mNearDist;
		*pFloat++ = vpRight; *pFloat++ = vpBottom; *pFloat++ = -mNearDist;

		*pFloat++ = vpRight; *pFloat++ = vpBottom; *pFloat++ = -mNearDist;
		*pFloat++ = vpLeft;  *pFloat++ = vpBottom; *pFloat++ = -mNearDist;

		*pFloat++ = vpLeft;  *pFloat++ = vpBottom; *pFloat++ = -mNearDist;
		*pFloat++ = vpLeft;  *pFloat++ = vpTop;    *pFloat++ = -mNearDist;

		//Զ����ƽ��
		*pFloat++ = farLeft;  *pFloat++ = farTop;    *pFloat++ = -farDist;
		*pFloat++ = farRight; *pFloat++ = farTop;    *pFloat++ = -farDist;

		*pFloat++ = farRight; *pFloat++ = farTop;    *pFloat++ = -farDist;
		*pFloat++ = farRight; *pFloat++ = farBottom; *pFloat++ = -farDist;

		*pFloat++ = farRight; *pFloat++ = farBottom; *pFloat++ = -farDist;
		*pFloat++ = farLeft;  *pFloat++ = farBottom; *pFloat++ = -farDist;

		*pFloat++ = farLeft;  *pFloat++ = farBottom; *pFloat++ = -farDist;
		*pFloat++ = farLeft;  *pFloat++ = farTop;    *pFloat++ = -farDist;

		// �������ı�
		*pFloat++ = 0.0f;    *pFloat++ = 0.0f;   *pFloat++ = 0.0f;
		*pFloat++ = vpLeft;  *pFloat++ = vpTop;  *pFloat++ = -mNearDist;

		*pFloat++ = 0.0f;    *pFloat++ = 0.0f;   *pFloat++ = 0.0f;
		*pFloat++ = vpRight; *pFloat++ = vpTop;    *pFloat++ = -mNearDist;

		*pFloat++ = 0.0f;    *pFloat++ = 0.0f;   *pFloat++ = 0.0f;
		*pFloat++ = vpRight; *pFloat++ = vpBottom; *pFloat++ = -mNearDist;

		*pFloat++ = 0.0f;    *pFloat++ = 0.0f;   *pFloat++ = 0.0f;
		*pFloat++ = vpLeft;  *pFloat++ = vpBottom; *pFloat++ = -mNearDist;

		// ���ӵı�

		*pFloat++ = vpLeft;  *pFloat++ = vpTop;  *pFloat++ = -mNearDist;
		*pFloat++ = farLeft;  *pFloat++ = farTop;  *pFloat++ = -farDist;

		*pFloat++ = vpRight; *pFloat++ = vpTop;    *pFloat++ = -mNearDist;
		*pFloat++ = farRight; *pFloat++ = farTop;    *pFloat++ = -farDist;

		*pFloat++ = vpRight; *pFloat++ = vpBottom; *pFloat++ = -mNearDist;
		*pFloat++ = farRight; *pFloat++ = farBottom; *pFloat++ = -farDist;

		*pFloat++ = vpLeft;  *pFloat++ = vpBottom; *pFloat++ = -mNearDist;
		*pFloat++ = farLeft;  *pFloat++ = farBottom; *pFloat++ = -farDist;


		vbuf->unlock();

		mRecalcVertexData = false;
		}
		*/
	}

	//-----------------------------------------------------------------------
	bool Frustum::isViewOutOfDate(void) const
	{
		/*
		// �������ڵ�
		if (mParentNode)
		{
		if (mRecalcView ||
		mParentNode->_getDerivedOrientation() != mLastParentOrientation ||
		mParentNode->_getDerivedPosition() != mLastParentPosition)
		{
		//����Ҫ�����Ķ��㳡���ڵ�ʱ�������
		mLastParentOrientation = mParentNode->_getDerivedOrientation();
		mLastParentPosition = mParentNode->_getDerivedPosition();
		mRecalcView = true;
		}
		}
		// ������ƽ��õ�����
		if (mLinkedReflectPlane &&
		!(mLastLinkedReflectionPlane == mLinkedReflectPlane->_getDerivedPlane()))
		{
		mReflectPlane = mLinkedReflectPlane->_getDerivedPlane();
		mReflectMatrix = Math::buildReflectionMatrix(mReflectPlane);
		mLastLinkedReflectionPlane = mLinkedReflectPlane->_getDerivedPlane();
		mRecalcView = true;
		}


		*/
		return mRecalcView;
	}


	bool Frustum::isFrustumOutOfDate(void) const
	{
		/*
		if (mObliqueDepthProjection)
		{

		if (isViewOutOfDate())
		{
		mRecalcFrustum = true;
		}

		if (mLinkedObliqueProjPlane &&
		!(mLastLinkedObliqueProjPlane == mLinkedObliqueProjPlane->_getDerivedPlane()))
		{
		mObliqueProjPlane = mLinkedObliqueProjPlane->_getDerivedPlane();
		mLastLinkedObliqueProjPlane = mObliqueProjPlane;
		mRecalcFrustum = true;
		}
		}
		*/
		return mRecalcFrustum;
	}



	void Frustum::updateViewImpl(void) const
	{
		// ----------------------
		// ���¹۲����
		// ----------------------

		// ��ȡ��Ԫ������

		if (!mCustomViewMatrix)
		{
			Matrix3 rot;
			const Quaternion& orientation = getOrientationForViewUpdate();
			const Vector3& position = getPositionForViewUpdate();

			mViewMatrix = Math::makeViewMatrix(position, orientation, mReflect ? &mReflectMatrix : 0);
		}

		mRecalcView = false;

		// ��Ǹ�����׶�����ƽ��
		mRecalcFrustumPlanes = true;
		// ��Ǹ�������ռ��
		mRecalcWorldSpaceCorners = true;
		// ���������бͶӰ��򿪸�����׶��,��Ϊ��ƽ����Ҫ�ڹ۲�ռ���
		if (mObliqueDepthProjection)
		{
			mRecalcFrustum = true;
		}
	}


	//---------------------------------------------------------------------
	void Frustum::calcViewMatrixRelative(const Vector3& relPos, Matrix4& matToUpdate) const
	{
		Matrix4 matTrans = Matrix4::IDENTITY;
		matTrans.setTrans(relPos);
		matToUpdate = getViewMatrix() * matTrans;

	}
	//-----------------------------------------------------------------------
	void Frustum::updateView(void) const
	{
		if (isViewOutOfDate())
		{
			updateViewImpl();
		}
	}
	//-----------------------------------------------------------------------
	void Frustum::updateFrustumPlanesImpl(void) const
	{
		// -------------------------
		// ������׶��ƽ��
		// -------------------------
		Matrix4 combo = mProjMatrix * mViewMatrix;   //MVP����

		mFrustumPlanes[FRUSTUM_PLANE_LEFT].normal.x = combo[3][0] + combo[0][0];
		mFrustumPlanes[FRUSTUM_PLANE_LEFT].normal.y = combo[3][1] + combo[0][1];
		mFrustumPlanes[FRUSTUM_PLANE_LEFT].normal.z = combo[3][2] + combo[0][2];
		mFrustumPlanes[FRUSTUM_PLANE_LEFT].d = combo[3][3] + combo[0][3];

		mFrustumPlanes[FRUSTUM_PLANE_RIGHT].normal.x = combo[3][0] - combo[0][0];
		mFrustumPlanes[FRUSTUM_PLANE_RIGHT].normal.y = combo[3][1] - combo[0][1];
		mFrustumPlanes[FRUSTUM_PLANE_RIGHT].normal.z = combo[3][2] - combo[0][2];
		mFrustumPlanes[FRUSTUM_PLANE_RIGHT].d = combo[3][3] - combo[0][3];

		mFrustumPlanes[FRUSTUM_PLANE_TOP].normal.x = combo[3][0] - combo[1][0];
		mFrustumPlanes[FRUSTUM_PLANE_TOP].normal.y = combo[3][1] - combo[1][1];
		mFrustumPlanes[FRUSTUM_PLANE_TOP].normal.z = combo[3][2] - combo[1][2];
		mFrustumPlanes[FRUSTUM_PLANE_TOP].d = combo[3][3] - combo[1][3];

		mFrustumPlanes[FRUSTUM_PLANE_BOTTOM].normal.x = combo[3][0] + combo[1][0];
		mFrustumPlanes[FRUSTUM_PLANE_BOTTOM].normal.y = combo[3][1] + combo[1][1];
		mFrustumPlanes[FRUSTUM_PLANE_BOTTOM].normal.z = combo[3][2] + combo[1][2];
		mFrustumPlanes[FRUSTUM_PLANE_BOTTOM].d = combo[3][3] + combo[1][3];

		mFrustumPlanes[FRUSTUM_PLANE_NEAR].normal.x = combo[3][0] + combo[2][0];
		mFrustumPlanes[FRUSTUM_PLANE_NEAR].normal.y = combo[3][1] + combo[2][1];
		mFrustumPlanes[FRUSTUM_PLANE_NEAR].normal.z = combo[3][2] + combo[2][2];
		mFrustumPlanes[FRUSTUM_PLANE_NEAR].d = combo[3][3] + combo[2][3];

		mFrustumPlanes[FRUSTUM_PLANE_FAR].normal.x = combo[3][0] - combo[2][0];
		mFrustumPlanes[FRUSTUM_PLANE_FAR].normal.y = combo[3][1] - combo[2][1];
		mFrustumPlanes[FRUSTUM_PLANE_FAR].normal.z = combo[3][2] - combo[2][2];
		mFrustumPlanes[FRUSTUM_PLANE_FAR].d = combo[3][3] - combo[2][3];

		// �����ϵ�λ����ʱ�����±�׼��
		for (int i = 0; i < 6; i++)
		{
			Real length = mFrustumPlanes[i].normal.normalise();
			mFrustumPlanes[i].d /= length;
		}

		mRecalcFrustumPlanes = false;
	}

	//-----------------------------------------------------------------------
	void Frustum::updateFrustumPlanes(void) const
	{
		updateView();
		updateFrustum();

		if (mRecalcFrustumPlanes)
		{
			updateFrustumPlanesImpl();
		}
	}
	//-----------------------------------------------------------------------
	void Frustum::updateWorldSpaceCornersImpl(void) const
	{
		Matrix4 eyeToWorld = mViewMatrix.inverseAffine(); //���۲�ռ�ת������ռ�ľ���

		// Note: ���������������ܹ���ͨ��ͶӰ����������������������Զƽ�治���ݣ�����������ȻҪ��ͶӰ��������  still need to working with projection parameters.

		// ���������ƽ��Ľ�
		Real nearLeft, nearRight, nearBottom, nearTop;
		calcProjectionParameters(nearLeft, nearRight, nearBottom, nearTop);

		// ��Զ����ƽ�����
		Real farDist = (mFarDist == 0) ? 100000 : mFarDist;

		// ����ƽ���Ľ�
		Real radio = mProjType == PT_PERSPECTIVE ? farDist / mNearDist : 1;
		Real farLeft = nearLeft * radio;
		Real farRight = nearRight * radio;
		Real farBottom = nearBottom * radio;
		Real farTop = nearTop * radio;

		// ������ƽ��
		mWorldSpaceCorners[0] = eyeToWorld.transformAffine(Vector3(nearRight, nearTop, -mNearDist));
		mWorldSpaceCorners[1] = eyeToWorld.transformAffine(Vector3(nearLeft, nearTop, -mNearDist));
		mWorldSpaceCorners[2] = eyeToWorld.transformAffine(Vector3(nearLeft, nearBottom, -mNearDist));
		mWorldSpaceCorners[3] = eyeToWorld.transformAffine(Vector3(nearRight, nearBottom, -mNearDist));
		// Զ����ƽ��
		mWorldSpaceCorners[4] = eyeToWorld.transformAffine(Vector3(farRight, farTop, -farDist));
		mWorldSpaceCorners[5] = eyeToWorld.transformAffine(Vector3(farLeft, farTop, -farDist));
		mWorldSpaceCorners[6] = eyeToWorld.transformAffine(Vector3(farLeft, farBottom, -farDist));
		mWorldSpaceCorners[7] = eyeToWorld.transformAffine(Vector3(farRight, farBottom, -farDist));


		mRecalcWorldSpaceCorners = false;
	}


	//-----------------------------------------------------------------------
	void Frustum::updateWorldSpaceCorners(void) const
	{
		updateView();

		if (mRecalcWorldSpaceCorners)
		{
			updateWorldSpaceCornersImpl();
		}

	}

	//-----------------------------------------------------------------------
	Real Frustum::getAspectRatio(void) const
	{
		return mAspect;
	}

	//-----------------------------------------------------------------------
	void Frustum::setAspectRatio(Real r)
	{
		mAspect = r;
		invalidateFrustum();
	}

	//-----------------------------------------------------------------------
	/*	const AxisAlignedBox& Frustum::getBoundingBox(void) const
	{
	return mBoundingBox;
	}
	//-----------------------------------------------------------------------
	void Frustum::_updateRenderQueue(RenderQueue* queue)
	{
	if (mDebugDisplay)
	{
	// Add self
	queue->addRenderable(this);
	}
	}
	//-----------------------------------------------------------------------
	const String& Frustum::getMovableType(void) const
	{
	return msMovableType;
	}
	//-----------------------------------------------------------------------
	Real Frustum::getBoundingRadius(void) const
	{
	return (mFarDist == 0) ? 100000 : mFarDist;
	}
	//-----------------------------------------------------------------------
	const MaterialPtr& Frustum::getMaterial(void) const
	{
	return mMaterial;
	}
	//-----------------------------------------------------------------------
	void Frustum::getRenderOperation(RenderOperation& op)
	{
	updateVertexData();
	op.operationType = RenderOperation::OT_LINE_LIST;
	op.useIndexes = false;
	op.useGlobalInstancingVertexBufferIsAvailable = false;
	op.vertexData = &mVertexData;
	}

	//-----------------------------------------------------------------------
	void Frustum::getWorldTransforms(Matrix4* xform) const
	{
	if (mParentNode)
	*xform = mParentNode->_getFullTransform();
	else
	*xform = Matrix4::IDENTITY;
	}
	//-----------------------------------------------------------------------
	Real Frustum::getSquaredViewDepth(const Camera* cam) const
	{
	// Calc from centre
	if (mParentNode)
	return (cam->getDerivedPosition()
	- mParentNode->_getDerivedPosition()).squaredLength();
	else
	return 0;
	}


	//-----------------------------------------------------------------------
	const LightList& Frustum::getLights(void) const
	{
	// N/A
	SAPPHIRE_DEFINE_STATIC_LOCAL(LightList, ll, ());
	//        static LightList ll;
	return ll;
	}
	//-----------------------------------------------------------------------
	void Frustum::_notifyCurrentCamera(Camera* cam)
	{
	// ȷ���߽����Ҫ����
	updateFrustum();

	MovableObject::_notifyCurrentCamera(cam);
	}
	*/
	// -------------------------------------------------------------------
	void Frustum::invalidateFrustum() const
	{
		mRecalcFrustum = true;
		mRecalcFrustumPlanes = true;
		mRecalcWorldSpaceCorners = true;
		mRecalcVertexData = true;
	}
	// -------------------------------------------------------------------
	void Frustum::invalidateView() const
	{
		mRecalcView = true;
		mRecalcFrustumPlanes = true;
		mRecalcWorldSpaceCorners = true;
	}
	// -------------------------------------------------------------------
	const Vector3* Frustum::getWorldSpaceCorners(void) const
	{
		updateWorldSpaceCorners();

		return mWorldSpaceCorners;
	}
	void Frustum::setProjectionType(ProjectionType pt)
	{
		mProjType = pt;
		invalidateFrustum();
	}
	//-----------------------------------------------------------------------
	ProjectionType Frustum::getProjectionType(void) const
	{
		return mProjType;
	}
	//-----------------------------------------------------------------------
	const Vector3& Frustum::getPositionForViewUpdate(void) const
	{
		return mLastParentPosition;
	}
	//-----------------------------------------------------------------------
	const Quaternion& Frustum::getOrientationForViewUpdate(void) const
	{
		return mLastParentOrientation;
	}
	//-----------------------------------------------------------------------
	/*
	void Frustum::enableReflection(const Plane& p)
	{
	mReflect = true;
	mReflectPlane = p;
	mLinkedReflectPlane = 0;
	mReflectMatrix = Math::buildReflectionMatrix(p);
	invalidateView();

	}
	//-----------------------------------------------------------------------
	void Frustum::enableReflection(const MovablePlane* p)
	{
	mReflect = true;
	mLinkedReflectPlane = p;
	mReflectPlane = mLinkedReflectPlane->_getDerivedPlane();
	mReflectMatrix = Math::buildReflectionMatrix(mReflectPlane);
	mLastLinkedReflectionPlane = mLinkedReflectPlane->_getDerivedPlane();
	invalidateView();
	}

	void Frustum::disableReflection(void)
	{
	mReflect = false;
	mLinkedReflectPlane = 0;
	mLastLinkedReflectionPlane.normal = Vector3::ZERO;
	invalidateView();
	}
	*/
	//---------------------------------------------------------------------

	bool Frustum::projectSphere(const Sphere& sphere,
		Real* left, Real* top, Real* right, Real* bottom) const
	{
		// �任��Դλ�õ�����ռ�

		updateView();
		Vector3 eyeSpacePos = mViewMatrix.transformAffine(sphere.getCenter()); //������������ת���۲�ռ���

		// ��ʼ��
		*left = *bottom = -1.0f;
		*right = *top = 1.0f;

		if (eyeSpacePos.z < 0)
		{
			updateFrustum();
			const Matrix4& projMatrix = getProjectionMatrix();
			Real r = sphere.getRadius();
			Real rsq = r * r;  //�뾶��ƽ��


			if (eyeSpacePos.squaredLength() <= rsq)
				return false;

			Real Lxz = Math::Sqr(eyeSpacePos.x) + Math::Sqr(eyeSpacePos.z);  //��ԭ��O,����P�� ����Vop��XZƽ���ϵ�ͶӰ
			Real Lyz = Math::Sqr(eyeSpacePos.y) + Math::Sqr(eyeSpacePos.z);  //��ԭ��O,����P�� ����Vop��YZƽ���ϵ�ͶӰ

			// �õ㷨ʽ �ҳ�������ƽ��
			// ����XZ 
			// ������θ��б�ʽ: b*b - 4ac
			// x = Nx
			// a = Lx^2 + Lz^2
			// b = -2rLx
			// c = r^2 - Lz^2
			Real a = Lxz;
			Real b = -2.0f * r * eyeSpacePos.x;
			Real c = rsq - Math::Sqr(eyeSpacePos.z);
			Real D = b*b - 4.0f*a*c;

			// ������
			if (D > 0)
			{
				Real sqrootD = Math::Sqrt(D);
				// ������η���ȡ���ߵķ���
				Real Nx0 = (-b + sqrootD) / (2 * a);
				Real Nx1 = (-b - sqrootD) / (2 * a);

				// ȡ��Z
				Real Nz0 = (r - Nx0 * eyeSpacePos.x) / eyeSpacePos.z;
				Real Nz1 = (r - Nx1 * eyeSpacePos.x) / eyeSpacePos.z;

				// ��ȡ�е�
				// ֻ�������ǰ����е�
				Real Pz0 = (Lxz - rsq) / (eyeSpacePos.z - ((Nz0 / Nx0) * eyeSpacePos.x));
				if (Pz0 < 0)
				{
					// ͶӰ�ڽ�����ƽ��������ռ�ĵ�
					Real nearx0 = (Nz0 * mNearDist) / Nx0;
					// ����������Ҫӳ�������ӿ�����
					// ��ͶӰ���󣬲��������е�����
					Vector3 relx0 = projMatrix * Vector3(nearx0, 0, -mNearDist);

					//�ҳ�������߻����ұ�
					Real Px0 = -(Pz0 * Nz0) / Nx0;
					if (Px0 > eyeSpacePos.x)
					{
						*right = std::min(*right, relx0.x);
					}
					else
					{
						*left = std::max(*left, relx0.x);
					}
				}
				Real Pz1 = (Lxz - rsq) / (eyeSpacePos.z - ((Nz1 / Nx1) * eyeSpacePos.x));
				if (Pz1 < 0)
				{
					//ͶӰ�ڽ�����ƽ��������ռ�ĵ�
					Real nearx1 = (Nz1 * mNearDist) / Nx1;
					// ����������Ҫӳ�������ӿ�����
					// ��ͶӰ���󣬲��������е�����
					Vector3 relx1 = projMatrix * Vector3(nearx1, 0, -mNearDist);

					//�ҳ�������߻����ұ�
					Real Px1 = -(Pz1 * Nz1) / Nx1;
					if (Px1 > eyeSpacePos.x)
					{
						*right = std::min(*right, relx1.x);
					}
					else
					{
						*left = std::max(*left, relx1.x);
					}
				}
			}


			// ������ YZ ƽ��
			// ������η����б�ʽ: b*b - 4ac
			// x = Ny
			// a = Ly^2 + Lz^2
			// b = -2rLy
			// c = r^2 - Lz^2
			a = Lyz;
			b = -2.0f * r * eyeSpacePos.y;
			c = rsq - Math::Sqr(eyeSpacePos.z);
			D = b*b - 4.0f*a*c;

			//������
			if (D > 0)
			{
				Real sqrootD = Math::Sqrt(D);
				// ������θ���÷��ߵķ���
				Real Ny0 = (-b + sqrootD) / (2 * a);
				Real Ny1 = (-b - sqrootD) / (2 * a);

				// ������ȡ��Z
				Real Nz0 = (r - Ny0 * eyeSpacePos.y) / eyeSpacePos.z;
				Real Nz1 = (r - Ny1 * eyeSpacePos.y) / eyeSpacePos.z;

				// ��ȡ�е�
				// ֻ�������ǰ����е�
				Real Pz0 = (Lyz - rsq) / (eyeSpacePos.z - ((Nz0 / Ny0) * eyeSpacePos.y));
				if (Pz0 < 0)
				{
					//ͶӰ�ڽ�����ƽ��������ռ�ĵ�
					Real neary0 = (Nz0 * mNearDist) / Ny0;
					// ����������Ҫӳ�������ӿ�����
					// ��ͶӰ���󣬲��������е�����
					Vector3 rely0 = projMatrix * Vector3(0, neary0, -mNearDist);

					//�ҳ�������߻����ұ�
					Real Py0 = -(Pz0 * Nz0) / Ny0;
					if (Py0 > eyeSpacePos.y)
					{
						*top = std::min(*top, rely0.y);
					}
					else
					{
						*bottom = std::max(*bottom, rely0.y);
					}
				}
				Real Pz1 = (Lyz - rsq) / (eyeSpacePos.z - ((Nz1 / Ny1) * eyeSpacePos.y));
				if (Pz1 < 0)
				{
					//ͶӰ�ڽ�����ƽ��������ռ�ĵ�
					Real neary1 = (Nz1 * mNearDist) / Ny1;
					// ����������Ҫӳ�������ӿ�����
					// ��ͶӰ���󣬲��������е�����
					Vector3 rely1 = projMatrix * Vector3(0, neary1, -mNearDist);

					//�ҳ�������߻����ұ�
					Real Py1 = -(Pz1 * Nz1) / Ny1;
					if (Py1 > eyeSpacePos.y)
					{
						*top = std::min(*top, rely1.y);
					}
					else
					{
						*bottom = std::max(*bottom, rely1.y);
					}
				}
			}
		}

		return (*left != -1.0f) || (*top != 1.0f) || (*right != 1.0f) || (*bottom != -1.0f);

	}
	void Frustum::setCustomProjectionMatrix(bool enable, const Matrix4& projMatrix)
	{
		mCustomProjMatrix = enable;
		if (enable)
		{
			mProjMatrix = projMatrix;
		}
		invalidateFrustum();
	}
	/*
	//---------------------------------------------------------------------
	void Frustum::enableCustomNearClipPlane(const MovablePlane* plane)
	{
		mObliqueDepthProjection = true;
		mLinkedObliqueProjPlane = plane;
		mObliqueProjPlane = plane->_getDerivedPlane();
		invalidateFrustum();
	}
	//---------------------------------------------------------------------
	void Frustum::enableCustomNearClipPlane(const Plane& plane)
	{
	mObliqueDepthProjection = true;
	mLinkedObliqueProjPlane = 0;
	mObliqueProjPlane = plane;
	invalidateFrustum();
	}
	//---------------------------------------------------------------------
	void Frustum::disableCustomNearClipPlane(void)
	{
	mObliqueDepthProjection = false;
	mLinkedObliqueProjPlane = 0;
	invalidateFrustum();
	}
	//---------------------------------------------------------------------
	
	*/

	//---------------------------------------------------------------------
	void Frustum::setCustomViewMatrix(bool enable, const Matrix4& viewMatrix)
	{
		mCustomViewMatrix = enable;
		if (enable)
		{
			assert(viewMatrix.isAffine());
			mViewMatrix = viewMatrix;
		}
		invalidateView();
	}

	//---------------------------------------------------------------------
	void Frustum::setOrthoWindow(Real w, Real h)
	{
		mOrthoHeight = h;
		mAspect = w / h;
		invalidateFrustum();
	}
	//---------------------------------------------------------------------
	void Frustum::setOrthoWindowHeight(Real h)
	{
		mOrthoHeight = h;
		invalidateFrustum();
	}
	//---------------------------------------------------------------------
	void Frustum::setOrthoWindowWidth(Real w)
	{
		mOrthoHeight = w / mAspect;
		invalidateFrustum();
	}
	//---------------------------------------------------------------------
	Real Frustum::getOrthoWindowHeight() const
	{
		return mOrthoHeight;
	}
	//---------------------------------------------------------------------
	Real Frustum::getOrthoWindowWidth() const
	{
		return mOrthoHeight * mAspect;
	}
	//---------------------------------------------------------------------
	/*
	void Frustum::visitRenderables(Renderable::Visitor* visitor,
	bool debugRenderables)
	{
	// Only displayed in debug
	if (debugRenderables)
	{
	visitor->visit(this, 0, true);
	}

	}
	*/
	//---------------------------------------------------------------------
	void Frustum::setFrustumExtents(Real left, Real right, Real top, Real bottom)
	{
		mFrustumExtentsManuallySet = true;
		mLeft = left;
		mRight = right;
		mTop = top;
		mBottom = bottom;

		invalidateFrustum();
	}

	//---------------------------------------------------------------------
	void Frustum::resetFrustumExtents()
	{
		mFrustumExtentsManuallySet = false;
		invalidateFrustum();
	}
	//---------------------------------------------------------------------
	void Frustum::getFrustumExtents(Real& outleft, Real& outright, Real& outtop, Real& outbottom) const
	{
		updateFrustum();
		outleft = mLeft;
		outright = mRight;
		outtop = mTop;
		outbottom = mBottom;
	}
	//---------------------------------------------------------------------

	PlaneBoundedVolume Frustum::getPlaneBoundedVolume()
	{
		updateFrustumPlanes();

		PlaneBoundedVolume volume;
		volume.planes.push_back(mFrustumPlanes[FRUSTUM_PLANE_NEAR]);
		volume.planes.push_back(mFrustumPlanes[FRUSTUM_PLANE_FAR]);
		volume.planes.push_back(mFrustumPlanes[FRUSTUM_PLANE_BOTTOM]);
		volume.planes.push_back(mFrustumPlanes[FRUSTUM_PLANE_TOP]);
		volume.planes.push_back(mFrustumPlanes[FRUSTUM_PLANE_LEFT]);
		volume.planes.push_back(mFrustumPlanes[FRUSTUM_PLANE_RIGHT]);
		return volume;
	}

	//---------------------------------------------------------------------
	void Frustum::setOrientationMode(OrientationMode orientationMode)
	{
#if SAPPHIRE_NO_VIEWPORT_ORIENTATIONMODE != 0
		SAPPHIRE_EXCEPT(Exception::ERR_NOT_IMPLEMENTED,
			"Setting Frustrum orientation mode is not supported",
			__FUNCTION__);
#endif
		mOrientationMode = orientationMode;
		invalidateFrustum();
	}
	//---------------------------------------------------------------------
	OrientationMode Frustum::getOrientationMode() const
	{
#if SAPPHIRE_NO_VIEWPORT_ORIENTATIONMODE != 0
		SAPPHIRE_EXCEPT(Exception::ERR_NOT_IMPLEMENTED,
			"Getting Frustrum orientation mode is not supported",
			__FUNCTION__);
#endif
		return mOrientationMode;
	}

}

