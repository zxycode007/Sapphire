#ifndef _SAPPHIRE_I_ANIMATED_MESH_MD2_
#define _SAPPHIRE_I_ANIMATED_MESH_MD2_

#include "SapphirePrerequisites.h"
#include "SapphireIReferenceCounter.h"
#include "SapphireIAnimatedMesh.h"
#

namespace Sapphire
{
	//! MD2的动画类型
	enum EMD2_ANIMATION_TYPE
	{
		EMAT_STAND = 0,
		EMAT_RUN,
		EMAT_ATTACK,
		EMAT_PAIN_A,
		EMAT_PAIN_B,
		EMAT_PAIN_C,
		EMAT_JUMP,
		EMAT_FLIP,
		EMAT_SALUTE,
		EMAT_FALLBACK,
		EMAT_WAVE,
		EMAT_POINT,
		EMAT_CROUCH_STAND,
		EMAT_CROUCH_WALK,
		EMAT_CROUCH_ATTACK,
		EMAT_CROUCH_PAIN,
		EMAT_CROUCH_DEATH,
		EMAT_DEATH_FALLBACK,
		EMAT_DEATH_FALLFORWARD,
		EMAT_DEATH_FALLBACKSLOW,
		EMAT_BOOM,

		//! 总的动画数量
		EMAT_COUNT
	};

	//! 用于使用一些MD2网格特有函数的接口
	class IAnimatedMeshMD2 : public IAnimatedMesh
	{
	public:

		//! 获取一个默认的MD2动画类型的帧循环数据
		/** \param l 要获取帧动画类型
		\param outBegin 返回这个特定动画类型的起始帧
		\param outEnd 返回这个特定动画类型的结束帧
		\param outFPS 每这动画播放时的每秒帧数
		\return 对于一个默认MD2动画类型的起始帧，结束帧，每秒帧数 */
		virtual void getFrameLoop(EMD2_ANIMATION_TYPE l, SINT32& outBegin,
			SINT32& outEnd, SINT32& outFPS) const = 0;

		//! 通过名字来获取一个特定MD2动画类型的帧循环数据
		/** \param name 动画名
		\param outBegin 返回这个特定动画类型的起始帧
		\param outEnd 返回这个特定动画类型的结束帧
		\param outFPS 每这动画播放时的每秒帧数
		\return 对于一个默认MD2动画类型的起始帧，结束帧，每秒帧数 */
		virtual bool getFrameLoop(const c8* name,
			SINT32& outBegin, SINT32& outEnd, SINT32& outFPS) const = 0;

		//! 获取文件中MD2动画的数量
		virtual SINT32 getAnimationCount() const = 0;

		//! 获取MD2动画的名字
		/** \param nr: 基于0的动画索引 */
		virtual const c8* getAnimationName(SINT32 nr) const = 0;
	};
}



#endif 