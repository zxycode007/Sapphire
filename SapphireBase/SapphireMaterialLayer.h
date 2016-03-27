#ifndef _SAPPHIRE_MATERIAL_LAYERS_
#define _SAPPHIRE_MATERIAL_LAYERS_

#include "SapphirePrerequisites.h"
#include "SapphireMatrix4.h"
#include "SapphireITexture.h"
#include "SapphireSharedPtr.h"

namespace Sapphire
{


	//! 纹理坐标被限制在[0.0, 1.0]
	enum E_TEXTURE_CLAMP
	{
		//! 纹理重复
		ETC_REPEAT = 0,
		//! 纹理被倍上一个像素限制
		ETC_CLAMP,
		//! 纹理被边界像素限制
		ETC_CLAMP_TO_EDGE,
		//! 纹理被边缘像素限制
		ETC_CLAMP_TO_BORDER,
		//! 纹理交替镜像(0..1..0..1..0..)
		ETC_MIRROR,
		//! 纹理镜像一次然后被限制 (0..1..0)
		ETC_MIRROR_CLAMP,
		//! 纹理镜像一次然后被限制到边界
		ETC_MIRROR_CLAMP_TO_EDGE,
		//! 纹理镜像一次然后被限制到边缘
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

	//! 保存每个纹理层都存在的材质参数的结构体
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

		//! 拷贝构造函数
		/** \param 其它的材质层. */
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

		//! 重载操作符
		/** \param其它的材质层
		\return 返回更新后的材质层 */
		SMaterialLayer& operator=(const SMaterialLayer& other)
		{
			// 判断是否是自身
			if (this == &other)
				return *this;

			Texture = other.Texture;
			//如果纹理矩阵存在
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
				//对面纹理矩阵存在
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

		//! 获取纹理变换矩阵
		/** \return 这一层纹理的变换矩阵
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

		//! 获取不可修改的纹理矩阵
		/** \return 这一层纹理的变换矩阵
		const core::matrix4& getTextureMatrix() const
		*/
		const Matrix4& getTextureMatrix() const
		{
			if (TextureMatrix)
				return *TextureMatrix;
			else
				return  Matrix4::IDENTITY;
		}

		//! 设置纹理变换矩阵
		/** \param mat 这一层纹理的新的纹理矩阵
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
		//清空纹理矩阵
		void emptyTextureMatrix()
		{
			//如果纹理矩阵不为空
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

		//! 纹理
		ITexture* Texture;

		//! 纹理限制模式
		/** 值取至E_TEXTURE_CLAMP. */
		UINT8 TextureWrapU : 4;
		UINT8 TextureWrapV : 4;

		//! 是否双线性滤波  默认: true
		bool BilinearFilter : 1;

		//!是否三线性滤波  默认: true
		/** 如果三线性打开，双线性就失效 */
		bool TrilinearFilter : 1;

		//! 是否各项异性滤波  默认: false
		/** 你能够同时使用各项异性滤波合双线性滤波或者三线性滤波以提高效果
		这个数字是给定的最大各项异性角度，通常范围砸2-16. 值1等价于0，但尽量避免使用1
		*/
		UINT8 AnisotropicFilter;

		//! mipmap选择偏差
		/** 这个值能够使纹理更多或更少的模糊比默认值0 。
		这个值（除以8.0f）是添加到最初选择的mipmap等级,并且因此，如果这个值为正，这个区域使用一个更小的纹理*/
		s8 LODBias;

	private:
		friend class SMaterial;
		// irr::core::irrAllocator<irr::core::matrix4> MatrixAllocator;

		//! 纹理矩阵
	
		Matrix4* TextureMatrix;
	};
}

#endif