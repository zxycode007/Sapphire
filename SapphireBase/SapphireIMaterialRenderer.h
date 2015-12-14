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