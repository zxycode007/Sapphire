#ifndef _SAPPHIRE_IANIMATED_MESH_
#define _SAPPHIRE_IANIMATED_MESH_

#include "SapphirePrerequisites.h"
#include "SapphireIMesh.h"

namespace Sapphire
{
	//! ���ɻ�����������
	enum E_ANIMATED_MESH_TYPE
	{
		//! Unknown animated mesh type.
		EAMT_UNKNOWN = 0,

		//! Quake 2 MD2 model file
		EAMT_MD2,

		//! Quake 3 MD3 model file
		EAMT_MD3,

		//! Maya .obj static model
		EAMT_OBJ,

		//! Quake 3 .bsp static Map
		EAMT_BSP,

		//! 3D Studio .3ds file
		EAMT_3DS,

		//! My3D Mesh, the file format by Zhuck Dimitry
		EAMT_MY3D,

		//! Pulsar LMTools .lmts file. This Irrlicht loader was written by Jonas Petersen
		EAMT_LMTS,

		//! Cartography Shop .csm file. This loader was created by Saurav Mohapatra.
		EAMT_CSM,

		//! .oct file for Paul Nette's FSRad or from Murphy McCauley's Blender .oct exporter.
		/** The oct file format contains 3D geometry and lightmaps and
		can be loaded directly by Irrlicht */
		EAMT_OCT,

		//! Halflife MDL model file
		EAMT_MDL_HALFLIFE,

		//! generic skinned mesh
		EAMT_SKINNED
	};

	//! һ����������Ľӿ�
	/** �����Ѿ��ṩ����ӿ�һ���򵥵�ʵ�֡�ʹ��SAnimatedMesh��SMesh
	etc���㲻��Ҫʵ���Լ��Ľӿ�
	  */
	class IAnimatedMesh : public IMesh
	{
	public:

		//! ��ȡ��������������֡��
		/** \return ��֡�������֡����1������һ����̬�ģ�û�ж���������*/
		virtual UINT32 getFrameCount() const = 0;

		//! ��ȡ�����������Ķ����ٶ�
		/** \return Ĭ�ϲ����������ÿ���֡��. ���������0�����Ǿ�̬�ģ�û�ж��������� */
		virtual FLOAT32 getAnimationSpeed() const = 0;

		//! ���ö�������Ķ����ٶ�
		/** \param fps Ĭ�ϲ����������ÿ���֡��. ���������0���������á���ʵ���ٶ���
		�����ڵ������ʵ����������
		*/
		virtual void setAnimationSpeed(FLOAT32 fps) = 0;

		//! ����һ��֡IMesh�ӿ�
		/** \param frame: ����0������֡�������ֵ��getFrameCount() - 1
		\param detailLevel: LOD�ȼ���0����ͣ�255�����LOD���������������Ч���ϸ�ڵȼ���
		\param startFrameLoop: ����ĳЩ��������MD2����������̬֮֡����л�ϣ������ܶ�����
		��һ��ѭ�����棬��ʼ֡ѭ���ͽ���֡ѭ����Ҫ�����壬ȥ���ö������֡�������ѭ��֮�⡣
		�����ʼ֡�ͽ���֡����-1��������Ч��
		\param endFrameLoop: ����ʼ֡
		\return Returns �������һ��LOD�Ķ������� */
		virtual IMesh* getMesh(SINT32 frame, SINT32 detailLevel = 255, SINT32 startFrameLoop = -1, SINT32 endFrameLoop = -1) = 0;

		//! ��������������������
		/** ���������£�ʹ������������Ǳ�Ҫ�ġ�����Ҫȷ�����°�ȫ��
		����getMeshType()���� EAMT_MD2�������԰�ȫת��IAnimatedMesh��
		IAnimatedMeshMD2
		\returns �������� */
		virtual E_ANIMATED_MESH_TYPE getMeshType() const
		{
			return EAMT_UNKNOWN;
		}
	};
}

#endif
