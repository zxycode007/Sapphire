#ifndef _SAPPHIRE_IMESH_CACHE_
#define _SAPPHIRE_IMESH_CACHE_

#include "SapphirePrerequisites.h"
#include "SapphirePath.h"
#include "SapphireIReferenceCounter.h"

namespace Sapphire
{
	class IMesh;
	class IAnimatedMesh;
	class IAnimatedMeshSceneNode;
	class IMeshLoader;

	//! ������񻺴汣���Ѿ����ص����񣬲��Ҹ������ṩһ���ӿ�
	/** ������ISceneManager::getMeshCache()�������ǡ����д��ڵ�scene Manager���᷵��ָ��ͬһ�����񻺴�
	��ָ�룬��Ϊ����һ�����档ͨ������ӿڣ������ֶ�������Ѽ��ص�����(���ISceneManager::getMesh()�����Ļ�)��
	�Ƴ����Ǻ͵��������Ѿ����ص�����
	 */
	class IMeshCache : public virtual IReferenceCounter
	{
	public:

		 
		virtual ~IMeshCache() {}

		//! ���һ���Ѽ��ص������ڲ��б�
		/** ͨ��ISceneManager::getMesh()�Ǵ�һ���ļ��������������������Ѱ�Ѽ��ص������б�
		���һ�������Ѿ������ز���������Ѿ����б���ڴ��д��ڷ���һ��ָ������ָ�롣�� With IMeshCache::addMesh()��
		��������������磺�����������Ҫһ�ε��ü��ض���������ǿ���ͨ�����������Ӷ��������scene manager��	 
		\param name ������������ISceneManager::getMesh()ʱ��ͨ��������ַ�������������õ�����
		\param mesh ָ����������ָ�룬��ָ��ᱻ������������ */
		virtual void addMesh(const path& name, IAnimatedMesh* mesh) = 0;

		//! �ӻ������Ƴ��������
		/** ��ͨ��getMesh()����һ�����������������ͨ����������ӻ������Ƴ����ͷŴ����ڴ�
		\param mesh ָ��Ҫ�Ƴ������ָ�� */
		virtual void removeMesh(const IMesh* const mesh) = 0;

		//! �����ڻ������Ѽ������������
		/** ����ʹ��getMesh������addMesh()�����µ����񵽻�����.�����ʹ��removeMesh(), getMeshNumber(),
		\return �����е������� */
		virtual UINT32 getMeshCount() const = 0;

		//! ���ص�ǰ��������������û�ҵ�����-1
		/** \param mesh Ҫ��ѯ������ָ��
		\return ��ǰ�����ڻ����е�������-1��ʾû�ҵ� */
		virtual SINT32 getMeshIndex(const IMesh* const mesh) const = 0;

		//! ����������ȡ��һ������
		/** \param index: �������������Χ��0��getMeshCount()-1.
		ע�⣺���ֻ����һ����������ػ����Ƴ�֮ǰ���ֵ����Ч
		\return ָ�������ָ�룬û�з���0.
		*/
		virtual IAnimatedMesh* getMeshByIndex(UINT32 index) = 0;


		//! ������������ȡ������
		/** \param name ��������ͨ�����ļ���
		\return ָ�������ָ�룬û�з���0. */
		virtual IAnimatedMesh* getMeshByName(const  path& name) = 0;

		//! ͨ��������ȡһ���Ѽ��ص�������
		/** \param index: �������������Χ��0��getMeshCount()-1.
		\return �����������ҵ������������������path�ǿյ�*/
		virtual const SNamedPath& getMeshName(UINT32 index) const = 0;

		//! ��ȡһ���Ѽ��ص����������
		/** \param mesh Ҫ��ѯ�����ָ��
		\return �����������ҵ������������������path�ǿյ�*/
		virtual const SNamedPath& getMeshName(const IMesh* const mesh) const = 0;

		//! ������һ���Ѽ��ص�����
		/** ע�������������ı������˳������ͨ��getMeshIndex()����������������
		����˷�ʽ�仯��
		\param index �ڻ����е�������������
		\param name ��������������
		\return �������ɹ�����true. */
		virtual bool renameMesh(UINT32 index, const path& name) = 0;

		//! �������Ѽ��ص�����
		/** ע�������������ı������˳������ͨ��getMeshIndex()����������������
		����˷�ʽ�仯��
		\param mesh Ҫ������������ָ��.
		\param name ��������������
		\return �������ɹ�����true. */
		virtual bool renameMesh(const IMesh* const mesh, const path& name) = 0;

		//! ���һ���������Ѿ�������
		/** \param name ��������ͨ�����ļ���
		\return ����Ѽ��ط���true�����򷵻�false*/
		virtual bool isMeshLoaded(const path& name) = 0;

		//! ����������񻺴棬�Ƴ���������
		/** ���ڵ������������ʹ��ISceneManager::getMesh()����������ᱻ���¼���
		���棺�������ͨ����ISceneManager::getMesh()��ȡ������ָ�벢����ÿ��grab���ǣ���ô���ǻ���Ч
	   */
		virtual void clear() = 0;

		//! ��������񻺴��������κεط�����ʹ�õ�����
		/**���ڵ������������ʹ��ISceneManager::getMesh()����������ᱻ���¼���
		���棺�������ͨ����ISceneManager::getMesh()��ȡ������ָ�벢����ÿ��grab���ǣ���ô���ǻ���Ч. */
		virtual void clearUnusedMeshes() = 0;
	};
}

#endif