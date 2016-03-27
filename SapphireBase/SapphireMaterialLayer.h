#ifndef _SAPPHIRE_MATERIAL_LAYERS_
#define _SAPPHIRE_MATERIAL_LAYERS_

#include "SapphirePrerequisites.h"
#include "SapphireMatrix4.h"
#include "SapphireITexture.h"
#include "SapphireSharedPtr.h"

namespace Sapphire
{


	//! �������걻������[0.0, 1.0]
	enum E_TEXTURE_CLAMP
	{
		//! �����ظ�
		ETC_REPEAT = 0,
		//! ��������һ����������
		ETC_CLAMP,
		//! �����߽���������
		ETC_CLAMP_TO_EDGE,
		//! ������Ե��������
		ETC_CLAMP_TO_BORDER,
		//! �����澵��(0..1..0..1..0..)
		ETC_MIRROR,
		//! ������һ��Ȼ������ (0..1..0)
		ETC_MIRROR_CLAMP,
		//! ������һ��Ȼ�����Ƶ��߽�
		ETC_MIRROR_CLAMP_TO_EDGE,
		//! ������һ��Ȼ�����Ƶ���Ե
		ETC_MIRROR_CLAMP_TO_BORDER
	};
	static const char* const aTextureClampNames[] = {
		"texture_clamp_repeat",
		"texture_clamp_clamp",
		"texture_clamp_clamp_to_edge",
		"texture_clamp_clamp_to_border",
		"texture_clamp_mirror",
		"texture_clamp_mirror_clamp",
		"texture_clamp_mirror_clamp_to_edge",
		"texture_clamp_mirror_clamp_to_border", 0 };

	//! ����ÿ������㶼���ڵĲ��ʲ����Ľṹ��
	class SMaterialLayer
	{
	public:
		
		SMaterialLayer()
			: Texture(0),
			TextureWrapU(ETC_REPEAT),
			TextureWrapV(ETC_REPEAT),
			BilinearFilter(true),
			TrilinearFilter(false),
			AnisotropicFilter(0),
			LODBias(0)
			//TextureMatrix(0)
		{
			//TextureMatrix = SAPPHIRE_NEW_T(Matrix4(Matrix4::IDENTITY), MEMCATEGORY_GENERAL);
			TextureMatrix = new Matrix4(Matrix4::IDENTITY);
		}

		//! �������캯��
		/** \param �����Ĳ��ʲ�. */
		SMaterialLayer(const SMaterialLayer& other)
		{
			//TextureMatrix = SAPPHIRE_NEW_T(Matrix4(Matrix4::IDENTITY), MEMCATEGORY_GENERAL);
			TextureMatrix = new Matrix4(Matrix4::IDENTITY);
			//TextureMatrix = 0;
			*this = other;
		}

		
		~SMaterialLayer()
		{
			//MatrixAllocator.destruct(TextureMatrix);
			//MatrixAllocator.deallocate(TextureMatrix);
			//SAPPHIRE_DELETE_T(TextureMatrix, Matrix4, MEMCATEGORY_GENERAL);
			delete TextureMatrix;
		}

		//! ���ز�����
		/** \param�����Ĳ��ʲ�
		\return ���ظ��º�Ĳ��ʲ� */
		SMaterialLayer& operator=(const SMaterialLayer& other)
		{
			// �ж��Ƿ�������
			if (this == &other)
				return *this;

			Texture = other.Texture;
			//�������������
			if (TextureMatrix)
			{
				if (other.TextureMatrix)
				{
					 
					*TextureMatrix = *(other.TextureMatrix);
				}	
				else
				{
					//MatrixAllocator.destruct(TextureMatrix);
					//MatrixAllocator.deallocate(TextureMatrix);
					
					SAPPHIRE_DELETE_T(TextureMatrix, Matrix4, MEMCATEGORY_GENERAL);
				}
			}
			else
			{
				//��������������
				if (other.TextureMatrix )
				{
					TextureMatrix = SAPPHIRE_NEW_T(Matrix4(*(other.TextureMatrix)), MEMCATEGORY_GENERAL);
					//TextureMatrix = MatrixAllocator.allocate(1);
					//MatrixAllocator.construct(TextureMatrix, *other.TextureMatrix);
				}
				else
				{}
					//TextureMatrix = 0;
			}
			TextureWrapU = other.TextureWrapU;
			TextureWrapV = other.TextureWrapV;
			BilinearFilter = other.BilinearFilter;
			TrilinearFilter = other.TrilinearFilter;
			AnisotropicFilter = other.AnisotropicFilter;
			LODBias = other.LODBias;

			return *this;
		}

		//! ��ȡ����任����
		/** \return ��һ������ı任����
		//core::matrix4& getTextureMatrix()*/
		Matrix4& getTextureMatrix()
		{
			if (!TextureMatrix)
			{
				//TextureMatrix = MatrixAllocator.allocate(1);
				//MatrixAllocator.construct(TextureMatrix, core::IdentityMatrix);
				TextureMatrix = SAPPHIRE_NEW_T(Matrix4(Matrix4::IDENTITY), MEMCATEGORY_GEOMETRY);
			}
			return *TextureMatrix;
		}

		//! ��ȡ�����޸ĵ��������
		/** \return ��һ������ı任����
		const core::matrix4& getTextureMatrix() const
		*/
		const Matrix4& getTextureMatrix() const
		{
			if (TextureMatrix)
				return *TextureMatrix;
			else
				return  Matrix4::IDENTITY;
		}

		//! ��������任����
		/** \param mat ��һ��������µ��������
		void setTextureMatrix(const core::matrix4& mat)
		*/
		void setTextureMatrix(const Matrix4& mat)
		{
			if (TextureMatrix)
			{
				//TextureMatrix = MatrixAllocator.allocate(1);
				//MatrixAllocator.construct(TextureMatrix, mat);
				*TextureMatrix = mat;
			}
			else
			{	 
				TextureMatrix = SAPPHIRE_NEW_T(Matrix4(mat), MEMCATEGORY_GEOMETRY);;
			}
				
		}
		//����������
		void emptyTextureMatrix()
		{
			//����������Ϊ��
			if (TextureMatrix)
			{
				SAPPHIRE_DELETE_T(TextureMatrix, Matrix4, MEMCATEGORY_GENERAL);
			}
		}

		 
		inline bool operator!=(const SMaterialLayer& b) const
		{
			bool different =
				Texture != b.Texture ||
				TextureWrapU != b.TextureWrapU ||
				TextureWrapV != b.TextureWrapV ||
				BilinearFilter != b.BilinearFilter ||
				TrilinearFilter != b.TrilinearFilter ||
				AnisotropicFilter != b.AnisotropicFilter ||
				LODBias != b.LODBias;
			if (different)
				return true;
			else
				different |= (TextureMatrix != b.TextureMatrix) &&
				TextureMatrix && b.TextureMatrix &&
				(*TextureMatrix != *(b.TextureMatrix));
			return different;
		}

		 
		inline bool operator==(const SMaterialLayer& b) const
		{
			return !(b != *this);
		}

		//! ����
		ITexture* Texture;

		//! ��������ģʽ
		/** ֵȡ��E_TEXTURE_CLAMP. */
		UINT8 TextureWrapU : 4;
		UINT8 TextureWrapV : 4;

		//! �Ƿ�˫�����˲�  Ĭ��: true
		bool BilinearFilter : 1;

		//!�Ƿ��������˲�  Ĭ��: true
		/** ��������Դ򿪣�˫���Ծ�ʧЧ */
		bool TrilinearFilter : 1;

		//! �Ƿ���������˲�  Ĭ��: false
		/** ���ܹ�ͬʱʹ�ø��������˲���˫�����˲������������˲������Ч��
		��������Ǹ��������������ԽǶȣ�ͨ����Χ��2-16. ֵ1�ȼ���0������������ʹ��1
		*/
		UINT8 AnisotropicFilter;

		//! mipmapѡ��ƫ��
		/** ���ֵ�ܹ�ʹ����������ٵ�ģ����Ĭ��ֵ0 ��
		���ֵ������8.0f������ӵ����ѡ���mipmap�ȼ�,������ˣ�������ֵΪ�����������ʹ��һ����С������*/
		s8 LODBias;

	private:
		friend class SMaterial;
		// irr::core::irrAllocator<irr::core::matrix4> MatrixAllocator;

		//! �������
	
		Matrix4* TextureMatrix;
	};
}

#endif