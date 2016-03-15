#ifndef _SAPPHIRE_SVERTEX_
#define _SAPPHIRE_SVERTEX_

#include "SapphirePrerequisites.h"
#include "SapphireVector2.h"
#include "SapphireVector3.h"
#include "SapphireColorValue.h"
#include "SapphireMath.h"

namespace Sapphire
{
	//! ���ж������͵�ö��
	enum E_VERTEX_TYPE
	{
		//! ��׼��������S3DVertex.
		EVT_STANDARD = 0,

		//! ����ʹ�������������꣬S3DVertex2TCoords.
		/** ͨ�����ڹ�����ͼ��������ʵļ����� */
		EVT_2TCOORDS,

		//! ʹ��һ�����ߺ͸����������Ķ��㣬 video::S3DVertexTangents.
		/** ͨ���������߿ռ�ķ�����ͼ*/
		EVT_TANGENTS
	};

	//!���涥�����͵�����
	const char* const sBuiltInVertexTypeNames[] =
	{
		"standard",
		"2tcoords",
		"tangents",
		0
	};


	//! ��׼����ṹ
	struct S3DVertex
	{
		 
		S3DVertex() {}

		 
		S3DVertex(Real x, Real y, Real z, Real nx, Real ny, Real nz, ColourValue c, Real tu, Real tv)
			: Pos(x, y, z), Normal(nx, ny, nz), Color(c), TCoords(tu, tv) {}

	 
		S3DVertex(const Vector3& pos, const Vector3& normal,
			ColourValue color, const Vector2& tcoords)
			: Pos(pos), Normal(normal), Color(color), TCoords(tcoords) {}

		//! λ��
		Vector3 Pos;

		//! ������
		Vector3 Normal;

		//! ��ɫ
		ColourValue Color;

		//! ��������
		Vector2 TCoords;

		bool operator==(const S3DVertex& other) const
		{
			return ((Pos == other.Pos) && (Normal == other.Normal) &&
				(Color == other.Color) && (TCoords == other.TCoords));
		}

		bool operator!=(const S3DVertex& other) const
		{
			return ((Pos != other.Pos) || (Normal != other.Normal) ||
				(Color != other.Color) || (TCoords != other.TCoords));
		}

		bool operator<(const S3DVertex& other) const
		{
			return ((Pos < other.Pos) ||
				((Pos == other.Pos) && (Normal < other.Normal)) ||
				((Pos == other.Pos) && (Normal == other.Normal) && (Color < other.Color)) ||
				((Pos == other.Pos) && (Normal == other.Normal) && (Color == other.Color) && (TCoords < other.TCoords)));
		}

		E_VERTEX_TYPE getType() const
		{
			return EVT_STANDARD;
		}

		S3DVertex getInterpolated(const S3DVertex& other, Real d)
		{
			d = Math::Clamp<Real>(d, 0.0f, 1.0f);
			return S3DVertex(Pos.getInterpolated(other.Pos, d),
				Normal.getInterpolated(other.Normal, d),
				Color.getInterpolated(other.Color, d),
				TCoords.getInterpolated(other.TCoords, d));
		}
	};


	//! ��������������Ķ���
	 /** ͨ�����ڹ�����ͼ��������ʵļ����� */
	 
	struct S3DVertex2TCoords : public S3DVertex
	{
		 
		S3DVertex2TCoords() : S3DVertex() {}

		//! ��������ͬ������������й��죬��û�з�����Ϣ
		S3DVertex2TCoords(Real x, Real y, Real z, ColourValue c, Real tu, Real tv, Real tu2, Real tv2)
			: S3DVertex(x, y, z, 0.0f, 0.0f, 0.0f, c, tu, tv), TCoords2(tu2, tv2) {}

		//!��������ͬ������������й��죬��û�з�����Ϣ
		S3DVertex2TCoords(const Vector3& pos, ColourValue color,
			const Vector2& tcoords, const Vector2& tcoords2)
			: S3DVertex(pos, Vector3(), color, tcoords), TCoords2(tcoords2) {}

		//! ������ֵ���й���
		S3DVertex2TCoords(const Vector3& pos, const Vector3& normal, const ColourValue& color,
			const Vector2& tcoords, const Vector2& tcoords2)
			: S3DVertex(pos, normal, color, tcoords), TCoords2(tcoords2) {}

		//! ������ֵ���й���
		S3DVertex2TCoords(Real x, Real y, Real z, Real nx, Real ny, Real nz, ColourValue c, Real tu, Real tv, Real tu2, Real tv2)
			: S3DVertex(x, y, z, nx, ny, nz, c, tu, tv), TCoords2(tu2, tv2) {}

		//! ����ͬ����������ͷ��߽��й���
		S3DVertex2TCoords(Real x, Real y, Real z, Real nx, Real ny, Real nz, ColourValue c, Real tu, Real tv)
			: S3DVertex(x, y, z, nx, ny, nz, c, tu, tv), TCoords2(tu, tv) {}

		//! ����ͬ����������ͷ��߽��й���
		S3DVertex2TCoords(const Vector3& pos, const Vector3& normal,
			ColourValue color, const Vector2& tcoords)
			: S3DVertex(pos, normal, color, tcoords), TCoords2(tcoords) {}

		//! ��S3DVertex����
		S3DVertex2TCoords(S3DVertex& o) : S3DVertex(o) {}

		//!�ڶ�����������
		Vector2 TCoords2;

		 
		bool operator==(const S3DVertex2TCoords& other) const
		{
			return ((static_cast<S3DVertex>(*this) == other) &&
				(TCoords2 == other.TCoords2));
		}

		 
		bool operator!=(const S3DVertex2TCoords& other) const
		{
			return ((static_cast<S3DVertex>(*this) != other) ||
				(TCoords2 != other.TCoords2));
		}

		bool operator<(const S3DVertex2TCoords& other) const
		{
			return ((static_cast<S3DVertex>(*this) < other) ||
				((static_cast<S3DVertex>(*this) == other) && (TCoords2 < other.TCoords2)));
		}

		E_VERTEX_TYPE getType() const
		{
			return EVT_2TCOORDS;
		}

		S3DVertex2TCoords getInterpolated(const S3DVertex2TCoords& other, Real d)
		{
			d = Math::Clamp<Real>(d, 0.0f, 1.0f);
			return S3DVertex2TCoords(Pos.getInterpolated(other.Pos, d),
				Normal.getInterpolated(other.Normal, d),
				Color.getInterpolated(other.Color, d),
				TCoords.getInterpolated(other.TCoords, d),
				TCoords2.getInterpolated(other.TCoords2, d));
		}
	};



	//! ʹ��һ�����ߺ͸����������Ķ��㣬 video::S3DVertexTangents.
	/**ͨ���������߿ռ�ķ�����ͼ. */
	struct S3DVertexTangents : public S3DVertex
	{
		 
		S3DVertexTangents() : S3DVertex() { }

		 
		S3DVertexTangents(Real x, Real y, Real z, Real nx = 0.0f, Real ny = 0.0f, Real nz = 0.0f,
			ColourValue c = ColourValue::White, Real tu = 0.0f, Real tv = 0.0f,
			Real tanx = 0.0f, Real tany = 0.0f, Real tanz = 0.0f,
			Real bx = 0.0f, Real by = 0.0f, Real bz = 0.0f)
			: S3DVertex(x, y, z, nx, ny, nz, c, tu, tv), Tangent(tanx, tany, tanz), Binormal(bx, by, bz) { }

		 
		S3DVertexTangents(const Vector3& pos, ColourValue c,
			const Vector2& tcoords)
			: S3DVertex(pos, Vector3(), c, tcoords) { }

	 
		S3DVertexTangents(const Vector3& pos,
			const Vector3& normal, ColourValue c,
			const Vector2& tcoords,
			const Vector3& tangent = Vector3(),
			const Vector3& binormal = Vector3())
			: S3DVertex(pos, normal, c, tcoords), Tangent(tangent), Binormal(binormal) { }

		//! ���������X���������
		Vector3 Tangent;

		//! ����������(tangent x normal)
		Vector3 Binormal;

		bool operator==(const S3DVertexTangents& other) const
		{
			return ((static_cast<S3DVertex>(*this) == other) &&
				(Tangent == other.Tangent) &&
				(Binormal == other.Binormal));
		}

		bool operator!=(const S3DVertexTangents& other) const
		{
			return ((static_cast<S3DVertex>(*this) != other) ||
				(Tangent != other.Tangent) ||
				(Binormal != other.Binormal));
		}

		bool operator<(const S3DVertexTangents& other) const
		{
			return ((static_cast<S3DVertex>(*this) < other) ||
				((static_cast<S3DVertex>(*this) == other) && (Tangent < other.Tangent)) ||
				((static_cast<S3DVertex>(*this) == other) && (Tangent == other.Tangent) && (Binormal < other.Binormal)));
		}

		E_VERTEX_TYPE getType() const
		{
			return EVT_TANGENTS;
		}

		S3DVertexTangents getInterpolated(const S3DVertexTangents& other, Real d)
		{
			d = Math::Clamp<Real>(d, 0.0f, 1.0f);
			return S3DVertexTangents(Pos.getInterpolated(other.Pos, d),
				Normal.getInterpolated(other.Normal, d),
				Color.getInterpolated(other.Color, d),
				TCoords.getInterpolated(other.TCoords, d),
				Tangent.getInterpolated(other.Tangent, d),
				Binormal.getInterpolated(other.Binormal, d));
		}
	};


	inline UINT32 getVertexPitchFromType(E_VERTEX_TYPE vertexType)
	{
		switch (vertexType)
		{
		case EVT_2TCOORDS:
			return sizeof(S3DVertex2TCoords);
		case EVT_TANGENTS:
			return sizeof(S3DVertexTangents);
		default:
			return sizeof(S3DVertex);
		}
	}
}

#endif