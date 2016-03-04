#ifndef _SAPPHIRE_C_SKINNED_MESH_
#define _SAPPHIRE_C_SKINNED_MESH_

#include "SapphireISkinnedMesh.h"
#include "SapphireSMeshBuffer.h"
#include "SapphireSVertex.h"
#include "SapphireMatrix4.h"
#include "SapphireQuaternion.h"

namespace Sapphire
{
	class IAnimatedMeshSceneNode;
	class IBoneSceneNode;
	class ISceneManager;

	class CSkinnedMesh : public ISkinnedMesh
	{
	public:

		//! constructor
		CSkinnedMesh();

		//! destructor
		virtual ~CSkinnedMesh();

		//! returns the amount of frames. If the amount is 1, it is a static (=non animated) mesh.
		virtual UINT32 getFrameCount() const;

		//! Gets the default animation speed of the animated mesh.
		/** \return Amount of frames per second. If the amount is 0, it is a static, non animated mesh. */
		virtual FLOAT32 getAnimationSpeed() const;

		//! Gets the frame count of the animated mesh.
		/** \param fps Frames per second to play the animation with. If the amount is 0, it is not animated.
		The actual speed is set in the scene node the mesh is instantiated in.*/
		virtual void setAnimationSpeed(FLOAT32 fps);

		//! returns the animated mesh based on a detail level (which is ignored)
		virtual IMesh* getMesh(SINT32 frame, SINT32 detailLevel = 255, SINT32 startFrameLoop = -1, SINT32 endFrameLoop = -1);

		//! Animates this mesh's joints based on frame input
		//! blend: {0-old position, 1-New position}
		virtual void animateMesh(FLOAT32 frame, FLOAT32 blend);

		//! Preforms a software skin on this mesh based of joint positions
		virtual void skinMesh();

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

		//! Gets joint count.
		virtual UINT32 getJointCount() const;

		//! Gets the name of a joint.
		virtual const c8* getJointName(UINT32 number) const;

		//! Gets a joint number from its name
		virtual SINT32 getJointNumber(const c8* name) const;

		//! uses animation from another mesh
		virtual bool useAnimationFrom(const ISkinnedMesh *mesh);

		//! Update Normals when Animating
		//! False= Don't (default)
		//! True = Update normals, slower
		virtual void updateNormalsWhenAnimating(bool on);

		//! Sets Interpolation Mode
		virtual void setInterpolationMode(E_INTERPOLATION_MODE mode);

		//! Convertes the mesh to contain tangent information
		virtual void convertMeshToTangents();

		//! Does the mesh have no animation
		virtual bool isStatic();

		//! (This feature is not implemented in irrlicht yet)
		virtual bool setHardwareSkinning(bool on);

		//Interface for the mesh loaders (finalize should lock these functions, and they should have some prefix like loader_
		//these functions will use the needed vectors, set values, etc to help the loaders

		//! exposed for loaders to add mesh buffers
		virtual vector<SSkinMeshBuffer*>::type &getMeshBuffers();

		//! alternative method for adding joints
		virtual vector<SJoint*>::type &getAllJoints();

		//! alternative method for adding joints
		virtual const vector<SJoint*>::type &getAllJoints() const;

		//! loaders should call this after populating the mesh
		virtual void finalize();

		//! Adds a new meshbuffer to the mesh, access it as last one
		virtual SSkinMeshBuffer *addMeshBuffer();

		//! Adds a new joint to the mesh, access it as last one
		virtual SJoint *addJoint(SJoint *parent = 0);

		//! Adds a new position key to the mesh, access it as last one
		virtual SPositionKey *addPositionKey(SJoint *joint);
		//! Adds a new rotation key to the mesh, access it as last one
		virtual SRotationKey *addRotationKey(SJoint *joint);
		//! Adds a new scale key to the mesh, access it as last one
		virtual SScaleKey *addScaleKey(SJoint *joint);

		//! Adds a new weight to the mesh, access it as last one
		virtual SWeight *addWeight(SJoint *joint);

		virtual void updateBoundingBox(void);

		//! Recovers the joints from the mesh
		void recoverJointsFromMesh(vector<IBoneSceneNode*>::type &jointChildSceneNodes);

		//! Tranfers the joint data to the mesh
		void transferJointsToMesh(const vector<IBoneSceneNode*>::type &jointChildSceneNodes);

		//! Tranfers the joint hints to the mesh
		void transferOnlyJointsHintsToMesh(const vector<IBoneSceneNode*>::type &jointChildSceneNodes);

		//! Creates an vector of joints from this mesh as children of node
		void addJoints(vector<IBoneSceneNode*>::type &jointChildSceneNodes,
			IAnimatedMeshSceneNode* node,
			ISceneManager* smgr);

	private:
		void checkForAnimation();

		void normalizeWeights();

		//构建本地动画矩阵列表
		void buildAllLocalAnimatedMatrices();
		//构建全局动画矩阵列表
		void buildAllGlobalAnimatedMatrices(SJoint *Joint = 0, SJoint *ParentJoint = 0);
		//获取帧数据
		void getFrameData(FLOAT32 frame, SJoint *Node,
			Vector3 &position, SINT32 &positionHint,
			Vector3 &scale, SINT32 &scaleHint,
			Quaternion &rotation, SINT32 &rotationHint);

		void calculateGlobalMatrices(SJoint *Joint, SJoint *ParentJoint);

		void skinJoint(SJoint *Joint, SJoint *ParentJoint);

		void calculateTangents(Vector3& normal,
			Vector3& tangent, Vector3& binormal,
			Vector3& vt1, Vector3& vt2, Vector3& vt3,
			Vector2& tc1, Vector2& tc2, Vector2& tc3);

		vector<SSkinMeshBuffer*>::type *SkinningBuffers; //Meshbuffer to skin, default is to skin localBuffers

		vector<SSkinMeshBuffer*>::type LocalBuffers;

		vector<SJoint*>::type AllJoints;
		vector<SJoint*>::type RootJoints;

		vector< deque<bool>::type >::type Vertices_Moved;

		AxisAlignedBox BoundingBox;

		FLOAT32 AnimationFrames;
		FLOAT32 FramesPerSecond;

		FLOAT32 LastAnimatedFrame;
		bool SkinnedLastFrame;

		E_INTERPOLATION_MODE InterpolationMode : 8;

		bool HasAnimation;
		bool PreparedForSkinning;
		bool AnimateNormals;
		bool HardwareSkinning;
	};
}

#endif