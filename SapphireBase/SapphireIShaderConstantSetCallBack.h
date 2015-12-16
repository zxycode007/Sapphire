#ifndef _SAPPHIRE_ISHADER_CONSTANT_SET_CALLBACK_
#define _SAPPHIRE_ISHADER_CONSTANT_SET_CALLBACK_

#include "SapphirePrerequisites.h"
#include "SapphireIReferenceCounter.h"

namespace Sapphire
{
	class IMaterialRendererServices;
	class SMaterial;

	//! ����ӿ�ʹGPU������ÿһ֡���ó�����Ϊ����
	/** ���Լ�����ʵ������ӿڲ��Ҳش���һ��shaderʱ����һ��ָ�뵽IGPUProgrammingServices��һ��������
	OnSetConstants��������ÿһ֡����
	*/
	class IShaderConstantSetCallBack : public virtual IReferenceCounter
	{
	public:

		//! �����ûص�ָ���õ��ĸ����ʣ���ѡ������
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

		//! �����ڲ�����Ⱦ��Ӧ��Ҫ���ö���/���� ��ɫ��������ͨ���������
		/**
		��һ���Լ�������ʵ��IShaderConstantSetCallBack������ʵ���Լ���OnSetConstants���������ڵ�����shaderʱ
		��IMaterialRendererServices����һ��������ָ�룬�ٵ�IGPUProgrammingServices��һ��������
		OnSetConstants������ÿ�ζ��������shader���ʻ��Ƽ�����֮ǰ���á�һ����ʵ�����£�
		 
		virtual void OnSetConstants(video::IMaterialRendererServices* services, s32 userData)
		{
		video::IVideoDriver* driver = services->getVideoDriver();

		// �ڼĴ���4���ü��þ���
		core::matrix4 worldViewProj(driver->getTransform(video::ETS_PROJECTION));
		worldViewProj *= driver->getTransform(video::ETS_VIEW);
		worldViewProj *= driver->getTransform(video::ETS_WORLD);
		services->setVertexShaderConstant(&worldViewProj.M[0], 4, 4);
		// ���ڸ߼�shader���ԣ��⽫�������������
		//services->setVertexShaderConstant("mWorldViewProj", worldViewProj.M, 16);

		// �ڼĴ���9����һЩ������ɫ
		video::SColorf col(0.0f,1.0f,1.0f,0.0f);
		services->setVertexShaderConstant(reinterpret_cast<const f32*>(&col), 9, 1);
		// ���ڸ߼���ɫ�����������ķ���
		//services->setVertexShaderConstant("myColor", reinterpret_cast<f32*>(&col), 4);
		}
		 
		\param services: ָ���ṩ����Ϊ���shader���ó����ķ����Ľӿ�
		\param userData: ���������shaderʱ����ָ�����û�����
		*/
		virtual void OnSetConstants(IMaterialRendererServices* services, SINT32 userData) = 0;
	};
}

#endif