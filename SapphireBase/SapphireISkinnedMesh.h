#ifndef _SAPPHIRE_I_SKINNED_MESH_
#define _SAPPHIRE_I_SKINNED_MESH_

#include "SapphirePrerequisites.h"
#include "SapphireVector3.h"
#include "SapphireIAnimatedMesh.h"
#include "SapphireSSkinMeshBuffer.h"

namespace Sapphire
{
	enum E_INTERPOLATION_MODE
	{
		// ʹ�ó���������ֵ 
		EIM_CONSTANT = 0,

		// ���Բ�ֵ
		EIM_LINEAR,

		
		EIM_COUNT
	};


	//! ��Ƥ�����һЩ���⺯���Ľӿ�
	class ISkinnedMesh : public IAnimatedMesh
	{
	public:

		//! ��ȡjoint����
		/** \return �������������joint����*/
		virtual UINT32 getJointCount() const = 0;

		//! ��ȡһ��joint������
		/** \param number: joint���� ������0��ʼ����С�� getJointCount()
		\return joint�����֣�����������󣬷��ؿ�*/
		virtual const c8* getJointName(UINT32 number) const = 0;

		//! ͨ��joint���ֻ�ȡ�����ĺ���
		/** \param name: joint������
		\return ����joint�ĺ��������û�ҵ�����-1 */
		virtual SINT32 getJointNumber(const c8* name) const = 0;

		//! �ӱ������ʹ�ö���
		/** �����������joint�������ӣ����ǿ�����
		����ȷ��������Ψһ��
		\return ��������������joint����ƥ���ϣ�����true,����ƥ���joint���ᱻ����
	    */
		virtual bool useAnimationFrom(const ISkinnedMesh *mesh) = 0;

		//! �����ж���ʱ���·���
		/** \param on �Ƿ���·��� */
		virtual void updateNormalsWhenAnimating(bool on) = 0;

		//! ���ò�ֵģʽ
		virtual void setInterpolationMode(E_INTERPOLATION_MODE mode) = 0;

		//! ��������֡�������������joint�Ķ���
		virtual void animateMesh(Real frame, Real blend) = 0;

		//! ����jointλ��Ӧ��һ�����Ƥ�������������
		virtual void skinMesh() = 0;

		//! ת���������񻺳����Ķ������͵�EVT_TANGENTS
		/** ���ڰ�͹ӳ��. */
		virtual void convertMeshToTangents() = 0;

		//! �����Ӳ��Ƥ��
		/* ���������û��ʵ��*/
		virtual bool setHardwareSkinning(bool on) = 0;

		//! ����Ȩ��
		struct SWeight
		{
			//! ���㻺����������
			UINT32 buffer_id; 

			//! ���������
			UINT32 vertex_id; //�������Ϊȫ��ID

			//! Ȩ��   ǿ��/�ٷֱ� ��0-1��
			Real strength;

		private:
			//!  ����CSkinnedMesh���ڲ�����
			friend class CSkinnedMesh;
			bool *Moved;
			Vector3 StaticPos;
			Vector3 StaticNormal;
		};


		//! ����һ����λ�õĶ����ؼ�֡
		struct SPositionKey
		{
			Real frame;
			Vector3 position;
		};

		//! ����һ�������ŵĶ����ؼ�֡
		struct SScaleKey
		{
			Real frame;
			Vector3 scale;
		};

		//! ����һ������ת�Ķ����ؼ�֡
		struct SRotationKey
		{
			Real frame;
			Quaternion rotation;
		};

		//! Joints
		struct SJoint
		{
			SJoint() : UseAnimationFrom(0), GlobalSkinningSpace(false),
				positionHint(-1), scaleHint(-1), rotationHint(-1)
			{
			}

			//! ���Joint������
			String Name;

			//! ���joint�ı��ؾ���
			Matrix4 LocalMatrix;

			//! ��joint�б�
			vector<SJoint*>::type Children;

			//! ���������б�
			vector<UINT32>::type AttachedMeshes;

			//! ���λ�øı�Ĺؼ�֡
			deque<SPositionKey>::type PositionKeys;

			//! ������Ÿı�Ĺؼ���֡
			deque<SScaleKey>::type ScaleKeys;

			//! �����ת�ı�Ĺؼ���
			deque<SRotationKey>::type RotationKeys;

			//! Ƥ��Ȩ��
			vector<SWeight>::type Weights;

			//! ���ڼ��������Ǳ���ģ����ջᱻ����д
			Matrix4 GlobalMatrix;
			Matrix4 GlobalAnimatedMatrix;
			Matrix4 LocalAnimatedMatrix;
			Vector3 Animatedposition;
			Vector3 Animatedscale;
			Quaternion Animatedrotation;

			Matrix4 GlobalInversedMatrix;  

		private:
			//! CSkinnedMesh �ڲ�ʹ��
			friend class CSkinnedMesh;

			SJoint *UseAnimationFrom;
			bool GlobalSkinningSpace;

			SINT32 positionHint;
			SINT32 scaleHint;
			SINT32 rotationHint;
		};


	 

		//! �������������������񻺳���
		virtual vector<SSkinMeshBuffer*>::type& getMeshBuffers() = 0;

		//! ������������� joints�б�
		virtual vector<SJoint*>::type& getAllJoints() = 0;

		//! ������������� joints �б�
		virtual const vector<SJoint*>::type& getAllJoints() const = 0;

		//! ���������������֮�����
		virtual void finalize() = 0;

		//! ���һ�������񻺳��������������Ϊ���һ��������
		virtual SSkinMeshBuffer* addMeshBuffer() = 0;

		//! ���һ����joint�����������Ϊ���һ��������
		virtual SJoint* addJoint(SJoint *parent = 0) = 0;

		//! ���һ����Ȩ�ظ����������Ϊ���һ��������
		virtual SWeight* addWeight(SJoint *joint) = 0;

		//! ���һ����λ�ü������������Ϊ���һ�������� 
		virtual SPositionKey* addPositionKey(SJoint *joint) = 0;
		//! ���һ�������ż������������Ϊ���һ��������
		virtual SScaleKey* addScaleKey(SJoint *joint) = 0;
		//! ���һ������ת�������������Ϊ���һ��������
		virtual SRotationKey* addRotationKey(SJoint *joint) = 0;

		//! �����������Ƿ��ж���
		virtual bool isStatic() = 0;
	};
}

#endif