#ifndef _SAPPHIRE_I_TERRIAN_SCENE_NODE_
#define _SAPPHIRE_I_TERRIAN_SCENE_NODE_

#include "SapphireETerrianElements.h"
#include "SapphirePrerequisites.h"
#include "SapphireIMesh.h"
#include "SapphireIDynamicMeshBuffer.h"
#include "SapphireISceneNode.h"


namespace Sapphire
{
	class IReadFile;

	class IMesh;

	//! һ����geo mipmap �㷨��ʾ���εĳ����ڵ�
	/**
	* ��������ڵ��ܹ��ǳ���ļ��ء���ʹ��һ��CLOD(Continous Level of Detail)�㷨�Ի���һ��LOD��ÿ��Patch����ͨ������patch������ľ�����ȷ������������
	  ������ε�patch�Ĵ�С������һ�� ( 2^N+1, i.e. 8+1(9), 16+1(17), etc. ).�Ĵ�С.
	  MaxLOD�Ƿ���Ч��������patch�Ĵ�С�� LOD 0�������е�����ȥ���ƶ���һ��patch���ϸ�������е������Ρ�����ÿ��LOD��step taken ����1���ڴ�������ʱ���� 2^LOD,���ԣ�
	  ����LOD1�� step taken ��2��  ����LOD 2�� step taken ��4�� LOD3 8.... step���ܴ���patch�Ĵ�С�����ԣ����ڴ�С17��patch�� LOD 8���㷨Ҫ��û2^8(256)�����㴴��
	  ����������patch ��С��������17�� ����patch��С����17����֧�ֵ����LOD �� 2^4(16). ���ԣ�����MaxLOD=5�� �����ʹ��LOD 0 �����ϸ�ڣ���LOD 1��ÿ�������㣩��LOD 2 (ÿ4������)
	  ,LOD 3 ��ÿ8�����㣩 ���� LOD 4 ��ÿ16�����㣩
	**/
	class ITerrainSceneNode : public ISceneNode
	{
	public:
		 
		ITerrainSceneNode(ISceneNode* parent, ISceneManager* mgr, SINT32 id,
			const Vector3& position =Vector3(0.0f, 0.0f, 0.0f),
			const Vector3& rotation =Vector3(0.0f, 0.0f, 0.0f),
			const Vector3& scale =Vector3(1.0f, 1.0f, 1.0f))
			: ISceneNode(parent, mgr, id, position, rotation, scale) {}

		//! ��ȡ���εİ�Χ�����
		/** \return ���εİ�Χ����� */
		virtual const AxisAlignedBox& getBoundingBox() const = 0;

		//! ��ȡ���εİ�Χ�����
		/** \return ���εİ�Χ����� */
		virtual const AxisAlignedBox& getBoundingBox(SINT32 patchX, SINT32 patchZ) const = 0;

		//! ��ȡ��ǰ���񻺳�����������
		/** \return ������ */
		virtual UINT32 getIndexCount() const = 0;

		//! ��ȡ����ָ��
		/** \return ����ָ�� */
		virtual IMesh* getMesh() = 0;

		//! ��ȡָ�����ڵ��λ�����ָ�루����ʱ����Ҫ��
		virtual IMeshBuffer* getRenderBuffer() = 0;


		//! ��ȡ����һ��ָ��LOD�����񻺳�������
		/** \param mb һ��IDynamicMeshBuffer���������
		\param LOD Ҫָ����LOD*/
		virtual void getMeshBufferForLOD(IDynamicMeshBuffer& mb, SINT32 LOD = 0) const = 0;

		//! ��ȡһ����ָ��LOD�µ�һ��ָ��patch������
		/** \param indices һ��UINT32 �������������
		\param patchX Patch x ����.
		\param patchZ Patch z y����.
		\param LOD �������Patch��LOD�����Ϊ-1����ȡ��ǰLOD�� �����ǰLOD����Ϊ-1,��ζ����������ʾ����ô�����ظ���һ��LOD ��LOD 0��
		��������
		\return ��������������������� */
		virtual SINT32 getIndicesForPatch(std::vector<UINT32>& indices,
			SINT32 patchX, SINT32 patchZ, SINT32 LOD = 0) = 0;

		//! ���ÿ��patch�ĵ�ǰLOD��һ������
		/** \param LODs һ��������Щֵ������
		\return ����Ԫ�ظ��� */
		virtual SINT32 getCurrentLODOfPatches(std::vector<SINT32>& LODs) const = 0;

		//! �ֶ�����һ��patch��LOD
		/** \param patchX patch��x����
		\param patchZ patch��z����
		\param LOD ���patch��LOD�ȼ�*/
		virtual void setLODOfPatch(SINT32 patchX, SINT32 patchZ, SINT32 LOD = 0) = 0;

		//! ��ȡ��������
		virtual const Vector3& getTerrainCenter() const = 0;

		//! ��ȡ���θ߶�
		virtual Real getHeight(Real x, Real y) const = 0;

		//! ��������ƶ���ֵ
		/** ������ȷ�����¼��㳡���ڵ�����������ֵĬ��10.0f */
		virtual void setCameraMovementDelta(Real delta) = 0;

		//! ���������ת��ֵ
		/** ������ȷ�����¼��㳡���ڵ�����������ֵĬ��10.0f */
		virtual void setCameraRotationDelta(Real delta) = 0;

		//! ���õ�geomipmap���ݸı䣬����ڵ��Ƿ���Ҫ��̬��������ص�selector
		/** \param bVal: ����ֵ��ʾ�Ƿ���Ҫ��̬����selector*/
		virtual void setDynamicSelectorUpdate(bool bVal) = 0;

		//! ����Ĭ�ϴ����ľ��뷧ֵ
		/** ����ȷ��һ������Ⱦpatch��LOD�� �������LOD�����ǣ���ô��������ڵ㲻�ٶ���ЩֵӦ���������ӡ�
		����㸲����Щ���룬���������������ӵ�����ڵ㣬 ����Ҫ�������ŵĵ��δ�Сһ������µľ���
		  */
		virtual bool overrideLODDistance(SINT32 LOD, Real newDistance) = 0;

		//! ���Ż�����������makePlanarTextureMapping 
		/** \param scale  ��������ֵ����1��0���������ڵ����ϻ��������ʱ�䡣ֵС��0��������ڵ����ϻ�����������硣
		�ø�ֵ�ᷭת����������Ȼ��������
		\param scale2 �������Ϊ0(Ĭ��), �⽫���õڶ��������������һ����������ͬ����ֵ�������0ֵ���������ŵڶ�����������*/
		virtual void scaleTexture(Real scale = 1.0f, Real scale2 = 0.0f) = 0;

		//! ��ʼ���������ݣ��Ӹ߳�ͼ�ļ��м��ض���
		/** ����ļ��������һ���ɼ��صĸ߳�ͼͼ������߳�ͼ������������
		\param file Ҫ���ص�ͼ���ļ��� 
		\param vertexColor ���ж�����ɫ
		\param smoothFactor ƽ������ */
		virtual bool loadHeightMap(IReadFile* file,
			//ColourValue vertexColor = ColourValue(1.0, 1.0, 1.0, 1.0),
			ColourValue vertexColor = ColourValue::getColourValue(255, 255, 255, 255),
			SINT32 smoothFactor = 0) = 0;

		//! ��ʼ���������ݣ��Ӹ߳�ͼ�ļ��м��ض���
		/** �����ݽ��Ը���λ��С�����Σ��з��ţ��򸡵�����32λ�з��ţ����в�ֵ.����Ϊ���ε�λ��С������8��16��32.
		�߳�ͼ������������
		\param file Ҫ��ȡRAW���ݵ��ļ�
		\param bitsPerPixel û�����ص�����λ��С�����ڸ�������32λ
		\param signedData ʹ��uint����int����float��Ч
		\param floatVals ʹ��int����float����
		\param width �߳�ͼ�Ŀ�ȣ��͸߶���ȣ�����0�Ļ����ļ���С�Զ�����
		\param vertexColor ���ж������ɫ
		\param smoothFactor ƽ������  */
		virtual bool loadHeightMapRAW(IReadFile* file, SINT32 bitsPerPixel = 16,
			bool signedData = false, bool floatVals = false, SINT32 width = 0,
			//ColourValue vertexColor = ColourValue(1.0, 1.0, 1.0, 1.0),
			ColourValue vertexColor = ColourValue::getColourValue(255, 255, 255, 255),
			SINT32 smoothFactor = 0) = 0;

	};
}

#endif