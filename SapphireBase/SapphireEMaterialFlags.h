#ifndef _SAPPHIRE_EMATERIAL_FLAGS_
#define _SAPPHIRE_EMATERIAL_FLAGS_

namespace Sapphire
{
	//! 材质标志
	enum E_MATERIAL_FLAG
	{
		//! 绘制线框或填充的三角形  默认：false
		EMF_WIREFRAME = 0x1,

		//! 绘制点云或填充的三角形  默认：false 
		EMF_POINTCLOUD = 0x2,

		//! Flat 或 Gouraud 着色? 默认：true
		EMF_GOURAUD_SHADING = 0x4,

		//! 材质进行光照计算  默认：true	
		EMF_LIGHTING = 0x8,

		//! 打开Zbuffer  默认： true
		EMF_ZBUFFER = 0x10,

		//! Zbuffer可写或者只读。  默认：true
		/** 这个标志无效， 如果材质类型是一个透明类型的话 */
		EMF_ZWRITE_ENABLE = 0x20,

		//! 背面剔除  默认：true
		EMF_BACK_FACE_CULLING = 0x40,

		//! 正面剔除? 默认：false
		EMF_FRONT_FACE_CULLING = 0x80,

		//! 双线性滤波  默认： true
		EMF_BILINEAR_FILTER = 0x100,

		//! 三线性滤波   默认： false
		/** 如果三线性滤波开启，那么双线性滤波标志无效 */
		EMF_TRILINEAR_FILTER = 0x200,

		//! 各向异性滤波  默认： false
		EMF_ANISOTROPIC_FILTER = 0x400,

		//! 开启雾化 默认： false
		EMF_FOG_ENABLE = 0x800,

		//! 标准化法线  默认 false
		/** 如果你需要缩放动态光源模式， 你可以打开这个模式
		它的法线可以缩放比较大，他会变暗。如果打开EMF_NORMALIZE_NORMALS标志，这法线会被标准化，
		模型看起来它应该有的亮度
		 */
		EMF_NORMALIZE_NORMALS = 0x1000,

		//! 访问所有层纹理的warp模式。  覆写分离层设置
		EMF_TEXTURE_WRAP = 0x2000,

		//! 抗锯齿模式
		EMF_ANTI_ALIASING = 0x4000,

		//! 颜色掩码位 用于打开颜色平面
		EMF_COLOR_MASK = 0x8000,

		//! 颜色材质ColorMaterial 对于顶点颜色插值的枚举
		EMF_COLOR_MATERIAL = 0x10000,

		//! 打开关闭使用mipmap的标志
		EMF_USE_MIP_MAPS = 0x20000,

		//! 混合操作标志
		EMF_BLEND_OPERATION = 0x40000,

		//! 多边形偏移标志
		EMF_POLYGON_OFFSET = 0x80000
	};
}

#endif