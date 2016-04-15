#include "SapphireSVertex.h"
#include "SapphireString.h"

namespace Sapphire
{
	size_t  S3DVertexHasher(const S3DVertex& sv)
	{
		String val = StringUtil::floatToString(sv.Pos.x) + StringUtil::floatToString(sv.Pos.y) + StringUtil::floatToString(sv.Pos.z) + StringUtil::floatToString(sv.TCoords.x) + StringUtil::floatToString(sv.TCoords.y)
			+ StringUtil::floatToString(sv.Normal.x) + StringUtil::floatToString(sv.Normal.y) + StringUtil::floatToString(sv.Normal.z) + StringUtil::floatToString(sv.Color.getAsARGB());

		return hash<string>()(val.c_str());
	}

	bool  S3DVertexEqualOperator(const S3DVertex& lhs, const S3DVertex& rhs)
	{
		return lhs == rhs;
	}


	size_t  S3DVertex2TCoordsHasher(const S3DVertex2TCoords& sv)
	{
		String val = StringUtil::floatToString(sv.Pos.x) + StringUtil::floatToString(sv.Pos.y) + StringUtil::floatToString(sv.Pos.z) + StringUtil::floatToString(sv.TCoords.x) + StringUtil::floatToString(sv.TCoords.y)
			+ StringUtil::floatToString(sv.TCoords2.x) + StringUtil::floatToString(sv.TCoords2.y) + StringUtil::floatToString(sv.Normal.x) + StringUtil::floatToString(sv.Normal.y) + StringUtil::floatToString(sv.Normal.z) 
			+ StringUtil::floatToString(sv.Color.getAsARGB());

		return hash<string>()(val.c_str());
	}

	bool  S3DVertex2TCoordsEqualOperator(const S3DVertex2TCoords& lhs, const S3DVertex2TCoords& rhs)
	{
		return lhs == rhs;
	}


	size_t  S3DVertexTangentsHasher(const S3DVertexTangents& sv)
	{
		String val = StringUtil::floatToString(sv.Pos.x) + StringUtil::floatToString(sv.Pos.y) + StringUtil::floatToString(sv.Pos.z) + StringUtil::floatToString(sv.TCoords.x) + StringUtil::floatToString(sv.TCoords.y)
			+ StringUtil::floatToString(sv.Normal.x) + StringUtil::floatToString(sv.Normal.y) + StringUtil::floatToString(sv.Normal.z) + StringUtil::floatToString(sv.Binormal.x) +StringUtil::floatToString(sv.Binormal.y)
			+ StringUtil::floatToString(sv.Binormal.z) + StringUtil::floatToString(sv.Tangent.x) + StringUtil::floatToString(sv.Tangent.y) + StringUtil::floatToString(sv.Tangent.z) + StringUtil::floatToString(sv.Normal.y) 
			+ StringUtil::floatToString(sv.Normal.z) + StringUtil::floatToString(sv.Color.getAsARGB());

		return hash<string>()(val.c_str());
	}

	bool  S3DVertexTangentsEqualOperator(const S3DVertexTangents& lhs, const S3DVertexTangents& rhs)
	{
		return lhs == rhs;
	}
}