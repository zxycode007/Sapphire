#ifndef _SAPPHIRE_IMESH_LOADER_
#define _SAPPHIRE_IMESH_LOADER_

#include "SapphireIReferenceCounter.h"
#include "SapphirePath.h"

namespace Sapphire
{
	class IReadFile;
	class IAnimatedMesh;


	//! �������Դ��ļ��м���һ����������
	/** �������Ҫ�������治֧�ֵĸ�ʽ����Ҫʵ�������MeshLoader��Ȼ��ͨ��ISceneManager::addExternalMeshLoader()
	��ӵ����档
    */
	class IMeshLoader : public virtual IReferenceCounter
	{
	public:

		 
		virtual ~IMeshLoader() {}

		//! �������ļ����Ա��������ط���true
		/** ����ͨ�������ļ���չ������
		\param filename Ҫ���Ե��ļ���
		\return �������ļ����Ա��������ط���true */
		virtual bool isALoadableFileExtension(const path& filename) const = 0;

		//! ��һ���ļ�����/����һ����������
		/** \param file Ҫ���ص��ļ����
		\return ָ�򴴽������ָ�룬�������ʧ�ܷ���0*/
		virtual IAnimatedMesh* createMesh(IReadFile* file) = 0;
	};

}


#endif