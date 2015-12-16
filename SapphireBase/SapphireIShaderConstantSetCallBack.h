#ifndef _SAPPHIRE_ISHADER_CONSTANT_SET_CALLBACK_
#define _SAPPHIRE_ISHADER_CONSTANT_SET_CALLBACK_

#include "SapphirePrerequisites.h"
#include "SapphireIReferenceCounter.h"

namespace Sapphire
{
	class IMaterialRendererServices;
	class SMaterial;

	//! 这个接口使GPU程序在每一帧设置常量成为可能
	/** 在自己的类实现这个接口并且藏创建一个shader时传递一个指针到IGPUProgrammingServices的一个方法里
	OnSetConstants方法将在每一帧调用
	*/
	class IShaderConstantSetCallBack : public virtual IReferenceCounter
	{
	public:

		//! 调用让回调指定用的哪个材质（可选方法）
		/**
		 
		class MyCallBack : public IShaderConstantSetCallBack
		{
		const video::SMaterial *UsedMaterial;

		OnSetMaterial(const video::SMaterial& material)
		{
		UsedMaterial=&material;
		}

		OnSetConstants(IMaterialRendererServices* services, s32 userData)
		{
		services->setVertexShaderConstant("myColor", reinterpret_cast<f32*>(&UsedMaterial->color), 4);
		}
		}
		 
		*/
		virtual void OnSetMaterial(const SMaterial& material) { }

		//! 当对于材质渲染器应该要设置顶点/像素 着色器常量，通过引擎调用
		/**
		在一个自己的类里实现IShaderConstantSetCallBack，并且实现自己的OnSetConstants方法，用于当创建shader时
		给IMaterialRendererServices传递一个这个类的指针，再到IGPUProgrammingServices的一个方法。
		OnSetConstants方法将每次都在用你的shader材质绘制几何体之前调用。一个简单实现如下：
		 
		virtual void OnSetConstants(video::IMaterialRendererServices* services, s32 userData)
		{
		video::IVideoDriver* driver = services->getVideoDriver();

		// 在寄存器4设置剪裁矩阵
		core::matrix4 worldViewProj(driver->getTransform(video::ETS_PROJECTION));
		worldViewProj *= driver->getTransform(video::ETS_VIEW);
		worldViewProj *= driver->getTransform(video::ETS_WORLD);
		services->setVertexShaderConstant(&worldViewProj.M[0], 4, 4);
		// 对于高级shader语言，这将有其他解决方法
		//services->setVertexShaderConstant("mWorldViewProj", worldViewProj.M, 16);

		// 在寄存器9设置一些光照颜色
		video::SColorf col(0.0f,1.0f,1.0f,0.0f);
		services->setVertexShaderConstant(reinterpret_cast<const f32*>(&col), 9, 1);
		// 对于高级着色语言有其他的方法
		//services->setVertexShaderConstant("myColor", reinterpret_cast<f32*>(&col), 4);
		}
		 
		\param services: 指向提供用于为这个shader设置常量的方法的接口
		\param userData: 当创建这个shader时用于指定的用户数据
		*/
		virtual void OnSetConstants(IMaterialRendererServices* services, SINT32 userData) = 0;
	};
}

#endif