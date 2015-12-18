#ifndef _SAPPHIRE_IMESH_WRITER_
#define _SAPPHIRE_IMESH_WRITER_

#include "SapphirePrerequisites.h"
#include "SapphireIReferenceCounter.h"
#include "SapphireEMeshWriterEnums.h"


namespace Sapphire
{
	class IWriteFile;
	class IMesh;

	//! д������Ľӿ�
	class IMeshWriter : public virtual IReferenceCounter
	{
	public:

		 
		virtual ~IMeshWriter() {}

		//! ��ȡ���mesh writer������
		/** �����Լ���ʵ�֣�ʹ��MAKE_IRR_ID��ʾEMESH_WRITER_TYPEö�ٷ�����Ψһ��������id
		type id.
		\return mesh writer����ID */
		virtual EMESH_WRITER_TYPE getType() const = 0;

		//! дһ����̬����
		/** \param file Ҫд��������ļ����
		\param mesh ָ��Ҫд�������ָ��
		\param flags ����writer���ԵĿ�ѡ��־
		\return ����ɹ�����true */
		virtual bool writeMesh(IWriteFile* file, IMesh* mesh,
			SINT32 flags = EMWF_NONE) = 0;

		// дһ����������
		// �����Ժ��ʹ�ã�û��writer����д���˵�ǰ��������
		/* \return ����ɹ�����true */
		//virtual bool writeAnimatedMesh(io::IWriteFile* file,
		// scene::IAnimatedMesh* mesh,
		// s32 flags=EMWF_NONE) = 0;
	};
}

#endif