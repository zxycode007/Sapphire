#ifndef _SAPPHIRE_I_ANIMATED_MESH_SCENE_NODE_
#define _SAPPHIRE_I_ANIMATED_MESH_SCENE_NODE_

#include "SapphirePrerequisites.h"
#include "SapphireIReferenceCounter.h"
#include "SapphireVector3.h"
#include "SapphireIAnimatedMesh.h"
#include "SapphireISceneNode.h"
#include "SapphireIBoneSceneNode.h"
#include "SapphireIAnimatedMeshMD2.h"



namespace Sapphire
{
	class IShadowVolumeSceneNode;

	enum E_JOINT_UPDATE_ON_RENDER
	{
		//!ʲô������
		EJUOR_NONE = 0,

		//! ��ȡ��������joint positions �����ڸ��ӽڵ㣩
		EJUOR_READ,

		//! ��������������joint positions (eg. ������,���Ǵ�animateJoint���ö���animation )
		EJUOR_CONTROL
	};


	class IAnimatedMeshSceneNode;

	//! ������ֹ��animation�������¼���׽�Ļص��ӿ�
	/** ʵ������ӿڲ���IAnimatedMeshSceneNode::setAnimationEndCallback��������һ�������Ƿ񲥷����
	**/
	class IAnimationEndCallBack : public virtual IReferenceCounter
	{
	public:

		//!������������ϵ�ʱ��ᱻ����
		/** �� IAnimatedMeshSceneNode::setAnimationEndCallback ������Ϣ
		\param node: �������������ϵĽڵ� */
		virtual void OnAnimationEnd(IAnimatedMeshSceneNode* node) = 0;
	};

	//! �ܹ���ʾһ��������������Ӱ�ĳ����ڵ�
	/** ��Ӱ�ǿ�ѡ�ģ������Ӱ������ʾ����ָ��IAnimatedMeshSceneNode::createShadowVolumeSceneNode().*/
	class IAnimatedMeshSceneNode : public ISceneNode
	{
	public:

		IAnimatedMeshSceneNode(ISceneNode* parent, ISceneManager* mgr, SINT32 id,
			const Vector3& position = Vector3(0, 0, 0),
			const Vector3& rotation = Vector3(0, 0, 0),
			const Vector3& scale = Vector3(1.0f, 1.0f, 1.0f))
			: ISceneNode(parent, mgr, id, position, rotation, scale) {}

		virtual ~IAnimatedMeshSceneNode() {}

		//! ���õ�ǰ֡��֡��
		/** �����ڿ�ʼ����������һ֡��ʼ����
		\param frame: �ö�������һ֡��ʼ���ŵ�֡����
		���֡�ű����Ƕ�����������ڵ��IMesh��Ч��֡�š�����ϸ�ڼ�IAnimatedMesh::getMesh() */
		virtual void setCurrentFrame(Real frame) = 0;

		//! ����ѭ��������֮���֡����
		/** Ĭ�������0 - MaximalFrameCount
		\param begin: ѭ����ʼ֡
		\param end: ѭ������֡
		\return ����ɹ�����true*/
		virtual bool setFrameLoop(SINT32 begin, SINT32 end) = 0;

		//! ���ö��������ٶ�
		/** \param framesPerSecond: ÿ�벥��֡�� */
		virtual void setAnimationSpeed(Real framesPerSecond) = 0;

		//! ��ȡ����������ŵ��ٶ�
		/** \return ÿ�벥��֡�� */
		virtual Real getAnimationSpeed() const = 0;

		//! ������Ӱ�ݻ������ڵ���Ϊ����ڵ���ӽڵ�
		/** 
		�����Ӱ������ZPass��Zfail������Ⱦ��ZPassҪ�Կ�һ�㣬���ڴ�����Ӱ�ݻ�������׵㣬
		������������ڵ��������Ӱ�ݻ��������ֿ�������ª�Ķ�������Щ������ZFail��������
		������
		\param shadowMesh: ��ѡ��  ������Ӱ�ݻ��Զ�������
		\param id: ��Ӱ�����ڵ��ID��  ���id�����Ժ��ʶ����ڵ�
		\param zfailmethod: �������Ϊtrue�������Ӱ��ʹ��zfail����������ʹ��zpass
		\param infinity: ��Ӱ�ݻ��㷨���������Ӱ�ݻ���ֵ(����zfail ��Ӱ�ݻ�������ֻ֧��finite ��Ӱ
		���������zfar������������Ӱ��back cap����������infinity����)
		\return ��������Ӱ�ڵ��ָ�롣 
		 */
		virtual IShadowVolumeSceneNode* addShadowVolumeSceneNode(const IMesh* shadowMesh = 0,
			SINT32 id = -1, bool zfailmethod = true, Real infinity = 1000.0f) = 0;


		//! ��ȡһ��ָ������������һ��joint(������������һ�����ڹ���������).
		/** 
		ͨ����������������Ը��ӳ����ڵ㵽joint ���磺�������һ��������һ������ģ�͵����֡�
		����
		\code
		ISceneNode* hand =
		yourAnimatedMeshSceneNode->getJointNode("LeftHand");
		hand->addChild(weaponSceneNode);
		\endcode
		��ע�⣺���������������ص�joint����ε���֮ǰ���ܻ᲻���ڣ�����
		������ڵ����joint��ͨ��������
		\param jointName: joint������
		\return ָ��ͨ��ָ�����ֱ�ʾ���joint�ĳ����ڵ�,����������������
		û��һ����Ƥ�������������ֵ�joint��û���ҵ�
		 */
		virtual IBoneSceneNode* getJointNode(const c8* jointName) = 0;

		//! ��getJointNode(const c8* jointName)һ��, ���ǻ���ID
		virtual IBoneSceneNode* getJointNode(UINT32 jointID) = 0;

		//! ��ȡjoint������
		/** \return �������joint������*/
		virtual UINT32 getJointCount() const = 0;

		//! ��ʼһ��Ĭ�ϵ�MD2����
		/** �������������ڵ��������һ��MD2����Ļ���ͨ��������������������׵Ŀ�ʼһ��Run��Attack��Die�������Ķ�����
		����ʲô�����ᷢ����
		\param anim: һ��MD2 �������ͣ������Ա����ţ�����վ������EMAT_STAND
		\return ����ɹ�����true�����򷵻�false */
		virtual bool setMD2Animation(EMD2_ANIMATION_TYPE anim) = 0;

		//! ��ʼһ���ر��MD2����
		/** 
		�������������ڵ��������һ��MD2����Ļ���ͨ��������������������׵Ŀ�ʼһ��Run��Attack��Die�������Ķ�����
		����ʲô�����ᷢ����
		���������һ���ַ����������������,������������һ����׼��MD2�������������EMD2_ANIMATION_TYPEö�ٴ���
		\param animationName: Ҫ���ŵĶ�����
		\return ����ɹ�����true�����򷵻�false */
		virtual bool setMD2Animation(const c8* animationName) = 0;

		//! ���ص�ǰ��ʾ��֡��
		virtual Real getFrameNr() const = 0;
		//! ���ص�ǰ��ʼ֡��
		virtual SINT32 getStartFrame() const = 0;
		//! ���ص�ǰ����֡��
		virtual SINT32 getEndFrame() const = 0;

		//! ����Ĭ�ϵ�ѭ��ģʽ
		/** �������Ϊfalse������������ѭ������*/
		virtual void setLoopMode(bool playAnimationLooped) = 0;

		//! ���ص�ǰ��ѭ��ģʽ
		/** ������ѭ�����ŷ���true */
		virtual bool getLoopMode() const = 0;

		//! �����ڶ����ط����ʱ���õĻص��ӿ�
		/** ����Ϊ0�ر�֮��ص�
		��ע��:��ֻ���ڷ�ѭ��ģʽ�²Żᱻ����
		��IAnimatedMeshSceneNode::setLoopMode(). */
		virtual void setAnimationEndCallback(IAnimationEndCallBack* callback = 0) = 0;

		//! ������������ڵ㲻�����������Ĳ����Ƿ���ֻ��ģʽʹ������
		/** ����������ܻ�ı�һ������Ĳ��ʣ�������������������������ڵ㷢���ı䡣
		\param readonly ������������Ƿ�Ӧ��ֻ�� */
		virtual void setReadOnlyMaterials(bool readonly) = 0;

		//! ��ⳡ���ڵ㲻����������ʣ����Ƿ���ֻ��ģʽʹ�����ǡ�
		/** �����־����ͨ��setReadOnlyMaterials()����
		\return ��������Ƿ�ֻ�� */
		virtual bool isReadOnlyMaterials() const = 0;

		//! ����һ��������
		virtual void setMesh(IAnimatedMesh* mesh) = 0;

		//! ���ص�ǰ����
		virtual IAnimatedMesh* getMesh(void) = 0;

		//! ������������һ��MD3���񣬻�һ���ر�MD3���Ա任�ı�ǩ��������һ�����������ڵ㣬���ؾ��Ա任
		virtual const SMD3QuaternionTag* getMD3TagTransformation(const String & tagname) = 0;

		//! ��������Ⱦ��jointӦ����������  ��Joint����ģʽ��
		virtual void setJointMode(E_JOINT_UPDATE_ON_RENDER mode) = 0;

		//! ���������ת��ʱ��
		/** 
		����Ҫ����joint����������Jointģʽ��EJUOR_CONTROL����������animateJoints(),
		����������񲻻�
		 */
		virtual void setTransitionTime(Real Time) = 0;

		//! ���ڵ�ǰ֡���������������joint
		/** ����Ҫ˵���任 */
		virtual void animateJoints(bool CalculateAbsolutePositions = true) = 0;

		//! ���ӱ任��Ⱦ���� 
		/** �޳�����Ӱ�� */
		virtual void setRenderFromIdentity(bool On) = 0;

		//! ����һ�������ڵ�������ӵĿ�¡
		/** \param newParent һ����ѡ�ĸ��ڵ�
		\param newManager һ����ѡ�ĳ���������
		\return ����ڵ���¿�¡ */
		virtual ISceneNode* clone(ISceneNode* newParent = 0, ISceneManager* newManager = 0) = 0;

	};
}

#endif