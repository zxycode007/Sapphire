#ifndef _SAPPHIRE_SLIGHT_
#define _SAPPHIRE_SLIGHT_

#include "SapphirePrerequisites.h"
#include "SapphireColorValue.h"
#include "SapphireVector3.h"

namespace Sapphire
{
	//! 不同光源类型的枚举
	enum E_LIGHT_TYPE
	{
		//! point light, it has a position in space and radiates light in all directions
		ELT_POINT,
		//! spot light, it has a position in space, a direction, and a limited cone of influence
		ELT_SPOT,
		//! directional light, coming from a direction from an infinite distance
		ELT_DIRECTIONAL,

		//! Only used for counting the elements of this enum
		ELT_COUNT
	};

	//! 光源类型名
	const c8* const LightTypeNames[] =
	{
		"Point",
		"Spot",
		"Directional",
		0
	};

	//! 保存描述动态点光源的结果
	/** 支持点光源，聚光灯，平行光
	*/
	struct SLight
	{
		SLight() : AmbientColor(0.f, 0.f, 0.f), DiffuseColor(1.f, 1.f, 1.f),
			SpecularColor(1.f, 1.f, 1.f), Attenuation(1.f, 0.f, 0.f),
			OuterCone(45.f), InnerCone(0.f), Falloff(2.f),
			Position(0.f, 0.f, 0.f), Direction(0.f, 0.f, 1.f),
			Radius(100.f), Type(ELT_POINT), CastShadows(true)
		{}

		//! 光源的发射的环境光颜色 
		ColourValue AmbientColor;

		//! 光源发射的漫反射光颜色
		/** 这是你要设置的光源主颜色*/
		ColourValue DiffuseColor;

		//! 光源发射的高光颜色
		/** 对于拥有镜面反射高光使用的细节  见SMaterial::Shininess */
		ColourValue SpecularColor;

		//! 衰减因子(常量, 线性, 二次的)
		/** 改变光源强度随距离影响的程度，能同时设置半径来修改，衰减将改变(0,1.f/radius,0)，
		可以在半径设置后覆盖
		 */
		Vector3 Attenuation;

		//! 聚光灯外锥角度，对其它光源无效
		Real OuterCone;

		//! 聚光灯内锥角度，对其它光源无效
		Real InnerCone;

		//! 在内锥和外锥之间降低的光源强度
		Real Falloff;

		//! 只读! 光源的位置
		/** 如果类型设置为ELT_DIRECTIONAL, 无效. 通过光源场景节点位置来改变 */
		Vector3 Position;

		//! 只读! 光源的方向
		/** 如果类型设定为ELT_POINT, 无效. 通过光源场景节点旋转来改变. */
		Vector3 Direction;

		//! 只读! 光源的半径. 半径内所有的东西都会被参与到此光照计算
		Real Radius;

		//! 只读! 光源的类型 默认：ELT_POINT
		E_LIGHT_TYPE Type;

		//! 只读! 光源是否投下阴影？
		bool CastShadows : 1;
	};
}

#endif