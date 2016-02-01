#ifndef _SAPPHIRE_S_ANIMATED_MESH_
#define _SAPPHIRE_S_ANIMATED_MESH_

#include "SapphireIAnimatedMesh.h"
#include "SapphireIMesh.h"
#include "SapphireAxisAlignedBox.h"

namespace Sapphire
{
	//! Simple implementation of the IAnimatedMesh interface.
	struct SAnimatedMesh : public IAnimatedMesh
	{
		//! constructor
		SAnimatedMesh(IMesh* mesh = 0, E_ANIMATED_MESH_TYPE type = EAMT_UNKNOWN) : IAnimatedMesh(), FramesPerSecond(25.f), Type(type)
		{
#ifdef _DEBUG
			setDebugName("SAnimatedMesh");
#endif
			addMesh(mesh);
			recalculateBoundingBox();
		}

		//! destructor
		virtual ~SAnimatedMesh()
		{
			// drop meshes
			for (UINT32 i = 0; i<Meshes.size(); ++i)
				Meshes[i]->drop();
		}

		//! Gets the frame count of the animated mesh.
		/** \return Amount of frames. If the amount is 1, it is a static, non animated mesh. */
		virtual UINT32 getFrameCount() const
		{
			return Meshes.size();
		}

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

		//! Returns the IMesh interface for a frame.
		/** \param frame: Frame number as zero based index. The maximum frame number is
		getFrameCount() - 1;
		\param detailLevel: Level of detail. 0 is the lowest,
		255 the highest level of detail. Most meshes will ignore the detail level.
		\param startFrameLoop: start frame
		\param endFrameLoop: end frame
		\return The animated mesh based on a detail level. */
		virtual IMesh* getMesh(SINT32 frame, SINT32 detailLevel = 255, SINT32 startFrameLoop = -1, SINT32 endFrameLoop = -1)
		{
			if (Meshes.empty())
				return 0;

			return Meshes[frame];
		}

		//! adds a Mesh
		void addMesh(IMesh* mesh)
		{
			if (mesh)
			{
				mesh->grab();
				Meshes.push_back(mesh);
			}
		}

		//! Returns an axis aligned bounding box of the mesh.
		/** \return A bounding box of this mesh is returned. */
		virtual const AxisAlignedBox& getBoundingBox() const
		{
			return Box;
		}

		//! set user axis aligned bounding box
		virtual void setBoundingBox(const AxisAlignedBox& box)
		{
			Box = box;
		}

		//! Recalculates the bounding box.
		void recalculateBoundingBox()
		{
			//Box.reset(0, 0, 0);
			Box.reset(Vector3(0, 0, 0));

			if (Meshes.empty())
				return;

			Box = Meshes[0]->getBoundingBox();

			for (UINT32 i = 1; i<Meshes.size(); ++i)
				Box.addInternalBox(Meshes[i]->getBoundingBox());
		}

		//! Returns the type of the animated mesh.
		virtual E_ANIMATED_MESH_TYPE getMeshType() const
		{
			return Type;
		}

		//! returns amount of mesh buffers.
		virtual UINT32 getMeshBufferCount() const
		{
			if (Meshes.empty())
				return 0;

			return Meshes[0]->getMeshBufferCount();
		}

		//! returns pointer to a mesh buffer
		virtual IMeshBuffer* getMeshBuffer(UINT32 nr) const
		{
			if (Meshes.empty())
				return 0;

			return Meshes[0]->getMeshBuffer(nr);
		}

		//! Returns pointer to a mesh buffer which fits a material
		/** \param material: material to search for
		\return Returns the pointer to the mesh buffer or
		NULL if there is no such mesh buffer. */
		virtual IMeshBuffer* getMeshBuffer(const SMaterial &material) const
		{
			if (Meshes.empty())
				return 0;

			return Meshes[0]->getMeshBuffer(material);
		}

		//! Set a material flag for all meshbuffers of this mesh.
		virtual void setMaterialFlag(E_MATERIAL_FLAG flag, bool newvalue)
		{
			for (UINT32 i = 0; i<Meshes.size(); ++i)
				Meshes[i]->setMaterialFlag(flag, newvalue);
		}

		//! set the hardware mapping hint, for driver
		virtual void setHardwareMappingHint(E_HARDWARE_MAPPING newMappingHint, E_BUFFER_TYPE buffer = EBT_VERTEX_AND_INDEX)
		{
			for (UINT32 i = 0; i<Meshes.size(); ++i)
				Meshes[i]->setHardwareMappingHint(newMappingHint, buffer);
		}

		//! flags the meshbuffer as changed, reloads hardware buffers
		virtual void setDirty(E_BUFFER_TYPE buffer = EBT_VERTEX_AND_INDEX)
		{
			for (UINT32 i = 0; i<Meshes.size(); ++i)
				Meshes[i]->setDirty(buffer);
		}

		//! All meshes defining the animated mesh
		vector<IMesh*>::type Meshes;

		//! The bounding box of this mesh
		AxisAlignedBox Box;

		//! Default animation speed of this mesh.
		FLOAT32 FramesPerSecond;

		//! The type of the mesh.
		E_ANIMATED_MESH_TYPE Type;
	};
}

#endif