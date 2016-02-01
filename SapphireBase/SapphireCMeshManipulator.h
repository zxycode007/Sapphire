#ifndef _SAPPHIRE_C_MESH_MANIPULATOR_
#define _SAPPHIRE_C_MESH_MANIPULATOR_

#include "SapphireIMeshManipulator.h"

namespace Sapphire
{
	//! 一个简单操作网格的接口
	/** 缩放，设置alpha值，翻转表面，等等。这些对于快速修正导入或输出网格的问题和错误.
	它不打算在运行时修改网格或动画
	*/
	class CMeshManipulator : public IMeshManipulator
	{
	public:
		//! 翻转表面的方向
		/** 改变背面三角形到正面朝向三角形并且反之亦然。
		\param mesh 要应用操作的网格 */
		virtual void flipSurfaces(IMesh* mesh) const;

		//! 重新计算网格的所有的法线
		/** \param mesh: 要应用操作的网格
		\param smooth: 法线是否需要平滑
		\param angleWeighted: 如果法线在它们相关的角度需要平滑。需更昂贵开销，更高的精度 */
		virtual void recalculateNormals(IMesh* mesh, bool smooth = false, bool angleWeighted = false) const;

		//! 重新计算网格缓冲区的所有的法线
		/** \param buffer: 要应用操作的网格缓冲区
		\param smooth: 法线是否需要平滑
		\param angleWeighted: 如果法线在它们相关的角度需要平滑。需更昂贵开销，更高的精度 */
		virtual void recalculateNormals(IMeshBuffer* buffer, bool smooth = false, bool angleWeighted = false) const;

		//! 克隆一个静态IMesh网格到一个可修改的SMesh
		/** 在所有返回的网格缓冲区，是SMeshBuffer或SMeshBufferLightMap类型的SMesh
		\param mesh 要复制的网格
		\return 克隆的网格  */
		virtual SMesh* createMeshCopy(IMesh* mesh) const;


		//! 在这个网格上创建一个planar纹理映射
		/** \param mesh:  要应用操作的网格
		\param resolution: plannar映射的分辨率。这是个特别的值，它与世界空间和纹理坐标空间相关
		*/
		virtual void makePlanarTextureMapping(IMesh* mesh, FLOAT32 resolution = 0.001f) const;


		//! 在这个网格缓冲区上创建一个planar纹理映射
		/** \param meshbuffer: 要应用操作的网格缓冲区
		\param resolution: plannar映射的分辨率。这是个特别的值，它与世界空间和纹理坐标空间相关 */
		virtual void makePlanarTextureMapping(IMeshBuffer* meshbuffer, FLOAT32 resolution = 0.001f) const;

		//! 在这个缓冲创建一个plannar纹理映射
		/** 这个方法是对LWO plannar 映射的当前实现。需要一个更加通用的偏移值
		\param mesh 要应用操作的网格缓冲区
		\param resolutionS 在水平方向planar映射的分辨率。 这是个在对象空间和纹理空间之间的比例。
		\param resolutionT 在垂直方向planar映射的分辨率。 这是个在对象空间和纹理空间之间的比例。
		\param axis 纹理沿着投影的轴。这允许值是0(x),1(y),和2(z)
		\param offset 添加到这个顶点位置的向量（在对象空间）
		*/
		void makePlanarTextureMapping(IMeshBuffer* buffer, FLOAT32 resolutionS, FLOAT32 resolutionT, UINT32 axis, const Vector3& offset) const;

		//! 在这个网格缓冲创建一个plannar纹理映射
		/** 这个方法是对LWO plannar 映射的当前实现。需要一个更加通用的偏移值
		\param mesh 要应用操作的网格缓冲区
		\param resolutionS 在水平方向planar映射的分辨率。 这是个在对象空间和纹理空间之间的比例。
		\param resolutionT 在垂直方向planar映射的分辨率。 这是个在对象空间和纹理空间之间的比例。
		\param axis 纹理沿着投影的轴。这允许值是0(x),1(y),和2(z)
		\param offset 添加到这个顶点位置的向量（在对象空间）
		*/
		void makePlanarTextureMapping(IMesh* mesh, FLOAT32 resolutionS, FLOAT32 resolutionT, UINT32 axis, const Vector3& offset) const;

		//! 重新计算切线，需要一个切线网格缓冲区
		/** \param buffer 要应用操作的网格缓冲区
		\param recalculateNormals 法线是否需要重新计算，否则网格的原法线使用未改变的。
		\param smooth 法线是否需要平滑
		\param angleWeighted 如果法线在它们相关的角度需要平滑。需更昂贵开销，更高的精度
		*/
		virtual void recalculateTangents(IMeshBuffer* buffer, bool recalculateNormals = false, bool smooth = false, bool angleWeighted = false) const;


		//! 重新计算切线，需要一个切线网格
		/** \param mesh要应用操作的网格
		\param recalculateNormals 法线是否需要重新计算，否则网格的原法线使用未改变的。
		\param smooth 法线是否需要平滑
		\param angleWeighted 如果法线在它们相关的角度需要平滑。需更昂贵开销，更高的精度
		*/
		virtual void recalculateTangents(IMesh* mesh, bool recalculateNormals = false, bool smooth = false, bool angleWeighted = false) const;

		//! 创建一个复制的网格，它只有S3DVertexTangents顶点组成
		/** 如果你想要绘制一个切线空间法线映射的几何体，它是有用的。因为它计算
		它这里需要的切线和副法线数据。
		\param mesh 输入的网格
		\param recalculateNormals 如果设置，法线会被重新计算。否则保存原来的。
		注意：如果这个法线非常不同于那些面法线的计算，保存法线可能引入不精确的切线。
		\param smooth 经过这个网格缓冲区的面法线/切线是否需要平滑
		\param angleWeighted 用于改善平滑计算
		\param recalculateTangents 是否需要实际的计算，或是就采用创建的合适类型
		\return 只由S3DVertexTangents顶点组成的网格。  */
		virtual IMesh* createMeshWithTangents(IMesh* mesh, bool recalculateNormals = false, bool smooth = false, bool angleWeighted = false, bool recalculateTangents = true) const;

		//! 创建一个复制的网格，它只有S3DVertex2TCoord顶点组成
		/** \param mesh 输入的网格
		\return 只由S3DVertex2TCoord顶点组成的网格。 */
		virtual IMesh* createMeshWith2TCoords(IMesh* mesh) const;

		//! 创建一个复制的网格，它只有S3DVertex顶点组成
		/** \param mesh 输入的网格
		\return 只由S3DVertex顶点组成的网格。*/
		virtual IMesh* createMeshWith1TCoords(IMesh* mesh) const;

		//! 创建一个使用所有未连接顶点的复制网格
		/** \param mesh 输入的网格
		\return 只由未知面组成的网格。所有顶点都与之前的重复共享。*/
		virtual IMesh* createMeshUniquePrimitives(IMesh* mesh) const;

		//! 创建一个使用所有连接顶点的复制网格 
		/** \param mesh 输入的网格
		\param tolerance 顶点比较的阀值
		\return 不使用冗余顶点列表的网格。*/
		virtual IMesh* createMeshWelded(IMesh *mesh, FLOAT32 tolerance = std::numeric_limits<FLOAT32>::epsilon()) const;

		//! 获取这个网格多边形数量
		/** \param mesh 输入的网格
		\return 这个网格的多边形数量. */
		virtual SINT32 getPolyCount(IMesh* mesh) const;

		//! 获取这个网格多边形数量
		/** \param mesh 输入的网格
		\return这个网格的多边形数量. */
		virtual SINT32 getPolyCount(IAnimatedMesh* mesh) const;

		//! 创建一个新动画网格并且添加这个网格
		/** \param mesh 输入网格
		\param type 这个要创建的动画网格的类型
		\return 新创建的动画网格 */
		virtual IAnimatedMesh * createAnimatedMesh(IMesh* mesh, E_ANIMATED_MESH_TYPE type) const;

		//!通过Forsyth paper顶点缓冲优化 
		/**
		http://home.comcast.net/~tom_forsyth/papers/fast_vert_cache_opt.html

		这个函数是线程安全 (你可以在不同的线程优化几个网格).

		\param mesh 操作的原网格
		\return 对于这个顶点缓冲的优化后的新网格 */
		virtual IMesh* createForsythOptimizedMesh(const IMesh *mesh) const;
	};
}


#endif