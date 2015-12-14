#ifndef _SAPPHIRE_IMESH_MANIPULATOR_
#define _SAPPHIRE_IMESH_MANIPULATOR_

#include "SapphirePrerequisites.h"
#include "SapphireIReferenceCounter.h"
#include "SapphireIMesh.h"
#include "SapphireIAnimatedMesh.h"
#include "SapphireVector3.h"
#include "SapphireSVertex.h"
#include "SapphireVector2.h"
#include "SapphireMatrix4.h"
#include "SapphireAxisAlignedBox.h"
#include "SapphireSVertexManipulator.h"


namespace Sapphire
{
	class IAnimatedMesh;

	struct SMesh;

	//! һ���򵥲�������Ľӿ�
	/** ���ţ�����alphaֵ����ת���棬�ȵȡ���Щ���ڿ������������������������ʹ���.
	��������������ʱ�޸�����򶯻�
	*/
	class IMeshManipulator : public virtual IReferenceCounter
	{
	public:

		//! ��ת����ķ���
		/** �ı䱳�������ε����泯�������β��ҷ�֮��Ȼ��
		\param mesh ҪӦ�ò��������� */
		virtual void flipSurfaces(IMesh* mesh) const = 0;

		//! �������������alpha������ɫֵ��һ����ֵ
		/** \param mesh ҪӦ�ò���������
		\param alpha �µ�alphaֵ��������0��255*/
		void setVertexColorAlpha(IMesh* mesh, SINT32 alpha) const
		{
			apply(SVertexColorSetAlphaManipulator(alpha), mesh);
		}

		//! �������������alpha������ɫֵ��һ����ֵ
		/** \param buffer ҪӦ�ò��������񻺳���
		\param alpha �µ�alphaֵ��������0��255 */
		void setVertexColorAlpha(IMeshBuffer* buffer, SINT32 alpha) const
		{
			apply(SVertexColorSetAlphaManipulator(alpha), buffer);
		}

		//! �������ж������ɫ��һ����ɫ
		/** \param mesh ҪӦ�ò���������
		\param color ����ɫ */
		void setVertexColors(IMesh* mesh, ColourValue color) const
		{
			apply(SVertexColorSetManipulator(color), mesh);
		}

		//! �������ж������ɫ��һ����ɫ
		/** \param buffer ҪӦ�ò��������񻺳���
		\param color �µ���ɫ */
		void setVertexColors(IMeshBuffer* buffer, ColourValue color) const
		{
			apply(SVertexColorSetManipulator(color), buffer);
		}

		//! ���¼�����������еķ���
		/** \param mesh: ҪӦ�ò��������� 
		\param smooth: �����Ƿ���Ҫƽ��
		\param angleWeighted: ���������������صĽǶ���Ҫƽ������������������ߵľ��� */
		virtual void recalculateNormals(IMesh* mesh, bool smooth = false,
			bool angleWeighted = false) const = 0;

		//! ���¼������񻺳��������еķ���
		/** \param buffer: ҪӦ�ò��������񻺳��� 
		\param smooth: �����Ƿ���Ҫƽ��
		\param angleWeighted: ���������������صĽǶ���Ҫƽ������������������ߵľ��� */
		virtual void recalculateNormals(IMeshBuffer* buffer,
			bool smooth = false, bool angleWeighted = false) const = 0;

		//! ���¼������ߣ���Ҫһ����������
		/** \param meshҪӦ�ò���������
		\param recalculateNormals �����Ƿ���Ҫ���¼��㣬���������ԭ����ʹ��δ�ı�ġ�
		\param smooth �����Ƿ���Ҫƽ��
		\param angleWeighted ���������������صĽǶ���Ҫƽ������������������ߵľ���
		*/
		virtual void recalculateTangents(IMesh* mesh,
			bool recalculateNormals = false, bool smooth = false,
			bool angleWeighted = false) const = 0;

		//! ���¼������ߣ���Ҫһ���������񻺳���
		/** \param buffer ҪӦ�ò��������񻺳���
		\param recalculateNormals �����Ƿ���Ҫ���¼��㣬���������ԭ����ʹ��δ�ı�ġ�
		\param smooth �����Ƿ���Ҫƽ��
		\param angleWeighted ���������������صĽǶ���Ҫƽ������������������ߵľ���
		*/
		virtual void recalculateTangents(IMeshBuffer* buffer,
			bool recalculateNormals = false, bool smooth = false,
			bool angleWeighted = false) const = 0;

		//! ��ʵ������������ţ������ǳ����ڵ�
		/** \param mesh ҪӦ�ò���������
		\param factor ����ÿ�������������*/
		void scale(IMesh* mesh, const Vector3& factor) const
		{
			apply(SVertexPositionScaleManipulator(factor), mesh, true);
		}

		//! ��ʵ�����񻺳����������ţ������ǳ����ڵ�
		/** \param buffer ҪӦ�ò��������񻺳���
		\param factor  ����ÿ�������������*/
		void scale(IMeshBuffer* buffer, const Vector3& factor) const
		{
			apply(SVertexPositionScaleManipulator(factor), buffer, true);
		}

	

		//! ����һ���������������
		/** \param mesh ҪӦ�ò���������
		\param factor ÿ��������ŵ�����
		\param level ��������������1��ʼ������LightMap Buffer֧�ֵȼ�2�Ĵ���*/
		void scaleTCoords(IMesh* mesh, const Vector2& factor, UINT32 level = 1) const
		{
			apply(SVertexTCoordsScaleManipulator(factor, level), mesh);
		}

		//! ����һ�����񻺳�������������
		/** \param buffer ҪӦ�ò��������񻺳���
		\param factor ÿ��������ŵ�����
		\param level ��������������1��ʼ������LightMap Buffer֧�ֵȼ�2�Ĵ���*/
		void scaleTCoords(IMeshBuffer* buffer, const Vector2& factor, UINT32 level = 1) const
		{
			apply(SVertexTCoordsScaleManipulator(factor, level), buffer);
		}

		//! Ӧ��һ���任��һ������
		/** \param mesh  ҪӦ�ò���������
		\param m �任���� */
		void transform(IMesh* mesh, const Matrix4& m) const
		{
			apply(SVertexPositionTransformManipulator(m), mesh, true);
		}

		//! Ӧ��һ���任��һ�����񻺳���
		/** \param buffer ҪӦ�ò��������񻺳���
		\param m �任���� */
		void transform(IMeshBuffer* buffer, const Matrix4& m) const
		{
			apply(SVertexPositionTransformManipulator(m), buffer, true);
		}

		

		//! ����������ϴ���һ��planar����ӳ��
		/** \param mesh:  ҪӦ�ò���������
		\param resolution: plannarӳ��ķֱ��ʡ����Ǹ��ر��ֵ����������ռ����������ռ����
		  */
		virtual void makePlanarTextureMapping(IMesh* mesh, Real resolution = 0.001f) const = 0;

		//! ��������񻺳����ϴ���һ��planar����ӳ��
		/** \param meshbuffer: ҪӦ�ò��������񻺳���
		\param resolution: plannarӳ��ķֱ��ʡ����Ǹ��ر��ֵ����������ռ����������ռ���� */
		virtual void makePlanarTextureMapping(IMeshBuffer* meshbuffer, Real resolution = 0.001f) const = 0;

		//! ��������崴��һ��plannar����ӳ��
		/** ��������Ƕ�LWO plannar ӳ��ĵ�ǰʵ�֡���Ҫһ������ͨ�õ�ƫ��ֵ
		\param mesh ҪӦ�ò��������񻺳���
		\param resolutionS ��ˮƽ����planarӳ��ķֱ��ʡ� ���Ǹ��ڶ���ռ������ռ�֮��ı�����
		\param resolutionT �ڴ�ֱ����planarӳ��ķֱ��ʡ� ���Ǹ��ڶ���ռ������ռ�֮��ı�����
		\param axis ��������ͶӰ���ᡣ������ֵ��0(x),1(y),��2(z)
		\param offset ��ӵ��������λ�õ��������ڶ���ռ䣩
		*/
		virtual void makePlanarTextureMapping(IMesh* mesh,
			Real resolutionS, Real resolutionT,
			UINT32 axis, const Vector3& offset) const = 0;

		//! ��������񻺳崴��һ��plannar����ӳ��
		/** ��������Ƕ�LWO plannar ӳ��ĵ�ǰʵ�֡���Ҫһ������ͨ�õ�ƫ��ֵ
		\param mesh ҪӦ�ò��������񻺳���
		\param resolutionS ��ˮƽ����planarӳ��ķֱ��ʡ� ���Ǹ��ڶ���ռ������ռ�֮��ı�����
		\param resolutionT �ڴ�ֱ����planarӳ��ķֱ��ʡ� ���Ǹ��ڶ���ռ������ռ�֮��ı�����
		\param axis ��������ͶӰ���ᡣ������ֵ��0(x),1(y),��2(z)
		\param offset ��ӵ��������λ�õ��������ڶ���ռ䣩
		*/
		virtual void makePlanarTextureMapping(IMeshBuffer* buffer,
			Real resolutionS, Real resolutionT,
			UINT32 axis, const Vector3& offset) const = 0;

		//! ��¡һ����̬IMesh����һ�����޸ĵ�SMesh
		/** �����з��ص����񻺳�������SMeshBuffer��SMeshBufferLightMap���͵�SMesh
		\param mesh Ҫ���Ƶ�����
		\return ��¡������  */
		virtual SMesh* createMeshCopy(IMesh* mesh) const = 0;

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
		virtual IMesh* createMeshWithTangents(IMesh* mesh,
			bool recalculateNormals = false, bool smooth = false,
			bool angleWeighted = false, bool recalculateTangents = true) const = 0;

		//! ����һ�����Ƶ�������ֻ��S3DVertex2TCoord�������
		/** \param mesh ���������
		\return ֻ��S3DVertex2TCoord������ɵ����� */
		virtual IMesh* createMeshWith2TCoords(IMesh* mesh) const = 0;

		//! ����һ�����Ƶ�������ֻ��S3DVertex�������
		/** \param mesh ���������
		\return ֻ��S3DVertex������ɵ�����*/
		virtual IMesh* createMeshWith1TCoords(IMesh* mesh) const = 0;

		//! ����һ��ʹ������δ���Ӷ���ĸ�������
		/** \param mesh ���������
		\return ֻ��δ֪����ɵ��������ж��㶼��֮ǰ���ظ�����*/
		virtual IMesh* createMeshUniquePrimitives(IMesh* mesh) const = 0;

		//! ����һ��ʹ���������Ӷ���ĸ������� 
		/** \param mesh ���������
		\param tolerance ����Ƚϵķ�ֵ
		\return ��ʹ�����ඥ���б������*/
		virtual IMesh* createMeshWelded(IMesh* mesh, Real tolerance = std::numeric_limits<Real>::epsilon()) const = 0;

		//! ��ȡ���������������
		/** \param mesh ���������
		\return �������Ķ��������. */
		virtual SINT32 getPolyCount(IMesh* mesh) const = 0;

		//! ��ȡ���������������
		/** \param mesh ���������
		\return�������Ķ��������. */
		virtual SINT32 getPolyCount(IAnimatedMesh* mesh) const = 0;

		//! ����һ���¶�������������������
		/** \param mesh ��������
		\param type ���Ҫ�����Ķ������������
		\return �´����Ķ������� */
		virtual IAnimatedMesh * createAnimatedMesh(IMesh* mesh,
			E_ANIMATED_MESH_TYPE type = EAMT_UNKNOWN) const = 0;

		//!ͨ��Forsyth paper���㻺���Ż� 
		/** 
		http://home.comcast.net/~tom_forsyth/papers/fast_vert_cache_opt.html

		����������̰߳�ȫ (������ڲ�ͬ���߳��Ż���������).

		\param mesh ������ԭ����
		\return ����������㻺����Ż���������� */
		virtual IMesh* createForsythOptimizedMesh(const IMesh *mesh) const = 0;

		//! �����񻺳���Ӧ��һ��������manipulator 
		/** \param func һ���������������ĺ���
		\param buffer ҪӦ����������������񻺳���
		\param boundingBoxUpdate �����ײ������Ҫͨ��������������£�ָ�����
		\return ��������ɹ�Ӧ�ã�����true������false */
		template <typename Functor>
		bool apply(const Functor& func, IMeshBuffer* buffer, bool boundingBoxUpdate = false) const
		{
			return apply_(func, buffer, boundingBoxUpdate, func);
		}


		//! ������Ӧ��һ��������manipulator 
		/** \param func һ���������������ĺ���
		\param mesh ҪӦ�����������������
		\param boundingBoxUpdate �����ײ������Ҫͨ��������������£�ָ�����
		\return ��������ɹ�Ӧ�ã�����true������false */
		template <typename Functor>
		bool apply(const Functor& func, IMesh* mesh, bool boundingBoxUpdate = false) const
		{
			if (!mesh)
				return true;
			bool result = true;
			AxisAlignedBox bufferbox;
			for (UINT32 i = 0; i<mesh->getMeshBufferCount(); ++i)
			{
				result &= apply(func, mesh->getMeshBuffer(i), boundingBoxUpdate);
				if (boundingBoxUpdate)
				{
					if (0 == i)
						bufferbox.reset(mesh->getMeshBuffer(i)->getBoundingBox());
					else
						bufferbox.addInternalBox(mesh->getMeshBuffer(i)->getBoundingBox());
				}
			}
			if (boundingBoxUpdate)
				mesh->setBoundingBox(bufferbox);
			return result;
		}

	protected:
		//! Ӧ��һ����������������͵Ĳ�����
		/** \param func ������������ĺ���
		\param buffer  ҪӦ����������������񻺳���
		\param boundingBoxUpdate �����ײ������Ҫͨ��������������£�ָ�����
		\param typeTest ���õĲ�����������Unused parameter,����ѡ����������δ��ݵĺ��������ͺ��ʵĵ��á�
		\return ��������ɹ�Ӧ�ã�����true������false */
		template <typename Functor>
		bool apply_(const Functor& func, IMeshBuffer* buffer, bool boundingBoxUpdate, const IVertexManipulator& typeTest) const
		{
			if (!buffer)
				return true;

			AxisAlignedBox bufferbox;
			for (UINT32 i = 0; i<buffer->getVertexCount(); ++i)
			{
				switch (buffer->getVertexType())
				{
				case EVT_STANDARD:
				{
					S3DVertex* verts = (S3DVertex*)buffer->getVertices();
					func(verts[i]);
				}
				break;
				case EVT_2TCOORDS:
				{
					S3DVertex2TCoords* verts = (S3DVertex2TCoords*)buffer->getVertices();
					func(verts[i]);
				}
				break;
				case EVT_TANGENTS:
				{
					S3DVertexTangents* verts = (S3DVertexTangents*)buffer->getVertices();
					func(verts[i]);
				}
				break;
				}
				if (boundingBoxUpdate)
				{
					if (0 == i)
						bufferbox.reset(buffer->getPosition(0));
					else
						bufferbox.addInternalPoint(buffer->getPosition(i));
				}
			}
			if (boundingBoxUpdate)
				buffer->setBoundingBox(bufferbox);
			return true;
		}
	};
}

#endif