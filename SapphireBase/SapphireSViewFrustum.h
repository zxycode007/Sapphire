#ifndef _SAPPHIRE_SVIEW_FRUSTUM_
#define _SAPPHIRE_SVIEW_FRUSTUM_

#include "SapphirePrerequisites.h"
#include "SapphireQMath.h"
#include "SapphireVector3.h"
#include "SapphireMatrix4.h"
#include "SapphireAxisAlignedBox.h"
#include "SapphireLine3D.h"
#include "SapphirePlane.h"
#include "SapphireIVideoDriver.h"


namespace Sapphire
{
	//! ������׶�塣ֻ������Ŀɼ��Կռ�
	/** �����׶����һ����յ�6���塣��6���湲��8�����㡣
	����ṹ�����һ��������8������İ�Χ�����
	*/
	struct SViewFrustum
	{
		enum VFPLANES
		{
			//! ��׶���Զ��ƽ�档���Ƕ��۾���˵��Զ��ƽ��
			VF_FAR_PLANE = 0,
			//! ��׶��Ľ���ƽ�档���Ƕ��۾���˵�����ƽ��
			VF_NEAR_PLANE,
			//! ��׶������ƽ��
			VF_LEFT_PLANE,
			//! ��׶����Ҳ�ƽ��
			VF_RIGHT_PLANE,
			//! ��׶��ĵײ�ƽ��
			VF_BOTTOM_PLANE,
			//! ��׶��Ķ���ƽ��
			VF_TOP_PLANE,

			//!��׶���յ�ƽ������Ӧ����6��
			VF_PLANE_COUNT
		};


		//! Ĭ�Ϲ��캯��
		SViewFrustum() { 
			for (int i = 0; i<ETS_COUNT_FRUSTUM; ++i)
				Matrices[i] = Matrix4::IDENTITY;
		}

		//! �������캯��
		SViewFrustum(const SViewFrustum& other);

		//! ����һ��ͶӰ��۲���󴴽�һ����׶��
		SViewFrustum(const Matrix4& mat);

		//!����һ��ͶӰ��۲���󴴽�һ����׶��
		inline void setFrom(const Matrix4& mat);

		//! ͨ������任��׶��
		/** \param mat: Ҫ�任�����׶��ľ���*/
		void transform(const Matrix4& mat);

		//! ���������׶���ڲ���Զ�����Ͻǵĵ�
		Vector3 getFarLeftUp() const;

		//!���������׶���ڲ���Զ�����½ǵĵ�
		Vector3 getFarLeftDown() const;

		//!���������׶���ڲ���Զ�����Ͻǵĵ�
		Vector3 getFarRightUp() const;

		//! ���������׶���ڲ���Զ�����½ǵĵ�
		Vector3 getFarRightDown() const;

		//! ���������׶���ڲ���������Ͻǵĵ�
		Vector3 getNearLeftUp() const;

		//! ���������׶���ڲ���������½ǵĵ�
		Vector3 getNearLeftDown() const;

		//!���������׶���ڲ���������Ͻǵĵ�
		Vector3 getNearRightUp() const;

		//!���������׶���ڲ���Զ�����½ǵĵ�
		Vector3 getNearRightDown() const;

		//! ���������׶��İ�Χ�����
		const AxisAlignedBox &getBoundingBox() const;

		//! ���¼��������6ƽ��İ�Χ�����
		inline void recalculateBoundingBox();

		//! ��ȡ������׶��E_TRANSFORMATION_STATE��״̬����
		Matrix4& getTransform(E_TRANSFORMATION_STATE state);

		//! ��ȡ������׶��E_TRANSFORMATION_STATE��״̬����
		const Matrix4& getTransform(E_TRANSFORMATION_STATE state) const;

		//! ͨ����׶������߶�
		/** \return ����߶α����÷���true�����򣬷���false*/
		bool clipLine(line3d<Real>& line) const;

		//! ���λ��
		Vector3 cameraPosition;

		//! ��׶�������6�����ƽ��
		Plane planes[VF_PLANE_COUNT];

		//! ��׶��AABB����
		AxisAlignedBox boundingBox;

	private:
		//! ����һ����Ҫ�任����Ŀ�������ö��
		enum E_TRANSFORMATION_STATE_FRUSTUM
		{
			ETS_VIEW = 0,
			ETS_PROJECTION = 1,
			ETS_COUNT_FRUSTUM
		};

		//! ����һ����Ҫ�任����Ŀ���
		Matrix4 Matrices[ETS_COUNT_FRUSTUM];  //��ʱû���õ�
	};

 
	inline SViewFrustum::SViewFrustum(const SViewFrustum& other)
	{
		cameraPosition = other.cameraPosition;
		boundingBox = other.boundingBox;

		UINT32 i;
		for (i = 0; i<VF_PLANE_COUNT; ++i)
			planes[i] = other.planes[i];

		for (i = 0; i<ETS_COUNT_FRUSTUM; ++i)
			Matrices[i] = other.Matrices[i];
	}

	inline SViewFrustum::SViewFrustum(const Matrix4& mat)
	{
		setFrom(mat);
	}

	//ͨ��һ���������׶����б任
	inline void SViewFrustum::transform(const Matrix4& mat)
	{
		for (UINT32 i = 0; i < VF_PLANE_COUNT; ++i)
			planes[i].transformPlane(mat);//mat.transformPlane(planes[i]);

		cameraPosition = mat * cameraPosition;
		//mat.transformVect(cameraPosition);
		recalculateBoundingBox();
	}


	inline Vector3 SViewFrustum::getFarLeftUp() const
	{
		Vector3 p;
		planes[SViewFrustum::VF_FAR_PLANE].getIntersectionWithPlanes(
			planes[SViewFrustum::VF_TOP_PLANE],
			planes[SViewFrustum::VF_LEFT_PLANE], p);

		return p;
	}

	inline Vector3 SViewFrustum::getFarLeftDown() const
	{
		Vector3 p;
		planes[SViewFrustum::VF_FAR_PLANE].getIntersectionWithPlanes(
			planes[SViewFrustum::VF_BOTTOM_PLANE],
			planes[SViewFrustum::VF_LEFT_PLANE], p);

		return p;
	}

	inline Vector3 SViewFrustum::getFarRightUp() const
	{
		Vector3 p;
		planes[SViewFrustum::VF_FAR_PLANE].getIntersectionWithPlanes(
			planes[SViewFrustum::VF_TOP_PLANE],
			planes[SViewFrustum::VF_RIGHT_PLANE], p);

		return p;
	}

	inline Vector3 SViewFrustum::getFarRightDown() const
	{
		Vector3 p;
		planes[SViewFrustum::VF_FAR_PLANE].getIntersectionWithPlanes(
			planes[SViewFrustum::VF_BOTTOM_PLANE],
			planes[SViewFrustum::VF_RIGHT_PLANE], p);

		return p;
	}

	inline Vector3 SViewFrustum::getNearLeftUp() const
	{
		Vector3 p;
		planes[SViewFrustum::VF_NEAR_PLANE].getIntersectionWithPlanes(
			planes[SViewFrustum::VF_TOP_PLANE],
			planes[SViewFrustum::VF_LEFT_PLANE], p);

		return p;
	}

	inline Vector3 SViewFrustum::getNearLeftDown() const
	{
		Vector3 p;
		planes[SViewFrustum::VF_NEAR_PLANE].getIntersectionWithPlanes(
			planes[SViewFrustum::VF_BOTTOM_PLANE],
			planes[SViewFrustum::VF_LEFT_PLANE], p);

		return p;
	}

	inline Vector3 SViewFrustum::getNearRightUp() const
	{
		Vector3 p;
		planes[SViewFrustum::VF_NEAR_PLANE].getIntersectionWithPlanes(
			planes[SViewFrustum::VF_TOP_PLANE],
			planes[SViewFrustum::VF_RIGHT_PLANE], p);

		return p;
	}

	inline Vector3 SViewFrustum::getNearRightDown() const
	{
		Vector3 p;
		planes[SViewFrustum::VF_NEAR_PLANE].getIntersectionWithPlanes(
			planes[SViewFrustum::VF_BOTTOM_PLANE],
			planes[SViewFrustum::VF_RIGHT_PLANE], p);

		return p;
	}
	
	inline const AxisAlignedBox &SViewFrustum::getBoundingBox() const
	{
		return boundingBox;
	}
	//���¼����Χ�����
	inline void SViewFrustum::recalculateBoundingBox()
	{
		 
		boundingBox.reset(cameraPosition);
		
		boundingBox.addInternalPoint(getFarLeftUp());
		boundingBox.addInternalPoint(getFarRightUp());
		boundingBox.addInternalPoint(getFarLeftDown());
		boundingBox.addInternalPoint(getFarRightDown());
	}

	//! �������������һ������͸��ͶӰ�͹۲�������׶��
	inline void SViewFrustum::setFrom(const Matrix4& matrix)
	{
		Matrix4 mat = matrix;
		if (mat.RowMajor == false)
		{
			mat.transpose();
		}
		// ���ƽ��
		planes[VF_LEFT_PLANE].normal.x = mat.getIndex(3) + mat.getIndex(0);
		planes[VF_LEFT_PLANE].normal.y = mat.getIndex(7) + mat.getIndex(4);
		planes[VF_LEFT_PLANE].normal.z = mat.getIndex(11) + mat.getIndex(8);
		planes[VF_LEFT_PLANE].d = mat.getIndex(15) + mat.getIndex(12);

		// �Ҳ�ƽ��
		planes[VF_RIGHT_PLANE].normal.x = mat.getIndex(3) - mat.getIndex(0);
		planes[VF_RIGHT_PLANE].normal.y = mat.getIndex(7) - mat.getIndex(4);
		planes[VF_RIGHT_PLANE].normal.z = mat.getIndex(11) - mat.getIndex(8);
		planes[VF_RIGHT_PLANE].d = mat.getIndex(15) - mat.getIndex(12);

		// ����ƽ��
		planes[VF_TOP_PLANE].normal.x = mat.getIndex(3) - mat.getIndex(1);
		planes[VF_TOP_PLANE].normal.y = mat.getIndex(7) - mat.getIndex(5);
		planes[VF_TOP_PLANE].normal.z = mat.getIndex(11) - mat.getIndex(9);
		planes[VF_TOP_PLANE].d = mat.getIndex(15) - mat.getIndex(13);

		// �ײ�ƽ��
		planes[VF_BOTTOM_PLANE].normal.x = mat.getIndex(3) + mat.getIndex(1);
		planes[VF_BOTTOM_PLANE].normal.y = mat.getIndex(7) + mat.getIndex(5);
		planes[VF_BOTTOM_PLANE].normal.z = mat.getIndex(11) + mat.getIndex(9);
		planes[VF_BOTTOM_PLANE].d = mat.getIndex(15) + mat.getIndex(13);

		// Զ��ƽ��
		planes[VF_FAR_PLANE].normal.x = mat.getIndex(3) - mat.getIndex(2);
		planes[VF_FAR_PLANE].normal.y = mat.getIndex(7) - mat.getIndex(6);
		planes[VF_FAR_PLANE].normal.z = mat.getIndex(11) - mat.getIndex(10);
		planes[VF_FAR_PLANE].d = mat.getIndex(15) - mat.getIndex(14);

		// ����ƽ��
		planes[VF_NEAR_PLANE].normal.x = mat.getIndex(2);
		planes[VF_NEAR_PLANE].normal.y = mat.getIndex(6);
		planes[VF_NEAR_PLANE].normal.z = mat.getIndex(10);
		planes[VF_NEAR_PLANE].d = mat.getIndex(14);

		// ��׼������
		UINT32 i;
		for (i = 0; i != VF_PLANE_COUNT; ++i)
		{
			const Real len = -Q_rsqrtHW(
				planes[i].normal.squaredLength());
			planes[i].normal *= len;
			planes[i].d *= len;
		}

		// ���¼����Χ�����
		recalculateBoundingBox();
	}

	/*!
	ȡ�ñ���׶���ͶӰ�͹۲����
	*/
	inline Matrix4& SViewFrustum::getTransform(E_TRANSFORMATION_STATE state)
	{
		UINT32 index = 0;
		switch (state)
		{
		case E_TRANSFORMATION_STATE::ETS_PROJECTION:
			index = SViewFrustum::ETS_PROJECTION; break;
		case E_TRANSFORMATION_STATE::ETS_VIEW:
			index = SViewFrustum::ETS_VIEW; break;
		default:
			break;
		}
		return Matrices[index];
	}

	/*!
	ȡ�ñ���׶���ͶӰ�͹۲����
	*/
	inline const Matrix4& SViewFrustum::getTransform(E_TRANSFORMATION_STATE state) const
	{
		UINT32 index = 0;
		switch (state)
		{
		case ETS_PROJECTION:
			index = SViewFrustum::ETS_PROJECTION; break;
		case ETS_VIEW:
			index = SViewFrustum::ETS_VIEW; break;
		default:
			break;
		}
		return Matrices[index];
	}

	//!��׶������߶�
	inline bool SViewFrustum::clipLine(line3d<Real>& line) const
	{
		bool wasClipped = false;
		for (UINT32 i = 0; i < VF_PLANE_COUNT; ++i)
		{
			if (planes[i].classifyPointRelation(line.start) == ISREL3D_FRONT)
			{
				line.start = line.start.getInterpolated(line.end,
					planes[i].getKnownIntersectionWithLine(line.start, line.end));
				wasClipped = true;
			}
			if (planes[i].classifyPointRelation(line.end) == ISREL3D_FRONT)
			{
				line.end = line.start.getInterpolated(line.end,
					planes[i].getKnownIntersectionWithLine(line.start, line.end));
				wasClipped = true;
			}
		}
		return wasClipped;
	}
}

#endif