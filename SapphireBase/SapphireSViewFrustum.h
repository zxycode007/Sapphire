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
	//! 定义视锥体。只是相机的可见性空间
	/** 这个视锥体是一个封闭的6面体。这6个面共享8个顶点。
	这个结构存放了一个环绕这8个顶点的包围体盒子
	*/
	struct SViewFrustum
	{
		enum VFPLANES
		{
			//! 视锥体的远裁平面。这是对眼睛来说最远的平面
			VF_FAR_PLANE = 0,
			//! 视锥体的近裁平面。这是对眼睛来说最近的平面
			VF_NEAR_PLANE,
			//! 视锥体的左裁平面
			VF_LEFT_PLANE,
			//! 视锥体的右裁平面
			VF_RIGHT_PLANE,
			//! 视锥体的底裁平面
			VF_BOTTOM_PLANE,
			//! 视锥体的顶裁平面
			VF_TOP_PLANE,

			//!视锥体封闭的平面数，应该是6个
			VF_PLANE_COUNT
		};


		//! 默认构造函数
		SViewFrustum() { 
			for (int i = 0; i<ETS_COUNT_FRUSTUM; ++i)
				Matrices[i] = Matrix4::IDENTITY;
		}

		//! 拷贝构造函数
		SViewFrustum(const SViewFrustum& other);

		//! 基于一个投影或观察矩阵创建一个视锥体
		SViewFrustum(const Matrix4& mat);

		//!基于一个投影或观察矩阵创建一个视锥体
		inline void setFrom(const Matrix4& mat);

		//! 通过矩阵变换视锥体
		/** \param mat: 要变换这个视锥体的矩阵*/
		void transform(const Matrix4& mat);

		//! 返回这个视锥体内部最远端左上角的点
		Vector3 getFarLeftUp() const;

		//!返回这个视锥体内部最远端左下角的点
		Vector3 getFarLeftDown() const;

		//!返回这个视锥体内部最远端右上角的点
		Vector3 getFarRightUp() const;

		//! 返回这个视锥体内部最远端右下角的点
		Vector3 getFarRightDown() const;

		//! 返回这个视锥体内部最近端左上角的点
		Vector3 getNearLeftUp() const;

		//! 返回这个视锥体内部最近端左下角的点
		Vector3 getNearLeftDown() const;

		//!返回这个视锥体内部最近端右上角的点
		Vector3 getNearRightUp() const;

		//!返回这个视锥体内部最远端右下角的点
		Vector3 getNearRightDown() const;

		//! 返回这个视锥体的包围体盒子
		const AxisAlignedBox &getBoundingBox() const;

		//! 重新计算基于这6平面的包围体盒子
		inline void recalculateBoundingBox();

		//! 获取基于视锥体E_TRANSFORMATION_STATE的状态矩阵
		Matrix4& getTransform(E_TRANSFORMATION_STATE state);

		//! 获取基于视锥体E_TRANSFORMATION_STATE的状态矩阵
		const Matrix4& getTransform(E_TRANSFORMATION_STATE state) const;

		//! 通过视锥体剪裁线段
		/** \return 如果线段被剪裁返回true，否则，返回false*/
		bool clipLine(line3d<Real>& line) const;

		//! 相机位置
		Vector3 cameraPosition;

		//! 视锥体包括的6个封闭平面
		Plane planes[VF_PLANE_COUNT];

		//! 视锥体AABB盒子
		AxisAlignedBox boundingBox;

	private:
		//! 保存一个重要变换矩阵的拷贝类型枚举
		enum E_TRANSFORMATION_STATE_FRUSTUM
		{
			ETS_VIEW = 0,
			ETS_PROJECTION = 1,
			ETS_COUNT_FRUSTUM
		};

		//! 保存一个重要变换矩阵的拷贝
		Matrix4 Matrices[ETS_COUNT_FRUSTUM];  //暂时没有用到
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

	//通过一个矩阵对视锥体进行变换
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
	//重新计算包围体盒子
	inline void SViewFrustum::recalculateBoundingBox()
	{
		 
		boundingBox.reset(cameraPosition);
		
		boundingBox.addInternalPoint(getFarLeftUp());
		boundingBox.addInternalPoint(getFarRightUp());
		boundingBox.addInternalPoint(getFarLeftDown());
		boundingBox.addInternalPoint(getFarRightDown());
	}

	//! 这个构造器创建一个基于透视投影和观察矩阵的视锥体
	inline void SViewFrustum::setFrom(const Matrix4& matrix)
	{
		Matrix4 mat = matrix;
		if (mat.RowMajor == false)
		{
			mat.transpose();
		}
		// 左裁平面
		planes[VF_LEFT_PLANE].normal.x = mat.getIndex(3) + mat.getIndex(0);
		planes[VF_LEFT_PLANE].normal.y = mat.getIndex(7) + mat.getIndex(4);
		planes[VF_LEFT_PLANE].normal.z = mat.getIndex(11) + mat.getIndex(8);
		planes[VF_LEFT_PLANE].d = mat.getIndex(15) + mat.getIndex(12);

		// 右裁平面
		planes[VF_RIGHT_PLANE].normal.x = mat.getIndex(3) - mat.getIndex(0);
		planes[VF_RIGHT_PLANE].normal.y = mat.getIndex(7) - mat.getIndex(4);
		planes[VF_RIGHT_PLANE].normal.z = mat.getIndex(11) - mat.getIndex(8);
		planes[VF_RIGHT_PLANE].d = mat.getIndex(15) - mat.getIndex(12);

		// 顶裁平面
		planes[VF_TOP_PLANE].normal.x = mat.getIndex(3) - mat.getIndex(1);
		planes[VF_TOP_PLANE].normal.y = mat.getIndex(7) - mat.getIndex(5);
		planes[VF_TOP_PLANE].normal.z = mat.getIndex(11) - mat.getIndex(9);
		planes[VF_TOP_PLANE].d = mat.getIndex(15) - mat.getIndex(13);

		// 底裁平面
		planes[VF_BOTTOM_PLANE].normal.x = mat.getIndex(3) + mat.getIndex(1);
		planes[VF_BOTTOM_PLANE].normal.y = mat.getIndex(7) + mat.getIndex(5);
		planes[VF_BOTTOM_PLANE].normal.z = mat.getIndex(11) + mat.getIndex(9);
		planes[VF_BOTTOM_PLANE].d = mat.getIndex(15) + mat.getIndex(13);

		// 远裁平面
		planes[VF_FAR_PLANE].normal.x = mat.getIndex(3) - mat.getIndex(2);
		planes[VF_FAR_PLANE].normal.y = mat.getIndex(7) - mat.getIndex(6);
		planes[VF_FAR_PLANE].normal.z = mat.getIndex(11) - mat.getIndex(10);
		planes[VF_FAR_PLANE].d = mat.getIndex(15) - mat.getIndex(14);

		// 近裁平面
		planes[VF_NEAR_PLANE].normal.x = mat.getIndex(2);
		planes[VF_NEAR_PLANE].normal.y = mat.getIndex(6);
		planes[VF_NEAR_PLANE].normal.z = mat.getIndex(10);
		planes[VF_NEAR_PLANE].d = mat.getIndex(14);

		// 标准化法线
		UINT32 i;
		for (i = 0; i != VF_PLANE_COUNT; ++i)
		{
			const Real len = -Q_rsqrtHW(
				planes[i].normal.squaredLength());
			planes[i].normal *= len;
			planes[i].d *= len;
		}

		// 重新计算包围体盒子
		recalculateBoundingBox();
	}

	/*!
	取得本视锥体的投影和观察矩阵
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
	取得本视锥体的投影和观察矩阵
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

	//!视锥体剪裁线段
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