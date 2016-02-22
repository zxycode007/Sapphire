#ifndef _SAPPHIRE_C_SHADOW_VOLUME_SCENE_NODE_
#define _SAPPHIRE_C_SHADOW_VOLUME_SCENE_NODE_

#include "SapphireIShadowVolumeSceneNode.h"

namespace Sapphire
{
	//! Scene node for rendering a shadow volume into a stencil buffer.
	class CShadowVolumeSceneNode : public IShadowVolumeSceneNode
	{
	public:

		//! constructor
		CShadowVolumeSceneNode(const IMesh* shadowMesh, ISceneNode* parent, ISceneManager* mgr,
			SINT32 id, bool zfailmethod = true, FLOAT32 infinity = 10000.0f);

		//! destructor
		virtual ~CShadowVolumeSceneNode();

		//! Sets the mesh from which the shadow volume should be generated.
		/** To optimize shadow rendering, use a simpler mesh for shadows.
		*/
		virtual void setShadowMesh(const IMesh* mesh);

		//! Updates the shadow volumes for current light positions.
		/** Called each render cycle from Animated Mesh SceneNode render method. */
		virtual void updateShadowVolumes();

		//! pre render method
		virtual void OnRegisterSceneNode();

		//! renders the node.
		virtual void render();

		//! returns the axis aligned bounding box of this node
		virtual const AxisAlignedBox& getBoundingBox() const;

		//! Returns type of the scene node
		virtual ESCENE_NODE_TYPE getType() const { return ESNT_SHADOW_VOLUME; }

	private:

		typedef vector<Vector3>::type SShadowVolume;

		void createShadowVolume(const Vector3& pos, bool isDirectional = false);
		UINT32 createEdgesAndCaps(const Vector3& light, SShadowVolume* svp, AxisAlignedBox* bb);

		//! Generates adjacency information based on mesh indices.
		void calculateAdjacency();

		AxisAlignedBox Box;

		// a shadow volume for every light
		vector<SShadowVolume>::type ShadowVolumes;

		// a back cap bounding box for every light
		vector<AxisAlignedBox >::type ShadowBBox;

		vector<Vector3>::type Vertices;
		vector<UINT16>::type Indices;
		vector<UINT16>::type Adjacency;
		vector<UINT16>::type Edges;
		// tells if face is front facing
		vector<bool>::type FaceData;

		const IMesh* ShadowMesh;

		UINT32 IndexCount;
		UINT32 VertexCount;
		UINT32 ShadowVolumesUsed;

		FLOAT32 Infinity;

		bool UseZFailMethod;
	};
}

#endif