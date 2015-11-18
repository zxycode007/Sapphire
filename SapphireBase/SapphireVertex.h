#ifndef _SAPPHIRE_VERTEX_
#define _SAPPHIRE_VERTEX_
#include "SapphirePrerequisites.h"
#include "SapphireVector4.h"
#include "SapphireVector3.h"
#include "SapphireVector2.h"
#include "SapphireColorValue.h"

namespace Sapphire
{
	
	class _SapphireExport SVertex : public VertexAlloc
	{
	public:
		
		Vector4 mPos;       //顶点位置
		Vector4 mNormal;        //法线信息
		Vector4 mBNormal;    //副法线信息
		Vector4 mTangent;      //切线信息
		ColourValue mColor;   //顶点颜色
		

		SVertex(): mTexNum(1)
		{
			mPos = Vector4(0, 0, 0, 0);
			mNormal = Vector4(0, 0, 0, 0);
			mBNormal = Vector4(0, 0, 0, 0);
			mTangent = Vector4(0, 0, 0, 0);
			mTexCoord[0] = Vector2(0, 0);
			mColor = ColourValue::White;
		};

		SVertex(Vector4 pos) 
		{
			mPos = pos;
			mNormal = Vector4(0, 0, 0, 0);
			mBNormal = Vector4(0, 0, 0, 0);
			mTangent = Vector4(0, 0, 0, 0);
			mTexNum = 1;
			mTexCoord[0] = Vector2(0, 0);
			mColor = ColourValue::White;
		};

		SVertex(Vector4 pos, Vector2 uv, Vector4 normal)
		{
			mPos = pos;
			mNormal = normal;
			mBNormal = Vector4(0, 0, 0, 0);
			mTangent = Vector4(0, 0, 0, 0);
			mTexNum = 1;
			mTexCoord[0] = uv;
			mColor = ColourValue::White;
		}

		SVertex(Real x, Real y, Real z)
		{ 
			mPos.x = x;
			mPos.y = y;
			mPos.z = z;
			mNormal = Vector4(0,0,0,0);
			mBNormal = Vector4(0, 0, 0, 0);
			mTangent = Vector4(0, 0, 0, 0);
			mTexNum = 1;
			mTexCoord[0] = Vector2(0, 0);
			mColor = ColourValue::White;
		}

		SVertex(Real x, Real y, Real z, Real nx, Real ny, Real nz)
		{
			 
			mPos.x = x;
			mPos.y = y;
			mPos.z = z;
			mNormal.x = nx;
			mNormal.y = ny;
			mNormal.z = nz;
			mBNormal = Vector4(0, 0, 0, 0);
			mTangent = Vector4(0, 0, 0, 0);
			mTexNum = 1;
			mTexCoord[0] = Vector2(0, 0);
			mColor = ColourValue::White;
		}

		SVertex(Real x, Real y, Real z, Real nx, Real ny, Real nz, Real u, Real v)
		{
		 
			mPos.x = x;
			mPos.y = y;
			mPos.z = z;
			mNormal.x = nx;
			mNormal.y = ny;
			mNormal.z = nz;
			mBNormal = Vector4(0, 0, 0, 0);
			mTangent = Vector4(0, 0, 0, 0);
			mTexNum = 1;
			mTexCoord[0].x = u;
			mTexCoord[0].y = v;
			mColor = ColourValue::White;
		}


		SVertex(const SVertex& other)
		{
			 
			mPos = other.mPos;
			mNormal = other.mNormal;
			mBNormal = other.mBNormal;
			mTangent = other.mTangent;
			mTexNum = other.mTexNum;
			for (int i = 0; i < mTexNum; i++)
			{
				mTexCoord[i] = other.mTexCoord[i];
			}
			mColor = other.mColor;
		}

		~SVertex()
		{}


		bool operator==(const SVertex& other)
		{
		
			if (mPos != other.mPos || mNormal != other.mNormal || mBNormal != other.mBNormal || mTangent != other.mTangent || mTexNum != other.mTexNum)
			{
				return false;
			}
			for (int i = 0; i < mTexNum; i++)
			{
				if (mTexCoord[i] != other.mTexCoord[i])
				{
					return false;
				}
				
			}
			return true;
		}
		bool operator!=(const SVertex& other)
		{

			if (mPos != other.mPos || mNormal != other.mNormal || mBNormal != other.mBNormal || mTangent != other.mTangent || mTexNum != other.mTexNum)
			{
				return true;
			}
			for (int i = 0; i < mTexNum; i++)
			{
				if (mTexCoord[i] != other.mTexCoord[i])
				{
					return true;
				}

			}
			return false;
		}
		void addTexCrood(Vector2 uv) 
		{
			
			if (mTexNum + 1 <= SAPPHIRE_MAX_TEXTURE_LAYERS)
			{
				mTexCoord[mTexNum] = uv;
				mTexNum++;
			}
			else
			{
				SAPPHIRE_EXCEPT(Exception::ERR_INTERNAL_ERROR, "Texture Coord Index More than SAPPHIRE_MAX_TEXTURE_LAYERS !", "SVertex::addTexCrood(Vector2 uv)");
			}
		}
		
		void setTexCoord(int index, Vector2 uv)
		{
			if (index > mTexNum)
			{
				SAPPHIRE_EXCEPT(Exception::ERR_INVALIDPARAMS, "Texture Coord Index Overflow!", "SVertex::setTexCoord(int index, Vector2 uv)");
				return;
			}
			mTexCoord[index] = uv;
		}

		Vector2 getTexCoord(int index)
		{
			if (index > mTexNum)
			{
				SAPPHIRE_EXCEPT(Exception::ERR_INVALIDPARAMS, "Texture Coord Index Overflow!", "SVertex::getTexCoord(int index)");
				return Vector2(0, 0);
			}
			return mTexCoord[index];

		};

	private:

		int mTexNum;        //纹理坐标层数（多重纹理）
		Vector2 mTexCoord[SAPPHIRE_MAX_TEXTURE_LAYERS];  //纹理坐标

	};
}



#endif