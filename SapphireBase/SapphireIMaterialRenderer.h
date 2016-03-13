#ifndef _SAPPHIRE_IMATERIAL_RENDERER_
#define _SAPPHIRE_IMATERIAL_RENDERER_

#include "SapphirePrerequisites.h"
#include "SapphireIReferenceCounter.h"
#include "SapphireSMaterial.h"
#include "SapphireSVertex.h"

namespace Sapphire
{
	class IVideoDriver;
	class IMaterialRendererServices;

	//! ������Ⱦ�Ľӿ�
	/** �ܹ�����Ϊ������չ֧���µĲ��ʡ������չ�ο�IVideoDriver::addMaterialRenderer()
	*/
	class IMaterialRenderer : public virtual IReferenceCounter
	{
	public:

		// Called by the IVideoDriver implementation the let the renderer set its needed render states.
		/** This is called during the IVideoDriver::setMaterial() call.
		When overriding this, you can set some renderstates or for example a
		vertex or pixel shader if you like.
		\param material: The new material parameters to be set. The renderer
		may change the material flags in this material. For example if this
		material does not accept the zbuffer = true, it can set it to false.
		This is useful, because in the next lastMaterial will be just the
		material in this call.
		\param lastMaterial: The material parameters which have been set before
		this material.
		\param resetAllRenderstates: True if all renderstates should really be
		reset. This is usually true if the last rendering mode was not a usual
		3d rendering mode, but for example a 2d rendering mode.
		You should reset really all renderstates if this is true, no matter if
		the lastMaterial had some similar settings. This is used because in
		most cases, some common renderstates are not changed if they are
		already there, for example bilinear filtering, wireframe,
		gouraudshading, lighting, zbuffer, zwriteenable, backfaceculling and
		fogenable.
		\param services: Interface providing some methods for changing
		advanced, internal states of a IVideoDriver. */
		//! ͨ��IVideoDriverʵ�ֵ��ã�����Ⱦ���������������Ⱦ״̬
		/** ���ĵ�����ͨ��IVideoDriver::setMaterial()��ʵ�֡��������������������
		һЩ��Ⱦ״̬�������綥���������ɫ����
		\param material: Ҫ�����µĲ��ʲ����������Ⱦ����ı����������Ĳ��ʱ�־��
		���磺���������ʲ�����zbuffer=true������������Ϊflase�� 
		\param lastMaterial: ������ʲ����������������֮ǰ������
		\param resetAllRenderstates: ���Ϊtrue��������Ⱦ״̬��ȫ������. ����һ����Ⱦ
		ģʽ����һ�����õ���Ⱦģʽ������һ��2D��Ⱦģʽ���⾭��Ϊtrue�������һ�β�����
		Щ���Ƶ����ã�û�й�ϵ�����
		\param services: ����ĳЩһ��IVideoDriver�ĸ߼��ı���ڲ�״̬��һЩ�����Ľӿ� */
		virtual void OnSetMaterial(const SMaterial& material, const SMaterial& lastMaterial,
			bool resetAllRenderstates, IMaterialRendererServices* services) {}

		//! ��ÿһ����������ʻ���һ���¼�����֮ǰ����,������drawIndexedTriangleList()����ǰ
		/** OnSetMaterial���������Ӧ��ֻ����Ⱦ��������Ⱦ״̬�ı������µ��á�������磺������
		����ͬ�Ĳ������õ�������drawIndexedTriangleList()���Ͳ�Ҫ������������ʹ��shader�Ĳ��ʣ���
		��ֹ�����µ���Ⱦ״̬������shader����
		\param service: ָ�����ó������������������Ľӿ�
		\param vtxtype: ��һ����ȾҪ�õĶ������͡�ӵ�в�����Ⱦ������һЩ�ر���Ż�shader
		�����������һ���������Ⱦ�������ݵĶ������ͣ��ܾ���Ⱦ
		\return ������ж�����ok������true�����û������Ⱦ�ķ���fasle�����������Ⱦ���ܹ�����
		��֧�ֵ����ⶥ������ѡ�񷵻�false��ʵ������D3D8��D3D9�У���һ������ӳ�����ʹ����һ��
		����EVT_TANGENTS�Ķ�������
		 */
		virtual bool OnRender(IMaterialRendererServices* service, E_VERTEX_TYPE vtxtype) { return true; }

		//! ͨ��IVideoDriver����ȡ��������ʵ�����
		/** ͨ��IVideoDriver::setMaterial()���ã����²��ʽ��� OnSetMaterial()֮ǰ����
		  */
		virtual void OnUnsetMaterial() {}

		//! ������������Ƿ�͸��
		/** ���������������Ҫ֪����ν�͸���Ͳ�͸���Ĳ��ʽ������� */
		virtual bool isTransparent() const { return false; }

		//! ����������ʵ���Ⱦ����
		/** ����ĳЩ����ĸ��Ӳ��ʵ�ʵ�����ɶ��ַ���������Ҫ�����Ӳ�����ԡ������ܲ�ѯ��ǰ�Ĳ�����Ⱦ��
		���ʹ���������Ӧ���ڵ�ǰӲ���ϡ�
		\return ������ж������еĺܺã�����0. ָ���κ�����ֵ�ǲ�����Ⱦ�����еģ�������˼���磺���ڲ����������µ�shader��
		��Ⱦ���л���ǰһ�����ʡ��������ӣ��̶���ϵ���ʴ�������Ӧ�÷���0��parallaxӳ������ڵ�pixel shader 1.4����ʱ
		Ӧ��ֻ����0
		 */
		virtual SINT32 getRenderCapability() const { return 0; }
	};
}

#endif 