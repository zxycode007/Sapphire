#ifndef _SAPPHIRE_EMATERIAL_TYPES_
#define _SAPPHIRE_EMATERIAL_TYPES_

namespace Sapphire
{

	//! 抽象的并且容易使用固定/可编程管线 材质方式
	enum E_MATERIAL_TYPE
	{
		//! 标准固体材质
		/** 只有使用第一个纹理， 它假定为漫反射材质*/
		EMT_SOLID = 0,

		//! 有两个纹理层的固体材质
		/** 第二个会用顶点颜色的alpha值混合到第一个上面。这个材质在OpenGL中不能被支持
		*/
		EMT_SOLID_2_LAYER,

		//! 标准光照贴图的材质类型
		/** 这有两个纹理：第一个纹理层是一个漫反射纹理，第二个是一个光照图。 动态光源无效 */
		EMT_LIGHTMAP,

		//! 类似EMT_LIGHTMAP的光照贴图的材质技术
		/** 但是光照图和漫反射纹理是以add代替modulated*/
		EMT_LIGHTMAP_ADD,

		//! 标准光照图的材质类型
		/** 包括两个纹理：第一个是一个漫反射图， 第二个是一个光照图，动态光源无效。
		这个纹理颜色亮度高效乘2倍。就像DirectX as D3DTOP_MODULATE2X.
		 */
		EMT_LIGHTMAP_M2,

		//! 标准光照图的材质类型
		/** 包括两个纹理：第一个是一个漫反射图， 第二个是一个光照图，动态光源无效。
		这个纹理颜色亮度高效乘4倍。就像DirectX as D3DTOP_MODULATE4X. */
		EMT_LIGHTMAP_M4,

		//! 类似EMT_LIGHTMAP, 但支持动态光源
		EMT_LIGHTMAP_LIGHTING,

		//! 类似EMT_LIGHTMAP_M2, 但支持动态光源
		EMT_LIGHTMAP_LIGHTING_M2,

		//! 类似EMT_LIGHTMAP_M4, 但支持动态光源
		EMT_LIGHTMAP_LIGHTING_M4,

		//! 细节映射材质
		/** 第一个纹理是漫反射颜色贴图， 第二个添加的并且通常显示一个更大的缩放值让它显示更多细节。
		这个细节贴图添加在漫反射贴图上用ADD_SIGNED。 所以，它可能添加并且从漫反射贴图减去颜色。例如：
		一个值（127，127，127）不会改变漫反射贴图的所有外观，经常用于地表渲染
		 */
		EMT_DETAIL_MAP,

		//! 看起来它像是反射周围环境到它身上
		/** 要使它成为可能， 要用到一个叫'球体贴图'的纹理，必须设置它为第一个纹理
		  */
		EMT_SPHERE_MAP,

		//! 一个用可选的非反射纹理层的反射材质
		/** 这个反射贴图应该用做第一个纹理 */
		EMT_REFLECTION_2_LAYER,

		//! 一个透明材质
		/** 只能作为第一个纹理使用。 这个新颜色通过简单源颜色加上目的颜色计算。
		这意味着如果一个billboard用一个带黑色背景和红色圆环的纹理，在用这个材质绘制的结果是
		只有红色圆环被绘制，并且所有黑色的东西都是100%透明和不可见。这个材质类型对于粒子效果非常有用
		  */
		EMT_TRANSPARENT_ADD_COLOR,

		//! 使材质透明基于纹理的alpha通道
		/** 最终颜色由目标颜色和纹理颜色混合而来， 用alpha通道值作为混合因子。只使用第一个纹理。
		如果你用这个材质用小纹理，它在32位模式加载纹理是个好主意
		( IVideoDriver::setTextureCreationFlag()). 此外，一个alpha引用被使用， 它能够使用SMaterial::MaterialTypeParam操作.
		当这个纹理由透明变换到一个固体时，这个值控制边缘的锐利度
		  */
		EMT_TRANSPARENT_ALPHA_CHANNEL,

		//! 使材质透明基于纹理的alpha通道
		/** 如果alpha通道值大于127，一个像素写到目标， 否则不写入。 
		这个材质不用alpha混合并且大大快于EMT_TRANSPARENT_ALPHA_CHANNEL
		 它的想法是绘制物体像植物的叶子， 因此边界不模糊，而很锐利。
		 只使用第一个纹理，如果你用这个材质使用小纹理和3d对象，它在32位模式加载纹理是个好主意
	    (IVideoDriver::setTextureCreationFlag()). */
		EMT_TRANSPARENT_ALPHA_CHANNEL_REF,

		//! 基于顶点的alpha值使材质透明
		EMT_TRANSPARENT_VERTEX_ALPHA,

		//! 一个带透明反射材质，可选附加非反射纹理层
		/** 
		这个反射贴图应该设置为第一个纹理。这个透明度依赖于alpha值顶点颜色中的。
		反射纹理只能设置为第一个纹理。请注意这个材质类型当前不能100%兼容OpenGL
		  */
		EMT_TRANSPARENT_REFLECTION_2_LAYER,

		//! 一个固体法线贴图渲染器
		/** 第一个纹理是颜色贴图，第二个是法线贴图。
		注意：你应该只在绘制S3DVertexTangents (EVT_TANGENTS)类型顶点构成的几何体中使用这材质
		你能用IMeshManipulator::createMeshWithTangents()转换任何网格到这个格式。见irr SpecialFX2教程
		 这个shader只支持两个光源，如果有更多的，只取最近的两个
		 */
		EMT_NORMAL_MAP_SOLID,

		//! 一个透明法线贴图渲染器
		/** 第一个纹理是颜色贴图，第二个是法线贴图。
		注意：你应该只在绘制S3DVertexTangents (EVT_TANGENTS)类型顶点构成的几何体中使用这材质。
		你能用IMeshManipulator::createMeshWithTangents()转换任何网格到这个格式。见irr SpecialFX2教程
		 这个shader只支持两个光源，如果有更多的，只取最近的两个 */
		EMT_NORMAL_MAP_TRANSPARENT_ADD_COLOR,

		//! 一个透明法线贴图渲染器（基于顶点alpha值） 
		/** 第一个纹理是颜色贴图，第二个是法线贴图。
		注意：你应该只在绘制S3DVertexTangents (EVT_TANGENTS)类型顶点构成的几何体中使用这材质。
		你能用IMeshManipulator::createMeshWithTangents()转换任何网格到这个格式。见irr SpecialFX2教程
		这个shader只支持两个光源，如果有更多的，只取最近的两个  */
		EMT_NORMAL_MAP_TRANSPARENT_VERTEX_ALPHA,

		//! 固体视察贴图 就像是EMT_NORMAL_MAP_SOLID, 但是使用视察映射。
		/** 看起来更加真实 第一个纹理是颜色贴图，第二个是法线贴图。法线贴图纹理应该包含在alpha分量中的高度值。
		当一个32为纹理从一个高度图中创建法线贴图时，IVideoDriver::makeNormalMapTexture()方法自动写入这个值。
		这个材质的高度缩放值(影响凹凸度)是通过SMaterial::MaterialTypeParam成员来控制的。如果设置为0，则使用默认值0.02、
		否则这个值 SMaterial::MaterialTypeParam中设置。这个值依赖在这个材质它所缩放的纹理。太高或太低的MaterialTypeParam
		会导致出现奇怪的问题
		 */
		EMT_PARALLAX_MAP_SOLID,

		//! 一个像EMT_PARALLAX_MAP_SOLID的材质, 但是透明
		/** 使用EMT_TRANSPARENT_ADD_COLOR作为基础材质. */
		EMT_PARALLAX_MAP_TRANSPARENT_ADD_COLOR,

		//! 一个像EMT_PARALLAX_MAP_SOLID的材质, 但是透明
		/** 使用EMT_TRANSPARENT_VERTEX_ALPHA作为基础材质. */
		EMT_PARALLAX_MAP_TRANSPARENT_VERTEX_ALPHA,

		//! 混合函数 = source * sourceFactor + dest * destFactor ( E_BLEND_FUNC )
		/** 只能用于第一个纹理，通用的混合方法 */
		EMT_ONETEXTURE_BLEND,

		//! 这个值不使用。 它强制枚举类型编译为32位
		EMT_FORCE_32BIT = 0x7fffffff
	};

	//! 对应的材质类型名
	const char* const sBuiltInMaterialTypeNames[] =
	{
		"solid",
		"solid_2layer",
		"lightmap",
		"lightmap_add",
		"lightmap_m2",
		"lightmap_m4",
		"lightmap_light",
		"lightmap_light_m2",
		"lightmap_light_m4",
		"detail_map",
		"sphere_map",
		"reflection_2layer",
		"trans_add",
		"trans_alphach",
		"trans_alphach_ref",
		"trans_vertex_alpha",
		"trans_reflection_2layer",
		"normalmap_solid",
		"normalmap_trans_add",
		"normalmap_trans_vertexalpha",
		"parallaxmap_solid",
		"parallaxmap_trans_add",
		"parallaxmap_trans_vertexalpha",
		"onetexture_blend",
		0
	};

}

#endif