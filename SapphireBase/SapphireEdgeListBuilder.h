#ifndef _SAPPHIRE_EDGE_LIST_BUILDER_
#define _SAPPHIRE_EDGE_LIST_BUILDER_
#include "SapphirePrerequisites.h"
#include "SapphireLog.h"
#include "SapphireVector3.h"

namespace Sapphire
{
	/**
	��������һ�������ĸ����Ķ��㼯���������ı����Ӷ���Ҫ����Ϣ
	@remarks
	��Щ��Ϣ��EdgeListBuilder�๹����ע�����һ���������������ɶ�������񹹳ɣ�
	��ʱ�Ƿ���ı߱�
	*/
	class _SapphireExport EdgeData : public EdgeDataAlloc
	{
		/** �����������νṹ */
		struct Triangle {
			/**
			��������ε��������ϣ�ע�⣺�����������һ���ߵ�һ��������һ���ò�ͬ�Ķ��㻺���ǿ��ܵģ�
			*/
			size_t indexSet;
			/** �����ڶ��㼯�ϵ���Щ���� */
			size_t vertexSet;
			size_t vertIndex[3];/// ������������ϵ��ԭ����
			size_t sharedVertIndex[3]; /// ������������ϵ��һ���������ظ��Ĺ���Ķ��㻺��(������������ǿ��ŵ�)

			Triangle() :indexSet(0), vertexSet(0) {}
		};

		/** ������. */
		struct Edge {
			/**
			��������ɸ��ӱ�ʾ���������tri��ע��tri0�Ǳ�ʾ�����߰���ʱ�뷽��tri1�����෴
			*/
			size_t triIndex[2];
			/**
			�����ߵ�������ע�⣺������������ָ���Ķ��㼯��'VertexSet'�У�
			*/
			size_t vertIndex[2];
			/**
			���ڹ���ߵĶ����������������
			*/
			size_t sharedVertIndex[2];
			/** ָ��������ǲ���һ���˻��ıߣ������������������� */
			bool degenerate;
		};

		// �������淨�ߵ�4d�������飬��������������������ĵ�λ����
		// ���ϵ�ԭ��ľ���(4d)
		//�����ڶ���SIMD�Ż���ʽ
		typedef std::vector<Vector4, STLAllocator<Vector4, CategorisedAlignAllocPolicy<MEMCATEGORY_GEOMETRY> > > TriangleFaceNormalList;

		// ���ڼ��������Ĺ�������
		// ��std::vector<char> ����std::vector<bool> �����Ա�ʵ������λ��bitfiled�ᵼ�¶�ʧ����
		typedef vector<char>::type TriangleLightFacingList;

		//�������б�
		typedef vector<Triangle>::type TriangleList;
		//�߱�
		typedef vector<Edge>::type EdgeList;



		/** һ�鹲����ͬ�������ݵı� */
		struct EdgeGroup
		{
			/**�������İ�����Щ����Ķ��㼯�ϵ����� */
			size_t vertexSet;
			/** �����������Ķ�������ָ�� */
			//	const VertexData* vertexData;
			/**
			�������������������ָʾ�������ĵ�һ�������Σ����������������������ζ��������������������
			*/
			size_t triStart;
			/** ������������� */
			size_t triCount;
			/** �߱� */
			EdgeList edges;

		};

		typedef vector<EdgeGroup>::type EdgeGroupList;

		/**
		�����������飬�������е������εı߱�ע�⣺��Щ��������Ա�����з���
		*/
		TriangleList triangles;
		/**
		���е��������淨�ߣ����������κ�һһ��Ӧ*/
		TriangleFaceNormalList triangleFaceNormals;
		/** �����γ���״̬�����������κ�һһ��Ӧ */
		TriangleLightFacingList triangleLightFacings;
		/** ��߱߱����еı���*/
		EdgeGroupList edgeGroups;
		/** ָʾ�����Ƕ෽��ı�׼ */
		bool isClosed;


		/**
		������߱�����������ι�Դ����״̬
		@remarks
		����ͨ���Ǽ��������ĵ�һ������������������������Դ���Ǳ����Դ��
		���״̬��'triangleLightFacings'
		@param lightPos �ǹ�Դ�ڶ���ռ��е�4Dλ�ã� ע�⣺����֮ƽ�й⣨û��λ�õģ�����w������0��xyzλ�����䷽��
		*/
		void updateTriangleLightFacing(const Vector4& lightPos);

		/**
		��������߱����λ����Ϣ���淨�ߣ����ڶ�������
		@param vertexSet ���Ǹ��µĶ��㼯��
		@param positionBuffer ����λ�û�����������ֻ����XYZ
		*/
		//void updateFaceNormals(size_t vertexSet, const HardwareVertexBufferSharedPtr& positionBuffer);



		// DEBUG�ķ���
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