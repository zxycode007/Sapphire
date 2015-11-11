#ifndef _SAPPHIRE_EDGE_LIST_BUILDER_
#define _SAPPHIRE_EDGE_LIST_BUILDER_
#include "SapphirePrerequisites.h"
#include "SapphireLog.h"

namespace Sapphire
{
	/**
	这个类包含一个描述的给定的顶点集和索引集的边连接度需要的信息
	@remarks
	这些信息用EdgeListBuilder类构建。注意对于一个给定的网格，它由多个子网格构成，
	这时是分离的边表
	*/
	class _SapphireExport EdgeData : public EdgeDataAlloc
	{
		/** 基本的三角形结构 */
		struct Triangle {
			/**
			这个三角形的索引集合（注意：在这个三角形一条边的一侧与另外一侧用不同的顶点缓冲是可能的）
			*/
			size_t indexSet;
			/** 来自于顶点集合的这些顶点 */
			size_t vertexSet;
			size_t vertIndex[3];/// 顶点索引，关系到原缓冲
			size_t sharedVertIndex[3]; /// 顶点索引，关系到一个用消除重复的共享的顶点缓冲(这个缓冲区不是开放的)

			Triangle() :indexSet(0), vertexSet(0) {}
		};

		/** 边数据. */
		struct Edge {
			/**
			这个索引由附加表示方向的两个tri，注意tri0是表示这条边按逆时针方向，tri1正好相反
			*/
			size_t triIndex[2];
			/**
			这条边的索引。注意：这两个顶点在指定的顶点集合'VertexSet'中，
			*/
			size_t vertIndex[2];
			/**
			用于共享边的顶点索引，不对外的
			*/
			size_t sharedVertIndex[2];
			/** 指出如果这是不是一个退化的边，它不能有两个三角形 */
			bool degenerate;
		};

		// 三角形面法线的4d向量数组，它是与这个三角形正交的单位向量， Array of 4D vector of triangle face normal, which is unit vector orthogonal
		// to the triangles, 加上到原点的距离(4d)
		//适用于对齐SIMD优化方式
		typedef std::vector<Vector4, STLAllocator<Vector4, CategorisedAlignAllocPolicy<MEMCATEGORY_GEOMETRY> > > TriangleFaceNormalList;

		// 用于计算轮廓的工作向量
		// 用std::vector<char> 代替std::vector<bool> 它可以被实现类似位域bitfiled会导致丢失性能
		typedef vector<char>::type TriangleLightFacingList;

		//三角形列表
		typedef vector<Triangle>::type TriangleList;
		//边表
		typedef vector<Edge>::type EdgeList;



		/** 一组共享相同顶点数据的边 */
		struct EdgeGroup
		{
			/**这个边组的包含这些顶点的顶点集合的索引 */
			size_t vertexSet;
			/** 用于这个边组的顶点数据指针 */
			//	const VertexData* vertexData;
			/**
			主三角形数组的索引，指示这个边组的第一个三角形，并且这个边组的所有三角形都存放在主三角形数组中
			*/
			size_t triStart;
			/** 边组的三角形数 */
			size_t triCount;
			/** 边表 */
			EdgeList edges;

		};

		typedef vector<EdgeGroup>::type EdgeGroupList;

		/**
		主三角形数组，保存所有的三角形的边表，注意：这些三角形针对边组进行分组
		*/
		//TriangleList triangles;
		/**
		所有的三角形面法线，它与三角形和一一对应*/
		//TriangleFaceNormalList triangleFaceNormals;
		/** 三角形朝向状态。它与三角形和一一对应 */
		//TriangleLightFacingList triangleLightFacings;
		/** 这边边表所有的边组*/
		//EdgeGroupList edgeGroups;
		/** 指示网格是多方面的标准 */
		bool isClosed;


		/**
		在这个边表里计算三角形光源朝向状态
		@remarks
		这是通常是计算轮廓的第一步，
		This is normally the first stage of calculating a silhouette, i.e.
		establishing which tris are facing the light and which are facing
		away. This state is stored in the 'triangleLightFacings'.
		@param lightPos 4D position of the light in object space, note that
		for directional lights (which have no position), the w component
		is 0 and the x/y/z position are the direction.
		*/
		void updateTriangleLightFacing(const Vector4& lightPos);
		/** Updates the face normals for this edge list based on (changed)
		position information, useful for animated objects.
		@param vertexSet The vertex set we are updating
		@param positionBuffer The updated position buffer, must contain ONLY xyz
		*/
		//void updateFaceNormals(size_t vertexSet, const HardwareVertexBufferSharedPtr& positionBuffer);



		// Debugging method
		void log(Log* log);

	};
}


#endif