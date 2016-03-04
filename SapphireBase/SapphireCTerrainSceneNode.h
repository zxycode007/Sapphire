#ifndef _SAPPHIRE_C_TERRIAN_SCENE_NODE_
#define _SAPPHIRE_C_TERRIAN_SCENE_NODE_

#include "SapphireITerrianSceneNode.h"
#include "SapphireIDynamicMeshBuffer.h"

namespace Sapphire
{
	class IFileSystem;
	class IReadFile;
}
namespace Sapphire
{
	struct SMesh;
	class ITextSceneNode;

	//! A scene node for displaying terrain using the geo mip map algorithm.
	class CTerrainSceneNode : public ITerrainSceneNode
	{
	public:

		//! constructor
		//! \param parent: The node which this node is a child of.  Making this node a child of another node, or
		//! making it a parent of another node is yet untested and most likely does not work properly.
		//! \param mgr: Pointer to the scene manager.
		//! \param id: The id of the node
		//! \param maxLOD: The maximum LOD ( Level of Detail ) for the node.
		//! \param patchSize: An E_GEOMIPMAP_PATCH_SIZE enumeration defining the size of each patch of the terrain.
		//! \param position: The absolute position of this node.
		//! \param rotation: The absolute rotation of this node. ( NOT YET IMPLEMENTED )
		//! \param scale: The scale factor for the terrain.  If you're using a heightmap of size 128x128 and would like
		//! your terrain to be 12800x12800 in game units, then use a scale factor of ( vector ( 100.0f, 100.0f, 100.0f ).
		//! If you use a Y scaling factor of 0.0f, then your terrain will be flat.
		CTerrainSceneNode(ISceneNode* parent, ISceneManager* mgr, IFileSystem* fs, SINT32 id,
			SINT32 maxLOD = 4, E_TERRAIN_PATCH_SIZE patchSize = ETPS_17,
			const Vector3& position = Vector3(0.0f, 0.0f, 0.0f),
			const Vector3& rotation = Vector3(0.0f, 0.0f, 0.0f),
			const Vector3& scale = Vector3(1.0f, 1.0f, 1.0f));

		virtual ~CTerrainSceneNode();

		//! Initializes the terrain data.  Loads the vertices from the heightMapFile.
		virtual bool loadHeightMap(IReadFile* file,
			ColourValue vertexColor = ColourValue::getColourValue(255, 255, 255, 255), SINT32 smoothFactor = 0);

		//! Initializes the terrain data.  Loads the vertices from the heightMapFile.
		virtual bool loadHeightMapRAW(IReadFile* file, SINT32 bitsPerPixel = 16,
			bool signedData = true, bool floatVals = false, SINT32 width = 0, ColourValue vertexColor = ColourValue(255, 255, 255, 255), SINT32 smoothFactor = 0);

		//! Returns the material based on the zero based index i. This scene node only uses
		//! 1 material.
		//! \param i: Zero based index i. UNUSED, left in for virtual purposes.
		//! \return Returns the single material this scene node uses.
		virtual SMaterial& getMaterial(UINT32 i);

		//! Returns amount of materials used by this scene node ( always 1 )
		//! \return Returns current count of materials used by this scene node ( always 1 )
		virtual UINT32 getMaterialCount() const;

		//! Gets the last scaling factor applied to the scene node.  This value only represents the
		//! last scaling factor presented to the node.  For instance, if you make create the node
		//! with a scale factor of ( 1.0f, 1.0f, 1.0f ) then call setScale ( 50.0f, 5.0f, 50.0f ),
		//! then make another call to setScale with the values ( 2.0f, 2.0f, 2.0f ), this will return
		//! Vector3 ( 2.0f, 2.0f, 2.0f ), although the total scaling of the scene node is
		//! Vector3 ( 100.0f, 10.0f, 100.0f ).
		//! \return Returns the last scaling factor passed to the scene node.
		virtual const Vector3& getScale() const
		{
			return TerrainData.Scale;
		}

		//! Scales the scene nodes vertices by the vector specified.
		//! \param scale: Scaling factor to apply to the node.
		virtual void setScale(const Vector3& scale);

		//! Gets the last rotation factor applied to the scene node.
		//! \return Returns the last rotation factor applied to the scene node.
		virtual const Vector3& getRotation() const
		{
			return TerrainData.Rotation;
		}

		//! Rotates the node. This only modifies the relative rotation of the node.
		//! \param rotation: New rotation of the node in degrees.
		virtual void setRotation(const Vector3& rotation);

		//! Sets the pivot point for rotation of this node.
		//! NOTE: The default for the RotationPivot will be the center of the individual tile.
		virtual void setRotationPivot(const Vector3& pivot);

		//! Gets the last positioning vector applied to the scene node.
		//! \return Returns the last position vector applied to the scene node.
		virtual const Vector3& getPosition() const
		{
			return TerrainData.Position;
		}

		//! Moves the scene nodes vertices by the vector specified.
		//! \param newpos: Vector specifying how much to move each vertex of the scene node.
		virtual void setPosition(const Vector3& newpos);

		//! Updates the scene nodes indices if the camera has moved or rotated by a certain
		//! threshold, which can be changed using the SetCameraMovementDeltaThreshold and
		//! SetCameraRotationDeltaThreshold functions.  This also determines if a given patch
		//! for the scene node is within the view frustum and if it's not the indices are not
		//! generated for that patch.
		virtual void OnRegisterSceneNode();

		//! Render the scene node
		virtual void render();

		//! Return the bounding box of the entire terrain.
		virtual const AxisAlignedBox& getBoundingBox() const;

		//! Return the bounding box of a patch
		virtual const AxisAlignedBox& getBoundingBox(SINT32 patchX, SINT32 patchZ) const;

		//! Return the number of indices currently used to draw the scene node.
		virtual UINT32 getIndexCount() const { return IndicesToRender; }

		//! Returns the mesh
		virtual IMesh* getMesh();

		//! Returns a pointer to the buffer used by the terrain (most users will not need this)
		virtual IMeshBuffer* getRenderBuffer() { return RenderBuffer; }

		//! Gets the meshbuffer data based on a specified Level of Detail.
		//! \param mb: A reference to an IDynamicMeshBuffer object
		//! \param LOD: The Level Of Detail you want the indices from.
		virtual void getMeshBufferForLOD(IDynamicMeshBuffer& mb, SINT32 LOD = 0) const;

		//! Gets the indices for a specified patch at a specified Level of Detail.
		//! \param indices: A reference to an array of UINT32 indices.
		//! \param patchX: Patch x coordinate.
		//! \param patchZ: Patch z coordinate.
		//! \param LOD: The level of detail to get for that patch.  If -1, then get
		//! the CurrentLOD.  If the CurrentLOD is set to -1, meaning it's not shown,
		//! then it will retrieve the triangles at the highest LOD (0).
		//! \return: Number of indices put into the buffer.
		virtual SINT32 getIndicesForPatch(vector<UINT32>::type& indices,
			SINT32 patchX, SINT32 patchZ, SINT32 LOD = 0);

		//! Populates an array with the CurrentLOD of each patch.
		//! \param LODs: A reference to a array<SINT32> to hold the values
		//! \return Returns the number of elements in the array
		virtual SINT32 getCurrentLODOfPatches(vector<SINT32>::type& LODs) const;

		//! Manually sets the LOD of a patch
		//! \param patchX: Patch x coordinate.
		//! \param patchZ: Patch z coordinate.
		//! \param LOD: The level of detail to set the patch to.
		virtual void setLODOfPatch(SINT32 patchX, SINT32 patchZ, SINT32 LOD = 0);

		//! Returns center of terrain.
		virtual const Vector3& getTerrainCenter() const
		{
			return TerrainData.Center;
		}

		//! Returns center of terrain.
		virtual FLOAT32 getHeight(FLOAT32 x, FLOAT32 y) const;

		//! Sets the movement camera threshold which is used to determine when to recalculate
		//! indices for the scene node.  The default value is 10.0f.
		virtual void setCameraMovementDelta(FLOAT32 delta)
		{
			CameraMovementDelta = delta;
		}

		//! Sets the rotation camera threshold which is used to determine when to recalculate
		//! indices for the scene node.  The default value is 1.0f.
		virtual void setCameraRotationDelta(FLOAT32 delta)
		{
			CameraRotationDelta = delta;
		}

		//! Sets whether or not the node should dynamically update it its associated selector when
		//! the geomipmap data changes.
		//! param bVal: Boolean value representing whether or not to update selector dynamically.
		//! NOTE: Temporarily disabled while working out issues with DynamicSelectorUpdate
		virtual void setDynamicSelectorUpdate(bool bVal) { DynamicSelectorUpdate = false; }

		//! Override the default generation of distance thresholds for determining the LOD a patch
		//! is rendered at. If any LOD is overridden, then the scene node will no longer apply
		//! scaling factors to these values. If you override these distances and then apply
		//! a scale to the scene node, it is your responsibility to update the new distances to
		//! work best with your new terrain size.
		virtual bool overrideLODDistance(SINT32 LOD, FLOAT64 newDistance);

		//! Scales the two textures
		virtual void scaleTexture(FLOAT32 scale = 1.0f, FLOAT32 scale2 = 0.0f);

		//! Returns type of the scene node
		virtual ESCENE_NODE_TYPE getType() const { return ESNT_TERRAIN; }

		//! Writes attributes of the scene node.
		virtual void serializeAttributes(IAttributes* out,
			SAttributeReadWriteOptions* options = 0) const;

		//! Reads attributes of the scene node.
		virtual void deserializeAttributes(IAttributes* in,
			SAttributeReadWriteOptions* options = 0);

		//! Creates a clone of this scene node and its children.
		virtual ISceneNode* clone(ISceneNode* newParent,
			ISceneManager* newManager);

	private:
		friend class CTerrainTriangleSelector;

		struct SPatch
		{
			SPatch()
				: Top(0), Bottom(0), Right(0), Left(0), CurrentLOD(-1)
			{
			}

			SPatch* Top;
			SPatch* Bottom;
			SPatch* Right;
			SPatch* Left;
			SINT32 CurrentLOD;
			AxisAlignedBox BoundingBox;
			Vector3 Center;
		};

		struct STerrainData
		{
			STerrainData(SINT32 patchSize, SINT32 maxLOD, const Vector3& position, const Vector3& rotation, const Vector3& scale)
				: Patches(0), Size(0), Position(position), Rotation(rotation),
				Scale(scale), PatchSize(patchSize), CalcPatchSize(patchSize - 1),
				PatchCount(0), MaxLOD(maxLOD)
			{
			}

			SPatch*		Patches;
			SINT32		Size;
			Vector3	Position;
			Vector3	Rotation;
			Vector3 RotationPivot;
			Vector3	Scale;
			Vector3 Center;
			SINT32		PatchSize;
			SINT32		CalcPatchSize;
			SINT32		PatchCount;
			SINT32		MaxLOD;
			AxisAlignedBox	BoundingBox;
			vector<FLOAT64>::type LODDistanceThreshold;
		};


		virtual void preRenderCalculationsIfNeeded();

		virtual void preRenderLODCalculations();
		virtual void preRenderIndicesCalculations();

		//! get indices when generating index data for patches at varying levels of detail.
		UINT32 getIndex(const SINT32 PatchX, const SINT32 PatchZ, const SINT32 PatchIndex, UINT32 vX, UINT32 vZ) const;

		//! smooth the terrain
		void smoothTerrain(IDynamicMeshBuffer* mb, SINT32 smoothFactor);

		//! calculate smooth normals
		void calculateNormals(IDynamicMeshBuffer* mb);

		//! create patches, stuff that needs to only be done once for patches goes here.
		void createPatches();

		//! calculate the internal STerrainData structure
		void calculatePatchData();

		//! calculate or recalculate the distance thresholds
		void calculateDistanceThresholds(bool scalechanged = false);

		//! sets the CurrentLOD of all patches to the specified LOD
		void setCurrentLODOfPatches(SINT32 i);

		//! sets the CurrentLOD of TerrainData patches to the LODs specified in the array
		void setCurrentLODOfPatches(const vector<SINT32>::type& lodarray);

		//! Apply transformation changes( scale, position, rotation )
		void applyTransformation();

		STerrainData TerrainData;
		SMesh* Mesh;

		IDynamicMeshBuffer *RenderBuffer;

		UINT32 VerticesToRender;
		UINT32 IndicesToRender;

		bool DynamicSelectorUpdate;
		bool OverrideDistanceThreshold;
		bool UseDefaultRotationPivot;
		bool ForceRecalculation;

		Vector3	OldCameraPosition;
		Vector3	OldCameraRotation;
		Vector3	OldCameraUp;
		FLOAT32             OldCameraFOV;
		FLOAT32 CameraMovementDelta;
		FLOAT32 CameraRotationDelta;
		FLOAT32 CameraFOVDelta;

		// needed for (de)serialization
		FLOAT32 TCoordScale1;
		FLOAT32 TCoordScale2;
		SINT32 SmoothFactor;
		path HeightmapFile;
		IFileSystem* FileSystem;
	};

}

#endif