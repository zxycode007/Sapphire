#ifndef _SAPPHIRE_TERRIAN_ENLEMENTS_
#define _SAPPHIRE_TERRIAN_ENLEMENTS_

namespace Sapphire
{
	//! 在地形场景节点TerrainSceneNode枚举patch大小来指定pathes大小
	enum E_TERRAIN_PATCH_SIZE
	{
		//! patch 大小9, 这个patch大小用4层LOD细节
		ETPS_9 = 9,

		//!patch 大小17, 这个patch大小用5层LOD细节
		ETPS_17 = 17,

		//!patch 大小33, 这个patch大小用6层LOD细节  
		ETPS_33 = 33,

		//!patch 大小65, 这个patch大小用7层LOD细节
		ETPS_65 = 65,

		//! patch 大小129, 这个patch大小用8层LOD细节
		ETPS_129 = 129
}

#endif