#ifndef _SAPPHIRE_I_CAMERA_SCENE_NODE_
#define _SAPPHIRE_I_CAMERA_SCENE_NODE_

#include "SapphireISceneNode.h"
#include "SapphireIEventRecevier.h"

namespace Sapphire
{
	struct SViewFrustum;

	//! ��������ĳ����ڵ�
	/** ���������������۲���λ����������Ⱦ������ICameraScenNode��һ�������ڵ㣬���ܹ������ӵ��κ�����
	�����ĳ����ڵ㣬���ҽ��������ĸ��ڵ��ƶ�����ת
	*/
	class ICameraSceneNode : public ISceneNode, public IEventReceiver
	{
	public:

		//! ������
		ICameraSceneNode(ISceneNode* parent, ISceneManager* mgr, SINT32 id,
			const Vector3& position = Vector3(0, 0, 0),
			const Vector3& rotation = Vector3(0, 0, 0),
			const Vector3& scale = Vector3(1.0f, 1.0f, 1.0f))
			: ISceneNode(parent, mgr, id, position, rotation, scale), IsOrthogonal(false) {}

		//! ���������ͶӰ����
		/** �������м�������ͶӰ����ķ���
		Matrix4::buildProjectionMatrixPerspectiveFovLH.
		ע����������һֱ����������������ã�ֱ�����з���������: setNearValue,
		setFarValue, setAspectRatio, setFOV.
		\param projection ��������ͶӰ����
		\param isOrthogonal ������������������������Ϊtrue
		orthogonal one (e.g. ���� matrix4::buildProjectionMatrixOrtho).
		*/
		virtual void setProjectionMatrix(const Matrix4& projection, bool isOrthogonal = false) = 0;

		//! ��ȡ��ǰ�����ͶӰ����
		/** \return ��ǰ�����ͶӰ���� */
		virtual const Matrix4& getProjectionMatrix() const = 0;

		//! ��ȡ��ǰ����Ĺ۲����
		/** \return ��ǰ����Ĺ۲���� */
		virtual const Matrix4& getViewMatrix() const = 0;

		//! ����һ����ǰ�۲����Affector
		/** ���ݹ����ľ��󣬵���Ҫȡ�ø���ʱ������һ���۲������ˣ��������Զ����������
		���磬һ���������
		\param affector affector ����. */
		virtual void setViewMatrixAffector(const Matrix4& affector) = 0;

		//! ��ȡ�Զ���Ĺ۲���� affector.
		/** \return affector ����. */
		virtual const Matrix4& getViewMatrixAffector() const = 0;

		//! ���Է������Ͱ����¼������
		/** ���������ܹ�����������룬����������ڵ������������´���ISceneManager::addCameraSceneNodeMaya
		ISceneManager::addCameraSceneNodeFPS������ȡ�ö������ı�λ�ú͹۲�Ŀ�������
		 */
		virtual bool OnEvent(const SEvent& event) = 0;

		//! ��������Ĺ۲�Ŀ��
		/** ��������Ŀ�����ת�ǰ󶨵ģ���bindTargetAndRotation()������ô����������ı�
		��������ڵ���תȥƥ�����Ŀ�ꡣ
		ע�⣺setTarget�ڲ��õ��ǵ�ǰ����λ�ã���������ı�����һ����Ⱦ��setPosition�������������֮ǰ��������updateAbsolutePosition
		
	 
		\param pos ����۲��Ŀ�꣬����������ϵ������*/
		virtual void setTarget(const Vector3& pos) = 0;

		//! ���ýڵ����ת ����setTarget�෴��
		/** ���ֵ�޸�����ڵ���ص���ת
		����ཻĿ�����ת���󶨣���bindTargetAndRotation()������ô��ε���Ҳ��ı�
		���Ŀ����ƥ�������ת
		\param rotation ����ڵ�����ת�ĽǶ�*/
		virtual void setRotation(const Vector3& rotation) = 0;

		//! ��ȡ��ǰ����۲��Ŀ��
		/** \return ��ǰ����۲��Ŀ�����������*/
		virtual const Vector3& getTarget() const = 0;

		//! ���������up����
		/** \param pos: �������up����*/
		virtual void setUpVector(const Vector3& pos) = 0;

		//! ��ȡ�����up����
		/** \return ����������up��������������*/
		virtual const Vector3& getUpVector() const = 0;

		//! ��ȡ�������Ľ���ƽ���ֵ
		/** \return ����Ľ���ƽ���ֵ */
		virtual Real getNearValue() const = 0;

		//! ��ȡ������Զ��ƽ���ֵ
		/** \return ���Զ��ƽ���ֵ */
		virtual Real getFarValue() const = 0;

		//! ��ȡ����ĺ��ݱ�
		/** \return ����ĺ��ݱ� */
		virtual Real getAspectRatio() const = 0;

		//! ��ȡ������������
		/** \return ��������򣨻��ȣ� */
		virtual Real getFOV() const = 0;

		//! ���ý���ƽ���ֵ��Ĭ��1.0f��
		/** \param zn: �µ������zֵ*/
		virtual void setNearValue(Real zn) = 0;

		//! ����Զ��ƽ���ֵ ��Ĭ��:2000f��
		/** \param zf: ����Զzֵ */
		virtual void setFarValue(Real zf) = 0;

		//! ��������ݺ��(Ĭ��: 4.0f / 3.0f)
		/** \param aspect: �µĺ��ݱ� */
		virtual void setAspectRatio(Real aspect) = 0;

		//! ����fov (Ĭ��: PI / 2.5f)
		/** \param fovy: �µ�fov���� */
		virtual void setFOV(Real fovy) = 0;

		//! ��ȡ��׶��
		/** ��ʱ����ͨ��BSP����LOD��Ⱦ�ڵ�
		\return ��ǰ��׶��*/
		virtual const SViewFrustum* getViewFrustum() const = 0;

		//! �رջ����������ȡ��������������
		/** �������Ϊtrue������������Ӧ���԰�������������*/
		virtual void setInputReceiverEnabled(bool enabled) = 0;

		//! ������������������Ƿ��Ǵ򿪵�
		virtual bool isInputReceiverEnabled() const = 0;

		//! �������ͷ���������
		virtual bool isOrthogonal() const
		{
			//_IRR_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX;
			return IsOrthogonal;
		}

		//! �������������ڵ����ת������Ŀ��λ�ã�����ȡ����
		/** ���󶨺󣬵���setRotation����Z��������������Ŀ��λ�ã�ͬ�����setTarget����
		������������ת����������+Z�Ὣָ��Ŀ��㡣FPS���Ĭ�������ְ󶨣����������������.
		\param bound true�Ļ�����������ڵ����ת��Ŀ�꣬falseȡ������֮���
		@�� getTargetAndRotationBinding() */
		virtual void bindTargetAndRotation(bool bound) = 0;

		//! ��ѯ�����������ڵ����ת���ҵ�Ŀ��λ���Ƿ�һ���
		/** @�� bindTargetAndRotation() */
		virtual bool getTargetAndRotationBinding(void) const = 0;

		//! ���л�д���������ڵ������
		virtual void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options = 0) const
		{
			ISceneNode::serializeAttributes(out, options);

			if (!out)
				return;
			out->addBool("IsOrthogonal", IsOrthogonal);
		}

		//! ��ȡ�������ڵ������ֵ
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