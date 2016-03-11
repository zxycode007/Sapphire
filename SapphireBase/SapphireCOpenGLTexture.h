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
	// OpenGL����
	class COpenGLTexture : public ITexture
	{
	public:

	 
		COpenGLTexture(IImage* surface, const path& name, void* mipmapData = 0, COpenGLDriver* driver = 0);

		 
		virtual ~COpenGLTexture();

		//! ������������
		virtual void* lock(E_TEXTURE_LOCK_MODE mode = ETLM_READ_WRITE, UINT32 mipmapLevel = 0);

		//! �����������
		virtual void unlock();

		//! ��������ͼ���ԭʼ��С
		virtual const dimension2du& getOriginalSize() const;

		//! ��������Ĵ�С
		virtual const dimension2du& getSize() const;

		//! �����������������
		virtual E_DRIVER_TYPE getDriverType() const;

		//! �����������ɫ��ʽ
		virtual ECOLOR_FORMAT getColorFormat() const;

		//! ����������п�ࣨ�ֽڣ�
		virtual UINT32 getPitch() const;

		//! return open gl texture name
		GLuint getOpenGLTextureName() const;

		//! ������������mipmaps
		virtual bool hasMipMaps() const;

		//! �ؽ�������mipmap�ȼ�
		/** ���������޸ĸ����������
		\param mipmapData ���û������mipmap����ͼ����������ָ��ԭʼmipmap���ݵ�ָ�룬 �������б�Ҫ��mip�ȼ��� ��������ͼ��������ͬ�ĸ�ʽ��
		 */
		virtual void regenerateMipMapLevels(void* mipmapData = 0);

		//! �Ƿ���һ����ȾĿ��Render Target
		virtual bool isRenderTarget() const;

		//! �Ƿ���һ��֡�������FBO
		virtual bool isFrameBufferObject() const;

		//! ����ȾĿ������Render Target Texture
		virtual void bindRTT();

		//! �����ȾĿ������İ�
		virtual void unbindRTT();

		//! ������������������һ����ȾĿ��
		void setIsRenderTarget(bool isTarget);

	protected:

		//! �û������õı������캯�������������࣬���ᴴ��GL ������
		COpenGLTexture(const path& name, COpenGLDriver* driver);

		//! ��ȡ������������־�������ʽ����������ɫ��ʽ
		ECOLOR_FORMAT getBestColorFormat(ECOLOR_FORMAT format);

		//! ��ȡ���ڸ���������ɫ��ʽ��OpenGL ��ɫ��ʽ
		GLint getOpenGLFormatAndParametersFromColorFormat(
			ECOLOR_FORMAT format, GLint& filtering, GLenum& colorformat, GLenum& type);

		//! ��ȡͼ���Ӳ���������Ҫ��ֵ
		void getImageValues(IImage* image);

		//! �����������һ��OpenGL������
		/** \param newTexture  ����Ƕ��ڵ�һ�ζ��´�����������ã�Ϊtrue������Ϊfalse����������ڴ�������
		\param mipmapData ָ��ԭmipmap���ݵ�ָ�룬 �������б�Ҫ��mip�ȼ�����������ͼ���и�ʽ��ͬ
		\param mipLevel �������Ϊ��0�� ��ֻ�����ر�ָ����mip�ȼ��� ����mipͼ���Ա*/
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

	//! OpenGL FBO����
	class COpenGLFBOTexture : public COpenGLTexture
	{
	public:

		//! ֡�������FBO������
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


	//! OpenGL FBO�������
	class COpenGLFBODepthTexture : public COpenGLTexture
	{
	public:
		//! ��ȵ�֡�������FBO�Ĺ�����
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