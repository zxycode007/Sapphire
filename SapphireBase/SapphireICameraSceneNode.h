#ifndef _SAPPHIRE_I_CAMERA_SCENE_NODE_
#define _SAPPHIRE_I_CAMERA_SCENE_NODE_

#include "SapphireISceneNode.h"
#include "SapphireIEventRecevier.h"

namespace Sapphire
{
	struct SViewFrustum;

	//! 控制相机的场景节点
	/** 这个场景将由相机观察点的位置来进行渲染。由于ICameraScenNode是一个场景节点，它能够被附加到任何任意
	其它的场景节点，并且将跟随它的父节点移动，旋转
	*/
	class ICameraSceneNode : public ISceneNode, public IEventReceiver
	{
	public:

		//! 构造器
		ICameraSceneNode(ISceneNode* parent, ISceneManager* mgr, SINT32 id,
			const Vector3& position = Vector3(0, 0, 0),
			const Vector3& rotation = Vector3(0, 0, 0),
			const Vector3& scale = Vector3(1.0f, 1.0f, 1.0f))
			: ISceneNode(parent, mgr, id, position, rotation, scale), IsOrthogonal(false) {}

		//! 设置相机的投影矩阵
		/** 矩阵类有几个构建投影矩阵的方法
		Matrix4::buildProjectionMatrixPerspectiveFovLH.
		注意这个矩阵会一直保持这个方法的设置，直到下列方法被调用: setNearValue,
		setFarValue, setAspectRatio, setFOV.
		\param projection 这个相机的投影矩阵
		\param isOrthogonal 如果这个矩阵是正交矩阵，设置为true
		orthogonal one (e.g. 对于 matrix4::buildProjectionMatrixOrtho).
		*/
		virtual void setProjectionMatrix(const Matrix4& projection, bool isOrthogonal = false) = 0;

		//! 获取当前相机的投影矩阵
		/** \return 当前相机的投影矩阵 */
		virtual const Matrix4& getProjectionMatrix() const = 0;

		//! 获取当前相机的观察矩阵
		/** \return 当前相机的观察矩阵 */
		virtual const Matrix4& getViewMatrix() const = 0;

		//! 设置一个当前观察矩阵Affector
		/** 传递过来的矩阵，当它要取得更新时将会与一个观察矩阵相乘，这允许自定义相机设置
		例如，一个反射相机
		\param affector affector 矩阵. */
		virtual void setViewMatrixAffector(const Matrix4& affector) = 0;

		//! 获取自定义的观察矩阵 affector.
		/** \return affector 矩阵. */
		virtual const Matrix4& getViewMatrixAffector() const = 0;

		//! 可以发生鼠标和按键事件给相机
		/** 大多数相机能够无视这个输入，但相机场景节点它可以由以下创建ISceneManager::addCameraSceneNodeMaya
		ISceneManager::addCameraSceneNodeFPS，可以取得对于它改变位置和观察目标的输入
		 */
		virtual bool OnEvent(const SEvent& event) = 0;

		//! 设置相机的观察目标
		/** 如果相机的目标和旋转是绑定的（见bindTargetAndRotation()），那么调用这个将改变
		相机场景节点旋转去匹配这个目标。
		注意：setTarget内部用的是当前绝对位置，所以如果改变自上一次渲染的setPosition，在用这个函数之前你必须调用updateAbsolutePosition
		
	 
		\param pos 相机观察的目标，是世界坐标系的坐标*/
		virtual void setTarget(const Vector3& pos) = 0;

		//! 设置节点的旋转 （和setTarget相反）
		/** 这个值修改这个节点相关的旋转
		如果相交目标和旋转被绑定（见bindTargetAndRotation()），那么这次调用也会改变
		相机目标来匹配这个旋转
		\param rotation 这个节点新旋转的角度*/
		virtual void setRotation(const Vector3& rotation) = 0;

		//! 获取当前相机观察的目标
		/** \return 当前相机观察的目标的世界坐标*/
		virtual const Vector3& getTarget() const = 0;

		//! 设置相机的up向量
		/** \param pos: 相机的新up向量*/
		virtual void setUpVector(const Vector3& pos) = 0;

		//! 获取相机的up向量
		/** \return 返回这个相机up向量的世界坐标*/
		virtual const Vector3& getUpVector() const = 0;

		//! 获取这个相机的近裁平面的值
		/** \return 相机的近裁平面的值 */
		virtual Real getNearValue() const = 0;

		//! 获取这个相机远裁平面的值
		/** \return 相机远裁平面的值 */
		virtual Real getFarValue() const = 0;

		//! 获取相机的横纵比
		/** \return 相机的横纵比 */
		virtual Real getAspectRatio() const = 0;

		//! 获取这个相机的视域
		/** \return 相机的视域（弧度） */
		virtual Real getFOV() const = 0;

		//! 设置近裁平面的值（默认1.0f）
		/** \param zn: 新的最近的z值*/
		virtual void setNearValue(Real zn) = 0;

		//! 设置远裁平面的值 （默认:2000f）
		/** \param zf: 新最远z值 */
		virtual void setFarValue(Real zf) = 0;

		//! 设置这个纵横比(默认: 4.0f / 3.0f)
		/** \param aspect: 新的横纵比 */
		virtual void setAspectRatio(Real aspect) = 0;

		//! 设置fov (默认: PI / 2.5f)
		/** \param fovy: 新的fov弧度 */
		virtual void setFOV(Real fovy) = 0;

		//! 获取视锥体
		/** 有时须有通过BSP树和LOD渲染节点
		\return 当前视锥体*/
		virtual const SViewFrustum* getViewFrustum() const = 0;

		//! 关闭或打开这个相机获取按键或鼠标的输入
		/** 如果设置为true，这个相机将相应来自按键或鼠标的输入*/
		virtual void setInputReceiverEnabled(bool enabled) = 0;

		//! 检测相机的输入接受器是否是打开的
		virtual bool isInputReceiverEnabled() const = 0;

		//! 检测相机释放是正交的
		virtual bool isOrthogonal() const
		{
			//_IRR_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX;
			return IsOrthogonal;
		}

		//! 绑定这个相机场景节点的旋转到它的目标位置，或者取消绑定
		/** 当绑定后，调用setRotation将沿Z正方向更新相机的目标位置，同理调用setTarget（）
		将更新它的旋转以至于它的+Z轴将指向目标点。FPS相机默认用这种绑定，其它相机不是这样.
		\param bound true的话绑定相机场景节点的旋转和目标，false取消它们之间绑定
		@见 getTargetAndRotationBinding() */
		virtual void bindTargetAndRotation(bool bound) = 0;

		//! 查询这个相机场景节点的旋转并且的目标位置是否一起绑定
		/** @见 bindTargetAndRotation() */
		virtual bool getTargetAndRotationBinding(void) const = 0;

		//! 序列化写入这个相机节点的属性
		virtual void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options = 0) const
		{
			ISceneNode::serializeAttributes(out, options);

			if (!out)
				return;
			out->addBool("IsOrthogonal", IsOrthogonal);
		}

		//! 读取这个相机节点的属性值
		virtual void deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options = 0)
		{
			ISceneNode::deserializeAttributes(in, options);
			if (!in)
				return;

			if (in->findAttribute("IsOrthogonal"))
				IsOrthogonal = in->getAttributeAsBool("IsOrthogonal");
		}

	protected:

		void cloneMembers(ICameraSceneNode* toCopyFrom)
		{
			IsOrthogonal = toCopyFrom->IsOrthogonal;
		}

		bool IsOrthogonal;
	};
}

#endif 