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

	//! ����������Ľӿ�
	/** ���Ҫ��ÿ��������ã���Ĳ�������Ҫ���������������ȡֻ����Ϊ�������Ĳ���
	*/
	struct IVertexManipulator
	{
	};
	//! �ö�������������ж���������ɫ��һ����������ɫ
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
	//! ������������ö�����ɫ��alpha����ɫ��һ��������ֵ
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
	//! ��תRGBֵ�Ķ��������
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
	//! ������������ö�����ɫ������һ��������ֵ������ֵ��һ��
	/** ��������ɫ��ƽ��ֵ>��ֵ������ɫ��ѡ�У�����ѡ��*/
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
	//! ����ͨ�����������������ȵĶ��������
	/** һ�������ȵ���ֵ��һ������ɫ�����ȵĸ�ֵ*/
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
	//! �ܹ�ͨ���������ӵ��ڶԱȶȵĶ��������
	/** ���ӳ���1��߶Աȶȣ�����1���ٶԱȶ� */
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
	//! ͨ��һ���������ӵ��ڶԱȶȣ���һ�������ķ������������ȵĶ��������
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
	//! ͨ��gamma�����������ȵĶ��������
	/** ����1�������ȣ�����1��������*/
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
	//! ������ɫֵ�Ķ��������
	/** �ܹ����ڰ�ƽ�⣬������Ϊ255/������ɫ������*/
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
	//! �ܽ�����ɫ���ͶȵĶ��������
	/** ʹ����ɫ������ֵ*/
	class SVertexColorDesaturateToLightnessManipulator : public IVertexManipulator
	{
	public:
		void operator()(S3DVertex& vertex) const
		{
			vertex.Color = vertex.Color.getLightness();
		}
	};
	//! �ܽ�����ɫ���ͶȵĶ��������
	/** ʹ����ɫ��ƽ��ֵ*/
	class SVertexColorDesaturateToAverageManipulator : public IVertexManipulator
	{
	public:
		void operator()(S3DVertex& vertex) const
		{
			vertex.Color = vertex.Color.getAverage();
		}
	};
	//! �ܽ�����ɫֵ�ı��ͶȵĶ��������
	/** ʹ����ɫ������*/
	class SVertexColorDesaturateToLuminanceManipulator : public IVertexManipulator
	{
	public:
		void operator()(S3DVertex& vertex) const
		{
			vertex.Color = vertex.Color.getLuminance();
		}
	};
	//! �ܶ���ɫֵ���в�ֵ�Ķ��������
	/** ʹ�����Բ�ֵ. */
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
	//!�ܶ���ɫֵ���в�ֵ�Ķ��������
	/**  ʹ�����Բ�ֵ. */
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

	//! �ܶԶ���λ�ý������ŵĶ��������
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

	//! ���ط��߶Զ���λ�ý������ŵĶ��������
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

	//! �ܶԶ���λ�ý��б任�Ķ��������
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

	//! �ܶԶ�����������������ŵĶ��������
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