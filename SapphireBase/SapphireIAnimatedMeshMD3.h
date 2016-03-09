#ifndef _SAPPHIRE_I_ANIMATED_MESH_MD3_
#define _SAPPHIRE_I_ANIMATED_MESH_MD3_

#include "SapphireIAnimatedMesh.h"
#include "SapphireMatrix4.h"
#include "SapphireQuaternion.h"
#include "SapphireIQ3Shader.h"


namespace Sapphire
{
	enum eMD3Models
	{
		EMD3_HEAD = 0,
		EMD3_UPPER,
		EMD3_LOWER,
		EMD3_WEAPON,
		EMD3_NUMMODELS
	};

	//! Animation list
	enum EMD3_ANIMATION_TYPE
	{
		// Animations for both lower and upper parts of the player
		EMD3_BOTH_DEATH_1 = 0,
		EMD3_BOTH_DEAD_1,
		EMD3_BOTH_DEATH_2,
		EMD3_BOTH_DEAD_2,
		EMD3_BOTH_DEATH_3,
		EMD3_BOTH_DEAD_3,

		// Animations for the upper part
		EMD3_TORSO_GESTURE,
		EMD3_TORSO_ATTACK_1,
		EMD3_TORSO_ATTACK_2,
		EMD3_TORSO_DROP,
		EMD3_TORSO_RAISE,
		EMD3_TORSO_STAND_1,
		EMD3_TORSO_STAND_2,

		// Animations for the lower part
		EMD3_LEGS_WALK_CROUCH,
		EMD3_LEGS_WALK,
		EMD3_LEGS_RUN,
		EMD3_LEGS_BACK,
		EMD3_LEGS_SWIM,
		EMD3_LEGS_JUMP_1,
		EMD3_LEGS_LAND_1,
		EMD3_LEGS_JUMP_2,
		EMD3_LEGS_LAND_2,
		EMD3_LEGS_IDLE,
		EMD3_LEGS_IDLE_CROUCH,
		EMD3_LEGS_TURN,

		//! Not an animation, but amount of animation types.
		EMD3_ANIMATION_COUNT
	};

	struct SMD3AnimationInfo
	{
		//! First frame
		SINT32 first;
		//! Last frame
		SINT32 num;
		//! Looping frames
		SINT32 looping;
		//! Frames per second
		SINT32 fps;
	};

	// byte-align structures
#include "SapphirePack.h"
	//! this holds the header info of the MD3 file
	struct SMD3Header
	{
		c8	headerID[4];	//id of file, always "IDP3"
		SINT32	Version;	//this is a version number, always 15
		s8	fileName[68];	//sometimes left Blank... 65 chars, 32bit aligned == 68 chars
		SINT32	numFrames;	//number of KeyFrames
		SINT32	numTags;	//number of 'tags' per frame
		SINT32	numMeshes;	//number of meshes/skins
		SINT32	numMaxSkins;	//maximum number of unique skins used in md3 file. artefact md2
		SINT32	frameStart;	//starting position of frame-structur
		SINT32	tagStart;	//starting position of tag-structures
		SINT32	tagEnd;		//ending position of tag-structures/starting position of mesh-structures
		SINT32	fileSize;
	} PACK_STRUCT;

	//! this holds the header info of an MD3 mesh section
	struct SMD3MeshHeader
	{
		c8 meshID[4];		//id, must be IDP3
		c8 meshName[68];	//name of mesh 65 chars, 32 bit aligned == 68 chars

		SINT32 numFrames;		//number of meshframes in mesh
		SINT32 numShader;		//number of skins in mesh
		SINT32 numVertices;	//number of vertices
		SINT32 numTriangles;	//number of Triangles

		SINT32 offset_triangles;	//starting position of Triangle data, relative to start of Mesh_Header
		SINT32 offset_shaders;	//size of header
		SINT32 offset_st;		//starting position of texvector data, relative to start of Mesh_Header
		SINT32 vertexStart;	//starting position of vertex data,relative to start of Mesh_Header
		SINT32 offset_end;
	} PACK_STRUCT;


	//! Compressed Vertex Data
	struct SMD3Vertex
	{
		SINT16 position[3];
		UINT8 normal[2];
	} PACK_STRUCT;

	//! Texture Coordinate
	struct SMD3TexCoord
	{
		Real u;
		Real v;
	} PACK_STRUCT;

	//! Triangle Index
	struct SMD3Face
	{
		SINT32 Index[3];
	} PACK_STRUCT;

	// Default alignment
#include "SapphireUnPack.h"

	//! Holding Frame Data for a Mesh
	struct SMD3MeshBuffer : public IReferenceCounter
	{
		SMD3MeshHeader MeshHeader;

		String Shader;
		vector < SINT32 >::type Indices;
		vector < SMD3Vertex >::type Vertices;
		vector < SMD3TexCoord >::type Tex;
	};

	//! hold a tag info for connecting meshes
	/** Basically its an alternate way to describe a transformation. */
	struct SMD3QuaternionTag
	{
		virtual ~SMD3QuaternionTag()
		{
			position.x = 0.f;
		}

		// construct copy constructor
		SMD3QuaternionTag(const SMD3QuaternionTag & copyMe)
		{
			*this = copyMe;
		}

		// construct for searching
		SMD3QuaternionTag(const String& name)
			: Name(name) {}

		// construct from a position and euler angles in degrees
		SMD3QuaternionTag(const Vector3 &pos, const Vector3 &radian)
			: position(pos), rotation(radian) {}

		// set to matrix
		void setto(Matrix4 &m)
		{
			rotation.getMatrix(m, position);
		}

		bool operator == (const SMD3QuaternionTag &other) const
		{
			return Name == other.Name;
		}

		SMD3QuaternionTag & operator=(const SMD3QuaternionTag & copyMe)
		{
			Name = copyMe.Name;
			position = copyMe.position;
			rotation = copyMe.rotation;
			return *this;
		}

		String Name;
		Vector3 position;
		Quaternion rotation;
	};

	//! holds a associative list of named quaternions
	struct SMD3QuaternionTagList
	{
		SMD3QuaternionTagList()
		{
			//Container.setAllocStrategy(core::ALLOC_STRATEGY_SAFE);
		}

		// construct copy constructor
		SMD3QuaternionTagList(const SMD3QuaternionTagList& copyMe)
		{
			*this = copyMe;
		}

		virtual ~SMD3QuaternionTagList() {}

		SMD3QuaternionTag* get(const String& name)
		{
			SMD3QuaternionTag search(name);
			//SINT32 index = Container.linear_search(search);
			vector<SMD3QuaternionTag>::iterator it = std::find(Container.begin(), Container.end(), search);
			SINT32 index = std::distance(Container.begin(), it);
			if (index >= 0)
				return &Container[index];
			return 0;
		}

		UINT32 size() const
		{
			return Container.size();
		}

		void set_used(UINT32 new_size)
		{
			//SINT32 diff = (SINT32)new_size - (SINT32)Container.allocated_size();
			SINT32 diff = (SINT32)new_size - (SINT32)Container.capacity();
			if (diff > 0)
			{
				SMD3QuaternionTag e("");
				for (SINT32 i = 0; i < diff; ++i)
					Container.push_back(e);
			}
		}

		const SMD3QuaternionTag& operator[](UINT32 index) const
		{
			return Container[index];
		}

		SMD3QuaternionTag& operator[](UINT32 index)
		{
			return Container[index];
		}

		void push_back(const SMD3QuaternionTag& other)
		{
			Container.push_back(other);
		}

		SMD3QuaternionTagList& operator = (const SMD3QuaternionTagList & copyMe)
		{
			Container = copyMe.Container;
			return *this;
		}

	private:
		std::vector < SMD3QuaternionTag > Container;
	};


	//! Holding Frames Buffers and Tag Infos
	struct SMD3Mesh : public IReferenceCounter
	{
		SMD3Mesh()
		{
			MD3Header.numFrames = 0;
		}

		virtual ~SMD3Mesh()
		{
			for (UINT32 i = 0; i<Buffer.size(); ++i)
				Buffer[i]->drop();
		}

		String Name;
		std::vector<SMD3MeshBuffer*> Buffer;
		SMD3QuaternionTagList TagList;
		SMD3Header MD3Header;
	};


	//! Interface for using some special functions of MD3 meshes
	class IAnimatedMeshMD3 : public IAnimatedMesh
	{
	public:

		//! tune how many frames you want to render inbetween.
		virtual void setInterpolationShift(UINT32 shift, UINT32 loopMode) = 0;

		//! get the tag list of the mesh.
		virtual SMD3QuaternionTagList* getTagList(SINT32 frame, SINT32 detailLevel, SINT32 startFrameLoop, SINT32 endFrameLoop) = 0;

		//! get the original md3 mesh.
		virtual SMD3Mesh* getOriginalMesh() = 0;
	};

}

#endif