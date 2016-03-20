#ifndef _SAPPHIRE_ISCENE_NODE_
#define _SAPPHIRE_ISCENE_NODE_


#include "SapphirePrerequisites.h"
#include "SapphireEDebugSceneTypes.h"
#include "SapphireIReferenceCounter.h"
#include "SapphireVector3.h"
#include "SapphireAxisAlignedBox.h"
#include "SapphireMatrix3.h"
#include "SapphireMatrix4.h"
#include "SapphireSharedPtr.h"
#include "SapphireIAttributes.h"
#include "SapphireIAttributeExchangingObject.h"
#include "SapphireISceneNodeAnimator.h"
#include "SapphireESceneNodeTypes.h"
#include "SapphireEMaterialFlags.h"
#include "SapphireEMaterialTypes.h"
#include "SapphireSMaterial.h"
#include "SapphireITriangleSelector.h"
#include "SapphireECullingTypes.h"





namespace Sapphire
{
	//class ISceneNodeAnimator;
	class ISceneManager;
	class ITriangleSelector;
}




namespace Sapphire
{
	//  场景节点的智能指针
	typedef SharedPtr<ISceneNode> ISceneNodePtr;
	//typedef shared_ptr<ITriangleSelector> ITriangleSelectorPtr;
	//! 定义场景节点列表
	typedef std::list<ISceneNode*> ISceneNodeList;
	//typedef std::list<ISceneNodePtr> ISceneNodeList;
	//! 定义场景节点动画器列表
	typedef std::list<ISceneNodeAnimator*> ISceneNodeAnimatorList;
	//typedef std::list<ISceneNodeAnimatorPtr> ISceneNodeAnimatorList;
	


	class ISceneNode : public virtual  IReferenceCounter
	{
	public:

		 
		ISceneNode(ISceneNode* parent, ISceneManager* mgr, SINT32 id = -1,
			const Vector3& position = Vector3(0, 0, 0),
			const Vector3& rotation = Vector3(0, 0, 0),
			const Vector3& scale = Vector3(1.0f, 1.0f, 1.0f))
			: RelativeTranslation(position), RelativeRotation(rotation), RelativeScale(scale),
			Parent(0), SceneManager(mgr), TriangleSelector(0), ID(id),
			AutomaticCullingState(EAC_BOX), DebugDataVisible(EDS_OFF),
			IsVisible(true), IsDebugObject(false)
		{
			if (parent)
				parent->addChild(this);

			updateAbsolutePosition();
		}

		~ISceneNode()
		{
			// 删除所有的孩子节点
			removeAll();
			// 删除所有的animators
			ISceneNodeAnimatorList::iterator ait = Animators.begin();
			for (; ait != Animators.end(); ++ait)
			{
				//SAPPHIRE_DELETE(*ait);
				(*ait)->drop();
				 
			}
			//如果三角形选择器不为空
			if (TriangleSelector)
			{
				//TriangleSelector->drop();
				//SAPPHIRE_DELETE(TriangleSelector);
				TriangleSelector->drop();
			}
				
			
		};
		//! 移除所有的场景节点孩子节点
		/** 
		在这个场景节点找出孩子列表并且删除
		*/
		virtual void removeAll()
		{
			ISceneNodeList::iterator it = Children.begin();

			for (; it != Children.end(); ++it)
			{
				(*it)->Parent->drop();
				//(*it)->drop();
				//SAPPHIRE_DELETE(*it);
				(*it)->drop();
			}

			Children.clear();
		}



		//! 这个方法在整个场景的渲染过程之前调用
		/** 通过调用这个方法，节点会在这次调用期间将它们注册到渲染管线中，预先计算将要渲染的几何体，并且防止它们的子节点如果被剔除掉仍然被注册。
		如果你实现自己的场景代码，你应该用下列类似的代码覆写这个方法
		if (IsVisible)
		SceneManager->registerNodeForRendering(this);
		ISceneNode::OnRegisterSceneNode();
		*/
		virtual void OnRegisterSceneNode()
		{
			if (IsVisible)
			{
				ISceneNodeList::iterator it = Children.begin();
				for (; it != Children.end(); ++it)
					(*it)->OnRegisterSceneNode();
			}
		}


		//! OnAnimate() 在整个场景的渲染过程之前调用
		/** 这里节点会计算或存储动画，并且做其它有用的事情，
		OnAnimate()在这里应该对于所有子场景节点调用。这个方法每一帧都会调用一次。不管场景节点
		是否可见
		\param timeMs 当前时间的毫秒数*/
		virtual void OnAnimate(UINT32 timeMs)
		{
			if (IsVisible)
			{
				// 激活这个节点所有的动画器animator

				ISceneNodeAnimatorList::iterator ait = Animators.begin();
				while (ait != Animators.end())
				{
					// 继续寻找下一个节点调用animateNode()
					// 以至于这个动画器animator可以从场景节点移除它们本身，而不会迭代器变得无效
					ISceneNodeAnimator* anim = *ait;
					++ait;
					anim->animateNode(this, timeMs);
				}

				// 更新绝对位置
				updateAbsolutePosition();

				// 对所有子节点也应用OnAnimate

				ISceneNodeList::iterator it = Children.begin();
				for (; it != Children.end(); ++it)
					(*it)->OnAnimate(timeMs);
			}
		}


		//! 渲染这个节点
		virtual void render() = 0;


		//! 返回这个场景节点的名字
		/** \return 场景节点的字符串名 */
		virtual const c8* getName() const
		{
			return Name.c_str();
		}


		//! 设置节点的字符串名
		/** \param name 场景节点的新名字 */
		virtual void setName(const c8* name)
		{
			Name = name;
		}


		//! 设置这个场景节点的名字
		/** \param name 这个场景的节点*/
		virtual void setName(const String& name)
		{
			Name = name;
		}


		//! 获取AABB盒子，不用转换这个节点边界盒子
		/** 这意味着如果这个节点是一个动画的3d角色，在一间室内移动，碰撞盒子将依照环绕在
		原点。获取盒子在真实世界坐标系，就要通过用getAbsoluteTransformation()矩阵转换它，或者
		简单使用getTransformedBoundingBox()，它也一样
		  */
		virtual const AxisAlignedBox& getBoundingBox() const = 0;


		//! 获取转换到真实世界坐标的这个节点的AABB
		/** \return 转换后的AABB */
		virtual const AxisAlignedBox getTransformedBoundingBox() const
		{
			AxisAlignedBox box = getBoundingBox();
			//AbsoluteTransformation.transformBoxEx();
			box.transform(AbsoluteTransformation);
			return box;
		}


		//! 获取场景节点的决定变换位置。在每一次OnAnimate()调用重复计算
		/** NOTE: 对于速度的原因，绝对变换在每次实际变换改变或者通过一个父节点的变换时不是自动重计算的
		.替代这个在每一帧OnAnimate发生时的更新，你可以用updateAbsolutePosition()强制更新
		\return 绝对变换矩阵. */
		virtual const Matrix4& getAbsoluteTransformation() const
		{
			return AbsoluteTransformation;
		}


		//! 返回这个场景节点实际变换
		/** 这个实际变换是存在3个向量中： translation, rotation 和 scale。要取得实际变换矩阵，
		它由这三个值计算。
		\return 实际的变换矩阵 */
		virtual Matrix4 getRelativeTransformation() const
		{
			Matrix4 mat;
			mat.setRotationRadians(RelativeRotation);
			mat.setTrans(RelativeTranslation);

			if (RelativeScale != Vector3(1.f, 1.f, 1.f))
			{
				Matrix4 smat;
				smat.setScale(RelativeScale);
				mat = smat * mat;
			}

			return mat;
		}


		//! 返回这个节点是否可见（如果它所有的父亲都可见的话）
		/** 这只是通过用户的选项设置，并不会使用几何剔除的
		\return 返回这个节点请求的可见性，true意味着可见（如果所有父亲节点都可见的话）*/
		virtual bool isVisible() const
		{
			
			return IsVisible;
		}

		//! 测试这个节点是否真的可见，并考虑它父亲的可见性
		/** \return 如果它和所有的父亲都可见返回true */
		virtual bool isTrulyVisible() const
		{
			
			if (!IsVisible)
				return false;
			//如果父亲节点为空，说明没有父亲节点，返回可见。
			if (!Parent)
				return true;
			//父亲节点不为空，返回父亲可见性
			return Parent->isTrulyVisible();
		}

		//! 设置这个节点是否可见或不可见
		/** 当设置为false，所有的孩子节点不可见。不可见节点对于碰撞管理的边界盒子方法是无效的
		\param isVisible 如果节点可见的设为true */
		virtual void setVisible(bool isVisible)
		{
			IsVisible = isVisible;
		}


		//! 获取场景节点ID
		/**  
		\return  id. */
		virtual SINT32 getID() const
		{
			return ID;
		}


		//! 设置场景节点ID
		/**  
		\param id 新ID. */
		virtual void setID(SINT32 id)
		{
			ID = id;
		}


		//! 添加一个孩子到这个场景节点
		/** 如果这个场景节点以及有一个父亲，它首先从其他父亲节点移除
		\param child 一个新孩子节点 */
		virtual void addChild(ISceneNode* child)
		{
			//不为空，并且指向的对象不相同
			if (child && (child != this))
			{
				// 是否改变场景管理器
				if (SceneManager != child->SceneManager)
					child->setSceneManager(SceneManager);

				child->grab();
				child->remove(); // 移除这个孩子节点以前的父亲
				Children.push_back(child);
				child->Parent = this;
			}
		}




		//! 从场景节点移除一个孩子
		/** 如果找到这个孩子列表，这个孩子指针被丢弃并且如果它没有其他引用存在，将会被删除
		\param child 将要被移除的孩子指针
		\return 如果这个孩子被移除返回true，如果没有返回false。 因为它不能在孩子列表中被找到
		*/
		virtual bool removeChild(ISceneNode* child)
		{
			ISceneNodeList::iterator it = Children.begin();
			for (; it != Children.end(); ++it)
				if ((*it) == child)
				{
					(*it)->Parent = 0;
					(*it)->drop();
					Children.erase(it);
					return true;
				}

			
			return false;
		}


 


		//! 从场景删除这个节点
		/** 如果这个节点没别的引用，那么会被删除掉
		*/
		virtual void remove()
		{
			if (Parent)
				Parent->removeChild(this);
		}


		//! 添加一个animator啦激活这个节点动画
		/** \param animator 一个新animator指针 */
		virtual void addAnimator(ISceneNodeAnimator* animator)
		{
			if (animator)
			{
				Animators.push_back(animator);
				animator->grab();
			}
		}


		//! 获取一个所有场景节点animator的列表
		/** \return 这个节点关联的索引animator */
		const ISceneNodeAnimatorList& getAnimators() const
		{
			return Animators;
		}


		//! 从这个场景节点移除一个animator
		/** 如果这个animator被找到，丢其它，如果没别的引用就会被删除
		\param animator 一个要删除animator的指针. */
		virtual void removeAnimator(ISceneNodeAnimator* animator)
		{
			ISceneNodeAnimatorList::iterator it = Animators.begin();
			for (; it != Animators.end(); ++it)
			{
				if ((*it) == animator)
				{
					(*it)->drop();
					Animators.erase(it);
					return;
				}
			}
		}


		//! 移除这个场景节点所有的animator
		virtual void removeAnimators()
		{
			ISceneNodeAnimatorList::iterator it = Animators.begin();
			for (; it != Animators.end(); ++it)
				(*it)->drop();

			Animators.clear();
		}


		//! 返回基于0开始的索引i材质
		/** 获取这个节点使用的材质数量，用getMaterialCount().
		对于插入节点到场景层次结构在一个可优化的位置使得对渲染状态有最小的改变，但是能够用于直接修改
		这个场景节点的材质
		\param num 基于0索引，最大值是getMaterialCount() - 1.
		\return 材质索引 */
		virtual  SMaterial& getMaterial(UINT32 num)
		{
			return  IdentityMaterial;
		}


		//! 获取这个场景节点的材质数量
		/** \return 当前场景的材质数量 */
		virtual UINT32 getMaterialCount() const
		{
			return 0;
		}


		//! 一次设置所有材质标志到一个新值
		/**  用于 例如：如果你想要整个网格都手动光源的影响
		\param flag 所有材质要设置的标志
		\param newvalue 这个标志的新值 */
		void setMaterialFlag( E_MATERIAL_FLAG flag, bool newvalue)
		{
			for (UINT32 i = 0; i<getMaterialCount(); ++i)
				getMaterial(i).setFlag(flag, newvalue);
		}


		//! 设置在这个场景节点所以材质的指定层纹理到一个新纹理
		/** \param textureLayer 要设置的纹理层，必须小于MATERIAL_MAX_TEXTURES
		\param texture 新纹理. */
		void setMaterialTexture(UINT32 textureLayer, ITexture* texture)
		{
			if (textureLayer >=  MATERIAL_MAX_TEXTURES)
				return;

			for (UINT32 i = 0; i<getMaterialCount(); ++i)
				getMaterial(i).setTexture(textureLayer, texture);
		}


		//! 设置这个场景节点所有材质的材质类型到一个新类型
		/** \param newType 要设置新材质类型 */
		void setMaterialType( E_MATERIAL_TYPE newType)
		{
			for (UINT32 i = 0; i<getMaterialCount(); ++i)
				getMaterial(i).MaterialType = newType;
		}


		//! 获取这个节点相对于父节点的缩放值
		/** 这个相对于它父节点的缩放值，如果你想要绝对缩放，用getAbsoluteTransformation().getScale()
		\return 返回这个场景节点的缩放值 */
		virtual const Vector3& getScale() const
		{
			return RelativeScale;
		}


		//! 设置这个场景节点的相对缩放值
		/** \param scale 新的缩放值，相对于父节点 */
		virtual void setScale(const Vector3& scale)
		{
			RelativeScale = scale;
		}


		//! 获取相对父节点的旋转角度
		/** 注意：这是相对旋转角度。如果你需要绝对旋转，用getAbsoluteTransformation().getRotation()
		\return 当前场景节点相对旋转*/
		virtual const Vector3& getRotation() const
		{
			return RelativeRotation;
		}


		//! 设置相对于父节点的旋转角度
		/** 这是修改这个节点相对旋转
		\param rotation 新的旋转角度 */
		virtual void setRotation(const Vector3& rotation)
		{
			RelativeRotation = rotation;
		}


		//! 获取这节点个相对于父节点的位置
		/** 注意：这个是相对于它父节点位置。如果你想要它世界坐标的位置， 用getAbsolutePosition()代替
		\return 返回当前节点相对于它父节点 */
		virtual const Vector3& getPosition() const
		{
			return RelativeTranslation;
		}


		//! 设置相对于父节点的位置
		/** 注意：这个是相对于它父节点位置
		\param newpos 这个场景节点的新相对位置 */
		virtual void setPosition(const Vector3& newpos)
		{
			RelativeTranslation = newpos;
		}


		//! 获取这个节点在世界坐标系的绝对位置
		/** 如果你想要这个节点相对于父节点的相对位置用 getPosition()
		注意：由于速度原因，绝对位置在每一个相对位置改变或者通过一个父节点的位置改变不是自动重计算的。
		在每一帧OnAnimate中更新是经常发生的，你可以强制调用updateAbsolutePosition()强制更新
		\return 返回当前节点的绝对位置（最后一次更新在调用updateAbsolutePosition后）*/
		virtual Vector3 getAbsolutePosition() const
		{
			return AbsoluteTransformation.getTrans();
			
		}


		//! 打开和关闭基于AABB的自动剔除
		/** 默认自动剔除是开启的、注意：不是所有场景节点都支持剔除，某些
		节点由于某些特殊原因存在，比如八叉树节点
		\param state 使用的剔除状态 */
		void setAutomaticCulling(UINT32 state)
		{
			AutomaticCullingState = state;
		}


		//! 获取自动剔除的状态
		/** \return 返回自动剔除的状态 */
		UINT32 getAutomaticCulling() const
		{
			return AutomaticCullingState;
		}


		//! 设置debug数据类似AABB盒子可见
		/** 一个自E_DEBUG_SCENE_TYPE类型的位或
		请注意：不是所有场景节点都支持所有的debug数据类型
		\param state 要使用的debug数据可视化状态*/
		virtual void setDebugDataVisible(UINT32 state)
		{
			DebugDataVisible = state;
		}

		//! 返回可视的debug数据，如AABB盒子可见性
		/** \return 一个自E_DEBUG_SCENE_TYPE类型的位或
		  */
		UINT32 isDebugDataVisible() const
		{
			return DebugDataVisible;
		}


		//! 设置这个场景节点是否是一个debug对象
		/** debug对象有些特殊属性，例如它们能够更容易排除碰撞检测或序列化 */
		void setIsDebugObject(bool debugObject)
		{
			IsDebugObject = debugObject;
		}


		//! 返回这个场景对象是否是一个debug对象
		/** debug对象有些特殊属性，例如它们能够更容易排除碰撞检测或序列化 
		\return 如果这个节点是一个debug对象返回true */
		bool isDebugObject() const
		{
			
			return IsDebugObject;
		}


		//! 返回所有孩子的常引用
		/** \return 此节点的所有孩子节点*/
		const ISceneNodeList& getChildren() const
		{
			return Children;
		}


		//! 改变这个场景节点的父节点
		/** \param newParent T新的父节点 */
		virtual void setParent(ISceneNode* newParent)
		{
			grab();
			remove();

			Parent = newParent;

			if (Parent)
				Parent->addChild(this);

			drop();
		}


		//! 返回这个场景管理的三角形选择器
		/** 你可以用ISceneManager::createTriangleSelector()或ISceneManager::createOctreeTriangleSelector()
		创建一个三角形选择器TriangleSelector。某些节点默认就能创建它们子节点选择器， 所以它需要调用ISceneNode::getTriangleSelector().检查这个节点
		是否已经有一个三角形节点
		\return 一个三角形选择器的指针，如果0，则没有 */
		virtual ITriangleSelector* getTriangleSelector() const
		{
			return TriangleSelector;
		}


		//! 设置这个节点三角形选择器TriangleSelector
		/** 这个选择器用于引擎碰撞检测。
		你可以用ISceneManager::createTriangleSelector()或ISceneManager::createOctreeTriangleSelector()
		创建一个三角形选择器TriangleSelector。某些节点默认就能创建它们子节点选择器， 所以它需要调用ISceneNode::getTriangleSelector().检查这个节点
		是否已经有一个三角形节点
		\param selector 这个场景节点新的三角形选择器 */
		virtual void setTriangleSelector(ITriangleSelector* selector)
		{
			if (TriangleSelector != selector)
			{
				if (TriangleSelector)
					TriangleSelector->drop();

				TriangleSelector = selector;
				if (TriangleSelector)
					TriangleSelector->grab();
			}
		}


		//! 基于相对它父节点位置更新绝对位置
		/** 注意：这不会递归更新它们父节点的绝对位置，所以，如果要深层次你需要从它父节点更新
		*/
		virtual void updateAbsolutePosition()
		{
			if (Parent)
			{
				AbsoluteTransformation =
					Parent->getAbsoluteTransformation() * getRelativeTransformation();
			}
			else
				AbsoluteTransformation = getRelativeTransformation();
		}


		//! 返回这个场景节点的父节点
		/** \return 父节点的指针 */
		ISceneNode* getParent() const
		{
			return Parent;
		}


		//! 返回节点类型
		/** \return 这个节点类型 */
		virtual ESCENE_NODE_TYPE getType() const
		{
			return ESNT_UNKNOWN;
		}


		//! 写这个场景节点的属性
		/** 实现这个展示这个场景节点的属性给脚本语言，编辑器，debugger或xml等序列化目标
		\param out 要写入的属性容器
		\param options 可能会影响序列化的附加选项 */
		virtual void serializeAttributes( IAttributes* out, SAttributeReadWriteOptions* options = 0) const
		{
			if (!out)
				return;
			out->addString("Name", Name.c_str());
			out->addInt("Id", ID);

			out->addVector3d("Position", getPosition());
			out->addVector3d("Rotation", getRotation());
			out->addVector3d("Scale", getScale());

			out->addBool("Visible", IsVisible);
			out->addInt("AutomaticCulling", AutomaticCullingState);
			out->addInt("DebugDataVisible", DebugDataVisible);
			out->addBool("IsDebugObject", IsDebugObject);
		}


		//! 读取这个场景节点的属性
		/** 实现从你脚本语言，编辑器，debugger或xml反序列化等设置设置属性
		\param in 要读取属性容器
		\param options 可能会影响反序列化的附加选项 */
		virtual void deserializeAttributes(IAttributes* in,  SAttributeReadWriteOptions* options = 0)
		{
			if (!in)
				return;
			Name = in->getAttributeAsString("Name");
			ID = in->getAttributeAsInt("Id");

			setPosition(in->getAttributeAsVector3d("Position"));
			setRotation(in->getAttributeAsVector3d("Rotation"));
			setScale(in->getAttributeAsVector3d("Scale"));

			IsVisible = in->getAttributeAsBool("Visible");
			SINT32 tmpState = in->getAttributeAsEnumeration("AutomaticCulling",
				 AutomaticCullingNames);
			if (tmpState != -1)
				AutomaticCullingState = (UINT32)tmpState;
			else
				AutomaticCullingState = in->getAttributeAsInt("AutomaticCulling");

			DebugDataVisible = in->getAttributeAsInt("DebugDataVisible");
			IsDebugObject = in->getAttributeAsBool("IsDebugObject");

			updateAbsolutePosition();
		}

		//! 创建这个场景节点和它孩子的克隆
		/** \param newParent 新孩子节点
		\param newManager 新场景管理器
		\return 新的该节点的克隆节点 */
		virtual ISceneNode* clone(ISceneNode* newParent = 0, ISceneManager* newManager = 0)
		{
			return 0; // to be implemented by derived classes
		}

		//! 返回这个节点的场景管理器
		/** \return 这个节点的场景管理器 */
		virtual ISceneManager* getSceneManager(void) const { return SceneManager; }
	protected:

		//! 对于ISceneNode成员的克隆函数
		/** 这个方法用于派生类的克隆
		\param toCopyFrom 要复制值的节点
		\param newManager 新的场景管理器 */
		void cloneMembers(ISceneNode* toCopyFrom, ISceneManager* newManager)
		{
			Name = toCopyFrom->Name;
			AbsoluteTransformation = toCopyFrom->AbsoluteTransformation;
			RelativeTranslation = toCopyFrom->RelativeTranslation;
			RelativeRotation = toCopyFrom->RelativeRotation;
			RelativeScale = toCopyFrom->RelativeScale;
			ID = toCopyFrom->ID;
			setTriangleSelector(toCopyFrom->TriangleSelector);
			AutomaticCullingState = toCopyFrom->AutomaticCullingState;
			DebugDataVisible = toCopyFrom->DebugDataVisible;
			IsVisible = toCopyFrom->IsVisible;
			IsDebugObject = toCopyFrom->IsDebugObject;

			if (newManager)
				SceneManager = newManager;
			else
				SceneManager = toCopyFrom->SceneManager;

			// 克隆孩子

			ISceneNodeList::iterator it = toCopyFrom->Children.begin();
			for (; it != toCopyFrom->Children.end(); ++it)
				(*it)->clone(this, newManager);

			// 克隆animator

			ISceneNodeAnimatorList::iterator ait = toCopyFrom->Animators.begin();
			for (; ait != toCopyFrom->Animators.end(); ++ait)
			{
				ISceneNodeAnimator* anim = (*ait)->createClone(this, SceneManager);
				if (anim)
				{
					addAnimator(anim);
					anim->drop();
				}
			}
		}

		//! 为该节点和所有孩子节点设置新的场景管理器
		//! 当在场景管理器之间移动节点添加孩子时调用
		void setSceneManager(ISceneManager* newManager)
		{
			SceneManager = newManager;

			ISceneNodeList::iterator it = Children.begin();
			for (; it != Children.end(); ++it)
				(*it)->setSceneManager(newManager);
		}

		//! 节点名
		String Name;

		//! 绝对变换矩阵
		Matrix4 AbsoluteTransformation;

		//! 场景相对位移变换
		Vector3 RelativeTranslation;

		//! 场景相对旋转变换
		Vector3 RelativeRotation;

		//! 场景相对缩放变换
		Vector3 RelativeScale;

		//! 父节点指针
		ISceneNode* Parent;

		//! 孩子节点列表
		ISceneNodeList Children;

		//! 所有animator列表
		ISceneNodeAnimatorList Animators;

		//! 场景管理器指针
		ISceneManager* SceneManager;

		//! 三角形选择器指针
		ITriangleSelector* TriangleSelector;

		//! ID 
		SINT32 ID;

		//! 自动剔除状态
		UINT32 AutomaticCullingState;

		//! debug数据可见标志
		UINT32 DebugDataVisible;

		//! 节点是否可见
		bool IsVisible;

		//! 是否是debug对象
		bool IsDebugObject;
	};






}

#endif