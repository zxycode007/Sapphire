#ifndef _SAPPHIRE_I_LIGHT_SCENE_NODE_
#define _SAPPHIRE_I_LIGHT_SCENE_NODE_

#include "SapphireISceneNode.h"
#include "SapphireSLight.h"

namespace Sapphire
{
	//! ��Ϊһ����̬��Դ�ĳ����ڵ�
	/** �����ͨ�����Ŀɼ�ѡ���л���Դ���ء� ���ܹ�ͨ����ͨ�ĳ����ڵ�animatior��������
	�����Դ������ƽ�й���ߵ��Դ����Դ����ͨ�������ڵ����ת�������(�ٶ�(0,0,1)Ϊ��Դ�ı��ط���)
	*/
	class ILightSceneNode : public ISceneNode
	{
	public:

		 
		ILightSceneNode(ISceneNode* parent, ISceneManager* mgr, SINT32 id,
			const Vector3& position = Vector3(0, 0, 0))
			: ISceneNode(parent, mgr, id, position) {}

		//! ���������ILightSceneNode��صĹ�Դ����
		/** \param light �µĹ�Դ���� */
		virtual void setLightData(const SLight& light) = 0;

		//! ��ȡ�����ILightSceneNode��صĹ�Դ����
		/** \return ��Դ������ */
		virtual const SLight& getLightData() const = 0;

		//! ��ȡ�����ILightSceneNode��صĹ�Դ����
		/** \return ��Դ������. */
		virtual SLight& getLightData() = 0;

		//! ��������ڵ��Ƿ�ɼ�
		/** ����Ϊtrue�����е��ӽڵ㽫���ɼ�
		\param isVisible ����ڵ㲻�ɼ� */
		virtual void setVisible(bool isVisible) = 0;

		//! ���ù�Դ��Ӱ��뾶
		/** ������뾶֮�⣬��Դ���԰뾶��ļ�������й��ղ��Ҳ���Ͷ����Ӱ��
		���ð뾶��Ӱ��˥������������(0,1/radius,0)���������Ҫ��д�������ð뾶֮������˥��
		\param radius �°뾶 */
		virtual void setRadius(Real radius) = 0;

		//! ��ȡ��Դ��Ӱ��뾶
		/** \return ��ǰ�뾶 */
		virtual Real getRadius() const = 0;

		//! ���ù�Դ������
		/** \param type �µ����� */
		virtual void setLightType(E_LIGHT_TYPE type) = 0;

		//! ��ȡ��Դ������
		/** \return ����Դ���� */
		virtual E_LIGHT_TYPE getLightType() const = 0;

		//! ���������Դ�Ƿ�Ͷ����Ӱ
		/** �������־�������Զ�Ͷ����Ӱ����Щ������Ȼ��Ҫ�����Ӱ�����ڵ㡣
		�򿪺͹ر�ֱ�ӹ�Դ����Ӱ͸�������ܵ�����
		\param shadow True ���Ϊtrue����Դ����Ͷ����Ӱ */
		virtual void enableCastShadow(bool shadow = true) = 0;

		//! ����Դ��Ӱ͸���Ƿ��
		/** \return ���Ϊtrue���˹�ԴͶ����Ӱ */
		virtual bool getCastShadow() const = 0;
	};
}

#endif