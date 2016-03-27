#ifndef _SAPPHIRE_COMPILE_CONFIG_
#define _SAPPHIRE_COMPILE_CONFIG_





//! Define _IRR_COMPILE_WITH_GUI_ to compile the engine with the built-in GUI
/** Disable this if you are using an external library to draw the GUI. If you disable this then
you will not be able to use anything provided by the GUI Environment, including loading fonts. */
#define _SAPPHIRE_COMPILE_WITH_GUI_
#ifdef NO_SAPPHIRE_COMPILE_WITH_GUI_
#undef _SAPPHIRE_COMPILE_WITH_GUI_
#endif


//! Comment this line to compile without the fallback console device.
#define SAPPHIRE_COMPILE_WITH_CONSOLE_DEVICE_
#ifdef NO_SAPPHIRE_COMPILE_WITH_CONSOLE_DEVICE_
#undef SAPPHIRE_COMPILE_WITH_CONSOLE_DEVICE_
#endif


#if defined(_WIN32)
#define _SAPPHIRE_COMPILE_WITH_WINDOWS_DEVICE_
#define _SAPPHIRE_WINDOWS_
#define _SAPPHIRE_WINDOWS_API_
#endif




//! Define _IRR_COMPILE_WITH_JOYSTICK_SUPPORT_ if you want joystick events.
#define _SAPPHIRE_COMPILE_WITH_JOYSTICK_EVENTS_
#ifdef NO_SAPPHIRE_COMPILE_WITH_JOYSTICK_EVENTS_
#undef _SAPPHIRE_COMPILE_WITH_JOYSTICK_EVENTS_
#endif


/** If you only want to use the software device or opengl you can disable those defines.
This switch is mostly disabled because people do not get the g++ compiler compile
directX header files, and directX is only available on Windows platforms. If you
are using Dev-Cpp, and want to compile this using a DX dev pack, you can define
_IRR_COMPILE_WITH_DX9_DEV_PACK_. So you simply need to add something like this
to the compiler settings: -DIRR_COMPILE_WITH_DX9_DEV_PACK
and this to the linker settings: -ld3dx9 -ld3dx8

Microsoft have chosen to remove D3D8 headers from their recent DXSDKs, and
so D3D8 support is now disabled by default.  If you really want to build
with D3D8 support, then you will have to source a DXSDK with the appropriate
headers, e.g. Summer 2004.  This is a Microsoft issue, not an Irrlicht one.
*/
#if defined(_SAPPHIRE_WINDOWS_API_) && (!defined(__GNUC__) || defined(SAPPHIRE_COMPILE_WITH_DX9_DEV_PACK))

//! Define _IRR_COMPILE_WITH_DIRECTINPUT_JOYSTICK_ if you want to use DirectInput for joystick handling.
/** This only applies to Windows devices, currently only supported under Win32 device.
If not defined, Windows Multimedia library is used, which offers also broad support for joystick devices. */
#define _SAPPHIRE_COMPILE_WITH_DIRECTINPUT_JOYSTICK_
#ifdef NO_SAPPHIRE_COMPILE_WITH_DIRECTINPUT_JOYSTICK_
#undef _SAPPHIRE_COMPILE_WITH_DIRECTINPUT_JOYSTICK_
#endif
// can't get this to compile currently under borland, can be removed if someone has a better solution
#if defined(__BORLANDC__)
#undef _SAPPHIRE_COMPILE_WITH_DIRECTINPUT_JOYSTICK_
#endif

//! Only define _IRR_COMPILE_WITH_DIRECT3D_8_ if you have an appropriate DXSDK, e.g. Summer 2004
// #define _IRR_COMPILE_WITH_DIRECT3D_8_
//#define _SAPPHIRE_COMPILE_WITH_DIRECT3D_9_

#ifdef NO_SAPPHIRE_COMPILE_WITH_DIRECT3D_8_
#undef _SAPPHIRE_COMPILE_WITH_DIRECT3D_8_
#endif
#ifdef NO_SAPPHIRE_COMPILE_WITH_DIRECT3D_9_
#undef _SAPPHIRE_COMPILE_WITH_DIRECT3D_9_
#endif

#endif



//! Define _IRR_COMPILE_WITH_SOFTWARE_ to compile the Irrlicht engine with software driver
/** If you do not need the software driver, or want to use Burning's Video instead,
comment this define out */
//#define _SAPPHIRE_COMPILE_WITH_SOFTWARE_
#ifdef NO_SAPPHIRE_COMPILE_WITH_SOFTWARE_
#undef _SAPPHIRE_COMPILE_WITH_SOFTWARE_
#endif

//! Define _IRR_COMPILE_WITH_BURNINGSVIDEO_ to compile the Irrlicht engine with Burning's video driver
/** If you do not need this software driver, you can comment this define out. */
//#define _SAPPHIRE_COMPILE_WITH_BURNINGSVIDEO_
#ifdef NO_SAPPHIRE_COMPILE_WITH_BURNINGSVIDEO_
#undef _SAPPHIRE_COMPILE_WITH_BURNINGSVIDEO_
#endif


//! Define SAPPHIRE_COMPILE_WITH_LIBPNG_ to enable compiling the engine using libpng.
/** This enables the engine to read png images. If you comment this out,
the engine will no longer read .png images. */
#define SAPPHIRE_COMPILE_WITH_LIBPNG_
#ifdef NOSAPPHIRE_COMPILE_WITH_LIBPNG_
#undef SAPPHIRE_COMPILE_WITH_LIBPNG_
#endif

//! Define SAPPHIRE_USE_NON_SYSTEM_LIBPNG_ to let irrlicht use the libpng which comes with irrlicht.
/** If this is commented out, Irrlicht will try to compile using the libpng installed in the system.
This is only used when SAPPHIRE_COMPILE_WITH_LIBPNG_ is defined. */
#define SAPPHIRE_USE_NON_SYSTEM_LIB_PNG_
#ifdef NOSAPPHIRE_USE_NON_SYSTEM_LIB_PNG_
#undef SAPPHIRE_USE_NON_SYSTEM_LIB_PNG_
#endif


//! Define SAPPHIRE_COMPILE_WITH_JPEGLIB_ to enable compiling the engine using libjpeg.
/** This enables the engine to read jpeg images. If you comment this out,
the engine will no longer read .jpeg images. */
#define SAPPHIRE_COMPILE_WITH_LIBJPEG_
#ifdef NOSAPPHIRE_COMPILE_WITH_LIBJPEG_
#undef SAPPHIRE_COMPILE_WITH_LIBJPEG_
#endif

//! Define SAPPHIRE_USE_NON_SYSTEM_JPEG_LIB_ to let irrlicht use the jpeglib which comes with irrlicht.
/** If this is commented out, Irrlicht will try to compile using the jpeg lib installed in the system.
This is only used when SAPPHIRE_COMPILE_WITH_LIBJPEG_ is defined. */
#define SAPPHIRE_USE_NON_SYSTEM_JPEG_LIB_
#ifdef NOSAPPHIRE_USE_NON_SYSTEM_JPEG_LIB_
#undef SAPPHIRE_USE_NON_SYSTEM_JPEG_LIB_
#endif

//! Define SAPPHIRE_COMPILE_WITHSAPPHIRE_SCENE_LOADER_ if you want to be able to load
/** .irr scenes using ISceneManager::loadScene */
#define SAPPHIRE_COMPILE_WITHSAPPHIRE_SCENE_LOADER_
#ifdef NOSAPPHIRE_COMPILE_WITHSAPPHIRE_SCENE_LOADER_
#undef SAPPHIRE_COMPILE_WITHSAPPHIRE_SCENE_LOADER_
#endif

//! Define SAPPHIRE_COMPILE_WITH_SKINNED_MESH_SUPPORT_ if you want to use bone based
/** animated meshes. If you compile without this, you will be unable to load
B3D, MS3D or X meshes */
#define SAPPHIRE_COMPILE_WITH_SKINNED_MESH_SUPPORT_
#ifdef NOSAPPHIRE_COMPILE_WITH_SKINNED_MESH_SUPPORT_
#undef SAPPHIRE_COMPILE_WITH_SKINNED_MESH_SUPPORT_
#endif

#ifdef SAPPHIRE_COMPILE_WITH_SKINNED_MESH_SUPPORT_
//! Define SAPPHIRE_COMPILE_WITH_B3D_LOADER_ if you want to use Blitz3D files
#define SAPPHIRE_COMPILE_WITH_B3D_LOADER_
#ifdef NOSAPPHIRE_COMPILE_WITH_B3D_LOADER_
#undef SAPPHIRE_COMPILE_WITH_B3D_LOADER_
#endif
//! Define SAPPHIRE_COMPILE_WITH_MS3D_LOADER_ if you want to Milkshape files
#define SAPPHIRE_COMPILE_WITH_MS3D_LOADER_
#ifdef NOSAPPHIRE_COMPILE_WITH_MS3D_LOADER_
#undef SAPPHIRE_COMPILE_WITH_MS3D_LOADER_
#endif
//! Define SAPPHIRE_COMPILE_WITH_X_LOADER_ if you want to use Microsoft X files
#define SAPPHIRE_COMPILE_WITH_X_LOADER_
#ifdef NOSAPPHIRE_COMPILE_WITH_X_LOADER_
#undef SAPPHIRE_COMPILE_WITH_X_LOADER_
#endif
//! Define SAPPHIRE_COMPILE_WITH_OGRE_LOADER_ if you want to load Ogre 3D files
#define SAPPHIRE_COMPILE_WITH_OGRE_LOADER_
#ifdef NOSAPPHIRE_COMPILE_WITH_OGRE_LOADER_
#undef SAPPHIRE_COMPILE_WITH_OGRE_LOADER_
#endif
#endif	// SAPPHIRE_COMPILE_WITH_SKINNED_MESH_SUPPORT_

//! Define SAPPHIRE_COMPILE_WITHSAPPHIRE_MESH_LOADER_ if you want to load Irrlicht Engine .irrmesh files
#define _SAPPHIRE_COMPILE_WITH_IRR_MESH_LOADER_
#ifdef NOSAPPHIRE_COMPILE_WITH_IRR_MESH_LOADER_
#undef _SAPPHIRE_COMPILE_WITH_IRR_MESH_LOADER_
#endif
//! Define SAPPHIRE_COMPILE_WITH_HALFLIFE_LOADER_ if you want to load Halflife animated files
//#define _SAPPHIRE_COMPILE_WITH_HALFLIFE_LOADER_
#ifdef NOSAPPHIRE_COMPILE_WITH_HALFLIFE_LOADER_
#undef _SAPPHIRE_COMPILE_WITH_HALFLIFE_LOADER_
#endif
//! Define SAPPHIRE_COMPILE_WITH_MD2_LOADER_ if you want to load Quake 2 animated files
#define _SAPPHIRE_COMPILE_WITH_MD2_LOADER_
#ifdef NOSAPPHIRE_COMPILE_WITH_MD2_LOADER_
#undef _SAPPHIRE_COMPILE_WITH_MD2_LOADER_
#endif
//! Define SAPPHIRE_COMPILE_WITH_MD3_LOADER_ if you want to load Quake 3 animated files
#define _SAPPHIRE_COMPILE_WITH_MD3_LOADER_
#ifdef NOSAPPHIRE_COMPILE_WITH_MD3_LOADER_
#undef _SAPPHIRE_COMPILE_WITH_MD3_LOADER_
#endif
//! Define SAPPHIRE_COMPILE_WITH_3DS_LOADER_ if you want to load 3D Studio Max files
//#define _SAPPHIRE_COMPILE_WITH_3DS_LOADER_
#ifdef NOSAPPHIRE_COMPILE_WITH_3DS_LOADER_
#undef _SAPPHIRE_COMPILE_WITH_3DS_LOADER_
#endif
//! Define SAPPHIRE_COMPILE_WITH_COLLADA_LOADER_ if you want to load Collada files
#define _SAPPHIRE_COMPILE_WITH_COLLADA_LOADER_
#ifdef NOSAPPHIRE_COMPILE_WITH_COLLADA_LOADER_
#undef _SAPPHIRE_COMPILE_WITH_COLLADA_LOADER_
#endif
//! Define SAPPHIRE_COMPILE_WITH_CSM_LOADER_ if you want to load Cartography Shop files
#define _SAPPHIRE_COMPILE_WITH_CSM_LOADER_
#ifdef NOSAPPHIRE_COMPILE_WITH_CSM_LOADER_
#undef _SAPPHIRE_COMPILE_WITH_CSM_LOADER_
#endif
//! Define SAPPHIRE_COMPILE_WITH_BSP_LOADER_ if you want to load Quake 3 BSP files
#define _SAPPHIRE_COMPILE_WITH_BSP_LOADER_
#ifdef NOSAPPHIRE_COMPILE_WITH_BSP_LOADER_
#undef _SAPPHIRE_COMPILE_WITH_BSP_LOADER_
#endif
//! Define SAPPHIRE_COMPILE_WITH_DMF_LOADER_ if you want to load DeleD files
#define _SAPPHIRE_COMPILE_WITH_DMF_LOADER_
#ifdef NOSAPPHIRE_COMPILE_WITH_DMF_LOADER_
#undef _SAPPHIRE_COMPILE_WITH_DMF_LOADER_
#endif
//! Define SAPPHIRE_COMPILE_WITH_LMTS_LOADER_ if you want to load LMTools files
#define _SAPPHIRE_COMPILE_WITH_LMTS_LOADER_
#ifdef NOSAPPHIRE_COMPILE_WITH_LMTS_LOADER_
#undef _SAPPHIRE_COMPILE_WITH_LMTS_LOADER_
#endif
//! Define SAPPHIRE_COMPILE_WITH_MY3D_LOADER_ if you want to load MY3D files
#define _SAPPHIRE_COMPILE_WITH_MY3D_LOADER_
#ifdef NOSAPPHIRE_COMPILE_WITH_MY3D_LOADER_
#undef _SAPPHIRE_COMPILE_WITH_MY3D_LOADER_
#endif
//! Define SAPPHIRE_COMPILE_WITH_OBJ_LOADER_ if you want to load Wavefront OBJ files
#define _SAPPHIRE_COMPILE_WITH_OBJ_LOADER_
#ifdef NOSAPPHIRE_COMPILE_WITH_OBJ_LOADER_
#undef _SAPPHIRE_COMPILE_WITH_OBJ_LOADER_
#endif
//! Define SAPPHIRE_COMPILE_WITH_OCT_LOADER_ if you want to load FSRad OCT files
//#define _SAPPHIRE_COMPILE_WITH_OCT_LOADER_
#ifdef NOSAPPHIRE_COMPILE_WITH_OCT_LOADER_
#undef _SAPPHIRE_COMPILE_WITH_OCT_LOADER_
#endif
//! Define SAPPHIRE_COMPILE_WITH_LWO_LOADER_ if you want to load Lightwave3D files
//#define _SAPPHIRE_COMPILE_WITH_LWO_LOADER_
#ifdef NOSAPPHIRE_COMPILE_WITH_LWO_LOADER_
#undef _SAPPHIRE_COMPILE_WITH_LWO_LOADER_
#endif
//! Define SAPPHIRE_COMPILE_WITH_STL_LOADER_ if you want to load stereolithography files
//#define _SAPPHIRE_COMPILE_WITH_STL_LOADER_
#ifdef NOSAPPHIRE_COMPILE_WITH_STL_LOADER_
#undef _SAPPHIRE_COMPILE_WITH_STL_LOADER_
#endif
//! Define SAPPHIRE_COMPILE_WITH_PLY_LOADER_ if you want to load Polygon (Stanford Triangle) files
//#define _SAPPHIRE_COMPILE_WITH_PLY_LOADER_
#ifdef NOSAPPHIRE_COMPILE_WITH_PLY_LOADER_
#undef _SAPPHIRE_COMPILE_WITH_PLY_LOADER_
#endif
//! Define SAPPHIRE_COMPILE_WITH_SMF_LOADER_ if you want to load 3D World Studio mesh files
//#define _SAPPHIRE_COMPILE_WITH_SMF_LOADER_
#ifdef NOSAPPHIRE_COMPILE_WITH_SMF_LOADER_
#undef _SAPPHIRE_COMPILE_WITH_SMF_LOADER_
#endif

//! Define SAPPHIRE_COMPILE_WITHSAPPHIRE_WRITER_ if you want to write static .irrMesh files
#define _SAPPHIRE_COMPILE_WITH_IRR_MESH_LOADER_
#ifdef NOSAPPHIRE_COMPILE_WITH_IRR_WRITER_
#undef _SAPPHIRE_COMPILE_WITH_IRR_MESH_LOADER_
#endif
//! Define SAPPHIRE_COMPILE_WITH_COLLADA_WRITER_ if you want to write Collada files
//#define _SAPPHIRE_COMPILE_WITH_COLLADA_WRITER_
#ifdef NOSAPPHIRE_COMPILE_WITH_COLLADA_WRITER_
#undef _SAPPHIRE_COMPILE_WITH_COLLADA_WRITER_
#endif
//! Define SAPPHIRE_COMPILE_WITH_STL_WRITER_ if you want to write .stl files
//#define _SAPPHIRE_COMPILE_WITH_STL_WRITER_
#ifdef NOSAPPHIRE_COMPILE_WITH_STL_WRITER_
#undef _SAPPHIRE_COMPILE_WITH_STL_WRITER_
#endif
//! Define SAPPHIRE_COMPILE_WITH_OBJ_WRITER_ if you want to write .obj files
//#define _SAPPHIRE_COMPILE_WITH_OBJ_WRITER_
#ifdef NOSAPPHIRE_COMPILE_WITH_OBJ_WRITER_
#undef _SAPPHIRE_COMPILE_WITH_OBJ_WRITER_
#endif
//! Define SAPPHIRE_COMPILE_WITH_PLY_WRITER_ if you want to write .ply files
//#define _SAPPHIRE_COMPILE_WITH_PLY_WRITER_
#ifdef NOSAPPHIRE_COMPILE_WITH_PLY_WRITER_
#undef _SAPPHIRE_COMPILE_WITH_PLY_WRITER_
#endif

//! Define SAPPHIRE_COMPILE_WITH_BMP_LOADER_ if you want to load .bmp files
//! Disabling this loader will also disable the built-in font
#define SAPPHIRE_COMPILE_WITH_BMP_LOADER_
#ifdef NOSAPPHIRE_COMPILE_WITH_BMP_LOADER_
#undef SAPPHIRE_COMPILE_WITH_BMP_LOADER_
#endif
//! Define SAPPHIRE_COMPILE_WITH_JPG_LOADER_ if you want to load .jpg files
#define SAPPHIRE_COMPILE_WITH_JPG_LOADER_
#ifdef NOSAPPHIRE_COMPILE_WITH_JPG_LOADER_
#undef SAPPHIRE_COMPILE_WITH_JPG_LOADER_
#endif
//! Define SAPPHIRE_COMPILE_WITH_PCX_LOADER_ if you want to load .pcx files
//#define SAPPHIRE_COMPILE_WITH_PCX_LOADER_
#ifdef NOSAPPHIRE_COMPILE_WITH_PCX_LOADER_
#undef SAPPHIRE_COMPILE_WITH_PCX_LOADER_
#endif
//! Define SAPPHIRE_COMPILE_WITH_PNG_LOADER_ if you want to load .png files
#define SAPPHIRE_COMPILE_WITH_PNG_LOADER_
#ifdef NOSAPPHIRE_COMPILE_WITH_PNG_LOADER_
#undef SAPPHIRE_COMPILE_WITH_PNG_LOADER_
#endif
//! Define SAPPHIRE_COMPILE_WITH_PPM_LOADER_ if you want to load .ppm/.pgm/.pbm files
//#define SAPPHIRE_COMPILE_WITH_PPM_LOADER_
#ifdef NOSAPPHIRE_COMPILE_WITH_PPM_LOADER_
#undef SAPPHIRE_COMPILE_WITH_PPM_LOADER_
#endif
//! Define SAPPHIRE_COMPILE_WITH_PSD_LOADER_ if you want to load .psd files
//#define SAPPHIRE_COMPILE_WITH_PSD_LOADER_
#ifdef NOSAPPHIRE_COMPILE_WITH_PSD_LOADER_
#undef SAPPHIRE_COMPILE_WITH_PSD_LOADER_
#endif
//! Define SAPPHIRE_COMPILE_WITH_DDS_LOADER_ if you want to load .dds files
// Outcommented because
// a) it doesn't compile on 64-bit currently
// b) anyone enabling it should be aware that S3TC compression algorithm which might be used in that loader
// is patented in the US by S3 and they do collect license fees when it's used in applications.
// So if you are unfortunate enough to develop applications for US market and their broken patent system be careful.
// #define SAPPHIRE_COMPILE_WITH_DDS_LOADER_
#ifdef NOSAPPHIRE_COMPILE_WITH_DDS_LOADER_
#undef SAPPHIRE_COMPILE_WITH_DDS_LOADER_
#endif
//! Define SAPPHIRE_COMPILE_WITH_TGA_LOADER_ if you want to load .tga files
#define SAPPHIRE_COMPILE_WITH_TGA_LOADER_
#ifdef NOSAPPHIRE_COMPILE_WITH_TGA_LOADER_
#undef SAPPHIRE_COMPILE_WITH_TGA_LOADER_
#endif
//! Define SAPPHIRE_COMPILE_WITH_WAL_LOADER_ if you want to load .wal files
//#define SAPPHIRE_COMPILE_WITH_WAL_LOADER_
#ifdef NOSAPPHIRE_COMPILE_WITH_WAL_LOADER_
#undef SAPPHIRE_COMPILE_WITH_WAL_LOADER_
#endif
//! Define SAPPHIRE_COMPILE_WITH_LMP_LOADER_ if you want to load .lmp files
//#define SAPPHIRE_COMPILE_WITH_LMP_LOADER_
#ifdef NOSAPPHIRE_COMPILE_WITH_LMP_LOADER_
#undef SAPPHIRE_COMPILE_WITH_LMP_LOADER_
#endif
//! Define SAPPHIRE_COMPILE_WITH_RGB_LOADER_ if you want to load Silicon Graphics .rgb/.rgba/.sgi/.int/.inta/.bw files
//#define SAPPHIRE_COMPILE_WITH_RGB_LOADER_
#ifdef NOSAPPHIRE_COMPILE_WITH_RGB_LOADER_
#undef SAPPHIRE_COMPILE_WITH_RGB_LOADER_
#endif

//! Define SAPPHIRE_COMPILE_WITH_BMP_WRITER_ if you want to write .bmp files
#define SAPPHIRE_COMPILE_WITH_BMP_WRITER_
#ifdef NOSAPPHIRE_COMPILE_WITH_BMP_WRITER_
#undef SAPPHIRE_COMPILE_WITH_BMP_WRITER_
#endif
//! Define SAPPHIRE_COMPILE_WITH_JPG_WRITER_ if you want to write .jpg files
#define SAPPHIRE_COMPILE_WITH_JPG_WRITER_
#ifdef NOSAPPHIRE_COMPILE_WITH_JPG_WRITER_
#undef SAPPHIRE_COMPILE_WITH_JPG_WRITER_
#endif
//! Define SAPPHIRE_COMPILE_WITH_PCX_WRITER_ if you want to write .pcx files
//#define SAPPHIRE_COMPILE_WITH_PCX_WRITER_
#ifdef NOSAPPHIRE_COMPILE_WITH_PCX_WRITER_
#undef SAPPHIRE_COMPILE_WITH_PCX_WRITER_
#endif
//! Define SAPPHIRE_COMPILE_WITH_PNG_WRITER_ if you want to write .png files
#define SAPPHIRE_COMPILE_WITH_PNG_WRITER_
#ifdef NOSAPPHIRE_COMPILE_WITH_PNG_WRITER_
#undef SAPPHIRE_COMPILE_WITH_PNG_WRITER_
#endif
//! Define SAPPHIRE_COMPILE_WITH_PPM_WRITER_ if you want to write .ppm files
//#define SAPPHIRE_COMPILE_WITH_PPM_WRITER_
#ifdef NOSAPPHIRE_COMPILE_WITH_PPM_WRITER_
#undef SAPPHIRE_COMPILE_WITH_PPM_WRITER_
#endif
//! Define SAPPHIRE_COMPILE_WITH_PSD_WRITER_ if you want to write .psd files
//#define SAPPHIRE_COMPILE_WITH_PSD_WRITER_
#ifdef NOSAPPHIRE_COMPILE_WITH_PSD_WRITER_
#undef SAPPHIRE_COMPILE_WITH_PSD_WRITER_
#endif
//! Define SAPPHIRE_COMPILE_WITH_TGA_WRITER_ if you want to write .tga files
#define SAPPHIRE_COMPILE_WITH_TGA_WRITER_
#ifdef NOSAPPHIRE_COMPILE_WITH_TGA_WRITER_
#undef SAPPHIRE_COMPILE_WITH_TGA_WRITER_
#endif

//! Define _SAPPHIRE_COMPILE_WITH_ZIP_ARCHIVE_LOADER_ if you want to open ZIP and GZIP archives
/** ZIP reading has several more options below to configure. */
#define _SAPPHIRE_COMPILE_WITH_ZIP_ARCHIVE_LOADER_
#ifdef NO_SAPPHIRE_COMPILE_WITH_ZIP_ARCHIVE_LOADER_
#undef _SAPPHIRE_COMPILE_WITH_ZIP_ARCHIVE_LOADER_
#endif
#ifdef _SAPPHIRE_COMPILE_WITH_ZIP_ARCHIVE_LOADER_
//! Define SAPPHIRE_COMPILE_WITH_ZLIB_ to enable compiling the engine using zlib.
/** This enables the engine to read from compressed .zip archives. If you
disable this feature, the engine can still read archives, but only uncompressed
ones. */
#define SAPPHIRE_COMPILE_WITH_ZLIB_
#ifdef NOSAPPHIRE_COMPILE_WITH_ZLIB_
#undef SAPPHIRE_COMPILE_WITH_ZLIB_
#endif
//! Define SAPPHIRE_USE_NON_SYSTEM_ZLIB_ to let irrlicht use the zlib which comes with irrlicht.
/** If this is commented out, Irrlicht will try to compile using the zlib
installed on the system. This is only used when SAPPHIRE_COMPILE_WITH_ZLIB_ is
defined. */
#define SAPPHIRE_USE_NON_SYSTEM_ZLIB_
#ifdef NOSAPPHIRE_USE_NON_SYSTEM_ZLIB_
#undef SAPPHIRE_USE_NON_SYSTEM_ZLIB_
#endif
//! Define SAPPHIRE_COMPILE_WITH_ZIP_ENCRYPTION_ if you want to read AES-encrypted ZIP archives
#define SAPPHIRE_COMPILE_WITH_ZIP_ENCRYPTION_
#ifdef NOSAPPHIRE_COMPILE_WITH_ZIP_ENCRYPTION_
#undef SAPPHIRE_COMPILE_WITH_ZIP_ENCRYPTION_
#endif
//! Define SAPPHIRE_COMPILE_WITH_BZIP2_ if you want to support bzip2 compressed zip archives
/** bzip2 is superior to the original zip file compression modes, but requires
a certain amount of memory for decompression and adds several files to the
library. */
#define SAPPHIRE_COMPILE_WITH_BZIP2_
#ifdef NOSAPPHIRE_COMPILE_WITH_BZIP2_
#undef SAPPHIRE_COMPILE_WITH_BZIP2_
#endif
//! Define SAPPHIRE_USE_NON_SYSTEM_BZLIB_ to let irrlicht use the bzlib which comes with irrlicht.
/** If this is commented out, Irrlicht will try to compile using the bzlib
installed on the system. This is only used when SAPPHIRE_COMPILE_WITH_BZLIB_ is
defined. */
#define SAPPHIRE_USE_NON_SYSTEM_BZLIB_
#ifdef NOSAPPHIRE_USE_NON_SYSTEM_BZLIB_
#undef SAPPHIRE_USE_NON_SYSTEM_BZLIB_
#endif
//! Define SAPPHIRE_COMPILE_WITH_LZMA_ if you want to use LZMA compressed zip files.
/** LZMA is a very efficient compression code, known from 7zip. Irrlicht
currently only supports zip archives, though. */
#define SAPPHIRE_COMPILE_WITH_LZMA_
#ifdef NOSAPPHIRE_COMPILE_WITH_LZMA_
#undef SAPPHIRE_COMPILE_WITH_LZMA_
#endif
#endif

//! Define _SAPPHIRE_COMPILE_WITH_MOUNT_ARCHIVE_LOADER_ if you want to mount folders as archives
#define _SAPPHIRE_COMPILE_WITH_MOUNT_ARCHIVE_LOADER_
#ifdef NO_SAPPHIRE_COMPILE_WITH_MOUNT_ARCHIVE_LOADER_
#undef _SAPPHIRE_COMPILE_WITH_MOUNT_ARCHIVE_LOADER_
#endif
//! Define _SAPPHIRE_COMPILE_WITH_PAK_ARCHIVE_LOADER_ if you want to open ID software PAK archives
#define _SAPPHIRE_COMPILE_WITH_PAK_ARCHIVE_LOADER_
#ifdef NO_SAPPHIRE_COMPILE_WITH_PAK_ARCHIVE_LOADER_
#undef _SAPPHIRE_COMPILE_WITH_PAK_ARCHIVE_LOADER_
#endif
//! Define _SAPPHIRE_COMPILE_WITH_NPK_ARCHIVE_LOADER_ if you want to open Nebula Device NPK archives
#define _SAPPHIRE_COMPILE_WITH_NPK_ARCHIVE_LOADER_
#ifdef NO_SAPPHIRE_COMPILE_WITH_NPK_ARCHIVE_LOADER_
#undef _SAPPHIRE_COMPILE_WITH_NPK_ARCHIVE_LOADER_
#endif
//! Define _SAPPHIRE_COMPILE_WITH_TAR_ARCHIVE_LOADER_ if you want to open TAR archives
#define _SAPPHIRE_COMPILE_WITH_TAR_ARCHIVE_LOADER_
#ifdef NO_SAPPHIRE_COMPILE_WITH_TAR_ARCHIVE_LOADER_
#undef _SAPPHIRE_COMPILE_WITH_TAR_ARCHIVE_LOADER_
#endif
//! Define _SAPPHIRE_COMPILE_WITH_WAD_ARCHIVE_LOADER_ if you want to open WAD archives
#define _SAPPHIRE_COMPILE_WITH_WAD_ARCHIVE_LOADER_
#ifdef NO_SAPPHIRE_COMPILE_WITH_WAD_ARCHIVE_LOADER_
#undef _SAPPHIRE_COMPILE_WITH_WAD_ARCHIVE_LOADER_
#endif





#define _SAPPHIRE_COMPILE_WITH_ZLIB_
#ifdef NO_SAPPHIRE_COMPILE_WITH_ZLIB_
#undef _SAPPHIRE_COMPILE_WITH_ZLIB_
#endif
//! Define _SAPPHIRE_USE_NON_SYSTEM_ZLIB_ to let irrlicht use the zlib which comes with irrlicht.
/** If this is commented out, Irrlicht will try to compile using the zlib
installed on the system. This is only used when _SAPPHIRE_COMPILE_WITH_ZLIB_ is
defined. */
#define _SAPPHIRE_USE_NON_SYSTEM_ZLIB_
#ifdef NO_SAPPHIRE_USE_NON_SYSTEM_ZLIB_
#undef _SAPPHIRE_USE_NON_SYSTEM_ZLIB_
#endif
//! Define _SAPPHIRE_COMPILE_WITH_ZIP_ENCRYPTION_ if you want to read AES-encrypted ZIP archives
#define _SAPPHIRE_COMPILE_WITH_ZIP_ENCRYPTION_
#ifdef NO_SAPPHIRE_COMPILE_WITH_ZIP_ENCRYPTION_
#undef _SAPPHIRE_COMPILE_WITH_ZIP_ENCRYPTION_
#endif
//! Define _SAPPHIRE_COMPILE_WITH_BZIP2_ if you want to support bzip2 compressed zip archives
/** bzip2 is superior to the original zip file compression modes, but requires
a certain amount of memory for decompression and adds several files to the
library. */
#define _SAPPHIRE_COMPILE_WITH_BZIP2_
#ifdef NO_SAPPHIRE_COMPILE_WITH_BZIP2_
#undef _SAPPHIRE_COMPILE_WITH_BZIP2_
#endif
//! Define _SAPPHIRE_USE_NON_SYSTEM_BZLIB_ to let irrlicht use the bzlib which comes with irrlicht.
/** If this is commented out, Irrlicht will try to compile using the bzlib
installed on the system. This is only used when _SAPPHIRE_COMPILE_WITH_BZLIB_ is
defined. */
#define _SAPPHIRE_USE_NON_SYSTEM_BZLIB_
#ifdef NO_SAPPHIRE_USE_NON_SYSTEM_BZLIB_
#undef _SAPPHIRE_USE_NON_SYSTEM_BZLIB_
#endif
//! Define _SAPPHIRE_COMPILE_WITH_LZMA_ if you want to use LZMA compressed zip files.
/** LZMA is a very efficient compression code, known from 7zip. Irrlicht
currently only supports zip archives, though. */
#define _SAPPHIRE_COMPILE_WITH_LZMA_
#ifdef NO_SAPPHIRE_COMPILE_WITH_LZMA_
#undef _SAPPHIRE_COMPILE_WITH_LZMA_
#endif
#endif





//! Define _IRR_COMPILE_WITH_SKINNED_MESH_SUPPORT_ if you want to use bone based
/** animated meshes. If you compile without this, you will be unable to load
B3D, MS3D or X meshes */
#define _SAPPHIRE_COMPILE_WITH_SKINNED_MESH_SUPPORT_
#ifdef NO_SAPPHIRE_COMPILE_WITH_SKINNED_MESH_SUPPORT_
#undef _SAPPHIRE_COMPILE_WITH_SKINNED_MESH_SUPPORT_
#endif





//! Define _SAPPHIRE_COMPILE_WITH_OPENGL_ to compile the Irrlicht engine with OpenGL.
/** If you do not wish the engine to be compiled with OpenGL, comment this
define out. */
#define _SAPPHIRE_COMPILE_WITH_OPENGL_
#ifdef NO_SAPPHIRE_COMPILE_WITH_OPENGL_
#undef _SAPPHIRE_COMPILE_WITH_OPENGL_
#endif



// Declare the calling convention.
#if defined(_STDCALL_SUPPORTED)
#define SAPPHIRECALLCONV __stdcall
#else
#define SAPPHIRECALLCONV __cdecl
#endif // STDCALL_SUPPORTED