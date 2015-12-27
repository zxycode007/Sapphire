#ifndef _SAPPHIRE_I_SCENE_LOADER_
#define _SAPPHIRE_I_SCENE_LOADER_

#include "SapphireIReferenceCounter.h"
#include "SapphirePath.h"

namespace Sapphire
{
	class IReadFile;
	class ISceneNode;
	class ISceneUserDataSerializer;

	//! ������ܹ�����һ��������������������
	/** �������Ҫ������ص�ǰ��֧�ֵĳ����ļ���ʽ����ôʵ������ӿڣ�������ISceneManager::addExternalSceneLoader()��������SceneLoader������  */
	class ISceneLoader : public virtual IReferenceCounter
	{
	public:

		//! ����������Լ�������ļ�������true
		/** ����ͨ���ļ���չ����������
		\param filename Ҫ���Ե��ļ���
		\return ����ļ���չ����֧�ֵ����ͷ���true */
		virtual bool isALoadableFileExtension(const path& filename) const = 0;

		//! ����������Լ�������ļ�������true
		/**  
		\param file Ҫ���Ե��ļ�
		\return ����ļ���չ����֧�ֵ����ͷ���true */
		virtual bool isALoadableFileFormat(IReadFile* file) const = 0;

		//! ���س�����������������
		/** \param file ��������������ļ�
		\param userDataSerializer: ���Ҫ�����ļ��е��û����ݣ�������ĳЩ�����ڵ㣬ʵ��ISceneUserDataSerializer
		�ӿڲ����ṩ����Ϊ����������ָ���������Ϊ0
		\param rootNode Ҫ���س����ĸ��ڵ㣬���û���ṩ����ô��������������ص����ڵ�
		\return ����ɹ�����true��ʧ�ܷ���false*/
		virtual bool loadScene(IReadFile* file, ISceneUserDataSerializer* userDataSerializer = 0,
			ISceneNode* rootNode = 0) = 0;

	};
}

#endif