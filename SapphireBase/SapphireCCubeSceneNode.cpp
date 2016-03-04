#include "SapphireCCubeSceneNode.h"
#include "SapphireIVideoDriver.h"
#include "SapphireIScenesManager.h"
#include "SapphireSVertex.h"
#include "SapphireSMeshBuffer.h"
#include "SapphireOS.h"
#include "SapphireCShadowVolumeSceneNode.h"
#include "SapphireSceneParameters.h"

namespace Sapphire
{

	/*
	011         111
	/6,8------/5        y
	/  |      / |        ^  z
	/   |     /  |        | /
	010 3,9-------2  |        |/
	|   7- - -10,4 101     *---->x
	|  /      |  /
	|/        | /
	0------11,1/
	000       100
	*/

	//! constructor
	CCubeSceneNode::CCubeSceneNode(FLOAT32 size, ISceneNode* parent, ISceneManager* mgr,
		SINT32 id, const Vector3& position,
		const Vector3& rotation, const Vector3& scale)
		: IMeshSceneNode(parent, mgr, id, position, rotation, scale),
		Mesh(0), Shadow(0), Size(size)
	{
#ifdef _DEBUG
		setDebugName("CCubeSceneNode");
#endif

		setSize();
	}


	CCubeSceneNode::~CCubeSceneNode()
	{
		if (Shadow)
			Shadow->drop();
		if (Mesh)
			Mesh->drop();
	}


	void CCubeSceneNode::setSize()
	{
		if (Mesh)
			Mesh->drop();
		//Mesh =  SceneManager->getGeometryCreator()->createCubeMesh(Vector3(Size));
		Mesh = SceneManager->getGeometryCreator()->createCubeMesh(Vector3(Size));
	}


	//! renders the node.
	void CCubeSceneNode::render()
	{
		IVideoDriver* driver = SceneManager->getVideoDriver();
		driver->setTransform(ETS_WORLD, AbsoluteTransformation);

		if (Shadow)
			Shadow->updateShadowVolumes();

		// for debug purposes only:
		SMaterial mat = Mesh->getMeshBuffer(0)->getMaterial();

		// overwrite half transparency
		if (DebugDataVisible & EDS_HALF_TRANSPARENCY)
			mat.MaterialType = EMT_TRANSPARENT_ADD_COLOR;
		driver->setMaterial(mat);
		driver->drawMeshBuffer(Mesh->getMeshBuffer(0));

		// for debug purposes only:
		if (DebugDataVisible)
		{
			SMaterial m;
			m.Lighting = false;
			m.AntiAliasing = 0;
			driver->setMaterial(m);

			if (DebugDataVisible & EDS_BBOX)
			{
				driver->draw3DBox(Mesh->getMeshBuffer(0)->getBoundingBox(), ColourValue::getColourValue(255, 255, 255, 255));
			}
			if (DebugDataVisible & EDS_BBOX_BUFFERS)
			{
				driver->draw3DBox(Mesh->getMeshBuffer(0)->getBoundingBox(),
					ColourValue::getColourValue(255, 190, 128, 128));
			}
			if (DebugDataVisible & EDS_NORMALS)
			{
				// draw normals
				const FLOAT32 debugNormalLength = SceneManager->getParameters()->getAttributeAsFloat(DEBUG_NORMAL_LENGTH);
				const ColourValue debugNormalColor = SceneManager->getParameters()->getAttributeAsColor(DEBUG_NORMAL_COLOR);
				const UINT32 count = Mesh->getMeshBufferCount();

				for (UINT32 i = 0; i != count; ++i)
				{
					driver->drawMeshBufferNormals(Mesh->getMeshBuffer(i), debugNormalLength, debugNormalColor);
				}
			}

			// show mesh
			if (DebugDataVisible & EDS_MESH_WIRE_OVERLAY)
			{
				m.Wireframe = true;
				driver->setMaterial(m);

				driver->drawMeshBuffer(Mesh->getMeshBuffer(0));
			}
		}
	}


	//! returns the axis aligned bounding box of this node
	const AxisAlignedBox& CCubeSceneNode::getBoundingBox() const
	{
		return Mesh->getMeshBuffer(0)->getBoundingBox();
	}


	//! Removes a child from this scene node.
	//! Implemented here, to be able to remove the shadow properly, if there is one,
	//! or to remove attached childs.
	bool CCubeSceneNode::removeChild(ISceneNode* child)
	{
		if (child && Shadow == child)
		{
			Shadow->drop();
			Shadow = 0;
		}

		return ISceneNode::removeChild(child);
	}


	//! Creates shadow volume scene node as child of this node
	//! and returns a pointer to it.
	IShadowVolumeSceneNode* CCubeSceneNode::addShadowVolumeSceneNode(
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


	void CCubeSceneNode::OnRegisterSceneNode()
	{
		if (IsVisible)
			SceneManager->registerNodeForRendering(this);
		ISceneNode::OnRegisterSceneNode();
	}


	//! returns the material based on the zero based index i.
	SMaterial& CCubeSceneNode::getMaterial(UINT32 i)
	{
		return Mesh->getMeshBuffer(0)->getMaterial();
	}


	//! returns amount of materials used by this scene node.
	UINT32 CCubeSceneNode::getMaterialCount() const
	{
		return 1;
	}


	//! Writes attributes of the scene node.
	void CCubeSceneNode::serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const
	{
		ISceneNode::serializeAttributes(out, options);

		out->addFloat("Size", Size);
	}


	//! Reads attributes of the scene node.
	void CCubeSceneNode::deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options)
	{
		FLOAT32 newSize = in->getAttributeAsFloat("Size");
		newSize = Math::max_(newSize, 0.0001f);
		if (newSize != Size)
		{
			Size = newSize;
			setSize();
		}

		ISceneNode::deserializeAttributes(in, options);
	}


	//! Creates a clone of this scene node and its children.
	ISceneNode* CCubeSceneNode::clone(ISceneNode* newParent, ISceneManager* newManager)
	{
		if (!newParent)
			newParent = Parent;
		if (!newManager)
			newManager = SceneManager;

		CCubeSceneNode* nb = new CCubeSceneNode(Size, newParent,
			newManager, ID, RelativeTranslation);

		nb->cloneMembers(this, newManager);
		nb->getMaterial(0) = getMaterial(0);
		nb->Shadow = Shadow;
		if (nb->Shadow)
			nb->Shadow->grab();

		if (newParent)
			nb->drop();
		return nb;
	}
}