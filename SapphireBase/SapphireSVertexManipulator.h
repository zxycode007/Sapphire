#ifndef _SAPPHIRE_SVERTEX_MANIPULATOR_
#define _SAPPHIRE_SVERTEX_MANIPULATOR_

#include "SapphirePrerequisites.h"
#include "SapphireVector3.h"
#include "SapphireVector2.h"
#include "SapphireColorValue.h"
#include "SapphireSVertex.h"
#include "SapphireMatrix4.h"

namespace Sapphire
{
	class IMesh;
	class IMeshBuffer;
	struct SMesh;

	//! 顶点操作器的接口
	/** 如果要对每个顶点调用，你的操作器需要从这个类派生，获取只是作为这个顶点的参数
	*/
	struct IVertexManipulator
	{
	};
	//! 用顶点操作器对所有顶点设置颜色到一个修正的颜色
	class SVertexColorSetManipulator : public IVertexManipulator
	{
	public:
		SVertexColorSetManipulator(ColourValue color) : Color(color) {}
		void operator()(S3DVertex& vertex) const
		{
			vertex.Color = Color;
		}
	private:
		ColourValue Color;
	};
	//! 顶点操作器设置顶点颜色的alpha的颜色到一个修正的值
	class SVertexColorSetAlphaManipulator : public IVertexManipulator
	{
	public:
		SVertexColorSetAlphaManipulator(UINT32 alpha) : Alpha(alpha) {}
		void operator()(S3DVertex& vertex) const
		{
			vertex.Color.setAlpha(Alpha);
		}
	private:
		UINT32 Alpha;
	};
	//! 翻转RGB值的顶点操作器
	class SVertexColorInvertManipulator : public IVertexManipulator
	{
	public:
		void operator()(S3DVertex& vertex) const
		{
			vertex.Color.setRed(255 - vertex.Color.getRed());
			vertex.Color.setGreen(255 - vertex.Color.getGreen());
			vertex.Color.setBlue(255 - vertex.Color.getBlue());
		}
	};
	//! 顶点操作器设置顶点颜色到依赖一个给定阀值的两个值的一个
	/** 如果这个颜色的平均值>阀值，高颜色被选中，否则选低*/
	class SVertexColorThresholdManipulator : public IVertexManipulator
	{
	public:
		SVertexColorThresholdManipulator(UINT8 threshold, ColourValue low,
			ColourValue high) : Threshold(threshold), Low(low), High(high) {}
		void operator()(S3DVertex& vertex) const
		{
			vertex.Color = ((UINT8)vertex.Color.getAverage()>Threshold) ? High : Low;
		}
	private:
		UINT8 Threshold;
		ColourValue Low;
		ColourValue High;
	};
	//! 可以通过给定的量调整亮度的顶点操作器
	/** 一增加亮度的正值，一个降低色彩亮度的负值*/
	class SVertexColorBrightnessManipulator : public IVertexManipulator
	{
	public:
		SVertexColorBrightnessManipulator(SINT32 amount) : Amount(amount) {}
		void operator()(S3DVertex& vertex) const
		{
			vertex.Color.setRed(Math::Clamp(vertex.Color.getRed() + Amount, 0u, 255u));
			vertex.Color.setGreen(Math::Clamp(vertex.Color.getGreen() + Amount, 0u, 255u));
			vertex.Color.setBlue(Math::Clamp(vertex.Color.getBlue() + Amount, 0u, 255u));
		}
	private:
		SINT32 Amount;
	};
	//! 能够通过给定因子调节对比度的顶点操作器
	/** 因子超过1提高对比度，降低1减少对比度 */
	class SVertexColorContrastManipulator : public IVertexManipulator
	{
	public:
		SVertexColorContrastManipulator(Real factor) : Factor(factor) {}
		void operator()(S3DVertex& vertex) const
		{
			vertex.Color.setRed(Math::Clamp(Math::round2i((vertex.Color.getRed() - 128)*Factor) + 128, 0, 255));
			vertex.Color.setGreen(Math::Clamp(Math::round2i((vertex.Color.getGreen() - 128)*Factor) + 128, 0, 255));
			vertex.Color.setBlue(Math::Clamp(Math::round2i((vertex.Color.getBlue() - 128)*Factor) + 128, 0, 255));
		}
	private:
		Real Factor;
	};
	//! 通过一个给定因子调节对比度，和一个给定的符号数调节亮度的顶点操作器
	class SVertexColorContrastBrightnessManipulator : public IVertexManipulator
	{
	public:
		SVertexColorContrastBrightnessManipulator(Real factor, SINT32 amount) : Factor(factor), Amount(amount + 128) {}
		void operator()(S3DVertex& vertex) const
		{
			vertex.Color.setRed(Math::Clamp(Math::round2i((vertex.Color.getRed() - 128)*Factor) + Amount, 0, 255));
			vertex.Color.setGreen(Math::Clamp(Math::round2i((vertex.Color.getGreen() - 128)*Factor) + Amount, 0, 255));
			vertex.Color.setBlue(Math::Clamp(Math::round2i((vertex.Color.getBlue() - 128)*Factor) + Amount, 0, 255));
		}
	private:
		Real Factor;
		SINT32 Amount;
	};
	//! 通过gamma操作调节亮度的顶点操作器
	/** 超过1增加亮度，低于1减少亮度*/
	class SVertexColorGammaManipulator : public IVertexManipulator
	{
	public:
		SVertexColorGammaManipulator(Real gamma) : Gamma(1.f)
		{
			if (gamma != 0.f)
				Gamma = 1.f / gamma;
		}
		void operator()(S3DVertex& vertex) const
		{
			vertex.Color.setRed(Math::Clamp(Math::round2i(powf((Real)(vertex.Color.getRed()), Gamma)), 0, 255));
			vertex.Color.setGreen(Math::Clamp(Math::round2i(powf((Real)(vertex.Color.getGreen()), Gamma)), 0, 255));
			vertex.Color.setBlue(Math::Clamp(Math::round2i(powf((Real)(vertex.Color.getBlue()), Gamma)), 0, 255));
		}
	private:
		Real Gamma;
	};
	//! 缩放颜色值的顶点操作器
	/** 能够用于白平衡，因子作为255/最亮颜色的因子*/
	class SVertexColorScaleManipulator : public IVertexManipulator
	{
	public:
		SVertexColorScaleManipulator(Real factor) : Factor(factor) {}
		void operator()(S3DVertex& vertex) const
		{
			vertex.Color.setRed(Math::Clamp(Math::round2i(vertex.Color.getRed()*Factor), 0, 255));
			vertex.Color.setGreen(Math::Clamp(Math::round2i(vertex.Color.getGreen()*Factor), 0, 255));
			vertex.Color.setBlue(Math::Clamp(Math::round2i(vertex.Color.getBlue()*Factor), 0, 255));
		}
	private:
		Real Factor;
	};
	//! 能降低颜色饱和度的顶点操作器
	/** 使用颜色的亮度值*/
	class SVertexColorDesaturateToLightnessManipulator : public IVertexManipulator
	{
	public:
		void operator()(S3DVertex& vertex) const
		{
			vertex.Color = vertex.Color.getLightness();
		}
	};
	//! 能降低颜色饱和度的顶点操作器
	/** 使用颜色的平均值*/
	class SVertexColorDesaturateToAverageManipulator : public IVertexManipulator
	{
	public:
		void operator()(S3DVertex& vertex) const
		{
			vertex.Color = vertex.Color.getAverage();
		}
	};
	//! 能降低颜色值的饱和度的顶点操作器
	/** 使用颜色的明度*/
	class SVertexColorDesaturateToLuminanceManipulator : public IVertexManipulator
	{
	public:
		void operator()(S3DVertex& vertex) const
		{
			vertex.Color = vertex.Color.getLuminance();
		}
	};
	//! 能对颜色值进行插值的顶点操作器
	/** 使用线性插值. */
	class SVertexColorInterpolateLinearManipulator : public IVertexManipulator
	{
	public:
		SVertexColorInterpolateLinearManipulator(ColourValue color, Real factor) :
			Color(color), Factor(factor) {}
		void operator()(S3DVertex& vertex) const
		{
			vertex.Color = vertex.Color.getInterpolated(Color, Factor);
		}
	private:
		ColourValue Color;
		Real Factor;
	};
	//!能对颜色值进行插值的顶点操作器
	/**  使用线性插值. */
	class SVertexColorInterpolateQuadraticManipulator : public IVertexManipulator
	{
	public:
		SVertexColorInterpolateQuadraticManipulator(ColourValue color1, ColourValue color2, Real factor) :
			Color1(color1), Color2(color2), Factor(factor) {}
		void operator()(S3DVertex& vertex) const
		{
			vertex.Color = vertex.Color.getInterpolated_quadratic(Color1, Color2, Factor);
		}
	private:
		ColourValue Color1;
		ColourValue Color2;
		Real Factor;
	};

	//! 能对顶点位置进行缩放的顶点操作器
	class SVertexPositionScaleManipulator : public IVertexManipulator
	{
	public:
		SVertexPositionScaleManipulator(const Vector3& factor) : Factor(factor) {}
		template <typename VType>
		void operator()(VType& vertex) const
		{
			vertex.Pos *= Factor;
		}
	private:
		Vector3 Factor;
	};

	//! 能沿法线对顶点位置进行缩放的顶点操作器
	class SVertexPositionScaleAlongNormalsManipulator : public IVertexManipulator
	{
	public:
		SVertexPositionScaleAlongNormalsManipulator(const Vector3& factor) : Factor(factor) {}
		template <typename VType>
		void operator()(VType& vertex) const
		{
			vertex.Pos += vertex.Normal*Factor;
		}
	private:
		Vector3 Factor;
	};

	//! 能对顶点位置进行变换的顶点操作器
	class SVertexPositionTransformManipulator : public IVertexManipulator
	{
	public:
		SVertexPositionTransformManipulator(const Matrix4& m) : Transformation(m) {}
		template <typename VType>
		void operator()(VType& vertex) const
		{
			Transformation.transformVect(vertex.Pos);
		}
	private:
		Matrix4 Transformation;
	};

	//! 能对顶点纹理坐标进行缩放的顶点操作器
	class SVertexTCoordsScaleManipulator : public IVertexManipulator
	{
	public:
		SVertexTCoordsScaleManipulator(const Vector2& factor, UINT32 uvSet = 1) : Factor(factor), UVSet(uvSet) {}
		void operator()(S3DVertex2TCoords& vertex) const
		{
			if (1 == UVSet)
				vertex.TCoords *= Factor;
			else if (2 == UVSet)
				vertex.TCoords2 *= Factor;
		}
		template <typename VType>
		void operator()(VType& vertex) const
		{
			if (1 == UVSet)
				vertex.TCoords *= Factor;
		}
	private:
		Vector2 Factor;
		UINT32 UVSet;
	};
}

#endif