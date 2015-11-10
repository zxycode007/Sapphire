#ifndef _SAPPHIRE_FRUSTUM_
#define _SAPPHIRE_FRUSTUM_

#include "SapphirePrerequisites.h"
#include "SapphireMath.h"
#include "SapphireVector2.h"
#include "SapphirePlane.h"
#include "SapphireMatrix4.h"
#include "SapphireString.h"
#include "SapphireAxisAlignedBox.h"

namespace Sapphire
{
	/**
	ָ������ģʽ
	*/
	enum OrientationMode
	{
		OR_DEGREE_0 = 0,
		OR_DEGREE_90 = 1,
		OR_DEGREE_180 = 2,
		OR_DEGREE_270 = 3,

		OR_PORTRAIT = OR_DEGREE_0,
		OR_LANDSCAPERIGHT = OR_DEGREE_90,
		OR_LANDSCAPELEFT = OR_DEGREE_270
	};

	/**
	ָ��͸��ͶӰ������ͶӰģʽ
	*/
	enum ProjectionType
	{
		PT_ORTHOGRAPHIC,
		PT_PERSPECTIVE
	};

	/** ����ռ���׶�����ƽ��
	*/
	enum FrustumPlane
	{
		FRUSTUM_PLANE_NEAR = 0,
		FRUSTUM_PLANE_FAR = 1,
		FRUSTUM_PLANE_LEFT = 2,
		FRUSTUM_PLANE_RIGHT = 3,
		FRUSTUM_PLANE_TOP = 4,
		FRUSTUM_PLANE_BOTTOM = 5
	};

	/**
	 ��׶�壬 ��ʾ���������ͶӰ��Χ
	 */
	class _SapphireExport Frustum
	{
	protected:
		/// ��������ͶӰ
		ProjectionType mProjType;

		/// y-����Ŀ�������Ĭ��45�ȣ�
		Radian mFOVy;
		/// Զ���þ��� Ĭ��10000
		Real mFarDist;
		/// �����þ��� Ĭ��100
		Real mNearDist;
		/// x/y �ӿں��ݱ� Ĭ��1.3333
		Real mAspect;
		/// �����߶ȴ�С�����絥λ��
		Real mOrthoHeight;
		/// ��׵������ƫ��
		Vector2 mFrustumOffset;
		/// ��׶�役����루����������Ⱦ, Ĭ��1.0��
		Real mFocalLength;

		/// 6������ƽ��
		mutable Plane mFrustumPlanes[6];

		/// ������һ���汾�����λ��
		mutable Quaternion mLastParentOrientation;
		mutable Vector3 mLastParentPosition;

		/// Ԥ�ȼ�����ض���Ⱦϵͳ��ͶӰ����
		mutable Matrix4 mProjMatrixRS;
		/// Ԥ�ȼ���������ȷ�Χ��Ⱦϵͳ�ı�׼ͶӰ����
		mutable Matrix4 mProjMatrixRSDepth;
		/// Ԥ�ȼ���ı�׼��ͶӰ����
		mutable Matrix4 mProjMatrix;
		/// Ԥ�ȼ���Ĺ۲����
		mutable Matrix4 mViewMatrix;

		/// ��׶����״�ı��Ƿ����¼���
		mutable bool mRecalcFrustum;
		/// ����۲�λ�øı��Ƿ����¼���
		mutable bool mRecalcView;
		/// ��׶��ƽ��ı��Ƿ����¼���
		mutable bool mRecalcFrustumPlanes;
		/// ����ռ����ı��Ƿ����¼���
		mutable bool mRecalcWorldSpaceCorners;
		/// ���������ݸı�ʱ�ؼ��㶥������
		mutable bool mRecalcVertexData;
		/// �Ƿ��Զ���۲����
		bool mCustomViewMatrix;
		/// �Ƿ��Զ���ͶӰ����
		bool mCustomProjMatrix;
		/// �Ƿ��ֶ���չ��׶������
		bool mFrustumExtentsManuallySet;
		/// ��׶����չ
		mutable Real mLeft, mRight, mTop, mBottom;
		/// ��׶�巽��ģʽ
		mutable OrientationMode mOrientationMode;


		// ���Լ�����ڲ�����
		// ͨ���������¼���ͶӰ����
		virtual void calcProjectionParameters(Real& left, Real& right, Real& bottom, Real& top) const;
		/// ����������ڸ�����׶�� 
		virtual void updateFrustum(void) const;
		/// ����������ڸ��¹۲�
		virtual void updateView(void) const;
		/// ʵ���˸�����׶�� (���������ڵ���)
		virtual void updateFrustumImpl(void) const;
		/// ʵ���˸�����׶�� (���������ڵ���)
		virtual void updateViewImpl(void) const;
		virtual void updateFrustumPlanes(void) const;
		/// ������׶��ƽ���ʵ��
		virtual void updateFrustumPlanesImpl(void) const;
		virtual void updateWorldSpaceCorners(void) const;
		virtual void updateWorldSpaceCornersImpl(void) const;
		virtual void updateVertexData(void) const;
		virtual bool isViewOutOfDate(void) const;
		virtual bool isFrustumOutOfDate(void) const;
		/// ���Ѹ�����׶����Ϣ
		virtual void invalidateFrustum(void) const;
		/// ���Ѹ��¹۲���Ϣ
		virtual void invalidateView(void) const;


		/// �Կ��ƶ����͹�����ȼ���
		//static String msMovableType;

		mutable AxisAlignedBox mBoundingBox;
		//mutable VertexData mVertexData;

		//MaterialPtr mMaterial;
		//����ռ�Ľ�������
		mutable Vector3 mWorldSpaceCorners[8];

		/// �����׶�弤����Ϊ��������
		bool mReflect;
		/// �����ķ������
		mutable Matrix4 mReflectMatrix;
		/// �̶��ķ���ƽ��
		mutable Plane mReflectPlane;
		/// ָ��һ������ƽ�棨�Զ����£�
		//const MovablePlane* mLinkedReflectPlane;
		/// ��¼��һ��ʹ�õ�����ռ䷴��ƽ����Ϣ
		mutable Plane mLastLinkedReflectionPlane;

		/// �����׶��ʹ��б���ͶӰ
		bool mObliqueDepthProjection;
		/// �̶�б���ͶӰƽ��
		mutable Plane mObliqueProjPlane;

		//const MovablePlane* mLinkedObliqueProjPlane;
		//mutable Plane mLastLinkedObliqueProjPlane;

	public:

		Frustum(const String& name = StringUtil::BLANK);
		virtual ~Frustum();


		/**
		������׶���Yά������
		@remarks
		����������׶��λ�ô��ĽǶȣ�������ͶӰ����Ļ�ı߽��С� ��ֵ��90�����ϣ��ǿ�Ƕȣ� ��������Ұ�� �ͽǶȣ�30�����ڣ��������죬��Զ����Ұ��
		����ֵ��45��60��֮��
		@par
		���ֵ��ʾ��ֱ����ˮƽ����ļ��������ӿڵľ���
		@note
		����FOV����ԭʼֵ���ṩ��frustum::setNearClipPlane.
		*/
		virtual void setFOVy(const Radian& fovy);

		/**
		������׶��Y-ά����FOV
		*/
		virtual const Radian& getFOVy(void) const;

		/** ���ý�����ƽ��ľ���
		@remarks
		������ƽ���λ��������׶��λ�õ�ͶӰ���絽��Ļ�����롣�������ƽ����룬�������ͺ��ݱȣ������˹۲�������ӿڵĴ�С��
		����ʾΪһ���ĵ�����������׶���ӿ�Ӧ�����ź���Ļ�ӿ�ͨ�õĺ��ݱȣ��Ա�����Ⱦ�������
		@param
		nearDist  ��������Ǵ�����������ϵ�е���׶�嵽����ƽ��ľ���
		*/
		virtual void setNearClipDistance(Real nearDist);

		/**
		ȡ�ý���ƽ��ľ���
		*/
		virtual Real getNearClipDistance(void) const;

		/** ����Զ��ƽ��ľ���
		@remarks
		��׶������׶��λ�ú��ӿڱ߽���ɵ�һ����������
		����������ý�������Զ�˵ľ��롣��ͬ��Ӧ�ó�����Ҫ��ͬ��ֵ������
		һ������ģ����Ҫ�ȵ�һ�˳����Զ�ķǳ�����Զ����ƽ�档�ص��ǽ�����ƽ���Զ����ƽ��֮��ı���Խ��������ʾ������ȵ�z-buffer�ľ��Ⱦ�Խ�͡�
		��������Z-buffer��Z��Χ�������ˣ�16����32λ���������ֵ�ض�ȱ�ݵķֲ���������ƽ�浽Զ����ƽ��֮�䡣
		�����������Щ�������ͨ���޸���������������Զ������Ӱ�쾫�Ⱥܶ࣬�μ���һ�㡣
		@param
		farDist  ��׶������������ϵ��λ�õ���Զ����ƽ��ľ��룬���������Ϊ0����ζ������Զ�ľ��룬��ͶӰ��Ӱʱ�ر����ã�����ҪС�Ĳ�Ҫ�ý���ƽ����ڽӽ�
		*/
		virtual void setFarClipDistance(Real farDist);

		/** ������׶���Զ����ƽ��ľ���
		*/
		virtual Real getFarClipDistance(void) const;

		/**
		������׶����ݱ�
		@par
		Ĭ��ֵ��1.3333, ���ڶ���������˵��
		*/
		virtual void setAspectRatio(Real ratio);

		/**
		������׶����ݱ�
		*/
		virtual Real getAspectRatio(void) const;

		/**
		������׶��ƫ����������������Ⱦ
		@remarks
		�����������ֻ�۾��Ĵ�ֱ��ˮƽƽ��ƫ��ֵ��
		��������Ⱦ��׶���ƶ�ˮƽƽ�档������Ⱦ��ֻ�۾�������Ҫ�����������Ⱦ������ȾĿ��
		@par
		����������ϵ����׶��ƫ��ֵ��Ĭ��(0,0), û��ƫ����
		@param
		offset ˮƽ�ʹ�ֱƽ���ƫ��ֵ
		*/
		virtual void setFrustumOffset(const Vector2& offset);

		/**
		������׶��ƫ����������������Ⱦ
		@remarks
		�����������ֻ�۾��Ĵ�ֱ��ˮƽƽ��ƫ��ֵ��
		��������Ⱦ��׶���ƶ�ˮƽƽ�档������Ⱦ��ֻ�۾�������Ҫ�����������Ⱦ������ȾĿ��
		@par
		����������ϵ����׶��ƫ��ֵ��Ĭ��(0,0), û��ƫ����
		@param
		��ֱƫ����
		@param
		ˮƽƫ����
		*/
		virtual void setFrustumOffset(Real horizontal = 0.0, Real vertical = 0.0);

		/**
		������׶��ƫ����
		*/
		virtual const Vector2& getFrustumOffset() const;

		/**
		������׶�役�ࣨ����������Ⱦ��
		@param
		focalLength ����׶����������굽��ƽ��ľ���
		*/
		virtual void setFocalLength(Real focalLength = 1.0);

		/** ���ؽ�ƽ�����
		*/
		virtual Real getFocalLength() const;


		/**
		�ֶ�������׶�����չ
		@param �������µ�λ�ã��ڹ۲�ռ��н�����ƽ����߼���ƽ���ཻ��λ��
		*/
		virtual void setFrustumExtents(Real left, Real right, Real top, Real bottom);
		/** ��λ��׶����չ�����������Զ��̳�*/
		virtual void resetFrustumExtents();
		/** ȡ����׶���ڹ۲�ռ����չ */
		virtual void getFrustumExtents(Real& outleft, Real& outright, Real& outtop, Real& outbottom) const;

		/**
		ȡ�������׶����ڵ�ǰ��Ⱦϵͳ��ǰ����ָ���ĵ�ͶӰ����DX,OGL,OGLESҲ�����ֻ����֣���ȷ�Χ�����в�ͬ��
		@remarks
		��������������ͶӰ����������ȾAPI�汾���������Ҫһ�����͵�ͶӰ������getProjectionMatrix

		*/
		virtual const Matrix4& getProjectionMatrixRS(void) const;

		/**
		ȡ�ö��ڵ�ǰ��ȾϵͳУ������ȵ�ͶӰ���󣬵�����ֻ��ѭ���ֹ���
		@remarks
		����������ȾAPI��getProjectionMatrix��Щ��ͬ
		��������Ȼ������������ͶӰ���󣬶�����ʹ�ú�����ȾAPI--��Vertex��Fragment Shader����Ϊ��������Ҫ����
		�����������⵼����ȷ�Χ����D3D[0,1]��GL[-1,1]������ͬ����Ⱦϵͳ���ǲ�ͬ������������Χ���뱣֤�ڿɱ�̻�̶���Ⱦ����������ͬ��
		*/
		virtual const Matrix4& getProjectionMatrixWithRSDepth(void) const;
		/**
		ȡ����׶��ı�׼ͶӰ�������ͶӰ��������ѭ����ͶӰ�����������ȷ�Χ[-1.+1]
		@remarks
		�������ȾAPI������getProjectionMatrixRS��ͬ�������ʼ�շ�����������淶ͶӰ�������ȷ�Χ[-1,+1],������ʹ��ʲô������ȾAPI
		*/
		virtual const Matrix4& getProjectionMatrix(void) const;

		/**ȡ����׶��Ĺ۲���� ��Ҫ��Ϊ�ڲ�ʹ��
		*/
		virtual const Matrix4& getViewMatrix(void) const;

		/** ���������׶���һ���۲���� ������һ�����ܵĶ�̬�㣬
		����Ҫ�ǵ��������ڶ�����׶�����Ⱦ���������ڲ����á�
		*/
		virtual void calcViewMatrixRelative(const Vector3& relPos, Matrix4& matToUpdate) const;


		/**
		�����Ƿ�������׶��ʹ��һ���Զ���Ĺ۲����
		@remarks
		����һ���߼����������������ֶ�����һ��Ϊ�����׶��۲���� �����Ǹ������Լ���λ�úͳ��������м���
		@note
		��һ���Զ���۲����֮�������׶�彫������������������Լ���λ�úͳ�������Ҫ��֤����۲��������
		����Զ�����󽫻���getViewMatrix���ء�
		@param enable ���Ϊtrue������Զ���۲�����ɵڶ����������룬�⽫�������������Զ����㡣���Ϊfalse�������׶�彫�ָ��Զ�����۲����
		@param viewMatrix Ҫ�õ��Զ���۲���������������Ƿ������
		@see ��Frustum::setCustomProjectionMatrix, Matrix4::isAffine
		*/
		virtual void setCustomViewMatrix(bool enable,
			const Matrix4& viewMatrix = Matrix4::IDENTITY);
		/// �����Ƿ���һ���Զ���Ĺ۲������ʹ��
		virtual bool isCustomViewMatrixEnabled(void) const
		{
			return mCustomViewMatrix;
		}
		/** 
		�����Ƿ�������׶��ʹ��һ���Զ����ͶӰ���� 
		@remarks
		����һ���߼����������������ֶ�����һ��Ϊ�����׶��ͶӰ���� �����Ǹ������Լ���λ�úͳ��������м���
		@note
		��һ���Զ���۲����֮�������׶�彫������������������Լ���λ�úͳ�������Ҫ��֤���ͶӰ��������
		����Զ�����󽫻���getProjectionMatrix�����������ĺ������ء�
		@param enable
		���Ϊtrue������Զ���ͶӰ�����ɵڶ����������룬�⽫�������������Զ����㡣���Ϊfalse�������׶�彫�ָ��Զ�����ͶӰ����
		@param projectionMatrix �Զ����ͶӰ����
		@see ��Frustum::setCustomViewMatrix
		*/
		virtual void setCustomProjectionMatrix(bool enable,
			const Matrix4& projectionMatrix = Matrix4::IDENTITY);
		/// �����Ƿ�ʹ��һ���Զ����ͶӰ����
		virtual bool isCustomProjectionMatrixEnabled(void) const
		{
			return mCustomProjMatrix;
		}

		/**
		���������׶��ļ���ƽ�棨����ռ䣩
		@remarks
		�������ƽ���ǰ�ö�ٳ���FrustumPlane˳��
		@return ������׶�����ƽ���б�
		*/
		virtual const Plane* getFrustumPlanes(void) const;

		/** ���������׶��ָ��ƽ�棨����ռ䣩
		@remarks
		��ȡ���������׶��ƽ���е�һ������
		@return
		����ָ����׶��ƽ��
		*/
		virtual const Plane& getFrustumPlane(unsigned short plane) const;

		/**
		���Ը�����AABB��������׶�����Ƿ�ɼ�
		@param
		bound Ҫ������AABB�ĺ��ӣ�����ռ䣩
		@param
		culledBy ��ѡ��ָ�룬������ؽ��Ϊfalse��ָ��һ�������޳�����ƽ������
		@return
		����������Ϊ�ɼ���������true
		@par
		����֮�⣬����false
		*/
		virtual bool isVisible(const AxisAlignedBox& bound, FrustumPlane* culledBy = 0) const;

		/**
		���Ը�������ײ����������׶���Ƿ�ɼ�
		@param
		bound ��ײ���Ƿ��޳� ������ռ䣩
		culledBy ��ѡ��ָ�룬������ؽ��Ϊfalse��ָ��һ�������޳�����ƽ������
		@return
		������ǿɼ��ģ�����true
		@par
		����֮�ⷵ��false
		*/
		virtual bool isVisible(const Sphere& bound, FrustumPlane* culledBy = 0) const;

		/** ���Ը����Ķ����Ƿ�����׶���пɼ�
		@param
		vert Ҫ�����õĲ��ԵĶ��㣨����ռ䣩
		@param
		culledBy ��ѡ��ָ�룬������ؽ��Ϊfalse��ָ��һ�������޳�����ƽ������
		@return
		����ɼ�������true
		@par
		���ⷵ��false
		*/
		virtual bool isVisible(const Vector3& vert, FrustumPlane* culledBy = 0) const;

		/// ����MovableObject::getTypeFlags
		uint32 getTypeFlags(void) const;


		/** ���� MovableObject */
		//const AxisAlignedBox& getBoundingBox(void) const;

		/** ���� MovableObject */
		//Real getBoundingRadius(void) const;

		/** ���� MovableObject */
		//void _updateRenderQueue(RenderQueue* queue);

		/** ���� MovableObject */
		//const String& getMovableType(void) const;

		/** ���� MovableObject */
		//void _notifyCurrentCamera(Camera* cam);

		/** ���� Renderable */
		//const MaterialPtr& getMaterial(void) const;

		/** ���� Renderable */
		//void getRenderOperation(RenderOperation& op);

		/** ���� Renderable */
		//void getWorldTransforms(Matrix4* xform) const;

		/** ���� Renderable */
		//Real getSquaredViewDepth(const Camera* cam) const;

		/** ���� Renderable */
		//const LightList& getLights(void) const;


		/**
		��ȡ��׶��ռ�ĵĽ���������ռ��
		@remarks
		��Щ�ǰ�����˳�����У� top-right near,
		top-left near, bottom-left near, bottom-right near,
		top-right far, top-left far, bottom-left far, bottom-right far.
		*/
		virtual const Vector3* getWorldSpaceCorners(void) const;

		/**
		����Ҫʹ�õ�ͶӰ������(������͸��  Ĭ����͸��)
		*/
		virtual void setProjectionType(ProjectionType pt);

		/**
		����ʹ�õ�ͶӰ����
		*/
		virtual ProjectionType getProjectionType(void) const;

		/**
		���������������ã� ֻ��������ͶӰ��Ⱦ������
		@note ����������������¼�����ݱȣ������Ҫ������ݱ���setOrthoWindowHeight ��setOrthoWindowHeight��������������һ������һ����������һ���ر�Ĵ�С��
		@param w, h �۲촰�������絥Ԫ��Ĵ�С
		*/
		virtual void setOrthoWindow(Real w, Real h);
		/**
		�����������ڸߣ� ֻ��������ͶӰ��Ⱦ������
		@note ��͵������¼��㴰�ں��ݱ�
		@param h �ǹ۲촰�������絥Ԫ��Ĵ�С
		*/
		virtual void setOrthoWindowHeight(Real h);
		/**
		�����������ڿ�ȣ� ֻ��������ͶӰ��Ⱦ������
		@note ��͵������¼��㴰�ں��ݱ�
		@param w  �ǹ۲촰�������絥Ԫ��Ĵ�С
		*/
		virtual void setOrthoWindowWidth(Real w);
		/**
		��ȡ�������ڸ߶ȣ�ֻ��������Ⱦ
		*/
		virtual Real getOrthoWindowHeight() const;
		/**
		��ȡ�������ڿ�ȣ�ֻ��������Ⱦ
		*/
		virtual Real getOrthoWindowWidth() const;


		/** �޸������׶�����������ܹ������Լ�ͨ��ָ������Ļ�ķ�������Ⱦ
		@remarks
		���plannar ����ǳ�����
		*/
		//virtual void enableReflection(const Plane& p);
		/**
		 �޸������׶�����������ܹ������Լ�ͨ��ָ������Ļ�ķ�������Ⱦ
		 ע�⣺��������İ汾���ӵ�һ��ƽ�������ڸı����Զ���ʰȡ��
		 �����������׶��֮ǰ�������ƽ��
		 @remarks
		 ���plannar ����ǳ�����
		 */
		//virtual void enableReflection(const MovablePlane* p);

		/// ���������׶���Ƿ񱻷���
		virtual bool isReflected(void) const { return mReflect; }
		/// ������ʷ��������׶�����ķ������
		virtual const Matrix4& getReflectionMatrix(void) const { return mReflectMatrix; }
		/// ��κ��ʷ����������ƽ��
		virtual const Plane& getReflectionPlane(void) const { return mReflectPlane; }

		/**
		ͶӰһ�����嵽������ƽ�棬����ȡ�߽����
		@param sphere ҪͶӰ��������ռ�����
		@param radius ����뾶
		@param left, top, right, bottom ָ��Ŀ���ֵ����Щ���ɱ�׼���豸�������(-1,1)
		@return �������ͶӰ��������ƽ����Ӽ�����true���������������ƽ�汻�����򷵻�false
		*/
		virtual bool projectSphere(const Sphere& sphere,
			Real* left, Real* top, Real* right, Real* bottom) const;

		/**
		���������׶�嵽һ���Զ���Ľ�����ƽ�棬���ܹ���������һ���Զ���ķ�ʽ�������û�����ƽ�������ü�����
		@remarks
		�м���Ӧ�ó������ͨ��һ��ƽ������Ĳü�һ�����������ձ���ǵ���Ⱦһ�����䵽һ������������ֻ��Ҫ��Ⱦ��ˮƽ������Ⱦ�����塣
		��Ȼ���ǿ������û�����ƽ��ʱ������֧�����е��Կ�������û��Ӳ�����١�����ķ����ǣ���һ��α��Ľ�����ƽ�棬������֧�����е�Ӳ����Ӧ�õ�����ļ���ƽ�档
		���ı���׶�����״���ᵽ������Ȼ���ʧȥ���ȣ���ʹ�����ּ������������������
		@par
		����汾�ķ������ӵ�һ��ƽ�棬������һ��ͨ��ֵ�����ƽ�棬 ������ܹ��ı����ƽ�棨�ƶ�/��ת����������Ľڵ㣩
		���������Զ���Ӱ�쵽����
		@note ע���������ֻ�ܹ�����͸��ͶӰ��
		@param plane Ҫ���ӵ�ҪӦ�ü��õ�ƽ�档���ƽ����������������ӵ���ʱ���ڣ�����������׶��֮ǰ������
		*/
		//virtual void enableCustomNearClipPlane(const MovablePlane* plane);

		/**
		���������׶�嵽һ���Զ���Ľ�����ƽ��,���ܹ���������һ���Զ���ķ�ʽ�������û�����ƽ�������ü�����
		@remarks
		�м���Ӧ�ó������ͨ��һ��ƽ������Ĳü�һ�����������ձ���ǵ���Ⱦһ�����䵽һ������������ֻ��Ҫ��Ⱦ��ˮƽ������Ⱦ�����塣
		��Ȼ���ǿ������û�����ƽ��ʱ������֧�����е��Կ�������û��Ӳ�����١�����ķ����ǣ���һ��α��Ľ�����ƽ�棬������֧�����е�Ӳ����Ӧ�õ�����ļ���ƽ�档
		���ı���׶�����״���ᵽ������Ȼ���ʧȥ���ȣ���ʹ�����ּ������������������
		@note ע���������ֻ�ܹ�����͸��ͶӰ��
		@param plane Ҫ���ӵ�ҪӦ�ü��õ�ƽ�档���ƽ����������������ӵ���ʱ���ڣ�����������׶��֮ǰ������
		*/
		//virtual void enableCustomNearClipPlane(const Plane& plane);

		/** �ر������Զ��������ƽ�� */
		//virtual void disableCustomNearClipPlane(void);
		/** �Ƿ�ʹ��һ��������ƽ��*/
		virtual bool isCustomNearClipPlaneEnabled(void) const
		{
			return mObliqueDepthProjection;
		}

		/// @copydoc MovableObject::visitRenderables
		//void visitRenderables(Renderable::Visitor* visitor,
		//	bool debugRenderables = false);

		/**
		@brief  ��������Զ����ƽ��ͶӰ,�Ա�������С����
		*/
		static const Real INFINITE_FAR_PLANE_ADJUST;
		/** ��ȡ�����׶��������λ��  */
		virtual const Vector3& getPositionForViewUpdate(void) const;
		/** ��ȡ�����׶�������ĳ��� */
		virtual const Quaternion& getOrientationForViewUpdate(void) const;

		/**
		��ȡһ������ռ��бպϵ���׶���б�
		*/
		PlaneBoundedVolume getPlaneBoundedVolume();

		/** ������׶�峯���ģʽ��Ĭ��OR_DEGREE_0
		@remarks
		����һ����׶��ĳ������ֻ֧��IOS���ڱ��ƽ̨���׳��쳣
		*/
		void setOrientationMode(OrientationMode orientationMode);

		/** ��ȡ��׶�峯���ģʽ
		@remarks
		��ȡһ����׶��ĳ������ֻ֧��IOS���ڱ��ƽ̨���׳��쳣
		*/
		OrientationMode getOrientationMode() const;

	};
}

#endif