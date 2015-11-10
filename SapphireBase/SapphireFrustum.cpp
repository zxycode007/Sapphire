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
		// 初始化材质
		//mMaterial = MaterialManager::getSingleton().getByName("BaseWhiteNoLighting");

		// 修改超类成员变量
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
		invalidateFrustum();   //无效化当前视锥体，进行更新
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
		//近剪裁平面不能小于=0
		if (nearPlane <= 0)
			//抛出异常
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
			//抛出异常
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
		// 使任何未发生的更新这个视锥体平面发生更新
		updateFrustumPlanes();

		return mFrustumPlanes;
	}

	//-----------------------------------------------------------------------
	const Plane& Frustum::getFrustumPlane(unsigned short plane) const
	{
		// 使任何未发生的更新这个视锥体平面发生更新
		updateFrustumPlanes();

		return mFrustumPlanes[plane];

	}


	//-----------------------------------------------------------------------
	bool Frustum::isVisible(const AxisAlignedBox& bound, FrustumPlane* culledBy) const
	{
		// AABB盒子是个空盒子，不可见
		if (bound.isNull()) return false;

		// AABB盒子无限大
		if (bound.isInfinite()) return true;

		// 进行必要的更新
		updateFrustumPlanes();

		// 获取AABB盒子的中心
		Vector3 centre = bound.getCenter();
		// 获取盒子半大小向量
		Vector3 halfSize = bound.getHalfSize();

		// 遍历视锥体的每个平面，对每个平面， 见如果所有的点都在反面
		// 如果这样，对象不可见
		for (int plane = 0; plane < 6; ++plane)
		{
			// 如果视锥体无限，跳过远裁平面
			if (plane == FRUSTUM_PLANE_FAR && mFarDist == 0)
				continue;

			Plane::Side side = mFrustumPlanes[plane].getSide(centre, halfSize);
			if (side == Plane::NEGATIVE_SIDE)
			{
				// 所有的角都在反面，那么在视域之外
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
		// 对视锥体进行必要的更新
		updateFrustumPlanes();

		// 遍历视锥体的每个平面，对每个平面， 见如果所有的点都在反面
		// 如果这样，对象不可见
		for (int plane = 0; plane < 6; ++plane)
		{
			// 如果视锥体无限，跳过远裁平面
			if (plane == FRUSTUM_PLANE_FAR && mFarDist == 0)
				continue;

			if (mFrustumPlanes[plane].getSide(vert) == Plane::NEGATIVE_SIDE)
			{
				// 所有的角都在反面，那么在视域之外
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

		// 遍历视锥体的每个平面，对每个平面， 见如果所有的点都在反面
		// 如果这样，对象不可见
		for (int plane = 0; plane < 6; ++plane)
		{
			// 如果视锥体空间无限，跳过
			if (plane == FRUSTUM_PLANE_FAR && mFarDist == 0)
				continue;

			// 如果球体中心到平面的距离为负，并且大于球体半径，那么球在视锥体之外
			if (mFrustumPlanes[plane].getDistance(sphere.getCenter()) < -sphere.getRadius())
			{
				// 所有的角在反面，那么不在视野
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
		if (mCustomProjMatrix)  //自定义投影矩阵
		{
			// 转换剪裁平面角到相机空间中
			Matrix4 invProj = mProjMatrix.inverse();   //对透视投影矩阵求逆得观察矩阵
			Vector3 topLeft(-0.5f, 0.5f, 0.0f);  //左上角
			Vector3 bottomRight(0.5f, -0.5f, 0.0f); //右下角

			topLeft = invProj * topLeft;
			bottomRight = invProj * bottomRight;

			left = topLeft.x;
			top = topLeft.y;
			right = bottomRight.x;
			bottom = bottomRight.y;

		}
		else
		{
			//视锥体手动扩展设置
			if (mFrustumExtentsManuallySet)
			{
				left = mLeft;
				right = mRight;
				top = mTop;
				bottom = mBottom;
			}
			// 计算通用的投影参数
			else if (mProjType == PT_PERSPECTIVE) //透视投影
			{
				Radian thetaY(mFOVy * 0.5f);  //thetaY= mFovy的一半
				Real tanThetaY = Math::Tan(thetaY);
				Real tanThetaX = tanThetaY * mAspect;
				//用于3d立体显示参数
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
				//正交投影
				// 不知道怎样应用视锥体偏移到正交投影相机，这里就无效化
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
		// 通用计算
		Real left, right, bottom, top;

#if SAPPHIRE_NO_VIEWPORT_ORIENTATIONMODE == 0
		if (mOrientationMode != OR_PORTRAIT)
			calcProjectionParameters(bottom, top, left, right);
		else
#endif
			calcProjectionParameters(left, right, bottom, top);

		//自定义投影矩阵
		if (!mCustomProjMatrix)
		{

			// 这些代码下面要处理通用的投影参数， glFrustum和glOrtho相似
			// 除了除法不手动优化， 所以这些代码多数自我解释
			//宽度w的倒数
			Real inv_w = 1 / (right - left);
			//高度h的倒数
			Real inv_h = 1 / (top - bottom);
			//深度d的倒数
			Real inv_d = 1 / (mFarDist - mNearDist);

			// 如果视锥体参数改变，重新计算
			if (mProjType == PT_PERSPECTIVE)  //透视投影
			{
				// 计算矩阵元素
				Real A = 2 * mNearDist * inv_w;
				Real B = 2 * mNearDist * inv_h;
				Real C = (right + left) * inv_w;
				Real D = (top + bottom) * inv_h;
				Real q, qn;
				if (mFarDist == 0)
				{
					// 无限远平面
					q = Frustum::INFINITE_FAR_PLANE_ADJUST - 1;
					qn = mNearDist * (Frustum::INFINITE_FAR_PLANE_ADJUST - 2);
				}
				else
				{
					q = -(mFarDist + mNearDist) * inv_d;
					qn = -2 * (mFarDist * mNearDist) * inv_d;
				}

				// NB: 这创建'uniform'的透视投影矩阵，深度范围[-1,1]，右手规则
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

				if (mObliqueDepthProjection)  //深度投影
				{
					// 变换这个平面的观察空间

					// 如果通过相机和返回一个剔除视锥体观察矩阵重载，不能使用getViewMatrix
					updateView();
					Plane plane = mViewMatrix * mObliqueProjPlane;

					//计算剪切空间对于剪裁平面的角点
					// 如 (sgn(clipPlane.x), sgn(clipPlane.y), 1, 1) 并且通过乘投影矩阵的逆矩阵变换它到相机空间


					/* 通用版本
					Vector4 q = matrix.inverse() *
					Vector4(Math::Sign(plane.normal.x),
					Math::Sign(plane.normal.y), 1.0f, 1.0f);
					*/
					Vector4 qVec;
					qVec.x = (Math::Sign(plane.normal.x) + mProjMatrix[0][2]) / mProjMatrix[0][0];
					qVec.y = (Math::Sign(plane.normal.y) + mProjMatrix[1][2]) / mProjMatrix[1][1];
					qVec.z = -1;
					qVec.w = (1 + mProjMatrix[2][2]) / mProjMatrix[2][3];

					// 计算缩放平面向量
					Vector4 clipPlane4d(plane.normal.x, plane.normal.y, plane.normal.z, plane.d);
					Vector4 c = clipPlane4d * (2 / (clipPlane4d.dotProduct(qVec)));

					// 替换投影矩阵的第三行
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
					// 不能出来无限远平面，要避免除0
					q = -Frustum::INFINITE_FAR_PLANE_ADJUST / mNearDist;
					qn = -Frustum::INFINITE_FAR_PLANE_ADJUST - 1;
				}
				else
				{
					q = -2 * inv_d;
					qn = -(mFarDist + mNearDist)  * inv_d;
				}

				// NB: 创建'uniform'正交投影矩阵，深度范围[-1,1]， 右手规范
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
		// 处理朝向模式
		mProjMatrix = mProjMatrix * Quaternion(Degree(mOrientationMode * 90.f), Vector3::UNIT_Z);
#endif

		//RenderSystem* renderSystem = Root::getSingleton().getRenderSystem();
		//指定API
		//renderSystem->_convertProjectionMatrix(mProjMatrix, mProjMatrixRS);
		//指定GPU编程的API模式
		//renderSystem->_convertProjectionMatrix(mProjMatrix, mProjMatrixRSDepth, true);


		// 计算AABB碰撞盒子（本地）
		// 盒子是从0到-z， 最大范围到远裁平面
		// 视锥体最大值
		Real farDist = (mFarDist == 0) ? 100000 : mFarDist;
		// 近剪裁平面的边界
		Vector3 min(left, bottom, -farDist);
		Vector3 max(right, top, 0);

		if (mCustomProjMatrix)
		{
			//有些自定义的投影矩阵能够用不常用的相反设置
			// 所以确定AABB是开始就正确的设置
			Vector3 tmp = min;
			min.makeFloor(max);
			max.makeCeil(tmp);
		}

		if (mProjType == PT_PERSPECTIVE)
		{
			// 合并远裁平面边界
			Real radio = farDist / mNearDist;
			min.makeFloor(Vector3(left * radio, bottom * radio, -farDist));
			max.makeCeil(Vector3(right * radio, top * radio, 0));
		}
		mBoundingBox.setExtents(min, max);

		mRecalcFrustum = false;

		//标记视锥体剪裁平面更新
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

	//更新顶点数据
	void Frustum::updateVertexData(void) const
	{
		/*
		if (mRecalcVertexData)
		{
		if (mVertexData.vertexBufferBinding->getBufferCount() <= 0)
		{
		// 初始化顶点&索引数据
		mVertexData.vertexDeclaration->addElement(0, 0, VET_FLOAT3, VES_POSITION);
		mVertexData.vertexCount = 32;
		mVertexData.vertexStart = 0;
		mVertexData.vertexBufferBinding->setBinding(0,
		HardwareBufferManager::getSingleton().createVertexBuffer(
		sizeof(float) * 3, 32, HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY));
		}

		// Note: 尽管我们在这里能够用通用投影矩阵处理，但是由于它与无限远平面不兼容，所以我们仍然要用投影参数处理

		// 计算近剪裁平面的四角
		Real vpLeft, vpRight, vpBottom, vpTop;
		calcProjectionParameters(vpLeft, vpRight, vpBottom, vpTop);

		// 对无限远距离进行限制，0为无限远
		Real farDist = (mFarDist == 0) ? 100000 : mFarDist;

		// 计算平面的角
		Real radio = mProjType == PT_PERSPECTIVE ? farDist / mNearDist : 1;
		Real farLeft = vpLeft * radio;
		Real farRight = vpRight * radio;
		Real farBottom = vpBottom * radio;
		Real farTop = vpTop * radio;

		// 计算顶点位置(局部)
		// 0 是原点
		// 1, 2, 3, 4 是近剪裁平面的左上角开始，顺时针方向
		// 5, 6, 7, 8 是远裁平面，左上角开始，顺时针方向
		HardwareVertexBufferSharedPtr vbuf = mVertexData.vertexBufferBinding->getBuffer(0);
		float* pFloat = static_cast<float*>(vbuf->lock(HardwareBuffer::HBL_DISCARD));

		// 近剪裁平面 (记得视锥体是-z 负方向， 右手坐标系)
		*pFloat++ = vpLeft;  *pFloat++ = vpTop;    *pFloat++ = -mNearDist;
		*pFloat++ = vpRight; *pFloat++ = vpTop;    *pFloat++ = -mNearDist;

		*pFloat++ = vpRight; *pFloat++ = vpTop;    *pFloat++ = -mNearDist;
		*pFloat++ = vpRight; *pFloat++ = vpBottom; *pFloat++ = -mNearDist;

		*pFloat++ = vpRight; *pFloat++ = vpBottom; *pFloat++ = -mNearDist;
		*pFloat++ = vpLeft;  *pFloat++ = vpBottom; *pFloat++ = -mNearDist;

		*pFloat++ = vpLeft;  *pFloat++ = vpBottom; *pFloat++ = -mNearDist;
		*pFloat++ = vpLeft;  *pFloat++ = vpTop;    *pFloat++ = -mNearDist;

		//远剪裁平面
		*pFloat++ = farLeft;  *pFloat++ = farTop;    *pFloat++ = -farDist;
		*pFloat++ = farRight; *pFloat++ = farTop;    *pFloat++ = -farDist;

		*pFloat++ = farRight; *pFloat++ = farTop;    *pFloat++ = -farDist;
		*pFloat++ = farRight; *pFloat++ = farBottom; *pFloat++ = -farDist;

		*pFloat++ = farRight; *pFloat++ = farBottom; *pFloat++ = -farDist;
		*pFloat++ = farLeft;  *pFloat++ = farBottom; *pFloat++ = -farDist;

		*pFloat++ = farLeft;  *pFloat++ = farBottom; *pFloat++ = -farDist;
		*pFloat++ = farLeft;  *pFloat++ = farTop;    *pFloat++ = -farDist;

		// 金字塔的边
		*pFloat++ = 0.0f;    *pFloat++ = 0.0f;   *pFloat++ = 0.0f;
		*pFloat++ = vpLeft;  *pFloat++ = vpTop;  *pFloat++ = -mNearDist;

		*pFloat++ = 0.0f;    *pFloat++ = 0.0f;   *pFloat++ = 0.0f;
		*pFloat++ = vpRight; *pFloat++ = vpTop;    *pFloat++ = -mNearDist;

		*pFloat++ = 0.0f;    *pFloat++ = 0.0f;   *pFloat++ = 0.0f;
		*pFloat++ = vpRight; *pFloat++ = vpBottom; *pFloat++ = -mNearDist;

		*pFloat++ = 0.0f;    *pFloat++ = 0.0f;   *pFloat++ = 0.0f;
		*pFloat++ = vpLeft;  *pFloat++ = vpBottom; *pFloat++ = -mNearDist;

		// 盒子的边

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
		// 关联到节点
		if (mParentNode)
		{
		if (mRecalcView ||
		mParentNode->_getDerivedOrientation() != mLastParentOrientation ||
		mParentNode->_getDerivedPosition() != mLastParentPosition)
		{
		//我们要关联的顶点场景节点时间过期了
		mLastParentOrientation = mParentNode->_getDerivedOrientation();
		mLastParentPosition = mParentNode->_getDerivedPosition();
		mRecalcView = true;
		}
		}
		// 从连接平面得到反射
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
		// 更新观察矩阵
		// ----------------------

		// 获取四元数朝向

		if (!mCustomViewMatrix)
		{
			Matrix3 rot;
			const Quaternion& orientation = getOrientationForViewUpdate();
			const Vector3& position = getPositionForViewUpdate();

			mViewMatrix = Math::makeViewMatrix(position, orientation, mReflect ? &mReflectMatrix : 0);
		}

		mRecalcView = false;

		// 标记更新视锥体剪裁平面
		mRecalcFrustumPlanes = true;
		// 标记更新世界空间角
		mRecalcWorldSpaceCorners = true;
		// 标记如果深度斜投影面打开更新视锥体,因为该平面需要在观察空间中
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
		// 更新视锥体平面
		// -------------------------
		Matrix4 combo = mProjMatrix * mViewMatrix;   //MVP矩阵

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

		// 当不上单位长度时，重新标准化
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
		Matrix4 eyeToWorld = mViewMatrix.inverseAffine(); //将观察空间转到世界空间的矩阵

		// Note: 尽管我们在这里能够用通用投影矩阵处理，但是由于它与无限远平面不兼容，所以我们仍然要用投影参数处理  still need to working with projection parameters.

		// 计算近剪裁平面的角
		Real nearLeft, nearRight, nearBottom, nearTop;
		calcProjectionParameters(nearLeft, nearRight, nearBottom, nearTop);

		// 最远剪裁平面距离
		Real farDist = (mFarDist == 0) ? 100000 : mFarDist;

		// 计算平面四角
		Real radio = mProjType == PT_PERSPECTIVE ? farDist / mNearDist : 1;
		Real farLeft = nearLeft * radio;
		Real farRight = nearRight * radio;
		Real farBottom = nearBottom * radio;
		Real farTop = nearTop * radio;

		// 近剪裁平面
		mWorldSpaceCorners[0] = eyeToWorld.transformAffine(Vector3(nearRight, nearTop, -mNearDist));
		mWorldSpaceCorners[1] = eyeToWorld.transformAffine(Vector3(nearLeft, nearTop, -mNearDist));
		mWorldSpaceCorners[2] = eyeToWorld.transformAffine(Vector3(nearLeft, nearBottom, -mNearDist));
		mWorldSpaceCorners[3] = eyeToWorld.transformAffine(Vector3(nearRight, nearBottom, -mNearDist));
		// 远剪裁平面
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
	// 确定边界盒子要更新
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
		// 变换光源位置到相机空间

		updateView();
		Vector3 eyeSpacePos = mViewMatrix.transformAffine(sphere.getCenter()); //将球中心坐标转到观察空间中

		// 初始化
		*left = *bottom = -1.0f;
		*right = *top = 1.0f;

		if (eyeSpacePos.z < 0)
		{
			updateFrustum();
			const Matrix4& projMatrix = getProjectionMatrix();
			Real r = sphere.getRadius();
			Real rsq = r * r;  //半径的平方


			if (eyeSpacePos.squaredLength() <= rsq)
				return false;

			Real Lxz = Math::Sqr(eyeSpacePos.x) + Math::Sqr(eyeSpacePos.z);  //设原点O,球心P， 向量Vop在XZ平面上的投影
			Real Lyz = Math::Sqr(eyeSpacePos.y) + Math::Sqr(eyeSpacePos.z);  //设原点O,球心P， 向量Vop在YZ平面上的投影

			// 用点法式 找出球体切平面
			// 首先XZ 
			// 计算二次根判别式: b*b - 4ac
			// x = Nx
			// a = Lx^2 + Lz^2
			// b = -2rLx
			// c = r^2 - Lz^2
			Real a = Lxz;
			Real b = -2.0f * r * eyeSpacePos.x;
			Real c = rsq - Math::Sqr(eyeSpacePos.z);
			Real D = b*b - 4.0f*a*c;

			// 两个根
			if (D > 0)
			{
				Real sqrootD = Math::Sqrt(D);
				// 解除二次方获取法线的分量
				Real Nx0 = (-b + sqrootD) / (2 * a);
				Real Nx1 = (-b - sqrootD) / (2 * a);

				// 取得Z
				Real Nz0 = (r - Nx0 * eyeSpacePos.x) / eyeSpacePos.z;
				Real Nz1 = (r - Nx1 * eyeSpacePos.x) / eyeSpacePos.z;

				// 获取切点
				// 只考虑相机前面的切点
				Real Pz0 = (Lxz - rsq) / (eyeSpacePos.z - ((Nz0 / Nx0) * eyeSpacePos.x));
				if (Pz0 < 0)
				{
					// 投影在近剪裁平面在世界空间的点
					Real nearx0 = (Nz0 * mNearDist) / Nx0;
					// 现在我们需要映射它到视口坐标
					// 用投影矩阵，并考虑所有的因素
					Vector3 relx0 = projMatrix * Vector3(nearx0, 0, -mNearDist);

					//找出这是左边还是右边
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
					//投影在近剪裁平面在世界空间的点
					Real nearx1 = (Nz1 * mNearDist) / Nx1;
					// 现在我们需要映射它到视口坐标
					// 用投影矩阵，并考虑所有的因素
					Vector3 relx1 = projMatrix * Vector3(nearx1, 0, -mNearDist);

					//找出这是左边还是右边
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


			// 现在是 YZ 平面
			// 计算二次方根判别式: b*b - 4ac
			// x = Ny
			// a = Ly^2 + Lz^2
			// b = -2rLy
			// c = r^2 - Lz^2
			a = Lyz;
			b = -2.0f * r * eyeSpacePos.y;
			c = rsq - Math::Sqr(eyeSpacePos.z);
			D = b*b - 4.0f*a*c;

			//两个根
			if (D > 0)
			{
				Real sqrootD = Math::Sqrt(D);
				// 解除二次根获得法线的分量
				Real Ny0 = (-b + sqrootD) / (2 * a);
				Real Ny1 = (-b - sqrootD) / (2 * a);

				// 从这里取得Z
				Real Nz0 = (r - Ny0 * eyeSpacePos.y) / eyeSpacePos.z;
				Real Nz1 = (r - Ny1 * eyeSpacePos.y) / eyeSpacePos.z;

				// 获取切点
				// 只考虑相机前面的切点
				Real Pz0 = (Lyz - rsq) / (eyeSpacePos.z - ((Nz0 / Ny0) * eyeSpacePos.y));
				if (Pz0 < 0)
				{
					//投影在近剪裁平面在世界空间的点
					Real neary0 = (Nz0 * mNearDist) / Ny0;
					// 现在我们需要映射它到视口坐标
					// 用投影矩阵，并考虑所有的因素
					Vector3 rely0 = projMatrix * Vector3(0, neary0, -mNearDist);

					//找出这是左边还是右边
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
					//投影在近剪裁平面在世界空间的点
					Real neary1 = (Nz1 * mNearDist) / Ny1;
					// 现在我们需要映射它到视口坐标
					// 用投影矩阵，并考虑所有的因素
					Vector3 rely1 = projMatrix * Vector3(0, neary1, -mNearDist);

					//找出这是左边还是右边
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

