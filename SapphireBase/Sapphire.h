#ifndef _SAPPHIRE_H_INCLUDE_
#define _SAPPHIRE_H_INCLUDE_

#include "SapphirePlatfrom.h"
#include "SapphireConfig.h"
#include "SapphirePrerequisites.h"
#include "SapphireCompileConifg.h"
#include "SapphireAxisAlignedBox.h"
#include "SapphireCDynamicMeshBuffer.h"
#include "SapphireCIndexBuffer.h"
#include "SapphireCMeshBuffer.h"
#include "SapphireCoreUtil.h"
#include "SapphireCVertexBuffer.h"
#include "SapphireDimension2d.h"
#include "SapphireECullingTypes.h"
#include "SapphireEDebugSceneTypes.h"
#include "SapphireEDriverFeatures.h"
#include "SapphireEDriverTypes.h"
#include "SapphireEGUIAlignment.h"
#include "SapphireEGUIElementTypes.h"
#include "SapphireEHardwareBufferFlags.h"
#include "SapphireEMaterialFlags.h"
#include "SapphireEMaterialTypes.h"
#include "SapphireEMeshWriterEnums.h"
#include "SapphireEMessageBoxFlags.h"
#include "SapphireESceneNodeAnimatorType.h"
#include "SapphireESceneNodeTypes.h"
#include "SapphireETerrianElements.h"
//#include "fast_atof.h"
//#include "heapsort.h"
#include "SapphireIAnimatedMesh.h"
#include "SapphireIAnimatedMeshMD2.h"
#include "SapphireIAnimatedMeshMD3.h"
#include "SapphireIAnimatedMeshSceneNode.h"
#include "SapphireIAttributeExchangingObject.h"
#include "SapphireIAttributes.h"
#include "SapphireIBillboardSceneNode.h"
#include "SapphireIBillboardTextSceneNode.h"
#include "SapphireIBoneSceneNode.h"
#include "SapphireICameraSceneNode.h"
#include "SapphireICursorControl.h"
#include "SapphireIDummyTransformationSceneNode.h"
#include "SapphireIDynamicMeshBuffer.h"
#include "SapphireIEventRecevier.h"
#include "SapphireIFileList.h"
#include "SapphireIFileSystem.h"
#include "SapphireIGeometryCreator.h"
#include "SapphireIGPUProgramServices.h"
#include "SapphireIGUIButton.h"
#include "SapphireIGUICheckBox.h"
#include "SapphireIGUIColorSelectDialog.h"
#include "SapphireIGUIComboBox.h"
#include "SapphireIGUIContextMenu.h"
#include "SapphireIGUIEditBox.h"
#include "SapphireIGUIElement.h"
#include "SapphireIGUIElementFactory.h"
#include "SapphireIGUIEnvironment.h"
#include "SapphireIGUIFileOpenDialog.h"
#include "SapphireFont.h"
#include "SapphireIGUIFontBitmap.h"
#include "SapphireIGUIImage.h"
#include "SapphireIGUIInOutFader.h"
#include "SapphireIGUIListBox.h"
#include "SapphireIGUIMeshViewer.h"
#include "SapphireIGUIScrollBar.h"
#include "SapphireIGUISkin.h"
#include "SapphireIGUISpinBox.h"
#include "SapphireIGUISpriteBank.h"
#include "SapphireIGUIStaticText.h"
#include "SapphireIGUITabControl.h"
#include "SapphireIGUITable.h"
#include "SapphireIGUIToolbar.h"
#include "SapphireIGUIWindow.h"
#include "SapphireIGUITreeView.h"
#include "SapphireIImage.h"
#include "SapphireIImageLoader.h"
#include "SapphireIImageWriter.h"
#include "SapphireIIndexBuffer.h"
#include "SapphireILightSceneNode.h"
#include "SapphireLog.h"
#include "SapphireLogManager.h"
#include "SapphireIMaterialRenderer.h"
#include "SapphireIMaterialRendererServices.h"
#include "SapphireIMesh.h"
#include "SapphireIMeshBuffer.h"
#include "SapphireIMeshCache.h"
#include "SapphireIMeshLoader.h"
#include "SapphireIMeshManipulator.h"
#include "SapphireIMeshSceneNode.h"
#include "SapphireIMeshWriter.h"
//#include "SapphireIColladaMeshWriter.h"
#include "SapphireIMetaTriangleSelector.h"
#include "SapphireIOSOperator.h"
#include "SapphireIParticleSysetmSceneNode.h"// also includes all emitters and attractors
#include "SapphireIQ3LevelMesh.h"
#include "SapphireIQ3Shader.h"
#include "SapphireIReadFile.h"
#include "SapphireIReferenceCounter.h"
//#include "SapphireirrArray.h"
#include "SapphireIRandomizer.h"
#include "SapphireDevice.h"
//#include "SapphireList.h"
//#include "SapphireMap.h"
#include "SapphireMath.h"
#include "SapphireString.h"
//#include "SapphireTypes.h"
#include "SapphirePath.h"
#include "SapphireIXML.h"
#include "SapphireISceneCollisionManager.h"
#include "SapphireISceneLoader.h"
#include "SapphireIScenesManager.h"
#include "SapphireISceneNode.h"
#include "SapphireISceneNodeAnimator.h"
#include "SapphireISceneNodeAnimatorCameraFPS.h"
#include "SapphireISceneNodeAnimatorCameraMaya.h"
#include "SapphireISceneNodeAnimatorCollisionResponse.h"
#include "SapphireISceneNodeAnimatorFactory.h"
#include "SapphireISceneNodeFactory.h"
//#include "SapphireISceneUserDataSerializer.h"
#include "SapphireIShaderConstantSetCallBack.h"
#include "SapphireIShadowVolumeSceneNode.h"
#include "SapphireISkinnedMesh.h"
#include "SapphireITerrianSceneNode.h"
#include "SapphireITextSceneNode.h"
#include "SapphireITexture.h"
#include "SapphireITimer.h"
#include "SapphireITriangleSelector.h"
#include "SapphireIVertexBuffer.h"
#include "SapphireIVideoDriver.h"
#include "SapphireIVideoModeList.h"
#include "SapphireIVolumeLightSceneNode.h"
#include "SapphireIWriteFile.h"
#include "SapphireIXMLReader.h"
#include "SapphireIXMLWriter.h"
#include "SapphireILightManager.h"
#include "SapphireKeycodes.h"
#include "Sapphireline2d.h"
#include "Sapphireline3d.h"
#include "SapphireMatrix4.h"
#include "SapphirePlane.h"
//#include "position2d.h"
#include "SapphireQuaternion.h"
#include "SapphireRectangle.h"
#include "SapphireSVertex.h"
#include "SapphireSAnimatedMesh.h"
#include "SapphireSceneParameters.h"
#include "SapphireColorValue.h"
#include "SapphireExposedVideoData.h"
#include "SapphireSCreationParameters.h"
#include "SapphireKeyMap.h"
#include "SapphireSLight.h"
#include "SapphireSMaterial.h"
#include "SapphireSMesh.h"
#include "SapphireSMeshBuffer.h"
#include "SapphireSMeshBufferLightMap.h"
//#include "SapphireSMeshBufferTangents.h"
#include "SapphireSParticle.h"
//#include "SapphireSSharedMeshBuffer.h"
#include "SapphireSSkinMeshBuffer.h"
#include "SapphireSVertexIndex.h"
#include "SapphireSViewFrustum.h"
#include "SapphireTriangle3D.h"
#include "SapphireVector2.h"
#include "SapphireVector3.h"
#include "SapphireMap.h"


//! Everything in the Sapphire Engine can be found in this namespace.
namespace Sapphire
{
	//! Creates an Sapphire device. The Sapphire device is the root object for using the engine.
	/** If you need more parameters to be passed to the creation of the Sapphire Engine device,
	use the createDeviceEx() function.
	\param deviceType: Type of the device. This can currently be EDT_NULL,
	EDT_SOFTWARE, EDT_BURNINGSVIDEO, EDT_DIRECT3D8, EDT_DIRECT3D9 and EDT_OPENGL.
	\param windowSize: Size of the window or the video mode in fullscreen mode.
	\param bits: Bits per pixel in fullscreen mode. Ignored if windowed mode.
	\param fullscreen: Should be set to true if the device should run in fullscreen. Otherwise
	the device runs in windowed mode.
	\param stencilbuffer: Specifies if the stencil buffer should be enabled. Set this to true,
	if you want the engine be able to draw stencil buffer shadows. Note that not all
	devices are able to use the stencil buffer. If they don't no shadows will be drawn.
	\param vsync: Specifies vertical syncronisation: If set to true, the driver will wait
	for the vertical retrace period, otherwise not.
	\param receiver: A user created event receiver.
	\return Returns pointer to the created SapphireDevice or null if the
	device could not be created.
	*/
	extern "C" _SapphireExport Device* SAPPHIRECALLCONV createDevice(
		E_DRIVER_TYPE deviceType = EDT_SOFTWARE,
		// parantheses are necessary for some compilers
		const dimension2d<UINT32>& windowSize = (dimension2d<UINT32>(640, 480)),
		UINT32 bits = 16,
		bool fullscreen = false,
		bool stencilbuffer = false,
		bool vsync = false,
		IEventReceiver* receiver = 0);

	//! typedef for Function Pointer
	typedef Device* (SAPPHIRECALLCONV *funcptr_createDevice)(
		E_DRIVER_TYPE deviceType,
		const dimension2d<UINT32>& windowSize,
		UINT32 bits,
		bool fullscreen,
		bool stencilbuffer,
		bool vsync,
		IEventReceiver* receiver);


	//! Creates an Sapphire device with the option to specify advanced parameters.
	/** Usually you should used createDevice() for creating an Sapphire Engine device.
	Use this function only if you wish to specify advanced parameters like a window
	handle in which the device should be created.
	\param parameters: Structure containing advanced parameters for the creation of the device.
	See irr::SSapphireCreationParameters for details.
	\return Returns pointer to the created SapphireDevice or null if the
	device could not be created. */
	extern "C" _SapphireExport Device* SAPPHIRECALLCONV createDeviceEx(
		const SSapphireCreationParameters& parameters);

	//! typedef for Function Pointer
	typedef Device* (SAPPHIRECALLCONV *funcptr_createDeviceEx)(const SSapphireCreationParameters& parameters);


	
}

/*! \file Sapphire.h
\brief Main header file of the Sapphire, the only file needed to include.
*/


#endif