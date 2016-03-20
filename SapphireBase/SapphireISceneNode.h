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
	//  �����ڵ������ָ��
	typedef SharedPtr<ISceneNode> ISceneNodePtr;
	//typedef shared_ptr<ITriangleSelector> ITriangleSelectorPtr;
	//! ���峡���ڵ��б�
	typedef std::list<ISceneNode*> ISceneNodeList;
	//typedef std::list<ISceneNodePtr> ISceneNodeList;
	//! ���峡���ڵ㶯�����б�
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
			// ɾ�����еĺ��ӽڵ�
			removeAll();
			// ɾ�����е�animators
			ISceneNodeAnimatorList::iterator ait = Animators.begin();
			for (; ait != Animators.end(); ++ait)
			{
				//SAPPHIRE_DELETE(*ait);
				(*ait)->drop();
				 
			}
			//���������ѡ������Ϊ��
			if (TriangleSelector)
			{
				//TriangleSelector->drop();
				//SAPPHIRE_DELETE(TriangleSelector);
				TriangleSelector->drop();
			}
				
			
		};
		//! �Ƴ����еĳ����ڵ㺢�ӽڵ�
		/** 
		����������ڵ��ҳ������б���ɾ��
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



		//! ���������������������Ⱦ����֮ǰ����
		/** ͨ����������������ڵ������ε����ڼ佫����ע�ᵽ��Ⱦ�����У�Ԥ�ȼ��㽫Ҫ��Ⱦ�ļ����壬���ҷ�ֹ���ǵ��ӽڵ�������޳�����Ȼ��ע�ᡣ
		�����ʵ���Լ��ĳ������룬��Ӧ�����������ƵĴ��븲д�������
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


		//! OnAnimate() ��������������Ⱦ����֮ǰ����
		/** ����ڵ������洢�������������������õ����飬
		OnAnimate()������Ӧ�ö��������ӳ����ڵ���á��������ÿһ֡�������һ�Ρ����ܳ����ڵ�
		�Ƿ�ɼ�
		\param timeMs ��ǰʱ��ĺ�����*/
		virtual void OnAnimate(UINT32 timeMs)
		{
			if (IsVisible)
			{
				// ��������ڵ����еĶ�����animator

				ISceneNodeAnimatorList::iterator ait = Animators.begin();
				while (ait != Animators.end())
				{
					// ����Ѱ����һ���ڵ����animateNode()
					// ���������������animator���Դӳ����ڵ��Ƴ����Ǳ�������������������Ч
					ISceneNodeAnimator* anim = *ait;
					++ait;
					anim->animateNode(this, timeMs);
				}

				// ���¾���λ��
				updateAbsolutePosition();

				// �������ӽڵ�ҲӦ��OnAnimate

				ISceneNodeList::iterator it = Children.begin();
				for (; it != Children.end(); ++it)
					(*it)->OnAnimate(timeMs);
			}
		}


		//! ��Ⱦ����ڵ�
		virtual void render() = 0;


		//! ������������ڵ������
		/** \return �����ڵ���ַ����� */
		virtual const c8* getName() const
		{
			return Name.c_str();
		}


		//! ���ýڵ���ַ�����
		/** \param name �����ڵ�������� */
		virtual void setName(const c8* name)
		{
			Name = name;
		}


		//! ������������ڵ������
		/** \param name ��������Ľڵ�*/
		virtual void setName(const String& name)
		{
			Name = name;
		}


		//! ��ȡAABB���ӣ�����ת������ڵ�߽����
		/** ����ζ���������ڵ���һ��������3d��ɫ����һ�������ƶ�����ײ���ӽ����ջ�����
		ԭ�㡣��ȡ��������ʵ��������ϵ����Ҫͨ����getAbsoluteTransformation()����ת����������
		��ʹ��getTransformedBoundingBox()����Ҳһ��
		  */
		virtual const AxisAlignedBox& getBoundingBox() const = 0;


		//! ��ȡת������ʵ�������������ڵ��AABB
		/** \return ת�����AABB */
		virtual const AxisAlignedBox getTransformedBoundingBox() const
		{
			AxisAlignedBox box = getBoundingBox();
			//AbsoluteTransformation.transformBoxEx();
			box.transform(AbsoluteTransformation);
			return box;
		}


		//! ��ȡ�����ڵ�ľ����任λ�á���ÿһ��OnAnimate()�����ظ�����
		/** NOTE: �����ٶȵ�ԭ�򣬾��Ա任��ÿ��ʵ�ʱ任�ı����ͨ��һ�����ڵ�ı任ʱ�����Զ��ؼ����
		.��������ÿһ֡OnAnimate����ʱ�ĸ��£��������updateAbsolutePosition()ǿ�Ƹ���
		\return ���Ա任����. */
		virtual const Matrix4& getAbsoluteTransformation() const
		{
			return AbsoluteTransformation;
		}


		//! ������������ڵ�ʵ�ʱ任
		/** ���ʵ�ʱ任�Ǵ���3�������У� translation, rotation �� scale��Ҫȡ��ʵ�ʱ任����
		����������ֵ���㡣
		\return ʵ�ʵı任���� */
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


		//! ��������ڵ��Ƿ�ɼ�����������еĸ��׶��ɼ��Ļ���
		/** ��ֻ��ͨ���û���ѡ�����ã�������ʹ�ü����޳���
		\return ��������ڵ�����Ŀɼ��ԣ�true��ζ�ſɼ���������и��׽ڵ㶼�ɼ��Ļ���*/
		virtual bool isVisible() const
		{
			
			return IsVisible;
		}

		//! ��������ڵ��Ƿ���Ŀɼ��������������׵Ŀɼ���
		/** \return ����������еĸ��׶��ɼ�����true */
		virtual bool isTrulyVisible() const
		{
			
			if (!IsVisible)
				return false;
			//������׽ڵ�Ϊ�գ�˵��û�и��׽ڵ㣬���ؿɼ���
			if (!Parent)
				return true;
			//���׽ڵ㲻Ϊ�գ����ظ��׿ɼ���
			return Parent->isTrulyVisible();
		}

		//! ��������ڵ��Ƿ�ɼ��򲻿ɼ�
		/** ������Ϊfalse�����еĺ��ӽڵ㲻�ɼ������ɼ��ڵ������ײ����ı߽���ӷ�������Ч��
		\param isVisible ����ڵ�ɼ�����Ϊtrue */
		virtual void setVisible(bool isVisible)
		{
			IsVisible = isVisible;
		}


		//! ��ȡ�����ڵ�ID
		/**  
		\return  id. */
		virtual SINT32 getID() const
		{
			return ID;
		}


		//! ���ó����ڵ�ID
		/**  
		\param id ��ID. */
		virtual void setID(SINT32 id)
		{
			ID = id;
		}


		//! ���һ�����ӵ���������ڵ�
		/** �����������ڵ��Լ���һ�����ף������ȴ��������׽ڵ��Ƴ�
		\param child һ���º��ӽڵ� */
		virtual void addChild(ISceneNode* child)
		{
			//��Ϊ�գ�����ָ��Ķ�����ͬ
			if (child && (child != this))
			{
				// �Ƿ�ı䳡��������
				if (SceneManager != child->SceneManager)
					child->setSceneManager(SceneManager);

				child->grab();
				child->remove(); // �Ƴ�������ӽڵ���ǰ�ĸ���
				Children.push_back(child);
				child->Parent = this;
			}
		}




		//! �ӳ����ڵ��Ƴ�һ������
		/** ����ҵ���������б��������ָ�뱻�������������û���������ô��ڣ����ᱻɾ��
		\param child ��Ҫ���Ƴ��ĺ���ָ��
		\return ���������ӱ��Ƴ�����true�����û�з���false�� ��Ϊ�������ں����б��б��ҵ�
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


 


		//! �ӳ���ɾ������ڵ�
		/** �������ڵ�û������ã���ô�ᱻɾ����
		*/
		virtual void remove()
		{
			if (Parent)
				Parent->removeChild(this);
		}


		//! ���һ��animator����������ڵ㶯��
		/** \param animator һ����animatorָ�� */
		virtual void addAnimator(ISceneNodeAnimator* animator)
		{
			if (animator)
			{
				Animators.push_back(animator);
				animator->grab();
			}
		}


		//! ��ȡһ�����г����ڵ�animator���б�
		/** \return ����ڵ����������animator */
		const ISceneNodeAnimatorList& getAnimators() const
		{
			return Animators;
		}


		//! ����������ڵ��Ƴ�һ��animator
		/** ������animator���ҵ��������������û������þͻᱻɾ��
		\param animator һ��Ҫɾ��animator��ָ��. */
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


		//! �Ƴ���������ڵ����е�animator
		virtual void removeAnimators()
		{
			ISceneNodeAnimatorList::iterator it = Animators.begin();
			for (; it != Animators.end(); ++it)
				(*it)->drop();

			Animators.clear();
		}


		//! ���ػ���0��ʼ������i����
		/** ��ȡ����ڵ�ʹ�õĲ�����������getMaterialCount().
		���ڲ���ڵ㵽������νṹ��һ�����Ż���λ��ʹ�ö���Ⱦ״̬����С�ĸı䣬�����ܹ�����ֱ���޸�
		��������ڵ�Ĳ���
		\param num ����0���������ֵ��getMaterialCount() - 1.
		\return �������� */
		virtual  SMaterial& getMaterial(UINT32 num)
		{
			return  IdentityMaterial;
		}


		//! ��ȡ��������ڵ�Ĳ�������
		/** \return ��ǰ�����Ĳ������� */
		virtual UINT32 getMaterialCount() const
		{
			return 0;
		}


		//! һ���������в��ʱ�־��һ����ֵ
		/**  ���� ���磺�������Ҫ���������ֶ���Դ��Ӱ��
		\param flag ���в���Ҫ���õı�־
		\param newvalue �����־����ֵ */
		void setMaterialFlag( E_MATERIAL_FLAG flag, bool newvalue)
		{
			for (UINT32 i = 0; i<getMaterialCount(); ++i)
				getMaterial(i).setFlag(flag, newvalue);
		}


		//! ��������������ڵ����Բ��ʵ�ָ��������һ��������
		/** \param textureLayer Ҫ���õ�����㣬����С��MATERIAL_MAX_TEXTURES
		\param texture ������. */
		void setMaterialTexture(UINT32 textureLayer, ITexture* texture)
		{
			if (textureLayer >=  MATERIAL_MAX_TEXTURES)
				return;

			for (UINT32 i = 0; i<getMaterialCount(); ++i)
				getMaterial(i).setTexture(textureLayer, texture);
		}


		//! ������������ڵ����в��ʵĲ������͵�һ��������
		/** \param newType Ҫ�����²������� */
		void setMaterialType( E_MATERIAL_TYPE newType)
		{
			for (UINT32 i = 0; i<getMaterialCount(); ++i)
				getMaterial(i).MaterialType = newType;
		}


		//! ��ȡ����ڵ�����ڸ��ڵ������ֵ
		/** �������������ڵ������ֵ���������Ҫ�������ţ���getAbsoluteTransformation().getScale()
		\return ������������ڵ������ֵ */
		virtual const Vector3& getScale() const
		{
			return RelativeScale;
		}


		//! ������������ڵ���������ֵ
		/** \param scale �µ�����ֵ������ڸ��ڵ� */
		virtual void setScale(const Vector3& scale)
		{
			RelativeScale = scale;
		}


		//! ��ȡ��Ը��ڵ����ת�Ƕ�
		/** ע�⣺���������ת�Ƕȡ��������Ҫ������ת����getAbsoluteTransformation().getRotation()
		\return ��ǰ�����ڵ������ת*/
		virtual const Vector3& getRotation() const
		{
			return RelativeRotation;
		}


		//! ��������ڸ��ڵ����ת�Ƕ�
		/** �����޸�����ڵ������ת
		\param rotation �µ���ת�Ƕ� */
		virtual void setRotation(const Vector3& rotation)
		{
			RelativeRotation = rotation;
		}


		//! ��ȡ��ڵ������ڸ��ڵ��λ��
		/** ע�⣺���������������ڵ�λ�á��������Ҫ�����������λ�ã� ��getAbsolutePosition()����
		\return ���ص�ǰ�ڵ�����������ڵ� */
		virtual const Vector3& getPosition() const
		{
			return RelativeTranslation;
		}


		//! ��������ڸ��ڵ��λ��
		/** ע�⣺���������������ڵ�λ��
		\param newpos ��������ڵ�������λ�� */
		virtual void setPosition(const Vector3& newpos)
		{
			RelativeTranslation = newpos;
		}


		//! ��ȡ����ڵ�����������ϵ�ľ���λ��
		/** �������Ҫ����ڵ�����ڸ��ڵ�����λ���� getPosition()
		ע�⣺�����ٶ�ԭ�򣬾���λ����ÿһ�����λ�øı����ͨ��һ�����ڵ��λ�øı䲻���Զ��ؼ���ġ�
		��ÿһ֡OnAnimate�и����Ǿ��������ģ������ǿ�Ƶ���updateAbsolutePosition()ǿ�Ƹ���
		\return ���ص�ǰ�ڵ�ľ���λ�ã����һ�θ����ڵ���updateAbsolutePosition��*/
		virtual Vector3 getAbsolutePosition() const
		{
			return AbsoluteTransformation.getTrans();
			
		}


		//! �򿪺͹رջ���AABB���Զ��޳�
		/** Ĭ���Զ��޳��ǿ����ġ�ע�⣺�������г����ڵ㶼֧���޳���ĳЩ
		�ڵ�����ĳЩ����ԭ����ڣ�����˲����ڵ�
		\param state ʹ�õ��޳�״̬ */
		void setAutomaticCulling(UINT32 state)
		{
			AutomaticCullingState = state;
		}


		//! ��ȡ�Զ��޳���״̬
		/** \return �����Զ��޳���״̬ */
		UINT32 getAutomaticCulling() const
		{
			return AutomaticCullingState;
		}


		//! ����debug��������AABB���ӿɼ�
		/** һ����E_DEBUG_SCENE_TYPE���͵�λ��
		��ע�⣺�������г����ڵ㶼֧�����е�debug��������
		\param state Ҫʹ�õ�debug���ݿ��ӻ�״̬*/
		virtual void setDebugDataVisible(UINT32 state)
		{
			DebugDataVisible = state;
		}

		//! ���ؿ��ӵ�debug���ݣ���AABB���ӿɼ���
		/** \return һ����E_DEBUG_SCENE_TYPE���͵�λ��
		  */
		UINT32 isDebugDataVisible() const
		{
			return DebugDataVisible;
		}


		//! ������������ڵ��Ƿ���һ��debug����
		/** debug������Щ�������ԣ����������ܹ��������ų���ײ�������л� */
		void setIsDebugObject(bool debugObject)
		{
			IsDebugObject = debugObject;
		}


		//! ����������������Ƿ���һ��debug����
		/** debug������Щ�������ԣ����������ܹ��������ų���ײ�������л� 
		\return �������ڵ���һ��debug���󷵻�true */
		bool isDebugObject() const
		{
			
			return IsDebugObject;
		}


		//! �������к��ӵĳ�����
		/** \return �˽ڵ�����к��ӽڵ�*/
		const ISceneNodeList& getChildren() const
		{
			return Children;
		}


		//! �ı���������ڵ�ĸ��ڵ�
		/** \param newParent T�µĸ��ڵ� */
		virtual void setParent(ISceneNode* newParent)
		{
			grab();
			remove();

			Parent = newParent;

			if (Parent)
				Parent->addChild(this);

			drop();
		}


		//! ����������������������ѡ����
		/** �������ISceneManager::createTriangleSelector()��ISceneManager::createOctreeTriangleSelector()
		����һ��������ѡ����TriangleSelector��ĳЩ�ڵ�Ĭ�Ͼ��ܴ��������ӽڵ�ѡ������ ��������Ҫ����ISceneNode::getTriangleSelector().�������ڵ�
		�Ƿ��Ѿ���һ�������νڵ�
		\return һ��������ѡ������ָ�룬���0����û�� */
		virtual ITriangleSelector* getTriangleSelector() const
		{
			return TriangleSelector;
		}


		//! ��������ڵ�������ѡ����TriangleSelector
		/** ���ѡ��������������ײ��⡣
		�������ISceneManager::createTriangleSelector()��ISceneManager::createOctreeTriangleSelector()
		����һ��������ѡ����TriangleSelector��ĳЩ�ڵ�Ĭ�Ͼ��ܴ��������ӽڵ�ѡ������ ��������Ҫ����ISceneNode::getTriangleSelector().�������ڵ�
		�Ƿ��Ѿ���һ�������νڵ�
		\param selector ��������ڵ��µ�������ѡ���� */
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


		//! ������������ڵ�λ�ø��¾���λ��
		/** ע�⣺�ⲻ��ݹ�������Ǹ��ڵ�ľ���λ�ã����ԣ����Ҫ��������Ҫ�������ڵ����
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


		//! ������������ڵ�ĸ��ڵ�
		/** \return ���ڵ��ָ�� */
		ISceneNode* getParent() const
		{
			return Parent;
		}


		//! ���ؽڵ�����
		/** \return ����ڵ����� */
		virtual ESCENE_NODE_TYPE getType() const
		{
			return ESNT_UNKNOWN;
		}


		//! д��������ڵ������
		/** ʵ�����չʾ��������ڵ�����Ը��ű����ԣ��༭����debugger��xml�����л�Ŀ��
		\param out Ҫд�����������
		\param options ���ܻ�Ӱ�����л��ĸ���ѡ�� */
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


		//! ��ȡ��������ڵ������
		/** ʵ�ִ���ű����ԣ��༭����debugger��xml�����л���������������
		\param in Ҫ��ȡ��������
		\param options ���ܻ�Ӱ�췴���л��ĸ���ѡ�� */
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

		//! ������������ڵ�������ӵĿ�¡
		/** \param newParent �º��ӽڵ�
		\param newManager �³���������
		\return �µĸýڵ�Ŀ�¡�ڵ� */
		virtual ISceneNode* clone(ISceneNode* newParent = 0, ISceneManager* newManager = 0)
		{
			return 0; // to be implemented by derived classes
		}

		//! ��������ڵ�ĳ���������
		/** \return ����ڵ�ĳ��������� */
		virtual ISceneManager* getSceneManager(void) const { return SceneManager; }
	protected:

		//! ����ISceneNode��Ա�Ŀ�¡����
		/** �����������������Ŀ�¡
		\param toCopyFrom Ҫ����ֵ�Ľڵ�
		\param newManager �µĳ��������� */
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

			// ��¡����

			ISceneNodeList::iterator it = toCopyFrom->Children.begin();
			for (; it != toCopyFrom->Children.end(); ++it)
				(*it)->clone(this, newManager);

			// ��¡animator

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

		//! Ϊ�ýڵ�����к��ӽڵ������µĳ���������
		//! ���ڳ���������֮���ƶ��ڵ���Ӻ���ʱ����
		void setSceneManager(ISceneManager* newManager)
		{
			SceneManager = newManager;

			ISceneNodeList::iterator it = Children.begin();
			for (; it != Children.end(); ++it)
				(*it)->setSceneManager(newManager);
		}

		//! �ڵ���
		String Name;

		//! ���Ա任����
		Matrix4 AbsoluteTransformation;

		//! �������λ�Ʊ任
		Vector3 RelativeTranslation;

		//! ���������ת�任
		Vector3 RelativeRotation;

		//! ����������ű任
		Vector3 RelativeScale;

		//! ���ڵ�ָ��
		ISceneNode* Parent;

		//! ���ӽڵ��б�
		ISceneNodeList Children;

		//! ����animator�б�
		ISceneNodeAnimatorList Animators;

		//! ����������ָ��
		ISceneManager* SceneManager;

		//! ������ѡ����ָ��
		ITriangleSelector* TriangleSelector;

		//! ID 
		SINT32 ID;

		//! �Զ��޳�״̬
		UINT32 AutomaticCullingState;

		//! debug���ݿɼ���־
		UINT32 DebugDataVisible;

		//! �ڵ��Ƿ�ɼ�
		bool IsVisible;

		//! �Ƿ���debug����
		bool IsDebugObject;
	};






}

#endif