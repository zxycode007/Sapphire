#ifndef _SAPPHIRE_USER_DATA_SERIALIZER
#define _SAPPHIRE_USER_DATA_SERIALIZER

class IAttributes;
class ISceneNode;

namespace Sapphire
{
	//! �ӱ����ļ���ȡ�û����ݵĽӿ�
	/** ����ӿ����û���ʵ�֣� ʹ����ͨ��ISceneManager����д�û����ݣ�����������ISceneManager::loadScene()��ISceneManager::saveScene()
	*/
	class ISceneUserDataSerializer
	{
	public:

		virtual ~ISceneUserDataSerializer() {}

		//! ����������������һ�������ڵ����һ���ļ�ʱ����
		virtual void OnCreateNode(ISceneNode* node) = 0;

		//! ��������������ȡһ�������ڵ����һ���ļ�ʱ����C
		/** �û����ݵ�ָ�����һ���û����ݵ������б����ڶ�ȡ�ĳ����ļ��й����������ڵ�*/
		virtual void OnReadUserData(ISceneNode* forSceneNode,  IAttributes* userData) = 0;

		//! �����������������д��һ�������ڵ㣨���絽һ��XML�ļ���ʱ����
		/** ʵ��������������ҷ���һ�������û����ݵ�����Ҫ�ͳ����ڵ�һ�𱣴�������б�
		 ע�⣺������������������drop������һ��ָ��󣬱㲻����Ҫ���� �����㲻��Ϊ�������ֵ����һ�������ҷ���һ�����ڴ��ڵ�IAttribute���� �ڷ��ؼ򵥵���grab������
		 */
		virtual IAttributes* createUserData(ISceneNode* forSceneNode) = 0;
	};
}

#endif