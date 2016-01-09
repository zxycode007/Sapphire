 #ifndef _SAPPHIRE_ITEXTURE_
#define _SAPPHIRE_ITEXTURE_

#include "SapphireEDriverTypes.h"
#include "SapphirePrerequisites.h"
#include "SapphireIReferenceCounter.h"
#include "SapphireDimension2D.h"
#include "SapphirePath.h"
#include "SapphireColorValue.h"


namespace Sapphire
{
	//! ������Ƶ������������ʽӦ����δ�����ö�ٱ�־
	enum E_TEXTURE_CREATION_FLAG
	{
		/**
		ǿ������һֱ����16λ�����ʽ�������������ϵ��ļ��ľ����ʽ����ѡ�������ʽ���ʧȥһЩɫ��ϸ�ڡ�
		���ǻ��ø�����ٶȺ͸�����ڴ档16λ�����ܹ�ת���ٶ��Ǳ�32λ�����2������ֻ��һ����ڴ�ռ䡣
		��ʹ�������־����Щ��־��û������		
		ETCF_ALWAYS_32_BIT, ETCF_OPTIMIZED_FOR_QUALITY, ����
		ETCF_OPTIMIZED_FOR_SPEED ͬʱ. */
		ETCF_ALWAYS_16_BIT = 0x00000001,

		/** ǿ������һֱ����32λ�����ʽ, �����������ϵ��ļ��ľ����ʽ��
		��ע����Щ��������Ч�����ģ�⣩����Ϊ����ֻ��ʹ��16λ�������� 
		��ʹ�������־����Щ��־��û������	
		ETCF_ALWAYS_16_BIT, ETCF_OPTIMIZED_FOR_QUALITY,  ����
		ETCF_OPTIMIZED_FOR_SPEED ͬʱ. */
		ETCF_ALWAYS_32_BIT = 0x00000002,

		/** �������Լ�������������ĸ�ʽ��ͨ����ѡ��洢�ڴ����ϵ������ʽ�����Ը���Ч��ΪĿ�괴����
		��ʹ�������־�����б�־��Ч
		ETCF_ALWAYS_16_BIT, ETCF_ALWAYS_32_BIT, ���� ETCF_OPTIMIZED_FOR_SPEED 
		ͬʱ */
		ETCF_OPTIMIZED_FOR_QUALITY = 0x00000004,

		/** �������Լ�������������ĸ�ʽ��ͨ����ѡ��洢�ڴ����ϵ������ʽ������������Ⱦ�ٶ�ΪĿ����������Lets the driver decide in which format the textures are created and
		��ʹ�������־�����б�־��Ч
		ETCF_ALWAYS_16_BIT, ETCF_ALWAYS_32_BIT, ���� ETCF_OPTIMIZED_FOR_SPEED 
		ͬʱ */
		ETCF_OPTIMIZED_FOR_SPEED = 0x00000008,

		/** Ϊ�����Զ�����mipmap�ȼ�*/
		ETCF_CREATE_MIP_MAPS = 0x00000010,

		/** �����κ�alpha�㲢��ʹ�÷�alpha��ɫ��ʽ */
		ETCF_NO_ALPHA_CHANNEL = 0x00000020,

		//! ����������ʹ�÷�2���ݵ�����
		/** BurningVideo�ܹ������2��������GUI������3d�ﲻ��*/
		ETCF_ALLOW_NON_POWER_2 = 0x00000040,

		/** �����־���ᱻʹ�ã���ǿ�Ʊ�������ö��ֵ����Ϊ32λ */
		ETCF_FORCE_32_BIT_DO_NOT_USE = 0x7fffffff
	};

	//! ����������ģʽö�١� ֻ���� ֻд���� ��/д
	enum E_TEXTURE_LOCK_MODE
	{
		//! Ĭ��ģʽ��������Ա���д
		ETLM_READ_WRITE = 0,

		//! ֻ��ģʽ��������������صģ������ܱ��ٴ��ϴ�
		/** ͨ�����ڶ�ȡ��shader���������� */
		ETLM_READ_ONLY,

		//! ֻд��������������صĲ��ҿ���δ��ʼ��
		/** �����������׼���ϴ���GPU
		���ڴ�CPU��ʼ����ɫ��*/
		ETLM_WRITE_ONLY
	};

	//! ��Ƶ������������Ľӿ�
	/** һ��ITexture ͨ��IVideoDriver::addTexture����IVideoDriver::getTexture������
	֮��,�������ֻ�ܱ�ʹ����Ƶ����ʹ�á�dx��ogl�豸�����������ݡ�����豸�Ϳ��豸�Ǹ����⣬���ǵ������Ǽ��ݵġ�
	����㳢��ʹ��һ���豸��һ�������豸��������������豸��ܾ��������������һ������������Ϣ�������������
	*/
	//class ITexture : public virtual IReferenceCounted
	class ITexture : public virtual IReferenceCounter
	{
	public:

		 
		ITexture(const String & name) : NamedPath(name)
		{
		}

		//! ��������
		/** ������������ҷ���һ���������ص�ָ�롣lock()���ú�����������ز�����ɺ�����������
		���������ۼƣ����һ���������֮ǰ�������������������á�����Ҫ����������ͬmip�㣬��û������������֮�������
		��Ϊֻ�����������һ��mip�ᱻ������������ֻ�������һ������
		i-th mipmap ���Ĵ�С������ max(getSize().Width>>i,1)�� max(getSize().Height>>i,1)
		\param mode ָ��Ҫ�ı��������������͡����������ֻ��ģʽд����д��ģʽ��ȡ����ֲ�ȷ������Ϊ
		���������֧�������������� ���Բ�����������ĵ���ֻ������ʱ��д�뱣��
		\param mipmapLevel mipmap�ȼ�����0�������������ڵĵȼ��ᰲ����ʧ�ܲ�����0
		\return ����һ��ָ���������ݵ�ָ�롣������صĸ�ʽ�ܹ���getColorFormat�����õ�.
		�����������ܱ�����������0 */
		virtual void* lock(E_TEXTURE_LOCK_MODE mode = ETLM_READ_WRITE, UINT32 mipmapLevel = 0) = 0;

		//! �������䣬�����ڶ��������lock������ʹ�á�
		/** Ҫ��������һ��������֮ǰ����һ�����Ͻ��������������mip �ȼ��ᱻ������
		 */
		virtual void unlock() = 0;

		//! ��ȡ�����Դ��С
		/** �������һ������ѵĴ�С�������������ͨ��������
		.���������С��2���ݡ������������������ǰ�Ĵ�С������ƽ�����2dͼ��ʱ�ǳ����ã�������ȡ�����Ԫ�ش�С��
		�������Ҫ֪������ϵͳ�д洢����ʵ��С����ITexture::getSize() 
		\return �����ԭʼ��С */
		virtual const dimension2d<UINT32>& getOriginalSize() const = 0;

		//!��ȡ�����dimension��С
		/** \return ����Ĵ�С */
		virtual const dimension2d<UINT32>& getSize() const = 0;

		//! ��ȡ���������
		/** ����Ǵ���������������������������Ƶ�豸�ڲ�ʹ�á�
	    ��Ϊ��������ڲ�ͬ���豸�䲻����
		\return �������������. */
		virtual E_DRIVER_TYPE getDriverType() const = 0;

		//! ��ȡ�������ɫ��ʽ
		/** \return �������ɫ��ʽ */
		virtual ECOLOR_FORMAT getColorFormat() const = 0;

		//! ��ȡ�������ࣨ�ֽڣ�
		/** ���������������һ�����ص��ֽ���
		\return ��������ֽ��� */
		virtual UINT32 getPitch() const = 0;

		//! �����������Ƿ���Mipmaps
		/** \return ��������з���true������false */
		virtual bool hasMipMaps() const { return false; }

		//! ���������Ƿ���Alphaͨ��
		virtual bool hasAlpha() const {
			 
			return getColorFormat() == ECF_A8R8G8B8 || getColorFormat() == ECF_A1R5G5B5;
		}

		//! ���´�����������mipmap�ȼ�
		/** ��Ҫ����������޸ĺ�ͨ���ڵ���unlock������.
		\param mipmapData ��ѡ����������ͼ�����ݣ������������֮ǰ�洢���Զ�������mipmap����
		������ݶ��������������Ϊһ���ɼ���ϸ��ֱ��1X1���������ݡ�ÿ��mipmap��֮ǰһ����һ��Ŀ�Ⱥ͸߶ȡ�
		���ֻ��1������
		 */
		virtual void regenerateMipMapLevels(void* mipmapData = 0) = 0;

		//! ��������Ƿ���һ����ȾĿ��
		/** ��ȾĿ������Ƶ�����б��뱻�������ã�Ϊ����Ⱦһ��������һ������Render targets can be set as such in the video driver, in order to
		һ����Ϊһ����ȾĿ�꣬����ͨ��������Ϊ����ʹ��
		\return �����һ����ȾĿ�귵��true������false*/
		virtual bool isRenderTarget() const { return false; }

		//! ��ȡ���������(����������һ���ļ���)
		const SNamedPath& getName() const { return NamedPath; }

	protected:

		//! ����������������ȡ�ӱ�־�õ�����Ĵ�����ʽ
		/** \return ����ETCF_ALWAYS_32_BIT, ETCF_ALWAYS_16_BIT,
		ETCF_OPTIMIZED_FOR_QUALITY, �� ETCF_OPTIMIZED_FOR_SPEED. */
		inline E_TEXTURE_CREATION_FLAG getTextureFormatFromFlags(UINT32 flags)
		{
			if (flags & ETCF_OPTIMIZED_FOR_SPEED)
				return ETCF_OPTIMIZED_FOR_SPEED;
			if (flags & ETCF_ALWAYS_16_BIT)
				return ETCF_ALWAYS_16_BIT;
			if (flags & ETCF_ALWAYS_32_BIT)
				return ETCF_ALWAYS_32_BIT;
			if (flags & ETCF_OPTIMIZED_FOR_QUALITY)
				return ETCF_OPTIMIZED_FOR_QUALITY;
			return ETCF_OPTIMIZED_FOR_SPEED;
		}

		SNamedPath NamedPath;
	};

	
}

#endif