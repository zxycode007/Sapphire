#include "SapphireCAnimatedMeshSceneNode.h"
#include "SapphireIVideoDriver.h"
#include "SapphireIScenesManager.h"
#include "SapphireSVertex.h"
#include "SapphireOS.h"
#include "SapphireCShadowVolumeSceneNode.h"
#include "SapphireIAnimatedMeshMD3.h"
#include "SapphireCSkinnedMesh.h"
#include "SapphireIDummyTransformationSceneNode.h"
#include "SapphireIBoneSceneNode.h"
#include "SapphireIMaterialRenderer.h"
#include "SapphireIMesh.h"
#include "SapphireIMeshCache.h"
#include "SapphireIAnimatedMesh.h"
#include "SapphireQuaternion.h"
#include "SapphireSceneParameters.h"
#include "SapphireCompileConifg.h"


namespace Sapphire
{

	//! constructor
	CAnimatedMeshSceneNode::CAnimatedMeshSceneNode(IAnimatedMesh* mesh,
		ISceneNode* parent, ISceneManager* mgr, SINT32 id,
		const Vector3& position,
		const Vector3& rotation,
		const Vector3& scale)
		: IAnimatedMeshSceneNode(parent, mgr, id, position, rotation, scale), Mesh(0),
		StartFrame(0), EndFrame(0), FramesPerSecond(0.025f),
		CurrentFrameNr(0.f), LastTimeMs(0),
		TransitionTime(0), Transiting(0.f), TransitingBlend(0.f),
		JointMode(EJUOR_NONE), JointsUsed(false),
		Looping(true), ReadOnlyMaterials(false), RenderFromIdentity(false),
		LoopCallBack(0), PassCount(0), Shadow(0), MD3Special(0)
	{
		
#ifdef _DEBUG
		setDebugName("CAnimatedMeshSceneNode");
#endif
		
		setMesh(mesh);
		
	}


	//! destructor
	CAnimatedMeshSceneNode::~CAnimatedMeshSceneNode()
	{
		if (MD3Special)
			MD3Special->drop();

		if (Mesh)
			Mesh->drop();

		if (Shadow)
			Shadow->drop();

		if (LoopCallBack)
			LoopCallBack->drop();
	}


	//! Sets the current frame. From now on the animation is played from this frame.
	void CAnimatedMeshSceneNode::setCurrentFrame(FLOAT32 frame)
	{
		// if you pass an out of range value, we just clamp it
		CurrentFrameNr = Math::Clamp(frame, (FLOAT32)StartFrame, (FLOAT32)EndFrame);

		beginTransition(); //transit to this frame if enabled
	}


	//! Returns the currently displayed frame number.
	FLOAT32 CAnimatedMeshSceneNode::getFrameNr() const
	{
		return CurrentFrameNr;
	}


	//! Get CurrentFrameNr and update transiting settings
	void CAnimatedMeshSceneNode::buildFrameNr(UINT32 timeMs)
	{
		if (Transiting != 0.f)
		{
			TransitingBlend += (FLOAT32)(timeMs)* Transiting;
			if (TransitingBlend > 1.f)
			{
				Transiting = 0.f;
				TransitingBlend = 0.f;
			}
		}

		if ((StartFrame == EndFrame))
		{
			CurrentFrameNr = (FLOAT32)StartFrame; //Support for non animated meshes
		}
		else if (Looping)
		{
			// play animation looped
			CurrentFrameNr += timeMs * FramesPerSecond;

			// We have no interpolation between EndFrame and StartFrame,
			// the last frame must be identical to first one with our current solution.
			if (FramesPerSecond > 0.f) //forwards...
			{
				if (CurrentFrameNr > EndFrame)
					CurrentFrameNr = StartFrame + fmod(CurrentFrameNr - StartFrame, (FLOAT32)(EndFrame - StartFrame));
			}
			else //backwards...
			{
				if (CurrentFrameNr < StartFrame)
					CurrentFrameNr = EndFrame - fmod(EndFrame - CurrentFrameNr, (FLOAT32)(EndFrame - StartFrame));
			}
		}
		else
		{
			// play animation non looped

			CurrentFrameNr += timeMs * FramesPerSecond;
			if (FramesPerSecond > 0.f) //forwards...
			{
				if (CurrentFrameNr > (FLOAT32)EndFrame)
				{
					CurrentFrameNr = (FLOAT32)EndFrame;
					if (LoopCallBack)
						LoopCallBack->OnAnimationEnd(this);
				}
			}
			else //backwards...
			{
				if (CurrentFrameNr < (FLOAT32)StartFrame)
				{
					CurrentFrameNr = (FLOAT32)StartFrame;
					if (LoopCallBack)
						LoopCallBack->OnAnimationEnd(this);
				}
			}
		}
	}


	void CAnimatedMeshSceneNode::OnRegisterSceneNode()
	{
		if (IsVisible)
		{
			// because this node supports rendering of mixed mode meshes consisting of
			// transparent and solid material at the same time, we need to go through all
			// materials, check of what type they are and register this node for the right
			// render pass according to that.
			// 由于这些节点要支持透明和固体材质的混合渲染模式，必须访问所有材质，检测他们的类型
			// 并且注册这些节点到正确渲染通道

			IVideoDriver* driver = SceneManager->getVideoDriver();

			PassCount = 0;
			int transparentCount = 0;
			int solidCount = 0;

			// count transparent and solid materials in this scene node
			// 计算当前场景节点的透明和固体材质数量
			for (UINT32 i = 0; i<Materials.size(); ++i)
			{
				//获取相应的材质渲染器
				IMaterialRenderer* rnd =
					driver->getMaterialRenderer(Materials[i].MaterialType);

				if (rnd && rnd->isTransparent())
					++transparentCount;
				else
					++solidCount;

				if (solidCount && transparentCount)
					break;
			}

			// register according to material types counted
			// 注册材质类型
			if (solidCount)
				SceneManager->registerNodeForRendering(this, ESNRP_SOLID);

			if (transparentCount)
				SceneManager->registerNodeForRendering(this, ESNRP_TRANSPARENT);

			ISceneNode::OnRegisterSceneNode();
		}
	}

	IMesh * CAnimatedMeshSceneNode::getMeshForCurrentFrame()
	{
		if (Mesh->getMeshType() != EAMT_SKINNED)
		{
			SINT32 frameNr = (SINT32)getFrameNr();
			SINT32 frameBlend = (SINT32)(fract(getFrameNr()) * 1000.f);
			return Mesh->getMesh(frameNr, frameBlend, StartFrame, EndFrame);
		}
		else
		{
#ifndef _SAPPHIRE_COMPILE_WITH_SKINNED_MESH_SUPPORT_
			return 0;
#else

			// As multiple scene nodes may be sharing the same skinned mesh, we have to
			// re-animate it every frame to ensure that this node gets the mesh that it needs.

			CSkinnedMesh* skinnedMesh = reinterpret_cast<CSkinnedMesh*>(Mesh);

			if (JointMode == EJUOR_CONTROL)//write to mesh
				skinnedMesh->transferJointsToMesh(JointChildSceneNodes);
			else
				skinnedMesh->animateMesh(getFrameNr(), 1.0f);

			// Update the skinned mesh for the current joint transforms.
			skinnedMesh->skinMesh();

			if (JointMode == EJUOR_READ)//read from mesh
			{
				skinnedMesh->recoverJointsFromMesh(JointChildSceneNodes);

				//---slow---
				for (UINT32 n = 0; n<JointChildSceneNodes.size(); ++n)
					if (JointChildSceneNodes[n]->getParent() == this)
					{
						JointChildSceneNodes[n]->updateAbsolutePositionOfAllChildren(); //temp, should be an option
					}
			}

			if (JointMode == EJUOR_CONTROL)
			{
				// For meshes other than EJUOR_CONTROL, this is done by calling animateMesh()
				skinnedMesh->updateBoundingBox();
			}

			return skinnedMesh;
#endif
		}
	}


	//! OnAnimate() is called just before rendering the whole scene.   
	//在渲染之前调用
	void CAnimatedMeshSceneNode::OnAnimate(UINT32 timeMs)
	{
		if (LastTimeMs == 0)	// first frame
		{
			LastTimeMs = timeMs;
		}

		// set CurrentFrameNr
		// 设置当前帧
		buildFrameNr(timeMs - LastTimeMs);

		// update bbox
		// 更新AABB
		if (Mesh)
		{
			IMesh * mesh = getMeshForCurrentFrame();

			if (mesh)
				Box = mesh->getBoundingBox();
		}
		LastTimeMs = timeMs;

		IAnimatedMeshSceneNode::OnAnimate(timeMs);
	}


	//! renders the node.
	// 渲染这个节点
	void CAnimatedMeshSceneNode::render()
	{
		//获取视频驱动
		IVideoDriver* driver = SceneManager->getVideoDriver();
		//判断驱动和网格是否有效
		if (!Mesh || !driver)
			return;

		//是否是透明通道
		bool isTransparentPass =
			SceneManager->getSceneNodeRenderPass() == ESNRP_TRANSPARENT;

		++PassCount;
		//获取当前帧的网格
		IMesh* m = getMeshForCurrentFrame();

		if (m)
		{
			//获取AABB碰撞盒子
			Box = m->getBoundingBox();
		}
		else
		{
#ifdef _DEBUG
			Printer::log(String("Animated Mesh returned no mesh to render. \n") + String(Mesh->getDebugName()) , LML_WARNING);
#endif
		}
		//设置变换矩阵
		driver->setTransform(ETS_WORLD, AbsoluteTransformation);

		if (Shadow && PassCount == 1)
			Shadow->updateShadowVolumes();

		// for debug purposes only:
		//debug模式 
		bool renderMeshes = true;
		SMaterial mat;
		if (DebugDataVisible && PassCount == 1)
		{
			// overwrite half transparency
			if (DebugDataVisible & EDS_HALF_TRANSPARENCY)
			{

				for (UINT32 i = 0; i<m->getMeshBufferCount(); ++i)
				{
					IMeshBuffer* mb = m->getMeshBuffer(i);
					mat = ReadOnlyMaterials ? mb->getMaterial() : Materials[i];
					mat.MaterialType = EMT_TRANSPARENT_ADD_COLOR;
					if (RenderFromIdentity)
						driver->setTransform(ETS_WORLD, Matrix4::IDENTITY);
					else if (Mesh->getMeshType() == EAMT_SKINNED)
						driver->setTransform(ETS_WORLD, AbsoluteTransformation * ((SSkinMeshBuffer*)mb)->Transformation);

					driver->setMaterial(mat);
					driver->drawMeshBuffer(mb);
				}
				renderMeshes = false;
			}
		}

		// render original meshes
		// 渲染原网格
		if (renderMeshes)
		{
			for (UINT32 i = 0; i<m->getMeshBufferCount(); ++i)
			{
				//获取材质渲染器
				IMaterialRenderer* rnd = driver->getMaterialRenderer(Materials[i].MaterialType);
				//是否透明
				bool transparent = (rnd && rnd->isTransparent());

				// only render transparent buffer if this is the transparent render pass
				// 如果这是一个透明渲染通道，只渲染透明缓冲区
				// and solid only in solid pass
			    // 并且固体只能在固体通道中
				if (transparent == isTransparentPass)
				{
					//获取网格缓冲区
					IMeshBuffer* mb = m->getMeshBuffer(i);
					//获取材质
					const SMaterial& material = ReadOnlyMaterials ? mb->getMaterial() : Materials[i];
					
					if (RenderFromIdentity)
						driver->setTransform(ETS_WORLD, Matrix4::IDENTITY);
					else if (Mesh->getMeshType() == EAMT_SKINNED)    //是否骨骼蒙皮
						driver->setTransform(ETS_WORLD, AbsoluteTransformation * ((SSkinMeshBuffer*)mb)->Transformation);

					{
						//////////////测试代码////////////////
						

						

						int vsize = mb->getVertexCount();
						const char* s = typeid(mb->getVertices()).name();
						E_VERTEX_TYPE t = mb->getVertexType();
						S3DVertex* vs = (S3DVertex*)mb->getVertices();

						
						for (size_t i = 0; i < vsize; i++)
						{
						StringStream ss;
						ss << "vertex " << i << " x:" << vs[i].Pos.x << "  y:" << vs[i].Pos.y << " z:" << vs[i].Pos.z;
						Printer::log(ss.str(), LML_NORMAL);
						}
						
						//StringStream ss;
						//ss << "vertex " << 0 << " x:" << vs[0].Pos.x << "  y:" << vs[0].Pos.y << " z:" << vs[0].Pos.z;
						//Printer::log(ss.str(), LML_NORMAL);
						//ss.clear();
						//Matrix4 mt = AbsoluteTransformation * ((SSkinMeshBuffer*)mb)->Transformation;
						//ss << endl;
						//ss << " ETS_WORLD MATRIX4 = " << mt.getIndex(0) << "   " << mt.getIndex(1) << "   " << mt.getIndex(2) << "   " << mt.getIndex(3)<< endl;
						//ss << " ETS_WORLD MATRIX4 = " << mt.getIndex(4) << "   " << mt.getIndex(5) << "   " << mt.getIndex(6) << "   " << mt.getIndex(7) << endl;
						//ss << " ETS_WORLD MATRIX4 = " << mt.getIndex(8) << "   " << mt.getIndex(9) << "   " << mt.getIndex(10) << "   " << mt.getIndex(11) << endl;
						//ss << " ETS_WORLD MATRIX4 = " << mt.getIndex(12) << "   " << mt.getIndex(13) << "   " << mt.getIndex(14) << "   " << mt.getIndex(15) << endl;
						//Printer::log(ss.str(), LML_NORMAL);
					}
					//设置材质
					driver->setMaterial(material);
					//绘制网格缓冲区
					driver->drawMeshBuffer(mb);
				}
			}
		}

		driver->setTransform(ETS_WORLD, AbsoluteTransformation);

		// for debug purposes only:
		if (DebugDataVisible && PassCount == 1)
		{
			SMaterial debug_mat;
			debug_mat.Lighting = false;
			debug_mat.AntiAliasing = 0;
			driver->setMaterial(debug_mat);
			// show normals
			if (DebugDataVisible & EDS_NORMALS)
			{
				const FLOAT32 debugNormalLength = SceneManager->getParameters()->getAttributeAsFloat(DEBUG_NORMAL_LENGTH);
				const ColourValue debugNormalColor = SceneManager->getParameters()->getAttributeAsColor(DEBUG_NORMAL_COLOR);
				const UINT32 count = m->getMeshBufferCount();

				// draw normals
				for (UINT32 g = 0; g < count; ++g)
				{
					driver->drawMeshBufferNormals(m->getMeshBuffer(g), debugNormalLength, debugNormalColor);
				}
			}

			debug_mat.ZBuffer = ECFN_NEVER;
			debug_mat.Lighting = false;
			driver->setMaterial(debug_mat);

			if (DebugDataVisible & EDS_BBOX)
				driver->draw3DBox(Box, ColourValue::getColourValue(255, 255, 255, 255));

			// show bounding box
			if (DebugDataVisible & EDS_BBOX_BUFFERS)
			{
				for (UINT32 g = 0; g< m->getMeshBufferCount(); ++g)
				{
					const IMeshBuffer* mb = m->getMeshBuffer(g);

					if (Mesh->getMeshType() == EAMT_SKINNED)
						driver->setTransform(ETS_WORLD, AbsoluteTransformation * ((SSkinMeshBuffer*)mb)->Transformation);
					driver->draw3DBox(mb->getBoundingBox(), ColourValue::getColourValue(255, 190, 128, 128));
				}
			}

			// show skeleton
			if (DebugDataVisible & EDS_SKELETON)
			{
				if (Mesh->getMeshType() == EAMT_SKINNED)
				{
					// draw skeleton

					for (UINT32 g = 0; g < ((ISkinnedMesh*)Mesh)->getAllJoints().size(); ++g)
					{
						ISkinnedMesh::SJoint *joint = ((ISkinnedMesh*)Mesh)->getAllJoints()[g];

						for (UINT32 n = 0; n<joint->Children.size(); ++n)
						{
							driver->draw3DLine(joint->GlobalAnimatedMatrix.getTrans(),
								joint->Children[n]->GlobalAnimatedMatrix.getTrans(),
								ColourValue::getColourValue(255, 51, 66, 255));
						}
					}
				}

				// show tag for quake3 models
				if (Mesh->getMeshType() == EAMT_MD3)
				{
					IAnimatedMesh * arrow =
						SceneManager->addArrowMesh(
						"__tag_show",
						0xFF0000FF, 0xFF000088,
						4, 8, 5.f, 4.f, 0.5f,
						1.f);
					if (!arrow)
					{
						arrow = SceneManager->getMesh("__tag_show");
					}
					IMesh *arrowMesh = arrow->getMesh(0);

					Matrix4 matr;

					SMD3QuaternionTagList *taglist = ((IAnimatedMeshMD3*)Mesh)->getTagList(
						(SINT32)getFrameNr(), 255,
						getStartFrame(), getEndFrame());
					if (taglist)
					{
						for (UINT32 ts = 0; ts != taglist->size(); ++ts)
						{
							(*taglist)[ts].setto(matr);

							driver->setTransform(ETS_WORLD, matr);

							for (UINT32 a = 0; a != arrowMesh->getMeshBufferCount(); ++a)
								driver->drawMeshBuffer(arrowMesh->getMeshBuffer(a));
						}
					}
				}
			}

			// show mesh
			if (DebugDataVisible & EDS_MESH_WIRE_OVERLAY)
			{
				debug_mat.Lighting = false;
				debug_mat.Wireframe = true;
				debug_mat.ZBuffer = ECFN_NEVER;
				driver->setMaterial(debug_mat);

				for (UINT32 g = 0; g<m->getMeshBufferCount(); ++g)
				{
					const IMeshBuffer* mb = m->getMeshBuffer(g);
					if (RenderFromIdentity)
						driver->setTransform(ETS_WORLD, Matrix4::IDENTITY);
					else if (Mesh->getMeshType() == EAMT_SKINNED)
						driver->setTransform(ETS_WORLD, AbsoluteTransformation * ((SSkinMeshBuffer*)mb)->Transformation);
					driver->drawMeshBuffer(mb);
				}
			}
		}
	}


	//! Returns the current start frame number.
	SINT32 CAnimatedMeshSceneNode::getStartFrame() const
	{
		return StartFrame;
	}


	//! Returns the current start frame number.
	SINT32 CAnimatedMeshSceneNode::getEndFrame() const
	{
		return EndFrame;
	}


	//! sets the frames between the animation is looped.
	//! the default is 0 - MaximalFrameCount of the mesh.
	bool CAnimatedMeshSceneNode::setFrameLoop(SINT32 begin, SINT32 end)
	{
		const SINT32 maxFrameCount = Mesh->getFrameCount() - 1;
		if (end < begin)
		{
			StartFrame = SINT32_clamp(end, 0, maxFrameCount);
			EndFrame = SINT32_clamp(begin, StartFrame, maxFrameCount);
		}
		else
		{
			StartFrame = SINT32_clamp(begin, 0, maxFrameCount);
			EndFrame = SINT32_clamp(end, StartFrame, maxFrameCount);
		}
		if (FramesPerSecond < 0)
			setCurrentFrame((FLOAT32)EndFrame);
		else
			setCurrentFrame((FLOAT32)StartFrame);

		return true;
	}


	//! sets the speed with witch the animation is played
	void CAnimatedMeshSceneNode::setAnimationSpeed(FLOAT32 framesPerSecond)
	{
		FramesPerSecond = framesPerSecond * 0.001f;
	}


	FLOAT32 CAnimatedMeshSceneNode::getAnimationSpeed() const
	{
		return FramesPerSecond * 1000.f;
	}


	//! returns the axis aligned bounding box of this node
	const AxisAlignedBox& CAnimatedMeshSceneNode::getBoundingBox() const
	{
		return Box;
	}


	//! returns the material based on the zero based index i. To get the amount
	//! of materials used by this scene node, use getMaterialCount().
	//! This function is needed for inserting the node into the scene hirachy on a
	//! optimal position for minimizing renderstate changes, but can also be used
	//! to directly modify the material of a scene node.
	SMaterial& CAnimatedMeshSceneNode::getMaterial(UINT32 i)
	{
		if (i >= Materials.size())
			return ISceneNode::getMaterial(i);

		return Materials[i];
	}



	//! returns amount of materials used by this scene node.
	UINT32 CAnimatedMeshSceneNode::getMaterialCount() const
	{
		return Materials.size();
	}


	//! Creates shadow volume scene node as child of this node
	//! and returns a pointer to it.
	IShadowVolumeSceneNode* CAnimatedMeshSceneNode::addShadowVolumeSceneNode(
		const IMesh* shadowMesh, SINT32 id, bool zfailmethod, FLOAT32 infinity)
	{
		if (!SceneManager->getVideoDriver()->queryFeature(EVDF_STENCIL_BUFFER))
			return 0;

		if (!shadowMesh)
			shadowMesh = Mesh; // if null is given, use the mesh of node

		if (Shadow)
			Shadow->drop();

		Shadow = new CShadowVolumeSceneNode(shadowMesh, this, SceneManager, id, zfailmethod, infinity);
		return Shadow;
	}

	//! Returns a pointer to a child node, which has the same transformation as
	//! the corresponding joint, if the mesh in this scene node is a skinned mesh.
	IBoneSceneNode* CAnimatedMeshSceneNode::getJointNode(const c8* jointName)
	{
#ifndef SAPPHIRE_COMPILE_WITH_SKINNED_MESH_SUPPORT_
		Printer::log("Compiled without _IRR_COMPILE_WITH_SKINNED_MESH_SUPPORT_", LML_WARNING);
		return 0;
#else

		if (!Mesh || Mesh->getMeshType() != EAMT_SKINNED)
		{
			Printer::log("No mesh, or mesh not of skinned mesh type", LML_WARNING);
			return 0;
		}

		checkJoints();

		ISkinnedMesh *skinnedMesh = (ISkinnedMesh*)Mesh;

		const SINT32 number = skinnedMesh->getJointNumber(jointName);

		if (number == -1)
		{
			Printer::log("Joint with specified name not found in skinned mesh", jointName, LML_DEBUG);
			return 0;
		}

		if ((SINT32)JointChildSceneNodes.size() <= number)
		{
			Printer::log("Joint was found in mesh, but is not loaded into node", jointName, LML_WARNING);
			return 0;
		}

		return JointChildSceneNodes[number];
#endif
	}



	//! Returns a pointer to a child node, which has the same transformation as
	//! the corresponding joint, if the mesh in this scene node is a skinned mesh.
	IBoneSceneNode* CAnimatedMeshSceneNode::getJointNode(UINT32 jointID)
	{
#ifndef SAPPHIRE_COMPILE_WITH_SKINNED_MESH_SUPPORT_
		Printer::log("Compiled without _SAPPHIRE_COMPILE_WITH_SKINNED_MESH_SUPPORT_", LML_WARNING);
		return 0;
#else

		if (!Mesh || Mesh->getMeshType() != EAMT_SKINNED)
		{
			Printer::log("No mesh, or mesh not of skinned mesh type", LML_WARNING);
			return 0;
		}

		checkJoints();

		if (JointChildSceneNodes.size() <= jointID)
		{
			Printer::log("Joint not loaded into node", LML_WARNING);
			return 0;
		}

		return JointChildSceneNodes[jointID];
#endif
	}

	//! Gets joint count.
	UINT32 CAnimatedMeshSceneNode::getJointCount() const
	{
#ifndef SAPPHIRE_COMPILE_WITH_SKINNED_MESH_SUPPORT_
		return 0;
#else

		if (!Mesh || Mesh->getMeshType() != EAMT_SKINNED)
			return 0;

		ISkinnedMesh *skinnedMesh = (ISkinnedMesh*)Mesh;

		return skinnedMesh->getJointCount();
#endif
	}


	//! Returns a pointer to a child node, which has the same transformation as
	//! the corresponding joint, if the mesh in this scene node is a ms3d mesh.
	ISceneNode* CAnimatedMeshSceneNode::getMS3DJointNode(const c8* jointName)
	{
		return  getJointNode(jointName);
	}


	//! Returns a pointer to a child node, which has the same transformation as
	//! the corresponding joint, if the mesh in this scene node is a .x mesh.
	ISceneNode* CAnimatedMeshSceneNode::getXJointNode(const c8* jointName)
	{
		return  getJointNode(jointName);
	}

	//! Removes a child from this scene node.
	//! Implemented here, to be able to remove the shadow properly, if there is one,
	//! or to remove attached childs.
	bool CAnimatedMeshSceneNode::removeChild(ISceneNode* child)
	{
		if (child && Shadow == child)
		{
			Shadow->drop();
			Shadow = 0;
		}

		if (ISceneNode::removeChild(child))
		{
			if (JointsUsed) //stop weird bugs caused while changing parents as the joints are being created
			{
				for (UINT32 i = 0; i<JointChildSceneNodes.size(); ++i)
				{
					if (JointChildSceneNodes[i] == child)
					{
						JointChildSceneNodes[i] = 0; //remove link to child
						break;
					}
				}
			}
			return true;
		}

		return false;
	}


	//! Starts a MD2 animation.
	bool CAnimatedMeshSceneNode::setMD2Animation(EMD2_ANIMATION_TYPE anim)
	{
		if (!Mesh || Mesh->getMeshType() != EAMT_MD2)
			return false;

		IAnimatedMeshMD2* md = (IAnimatedMeshMD2*)Mesh;

		SINT32 begin, end, speed;
		md->getFrameLoop(anim, begin, end, speed);

		setAnimationSpeed(FLOAT32(speed));
		setFrameLoop(begin, end);
		return true;
	}


	//! Starts a special MD2 animation.
	bool CAnimatedMeshSceneNode::setMD2Animation(const c8* animationName)
	{
		if (!Mesh || Mesh->getMeshType() != EAMT_MD2)
			return false;

		IAnimatedMeshMD2* md = (IAnimatedMeshMD2*)Mesh;

		SINT32 begin, end, speed;
		if (!md->getFrameLoop(animationName, begin, end, speed))
			return false;

		setAnimationSpeed((FLOAT32)speed);
		setFrameLoop(begin, end);
		return true;
	}


	//! Sets looping mode which is on by default. If set to false,
	//! animations will not be looped.
	void CAnimatedMeshSceneNode::setLoopMode(bool playAnimationLooped)
	{
		Looping = playAnimationLooped;
	}

	//! returns the current loop mode
	bool CAnimatedMeshSceneNode::getLoopMode() const
	{
		return Looping;
	}


	//! Sets a callback interface which will be called if an animation
	//! playback has ended. Set this to 0 to disable the callback again.
	void CAnimatedMeshSceneNode::setAnimationEndCallback(IAnimationEndCallBack* callback)
	{
		if (callback == LoopCallBack)
			return;

		if (LoopCallBack)
			LoopCallBack->drop();

		LoopCallBack = callback;

		if (LoopCallBack)
			LoopCallBack->grab();
	}


	//! Sets if the scene node should not copy the materials of the mesh but use them in a read only style.
	void CAnimatedMeshSceneNode::setReadOnlyMaterials(bool readonly)
	{
		ReadOnlyMaterials = readonly;
	}


	//! Returns if the scene node should not copy the materials of the mesh but use them in a read only style
	bool CAnimatedMeshSceneNode::isReadOnlyMaterials() const
	{
		return ReadOnlyMaterials;
	}


	//! Writes attributes of the scene node.
	void CAnimatedMeshSceneNode::serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const
	{
		IAnimatedMeshSceneNode::serializeAttributes(out, options);

		if (options && (options->Flags&EARWF_USE_RELATIVE_PATHS) && (!options->Filename.empty()))
		{
			const path path = SceneManager->getFileSystem()->getRelativeFilename(
				SceneManager->getFileSystem()->getAbsolutePath(SceneManager->getMeshCache()->getMeshName(Mesh).getPath()),
				options->Filename);
			out->addString("Mesh", path.c_str());
		}
		else
			out->addString("Mesh", SceneManager->getMeshCache()->getMeshName(Mesh).getPath().c_str());
		out->addBool("Looping", Looping);
		out->addBool("ReadOnlyMaterials", ReadOnlyMaterials);
		out->addFloat("FramesPerSecond", FramesPerSecond);
		out->addInt("StartFrame", StartFrame);
		out->addInt("EndFrame", EndFrame);
	}


	//! Reads attributes of the scene node.
	void CAnimatedMeshSceneNode::deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options)
	{
		IAnimatedMeshSceneNode::deserializeAttributes(in, options);

		path oldMeshStr = SceneManager->getMeshCache()->getMeshName(Mesh).getPath();
		path newMeshStr = in->getAttributeAsString("Mesh");

		Looping = in->getAttributeAsBool("Looping");
		ReadOnlyMaterials = in->getAttributeAsBool("ReadOnlyMaterials");
		FramesPerSecond = in->getAttributeAsFloat("FramesPerSecond");
		StartFrame = in->getAttributeAsInt("StartFrame");
		EndFrame = in->getAttributeAsInt("EndFrame");

		if (newMeshStr != "" && oldMeshStr != newMeshStr)
		{
			IAnimatedMesh* newAnimatedMesh = SceneManager->getMesh(newMeshStr.c_str());

			if (newAnimatedMesh)
				setMesh(newAnimatedMesh);
		}

		// TODO: read animation names instead of frame begin and ends
	}


	//! Sets a new mesh
	void CAnimatedMeshSceneNode::setMesh(IAnimatedMesh* mesh)
	{
		if (!mesh)
			return; // won't set null mesh
		
		if (Mesh != mesh)
		{
			if (Mesh)
				Mesh->drop();

			Mesh = mesh;

			// grab the mesh (it's non-null!)
			Mesh->grab();
		}
		
		// get materials and bounding box
		Box = Mesh->getBoundingBox();
		 
		IMesh* m = Mesh->getMesh(0, 0);
		 
		if (m)
		{
			Materials.clear();
			//Materials.reallocate(m->getMeshBufferCount());
			Materials.reserve(m->getMeshBufferCount());
			for (UINT32 i = 0; i<m->getMeshBufferCount(); ++i)
			{
				IMeshBuffer* mb = m->getMeshBuffer(i);
				
				if (mb)
				{
	
					Materials.push_back(mb->getMaterial());				
					//Materials.push_back(mb->getMaterial());
					
				}
				else
					Materials.push_back(SMaterial());
			}
			
		}
		
		// clean up joint nodes
		if (JointsUsed)
		{
			JointsUsed = false;
			checkJoints();
		}
		 
		// get start and begin time
		//	setAnimationSpeed(Mesh->getAnimationSpeed());
		setFrameLoop(0, Mesh->getFrameCount());
	}


	// returns the absolute transformation for a special MD3 Tag if the mesh is a md3 mesh,
	// or the absolutetransformation if it's a normal scenenode
	const SMD3QuaternionTag* CAnimatedMeshSceneNode::getMD3TagTransformation(const String& tagname)
	{
		return MD3Special ? MD3Special->AbsoluteTagList.get(tagname) : 0;
	}


	//! updates the absolute position based on the relative and the parents position
	void CAnimatedMeshSceneNode::updateAbsolutePosition()
	{
		IAnimatedMeshSceneNode::updateAbsolutePosition();

		if (!Mesh || Mesh->getMeshType() != EAMT_MD3)
			return;

		SMD3QuaternionTagList *taglist;
		taglist = ((IAnimatedMeshMD3*)Mesh)->getTagList((SINT32)getFrameNr(), 255, getStartFrame(), getEndFrame());
		if (taglist)
		{
			if (!MD3Special)
			{
				MD3Special = new SMD3Special();
			}

			SMD3QuaternionTag parent(MD3Special->Tagname);
			if (Parent && Parent->getType() == ESNT_ANIMATED_MESH)
			{
				const SMD3QuaternionTag * p = ((IAnimatedMeshSceneNode*)Parent)->getMD3TagTransformation
					(MD3Special->Tagname);

				if (p)
					parent = *p;
			}

			SMD3QuaternionTag relative(RelativeTranslation, RelativeRotation);

			MD3Special->AbsoluteTagList.set_used(taglist->size());
			for (UINT32 i = 0; i != taglist->size(); ++i)
			{
				MD3Special->AbsoluteTagList[i].position = parent.position + (*taglist)[i].position + relative.position;
				MD3Special->AbsoluteTagList[i].rotation = parent.rotation * (*taglist)[i].rotation * relative.rotation;
			}
		}
	}

	//! Set the joint update mode (0-unused, 1-get joints only, 2-set joints only, 3-move and set)
	void CAnimatedMeshSceneNode::setJointMode(E_JOINT_UPDATE_ON_RENDER mode)
	{
		checkJoints();
		JointMode = mode;
	}

	//! Sets the transition time in seconds (note: This needs to enable joints, and setJointmode maybe set to 2)
	//! you must call animateJoints(), or the mesh will not animate
	void CAnimatedMeshSceneNode::setTransitionTime(FLOAT32 time)
	{
		const UINT32 ttime = (UINT32)floor32(time*1000.0f);
		if (TransitionTime == ttime)
			return;
		TransitionTime = ttime;
		if (ttime != 0)
			setJointMode(EJUOR_CONTROL);
		else
			setJointMode(EJUOR_NONE);
	}


	//! render mesh ignoring its transformation. Used with ragdolls. (culling is unaffected)
	void CAnimatedMeshSceneNode::setRenderFromIdentity(bool enable)
	{
		RenderFromIdentity = enable;
	}


	//! updates the joint positions of this mesh
	void CAnimatedMeshSceneNode::animateJoints(bool CalculateAbsolutePositions)
	{
#ifndef _IRR_COMPILE_WITH_SKINNED_MESH_SUPPORT_
		return;
#else
		if (Mesh && Mesh->getMeshType() == EAMT_SKINNED)
		{
			checkJoints();
			const FLOAT32 frame = getFrameNr(); //old?

			CSkinnedMesh* skinnedMesh = reinterpret_cast<CSkinnedMesh*>(Mesh);

			skinnedMesh->transferOnlyJointsHintsToMesh(JointChildSceneNodes);
			skinnedMesh->animateMesh(frame, 1.0f);
			skinnedMesh->recoverJointsFromMesh(JointChildSceneNodes);

			//-----------------------------------------
			//		Transition
			//-----------------------------------------

			if (Transiting != 0.f)
			{
				// Init additional matrices
				if (PretransitingSave.size()<JointChildSceneNodes.size())
				{
					for (UINT32 n = PretransitingSave.size(); n<JointChildSceneNodes.size(); ++n)
						PretransitingSave.push_back(Matrix4());
				}

				for (UINT32 n = 0; n<JointChildSceneNodes.size(); ++n)
				{
					//------Position------

					JointChildSceneNodes[n]->setPosition(
						lerp(
						PretransitingSave[n].getTranslation(),
						JointChildSceneNodes[n]->getPosition(),
						TransitingBlend));

					//------Rotation------

					//Code is slow, needs to be fixed up

					const quaternion RotationStart(PretransitingSave[n].getRotationDegrees()*DEGTORAD);
					const quaternion RotationEnd(JointChildSceneNodes[n]->getRotation()*DEGTORAD);

					quaternion QRotation;
					QRotation.slerp(RotationStart, RotationEnd, TransitingBlend);

					Vector3 tmpVector;
					QRotation.toEuler(tmpVector);
					tmpVector *= RADTODEG; //convert from radians back to degrees
					JointChildSceneNodes[n]->setRotation(tmpVector);

					//------Scale------

					//JointChildSceneNodes[n]->setScale(
					//		lerp(
					//			PretransitingSave[n].getScale(),
					//			JointChildSceneNodes[n]->getScale(),
					//			TransitingBlend));
				}
			}

			if (CalculateAbsolutePositions)
			{
				//---slow---
				for (UINT32 n = 0; n<JointChildSceneNodes.size(); ++n)
				{
					if (JointChildSceneNodes[n]->getParent() == this)
					{
						JointChildSceneNodes[n]->updateAbsolutePositionOfAllChildren(); //temp, should be an option
					}
				}
			}
		}
#endif
	}

	/*!
	*/
	void CAnimatedMeshSceneNode::checkJoints()
	{
#ifndef _IRR_COMPILE_WITH_SKINNED_MESH_SUPPORT_
		return;
#else

		if (!Mesh || Mesh->getMeshType() != EAMT_SKINNED)
			return;

		if (!JointsUsed)
		{
			for (UINT32 i = 0; i<JointChildSceneNodes.size(); ++i)
				removeChild(JointChildSceneNodes[i]);
			JointChildSceneNodes.clear();

			//Create joints for SkinnedMesh
			((CSkinnedMesh*)Mesh)->addJoints(JointChildSceneNodes, this, SceneManager);
			((CSkinnedMesh*)Mesh)->recoverJointsFromMesh(JointChildSceneNodes);

			JointsUsed = true;
			JointMode = EJUOR_READ;
		}
#endif
	}

	/*!
	*/
	void CAnimatedMeshSceneNode::beginTransition()
	{
		if (!JointsUsed)
			return;

		if (TransitionTime != 0)
		{
			//Check the array is big enough
			if (PretransitingSave.size()<JointChildSceneNodes.size())
			{
				for (UINT32 n = PretransitingSave.size(); n<JointChildSceneNodes.size(); ++n)
					PretransitingSave.push_back(Matrix4());
			}

			//Copy the position of joints
			for (UINT32 n = 0; n<JointChildSceneNodes.size(); ++n)
				PretransitingSave[n] = JointChildSceneNodes[n]->getRelativeTransformation();

			Transiting = reciprocal((FLOAT32)TransitionTime);
		}
		TransitingBlend = 0.f;
	}


	/*!
	*/
	ISceneNode* CAnimatedMeshSceneNode::clone(ISceneNode* newParent, ISceneManager* newManager)
	{
		if (!newParent)
			newParent = Parent;
		if (!newManager)
			newManager = SceneManager;

		CAnimatedMeshSceneNode* newNode =
			new CAnimatedMeshSceneNode(Mesh, NULL, newManager, ID, RelativeTranslation,
			RelativeRotation, RelativeScale);

		if (newParent)
		{
			newNode->setParent(newParent); 	// not in constructor because virtual overload for updateAbsolutePosition won't be called
			newNode->drop();
		}

		newNode->cloneMembers(this, newManager);

		newNode->Materials = Materials;
		newNode->Box = Box;
		newNode->Mesh = Mesh;
		newNode->StartFrame = StartFrame;
		newNode->EndFrame = EndFrame;
		newNode->FramesPerSecond = FramesPerSecond;
		newNode->CurrentFrameNr = CurrentFrameNr;
		newNode->JointMode = JointMode;
		newNode->JointsUsed = JointsUsed;
		newNode->TransitionTime = TransitionTime;
		newNode->Transiting = Transiting;
		newNode->TransitingBlend = TransitingBlend;
		newNode->Looping = Looping;
		newNode->ReadOnlyMaterials = ReadOnlyMaterials;
		newNode->LoopCallBack = LoopCallBack;
		if (newNode->LoopCallBack)
			newNode->LoopCallBack->grab();
		newNode->PassCount = PassCount;
		newNode->Shadow = Shadow;
		if (newNode->Shadow)
			newNode->Shadow->grab();
		newNode->JointChildSceneNodes = JointChildSceneNodes;
		newNode->PretransitingSave = PretransitingSave;
		newNode->RenderFromIdentity = RenderFromIdentity;
		newNode->MD3Special = MD3Special;

		return newNode;
	}

}