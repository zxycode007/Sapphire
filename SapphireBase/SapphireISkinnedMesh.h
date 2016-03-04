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
		// 使用常量，不插值 
		EIM_CONSTANT = 0,

		// 线性插值
		EIM_LINEAR,

		
		EIM_COUNT
	};


	//! 蒙皮网格的一些特殊函数的接口
	class ISkinnedMesh : public IAnimatedMesh
	{
	public:

		//! 获取joint数量
		/** \return 骨骼动画网格的joint数量*/
		virtual UINT32 getJointCount() const = 0;

		//! 获取一个joint的名字
		/** \param number: joint索引 （基于0开始），小于 getJointCount()
		\return joint的名字，如果发生错误，返回空*/
		virtual const c8* getJointName(UINT32 number) const = 0;

		//! 通过joint名字获取的它的号数
		/** \param name: joint的名字
		\return 返回joint的号数，如果没找到返回-1 */
		virtual SINT32 getJointNumber(const c8* name) const = 0;

		//! 从别的网格使用动画
		/** 这个动画基于joint名字链接（不是拷贝）
		所以确定它们是唯一的
		\return 如果这个网格所有joint都能匹配上，返回true,不能匹配的joint不会被动画
	    */
		virtual bool useAnimationFrom(const ISkinnedMesh *mesh) = 0;

		//! 当进行动画时更新法线
		/** \param on 是否更新法线 */
		virtual void updateNormalsWhenAnimating(bool on) = 0;

		//! 设置插值模式
		virtual void setInterpolationMode(E_INTERPOLATION_MODE mode) = 0;

		//! 基于输入帧来播放这个网格joint的动画
		virtual void animateMesh(Real frame, Real blend) = 0;

		//! 基于joint位置应用一个软件皮肤到这个网格上
		virtual void skinMesh() = 0;

		//! 转换所有网格缓冲区的顶点类型到EVT_TANGENTS
		/** 用于凹凸映射. */
		virtual void convertMeshToTangents() = 0;

		//! 允许打开硬件皮肤
		/* 这个特征还没有实现*/
		virtual bool setHardwareSkinning(bool on) = 0;

		//! 顶点权重
		struct SWeight
		{
			//! 顶点缓冲区的索引
			UINT32 buffer_id; 

			//! 顶点的索引
			UINT32 vertex_id; //这儿保存为全局ID

			//! 权重   强度/百分比 （0-1）
			Real strength;

		private:
			//!  用于CSkinnedMesh的内部变量
			friend class CSkinnedMesh;
			bool *Moved;
			Vector3 StaticPos;
			Vector3 StaticNormal;
		};


		//! 描述一个新位置的动画关键帧
		struct SPositionKey
		{
			Real frame;
			Vector3 position;
		};

		//! 描述一个新缩放的动画关键帧
		struct SScaleKey
		{
			Real frame;
			Vector3 scale;
		};

		//! 描述一个新旋转的动画关键帧
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

			//! 这个Joint的名字
			String Name;

			//! 这个joint的本地矩阵
			Matrix4 LocalMatrix;

			//! 子joint列表
			vector<SJoint*>::type Children;

			//! 附加网格列表
			vector<UINT32>::type AttachedMeshes;

			//! 造成位置改变的关键帧
			deque<SPositionKey>::type PositionKeys;

			//! 造成缩放改变的关键字帧
			deque<SScaleKey>::type ScaleKeys;

			//! 造成旋转改变的关键字
			deque<SRotationKey>::type RotationKeys;

			//! 皮肤权重
			vector<SWeight>::type Weights;

			//! 对于加载器不是必须的，最终会被重新写
			Matrix4 GlobalMatrix;
			Matrix4 GlobalAnimatedMatrix;
			Matrix4 LocalAnimatedMatrix;
			Vector3 Animatedposition;
			Vector3 Animatedscale;
			Quaternion Animatedrotation;

			Matrix4 GlobalInversedMatrix;  

		private:
			//! CSkinnedMesh 内部使用
			friend class CSkinnedMesh;

			SJoint *UseAnimationFrom;
			bool GlobalSkinningSpace;

			SINT32 positionHint;
			SINT32 scaleHint;
			SINT32 rotationHint;
		};


	 

		//! 暴光给加载器：添加网格缓冲区
		virtual vector<SSkinMeshBuffer*>::type& getMeshBuffers() = 0;

		//! 暴光给加载器： joints列表
		virtual vector<SJoint*>::type& getAllJoints() = 0;

		//! 暴光给加载器： joints 列表
		virtual const vector<SJoint*>::type& getAllJoints() const = 0;

		//! 加载器会填充网格之后调用
		virtual void finalize() = 0;

		//! 添加一个新网格缓冲区给这个网格，作为最后一个访问它
		virtual SSkinMeshBuffer* addMeshBuffer() = 0;

		//! 添加一个新joint给这个网格，作为最后一个访问它
		virtual SJoint* addJoint(SJoint *parent = 0) = 0;

		//! 添加一个新权重给这个网格，作为最后一个访问它
		virtual SWeight* addWeight(SJoint *joint) = 0;

		//! 添加一个新位置键给这个网格，作为最后一个访问它 
		virtual SPositionKey* addPositionKey(SJoint *joint) = 0;
		//! 添加一个新缩放键给这个网格，作为最后一个访问它
		virtual SScaleKey* addScaleKey(SJoint *joint) = 0;
		//! 添加一个新旋转键给这个网格，作为最后一个访问它
		virtual SRotationKey* addRotationKey(SJoint *joint) = 0;

		//! 检测这个网格是否有动画
		virtual bool isStatic() = 0;
	};
}

#endif