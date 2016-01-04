#ifndef _SAPPHIRE_I_MATERIAL_RENDERER_SERVERES_
#define _SAPPHIRE_I_MATERIAL_RENDERER_SERVERES_

#include "SapphireSMaterial.h"
#include "SapphireSVertex.h"	

namespace Sapphire
{
	class IVideoDriver;

	//! 为改变一个IVideoDriver内部状态的提供内部接口
	class IMaterialRendererServices
	{
	public:

		 
		virtual ~IMaterialRendererServices() {}

		//! 由一个IMaterialRenderer使工作更容易
		/** 如果需要设置所有的基础渲染状态
		基础渲染状态是漫反射diffuse, 环境光ambient, 高光specular, 发射光emissive颜色
		高光强度， 双线性和三线性滤波，线框模式，高洛德着色，光照，z缓冲， zwrite打开，
		背面剔除和雾化开启
		\param material 要使用的新材质
		\param lastMaterial 旧材质
		\param resetAllRenderstates 如果所有渲染状态都要被设置，设为true,无视当前它们的渲染状态
		 */
		virtual void setBasicRenderStates(const SMaterial& material,
			const SMaterial& lastMaterial,
			bool resetAllRenderstates) = 0;

		//! 通过一个名字来为Vertex Shader设置一个常量
		/** 这可以用于以GLSL或HLSL来创建一个shader。例如：如果你创建一个shader，里面有变量叫
		'mWorldViewProj'（包含WorldViewProjection矩阵）和另一个包含一个浮点数叫'fTime' 的变量，
		你可以通过在IShaderConstantSetCallBack的派生类里如下设置它们:
		 
		\code
		virtual void OnSetConstants(video::IMaterialRendererServices* services, SINT32 userData)
		{
		video::IVideoDriver* driver = services->getVideoDriver();

		FLOAT32 time = (FLOAT32)os::Timer::getTime()/100000.0f;
		services->setVertexShaderConstant("fTime", &time, 1);

		core::matrix4 worldViewProj(driver->getTransform(video::ETS_PROJECTION));
		worldViewProj *= driver->getTransform(video::ETS_VIEW);
		worldViewProj *= driver->getTransform(video::ETS_WORLD);
		services->setVertexShaderConstant("mWorldViewProj", worldViewProj.M, 16);
		}
		\endcode
		\param name 变量名字
		\param floats 浮点数组指针
		\param count 浮点数组元素个数
		\return 如果成功返回true
		*/
		virtual bool setVertexShaderConstant(const c8* name, const FLOAT32* floats, int count) = 0;

		//! 设置Bool常量的接口
		virtual bool setVertexShaderConstant(const c8* name, const bool* bools, int count) = 0;

		//! 设置Int常量的接口
		virtual bool setVertexShaderConstant(const c8* name, const SINT32* ints, int count) = 0;

		//! 设置一个Vertex Shader常量
		/** 可以在用pixel/vertex shader assembler 或ARB_fragment_program 或ARB_vertex_program创建一个shader，可以使用 
		\param data: 要设置的常量数据
		\param startRegister: 要设置的第一个寄存器
		\param constantAmount: 要设置的寄存器数量。一个寄存器由4个float组成*/
		virtual void setVertexShaderConstant(const FLOAT32* data, SINT32 startRegister, SINT32 constantAmount = 1) = 0;

		//! 基于一个名字来设置一个pixel shader的常量
		/** 如果你要使用HLSL或GLSL来创建一个shader。见setVertexShaderConstant()使用的例子
		\param name 变量名
		\param floats 浮点数组指针
		\param count 浮点数组元素数量
		\return 如果成功返回true */
		virtual bool setPixelShaderConstant(const c8* name, const FLOAT32* floats, int count) = 0;

		//!  设置Bool常量的接口
		virtual bool setPixelShaderConstant(const c8* name, const bool* bools, int count) = 0;

		//! 设置Int常量的接口
		virtual bool setPixelShaderConstant(const c8* name, const SINT32* ints, int count) = 0;

		//! 设置一个Pixel Shader 常量
		/** 可以在用pixel/vertex shader assembler 或ARB_fragment_program 或ARB_vertex_program创建一个shader，可以使用
		\param data: 要设置的常量数据
		\param startRegister: 要设置的第一个寄存器
		\param constantAmount: 要设置的寄存器数量。一个寄存器由4个float组成*/
		virtual void setPixelShaderConstant(const FLOAT32* data, SINT32 startRegister, SINT32 constantAmount = 1) = 0;

		//! 获取一个IVideoDriver 接口的指针
		/** \return 指向IVideoDriver的接口指针 */
		virtual IVideoDriver* getVideoDriver() = 0;
	};

}

#endif