#ifndef _SAPPHIRE_C_DUMMY_TRANSFORMATION_SCENE_NODE_
#define _SAPPHIRE_C_DUMMY_TRANSFORMATION_SCENE_NODE_

#include "SapphireIDummyTransformationSceneNode.h"

namespace Sapphire
{
	class CDummyTransformationSceneNode : public IDummyTransformationSceneNode
	{
	public:

		//! constructor
		CDummyTransformationSceneNode(ISceneNode* parent, ISceneManager* mgr, SINT32 id);

		//! returns the axis aligned bounding box of this node
		virtual const AxisAlignedBox& getBoundingBox() const;

		//! Returns a reference to the current relative transformation matrix.
		//! This is the matrix, this scene node uses instead of scale, translation
		//! and rotation.
		virtual Matrix4& getRelativeTransformationMatrix();

		//! Returns the relative transformation of the scene node.
		virtual Matrix4 getRelativeTransformation() const;

		//! does nothing.
		virtual void render() {}

		//! Returns type of the scene node
		virtual ESCENE_NODE_TYPE getType() const { return ESNT_DUMMY_TRANSFORMATION; }

		//! Creates a clone of this scene node and its children.
		virtual ISceneNode* clone(ISceneNode* newParent = 0, ISceneManager* newManager = 0);


	private:

		// TODO: We can add least add some warnings to find troubles faster until we have 
		// fixed bug id 2318691.
		virtual const Vector3& getScale() const;
		virtual void setScale(const Vector3& scale);
		virtual const Vector3& getRotation() const;
		virtual void setRotation(const Vector3& rotation);
		virtual const Vector3& getPosition() const;
		virtual void setPosition(const Vector3& newpos);

		Matrix4 RelativeTransformationMatrix;
		AxisAlignedBox Box;
	};
}

#endif