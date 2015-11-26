#ifndef _SAPPHIRE_EMATERIAL_TYPES_
#define _SAPPHIRE_EMATERIAL_TYPES_

namespace Sapphire
{

	//! ����Ĳ�������ʹ�ù̶�/�ɱ�̹��� ���ʷ�ʽ
	enum E_MATERIAL_TYPE
	{
		//! ��׼�������
		/** ֻ��ʹ�õ�һ������ ���ٶ�Ϊ���������*/
		EMT_SOLID = 0,

		//! �����������Ĺ������
		/** �ڶ������ö�����ɫ��alphaֵ��ϵ���һ�����档���������OpenGL�в��ܱ�֧��
		*/
		EMT_SOLID_2_LAYER,

		//! ��׼������ͼ�Ĳ�������
		/** ��������������һ���������һ�������������ڶ�����һ������ͼ�� ��̬��Դ��Ч */
		EMT_LIGHTMAP,

		//! ����EMT_LIGHTMAP�Ĺ�����ͼ�Ĳ��ʼ���
		/** ���ǹ���ͼ����������������add����modulated*/
		EMT_LIGHTMAP_ADD,

		//! ��׼����ͼ�Ĳ�������
		/** ��������������һ����һ��������ͼ�� �ڶ�����һ������ͼ����̬��Դ��Ч��
		���������ɫ���ȸ�Ч��2��������DirectX as D3DTOP_MODULATE2X.
		 */
		EMT_LIGHTMAP_M2,

		//! ��׼����ͼ�Ĳ�������
		/** ��������������һ����һ��������ͼ�� �ڶ�����һ������ͼ����̬��Դ��Ч��
		���������ɫ���ȸ�Ч��4��������DirectX as D3DTOP_MODULATE4X. */
		EMT_LIGHTMAP_M4,

		//! ����EMT_LIGHTMAP, ��֧�ֶ�̬��Դ
		EMT_LIGHTMAP_LIGHTING,

		//! ����EMT_LIGHTMAP_M2, ��֧�ֶ�̬��Դ
		EMT_LIGHTMAP_LIGHTING_M2,

		//! ����EMT_LIGHTMAP_M4, ��֧�ֶ�̬��Դ
		EMT_LIGHTMAP_LIGHTING_M4,

		//! ϸ��ӳ�����
		/** ��һ����������������ɫ��ͼ�� �ڶ�����ӵĲ���ͨ����ʾһ�����������ֵ������ʾ����ϸ�ڡ�
		���ϸ����ͼ�������������ͼ����ADD_SIGNED�� ���ԣ���������Ӳ��Ҵ���������ͼ��ȥ��ɫ�����磺
		һ��ֵ��127��127��127������ı���������ͼ��������ۣ��������ڵر���Ⱦ
		 */
		EMT_DETAIL_MAP,

		//! �����������Ƿ�����Χ������������
		/** Ҫʹ����Ϊ���ܣ� Ҫ�õ�һ����'������ͼ'����������������Ϊ��һ������
		  */
		EMT_SPHERE_MAP,

		//! һ���ÿ�ѡ�ķǷ��������ķ������
		/** ���������ͼӦ��������һ������ */
		EMT_REFLECTION_2_LAYER,

		//! һ��͸������
		/** ֻ����Ϊ��һ������ʹ�á� �������ɫͨ����Դ��ɫ����Ŀ����ɫ���㡣
		����ζ�����һ��billboard��һ������ɫ�����ͺ�ɫԲ������������������ʻ��ƵĽ����
		ֻ�к�ɫԲ�������ƣ��������к�ɫ�Ķ�������100%͸���Ͳ��ɼ�������������Ͷ�������Ч���ǳ�����
		  */
		EMT_TRANSPARENT_ADD_COLOR,

		//! ʹ����͸�����������alphaͨ��
		/** ������ɫ��Ŀ����ɫ��������ɫ��϶����� ��alphaͨ��ֵ��Ϊ������ӡ�ֻʹ�õ�һ������
		����������������С��������32λģʽ���������Ǹ�������
		( IVideoDriver::setTextureCreationFlag()). ���⣬һ��alpha���ñ�ʹ�ã� ���ܹ�ʹ��SMaterial::MaterialTypeParam����.
		�����������͸���任��һ������ʱ�����ֵ���Ʊ�Ե��������
		  */
		EMT_TRANSPARENT_ALPHA_CHANNEL,

		//! ʹ����͸�����������alphaͨ��
		/** ���alphaͨ��ֵ����127��һ������д��Ŀ�꣬ ����д�롣 
		������ʲ���alpha��ϲ��Ҵ�����EMT_TRANSPARENT_ALPHA_CHANNEL
		 �����뷨�ǻ���������ֲ���Ҷ�ӣ� ��˱߽粻ģ��������������
		 ֻʹ�õ�һ��������������������ʹ��С�����3d��������32λģʽ���������Ǹ�������
	    (IVideoDriver::setTextureCreationFlag()). */
		EMT_TRANSPARENT_ALPHA_CHANNEL_REF,

		//! ���ڶ����alphaֵʹ����͸��
		EMT_TRANSPARENT_VERTEX_ALPHA,

		//! һ����͸��������ʣ���ѡ���ӷǷ��������
		/** 
		���������ͼӦ������Ϊ��һ���������͸����������alphaֵ������ɫ�еġ�
		��������ֻ������Ϊ��һ��������ע������������͵�ǰ����100%����OpenGL
		  */
		EMT_TRANSPARENT_REFLECTION_2_LAYER,

		//! һ�����巨����ͼ��Ⱦ��
		/** ��һ����������ɫ��ͼ���ڶ����Ƿ�����ͼ��
		ע�⣺��Ӧ��ֻ�ڻ���S3DVertexTangents (EVT_TANGENTS)���Ͷ��㹹�ɵļ�������ʹ�������
		������IMeshManipulator::createMeshWithTangents()ת���κ����������ʽ����irr SpecialFX2�̳�
		 ���shaderֻ֧��������Դ������и���ģ�ֻȡ���������
		 */
		EMT_NORMAL_MAP_SOLID,

		//! һ��͸��������ͼ��Ⱦ��
		/** ��һ����������ɫ��ͼ���ڶ����Ƿ�����ͼ��
		ע�⣺��Ӧ��ֻ�ڻ���S3DVertexTangents (EVT_TANGENTS)���Ͷ��㹹�ɵļ�������ʹ������ʡ�
		������IMeshManipulator::createMeshWithTangents()ת���κ����������ʽ����irr SpecialFX2�̳�
		 ���shaderֻ֧��������Դ������и���ģ�ֻȡ��������� */
		EMT_NORMAL_MAP_TRANSPARENT_ADD_COLOR,

		//! һ��͸��������ͼ��Ⱦ�������ڶ���alphaֵ�� 
		/** ��һ����������ɫ��ͼ���ڶ����Ƿ�����ͼ��
		ע�⣺��Ӧ��ֻ�ڻ���S3DVertexTangents (EVT_TANGENTS)���Ͷ��㹹�ɵļ�������ʹ������ʡ�
		������IMeshManipulator::createMeshWithTangents()ת���κ����������ʽ����irr SpecialFX2�̳�
		���shaderֻ֧��������Դ������и���ģ�ֻȡ���������  */
		EMT_NORMAL_MAP_TRANSPARENT_VERTEX_ALPHA,

		//! �����Ӳ���ͼ ������EMT_NORMAL_MAP_SOLID, ����ʹ���Ӳ�ӳ�䡣
		/** ������������ʵ ��һ����������ɫ��ͼ���ڶ����Ƿ�����ͼ��������ͼ����Ӧ�ð�����alpha�����еĸ߶�ֵ��
		��һ��32Ϊ�����һ���߶�ͼ�д���������ͼʱ��IVideoDriver::makeNormalMapTexture()�����Զ�д�����ֵ��
		������ʵĸ߶�����ֵ(Ӱ�찼͹��)��ͨ��SMaterial::MaterialTypeParam��Ա�����Ƶġ��������Ϊ0����ʹ��Ĭ��ֵ0.02��
		�������ֵ SMaterial::MaterialTypeParam�����á����ֵ��������������������ŵ�����̫�߻�̫�͵�MaterialTypeParam
		�ᵼ�³�����ֵ�����
		 */
		EMT_PARALLAX_MAP_SOLID,

		//! һ����EMT_PARALLAX_MAP_SOLID�Ĳ���, ����͸��
		/** ʹ��EMT_TRANSPARENT_ADD_COLOR��Ϊ��������. */
		EMT_PARALLAX_MAP_TRANSPARENT_ADD_COLOR,

		//! һ����EMT_PARALLAX_MAP_SOLID�Ĳ���, ����͸��
		/** ʹ��EMT_TRANSPARENT_VERTEX_ALPHA��Ϊ��������. */
		EMT_PARALLAX_MAP_TRANSPARENT_VERTEX_ALPHA,

		//! ��Ϻ��� = source * sourceFactor + dest * destFactor ( E_BLEND_FUNC )
		/** ֻ�����ڵ�һ������ͨ�õĻ�Ϸ��� */
		EMT_ONETEXTURE_BLEND,

		//! ���ֵ��ʹ�á� ��ǿ��ö�����ͱ���Ϊ32λ
		EMT_FORCE_32BIT = 0x7fffffff
	};

	//! ��Ӧ�Ĳ���������
	const char* const sBuiltInMaterialTypeNames[] =
	{
		"solid",
		"solid_2layer",
		"lightmap",
		"lightmap_add",
		"lightmap_m2",
		"lightmap_m4",
		"lightmap_light",
		"lightmap_light_m2",
		"lightmap_light_m4",
		"detail_map",
		"sphere_map",
		"reflection_2layer",
		"trans_add",
		"trans_alphach",
		"trans_alphach_ref",
		"trans_vertex_alpha",
		"trans_reflection_2layer",
		"normalmap_solid",
		"normalmap_trans_add",
		"normalmap_trans_vertexalpha",
		"parallaxmap_solid",
		"parallaxmap_trans_add",
		"parallaxmap_trans_vertexalpha",
		"onetexture_blend",
		0
	};

}

#endif