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

	//! 材质渲染的接口
	/** 能够用于为引擎扩展支持新的材质。如何扩展参考IVideoDriver::addMaterialRenderer()
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
		//! 通过IVideoDriver实现调用，让渲染器设置它所需的渲染状态
		/** 它的调用是通过IVideoDriver::setMaterial()来实现。当重载它，你可以设置
		一些渲染状态或者例如顶点或像素着色器的
		\param material: 要设置新的材质参数。这个渲染器会改变这个材质里的材质标志。
		例如：如果这个材质不接受zbuffer=true，它可以设置为flase。 
		\param lastMaterial: 这个材质参数，它是这个材质之前的设置
		\param resetAllRenderstates: 如果为true，所有渲染状态将全部重置. 如上一次渲染
		模式不是一个常用的渲染模式，比如一个2D渲染模式，这经常为true。如果上一次材质有
		些相似的设置，没有关系。这点
		\param services: 对于某些一个IVideoDriver的高级改变和内部状态的一些方法的接口 */
		virtual void OnSetMaterial(const SMaterial& material, const SMaterial& lastMaterial,
			bool resetAllRenderstates, IMaterialRendererServices* services) {}

		//! 在每一次用这个材质绘制一组新几何体之前调用,例如在drawIndexedTriangleList()调用前
		/** OnSetMaterial正常情况下应该只在渲染器发现渲染状态改变的情况下调用。如果例如：不想在
		在相同的材质设置调用两次drawIndexedTriangleList()，就不要调用它。对于使用shader的材质，它
		不止设置新的渲染状态，还有shader常量
		\param service: 指向设置常量和其它东西方法的接口
		\param vtxtype: 下一次渲染要用的顶点类型。拥有材质渲染器设置一些特别的优化shader
		或者如果这是一个与这个渲染器不兼容的顶点类型，拒绝渲染
		\return 如果所有东西都ok，返回true，如果没有能渲染的返回fasle。这个材质渲染器能够对于
		不支持的特殊顶点类型选择返回false。实际上在D3D8和D3D9中，当一个法线映射材质使用了一个
		不是EVT_TANGENTS的顶点类型
		 */
		virtual bool OnRender(IMaterialRendererServices* service, E_VERTEX_TYPE vtxtype) { return true; }

		//! 通过IVideoDriver调用取消这个材质的设置
		/** 通过IVideoDriver::setMaterial()调用，在新材质进行 OnSetMaterial()之前调用
		  */
		virtual void OnUnsetMaterial() {}

		//! 返回这个材质是否透明
		/** 这个场景管理器需要知道如何将透明和不透明的材质进行排序 */
		virtual bool isTransparent() const { return false; }

		//! 返回这个材质的渲染特性
		/** 由于某些更多的复杂材质的实现是由多种方法并且需要特殊的硬件特性。它可能查询当前的材质渲染器
		如何使用这个函数应用在当前硬件上。
		\return 如果所有东西运行的很好，返回0. 指定任何其它值是材质渲染器特有的，并且意思是如：由于不能用于最新的shader。
		渲染器切换回前一个材质。更多例子：固定关系材质大多数情况应该返回0，parallax映射材质在当pixel shader 1.4可用时
		应该只返回0
		 */
		virtual SINT32 getRenderCapability() const { return 0; }
	};
}

#endif 