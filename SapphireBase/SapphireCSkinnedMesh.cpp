#include "SapphireCompileConifg.h"
#include "SapphireCSkinnedMesh.h"
#include "SapphireCBoneSceneNode.h"
#include "SapphireIAnimatedMeshSceneNode.h"
#include "SapphireOS.h"

namespace Sapphire
{

	//! constructor
	CSkinnedMesh::CSkinnedMesh()
		: SkinningBuffers(0), AnimationFrames(0.f), FramesPerSecond(25.f),
		LastAnimatedFrame(-1), SkinnedLastFrame(false),
		InterpolationMode(EIM_LINEAR),
		HasAnimation(false), PreparedForSkinning(false),
		AnimateNormals(true), HardwareSkinning(false)
	{
#ifdef _DEBUG
		setDebugName("CSkinnedMesh");
#endif

		SkinningBuffers = &LocalBuffers;
	}


	//! destructor
	CSkinnedMesh::~CSkinnedMesh()
	{
		for (UINT32 i = 0; i<AllJoints.size(); ++i)
			delete AllJoints[i];

		for (UINT32 j = 0; j<LocalBuffers.size(); ++j)
		{
			if (LocalBuffers[j])
				LocalBuffers[j]->drop();
		}
	}


	//! returns the amount of frames in milliseconds.
	//! If the amount is 1, it is a static (=non animated) mesh.
	UINT32 CSkinnedMesh::getFrameCount() const
	{
		return floor32(AnimationFrames);
	}


	//! Gets the default animation speed of the animated mesh.
	/** \return Amount of frames per second. If the amount is 0, it is a static, non animated mesh. */
	FLOAT32 CSkinnedMesh::getAnimationSpeed() const
	{
		return FramesPerSecond;
	}


	//! Gets the frame count of the animated mesh.
	/** \param fps Frames per second to play the animation with. If the amount is 0, it is not animated.
	The actual speed is set in the scene node the mesh is instantiated in.*/
	void CSkinnedMesh::setAnimationSpeed(FLOAT32 fps)
	{
		FramesPerSecond = fps;
	}


	//! returns the animated mesh based on a detail level. 0 is the lowest, 255 the highest detail. Note, that some Meshes will ignore the detail level.
	IMesh* CSkinnedMesh::getMesh(SINT32 frame, SINT32 detailLevel, SINT32 startFrameLoop, SINT32 endFrameLoop)
	{
		//animate(frame,startFrameLoop, endFrameLoop);
		if (frame == -1)
			return this;

		animateMesh((FLOAT32)frame, 1.0f);
		skinMesh();
		return this;
	}


	//--------------------------------------------------------------------------
	//			Keyframe Animation
	//--------------------------------------------------------------------------


	//! Animates this mesh's joints based on frame input
	//! blend: {0-old position, 1-New position}
	void CSkinnedMesh::animateMesh(FLOAT32 frame, FLOAT32 blend)
	{
		if (!HasAnimation || LastAnimatedFrame == frame)
			return;

		LastAnimatedFrame = frame;
		SkinnedLastFrame = false;

		if (blend <= 0.f)
			return; //No need to animate

		for (UINT32 i = 0; i<AllJoints.size(); ++i)
		{
			//The joints can be animated here with no input from their
			//parents, but for setAnimationMode extra checks are needed
			//to their parents
			SJoint *joint = AllJoints[i];

			const Vector3 oldPosition = joint->Animatedposition;
			const Vector3 oldScale = joint->Animatedscale;
			const Quaternion oldRotation = joint->Animatedrotation;

			Vector3 position = oldPosition;
			Vector3 scale = oldScale;
			Quaternion rotation = oldRotation;

			getFrameData(frame, joint,
				position, joint->positionHint,
				scale, joint->scaleHint,
				rotation, joint->rotationHint);

			if (blend == 1.0f)
			{
				//No blending needed
				joint->Animatedposition = position;
				joint->Animatedscale = scale;
				joint->Animatedrotation = rotation;
			}
			else
			{
				//Blend animation
				joint->Animatedposition = Math::lerp(oldPosition, position, blend);
				joint->Animatedscale = Math::lerp(oldScale, scale, blend);
				//joint->Animatedrotation.slerp(oldRotation, rotation, blend);
				joint->Animatedrotation.Slerp(oldRotation, rotation, blend);
			}
		}

		//Note:
		//LocalAnimatedMatrix needs to be built at some point, but this function may be called lots of times for
		//one render (to play two animations at the same time) LocalAnimatedMatrix only needs to be built once.
		//a call to buildAllLocalAnimatedMatrices is needed before skinning the mesh, and before the user gets the joints to move

		//----------------
		// Temp!
		buildAllLocalAnimatedMatrices();
		//-----------------

		updateBoundingBox();
	}


	void CSkinnedMesh::buildAllLocalAnimatedMatrices()
	{
		//遍历所有joint
		for (UINT32 i = 0; i<AllJoints.size(); ++i)
		{
			//当前joint
			SJoint *joint = AllJoints[i];

			//Could be faster:

			if (joint->UseAnimationFrom &&
				(joint->UseAnimationFrom->PositionKeys.size() ||
				joint->UseAnimationFrom->ScaleKeys.size() ||
				joint->UseAnimationFrom->RotationKeys.size()))
			{
				joint->GlobalSkinningSpace = false;

				// IRR_TEST_BROKEN_QUATERNION_USE: TODO - switched to getMatrix_transposed instead of getMatrix for downward compatibility. 
				//								   Not tested so far if this was correct or wrong before Quaternion fix!
				//joint->Animatedrotation.getMatrix_transposed(joint->LocalAnimatedMatrix);
				joint->Animatedrotation.getMatrix(joint->LocalAnimatedMatrix);

				// --- joint->LocalAnimatedMatrix *= joint->Animatedrotation.getMatrix() ---
				//设置矩阵的位移参数
				//FLOAT32 *m1 = joint->LocalAnimatedMatrix.pointer();
				//Vector3 &Pos = joint->Animatedposition;
				//m1[0] += Pos.x*m1[3];
				//m1[1] += Pos.y*m1[3];
				//m1[2] += Pos.z*m1[3];
				//m1[4] += Pos.x*m1[7];
				//m1[5] += Pos.y*m1[7];
				//m1[6] += Pos.z*m1[7];
				//m1[8] += Pos.x*m1[11];
				//m1[9] += Pos.y*m1[11];
				//m1[10] += Pos.z*m1[11];
				//m1[12] += Pos.x*m1[15];
				//m1[13] += Pos.y*m1[15];
				//m1[14] += Pos.z*m1[15];
				//设置矩阵的位移参数
				//Matrix4 m1 = joint->LocalAnimatedMatrix;
				Vector3 &Pos = joint->Animatedposition;
				joint->LocalAnimatedMatrix.setTrans(Pos);
				// -----------------------------------
				//判断缩放关键帧的数量
				if (joint->ScaleKeys.size())
				{
					/*
					Matrix4 scaleMatrix;
					scaleMatrix.setScale(joint->Animatedscale);
					joint->LocalAnimatedMatrix *= scaleMatrix;
					*/

					// -------- joint->LocalAnimatedMatrix *= scaleMatrix -----------------
					//Matrix4& mat = joint->LocalAnimatedMatrix;					
					//mat[0] *= joint->Animatedscale.x;
					//mat[1] *= joint->Animatedscale.x;
					//mat[2] *= joint->Animatedscale.x;
					//mat[3] *= joint->Animatedscale.x;
					//mat[4] *= joint->Animatedscale.y;
					//mat[5] *= joint->Animatedscale.y;
					//mat[6] *= joint->Animatedscale.y;
					//mat[7] *= joint->Animatedscale.y;
					//mat[8] *= joint->Animatedscale.z;
					//mat[9] *= joint->Animatedscale.z;
					//mat[10] *= joint->Animatedscale.z;
					//mat[11] *= joint->Animatedscale.z;
					//mat.setScale(joint->Animatedscale);
					joint->LocalAnimatedMatrix.setScale(joint->Animatedscale);
					// -----------------------------------
				}
			}
			else
			{
				joint->LocalAnimatedMatrix = joint->LocalMatrix;
			}
		}
		SkinnedLastFrame = false;
	}


	void CSkinnedMesh::buildAllGlobalAnimatedMatrices(SJoint *joint, SJoint *parentJoint)
	{
		//如果该Joint为空
		if (!joint)
		{
			//依次遍历RootJoint所有的Joint，构建全部的全局动画矩阵
			for (UINT32 i = 0; i<RootJoints.size(); ++i)
				buildAllGlobalAnimatedMatrices(RootJoints[i], 0);
			return;
		}
		else
		{
			// 查找全局矩阵
			if (!parentJoint || joint->GlobalSkinningSpace)
				joint->GlobalAnimatedMatrix = joint->LocalAnimatedMatrix;
			else
				joint->GlobalAnimatedMatrix = parentJoint->GlobalAnimatedMatrix * joint->LocalAnimatedMatrix;
		}

		for (UINT32 j = 0; j<joint->Children.size(); ++j)
			buildAllGlobalAnimatedMatrices(joint->Children[j], joint);
	}

	//获取帧数据
	void CSkinnedMesh::getFrameData(FLOAT32 frame, SJoint *joint,
		Vector3 &position, SINT32 &positionHint,
		Vector3 &scale, SINT32 &scaleHint,
		Quaternion &rotation, SINT32 &rotationHint)
	{
		SINT32 foundPositionIndex = -1;
		SINT32 foundScaleIndex = -1;
		SINT32 foundRotationIndex = -1;
		//是否使用其它网格的外部joint
		if (joint->UseAnimationFrom)
		{
			const deque<SPositionKey>::type &PositionKeys = joint->UseAnimationFrom->PositionKeys;
			const deque<SScaleKey>::type &ScaleKeys = joint->UseAnimationFrom->ScaleKeys;
			const deque<SRotationKey>::type &RotationKeys = joint->UseAnimationFrom->RotationKeys;

			if (PositionKeys.size())
			{
				foundPositionIndex = -1;

				//Test the Hints...
				if (positionHint >= 0 && (UINT32)positionHint < PositionKeys.size())
				{
					//check this hint   判断当前frame是否在PositionHint指示的帧数和PositionHint指示-1帧数之间
					if (positionHint>0 && PositionKeys[positionHint].frame >= frame && PositionKeys[positionHint - 1].frame<frame)
						foundPositionIndex = positionHint;
					else if (positionHint + 1 < (SINT32)PositionKeys.size())
					{
						//check the next index
						if (PositionKeys[positionHint + 1].frame >= frame &&
							PositionKeys[positionHint + 0].frame<frame)
						{
							positionHint++;
							foundPositionIndex = positionHint;
						}
					}
				}

				//The hint test failed, do a full scan...
				if (foundPositionIndex == -1)
				{
					for (UINT32 i = 0; i<PositionKeys.size(); ++i)
					{
						if (PositionKeys[i].frame >= frame) //Keys should to be sorted by frame
						{
							foundPositionIndex = i;
							positionHint = i;
							break;
						}
					}
				}

				//Do interpolation...
				if (foundPositionIndex != -1)
				{
					if (InterpolationMode == EIM_CONSTANT || foundPositionIndex == 0)
					{
						position = PositionKeys[foundPositionIndex].position;
					}
					else if (InterpolationMode == EIM_LINEAR)
					{
						const SPositionKey& KeyA = PositionKeys[foundPositionIndex];
						const SPositionKey& KeyB = PositionKeys[foundPositionIndex - 1];

						const FLOAT32 fd1 = frame - KeyA.frame;
						const FLOAT32 fd2 = KeyB.frame - frame;
						position = ((KeyB.position - KeyA.position) / (fd1 + fd2))*fd1 + KeyA.position;
					}
				}
			}

			//------------------------------------------------------------

			if (ScaleKeys.size())
			{
				foundScaleIndex = -1;

				//Test the Hints...
				if (scaleHint >= 0 && (UINT32)scaleHint < ScaleKeys.size())
				{
					//check this hint
					if (scaleHint>0 && ScaleKeys[scaleHint].frame >= frame && ScaleKeys[scaleHint - 1].frame<frame)
						foundScaleIndex = scaleHint;
					else if (scaleHint + 1 < (SINT32)ScaleKeys.size())
					{
						//check the next index
						if (ScaleKeys[scaleHint + 1].frame >= frame &&
							ScaleKeys[scaleHint + 0].frame<frame)
						{
							scaleHint++;
							foundScaleIndex = scaleHint;
						}
					}
				}


				//The hint test failed, do a full scan...
				if (foundScaleIndex == -1)
				{
					for (UINT32 i = 0; i<ScaleKeys.size(); ++i)
					{
						if (ScaleKeys[i].frame >= frame) //Keys should to be sorted by frame
						{
							foundScaleIndex = i;
							scaleHint = i;
							break;
						}
					}
				}

				//Do interpolation...
				if (foundScaleIndex != -1)
				{
					if (InterpolationMode == EIM_CONSTANT || foundScaleIndex == 0)
					{
						scale = ScaleKeys[foundScaleIndex].scale;
					}
					else if (InterpolationMode == EIM_LINEAR)
					{
						const SScaleKey& KeyA = ScaleKeys[foundScaleIndex];
						const SScaleKey& KeyB = ScaleKeys[foundScaleIndex - 1];

						const FLOAT32 fd1 = frame - KeyA.frame;
						const FLOAT32 fd2 = KeyB.frame - frame;
						scale = ((KeyB.scale - KeyA.scale) / (fd1 + fd2))*fd1 + KeyA.scale;
					}
				}
			}

			//-------------------------------------------------------------

			if (RotationKeys.size())
			{
				foundRotationIndex = -1;

				//Test the Hints...
				if (rotationHint >= 0 && (UINT32)rotationHint < RotationKeys.size())
				{
					//check this hint
					if (rotationHint>0 && RotationKeys[rotationHint].frame >= frame && RotationKeys[rotationHint - 1].frame<frame)
						foundRotationIndex = rotationHint;
					else if (rotationHint + 1 < (SINT32)RotationKeys.size())
					{
						//check the next index
						if (RotationKeys[rotationHint + 1].frame >= frame &&
							RotationKeys[rotationHint + 0].frame<frame)
						{
							rotationHint++;
							foundRotationIndex = rotationHint;
						}
					}
				}


				//The hint test failed, do a full scan...
				if (foundRotationIndex == -1)
				{
					for (UINT32 i = 0; i<RotationKeys.size(); ++i)
					{
						if (RotationKeys[i].frame >= frame) //Keys should be sorted by frame
						{
							foundRotationIndex = i;
							rotationHint = i;
							break;
						}
					}
				}

				//Do interpolation...
				if (foundRotationIndex != -1)
				{
					if (InterpolationMode == EIM_CONSTANT || foundRotationIndex == 0)
					{
						rotation = RotationKeys[foundRotationIndex].rotation;
					}
					else if (InterpolationMode == EIM_LINEAR)
					{
						const SRotationKey& KeyA = RotationKeys[foundRotationIndex];
						const SRotationKey& KeyB = RotationKeys[foundRotationIndex - 1];

						const FLOAT32 fd1 = frame - KeyA.frame;
						const FLOAT32 fd2 = KeyB.frame - frame;
						const FLOAT32 t = fd1 / (fd1 + fd2);

						/*
						FLOAT32 t = 0;
						if (KeyA.frame!=KeyB.frame)
						t = (frame-KeyA.frame) / (KeyB.frame - KeyA.frame);
						*/

						rotation.Slerp(KeyA.rotation, KeyB.rotation, t);
					}
				}
			}
		}
	}

	//--------------------------------------------------------------------------
	//				Software Skinning 软件蒙皮
	//--------------------------------------------------------------------------

	//! Preforms a software skin on this mesh based of joint positions
	void CSkinnedMesh::skinMesh()
	{
		if (!HasAnimation || SkinnedLastFrame)
			return;

		//----------------
		// This is marked as "Temp!".  A shiny dubloon to whomever can tell me why.
		buildAllGlobalAnimatedMatrices();
		//-----------------

		SkinnedLastFrame = true;
		if (!HardwareSkinning)
		{
			//Software skin....
			UINT32 i;

			//rigid animation
			for (i = 0; i<AllJoints.size(); ++i)
			{
				for (UINT32 j = 0; j<AllJoints[i]->AttachedMeshes.size(); ++j)
				{
					SSkinMeshBuffer* Buffer = (*SkinningBuffers)[AllJoints[i]->AttachedMeshes[j]];
					Buffer->Transformation = AllJoints[i]->GlobalAnimatedMatrix;
				}
			}

			//clear skinning helper vector
			for (i = 0; i<Vertices_Moved.size(); ++i)
				for (UINT32 j = 0; j<Vertices_Moved[i].size(); ++j)
					Vertices_Moved[i][j] = false;

			//skin starting with the root joints
			for (i = 0; i<RootJoints.size(); ++i)
				skinJoint(RootJoints[i], 0);

			for (i = 0; i<SkinningBuffers->size(); ++i)
				(*SkinningBuffers)[i]->setDirty(EBT_VERTEX);
		}
		updateBoundingBox();
	}


	void CSkinnedMesh::skinJoint(SJoint *joint, SJoint *parentJoint)
	{
		if (joint->Weights.size())
		{
			//Find this joints pull on vertices...
			//Matrix4 jointVertexPull(Matrix4::EM4CONST_NOTHING);
			Matrix4 jointVertexPull = Matrix4::ZERO;
			jointVertexPull.setbyProduct(joint->GlobalAnimatedMatrix, joint->GlobalInversedMatrix);

			Vector3 thisVertexMove, thisNormalMove;

			vector<SSkinMeshBuffer*>::type &buffersUsed = *SkinningBuffers;

			//Skin Vertices Positions and Normals...
			for (UINT32 i = 0; i<joint->Weights.size(); ++i)
			{
				SWeight& weight = joint->Weights[i];

				// Pull this vertex...
				//jointVertexPull.transformVect(thisVertexMove, weight.StaticPos);
				thisVertexMove = jointVertexPull.transformAffine(weight.StaticPos);

				if (AnimateNormals)
					//jointVertexPull.rotateVect(thisNormalMove, weight.StaticNormal);
					thisNormalMove = jointVertexPull.rotateVect(weight.StaticNormal);

				if (!(*(weight.Moved)))
				{
					*(weight.Moved) = true;

					buffersUsed[weight.buffer_id]->getVertex(weight.vertex_id)->Pos = thisVertexMove * weight.strength;

					if (AnimateNormals)
						buffersUsed[weight.buffer_id]->getVertex(weight.vertex_id)->Normal = thisNormalMove * weight.strength;

					//*(weight._Pos) = thisVertexMove * weight.strength;
				}
				else
				{
					buffersUsed[weight.buffer_id]->getVertex(weight.vertex_id)->Pos += thisVertexMove * weight.strength;

					if (AnimateNormals)
						buffersUsed[weight.buffer_id]->getVertex(weight.vertex_id)->Normal += thisNormalMove * weight.strength;

					//*(weight._Pos) += thisVertexMove * weight.strength;
				}

				buffersUsed[weight.buffer_id]->boundingBoxNeedsRecalculated();
			}
		}

		//Skin all children
		for (UINT32 j = 0; j<joint->Children.size(); ++j)
			skinJoint(joint->Children[j], joint);
	}


	E_ANIMATED_MESH_TYPE CSkinnedMesh::getMeshType() const
	{
		return EAMT_SKINNED;
	}


	//! Gets joint count.
	UINT32 CSkinnedMesh::getJointCount() const
	{
		return AllJoints.size();
	}


	//! Gets the name of a joint.
	const c8* CSkinnedMesh::getJointName(UINT32 number) const
	{
		if (number >= AllJoints.size())
			return 0;
		return AllJoints[number]->Name.c_str();
	}


	//! Gets a joint number from its name
	SINT32 CSkinnedMesh::getJointNumber(const c8* name) const
	{
		for (UINT32 i = 0; i<AllJoints.size(); ++i)
		{
			if (AllJoints[i]->Name == name)
				return i;
		}

		return -1;
	}


	//! returns amount of mesh buffers.
	UINT32 CSkinnedMesh::getMeshBufferCount() const
	{
		return LocalBuffers.size();
	}


	//! returns pointer to a mesh buffer
	IMeshBuffer* CSkinnedMesh::getMeshBuffer(UINT32 nr) const
	{
		if (nr < LocalBuffers.size())
			return LocalBuffers[nr];
		else
			return 0;
	}


	//! Returns pointer to a mesh buffer which fits a material
	IMeshBuffer* CSkinnedMesh::getMeshBuffer(const SMaterial &material) const
	{
		for (UINT32 i = 0; i<LocalBuffers.size(); ++i)
		{
			if (LocalBuffers[i]->getMaterial() == material)
				return LocalBuffers[i];
		}
		return 0;
	}


	//! returns an axis aligned bounding box
	const AxisAlignedBox& CSkinnedMesh::getBoundingBox() const
	{
		return BoundingBox;
	}


	//! set user axis aligned bounding box
	void CSkinnedMesh::setBoundingBox(const AxisAlignedBox& box)
	{
		BoundingBox = box;
	}


	//! sets a flag of all contained materials to a new value
	void CSkinnedMesh::setMaterialFlag(E_MATERIAL_FLAG flag, bool newvalue)
	{
		for (UINT32 i = 0; i<LocalBuffers.size(); ++i)
			LocalBuffers[i]->Material.setFlag(flag, newvalue);
	}


	//! set the hardware mapping hint, for driver
	void CSkinnedMesh::setHardwareMappingHint(E_HARDWARE_MAPPING newMappingHint,
		E_BUFFER_TYPE buffer)
	{
		for (UINT32 i = 0; i<LocalBuffers.size(); ++i)
			LocalBuffers[i]->setHardwareMappingHint(newMappingHint, buffer);
	}


	//! flags the meshbuffer as changed, reloads hardware buffers
	void CSkinnedMesh::setDirty(E_BUFFER_TYPE buffer)
	{
		for (UINT32 i = 0; i<LocalBuffers.size(); ++i)
			LocalBuffers[i]->setDirty(buffer);
	}


	//! uses animation from another mesh
	bool CSkinnedMesh::useAnimationFrom(const ISkinnedMesh *mesh)
	{
		bool unmatched = false;

		for (UINT32 i = 0; i<AllJoints.size(); ++i)
		{
			SJoint *joint = AllJoints[i];
			joint->UseAnimationFrom = 0;

			if (joint->Name == "")
				unmatched = true;
			else
			{
				for (UINT32 j = 0; j<mesh->getAllJoints().size(); ++j)
				{
					SJoint *otherJoint = mesh->getAllJoints()[j];
					if (joint->Name == otherJoint->Name)
					{
						joint->UseAnimationFrom = otherJoint;
					}
				}
				if (!joint->UseAnimationFrom)
					unmatched = true;
			}
		}

		checkForAnimation();

		return !unmatched;
	}


	//!Update Normals when Animating
	//!False= Don't animate them, faster
	//!True= Update normals (default)
	void CSkinnedMesh::updateNormalsWhenAnimating(bool on)
	{
		AnimateNormals = on;
	}


	//!Sets Interpolation Mode
	void CSkinnedMesh::setInterpolationMode(E_INTERPOLATION_MODE mode)
	{
		InterpolationMode = mode;
	}


	vector<SSkinMeshBuffer*>::type &CSkinnedMesh::getMeshBuffers()
	{
		return LocalBuffers;
	}


	vector<CSkinnedMesh::SJoint*>::type &CSkinnedMesh::getAllJoints()
	{
		return AllJoints;
	}


	const vector<CSkinnedMesh::SJoint*>::type &CSkinnedMesh::getAllJoints() const
	{
		return AllJoints;
	}


	//! (This feature is not implementated in irrlicht yet)
	bool CSkinnedMesh::setHardwareSkinning(bool on)
	{
		if (HardwareSkinning != on)
		{
			if (on)
			{

				//set mesh to static pose...
				for (UINT32 i = 0; i<AllJoints.size(); ++i)
				{
					SJoint *joint = AllJoints[i];
					for (UINT32 j = 0; j<joint->Weights.size(); ++j)
					{
						const UINT16 buffer_id = joint->Weights[j].buffer_id;
						const UINT32 vertex_id = joint->Weights[j].vertex_id;
						LocalBuffers[buffer_id]->getVertex(vertex_id)->Pos = joint->Weights[j].StaticPos;
						LocalBuffers[buffer_id]->getVertex(vertex_id)->Normal = joint->Weights[j].StaticNormal;
						LocalBuffers[buffer_id]->boundingBoxNeedsRecalculated();
					}
				}
			}

			HardwareSkinning = on;
		}
		return HardwareSkinning;
	}


	void CSkinnedMesh::calculateGlobalMatrices(SJoint *joint, SJoint *parentJoint)
	{
		if (!joint && parentJoint) // bit of protection from endless loops
			return;

		//Go through the root bones
		if (!joint)
		{
			for (UINT32 i = 0; i<RootJoints.size(); ++i)
				calculateGlobalMatrices(RootJoints[i], 0);
			return;
		}

		if (!parentJoint)
			joint->GlobalMatrix = joint->LocalMatrix;
		else
			joint->GlobalMatrix = parentJoint->GlobalMatrix * joint->LocalMatrix;

		joint->LocalAnimatedMatrix = joint->LocalMatrix;
		joint->GlobalAnimatedMatrix = joint->GlobalMatrix;

		if (joint->GlobalInversedMatrix.isIdentify())//might be pre calculated
		{
			joint->GlobalInversedMatrix = joint->GlobalMatrix;
			//joint->GlobalInversedMatrix.makeInverse(); // slow
			joint->GlobalInversedMatrix = joint->GlobalInversedMatrix.inverse();
		}

		for (UINT32 j = 0; j<joint->Children.size(); ++j)
			calculateGlobalMatrices(joint->Children[j], joint);
		SkinnedLastFrame = false;
	}


	void CSkinnedMesh::checkForAnimation()
	{
		UINT32 i, j;
		//Check for animation...
		HasAnimation = false;
		for (i = 0; i<AllJoints.size(); ++i)
		{
			if (AllJoints[i]->UseAnimationFrom)
			{
				if (AllJoints[i]->UseAnimationFrom->PositionKeys.size() ||
					AllJoints[i]->UseAnimationFrom->ScaleKeys.size() ||
					AllJoints[i]->UseAnimationFrom->RotationKeys.size())
				{
					HasAnimation = true;
				}
			}
		}

		//meshes with weights, are still counted as animated for ragdolls, etc
		if (!HasAnimation)
		{
			for (i = 0; i<AllJoints.size(); ++i)
			{
				if (AllJoints[i]->Weights.size())
					HasAnimation = true;
			}
		}

		if (HasAnimation)
		{
			//--- Find the length of the animation ---
			AnimationFrames = 0;
			for (i = 0; i<AllJoints.size(); ++i)
			{
				if (AllJoints[i]->UseAnimationFrom)
				{
					if (AllJoints[i]->UseAnimationFrom->PositionKeys.size())
						if (AllJoints[i]->UseAnimationFrom->PositionKeys.back().frame > AnimationFrames)
							AnimationFrames = AllJoints[i]->UseAnimationFrom->PositionKeys.back().frame;

					if (AllJoints[i]->UseAnimationFrom->ScaleKeys.size())
						if (AllJoints[i]->UseAnimationFrom->ScaleKeys.back().frame > AnimationFrames)
							AnimationFrames = AllJoints[i]->UseAnimationFrom->ScaleKeys.back().frame;

					if (AllJoints[i]->UseAnimationFrom->RotationKeys.size())
						if (AllJoints[i]->UseAnimationFrom->RotationKeys.back().frame > AnimationFrames)
							AnimationFrames = AllJoints[i]->UseAnimationFrom->RotationKeys.back().frame;
				}
			}
		}

		if (HasAnimation && !PreparedForSkinning)
		{
			PreparedForSkinning = true;

			//check for bugs:
			for (i = 0; i < AllJoints.size(); ++i)
			{
				SJoint *joint = AllJoints[i];
				for (j = 0; j<joint->Weights.size(); ++j)
				{
					const UINT16 buffer_id = joint->Weights[j].buffer_id;
					const UINT32 vertex_id = joint->Weights[j].vertex_id;

					//check for invalid ids
					if (buffer_id >= LocalBuffers.size())
					{
						Printer::log("Skinned Mesh: Weight buffer id too large", LML_WARNING);
						joint->Weights[j].buffer_id = joint->Weights[j].vertex_id = 0;
					}
					else if (vertex_id >= LocalBuffers[buffer_id]->getVertexCount())
					{
						Printer::log("Skinned Mesh: Weight vertex id too large", LML_WARNING);
						joint->Weights[j].buffer_id = joint->Weights[j].vertex_id = 0;
					}
				}
			}

			//An vector used in skinning

			for (i = 0; i<Vertices_Moved.size(); ++i)
				for (j = 0; j<Vertices_Moved[i].size(); ++j)
					Vertices_Moved[i][j] = false;

			// For skinning: cache weight values for speed

			for (i = 0; i<AllJoints.size(); ++i)
			{
				SJoint *joint = AllJoints[i];
				for (j = 0; j<joint->Weights.size(); ++j)
				{
					const UINT16 buffer_id = joint->Weights[j].buffer_id;
					const UINT32 vertex_id = joint->Weights[j].vertex_id;

					//joint->Weights[j].Moved = &Vertices_Moved[buffer_id][vertex_id];
					joint->Weights[j].Moved = &Vertices_Moved[buffer_id][vertex_id];
					joint->Weights[j].StaticPos = LocalBuffers[buffer_id]->getVertex(vertex_id)->Pos;
					joint->Weights[j].StaticNormal = LocalBuffers[buffer_id]->getVertex(vertex_id)->Normal;

					//joint->Weights[j]._Pos=&Buffers[buffer_id]->getVertex(vertex_id)->Pos;
				}
			}

			// normalize weights
			normalizeWeights();
		}
		SkinnedLastFrame = false;
	}


	//! called by loader after populating with mesh and bone data
	void CSkinnedMesh::finalize()
	{
		UINT32 i;

		// Make sure we recalc the next frame
		LastAnimatedFrame = -1;
		SkinnedLastFrame = false;

		//calculate bounding box
		for (i = 0; i<LocalBuffers.size(); ++i)
		{
			LocalBuffers[i]->recalculateBoundingBox();
		}

		if (AllJoints.size() || RootJoints.size())
		{
			// populate AllJoints or RootJoints, depending on which is empty
			if (!RootJoints.size())
			{

				for (UINT32 CheckingIdx = 0; CheckingIdx < AllJoints.size(); ++CheckingIdx)
				{

					bool foundParent = false;
					for (i = 0; i < AllJoints.size(); ++i)
					{
						for (UINT32 n = 0; n < AllJoints[i]->Children.size(); ++n)
						{
							if (AllJoints[i]->Children[n] == AllJoints[CheckingIdx])
								foundParent = true;
						}
					}

					if (!foundParent)
						RootJoints.push_back(AllJoints[CheckingIdx]);
				}
			}
			else
			{
				AllJoints = RootJoints;
			}
		}

		for (i = 0; i < AllJoints.size(); ++i)
		{
			AllJoints[i]->UseAnimationFrom = AllJoints[i];
		}

		//Set vector sizes...

		for (i = 0; i<LocalBuffers.size(); ++i)
		{
			Vertices_Moved.push_back(deque<bool>::type());
			Vertices_Moved[i].resize(LocalBuffers[i]->getVertexCount());
		}

		//Todo: optimise keys here...

		checkForAnimation();

		if (HasAnimation)
		{
			//--- optimize and check keyframes ---
			for (i = 0; i<AllJoints.size(); ++i)
			{
				deque<SPositionKey>::type &PositionKeys = AllJoints[i]->PositionKeys;
				deque<SScaleKey>::type &ScaleKeys = AllJoints[i]->ScaleKeys;
				deque<SRotationKey>::type &RotationKeys = AllJoints[i]->RotationKeys;

				if (PositionKeys.size()>2)
				{
					for (UINT32 j = 0; j<PositionKeys.size() - 2; ++j)
					{
						if (PositionKeys[j].position == PositionKeys[j + 1].position && PositionKeys[j + 1].position == PositionKeys[j + 2].position)
						{
							PositionKeys.erase(PositionKeys.begin()+j + 1); //the middle key is unneeded
							--j;
						}
					}
				}

				if (PositionKeys.size()>1)
				{
					for (UINT32 j = 0; j<PositionKeys.size() - 1; ++j)
					{
						if (PositionKeys[j].frame >= PositionKeys[j + 1].frame) //bad frame, unneed and may cause problems
						{
							PositionKeys.erase(PositionKeys.begin() + j + 1);
							--j;
						}
					}
				}

				if (ScaleKeys.size()>2)
				{
					for (UINT32 j = 0; j<ScaleKeys.size() - 2; ++j)
					{
						if (ScaleKeys[j].scale == ScaleKeys[j + 1].scale && ScaleKeys[j + 1].scale == ScaleKeys[j + 2].scale)
						{
							ScaleKeys.erase(ScaleKeys.begin() + j + 1); //the middle key is unneeded
							--j;
						}
					}
				}

				if (ScaleKeys.size()>1)
				{
					for (UINT32 j = 0; j<ScaleKeys.size() - 1; ++j)
					{
						if (ScaleKeys[j].frame >= ScaleKeys[j + 1].frame) //bad frame, unneed and may cause problems
						{
							ScaleKeys.erase(ScaleKeys.begin() + j + 1);
							--j;
						}
					}
				}

				if (RotationKeys.size()>2)
				{
					for (UINT32 j = 0; j<RotationKeys.size() - 2; ++j)
					{
						if (RotationKeys[j].rotation == RotationKeys[j + 1].rotation && RotationKeys[j + 1].rotation == RotationKeys[j + 2].rotation)
						{
							RotationKeys.erase(RotationKeys.begin() + j + 1); //the middle key is unneeded
							--j;
						}
					}
				}

				if (RotationKeys.size()>1)
				{
					for (UINT32 j = 0; j<RotationKeys.size() - 1; ++j)
					{
						if (RotationKeys[j].frame >= RotationKeys[j + 1].frame) //bad frame, unneed and may cause problems
						{
							RotationKeys.erase(RotationKeys.begin() + j + 1);
							--j;
						}
					}
				}


				//Fill empty keyframe areas
				if (PositionKeys.size())
				{
					SPositionKey *Key;
					Key = &PositionKeys[0];//getFirst
					if (Key->frame != 0)
					{
						PositionKeys.push_front(*Key);
						Key = &PositionKeys[0];//getFirst
						Key->frame = 0;
					}

					Key = &PositionKeys.back();
					if (Key->frame != AnimationFrames)
					{
						PositionKeys.push_back(*Key);
						Key = &PositionKeys.back();
						Key->frame = AnimationFrames;
					}
				}

				if (ScaleKeys.size())
				{
					SScaleKey *Key;
					Key = &ScaleKeys[0];//getFirst
					if (Key->frame != 0)
					{
						ScaleKeys.push_front(*Key);
						Key = &ScaleKeys[0];//getFirst
						Key->frame = 0;
					}

					Key = &ScaleKeys.back();
					if (Key->frame != AnimationFrames)
					{
						ScaleKeys.push_back(*Key);
						Key = &ScaleKeys.back();
						Key->frame = AnimationFrames;
					}
				}

				if (RotationKeys.size())
				{
					SRotationKey *Key;
					Key = &RotationKeys[0];//getFirst
					if (Key->frame != 0)
					{
						RotationKeys.push_front(*Key);
						Key = &RotationKeys[0];//getFirst
						Key->frame = 0;
					}

					Key = &RotationKeys.back();
					if (Key->frame != AnimationFrames)
					{
						RotationKeys.push_back(*Key);
						Key = &RotationKeys.back();
						Key->frame = AnimationFrames;
					}
				}
			}
		}

		//Needed for animation and skinning...

		calculateGlobalMatrices(0, 0);

		//animateMesh(0, 1);
		//buildAllLocalAnimatedMatrices();
		//buildAllGlobalAnimatedMatrices();

		//rigid animation for non animated meshes
		for (i = 0; i<AllJoints.size(); ++i)
		{
			for (UINT32 j = 0; j<AllJoints[i]->AttachedMeshes.size(); ++j)
			{
				SSkinMeshBuffer* Buffer = (*SkinningBuffers)[AllJoints[i]->AttachedMeshes[j]];
				Buffer->Transformation = AllJoints[i]->GlobalAnimatedMatrix;
			}
		}

		//calculate bounding box
		if (LocalBuffers.empty())
			BoundingBox.reset(Vector3(0, 0, 0));
		else
		{
			AxisAlignedBox bb(LocalBuffers[0]->BoundingBox);
			//LocalBuffers[0]->Transformation.transformBoxEx(bb);
			//LocalBuffers[0]->Transformation.transformBoxEx();
			bb.transform(LocalBuffers[0]->Transformation);
			BoundingBox.reset(bb);

			for (UINT32 j = 1; j<LocalBuffers.size(); ++j)
			{
				bb = LocalBuffers[j]->BoundingBox;
				//LocalBuffers[j]->Transformation.transformBoxEx(bb);
				bb.transform(LocalBuffers[j]->Transformation);
				BoundingBox.addInternalBox(bb);
			}
		}
	}


	void CSkinnedMesh::updateBoundingBox(void)
	{
		if (!SkinningBuffers)
			return;

		vector<SSkinMeshBuffer*>::type & buffer = *SkinningBuffers;
		BoundingBox.reset(Vector3(0, 0, 0));

		if (!buffer.empty())
		{
			for (UINT32 j = 0; j<buffer.size(); ++j)
			{
				buffer[j]->recalculateBoundingBox();
				AxisAlignedBox bb = buffer[j]->BoundingBox;
				//buffer[j]->Transformation.transformBoxEx(bb);
				bb.transform(buffer[j]->Transformation);
				BoundingBox.addInternalBox(bb);
			}
		}
	}


	SSkinMeshBuffer *CSkinnedMesh::addMeshBuffer()
	{
		SSkinMeshBuffer *buffer = new SSkinMeshBuffer();
		LocalBuffers.push_back(buffer);
		return buffer;
	}


	CSkinnedMesh::SJoint *CSkinnedMesh::addJoint(SJoint *parent)
	{
		SJoint *joint = new SJoint;

		AllJoints.push_back(joint);
		if (!parent)
		{
			//Add root joints to vector in finalize()
		}
		else
		{
			//Set parent (Be careful of the mesh loader also setting the parent)
			parent->Children.push_back(joint);
		}

		return joint;
	}


	CSkinnedMesh::SPositionKey *CSkinnedMesh::addPositionKey(SJoint *joint)
	{
		if (!joint)
			return 0;

		joint->PositionKeys.push_back(SPositionKey());
		return &joint->PositionKeys.back();
	}


	CSkinnedMesh::SScaleKey *CSkinnedMesh::addScaleKey(SJoint *joint)
	{
		if (!joint)
			return 0;

		joint->ScaleKeys.push_back(SScaleKey());
		return &joint->ScaleKeys.back();
	}


	CSkinnedMesh::SRotationKey *CSkinnedMesh::addRotationKey(SJoint *joint)
	{
		if (!joint)
			return 0;

		joint->RotationKeys.push_back(SRotationKey());
		return &joint->RotationKeys.back();
	}


	CSkinnedMesh::SWeight *CSkinnedMesh::addWeight(SJoint *joint)
	{
		if (!joint)
			return 0;

		joint->Weights.push_back(SWeight());
		return &joint->Weights.back();
	}


	bool CSkinnedMesh::isStatic()
	{
		return !HasAnimation;
	}


	void CSkinnedMesh::normalizeWeights()
	{
		// note: unsure if weights ids are going to be used.

		// Normalise the weights on bones....

		UINT32 i, j;
		vector< vector<FLOAT32>::type >::type verticesTotalWeight;

		verticesTotalWeight.reserve(LocalBuffers.size());
		for (i = 0; i<LocalBuffers.size(); ++i)
		{
			verticesTotalWeight.push_back(vector<FLOAT32>::type());
			verticesTotalWeight[i].reserve(LocalBuffers[i]->getVertexCount());
		}

		for (i = 0; i<verticesTotalWeight.size(); ++i)
			for (j = 0; j<verticesTotalWeight[i].size(); ++j)
				verticesTotalWeight[i][j] = 0;

		for (i = 0; i<AllJoints.size(); ++i)
		{
			SJoint *joint = AllJoints[i];
			for (j = 0; j<joint->Weights.size(); ++j)
			{
				if (joint->Weights[j].strength <= 0)//Check for invalid weights
				{
					joint->Weights.erase(joint->Weights.begin()+j);
					--j;
				}
				else
				{
					verticesTotalWeight[joint->Weights[j].buffer_id][joint->Weights[j].vertex_id] += joint->Weights[j].strength;
				}
			}
		}

		for (i = 0; i<AllJoints.size(); ++i)
		{
			SJoint *joint = AllJoints[i];
			for (j = 0; j< joint->Weights.size(); ++j)
			{
				const FLOAT32 total = verticesTotalWeight[joint->Weights[j].buffer_id][joint->Weights[j].vertex_id];
				if (total != 0 && total != 1)
					joint->Weights[j].strength /= total;
			}
		}
	}


	void CSkinnedMesh::recoverJointsFromMesh(vector<IBoneSceneNode*>::type &jointChildSceneNodes)
	{
		for (UINT32 i = 0; i<AllJoints.size(); ++i)
		{
			IBoneSceneNode* node = jointChildSceneNodes[i];
			SJoint *joint = AllJoints[i];
			node->setPosition(joint->LocalAnimatedMatrix.getTrans());
			node->setRotation(joint->LocalAnimatedMatrix.getRotationDegrees());
			node->setScale(joint->LocalAnimatedMatrix.getScale());

			node->positionHint = joint->positionHint;
			node->scaleHint = joint->scaleHint;
			node->rotationHint = joint->rotationHint;

			node->updateAbsolutePosition();
		}
	}


	void CSkinnedMesh::transferJointsToMesh(const vector<IBoneSceneNode*>::type &jointChildSceneNodes)
	{
		for (UINT32 i = 0; i<AllJoints.size(); ++i)
		{
			const IBoneSceneNode* const node = jointChildSceneNodes[i];
			SJoint *joint = AllJoints[i];

			joint->LocalAnimatedMatrix.setRotationDegrees(node->getRotation());
			joint->LocalAnimatedMatrix.setTrans(node->getPosition());
			Matrix4 mat = Matrix4();
			mat.setScale(node->getScale());
			joint->LocalAnimatedMatrix = mat*joint->LocalAnimatedMatrix;

			joint->positionHint = node->positionHint;
			joint->scaleHint = node->scaleHint;
			joint->rotationHint = node->rotationHint;

			joint->GlobalSkinningSpace = (node->getSkinningSpace() == EBSS_GLOBAL);
		}
		// Make sure we recalc the next frame
		LastAnimatedFrame = -1;
		SkinnedLastFrame = false;
	}


	void CSkinnedMesh::transferOnlyJointsHintsToMesh(const vector<IBoneSceneNode*>::type &jointChildSceneNodes)
	{
		for (UINT32 i = 0; i<AllJoints.size(); ++i)
		{
			const IBoneSceneNode* const node = jointChildSceneNodes[i];
			SJoint *joint = AllJoints[i];

			joint->positionHint = node->positionHint;
			joint->scaleHint = node->scaleHint;
			joint->rotationHint = node->rotationHint;
		}
		SkinnedLastFrame = false;
	}


	void CSkinnedMesh::addJoints(vector<IBoneSceneNode*>::type &jointChildSceneNodes,
		IAnimatedMeshSceneNode* node, ISceneManager* smgr)
	{
		//Create new joints
		for (UINT32 i = 0; i<AllJoints.size(); ++i)
		{
			jointChildSceneNodes.push_back(new CBoneSceneNode(0, smgr, 0, i, AllJoints[i]->Name.c_str()));
		}

		//Match up parents
		for (UINT32 i = 0; i<jointChildSceneNodes.size(); ++i)
		{
			const SJoint* const joint = AllJoints[i]; //should be fine

			SINT32 parentID = -1;

			for (UINT32 j = 0; (parentID == -1) && (j<AllJoints.size()); ++j)
			{
				if (i != j)
				{
					const SJoint* const parentTest = AllJoints[j];
					for (UINT32 n = 0; n<parentTest->Children.size(); ++n)
					{
						if (parentTest->Children[n] == joint)
						{
							parentID = j;
							break;
						}
					}
				}
			}

			IBoneSceneNode* bone = jointChildSceneNodes[i];
			if (parentID != -1)
				bone->setParent(jointChildSceneNodes[parentID]);
			else
				bone->setParent(node);

			bone->drop();
		}
		SkinnedLastFrame = false;
	}


	void CSkinnedMesh::convertMeshToTangents()
	{
		// now calculate tangents
		for (UINT32 b = 0; b < LocalBuffers.size(); ++b)
		{
			if (LocalBuffers[b])
			{
				LocalBuffers[b]->convertToTangents();

				const SINT32 idxCnt = LocalBuffers[b]->getIndexCount();

				UINT16* idx = LocalBuffers[b]->getIndices();
				S3DVertexTangents* v =
					(S3DVertexTangents*)LocalBuffers[b]->getVertices();

				for (SINT32 i = 0; i<idxCnt; i += 3)
				{
					calculateTangents(
						v[idx[i + 0]].Normal,
						v[idx[i + 0]].Tangent,
						v[idx[i + 0]].Binormal,
						v[idx[i + 0]].Pos,
						v[idx[i + 1]].Pos,
						v[idx[i + 2]].Pos,
						v[idx[i + 0]].TCoords,
						v[idx[i + 1]].TCoords,
						v[idx[i + 2]].TCoords);

					calculateTangents(
						v[idx[i + 1]].Normal,
						v[idx[i + 1]].Tangent,
						v[idx[i + 1]].Binormal,
						v[idx[i + 1]].Pos,
						v[idx[i + 2]].Pos,
						v[idx[i + 0]].Pos,
						v[idx[i + 1]].TCoords,
						v[idx[i + 2]].TCoords,
						v[idx[i + 0]].TCoords);

					calculateTangents(
						v[idx[i + 2]].Normal,
						v[idx[i + 2]].Tangent,
						v[idx[i + 2]].Binormal,
						v[idx[i + 2]].Pos,
						v[idx[i + 0]].Pos,
						v[idx[i + 1]].Pos,
						v[idx[i + 2]].TCoords,
						v[idx[i + 0]].TCoords,
						v[idx[i + 1]].TCoords);
				}
			}
		}
	}


	void CSkinnedMesh::calculateTangents(
		Vector3& normal,
		Vector3& tangent,
		Vector3& binormal,
		Vector3& vt1, Vector3& vt2, Vector3& vt3, // vertices
		Vector2& tc1, Vector2& tc2, Vector2& tc3) // texture coords

	{
		Vector3 v1 = vt1 - vt2;
		Vector3 v2 = vt3 - vt1;
		normal = v2.crossProduct(v1);
		normal.normalize();

		// binormal

		FLOAT32 deltaX1 = tc1.x - tc2.x;
		FLOAT32 deltaX2 = tc3.x - tc1.x;
		binormal = (v1 * deltaX2) - (v2 * deltaX1);
		binormal.normalize();

		// tangent

		FLOAT32 deltaY1 = tc1.y - tc2.y;
		FLOAT32 deltaY2 = tc3.y - tc1.y;
		tangent = (v1 * deltaY2) - (v2 * deltaY1);
		tangent.normalize();

		// adjust

		Vector3 txb = tangent.crossProduct(binormal);
		if (txb.dotProduct(normal) < 0.0f)
		{
			tangent *= -1.0f;
			binormal *= -1.0f;
		}
	}
}
#ifdef _SAPPHIRE_COMPILE_WITH_SKINNED_MESH_SUPPORT_


#endif