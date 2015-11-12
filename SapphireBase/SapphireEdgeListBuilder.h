#ifndef _SAPPHIRE_EDGE_LIST_BUILDER_
#define _SAPPHIRE_EDGE_LIST_BUILDER_
#include "SapphirePrerequisites.h"
#include "SapphireLog.h"
#include "SapphireVector3.h"

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

		// 三角形面法线的4d向量数组，它是与这个三角形正交的单位向量
		// 加上到原点的距离(4d)
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
		TriangleList triangles;
		/**
		所有的三角形面法线，它与三角形和一一对应*/
		TriangleFaceNormalList triangleFaceNormals;
		/** 三角形朝向状态。它与三角形和一一对应 */
		TriangleLightFacingList triangleLightFacings;
		/** 这边边表所有的边组*/
		EdgeGroupList edgeGroups;
		/** 指示网格是多方面的标准 */
		bool isClosed;


		/**
		在这个边表里计算三角形光源朝向状态
		@remarks
		这是通常是计算轮廓的第一步，建立这个三角形是面向光源或是背离光源。
		这个状态在'triangleLightFacings'
		@param lightPos 是光源在对象空间中的4D位置， 注意：对于之平行光（没有位置的），其w分量是0和xyz位置是其方向
		*/
		void updateTriangleLightFacing(const Vector4& lightPos);

		/**
		更新这个边表基于位置信息的面法线，用于动画对象
		@param vertexSet 我们更新的顶点集合
		@param positionBuffer 更新位置缓冲区，必须只包含XYZ
		*/
		//void updateFaceNormals(size_t vertexSet, const HardwareVertexBufferSharedPtr& positionBuffer);



		// DEBUG的方法
		void log(Log* log);

	};

	/** General utility class for building edge lists for geometry.
	@remarks
	You can add multiple sets of vertex and index data to build and edge list.
	Edges will be built between the various sets as well as within sets; this allows
	you to use a model which is built from multiple SubMeshes each using
	separate index and (optionally) vertex data and still get the same connectivity
	information. It's important to note that the indexes for the edge will be constrained
	to a single vertex buffer though (this is required in order to render the edge).
	*/
	class _SapphireExport EdgeListBuilder
	{
	public:

		EdgeListBuilder();
		virtual ~EdgeListBuilder();
		/** Add a set of vertex geometry data to the edge builder.
		@remarks
		You must add at least one set of vertex data to the builder before invoking the
		build method.
		*/
		void addVertexData(const VertexData* vertexData);
		/** Add a set of index geometry data to the edge builder.
		@remarks
		You must add at least one set of index data to the builder before invoking the
		build method.
		@param indexData The index information which describes the triangles.
		@param vertexSet The vertex data set this index data refers to; you only need to alter this
		if you have added multiple sets of vertices
		@param opType The operation type used to render these indexes. Only triangle types
		are supported (no point or line types)
		*/
		void addIndexData(const IndexData* indexData, size_t vertexSet = 0,
			RenderOperation::OperationType opType = RenderOperation::OT_TRIANGLE_LIST);

		/** Builds the edge information based on the information built up so far.
		@remarks
		The caller takes responsibility for deleting the returned structure.
		*/
		EdgeData* build(void);

		/// Debugging method
		void log(Log* l);
	protected:

		/** A vertex can actually represent several vertices in the final model, because
		vertices along texture seams etc will have been duplicated. In order to properly
		evaluate the surface properties, a single common vertex is used for these duplicates,
		and the faces hold the detail of the duplicated vertices.
		*/
		struct CommonVertex {
			Vector3  position;  // location of point in euclidean space
			size_t index;       // place of vertex in common vertex list
			size_t vertexSet;   // The vertex set this came from
			size_t indexSet;    // The index set this was referenced (first) from
			size_t originalIndex; // place of vertex in original vertex set
		};
		/** A set of indexed geometry data */
		struct Geometry {
			size_t vertexSet;           // The vertex data set this geometry data refers to
			size_t indexSet;            // The index data set this geometry data refers to
			const IndexData* indexData; // The index information which describes the triangles.
			RenderOperation::OperationType opType;  // The operation type used to render this geometry
		};
		/** Comparator for sorting geometries by vertex set */
		struct geometryLess {
			bool operator()(const Geometry& a, const Geometry& b) const
			{
				if (a.vertexSet < b.vertexSet) return true;
				if (a.vertexSet > b.vertexSet) return false;
				return a.indexSet < b.indexSet;
			}
		};
		/** Comparator for unique vertex list */
		struct vectorLess {
			bool operator()(const Vector3& a, const Vector3& b) const
			{
				if (a.x < b.x) return true;
				if (a.x > b.x) return false;
				if (a.y < b.y) return true;
				if (a.y > b.y) return false;
				return a.z < b.z;
			}
		};

		typedef vector<const VertexData*>::type VertexDataList;
		typedef vector<Geometry>::type GeometryList;
		typedef vector<CommonVertex>::type CommonVertexList;

		GeometryList mGeometryList;
		VertexDataList mVertexDataList;
		CommonVertexList mVertices;
		EdgeData* mEdgeData;
		/// Map for identifying common vertices
		typedef map<Vector3, size_t, vectorLess>::type CommonVertexMap;
		CommonVertexMap mCommonVertexMap;
		/** Edge map, used to connect edges. Note we allow many triangles on an edge,
		after connected an existing edge, we will remove it and never used again.
		*/
		typedef multimap< std::pair<size_t, size_t>, std::pair<size_t, size_t> >::type EdgeMap;
		EdgeMap mEdgeMap;

		void buildTrianglesEdges(const Geometry &geometry);

		/// Finds an existing common vertex, or inserts a new one
		size_t findOrCreateCommonVertex(const Vector3& vec, size_t vertexSet,
			size_t indexSet, size_t originalIndex);
		/// Connect existing edge or create a new edge - utility method during building
		void connectOrCreateEdge(size_t vertexSet, size_t triangleIndex, size_t vertIndex0, size_t vertIndex1,
			size_t sharedVertIndex0, size_t sharedVertIndex1);
	};
}


#endif