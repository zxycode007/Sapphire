#ifndef _SAPPHIRE_ITRIANGLE_SELECTOR_
#define _SAPPHIRE_ITRIANGLE_SELECTOR_

#include "SapphirePrerequisites.h"
#include "SapphireIReferenceCounter.h"
#include "SapphireTriangle3D.h"
#include "SapphireAxisAlignedBox.h"
#include "SapphireLine3D.h"


namespace Sapphire
{
	class ISceneNode;

	//! 返回有特别属性的三角形的接口
	/** 每个ISceneNode都可以有一个三角形选择器，可以用ISceneNode::getTriangleScelector()或ISceneManager::createTriangleSelector.
	获得。这用于碰撞检测：例如你如果指定，有一个碰撞可能发生在区域（1，1，1）和（10，10，10），你可以通过ITriangleSelector获取容易
	的场景节点在这个区域内的所有三角形，并且检测这些三角形是否每一个都发生碰撞
	 */
	class ITriangleSelector : public SceneAlloc, public virtual IReferenceCounter
	{
		//! 获取这个selector所有可用的三角形数量
		virtual SINT32 getTriangleCount() const = 0;

		//! 获取一个关联节点的三角形
		/**
		它返回一个场景节点与这个selector有联系的所有三角形. 
		如果有多于一个场景节点有关系（例如IMetaTriangleSelector）
		这个，该函数会调用多次返回所有多边形
		\param triangles 计算结果的三角形数组
		\param arraySize 数组大小
		\param outTriangleCount: 写入这个数组的三角形数量
		\param transform 在它们返回之前变换这些三角形的矩阵指针
	    例如用于缩放所有三角形到一个椭圆形空间中。如果这个指针是NULL，没有变换*/
		virtual void getTriangles(triangle3d<Real>* triangles, SINT32 arraySize,
			SINT32& outTriangleCount, const Matrix4* transform = 0) const = 0;

		//! 获取一个与指定一个AABB盒子的节点联系的所有三角形
		/**
		它返回一个场景节点与这个selector有联系的所有三角形. 
		如果有多于一个场景节点有关系（例如IMetaTriangleSelector）
		这个，该函数会调用多次返回所有多边形.
		这个方法将最少返回与盒子相交的三角形，但是也可能返回其它的三角形
		
		\param triangles 计算结果的三角形数组
		\param arraySize 数组大小
		\param outTriangle 写入这个数组的三角形数量
		\param box 只包括在AABB盒子里的三角形写入数组
		\param transform 在它们返回之前变换这些三角形的矩阵指针
	    例如用于缩放所有三角形到一个椭圆形空间中。如果这个指针是NULL，没有变换 */
		virtual void getTriangles(triangle3d<Real>* triangles, SINT32 arraySize,
			SINT32& outTriangleCount, const AxisAlignedBox& box,
			const Matrix4* transform = 0) const = 0;

		//! 获取一个与指定与一个3d线段的有关联的节点的所有三角形 
		/**
		它返回一个场景节点与这个selector有联系的所有三角形. 
		如果有多于一个场景节点有关系（例如IMetaTriangleSelector）
		这个，该函数会调用多次返回所有多边形.
		请注意：这个方法将最少返回与盒子相交的三角形，但是也可能返回其它的三角形

		请注意：没优化的三角形选择器也将返回这些与3d线段没有联系的三角形
		\param triangles 计算结果的三角形数组
		\param arraySize 数组大小
		\param outTriangle 写入这个数组的三角形数量
		\param line 只有与这个3d线段有联系的三角形会写入到这个数组
		\param transform 在它们返回之前变换这些三角形的矩阵指针
	    例如用于缩放所有三角形到一个椭圆形空间中。如果这个指针是NULL，没有变换 */
		virtual void getTriangles(triangle3d<Real>* triangles, SINT32 arraySize,
			SINT32& outTriangleCount, const line3d<Real>& line,
			const Matrix4* transform = 0) const = 0;

		//! 获取与一个给定三角形相关的场景节点
		/**
		这允许找到哪一个场景节点（可能有几个）与一个指定的三角形相关 

		\param triangleIndex: 要想找到与这个场景节点相关的三角形索引 
		\return 和指定三角形相关的场景节点
		*/
		virtual ISceneNode* getSceneNodeForTriangle(UINT32 triangleIndex) const = 0;

		//! 获取三角形选择器的数量
		/** 只用于MetaTriangleSelector, 其它返回1
		*/
		virtual UINT32 getSelectorCount() const = 0;

		//! 获取基于索引和基于getSelectorCount的三角形选择器
		/** 只用于MetaTriangleSelector,其它的返回'this' 或 0
		*/
		virtual ITriangleSelector* getSelector(UINT32 index) = 0;

		//! 获取基于索引和基于getSelectorCount的三角形选择器（常）
		/** 只用于MetaTriangleSelector,其它的返回'this' 或 0
		*/
		virtual const ITriangleSelector* getSelector(UINT32 index) const = 0;
	};
}



#endif
