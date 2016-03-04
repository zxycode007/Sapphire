#include "SapphireCSkyBoxSceneNode.h"
#include "SapphireIVideoDriver.h"
#include "SapphireIScenesManager.h"
#include "SapphireICameraSceneNode.h"
#include "SapphireSVertex.h"
#include "SapphireOS.h"

namespace Sapphire
{

	//! constructor
	CSkyBoxSceneNode::CSkyBoxSceneNode(ITexture* top, ITexture* bottom, ITexture* left,
		ITexture* right, ITexture* front, ITexture* back, ISceneNode* parent, ISceneManager* mgr, SINT32 id)
		: ISceneNode(parent, mgr, id)
	{
#ifdef _DEBUG
		setDebugName("CSkyBoxSceneNode");
#endif

		setAutomaticCulling(EAC_OFF);
		Box.setMaximum(0, 0, 0);
		Box.setMinimum(0, 0, 0);

		// create indices

		Indices[0] = 0;
		Indices[1] = 1;
		Indices[2] = 2;
		Indices[3] = 3;

		// create material

		SMaterial mat;
		mat.Lighting = false;
		mat.ZBuffer = ECFN_NEVER;
		mat.ZWriteEnable = false;
		mat.AntiAliasing = 0;
		mat.TextureLayer[0].TextureWrapU = ETC_CLAMP_TO_EDGE;
		mat.TextureLayer[0].TextureWrapV = ETC_CLAMP_TO_EDGE;

		/* Hey, I am no artist, but look at that
		cool ASCII art I made! ;)

		-111         111
		/6--------/5        y
		/  |      / |        ^  z
		/   |   11-1 |        | /
		-11-1 3---------2  |        |/
		|   7- - -| -4 1-11    *---->x
		| -1-11   |  /       3-------|2
		|/        | /         |    //|
		0---------1/          |  //  |
		-1-1-1     1-1-1         |//    |
		0--------1
		*/

		ITexture* tex = front;
		if (!tex) tex = left;
		if (!tex) tex = back;
		if (!tex) tex = right;
		if (!tex) tex = top;
		if (!tex) tex = bottom;

		const FLOAT32 onepixel = tex ? (1.0f / (tex->getSize().Width * 1.5f)) : 0.0f;
		const FLOAT32 t = 1.0f - onepixel;
		const FLOAT32 o = 0.0f + onepixel;

		// create front side

		Material[0] = mat;
		Material[0].setTexture(0, front);
		Vertices[0] = S3DVertex(-1, -1, -1, 0, 0, 1, ColourValue::getColourValue(255, 255, 255, 255), t, t);
		Vertices[1] = S3DVertex(1, -1, -1, 0, 0, 1, ColourValue::getColourValue(255, 255, 255, 255), o, t);
		Vertices[2] = S3DVertex(1, 1, -1, 0, 0, 1, ColourValue::getColourValue(255, 255, 255, 255), o, o);
		Vertices[3] = S3DVertex(-1, 1, -1, 0, 0, 1, ColourValue::getColourValue(255, 255, 255, 255), t, o);

		// create left side

		Material[1] = mat;
		Material[1].setTexture(0, left);
		Vertices[4] = S3DVertex(1, -1, -1, -1, 0, 0, ColourValue::getColourValue(255, 255, 255, 255), t, t);
		Vertices[5] = S3DVertex(1, -1, 1, -1, 0, 0, ColourValue::getColourValue(255, 255, 255, 255), o, t);
		Vertices[6] = S3DVertex(1, 1, 1, -1, 0, 0, ColourValue::getColourValue(255, 255, 255, 255), o, o);
		Vertices[7] = S3DVertex(1, 1, -1, -1, 0, 0, ColourValue::getColourValue(255, 255, 255, 255), t, o);

		// create back side

		Material[2] = mat;
		Material[2].setTexture(0, back);
		Vertices[8] = S3DVertex(1, -1, 1, 0, 0, -1, ColourValue::getColourValue(255, 255, 255, 255), t, t);
		Vertices[9] = S3DVertex(-1, -1, 1, 0, 0, -1, ColourValue::getColourValue(255, 255, 255, 255), o, t);
		Vertices[10] = S3DVertex(-1, 1, 1, 0, 0, -1, ColourValue::getColourValue(255, 255, 255, 255), o, o);
		Vertices[11] = S3DVertex(1, 1, 1, 0, 0, -1, ColourValue::getColourValue(255, 255, 255, 255), t, o);

		// create right side

		Material[3] = mat;
		Material[3].setTexture(0, right);
		Vertices[12] = S3DVertex(-1, -1, 1, 1, 0, 0, ColourValue::getColourValue(255, 255, 255, 255), t, t);
		Vertices[13] = S3DVertex(-1, -1, -1, 1, 0, 0, ColourValue::getColourValue(255, 255, 255, 255), o, t);
		Vertices[14] = S3DVertex(-1, 1, -1, 1, 0, 0, ColourValue::getColourValue(255, 255, 255, 255), o, o);
		Vertices[15] = S3DVertex(-1, 1, 1, 1, 0, 0, ColourValue::getColourValue(255, 255, 255, 255), t, o);

		// create top side

		Material[4] = mat;
		Material[4].setTexture(0, top);
		Vertices[16] = S3DVertex(1, 1, -1, 0, -1, 0, ColourValue::getColourValue(255, 255, 255, 255), t, t);
		Vertices[17] = S3DVertex(1, 1, 1, 0, -1, 0, ColourValue::getColourValue(255, 255, 255, 255), o, t);
		Vertices[18] = S3DVertex(-1, 1, 1, 0, -1, 0, ColourValue::getColourValue(255, 255, 255, 255), o, o);
		Vertices[19] = S3DVertex(-1, 1, -1, 0, -1, 0, ColourValue::getColourValue(255, 255, 255, 255), t, o);

		// create bottom side

		Material[5] = mat;
		Material[5].setTexture(0, bottom);
		Vertices[20] = S3DVertex(1, -1, 1, 0, 1, 0, ColourValue::getColourValue(255, 255, 255, 255), o, o);
		Vertices[21] = S3DVertex(1, -1, -1, 0, 1, 0, ColourValue::getColourValue(255, 255, 255, 255), t, o);
		Vertices[22] = S3DVertex(-1, -1, -1, 0, 1, 0, ColourValue::getColourValue(255, 255, 255, 255), t, t);
		Vertices[23] = S3DVertex(-1, -1, 1, 0, 1, 0, ColourValue::getColourValue(255, 255, 255, 255), o, t);
	}


	//! renders the node.
	void CSkyBoxSceneNode::render()
	{
		IVideoDriver* driver = SceneManager->getVideoDriver();
		ICameraSceneNode* camera = SceneManager->getActiveCamera();

		if (!camera || !driver)
			return;

		if (!camera->isOrthogonal())
		{
			// draw perspective skybox

			Matrix4 translate(AbsoluteTransformation);
			//translate.setTranslation(camera->getAbsolutePosition());
			translate.setTrans(camera->getAbsolutePosition());

			// Draw the sky box between the near and far clip plane
			const FLOAT32 viewDistance = (camera->getNearValue() + camera->getFarValue()) * 0.5f;
			Matrix4 scale;
			scale.setScale(Vector3(viewDistance, viewDistance, viewDistance));

			driver->setTransform(ETS_WORLD, translate * scale);

			for (SINT32 i = 0; i<6; ++i)
			{
				driver->setMaterial(Material[i]);
				driver->drawIndexedTriangleFan(&Vertices[i * 4], 4, Indices, 2);
			}
		}
		else
		{
			// draw orthogonal skybox,
			// simply choose one texture and draw it as 2d picture.
			// there could be better ways to do this, but currently I think this is ok.

			Vector3 lookVect = camera->getTarget() - camera->getAbsolutePosition();
			lookVect.normalize();
			Vector3 absVect(Math::Abs(lookVect.x),
				Math::Abs(lookVect.y),
				Math::Abs(lookVect.z));

			int idx = 0;

			if (absVect.x >= absVect.y && absVect.x >= absVect.z)
			{
				// x direction
				idx = lookVect.x > 0 ? 0 : 2;
			}
			else
				if (absVect.y >= absVect.x && absVect.y >= absVect.z)
				{
					// y direction
					idx = lookVect.y > 0 ? 4 : 5;
				}
				else
					if (absVect.z >= absVect.x && absVect.z >= absVect.y)
					{
						// z direction
						idx = lookVect.z > 0 ? 1 : 3;
					}

			ITexture* tex = Material[idx].getTexture(0);

			if (tex)
			{
				rect<SINT32> rctDest(Position2d(-1, 0),
					dimension2di(driver->getCurrentRenderTargetSize()));
				rect<SINT32> rctSrc(Position2d(0, 0),
					dimension2di(tex->getSize()));

				driver->draw2DImage(tex, rctDest, rctSrc);
			}
		}
	}



	//! returns the axis aligned bounding box of this node
	const AxisAlignedBox& CSkyBoxSceneNode::getBoundingBox() const
	{
		return Box;
	}


	void CSkyBoxSceneNode::OnRegisterSceneNode()
	{
		if (IsVisible)
			SceneManager->registerNodeForRendering(this, ESNRP_SKY_BOX);

		ISceneNode::OnRegisterSceneNode();
	}


	//! returns the material based on the zero based index i. To get the amount
	//! of materials used by this scene node, use getMaterialCount().
	//! This function is needed for inserting the node into the scene hirachy on a
	//! optimal position for minimizing renderstate changes, but can also be used
	//! to directly modify the material of a scene node.
	SMaterial& CSkyBoxSceneNode::getMaterial(UINT32 i)
	{
		return Material[i];
	}


	//! returns amount of materials used by this scene node.
	UINT32 CSkyBoxSceneNode::getMaterialCount() const
	{
		return 6;
	}


	//! Creates a clone of this scene node and its children.
	ISceneNode* CSkyBoxSceneNode::clone(ISceneNode* newParent, ISceneManager* newManager)
	{
		if (!newParent) newParent = Parent;
		if (!newManager) newManager = SceneManager;

		CSkyBoxSceneNode* nb = new CSkyBoxSceneNode(0, 0, 0, 0, 0, 0, newParent,
			newManager, ID);

		nb->cloneMembers(this, newManager);

		for (UINT32 i = 0; i<6; ++i)
			nb->Material[i] = Material[i];

		if (newParent)
			nb->drop();
		return nb;
	}

}