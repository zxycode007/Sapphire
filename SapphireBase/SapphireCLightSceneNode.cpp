#include "SapphireCLightSceneNode.h"
#include "SapphireIVideoDriver.h"
#include "SapphireIScenesManager.h"
#include "SapphireICameraSceneNode.h"

#include "SapphireOS.h"


namespace Sapphire
{

	//! constructor
	CLightSceneNode::CLightSceneNode(ISceneNode* parent, ISceneManager* mgr, SINT32 id,
		const Vector3& position, ColourValue color, FLOAT32 radius)
		: ILightSceneNode(parent, mgr, id, position), DriverLightIndex(-1), LightIsOn(true)
	{
#ifdef _DEBUG
		setDebugName("CLightSceneNode");
#endif

		LightData.DiffuseColor = color;
		// set some useful specular color
		LightData.SpecularColor = color.getInterpolated(ColourValue::getColourValue(255, 255, 255, 255), 0.7f);

		setRadius(radius);
	}


	//! pre render event
	void CLightSceneNode::OnRegisterSceneNode()
	{
		doLightRecalc();

		if (IsVisible)
			SceneManager->registerNodeForRendering(this, ESNRP_LIGHT);

		ISceneNode::OnRegisterSceneNode();
	}


	//! render
	void CLightSceneNode::render()
	{
		IVideoDriver* driver = SceneManager->getVideoDriver();
		if (!driver)
			return;

		if (DebugDataVisible & EDS_BBOX)
		{
			driver->setTransform(ETS_WORLD, AbsoluteTransformation);
			SMaterial m;
			m.Lighting = false;
			driver->setMaterial(m);

			switch (LightData.Type)
			{
			case ELT_POINT:
			case ELT_SPOT:
				driver->draw3DBox(BBox, LightData.DiffuseColor);
				break;

			case ELT_DIRECTIONAL:
				driver->draw3DLine(Vector3(0.f, 0.f, 0.f),
					LightData.Direction * LightData.Radius,
					LightData.DiffuseColor);
				break;
			default:
				break;
			}
		}

		DriverLightIndex = driver->addDynamicLight(LightData);
		setVisible(LightIsOn);
	}


	//! sets the light data
	void CLightSceneNode::setLightData(const SLight& light)
	{
		LightData = light;
	}


	//! \return Returns the light data.
	const SLight& CLightSceneNode::getLightData() const
	{
		return LightData;
	}


	//! \return Returns the light data.
	SLight& CLightSceneNode::getLightData()
	{
		return LightData;
	}

	void CLightSceneNode::setVisible(bool isVisible)
	{
		ISceneNode::setVisible(isVisible);

		if (DriverLightIndex < 0)
			return;
		IVideoDriver* driver = SceneManager->getVideoDriver();
		if (!driver)
			return;

		LightIsOn = isVisible;
		driver->turnLightOn((UINT32)DriverLightIndex, LightIsOn);
	}

	//! returns the axis aligned bounding box of this node
	const AxisAlignedBox& CLightSceneNode::getBoundingBox() const
	{
		return BBox;
	}


	//! Sets the light's radius of influence.
	/** Outside this radius the light won't lighten geometry and cast no
	shadows. Setting the radius will also influence the attenuation, setting
	it to (0,1/radius,0). If you want to override this behavior, set the
	attenuation after the radius.
	\param radius The new radius. */
	void CLightSceneNode::setRadius(Real radius)
	{
		LightData.Radius = radius;
		LightData.Attenuation.set(0.f, 1.f / radius, 0.f);
		doLightRecalc();
	}


	//! Gets the light's radius of influence.
	/** \return The current radius. */
	Real CLightSceneNode::getRadius() const
	{
		return LightData.Radius;
	}


	//! Sets the light type.
	/** \param type The new type. */
	void CLightSceneNode::setLightType(E_LIGHT_TYPE type)
	{
		LightData.Type = type;
	}


	//! Gets the light type.
	/** \return The current light type. */
	E_LIGHT_TYPE CLightSceneNode::getLightType() const
	{
		return LightData.Type;
	}


	//! Sets whether this light casts shadows.
	/** Enabling this flag won't automatically cast shadows, the meshes
	will still need shadow scene nodes attached. But one can enable or
	disable distinct lights for shadow casting for performance reasons.
	\param shadow True if this light shall cast shadows. */
	void CLightSceneNode::enableCastShadow(bool shadow)
	{
		LightData.CastShadows = shadow;
	}


	//! Check whether this light casts shadows.
	/** \return True if light would cast shadows, else false. */
	bool CLightSceneNode::getCastShadow() const
	{
		return LightData.CastShadows;
	}


	void CLightSceneNode::doLightRecalc()
	{
		if ((LightData.Type == ELT_SPOT) || (LightData.Type == ELT_DIRECTIONAL))
		{
			LightData.Direction = Vector3(.0f, .0f, 1.0f);
			//getAbsoluteTransformation().rotateVect(LightData.Direction);
			Matrix4 mat = getAbsoluteTransformation();
			LightData.Direction = mat.rotateVect(LightData.Direction);
			LightData.Direction.normalize();
		}
		if ((LightData.Type == ELT_SPOT) || (LightData.Type == ELT_POINT))
		{ 
			const FLOAT32 r = LightData.Radius * LightData.Radius * 0.5f;
			//BBox.MaxEdge.set(r, r, r);
			//BBox.MinEdge.set(-r, -r, -r);
			BBox.setMaximum(Vector3(r, r, r));
			BBox.setMinimum(Vector3(-r, -r, -r));
			//setAutomaticCulling( scene::EAC_BOX );
			setAutomaticCulling(EAC_OFF);
			LightData.Position = getAbsolutePosition();
		}
		if (LightData.Type == ELT_DIRECTIONAL)
		{
			BBox.reset(Vector3(0, 0, 0));
			setAutomaticCulling(EAC_OFF);
		}
	}


	//! Writes attributes of the scene node.
	void CLightSceneNode::serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const
	{
		ILightSceneNode::serializeAttributes(out, options);

		out->addColor("AmbientColor", LightData.AmbientColor);
		out->addColor("DiffuseColor", LightData.DiffuseColor);
		out->addColor("SpecularColor", LightData.SpecularColor);
		out->addVector3d("Attenuation", LightData.Attenuation);
		out->addFloat("Radius", LightData.Radius);
		out->addFloat("OuterCone", LightData.OuterCone);
		out->addFloat("InnerCone", LightData.InnerCone);
		out->addFloat("Falloff", LightData.Falloff);
		out->addBool("CastShadows", LightData.CastShadows);
		out->addEnum("LightType", LightData.Type, LightTypeNames);
	}

	//! Reads attributes of the scene node.
	void CLightSceneNode::deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options)
	{
		LightData.AmbientColor = in->getAttributeAsColor("AmbientColor");
		LightData.DiffuseColor = in->getAttributeAsColor("DiffuseColor");
		LightData.SpecularColor = in->getAttributeAsColor("SpecularColor");

		//TODO: clearify Radius and Linear Attenuation
#if 0
		setRadius(in->getAttributeAsFloat("Radius"));
#else
		LightData.Radius = in->getAttributeAsFloat("Radius");
#endif

		if (in->existsAttribute("Attenuation")) // might not exist in older files
			LightData.Attenuation = in->getAttributeAsVector3d("Attenuation");

		if (in->existsAttribute("OuterCone")) // might not exist in older files
			LightData.OuterCone = in->getAttributeAsFloat("OuterCone");
		if (in->existsAttribute("InnerCone")) // might not exist in older files
			LightData.InnerCone = in->getAttributeAsFloat("InnerCone");
		if (in->existsAttribute("Falloff")) // might not exist in older files
			LightData.Falloff = in->getAttributeAsFloat("Falloff");
		LightData.CastShadows = in->getAttributeAsBool("CastShadows");
		LightData.Type = (E_LIGHT_TYPE)in->getAttributeAsEnumeration("LightType", LightTypeNames);

		doLightRecalc();

		ILightSceneNode::deserializeAttributes(in, options);
	}

	//! Creates a clone of this scene node and its children.
	ISceneNode* CLightSceneNode::clone(ISceneNode* newParent, ISceneManager* newManager)
	{
		if (!newParent)
			newParent = Parent;
		if (!newManager)
			newManager = SceneManager;

		CLightSceneNode* nb = new CLightSceneNode(newParent,
			newManager, ID, RelativeTranslation, LightData.DiffuseColor, LightData.Radius);

		nb->cloneMembers(this, newManager);
		nb->LightData = LightData;
		nb->BBox = BBox;

		if (newParent)
			nb->drop();
		return nb;
	}
}