#ifndef _SAPPHIRE_C_MESH_MANIPULATOR_
#define _SAPPHIRE_C_MESH_MANIPULATOR_

#include "SapphireIMeshManipulator.h"

namespace Sapphire
{
	//! һ���򵥲�������Ľӿ�
	/** ���ţ�����alphaֵ����ת���棬�ȵȡ���Щ���ڿ������������������������ʹ���.
	��������������ʱ�޸�����򶯻�
	*/
	class CMeshManipulator : public IMeshManipulator
	{
	public:
		//! ��ת����ķ���
		/** �ı䱳�������ε����泯�������β��ҷ�֮��Ȼ��
		\param mesh ҪӦ�ò��������� */
		virtual void flipSurfaces(IMesh* mesh) const;

		//! ���¼�����������еķ���
		/** \param mesh: ҪӦ�ò���������
		\param smooth: �����Ƿ���Ҫƽ��
		\param angleWeighted: ���������������صĽǶ���Ҫƽ������������������ߵľ��� */
		virtual void recalculateNormals(IMesh* mesh, bool smooth = false, bool angleWeighted = false) const;

		//! ���¼������񻺳��������еķ���
		/** \param buffer: ҪӦ�ò��������񻺳���
		\param smooth: �����Ƿ���Ҫƽ��
		\param angleWeighted: ���������������صĽǶ���Ҫƽ������������������ߵľ��� */
		virtual void recalculateNormals(IMeshBuffer* buffer, bool smooth = false, bool angleWeighted = false) const;

		//! ��¡һ����̬IMesh����һ�����޸ĵ�SMesh
		/** �����з��ص����񻺳�������SMeshBuffer��SMeshBufferLightMap���͵�SMesh
		\param mesh Ҫ���Ƶ�����
		\return ��¡������  */
		virtual SMesh* createMeshCopy(IMesh* mesh) const;


		//! ����������ϴ���һ��planar����ӳ��
		/** \param mesh:  ҪӦ�ò���������
		\param resolution: plannarӳ��ķֱ��ʡ����Ǹ��ر��ֵ����������ռ����������ռ����
		*/
		virtual void makePlanarTextureMapping(IMesh* mesh, FLOAT32 resolution = 0.001f) const;


		//! ��������񻺳����ϴ���һ��planar����ӳ��
		/** \param meshbuffer: ҪӦ�ò��������񻺳���
		\param resolution: plannarӳ��ķֱ��ʡ����Ǹ��ر��ֵ����������ռ����������ռ���� */
		virtual void makePlanarTextureMapping(IMeshBuffer* meshbuffer, FLOAT32 resolution = 0.001f) const;

		//! ��������崴��һ��plannar����ӳ��
		/** ��������Ƕ�LWO plannar ӳ��ĵ�ǰʵ�֡���Ҫһ������ͨ�õ�ƫ��ֵ
		\param mesh ҪӦ�ò��������񻺳���
		\param resolutionS ��ˮƽ����planarӳ��ķֱ��ʡ� ���Ǹ��ڶ���ռ������ռ�֮��ı�����
		\param resolutionT �ڴ�ֱ����planarӳ��ķֱ��ʡ� ���Ǹ��ڶ���ռ������ռ�֮��ı�����
		\param axis ��������ͶӰ���ᡣ������ֵ��0(x),1(y),��2(z)
		\param offset ��ӵ��������λ�õ��������ڶ���ռ䣩
		*/
		void makePlanarTextureMapping(IMeshBuffer* buffer, FLOAT32 resolutionS, FLOAT32 resolutionT, UINT32 axis, const Vector3& offset) const;

		//! ��������񻺳崴��һ��plannar����ӳ��
		/** ��������Ƕ�LWO plannar ӳ��ĵ�ǰʵ�֡���Ҫһ������ͨ�õ�ƫ��ֵ
		\param mesh ҪӦ�ò��������񻺳���
		\param resolutionS ��ˮƽ����planarӳ��ķֱ��ʡ� ���Ǹ��ڶ���ռ������ռ�֮��ı�����
		\param resolutionT �ڴ�ֱ����planarӳ��ķֱ��ʡ� ���Ǹ��ڶ���ռ������ռ�֮��ı�����
		\param axis ��������ͶӰ���ᡣ������ֵ��0(x),1(y),��2(z)
		\param offset ��ӵ��������λ�õ��������ڶ���ռ䣩
		*/
		void makePlanarTextureMapping(IMesh* mesh, FLOAT32 resolutionS, FLOAT32 resolutionT, UINT32 axis, const Vector3& offset) const;

		//! ���¼������ߣ���Ҫһ���������񻺳���
		/** \param buffer ҪӦ�ò��������񻺳���
		\param recalculateNormals �����Ƿ���Ҫ���¼��㣬���������ԭ����ʹ��δ�ı�ġ�
		\param smooth �����Ƿ���Ҫƽ��
		\param angleWeighted ���������������صĽǶ���Ҫƽ������������������ߵľ���
		*/
		virtual void recalculateTangents(IMeshBuffer* buffer, bool recalculateNormals = false, bool smooth = false, bool angleWeighted = false) const;


		//! ���¼������ߣ���Ҫһ����������
		/** \param meshҪӦ�ò���������
		\param recalculateNormals �����Ƿ���Ҫ���¼��㣬���������ԭ����ʹ��δ�ı�ġ�
		\param smooth �����Ƿ���Ҫƽ��
		\param angleWeighted ���������������صĽǶ���Ҫƽ������������������ߵľ���
		*/
		virtual void recalculateTangents(IMesh* mesh, bool recalculateNormals = false, bool smooth = false, bool angleWeighted = false) const;

		//! ����һ�����Ƶ�������ֻ��S3DVertexTangents�������
		/** �������Ҫ����һ�����߿ռ䷨��ӳ��ļ����壬�������õġ���Ϊ������
		��������Ҫ�����ߺ͸��������ݡ�
		\param mesh ���������
		\param recalculateNormals ������ã����߻ᱻ���¼��㡣���򱣴�ԭ���ġ�
		ע�⣺���������߷ǳ���ͬ����Щ�淨�ߵļ��㣬���淨�߿������벻��ȷ�����ߡ�
		\param smooth ����������񻺳������淨��/�����Ƿ���Ҫƽ��
		\param angleWeighted ���ڸ���ƽ������
		\param recalculateTangents �Ƿ���Ҫʵ�ʵļ��㣬���ǾͲ��ô����ĺ�������
		\return ֻ��S3DVertexTangents������ɵ�����  */
		virtual IMesh* createMeshWithTangents(IMesh* mesh, bool recalculateNormals = false, bool smooth = false, bool angleWeighted = false, bool recalculateTangents = true) const;

		//! ����һ�����Ƶ�������ֻ��S3DVertex2TCoord�������
		/** \param mesh ���������
		\return ֻ��S3DVertex2TCoord������ɵ����� */
		virtual IMesh* createMeshWith2TCoords(IMesh* mesh) const;

		//! ����һ�����Ƶ�������ֻ��S3DVertex�������
		/** \param mesh ���������
		\return ֻ��S3DVertex������ɵ�����*/
		virtual IMesh* createMeshWith1TCoords(IMesh* mesh) const;

		//! ����һ��ʹ������δ���Ӷ���ĸ�������
		/** \param mesh ���������
		\return ֻ��δ֪����ɵ��������ж��㶼��֮ǰ���ظ�����*/
		virtual IMesh* createMeshUniquePrimitives(IMesh* mesh) const;

		//! ����һ��ʹ���������Ӷ���ĸ������� 
		/** \param mesh ���������
		\param tolerance ����Ƚϵķ�ֵ
		\return ��ʹ�����ඥ���б������*/
		virtual IMesh* createMeshWelded(IMesh *mesh, FLOAT32 tolerance = std::numeric_limits<FLOAT32>::epsilon()) const;

		//! ��ȡ���������������
		/** \param mesh ���������
		\return �������Ķ��������. */
		virtual SINT32 getPolyCount(IMesh* mesh) const;

		//! ��ȡ���������������
		/** \param mesh ���������
		\return�������Ķ��������. */
		virtual SINT32 getPolyCount(IAnimatedMesh* mesh) const;

		//! ����һ���¶�������������������
		/** \param mesh ��������
		\param type ���Ҫ�����Ķ������������
		\return �´����Ķ������� */
		virtual IAnimatedMesh * createAnimatedMesh(IMesh* mesh, E_ANIMATED_MESH_TYPE type) const;

		//!ͨ��Forsyth paper���㻺���Ż� 
		/**
		http://home.comcast.net/~tom_forsyth/papers/fast_vert_cache_opt.html

		����������̰߳�ȫ (������ڲ�ͬ���߳��Ż���������).

		\param mesh ������ԭ����
		\return ����������㻺����Ż���������� */
		virtual IMesh* createForsythOptimizedMesh(const IMesh *mesh) const;
	};
}


#endif