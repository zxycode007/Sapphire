#ifndef _SAPPHIRE_I_MATERIAL_RENDERER_SERVERES_
#define _SAPPHIRE_I_MATERIAL_RENDERER_SERVERES_

#include "SapphireSMaterial.h"
#include "SapphireSVertex.h"	

namespace Sapphire
{
	class IVideoDriver;

	//! Ϊ�ı�һ��IVideoDriver�ڲ�״̬���ṩ�ڲ��ӿ�
	class IMaterialRendererServices
	{
	public:

		 
		virtual ~IMaterialRendererServices() {}

		//! ��һ��IMaterialRendererʹ����������
		/** �����Ҫ�������еĻ�����Ⱦ״̬
		������Ⱦ״̬��������diffuse, ������ambient, �߹�specular, �����emissive��ɫ
		�߹�ǿ�ȣ� ˫���Ժ��������˲����߿�ģʽ���������ɫ�����գ�z���壬 zwrite�򿪣�
		�����޳���������
		\param material Ҫʹ�õ��²���
		\param lastMaterial �ɲ���
		\param resetAllRenderstates ���������Ⱦ״̬��Ҫ�����ã���Ϊtrue,���ӵ�ǰ���ǵ���Ⱦ״̬
		 */
		virtual void setBasicRenderStates(const SMaterial& material,
			const SMaterial& lastMaterial,
			bool resetAllRenderstates) = 0;

		//! ͨ��һ��������ΪVertex Shader����һ������
		/** �����������GLSL��HLSL������һ��shader�����磺����㴴��һ��shader�������б�����
		'mWorldViewProj'������WorldViewProjection���󣩺���һ������һ����������'fTime' �ı�����
		�����ͨ����IShaderConstantSetCallBack����������������������:
		 
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
		\param name ��������
		\param floats ��������ָ��
		\param count ��������Ԫ�ظ���
		\return ����ɹ�����true
		*/
		virtual bool setVertexShaderConstant(const c8* name, const FLOAT32* floats, int count) = 0;

		//! ����Bool�����Ľӿ�
		virtual bool setVertexShaderConstant(const c8* name, const bool* bools, int count) = 0;

		//! ����Int�����Ľӿ�
		virtual bool setVertexShaderConstant(const c8* name, const SINT32* ints, int count) = 0;

		//! ����һ��Vertex Shader����
		/** ��������pixel/vertex shader assembler ��ARB_fragment_program ��ARB_vertex_program����һ��shader������ʹ�� 
		\param data: Ҫ���õĳ�������
		\param startRegister: Ҫ���õĵ�һ���Ĵ���
		\param constantAmount: Ҫ���õļĴ���������һ���Ĵ�����4��float���*/
		virtual void setVertexShaderConstant(const FLOAT32* data, SINT32 startRegister, SINT32 constantAmount = 1) = 0;

		//! ����һ������������һ��pixel shader�ĳ���
		/** �����Ҫʹ��HLSL��GLSL������һ��shader����setVertexShaderConstant()ʹ�õ�����
		\param name ������
		\param floats ��������ָ��
		\param count ��������Ԫ������
		\return ����ɹ�����true */
		virtual bool setPixelShaderConstant(const c8* name, const FLOAT32* floats, int count) = 0;

		//!  ����Bool�����Ľӿ�
		virtual bool setPixelShaderConstant(const c8* name, const bool* bools, int count) = 0;

		//! ����Int�����Ľӿ�
		virtual bool setPixelShaderConstant(const c8* name, const SINT32* ints, int count) = 0;

		//! ����һ��Pixel Shader ����
		/** ��������pixel/vertex shader assembler ��ARB_fragment_program ��ARB_vertex_program����һ��shader������ʹ��
		\param data: Ҫ���õĳ�������
		\param startRegister: Ҫ���õĵ�һ���Ĵ���
		\param constantAmount: Ҫ���õļĴ���������һ���Ĵ�����4��float���*/
		virtual void setPixelShaderConstant(const FLOAT32* data, SINT32 startRegister, SINT32 constantAmount = 1) = 0;

		//! ��ȡһ��IVideoDriver �ӿڵ�ָ��
		/** \return ָ��IVideoDriver�Ľӿ�ָ�� */
		virtual IVideoDriver* getVideoDriver() = 0;
	};

}

#endif