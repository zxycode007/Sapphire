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
	指定方向模式
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
	指定透视投影和正交投影模式
	*/
	enum ProjectionType
	{
		PT_ORTHOGRAPHIC,
		PT_PERSPECTIVE
	};

	/** 世界空间视锥体剪裁平面
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
	 视锥体， 表示可视区域和投影范围
	 */
	class _SapphireExport Frustum
	{
	protected:
		/// 正交还是投影
		ProjectionType mProjType;

		/// y-方向的可视区域（默认45度）
		Radian mFOVy;
		/// 远剪裁距离 默认10000
		Real mFarDist;
		/// 近剪裁距离 默认100
		Real mNearDist;
		/// x/y 视口横纵比 默认1.3333
		Real mAspect;
		/// 正交高度大小（世界单位）
		Real mOrthoHeight;
		/// 视椎体中心偏移
		Vector2 mFrustumOffset;
		/// 视锥体焦点距离（用于立体渲染, 默认1.0）
		Real mFocalLength;

		/// 6个剪裁平面
		mutable Plane mFrustumPlanes[6];

		/// 保存上一个版本朝向和位置
		mutable Quaternion mLastParentOrientation;
		mutable Vector3 mLastParentPosition;

		/// 预先计算的特定渲染系统的投影矩阵
		mutable Matrix4 mProjMatrixRS;
		/// 预先计算用以深度范围渲染系统的标准投影矩阵
		mutable Matrix4 mProjMatrixRSDepth;
		/// 预先计算的标准的投影矩阵
		mutable Matrix4 mProjMatrix;
		/// 预先计算的观察矩阵
		mutable Matrix4 mViewMatrix;

		/// 视锥体形状改变是否重新计算
		mutable bool mRecalcFrustum;
		/// 计算观察位置改变是否重新计算
		mutable bool mRecalcView;
		/// 视锥体平面改变是否重新计算
		mutable bool mRecalcFrustumPlanes;
		/// 世界空间角落改变是否重新计算
		mutable bool mRecalcWorldSpaceCorners;
		/// 当顶点数据改变时重计算顶点数据
		mutable bool mRecalcVertexData;
		/// 是否自定义观察矩阵
		bool mCustomViewMatrix;
		/// 是否自定义投影矩阵
		bool mCustomProjMatrix;
		/// 是否手动扩展视锥体设置
		bool mFrustumExtentsManuallySet;
		/// 视锥体扩展
		mutable Real mLeft, mRight, mTop, mBottom;
		/// 视锥体方向模式
		mutable OrientationMode mOrientationMode;


		// 用以计算的内部函数
		// 通过左右上下计算投影参数
		virtual void calcProjectionParameters(Real& left, Real& right, Real& bottom, Real& top) const;
		/// 如果超过日期更新视锥体 
		virtual void updateFrustum(void) const;
		/// 如果超过日期更新观察
		virtual void updateView(void) const;
		/// 实现了更新视锥体 (超过了日期调用)
		virtual void updateFrustumImpl(void) const;
		/// 实现了更新视锥体 (超过了日期调用)
		virtual void updateViewImpl(void) const;
		virtual void updateFrustumPlanes(void) const;
		/// 更新视锥体平面的实现
		virtual void updateFrustumPlanesImpl(void) const;
		virtual void updateWorldSpaceCorners(void) const;
		virtual void updateWorldSpaceCornersImpl(void) const;
		virtual void updateVertexData(void) const;
		virtual bool isViewOutOfDate(void) const;
		virtual bool isFrustumOutOfDate(void) const;
		/// 提醒更新视锥体信息
		virtual void invalidateFrustum(void) const;
		/// 提醒更新观察信息
		virtual void invalidateView(void) const;


		/// 对可移动类型共享类等级名
		//static String msMovableType;

		mutable AxisAlignedBox mBoundingBox;
		//mutable VertexData mVertexData;

		//MaterialPtr mMaterial;
		//世界空间的角落坐标
		mutable Vector3 mWorldSpaceCorners[8];

		/// 如果视锥体激活作为它自身反射
		bool mReflect;
		/// 派生的反射矩阵
		mutable Matrix4 mReflectMatrix;
		/// 固定的反射平面
		mutable Plane mReflectPlane;
		/// 指向一个反射平面（自动更新）
		//const MovablePlane* mLinkedReflectPlane;
		/// 记录上一次使用的世界空间反射平面信息
		mutable Plane mLastLinkedReflectionPlane;

		/// 这个视锥体使用斜深度投影
		bool mObliqueDepthProjection;
		/// 固定斜深度投影平面
		mutable Plane mObliqueProjPlane;

		//const MovablePlane* mLinkedObliqueProjPlane;
		//mutable Plane mLastLinkedObliqueProjPlane;

	public:

		Frustum(const String& name = StringUtil::BLANK);
		virtual ~Frustum();


		/**
		设置视锥体的Y维的视域
		@remarks
		视域是在视锥体位置处的角度，场景的投影到屏幕的边界中。 高值（90度以上）是宽角度， 鱼眼类视野， 低角度（30度以内）做了拉伸，望远类视野。
		典型值在45到60度之间
		@par
		这个值表示垂直视域，水平视域的计算依赖视口的矩形
		@note
		设置FOV覆盖原始值，提供给frustum::setNearClipPlane.
		*/
		virtual void setFOVy(const Radian& fovy);

		/**
		返回视锥体Y-维视域FOV
		*/
		virtual const Radian& getFOVy(void) const;

		/** 设置近剪裁平面的距离
		@remarks
		近剪裁平面的位置是与视锥体位置到投影世界到屏幕处距离。这近剪裁平面距离，组合视域和横纵比，决定了观察世界的视口的大小，
		它标示为一定的的像素数。视锥体视口应该有着和屏幕视口通用的横纵比，以保障渲染不会变形
		@param
		nearDist  这个距离是从在世界坐标系中的视锥体到近裁平面的距离
		*/
		virtual void setNearClipDistance(Real nearDist);

		/**
		取得近裁平面的距离
		*/
		virtual Real getNearClipDistance(void) const;

		/** 设置远裁平面的距离
		@remarks
		视锥体是视锥体位置和视口边界组成的一个金字塔形
		这个方法设置金字塔最远端的距离。不同的应用程序需要不同的值：例如
		一个飞行模拟需要比第一人称射击远的非常多最远剪裁平面。重点是近剪裁平面和远剪裁平面之间的比例越大，用于提示像素深度的z-buffer的精度就越低。
		这是由于Z-buffer的Z范围被限制了（16或者32位）并且最大值必定缺陷的分布到近剪裁平面到远剪裁平面之间。
		如果发生了这些，你可以通过修改最近距离而不是最远距离来影响精度很多，牢记这一点。
		@param
		farDist  视锥体在世界坐标系的位置到最远剪裁平面的距离，如果你设置为0，意味这无限远的距离，当投影阴影时特别有用，但是要小心不要让近裁平面过于接近
		*/
		virtual void setFarClipDistance(Real farDist);

		/** 返回视锥体的远剪裁平面的距离
		*/
		virtual Real getFarClipDistance(void) const;

		/**
		设置视锥体横纵比
		@par
		默认值是1.3333, 对于多数窗口来说是
		*/
		virtual void setAspectRatio(Real ratio);

		/**
		返回视锥体横纵比
		*/
		virtual Real getAspectRatio(void) const;

		/**
		设置视锥体偏移量，用于立体渲染
		@remarks
		你可以设置两只眼睛的垂直和水平平面偏移值：
		在立体渲染视锥体移动水平平面。可以渲染两只眼睛，你需要两个摄像机渲染两个渲染目标
		@par
		在世界坐标系里视锥体偏移值，默认(0,0), 没有偏移量
		@param
		offset 水平和垂直平面的偏移值
		*/
		virtual void setFrustumOffset(const Vector2& offset);

		/**
		设置视锥体偏移量，用于立体渲染
		@remarks
		你可以设置两只眼睛的垂直和水平平面偏移值：
		在立体渲染视锥体移动水平平面。可以渲染两只眼睛，你需要两个摄像机渲染两个渲染目标
		@par
		在世界坐标系里视锥体偏移值，默认(0,0), 没有偏移量
		@param
		垂直偏移量
		@param
		水平偏移量
		*/
		virtual void setFrustumOffset(Real horizontal = 0.0, Real vertical = 0.0);

		/**
		返回视锥体偏移量
		*/
		virtual const Vector2& getFrustumOffset() const;

		/**
		设置视锥体焦距（用于立体渲染）
		@param
		focalLength 是视锥体的世界坐标到焦平面的距离
		*/
		virtual void setFocalLength(Real focalLength = 1.0);

		/** 返回焦平面距离
		*/
		virtual Real getFocalLength() const;


		/**
		手动设置视锥体的扩展
		@param 左右上下的位置，在观察空间中近剪裁平面与边剪裁平面相交的位置
		*/
		virtual void setFrustumExtents(Real left, Real right, Real top, Real bottom);
		/** 复位视锥体扩展由其他参数自动继承*/
		virtual void resetFrustumExtents();
		/** 取得视锥体在观察空间的扩展 */
		virtual void getFrustumExtents(Real& outleft, Real& outright, Real& outtop, Real& outbottom) const;

		/**
		取得这个视锥体对于当前渲染系统当前调整指定的的投影矩阵（DX,OGL,OGLES也许右手或左手，深度范围可能有不同）
		@remarks
		这个方法返回这个投影矩阵依赖渲染API版本，如果你想要一个典型的投影矩阵用getProjectionMatrix

		*/
		virtual const Matrix4& getProjectionMatrixRS(void) const;

		/**
		取得对于当前渲染系统校正过深度的投影矩阵，但是它只遵循右手规则
		@remarks
		这与依赖渲染API的getProjectionMatrix有些不同
		由于它仍然返回右手坐标投影矩阵，而无论使用何种渲染API--以Vertex和Fragment Shader程序为例他们需要这样
		不管怎样，这导致深度范围会在D3D[0,1]和GL[-1,1]这样不同的渲染系统中是不同，而且这样范围必须保证在可编程或固定渲染管线中是相同的
		*/
		virtual const Matrix4& getProjectionMatrixWithRSDepth(void) const;
		/**
		取得视锥体的标准投影矩阵，这个投影矩阵它遵循右手投影矩阵规则并且深度范围[-1.+1]
		@remarks
		这和与渲染API依赖的getProjectionMatrixRS不同，因此它始终返回右手坐标规范投影矩阵和深度范围[-1,+1],而不论使用什么样的渲染API
		*/
		virtual const Matrix4& getProjectionMatrix(void) const;

		/**取得视锥体的观察矩阵， 主要作为内部使用
		*/
		virtual const Matrix4& getViewMatrix(void) const;

		/** 计算这个视锥体的一个观察矩阵， 关联到一个可能的动态点，
		这主要是当相机相对于对于视锥体的渲染，由引擎内部调用。
		*/
		virtual void calcViewMatrixRelative(const Vector3& relPos, Matrix4& matToUpdate) const;


		/**
		设置是否对这个视锥体使用一个自定义的观察矩阵
		@remarks
		这是一个高级方法，它允许你手动设置一个为这个视锥体观察矩阵， 而不是根据它自己的位置和朝向来进行计算
		@note
		打开一个自定义观察矩阵之后，这个视锥体将会根据这个矩阵更新它自己的位置和朝向。你需要保证这个观察矩阵最新
		这个自定义矩阵将会由getViewMatrix返回。
		@param enable 如果为true，这个自定义观察矩阵将由第二个参数传入，这将在性能上由于自动计算。如果为false，这个视锥体将恢复自动计算观察矩阵
		@param viewMatrix 要用的自定义观察矩阵，这个矩阵必须是仿射矩阵
		@see 见Frustum::setCustomProjectionMatrix, Matrix4::isAffine
		*/
		virtual void setCustomViewMatrix(bool enable,
			const Matrix4& viewMatrix = Matrix4::IDENTITY);
		/// 返回是否是一个自定义的观察矩阵在使用
		virtual bool isCustomViewMatrixEnabled(void) const
		{
			return mCustomViewMatrix;
		}
		/** 
		设置是否对这个视锥体使用一个自定义的投影矩阵 
		@remarks
		这是一个高级方法，它允许你手动设置一个为这个视锥体投影矩阵， 而不是根据它自己的位置和朝向来进行计算
		@note
		打开一个自定义观察矩阵之后，这个视锥体将会根据这个矩阵更新它自己的位置和朝向。你需要保证这个投影矩阵最新
		这个自定义矩阵将会由getProjectionMatrix和其它派生的函数返回。
		@param enable
		如果为true，这个自定义投影矩阵将由第二个参数传入，这将在性能上由于自动计算。如果为false，这个视锥体将恢复自动计算投影矩阵
		@param projectionMatrix 自定义的投影矩阵
		@see 见Frustum::setCustomViewMatrix
		*/
		virtual void setCustomProjectionMatrix(bool enable,
			const Matrix4& projectionMatrix = Matrix4::IDENTITY);
		/// 返回是否使用一个自定义的投影矩阵
		virtual bool isCustomProjectionMatrixEnabled(void) const
		{
			return mCustomProjMatrix;
		}

		/**
		返回这个视锥体的剪裁平面（世界空间）
		@remarks
		这个剪裁平面是按枚举常量FrustumPlane顺序
		@return 返回视锥体剪裁平面列表
		*/
		virtual const Plane* getFrustumPlanes(void) const;

		/** 返回这个视锥体指定平面（世界空间）
		@remarks
		获取这个构成视锥体平面中的一个引用
		@return
		返回指定视锥体平面
		*/
		virtual const Plane& getFrustumPlane(unsigned short plane) const;

		/**
		测试给定的AABB容器在视锥体中是否可见
		@param
		bound 要被检查的AABB的盒子（世界空间）
		@param
		culledBy 可选的指针，如果返回结果为false，指向一个将被剔除掉的平面数量
		@return
		如果这个盒子为可见，将返回true
		@par
		除此之外，返回false
		*/
		virtual bool isVisible(const AxisAlignedBox& bound, FrustumPlane* culledBy = 0) const;

		/**
		测试给定的碰撞球容器在视锥体是否可见
		@param
		bound 碰撞球是否被剔除 （世界空间）
		culledBy 可选的指针，如果返回结果为false，指向一个将被剔除掉的平面数量
		@return
		如果求是可见的，返回true
		@par
		除此之外返回false
		*/
		virtual bool isVisible(const Sphere& bound, FrustumPlane* culledBy = 0) const;

		/** 测试给定的顶点是否在视锥体中可见
		@param
		vert 要被剪裁的测试的顶点（世界空间）
		@param
		culledBy 可选的指针，如果返回结果为false，指向一个将被剔除掉的平面数量
		@return
		如果可见，返回true
		@par
		此外返回false
		*/
		virtual bool isVisible(const Vector3& vert, FrustumPlane* culledBy = 0) const;

		/// 重载MovableObject::getTypeFlags
		uint32 getTypeFlags(void) const;


		/** 重载 MovableObject */
		//const AxisAlignedBox& getBoundingBox(void) const;

		/** 重载 MovableObject */
		//Real getBoundingRadius(void) const;

		/** 重载 MovableObject */
		//void _updateRenderQueue(RenderQueue* queue);

		/** 重载 MovableObject */
		//const String& getMovableType(void) const;

		/** 重载 MovableObject */
		//void _notifyCurrentCamera(Camera* cam);

		/** 重载 Renderable */
		//const MaterialPtr& getMaterial(void) const;

		/** 重载 Renderable */
		//void getRenderOperation(RenderOperation& op);

		/** 重载 Renderable */
		//void getWorldTransforms(Matrix4* xform) const;

		/** 重载 Renderable */
		//Real getSquaredViewDepth(const Camera* cam) const;

		/** 重载 Renderable */
		//const LightList& getLights(void) const;


		/**
		获取视锥体空间的的角落在世界空间的
		@remarks
		这些角按下列顺序排列： top-right near,
		top-left near, bottom-left near, bottom-right near,
		top-right far, top-left far, bottom-left far, bottom-right far.
		*/
		virtual const Vector3* getWorldSpaceCorners(void) const;

		/**
		设置要使用的投影的类型(正交或透视  默认是透视)
		*/
		virtual void setProjectionType(ProjectionType pt);

		/**
		返回使用的投影类型
		*/
		virtual ProjectionType getProjectionType(void) const;

		/**
		设置正交窗口设置， 只对于正交投影渲染起作用
		@note 调用这个方法将重新计算横纵比，如果你要保存横纵比用setOrthoWindowHeight 或setOrthoWindowHeight，但它将会修正一个或另一个矩形区域到一个特别的大小。
		@param w, h 观察窗口在世界单元里的大小
		*/
		virtual void setOrthoWindow(Real w, Real h);
		/**
		设置正交窗口高， 只对于正交投影渲染起作用
		@note 这和导致重新计算窗口横纵比
		@param h 是观察窗口在世界单元里的大小
		*/
		virtual void setOrthoWindowHeight(Real h);
		/**
		设置正交窗口宽度， 只对于正交投影渲染起作用
		@note 这和导致重新计算窗口横纵比
		@param w  是观察窗口在世界单元里的大小
		*/
		virtual void setOrthoWindowWidth(Real w);
		/**
		获取正交窗口高度，只对正交渲染
		*/
		virtual Real getOrthoWindowHeight() const;
		/**
		获取正交窗口宽度，只对正交渲染
		*/
		virtual Real getOrthoWindowWidth() const;


		/** 修改这个视锥体以至于他能够由它自己通过指定的屏幕的反射来渲染
		@remarks
		这对plannar 反射非常有用
		*/
		//virtual void enableReflection(const Plane& p);
		/**
		 修改这个视锥体以至于他能够由它自己通过指定的屏幕的反射来渲染
		 注意：这个方法的版本连接到一个平面以至于改变它自动的拾取。
		 不能在这个视锥体之前销毁这个平面
		 @remarks
		 这对plannar 反射非常有用
		 */
		//virtual void enableReflection(const MovablePlane* p);

		/// 返回这个视锥体是否被反射
		virtual bool isReflected(void) const { return mReflect; }
		/// 如果合适返回这个视锥体矩阵的反射矩阵
		virtual const Matrix4& getReflectionMatrix(void) const { return mReflectMatrix; }
		/// 如何合适返回这个反射平面
		virtual const Plane& getReflectionPlane(void) const { return mReflectPlane; }

		/**
		投影一个球体到近剪裁平面，并获取边界矩形
		@param sphere 要投影的在世界空间球体
		@param radius 球体半径
		@param left, top, right, bottom 指向目标的值，这些将由标准化设备坐标完成(-1,1)
		@return 如果球体投影到近剪裁平面的子集返回true，如果整个近剪裁平面被包含则返回false
		*/
		virtual bool projectSphere(const Sphere& sphere,
			Real* left, Real* top, Real* right, Real* bottom) const;

		/**
		链接这个视锥体到一个自定义的近剪裁平面，它能够被用于在一个自定义的方式而不用用户剪裁平面来剪裁几何体
		@remarks
		有几个应用程序可以通过一个平面任意的裁剪一个场景。最普遍的是当渲染一个反射到一个纹理，正如你只想要渲染在水平面上渲染几何体。
		虽然他们可以用用户剪裁平面时，但不支持所有的显卡，可能没有硬件加速。替代的方法是，用一种伪造的近剪裁平面，它可以支持所有的硬件，应用到任意的剪裁平面。
		这会改变视锥体的形状，会到导致深度缓冲失去精度，但使用这种技术多数情况不是问题
		@par
		这个版本的方法链接到一个平面，而不是一个通过值定义的平面， 因此你能够改变这个平面（移动/旋转这个关联到的节点）
		并且它会自动的影响到对象
		@note 注意这个技术只能工作在透视投影下
		@param plane 要链接到要应用剪裁的平面。这个平面必须在这个相机链接到它时存在；不能先与视锥体之前销毁它
		*/
		//virtual void enableCustomNearClipPlane(const MovablePlane* plane);

		/**
		链接这个视锥体到一个自定义的近剪裁平面,它能够被用于在一个自定义的方式而不用用户剪裁平面来剪裁几何体
		@remarks
		有几个应用程序可以通过一个平面任意的裁剪一个场景。最普遍的是当渲染一个反射到一个纹理，正如你只想要渲染在水平面上渲染几何体。
		虽然他们可以用用户剪裁平面时，但不支持所有的显卡，可能没有硬件加速。替代的方法是，用一种伪造的近剪裁平面，它可以支持所有的硬件，应用到任意的剪裁平面。
		这会改变视锥体的形状，会到导致深度缓冲失去精度，但使用这种技术多数情况不是问题
		@note 注意这个技术只能工作在透视投影下
		@param plane 要链接到要应用剪裁的平面。这个平面必须在这个相机链接到它时存在；不能先与视锥体之前销毁它
		*/
		//virtual void enableCustomNearClipPlane(const Plane& plane);

		/** 关闭任意自定义近剪裁平面 */
		//virtual void disableCustomNearClipPlane(void);
		/** 是否使用一个近剪裁平面*/
		virtual bool isCustomNearClipPlaneEnabled(void) const
		{
			return mObliqueDepthProjection;
		}

		/// @copydoc MovableObject::visitRenderables
		//void visitRenderables(Renderable::Visitor* visitor,
		//	bool debugRenderables = false);

		/**
		@brief  用来减少远剪裁平面投影,以避免错误的小常数
		*/
		static const Real INFINITE_FAR_PLANE_ADJUST;
		/** 获取这个视锥体派生的位置  */
		virtual const Vector3& getPositionForViewUpdate(void) const;
		/** 获取这个视锥体派生的朝向 */
		virtual const Quaternion& getOrientationForViewUpdate(void) const;

		/**
		获取一个世界空间中闭合的视锥体列表
		*/
		PlaneBoundedVolume getPlaneBoundedVolume();

		/** 设置视锥体朝向的模式，默认OR_DEGREE_0
		@remarks
		设置一个视锥体的朝向，这个只支持IOS，在别的平台会抛出异常
		*/
		void setOrientationMode(OrientationMode orientationMode);

		/** 获取视锥体朝向的模式
		@remarks
		获取一个视锥体的朝向，这个只支持IOS，在别的平台会抛出异常
		*/
		OrientationMode getOrientationMode() const;

	};
}

#endif