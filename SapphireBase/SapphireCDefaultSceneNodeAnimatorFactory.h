#ifndef _SAPPHIRE_C_DEFAULT_ANIMATOR_FACTORY_
#define _SAPPHIRE_C_DEFAULT_ANIMATOR_FACTORY_

#include "SapphireISceneNodeAnimatorFactory.h"

namespace Sapphire
{
	class ICursorControl;

	class ISceneNodeAnimator;
	class ISceneManager;

	//!	Interface making it possible to dynamicly create scene nodes animators 
	class CDefaultSceneNodeAnimatorFactory : public ISceneNodeAnimatorFactory
	{
	public:

		CDefaultSceneNodeAnimatorFactory(ISceneManager* mgr, ICursorControl* crs);

		virtual ~CDefaultSceneNodeAnimatorFactory();

		//! creates a scene node animator based on its type id
		/** \param type: Type of the scene node animator to add.
		\param target: Target scene node of the new animator.
		\return Returns pointer to the new scene node animator or null if not successful. You need to
		drop this pointer after calling this, see IReferenceCounted::drop() for details. */
		virtual ISceneNodeAnimator* createSceneNodeAnimator(ESCENE_NODE_ANIMATOR_TYPE type, ISceneNode* target);

		//! creates a scene node animator based on its type name
		/** \param typeName: Type of the scene node animator to add.
		\param target: Target scene node of the new animator.
		\return Returns pointer to the new scene node animator or null if not successful. You need to
		drop this pointer after calling this, see IReferenceCounted::drop() for details. */
		virtual ISceneNodeAnimator* createSceneNodeAnimator(const char* typeName, ISceneNode* target);

		//! returns amount of scene node animator types this factory is able to create
		virtual UINT32 getCreatableSceneNodeAnimatorTypeCount() const;

		//! returns type of a createable scene node animator type
		/** \param idx: Index of scene node animator type in this factory. Must be a value between 0 and
		getCreatableSceneNodeTypeCount() */
		virtual ESCENE_NODE_ANIMATOR_TYPE getCreateableSceneNodeAnimatorType(UINT32 idx) const;

		//! returns type name of a createable scene node animator type 
		/** \param idx: Index of scene node animator type in this factory. Must be a value between 0 and
		getCreatableSceneNodeAnimatorTypeCount() */
		virtual const c8* getCreateableSceneNodeAnimatorTypeName(UINT32 idx) const;

		//! returns type name of a createable scene node animator type 
		/** \param type: Type of scene node animator.
		\return: Returns name of scene node animator type if this factory can create the type, otherwise 0. */
		virtual const c8* getCreateableSceneNodeAnimatorTypeName(ESCENE_NODE_ANIMATOR_TYPE type) const;

	private:

		ESCENE_NODE_ANIMATOR_TYPE getTypeFromName(const c8* name) const;

		ISceneManager* Manager;
		ICursorControl* CursorControl;
	};
}

#endif