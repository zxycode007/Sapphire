#ifndef _SAPPHIRE_IANIMATED_MESH_
#define _SAPPHIRE_IANIMATED_MESH_

#include "SapphirePrerequisites.h"
#include "SapphireIMesh.h"

namespace Sapphire
{
	//! （可活动）网格的类型
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

	//! 一个动画网格的接口
	/** 这里已经提供这个接口一个简单的实现。使用SAnimatedMesh，SMesh
	etc，你不需要实现自己的接口
	  */
	class IAnimatedMesh : public IMesh
	{
	public:

		//! 获取这个动画网格的总帧数
		/** \return 总帧数。如果帧数是1，它是一个静态的，没有动画的网格*/
		virtual UINT32 getFrameCount() const = 0;

		//! 获取这个动画网格的动画速度
		/** \return 默认播放这个动画每秒的帧数. 如果数量是0，它是静态的，没有动画的网格 */
		virtual FLOAT32 getAnimationSpeed() const = 0;

		//! 设置动画网格的动画速度
		/** \param fps 默认播放这个动画每秒的帧数. 如果数量是0，它不会获得。这实际速度在
		场景节点的网格实例化中设置
		*/
		virtual void setAnimationSpeed(FLOAT32 fps) = 0;

		//! 返回一个帧IMesh接口
		/** \param frame: 基于0索引的帧数。最大值是getFrameCount() - 1
		\param detailLevel: LOD等级，0是最低，255是最高LOD。绝大多数网格将无效这个细节等级。
		\param startFrameLoop: 由于某些动画网格MD2是在两个静态帧之间进行混合，但可能动画是
		在一个循环里面，起始帧循环和结束帧循环需要被定义，去放置动画混合帧超出这个循环之外。
		如果起始帧和结束帧都是-1，它们无效。
		\param endFrameLoop: 见起始帧
		\return Returns 这个基于一个LOD的动画网格 */
		virtual IMesh* getMesh(SINT32 frame, SINT32 detailLevel = 255, SINT32 startFrameLoop = -1, SINT32 endFrameLoop = -1) = 0;

		//! 返回这个动画网格的类型
		/** 大多数情况下，使用这个方法不是必要的。它主要确保向下安全。
		例如getMeshType()返回 EAMT_MD2，它可以安全转换IAnimatedMesh到
		IAnimatedMeshMD2
		\returns 网格类型 */
		virtual E_ANIMATED_MESH_TYPE getMeshType() const
		{
			return EAMT_UNKNOWN;
		}
	};
}

#endif
