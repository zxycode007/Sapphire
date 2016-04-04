#ifndef _SAPPHIRE_I_LIGHT_MANAGER_
#define _SAPPHIRE_I_LIGHT_MANAGER_

#include "SapphireIReferenceCounter.h"
#include "SapphireIScenesManager.h"

namespace Sapphire
{
	class ILightSceneNode;

	//! ILightManager provides an interface for user applications to manipulate the list of lights in the scene.
	/** The light list can be trimmed or re-ordered before device/ hardware
	lights are created, and/or individual lights can be switched on and off
	before or after each scene node is rendered. It is assumed that the
	ILightManager implementation will store any data that it wishes to
	retain, i.e. the ISceneManager to which it is assigned, the lightList,
	the current render pass, and the current scene node. */
	class ILightManager : public IReferenceCounter
	{
	public:
		//! Called after the scene's light list has been built, but before rendering has begun.
		/** As actual device/hardware lights are not created until the
		ESNRP_LIGHT render pass, this provides an opportunity for the
		light manager to trim or re-order the light list, before any
		device/hardware lights have actually been created.
		\param lightList: the Scene Manager's light list, which
		the light manager may modify. This reference will remain valid
		until OnPostRender().
		*/
		virtual void OnPreRender(vector<ISceneNode*>::type & lightList) = 0;

		//! Called after the last scene node is rendered.
		/** After this call returns, the lightList passed to OnPreRender() becomes invalid. */
		virtual void OnPostRender(void) = 0;

		//! Called before a render pass begins
		// 在一个渲染通道开始前调用
		/** \param renderPass: the render pass that's about to begin */
		/** \param renderPass: 开始的渲染通道 */
		virtual void OnRenderPassPreRender(E_SCENE_NODE_RENDER_PASS renderPass) = 0;

		//! Called after the render pass specified in OnRenderPassPreRender() ends
		// 在OnRenderPreRender（）特定的渲染通道结束后调用
		/** \param[in] renderPass: the render pass that has finished */
		virtual void OnRenderPassPostRender(E_SCENE_NODE_RENDER_PASS renderPass) = 0;

		//! Called before the given scene node is rendered
		// 在给定场景节点被渲染之前调用
		/** \param[in] node: the scene node that's about to be rendered */
		virtual void OnNodePreRender(ISceneNode* node) = 0;

		//! Called after the the node specified in OnNodePreRender() has been rendered
		// 在OnNodePreRender（）指定节点被渲染后调用
		/** \param[in] node: the scene node that has just been rendered */
		virtual void OnNodePostRender(ISceneNode* node) = 0;
	};
}

#endif