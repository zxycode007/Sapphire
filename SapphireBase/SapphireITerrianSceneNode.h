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

	//! 一个用geo mipmap 算法显示地形的场景节点
	/**
	* 这个场景节点能够非常快的加载。它使用一个CLOD(Continous Level of Detail)算法对基于一个LOD的每个Patch，它通过基于patch到相机的距离来确定更新索引。
	  这个地形的patch的大小必须是一个 ( 2^N+1, i.e. 8+1(9), 16+1(17), etc. ).的大小.
	  MaxLOD是否有效依赖地形patch的大小。 LOD 0包含所有的索引去绘制对于一个patch最大细节下所有的三角形。对于每个LOD的step taken 增加1，在创建索引时增加 2^LOD,所以，
	  对于LOD1， step taken 是2，  对于LOD 2， step taken 是4， LOD3 8.... step不能大于patch的大小，所以，对于大小17的patch， LOD 8，算法要求没2^8(256)个顶点创建
	  索引，它的patch 大小不可能是17。 对于patch大小等于17所能支持的最大LOD 是 2^4(16). 所以，对于MaxLOD=5， 你可以使用LOD 0 （最大细节），LOD 1（每两个顶点），LOD 2 (每4个顶点)
	  ,LOD 3 （每8个顶点） 还有 LOD 4 （每16个顶点）
	**/
	class ITerrainSceneNode : public ISceneNode
	{
	public:
		 
		ITerrainSceneNode(ISceneNode* parent, ISceneManager* mgr, SINT32 id,
			const Vector3& position =Vector3(0.0f, 0.0f, 0.0f),
			const Vector3& rotation =Vector3(0.0f, 0.0f, 0.0f),
			const Vector3& scale =Vector3(1.0f, 1.0f, 1.0f))
			: ISceneNode(parent, mgr, id, position, rotation, scale) {}

		//! 获取地形的包围体盒子
		/** \return 地形的包围体盒子 */
		virtual const AxisAlignedBox& getBoundingBox() const = 0;

		//! 获取地形的包围体盒子
		/** \return 地形的包围体盒子 */
		virtual const AxisAlignedBox& getBoundingBox(SINT32 patchX, SINT32 patchZ) const = 0;

		//! 获取当前网格缓冲区的索引数
		/** \return 索引数 */
		virtual UINT32 getIndexCount() const = 0;

		//! 获取网格指针
		/** \return 网格指针 */
		virtual IMesh* getMesh() = 0;

		//! 获取指向用于地形缓冲区指针（多数时候不需要）
		virtual IMeshBuffer* getRenderBuffer() = 0;


		//! 获取基于一个指定LOD的网格缓冲区数据
		/** \param mb 一个IDynamicMeshBuffer对象的引用
		\param LOD 要指定的LOD*/
		virtual void getMeshBufferForLOD(IDynamicMeshBuffer& mb, SINT32 LOD = 0) const = 0;

		//! 获取一个在指定LOD下的一个指定patch的索引
		/** \param indices 一个UINT32 索引数组的引用
		\param patchX Patch x 坐标.
		\param patchZ Patch z y坐标.
		\param LOD 对于这个Patch的LOD，如果为-1，获取当前LOD。 如果当前LOD设置为-1,意味着它不会显示，那么它返回更高一级LOD （LOD 0）
		的三角形
		\return 放入这个缓冲区的索引数 */
		virtual SINT32 getIndicesForPatch(std::vector<UINT32>& indices,
			SINT32 patchX, SINT32 patchZ, SINT32 LOD = 0) = 0;

		//! 填充每个patch的当前LOD到一个数组
		/** \param LODs 一个保存这些值的引用
		\return 数组元素个数 */
		virtual SINT32 getCurrentLODOfPatches(std::vector<SINT32>& LODs) const = 0;

		//! 手动设置一个patch的LOD
		/** \param patchX patch的x坐标
		\param patchZ patch的z坐标
		\param LOD 这个patch的LOD等级*/
		virtual void setLODOfPatch(SINT32 patchX, SINT32 patchZ, SINT32 LOD = 0) = 0;

		//! 获取地形中心
		virtual const Vector3& getTerrainCenter() const = 0;

		//! 获取地形高度
		virtual Real getHeight(Real x, Real y) const = 0;

		//! 设置相机移动阀值
		/** 它用于确定重新计算场景节点的索引，这个值默认10.0f */
		virtual void setCameraMovementDelta(Real delta) = 0;

		//! 设置相机旋转阀值
		/** 它用于确定重新计算场景节点的索引，这个值默认10.0f */
		virtual void setCameraRotationDelta(Real delta) = 0;

		//! 设置当geomipmap数据改变，这个节点是否需要动态更新它相关的selector
		/** \param bVal: 布尔值表示是否需要动态更新selector*/
		virtual void setDynamicSelectorUpdate(bool bVal) = 0;

		//! 覆盖默认创建的距离阀值
		/** 对于确定一个在渲染patch的LOD， 如果任意LOD被覆盖，那么这个场景节点不再对这些值应用缩放因子、
		如果你覆盖这些距离，并且引用缩放因子到这个节点， 你需要最后和你信的地形大小一起更新新的距离
		  */
		virtual bool overrideLODDistance(SINT32 LOD, Real newDistance) = 0;

		//! 缩放基础纹理，类似makePlanarTextureMapping 
		/** \param scale  缩放量，值大于1，0，会增加在地形上绘制纹理的时间。值小于0，会减少在地形上绘制纹理的世界。
		用负值会翻转纹理，而且仍然能缩放它
		\param scale2 如果设置为0(默认), 这将设置第二个纹理坐标与第一个纹理坐标同样的值，如果非0值，它将缩放第二个纹理坐标*/
		virtual void scaleTexture(Real scale = 1.0f, Real scale2 = 0.0f) = 0;

		//! 初始化地形数据，从高程图文件中加载顶点
		/** 这个文件必须包含一个可加载的高程图图像。这个高程图必须是正方形
		\param file 要加载的图像文件。 
		\param vertexColor 所有顶点颜色
		\param smoothFactor 平滑因子 */
		virtual bool loadHeightMap(IReadFile* file,
			//ColourValue vertexColor = ColourValue(1.0, 1.0, 1.0, 1.0),
			ColourValue vertexColor = ColourValue::getColourValue(255, 255, 255, 255),
			SINT32 smoothFactor = 0) = 0;

		//! 初始化地形数据，从高程图文件中加载顶点
		/** 这数据将以给定位大小的整形（有符号）或浮点数（32位有符号）进行插值.允许为整形的位大小可以是8，16，32.
		高程图必须是正方形
		\param file 要读取RAW数据的文件
		\param bitsPerPixel 没个像素的数据位大小，对于浮点数用32位
		\param signedData 使用uint还是int，对float无效
		\param floatVals 使用int还是float数据
		\param width 高程图的宽度（和高度相等）。用0的话由文件大小自动计算
		\param vertexColor 所有顶点的颜色
		\param smoothFactor 平滑因子  */
		virtual bool loadHeightMapRAW(IReadFile* file, SINT32 bitsPerPixel = 16,
			bool signedData = false, bool floatVals = false, SINT32 width = 0,
			//ColourValue vertexColor = ColourValue(1.0, 1.0, 1.0, 1.0),
			ColourValue vertexColor = ColourValue::getColourValue(255, 255, 255, 255),
			SINT32 smoothFactor = 0) = 0;

	};
}

#endif