#ifndef _SAPPHIRE_EDGE_LIST_BUILDER_
#define _SAPPHIRE_EDGE_LIST_BUILDER_
#include "SapphirePrerequisites.h"
#include "SapphireLog.h"

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

		// �������淨�ߵ�4d�������飬��������������������ĵ�λ������ Array of 4D vector of triangle face normal, which is unit vector orthogonal
		// to the triangles, ���ϵ�ԭ��ľ���(4d)
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
		//TriangleList triangles;
		/**
		���е��������淨�ߣ����������κ�һһ��Ӧ*/
		//TriangleFaceNormalList triangleFaceNormals;
		/** �����γ���״̬�����������κ�һһ��Ӧ */
		//TriangleLightFacingList triangleLightFacings;
		/** ��߱߱����еı���*/
		//EdgeGroupList edgeGroups;
		/** ָʾ�����Ƕ෽��ı�׼ */
		bool isClosed;


		/**
		������߱�����������ι�Դ����״̬
		@remarks
		����ͨ���Ǽ��������ĵ�һ����
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