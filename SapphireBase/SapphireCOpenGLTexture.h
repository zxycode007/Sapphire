#ifndef _SAPPHIRE_C_OPENGL_TEXTURE_
#define _SAPPHIRE_C_OPENGL_TEXTURE_

#include "SapphireITexture.h"
#include "SapphireIImage.h"

#include "SapphireCompileConifg.h"
#ifdef _SAPPHIRE_C_OPENGL_TEXTURE_

#if defined(SAPPHIRE_OPENGL_USE_EXTPOINTER)
#define GL_GLEXT_LEGACY 1
#else
#define GL_GLEXT_PROTOTYPES 1
#endif
#if SAPPHIRE_PLATFORM == SAPPHIRE_PLATFORM_WIN32
// include windows headers for HWND
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <GL/gl.h>
#if SAPPHIRE_COMPILER == SAPPHIRE_COMPILER_MSVC
#pragma comment(lib, "OpenGL32.lib")
#endif
#elif SAPPHIRE_PLATFORM == SAPPHIRE_PLATFORM_APPLE
#include <OpenGL/gl.h>
#elif defined(_SAPPHIRE_COMPILE_WITH_SDL_DEVICE_)
#define NO_SDL_GLEXT
#include <SDL/SDL_video.h>
#include <SDL/SDL_opengl.h>
#else
#if SAPPHIRE_PLATFORM == SAPPHIRE_PLATFORM_APPLE
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif
#endif


namespace Sapphire
{
	class COpenGLDriver;
	//! OpenGL texture.
	// OpenGL纹理
	class COpenGLTexture : public ITexture
	{
	public:

	 
		COpenGLTexture(IImage* surface, const path& name, void* mipmapData = 0, COpenGLDriver* driver = 0);

		 
		virtual ~COpenGLTexture();

		//! 纹理锁定函数
		virtual void* lock(E_TEXTURE_LOCK_MODE mode = ETLM_READ_WRITE, UINT32 mipmapLevel = 0);

		//! 纹理解锁函数
		virtual void unlock();

		//! 返回纹理图像的原始大小
		virtual const dimension2du& getOriginalSize() const;

		//! 返回纹理的大小
		virtual const dimension2du& getSize() const;

		//! 返回纹理的驱动类型
		virtual E_DRIVER_TYPE getDriverType() const;

		//! 返回纹理的颜色格式
		virtual ECOLOR_FORMAT getColorFormat() const;

		//! 返回纹理的行跨距（字节）
		virtual UINT32 getPitch() const;

		//! return open gl texture name
		GLuint getOpenGLTextureName() const;

		//! 返回这个纹理的mipmaps
		virtual bool hasMipMaps() const;

		//! 重建该纹理mipmap等级
		/** 在锁定和修改该纹理后有用
		\param mipmapData 如果没有设置mipmap从主图像派生，需指向原始mipmap数据的指针， 包含所有必要的mip等级， 在主纹理图像中有相同的格式。
		 */
		virtual void regenerateMipMapLevels(void* mipmapData = 0);

		//! 是否是一个渲染目标Render Target
		virtual bool isRenderTarget() const;

		//! 是否是一个帧缓冲对象FBO
		virtual bool isFrameBufferObject() const;

		//! 绑定渲染目标纹理Render Target Texture
		virtual void bindRTT();

		//! 解除渲染目标纹理的绑定
		virtual void unbindRTT();

		//! 设置这个纹理可以用于一个渲染目标
		void setIsRenderTarget(bool isTarget);

	protected:

		//! 用基础设置的保护构造函数，对于派生类，不会创建GL 纹理名
		COpenGLTexture(const path& name, COpenGLDriver* driver);

		//! 获取基于纹理创建标志和输入格式所描述的颜色格式
		ECOLOR_FORMAT getBestColorFormat(ECOLOR_FORMAT format);

		//! 获取基于给定引擎颜色格式的OpenGL 颜色格式
		GLint getOpenGLFormatAndParametersFromColorFormat(
			ECOLOR_FORMAT format, GLint& filtering, GLenum& colorformat, GLenum& type);

		//! 获取图像和硬件纹理的重要数值
		void getImageValues(IImage* image);

		//! 复制这个纹理到一个OpenGL纹理中
		/** \param newTexture  如果是对于第一次对新创建的纹理调用，为true，否则为false，可以提高内存利用率
		\param mipmapData 指向原mipmap数据的指针， 包含所有必要的mip等级，在主纹理图像中格式相同
		\param mipLevel 如果设置为非0， 将只更新特别指定的mip等级， 用与mip图像成员*/
		void uploadTexture(bool newTexture = false, void* mipmapData = 0, UINT32 mipLevel = 0);

		dimension2du ImageSize;
		dimension2du TextureSize;
		ECOLOR_FORMAT ColorFormat;
		COpenGLDriver* Driver;
		IImage* Image;
		IImage* MipImage;

		GLuint TextureName;
		GLint InternalFormat;
		GLenum PixelFormat;
		GLenum PixelType;

		UINT8 MipLevelStored;
		bool HasMipMaps;
		bool MipmapLegacyMode;
		bool IsRenderTarget;
		bool AutomaticMipmapUpdate;
		bool ReadOnlyLock;
		bool KeepImage;
	};

	//! OpenGL FBO纹理
	class COpenGLFBOTexture : public COpenGLTexture
	{
	public:

		//! 帧缓冲对象FBO构造器
		COpenGLFBOTexture(const dimension2du& size, const path& name,
			COpenGLDriver* driver = 0, ECOLOR_FORMAT format = ECF_UNKNOWN);
 
		virtual ~COpenGLFBOTexture();

		 
		virtual bool isFrameBufferObject() const;

		 
		virtual void bindRTT();

	 
		virtual void unbindRTT();

		ITexture* DepthTexture;
	protected:
		GLuint ColorFrameBuffer;
	};


	//! OpenGL FBO深度纹理
	class COpenGLFBODepthTexture : public COpenGLTexture
	{
	public:
		//! 深度的帧缓冲对象FBO的构造器
		COpenGLFBODepthTexture(const dimension2du& size, const path& name, COpenGLDriver* driver = 0, bool useStencil = false);

		 
		virtual ~COpenGLFBODepthTexture();

	 
		virtual void bindRTT();

	 
		virtual void unbindRTT();

		bool attach(ITexture*);

	protected:
		GLuint DepthRenderBuffer;
		GLuint StencilRenderBuffer;
		bool UseStencil;
	};
}

#endif
#endif