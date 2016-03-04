#include "SapphireCDummyTransformationSceneNode.h"
#include "SapphireOS.h"

namespace Sapphire
{

	//! constructor
	CDummyTransformationSceneNode::CDummyTransformationSceneNode(
		ISceneNode* parent, ISceneManager* mgr, SINT32 id)
		: IDummyTransformationSceneNode(parent, mgr, id)
	{
#ifdef _DEBUG
		setDebugName("CDummyTransformationSceneNode");
#endif

		setAutomaticCulling( EAC_OFF);
	}


	//! returns the axis aligned bounding box of this node
	const AxisAlignedBox& CDummyTransformationSceneNode::getBoundingBox() const
	{
		return Box;
	}


	//! Returns a reference to the current relative transformation matrix.
	//! This is the matrix, this scene node uses instead of scale, translation
	//! and rotation.
	Matrix4& CDummyTransformationSceneNode::getRelativeTransformationMatrix()
	{
		return RelativeTransformationMatrix;
	}


	//! Returns the relative transformation of the scene node.
	Matrix4 CDummyTransformationSceneNode::getRelativeTransformation() const
	{
		return RelativeTransformationMatrix;
	}

	//! Creates a clone of this scene node and its children.
	ISceneNode* CDummyTransformationSceneNode::clone(ISceneNode* newParent, ISceneManager* newManager)
	{
		if (!newParent)
			newParent = Parent;
		if (!newManager)
			newManager = SceneManager;

		CDummyTransformationSceneNode* nb = new CDummyTransformationSceneNode(newParent,
			newManager, ID);

		nb->cloneMembers(this, newManager);
		nb->RelativeTransformationMatrix = RelativeTransformationMatrix;
		nb->Box = Box;

		if (newParent)
			nb->drop();
		return nb;
	}

	const Vector3& CDummyTransformationSceneNode::getScale() const
	{
		Printer::log("CDummyTransformationSceneNode::getScale() does not contain the relative transformation.", LML_DEBUG);
		return RelativeScale;
	}

	void CDummyTransformationSceneNode::setScale(const Vector3& scale)
	{
		Printer::log("CDummyTransformationSceneNode::setScale() does not affect the relative transformation.", LML_DEBUG);
		RelativeScale = scale;
	}

	const Vector3& CDummyTransformationSceneNode::getRotation() const
	{
		Printer::log("CDummyTransformationSceneNode::getRotation() does not contain the relative transformation.", LML_DEBUG);
		return RelativeRotation;
	}

	void CDummyTransformationSceneNode::setRotation(const Vector3& rotation)
	{
		Printer::log("CDummyTransformationSceneNode::setRotation() does not affect the relative transformation.", LML_DEBUG);
		RelativeRotation = rotation;
	}

	const Vector3& CDummyTransformationSceneNode::getPosition() const
	{
		Printer::log("CDummyTransformationSceneNode::getPosition() does not contain the relative transformation.", LML_DEBUG);
		return RelativeTranslation;
	}

	void CDummyTransformationSceneNode::setPosition(const Vector3& newpos)
	{
		Printer::log("CDummyTransformationSceneNode::setPosition() does not affect the relative transformation.", LML_DEBUG);
		RelativeTranslation = newpos;
	}

}