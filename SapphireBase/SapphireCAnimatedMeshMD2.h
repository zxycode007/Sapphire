#ifndef _SAPPHIRE_C_ANIMATED_MESH_MD2_
#define _SAPPHIRE_C_ANIMATED_MESH_MD2_

#include "SapphireIAnimatedMeshMD2.h"
#include "SapphireIMesh.h"
#include "SapphireCMeshBuffer.h"
#include "SapphireIReadFile.h"
#include "SapphireSVertex.h"

namespace Sapphire
{
	class CAnimatedMeshMD2 : public IAnimatedMeshMD2
	{
	public:

		//! constructor
		CAnimatedMeshMD2();

		//! destructor
		virtual ~CAnimatedMeshMD2();

		//! returns the amount of frames. If the amount is 1, it is a static (=non animated) mesh.
		virtual UINT32 getFrameCount() const;

		//! Gets the default animation speed of the animated mesh.
		/** \return Amount of frames per second. If the amount is 0, it is a static, non animated mesh. */
		virtual FLOAT32 getAnimationSpeed() const
		{
			return FramesPerSecond;
		}

		//! Gets the frame count of the animated mesh.
		/** \param fps Frames per second to play the animation with. If the amount is 0, it is not animated.
		The actual speed is set in the scene node the mesh is instantiated in.*/
		virtual void setAnimationSpeed(FLOAT32 fps)
		{
			FramesPerSecond = fps;
		}

		//! returns the animated mesh based on a detail level. 0 is the lowest, 255 the highest detail. Note, that some Meshes will ignore the detail level.
		virtual IMesh* getMesh(SINT32 frame, SINT32 detailLevel = 255, SINT32 startFrameLoop = -1, SINT32 endFrameLoop = -1);

		//! returns amount of mesh buffers.
		virtual UINT32 getMeshBufferCount() const;

		//! returns pointer to a mesh buffer
		virtual IMeshBuffer* getMeshBuffer(UINT32 nr) const;

		//! Returns pointer to a mesh buffer which fits a material
		/** \param material: material to search for
		\return Returns the pointer to the mesh buffer or
		NULL if there is no such mesh buffer. */
		virtual IMeshBuffer* getMeshBuffer(const SMaterial &material) const;

		//! returns an axis aligned bounding box
		virtual const AxisAlignedBox& getBoundingBox() const;

		//! set user axis aligned bounding box
		virtual void setBoundingBox(const AxisAlignedBox& box);

		//! sets a flag of all contained materials to a new value
		virtual void setMaterialFlag(E_MATERIAL_FLAG flag, bool newvalue);

		//! set the hardware mapping hint, for driver
		virtual void setHardwareMappingHint(E_HARDWARE_MAPPING newMappingHint, E_BUFFER_TYPE buffer = EBT_VERTEX_AND_INDEX);

		//! flags the meshbuffer as changed, reloads hardware buffers
		virtual void setDirty(E_BUFFER_TYPE buffer = EBT_VERTEX_AND_INDEX);

		//! Returns the type of the animated mesh.
		virtual E_ANIMATED_MESH_TYPE getMeshType() const;

		//! Returns frame loop data for a special MD2 animation type.
		virtual void getFrameLoop(EMD2_ANIMATION_TYPE,
			SINT32& outBegin, SINT32& outEnd, SINT32& outFps) const;

		//! Returns frame loop data for a special MD2 animation type.
		virtual bool getFrameLoop(const c8* name,
			SINT32& outBegin, SINT32& outEnd, SINT32& outFps) const;

		//! Returns amount of md2 animations in this file.
		virtual SINT32 getAnimationCount() const;

		//! Returns name of md2 animation.
		//! \param nr: Zero based index of animation.
		virtual const c8* getAnimationName(SINT32 nr) const;


		//
		// exposed for loader
		//

		//! the buffer that contains the most recent animation
		SMeshBuffer* InterpolationBuffer;

		//! named animations
		struct SAnimationData
		{
			String name;
			SINT32 begin;
			SINT32 end;
			SINT32 fps;
		};

		//! scale and translations for keyframes
		struct SKeyFrameTransform
		{
			Vector3 scale;
			Vector3 translate;
		};

		//! md2 vertex data
		struct SMD2Vert
		{
			Vector3 Pos;
			UINT8                 NormalIdx;
		};

		//! keyframe transformations
		vector<SKeyFrameTransform>::type FrameTransforms;

		//! keyframe vertex data
		vector<SMD2Vert>::type *FrameList;

		//! bounding boxes for each keyframe
		vector<AxisAlignedBox >::type BoxList;

		//! named animations
		vector< SAnimationData >::type AnimationData;

		UINT32 FrameCount;

	private:

		//! updates the interpolation buffer
		void updateInterpolationBuffer(SINT32 frame, SINT32 startFrame, SINT32 endFrame);

		FLOAT32 FramesPerSecond;
	};
}

#endif