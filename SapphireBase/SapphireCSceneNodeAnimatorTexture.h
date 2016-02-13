#ifndef _SAPPHIRE_C_SCENE_NODE_ANIMATOR_TEXTURE_
#define _SAPPHIRE_C_SCENE_NODE_ANIMATOR_TEXTURE_

#include "SapphireISceneNodeAnimatorFinishing.h"

namespace Sapphire
{

	class CSceneNodeAnimatorTexture : public ISceneNodeAnimatorFinishing
	{
	public:

		//! constructor
		CSceneNodeAnimatorTexture(const vector<ITexture*>::type& textures,
			SINT32 timePerFrame, bool loop, UINT32 now);

		//! destructor
		virtual ~CSceneNodeAnimatorTexture();

		//! animates a scene node
		virtual void animateNode(ISceneNode* node, UINT32 timeMs);

		//! Writes attributes of the scene node animator.
		virtual void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options = 0) const;

		//! Reads attributes of the scene node animator.
		virtual void deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options = 0);

		//! Returns type of the scene node animator
		virtual ESCENE_NODE_ANIMATOR_TYPE getType() const { return ESNAT_TEXTURE; }

		//! Creates a clone of this animator.
		/** Please note that you will have to drop
		(IReferenceCounted::drop()) the returned pointer after calling
		this. */
		virtual ISceneNodeAnimator* createClone(ISceneNode* node, ISceneManager* newManager = 0);

	private:

		void clearTextures();

		vector<ITexture*>::type Textures;
		UINT32 TimePerFrame;
		UINT32 StartTime;
		bool Loop;
	};
}

#endif