#ifndef _SAPPHIRE_IQ3LevelMesh_
#define _SAPPHIRE_IQ3LevelMesh_

#include "SapphirePrerequisites.h"
#include "SapphireIQ3Shader.h"

namespace Sapphire
{
	namespace Quake3
	{
		//! ���Դ�Quake3.bsp�ļ�ֱ�Ӽ�������Ľӿ�
		/** �������᳢�Լ�������������ӳ��*/
		class IQ3LevelMesh : public IAnimatedMesh
		{
		public:

			//! ���ļ�����shader����
			/** \param filename shader�ļ������֣� ���fileNameIsValidΪfalse��Ĭ��Ϊ/scripts
			\param fileNameIsValid ָ���ڵ�ǰ����µ��ļ����Ƿ���Ч*/
			virtual const Quake3::IShader* getShader(const c8* filename, bool fileNameIsValid = true) = 0;

			//! ����һ���Ѿ����ص�shader
			virtual const Quake3::IShader* getShader(UINT32 index) const = 0;

			//! ��ȡһ����Entity�б�Ľӿ�
			virtual Quake3::tQ3EntityList& getEntityList() = 0;

			//! �����������Ļ�ˢʵ��
			/** \param num ʵ���ģ�ͼ�������

			�����Ҫ�����Լ���ʵ�壬������ӿ�*/
			virtual IMesh* getBrushEntityMesh(SINT32 num) const = 0;

			//! ��������Ļ�ˢʵ��
			virtual IMesh* getBrushEntityMesh(Quake3::IEntity &ent) const = 0;
		};
	}
}

#endif