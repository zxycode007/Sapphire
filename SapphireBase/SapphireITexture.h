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
	//! 告诉视频驱动这个纹理格式应该如何创建的枚举标志
	enum E_TEXTURE_CREATION_FLAG
	{
		/**
		强制驱动一直创建16位纹理格式，不依赖磁盘上的文件的具体格式。当选择这个格式你会失去一些色彩细节。
		但是会获得更快的速度和更大的内存。16位纹理能够转换速度是比32位纹理块2倍并且只用一半的内存空间。
		当使用这个标志，这些标志会没有意义		
		ETCF_ALWAYS_32_BIT, ETCF_OPTIMIZED_FOR_QUALITY, 或者
		ETCF_OPTIMIZED_FOR_SPEED 同时. */
		ETCF_ALWAYS_16_BIT = 0x00000001,

		/** 强制驱动一直创建32位纹理格式, 不依赖磁盘上的文件的具体格式。
		请注意有些驱动会无效（软件模拟），因为它们只能使用16位纹理创建。 
		当使用这个标志，这些标志会没有意义	
		ETCF_ALWAYS_16_BIT, ETCF_OPTIMIZED_FOR_QUALITY,  或者
		ETCF_OPTIMIZED_FOR_SPEED 同时. */
		ETCF_ALWAYS_32_BIT = 0x00000002,

		/** 让驱动自己决定创建纹理的格式，通常它选择存储在磁盘上的纹理格式，并以更好效果为目标创建。
		当使用这个标志，下列标志无效
		ETCF_ALWAYS_16_BIT, ETCF_ALWAYS_32_BIT, 或者 ETCF_OPTIMIZED_FOR_SPEED 
		同时 */
		ETCF_OPTIMIZED_FOR_QUALITY = 0x00000004,

		/** 让驱动自己决定创建纹理的格式，通常它选择存储在磁盘上的纹理格式，并以最大的渲染速度为目标来创建。Lets the driver decide in which format the textures are created and
		当使用这个标志，下列标志无效
		ETCF_ALWAYS_16_BIT, ETCF_ALWAYS_32_BIT, 或者 ETCF_OPTIMIZED_FOR_SPEED 
		同时 */
		ETCF_OPTIMIZED_FOR_SPEED = 0x00000008,

		/** 为纹理自动创建mipmap等级*/
		ETCF_CREATE_MIP_MAPS = 0x00000010,

		/** 丢弃任何alpha层并且使用非alpha颜色格式 */
		ETCF_NO_ALPHA_CHANNEL = 0x00000020,

		//! 允许这驱动使用非2次幂的纹理
		/** BurningVideo能够处理非2次幂纹理（GUI），但3d里不行*/
		ETCF_ALLOW_NON_POWER_2 = 0x00000040,

		/** 这个标志不会被使用，它强制编译器将枚举值编译为32位 */
		ETCF_FORCE_32_BIT_DO_NOT_USE = 0x7fffffff
	};

	//! 纹理锁定的模式枚举。 只读， 只写或者 读/写
	enum E_TEXTURE_LOCK_MODE
	{
		//! 默认模式，纹理可以被读写
		ETLM_READ_WRITE = 0,

		//! 只读模式，这个纹理是下载的，而不能被再次上传
		/** 通常用于读取后备shader创建的纹理 */
		ETLM_READ_ONLY,

		//! 只写，这个纹理不是下载的并且可能未初始化
		/** 这个更新纹理准备上传给GPU
		用于从CPU初始化着色器*/
		ETLM_WRITE_ONLY
	};

	//! 视频驱动依赖纹理的接口
	/** 一个ITexture 通过IVideoDriver::addTexture或者IVideoDriver::getTexture创建。
	之后,这个纹理只能被使用视频驱动使用。dx和ogl设备的纹理并不兼容。软件设备和空设备是个例外，他们的纹理是兼容的。
	如果你尝试使用一个设备与一个其它设备创建的纹理，这个设备会拒绝这样做并且输出一个警告或错误信息到输出缓冲区。
	*/
	//class ITexture : public virtual IReferenceCounted
	class ITexture : public virtual IReferenceCounter
	{
	public:

		 
		ITexture(const String & name) : NamedPath(name)
		{
		}

		//! 锁定函数
		/** 锁定这个纹理并且返回一个访问像素的指针。lock()调用后和所有在像素操作完成后，你必须解锁。
		锁定不能累计，因此一个解锁会对之前任意数量的锁都起作用。你需要避免锁定不同mip层，而没有事先在它们之间解锁，
		因为只有最后锁定的一层mip会被解锁。（解锁只对针对上一次锁）
		i-th mipmap 级的大小被定义 max(getSize().Width>>i,1)和 max(getSize().Height>>i,1)
		\param mode 指定要改变的锁定纹理的类型。如果纹理在只读模式写入或从写入模式读取会出现不确定的行为
		这个特征的支持依赖于驱动， 所以不能依靠纹理的当用只读锁定时的写入保护
		\param mipmapLevel mipmap等级数，0是主纹理。不存在的等级会安静的失败并返回0
		\return 返回一个指向像素数据的指针。这个像素的格式能够由getColorFormat（）得到.
		如果这个纹理不能被锁定，返回0 */
		virtual void* lock(E_TEXTURE_LOCK_MODE mode = ETLM_READ_WRITE, UINT32 mipmapLevel = 0) = 0;

		//! 解锁反射，必须在对纹理调用lock（）后使用。
		/** 要避免再另一个锁锁上之前进行一次以上解锁，最后锁定的mip 等级会被解锁。
		 */
		virtual void unlock() = 0;

		//! 获取纹理的源大小
		/** 如果它用一个非最佳的大小创建，这个纹理通常被缩放
		.例如如果大小非2次幂。这个方法返回它缩放前的大小。当在平面绘制2d图像时非常有用，它会提取纹理的元素大小。
		如果你想要知道它在系统中存储的真实大小调用ITexture::getSize() 
		\return 纹理的原始大小 */
		virtual const dimension2d<UINT32>& getOriginalSize() const = 0;

		//!获取纹理的dimension大小
		/** \return 纹理的大小 */
		virtual const dimension2d<UINT32>& getSize() const = 0;

		//! 获取纹理的驱动
		/** 这个是创建纹理的驱动。这个方法用于视频设备内部使用。
	    因为纹理可能在不同的设备间不兼容
		\return 纹理的驱动类型. */
		virtual E_DRIVER_TYPE getDriverType() const = 0;

		//! 获取纹理的颜色格式
		/** \return 纹理的颜色格式 */
		virtual ECOLOR_FORMAT getColorFormat() const = 0;

		//! 获取主纹理跨距（字节）
		/** 这个跨距是这个纹理一行像素的字节数
		\return 纹理跨距的字节数 */
		virtual UINT32 getPitch() const = 0;

		//! 检测这个纹理是否有Mipmaps
		/** \return 如果纹理有返回true，否则false */
		virtual bool hasMipMaps() const { return false; }

		//! 返回纹理是否有Alpha通道
		virtual bool hasAlpha() const {
			 
			return getColorFormat() == ECF_A8R8G8B8 || getColorFormat() == ECF_A1R5G5B5;
		}

		//! 重新创建这个纹理的mipmap等级
		/** 需要这个在纹理修改后，通常在调用unlock（）后.
		\param mipmapData 可选参数，传递图像数据，它将用于替代之前存储或自动创建的mipmap数据
		这个数据对于所有纹理必须为一个可继续细分直到1X1的像素数据。每个mipmap有之前一级的一半的宽度和高度。
		最后只有1个像素
		 */
		virtual void regenerateMipMapLevels(void* mipmapData = 0) = 0;

		//! 检测纹理是否是一个渲染目标
		/** 渲染目标在视频驱动中必须被如下设置，为了渲染一个场景到一个纹理。Render targets can be set as such in the video driver, in order to
		一旦绑定为一个渲染目标，它们通常可以作为纹理使用
		\return 如果是一个渲染目标返回true，否则false*/
		virtual bool isRenderTarget() const { return false; }

		//! 获取纹理的名字(大多数情况是一个文件名)
		const SNamedPath& getName() const { return NamedPath; }

	protected:

		//! 帮助函数，帮助获取从标志得到纹理的创建格式
		/** \return 不是ETCF_ALWAYS_32_BIT, ETCF_ALWAYS_16_BIT,
		ETCF_OPTIMIZED_FOR_QUALITY, 或 ETCF_OPTIMIZED_FOR_SPEED. */
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