#ifndef _SAPPHIRE_I_BILLBOARD_TEXT_SCENE_NODE_
#define _SAPPHIRE_I_BILLBOARD_TEXT_SCENE_NODE_

#include "SapphireIBillboardSceneNode.h"

namespace Sapphire
{
	//! A billboard text scene node.
	/** Acts like a billboard which displays the currently set text.
	Due to the exclusion of RTTI in Irrlicht we have to avoid multiple
	inheritance. Hence, changes to the ITextSceneNode interface have
	to be copied here manually.
	*/
	class IBillboardTextSceneNode : public IBillboardSceneNode
	{
	public:

		//! Constructor
		IBillboardTextSceneNode(ISceneNode* parent, ISceneManager* mgr, SINT32 id,
			const Vector3& position = Vector3(0, 0, 0))
			: IBillboardSceneNode(parent, mgr, id, position) {}

		//! Sets the size of the billboard.
		virtual void setSize(const dimension2df& size) = 0;

		//! Returns the size of the billboard.
		virtual const dimension2df& getSize() const = 0;

		//! Set the color of all vertices of the billboard
		/** \param overallColor: the color to set */
		virtual void setColor(const ColourValue & overallColor) = 0;

		//! Set the color of the top and bottom vertices of the billboard
		/** \param topColor: the color to set the top vertices
		\param bottomColor: the color to set the bottom vertices */
		virtual void setColor(const ColourValue & topColor, const ColourValue & bottomColor) = 0;

		//! Gets the color of the top and bottom vertices of the billboard
		/** \param topColor: stores the color of the top vertices
		\param bottomColor: stores the color of the bottom vertices */
		virtual void getColor(ColourValue & topColor, ColourValue & bottomColor) const = 0;

		//! sets the text string
		virtual void setText(const wchar_t* text) = 0;

		//! sets the color of the text
		virtual void setTextColor(ColourValue color) = 0;
	};
}

#endif