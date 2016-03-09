#ifndef _SAPPHIRE_C_ANIMATED_MESH_MD3_
#define _SAPPHIRE_C_ANIMATED_MESH_MD3_

#include "SapphireIAnimatedMeshMD3.h"
#include "SapphireIReadFile.h"
#include "SapphireIFileSystem.h"

#include "SapphireSMesh.h"
#include "SapphireSMeshBuffer.h"
#include "SapphireIQ3Shader.h"

namespace Sapphire
{
	class CAnimatedMeshMD3 : public IAnimatedMeshMD3
	{
	public:

		//! constructor
		CAnimatedMeshMD3();

		//! destructor
		virtual ~CAnimatedMeshMD3();

		//! loads a quake3 md3 file
		virtual bool loadModelFile(UINT32 modelIndex, IReadFile* file,
			IFileSystem* fs, IVideoDriver* driver);

		// IAnimatedMeshMD3
		virtual void setInterpolationShift(UINT32 shift, UINT32 loopMode);
		virtual SMD3Mesh* getOriginalMesh();
		virtual SMD3QuaternionTagList* getTagList(SINT32 frame, SINT32 detailLevel, SINT32 startFrameLoop, SINT32 endFrameLoop);

		//IAnimatedMesh
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

		virtual IMesh* getMesh(SINT32 frame, SINT32 detailLevel,
			SINT32 startFrameLoop, SINT32 endFrameLoop);
		virtual const AxisAlignedBox& getBoundingBox() const;
		virtual E_ANIMATED_MESH_TYPE getMeshType() const;

		//! returns amount of mesh buffers.
		virtual UINT32 getMeshBufferCount() const;

		//! returns pointer to a mesh buffer
		virtual IMeshBuffer* getMeshBuffer(UINT32 nr) const;

		//! Returns pointer to a mesh buffer which fits a material
		virtual IMeshBuffer* getMeshBuffer(const SMaterial &material) const;

		virtual void setMaterialFlag(E_MATERIAL_FLAG flag, bool newvalue);

		//! set user axis aligned bounding box
		virtual void setBoundingBox(const AxisAlignedBox& box);

		//! set the hardware mapping hint, for driver
		virtual void setHardwareMappingHint(E_HARDWARE_MAPPING newMappingHint, E_BUFFER_TYPE buffer = EBT_VERTEX_AND_INDEX);

		//! flags the meshbuffer as changed, reloads hardware buffers
		virtual void setDirty(E_BUFFER_TYPE buffer = EBT_VERTEX_AND_INDEX);

	private:
		//! animates one frame
		inline void Animate(UINT32 frame);

		SMaterial Material;

		//! hold original compressed MD3 Info
		SMD3Mesh *Mesh;

		UINT32 IPolShift;
		UINT32 LoopMode;
		FLOAT32 Scaling;

		//! Cache Info
		struct SCacheInfo
		{
			SCacheInfo(SINT32 frame = -1, SINT32 start = -1, SINT32 end = -1) :
				Frame(frame), startFrameLoop(start),
				endFrameLoop(end)
			{}

			bool operator == (const SCacheInfo &other) const
			{
				return 0 == memcmp(this, &other, sizeof(SCacheInfo));
			}
			SINT32 Frame;
			SINT32 startFrameLoop;
			SINT32 endFrameLoop;
		};
		SCacheInfo Current;

		//! return a Mesh per frame
		SMesh* MeshIPol;
		SMD3QuaternionTagList TagListIPol;

		IMeshBuffer* createMeshBuffer(const SMD3MeshBuffer* source,
			IFileSystem* fs, IVideoDriver* driver);

		void buildVertexArray(UINT32 frameA, UINT32 frameB, FLOAT32 interpolate,
			const SMD3MeshBuffer* source,
			SMeshBufferLightMap* dest);

		void buildTagArray(UINT32 frameA, UINT32 frameB, FLOAT32 interpolate);
		FLOAT32 FramesPerSecond;
	};
}

#endif