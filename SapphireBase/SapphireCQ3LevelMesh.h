#ifndef _SAPPHIRE_C_Q3_LEVEL_MESH_
#define _SAPPHIRE_C_Q3_LEVEL_MESH_


#include "SapphireIQ3LevelMesh.h"
#include "SapphireIReadFile.h"
#include "SapphireIFileSystem.h"
#include "SapphireSMesh.h"
#include "SapphireSMeshBufferLightMap.h"
#include "SapphireIVideoDriver.h"
#include "SapphireIScenesManager.h"
#include "SapphireOS.h"

namespace Sapphire
{
	class CQ3LevelMesh : public Quake3::IQ3LevelMesh
	{
	public:

		//! constructor
		CQ3LevelMesh(IFileSystem* fs, ISceneManager* smgr,
			const Quake3::Q3LevelLoadParameter &loadParam);

		//! destructor
		virtual ~CQ3LevelMesh();

		//! loads a level from a .bsp-File. Also tries to load all
		//! needed textures. Returns true if successful.
		bool loadFile(IReadFile* file);

		//! returns the amount of frames in milliseconds. If the amount
		//! is 1, it is a static (=non animated) mesh.
		virtual UINT32 getFrameCount() const;

		//! Gets the default animation speed of the animated mesh.
		/** \return Amount of frames per second. If the amount is 0, it is a static, non animated mesh. */
		virtual FLOAT32 getAnimationSpeed() const
		{
			return FramesPerSecond;
		}

		//! Gets the frame count of the animated mesh.
		/** \param fps Frames per second to play the animation with. If the amount is 0, it is not animated.
		The actual speed is set in the scene node the mesh is instantiated in.*/
		virtual void setAnimationSpeed(FLOAT32 fps)
		{
			FramesPerSecond = fps;
		}

		//! returns the animated mesh based on a detail level. 0 is the
		//! lowest, 255 the highest detail. Note, that some Meshes will
		//! ignore the detail level.
		virtual IMesh* getMesh(SINT32 frameInMs, SINT32 detailLevel = 255,
			SINT32 startFrameLoop = -1, SINT32 endFrameLoop = -1);

		//! Returns an axis aligned bounding box of the mesh.
		//! \return A bounding box of this mesh is returned.
		virtual const AxisAlignedBox& getBoundingBox() const;

		virtual void setBoundingBox(const AxisAlignedBox& box);

		//! Returns the type of the animated mesh.
		virtual E_ANIMATED_MESH_TYPE getMeshType() const;

		//! loads the shader definition
		virtual void getShader(IReadFile* file);

		//! loads the shader definition
		virtual const Quake3::IShader * getShader(const c8 * filename, bool fileNameIsValid = true);

		//! returns a already loaded Shader
		virtual const Quake3::IShader * getShader(UINT32 index) const;


		//! loads a configuration file
		virtual void getConfiguration(IReadFile* file);
		//! get's an interface to the entities
		virtual Quake3::tQ3EntityList & getEntityList();

		//! returns the requested brush entity
		virtual IMesh* getBrushEntityMesh(SINT32 num) const;

		//! returns the requested brush entity
		virtual IMesh* getBrushEntityMesh(Quake3::IEntity &ent) const;

		//Link to held meshes? ...


		//! returns amount of mesh buffers.
		virtual UINT32 getMeshBufferCount() const
		{
			return 0;
		}

		//! returns pointer to a mesh buffer
		virtual IMeshBuffer* getMeshBuffer(UINT32 nr) const
		{
			return 0;
		}

		//! Returns pointer to a mesh buffer which fits a material
		/** \param material: material to search for
		\return Pointer to the mesh buffer or 0 if there is no such mesh buffer. */
		virtual IMeshBuffer* getMeshBuffer(const SMaterial &material) const
		{
			return 0;
		}

		virtual void setMaterialFlag(E_MATERIAL_FLAG flag, bool newvalue)
		{
			return;
		}

		//! set the hardware mapping hint, for driver
		virtual void setHardwareMappingHint(E_HARDWARE_MAPPING newMappingHint, E_BUFFER_TYPE buffer = EBT_VERTEX_AND_INDEX)
		{
			return;
		}

		//! flags the meshbuffer as changed, reloads hardware buffers
		virtual void setDirty(E_BUFFER_TYPE buffer = EBT_VERTEX_AND_INDEX)
		{
			return;
		}

	private:


		void constructMesh();
		void solveTJunction();
		void loadTextures();
		SMesh** buildMesh(SINT32 num);

		struct STexShader
		{
			ITexture* Texture;
			SINT32 ShaderID;
		};

		vector< STexShader >::type Tex;
		vector<ITexture*>::type Lightmap;

		enum eLumps
		{
			kEntities = 0,	// Stores player/object positions, etc...
			kShaders = 1,	// Stores texture information
			kPlanes = 2,	// Stores the splitting planes
			kNodes = 3,	// Stores the BSP nodes
			kLeafs = 4,	// Stores the leafs of the nodes
			kLeafFaces = 5,	// Stores the leaf's indices into the faces
			kLeafBrushes = 6,	// Stores the leaf's indices into the brushes
			kModels = 7,	// Stores the info of world models
			kBrushes = 8,	// Stores the brushes info (for collision)
			kBrushSides = 9,	// Stores the brush surfaces info
			kVertices = 10,	// Stores the level vertices
			kMeshVerts = 11,	// Stores the model vertices offsets
			kFogs = 12,	// Stores the shader files (blending, anims..)
			kFaces = 13,	// Stores the faces for the level
			kLightmaps = 14,	// Stores the lightmaps for the level
			kLightGrid = 15,	// Stores extra world lighting information
			kVisData = 16,	// Stores PVS and cluster info (visibility)
			kLightArray = 17,	// RBSP
			kMaxLumps				// A constant to store the number of lumps
		};

		enum eBspSurfaceType
		{
			BSP_MST_BAD,
			BSP_MST_PLANAR,
			BSP_MST_PATCH,
			BSP_MST_TRIANGLE_SOUP,
			BSP_MST_FLARE,
			BSP_MST_FOLIAGE

		};

		struct tBSPHeader
		{
			SINT32 strID;     // This should always be 'IBSP'
			SINT32 version;       // This should be 0x2e for Quake 3 files
		};
		tBSPHeader header;

		struct tBSPLump
		{
			SINT32 offset;
			SINT32 length;
		};


		struct tBSPVertex
		{
			FLOAT32 vPosition[3];      // (x, y, z) position.
			FLOAT32 vTextureCoord[2];  // (u, v) texture coordinate
			FLOAT32 vLightmapCoord[2]; // (u, v) lightmap coordinate
			FLOAT32 vNormal[3];        // (x, y, z) normal vector
			UINT8 color[4];           // RGBA color for the vertex
		};

		struct tBSPFace
		{
			SINT32 textureID;        // The index into the texture array
			SINT32 fogNum;           // The index for the effects (or -1 = n/a)
			SINT32 type;             // 1=polygon, 2=patch, 3=mesh, 4=billboard
			SINT32 vertexIndex;      // The index into this face's first vertex
			SINT32 numOfVerts;       // The number of vertices for this face
			SINT32 meshVertIndex;    // The index into the first meshvertex
			SINT32 numMeshVerts;     // The number of mesh vertices
			SINT32 lightmapID;       // The texture index for the lightmap
			SINT32 lMapCorner[2];    // The face's lightmap corner in the image
			SINT32 lMapSize[2];      // The size of the lightmap section
			FLOAT32 lMapPos[3];     // The 3D origin of lightmap.
			FLOAT32 lMapBitsets[2][3]; // The 3D space for s and t unit vectors.
			FLOAT32 vNormal[3];     // The face normal.
			SINT32 size[2];          // The bezier patch dimensions.
		};

		struct tBSPTexture
		{
			c8 strName[64];   // The name of the texture w/o the extension
			UINT32 flags;          // The surface flags (unknown)
			UINT32 contents;       // The content flags (unknown)
		};

		struct tBSPLightmap
		{
			UINT8 imageBits[128][128][3];   // The RGB data in a 128x128 image
		};

		struct tBSPNode
		{
			SINT32 plane;      // The index into the planes array
			SINT32 front;      // The child index for the front node
			SINT32 back;       // The child index for the back node
			SINT32 mins[3];    // The bounding box min position.
			SINT32 maxs[3];    // The bounding box max position.
		};

		struct tBSPLeaf
		{
			SINT32 cluster;           // The visibility cluster
			SINT32 area;              // The area portal
			SINT32 mins[3];           // The bounding box min position
			SINT32 maxs[3];           // The bounding box max position
			SINT32 leafface;          // The first index into the face array
			SINT32 numOfLeafFaces;    // The number of faces for this leaf
			SINT32 leafBrush;         // The first index for into the brushes
			SINT32 numOfLeafBrushes;  // The number of brushes for this leaf
		};

		struct tBSPPlane
		{
			FLOAT32 vNormal[3];     // Plane normal.
			FLOAT32 d;              // The plane distance from origin
		};

		struct tBSPVisData
		{
			SINT32 numOfClusters;   // The number of clusters
			SINT32 bytesPerCluster; // Bytes (8 bits) in the cluster's bitset
			c8 *pBitsets;      // Array of bytes holding the cluster vis.
		};

		struct tBSPBrush
		{
			SINT32 brushSide;           // The starting brush side for the brush
			SINT32 numOfBrushSides;     // Number of brush sides for the brush
			SINT32 textureID;           // The texture index for the brush
		};

		struct tBSPBrushSide
		{
			SINT32 plane;              // The plane index
			SINT32 textureID;          // The texture index
		};

		struct tBSPModel
		{
			FLOAT32 min[3];           // The min position for the bounding box
			FLOAT32 max[3];           // The max position for the bounding box.
			SINT32 faceIndex;          // The first face index in the model
			SINT32 numOfFaces;         // The number of faces in the model
			SINT32 brushIndex;         // The first brush index in the model
			SINT32 numOfBrushes;       // The number brushes for the model
		};

		struct tBSPFog
		{
			c8 shader[64];		// The name of the shader file
			SINT32 brushIndex;		// The brush index for this shader
			SINT32 visibleSide;	// the brush side that ray tests need to clip against (-1 == none
		};
		vector < STexShader >::type FogMap;

		struct tBSPLights
		{
			UINT8 ambient[3];     // This is the ambient color in RGB
			UINT8 directional[3]; // This is the directional color in RGB
			UINT8 direction[2];   // The direction of the light: [phi,theta]
		};

		void loadTextures(tBSPLump* l, IReadFile* file); // Load the textures
		void loadLightmaps(tBSPLump* l, IReadFile* file); // Load the lightmaps
		void loadVerts(tBSPLump* l, IReadFile* file); // Load the vertices
		void loadFaces(tBSPLump* l, IReadFile* file); // Load the faces
		void loadPlanes(tBSPLump* l, IReadFile* file); // Load the Planes of the BSP
		void loadNodes(tBSPLump* l, IReadFile* file); // load the Nodes of the BSP
		void loadLeafs(tBSPLump* l, IReadFile* file); // load the Leafs of the BSP
		void loadLeafFaces(tBSPLump* l, IReadFile* file); // load the Faces of the Leafs of the BSP
		void loadVisData(tBSPLump* l, IReadFile* file); // load the visibility data of the clusters
		void loadEntities(tBSPLump* l, IReadFile* file); // load the entities
		void loadModels(tBSPLump* l, IReadFile* file); // load the models
		void loadMeshVerts(tBSPLump* l, IReadFile* file); // load the mesh vertices
		void loadBrushes(tBSPLump* l, IReadFile* file); // load the brushes of the BSP
		void loadBrushSides(tBSPLump* l, IReadFile* file); // load the brushsides of the BSP
		void loadLeafBrushes(tBSPLump* l, IReadFile* file); // load the brushes of the leaf
		void loadFogs(tBSPLump* l, IReadFile* file); // load the shaders

		//bi-quadratic bezier patches
		void createCurvedSurface_bezier(SMeshBufferLightMap* meshBuffer,
			SINT32 faceIndex, SINT32 patchTesselation, SINT32 storevertexcolor);

		void createCurvedSurface_nosubdivision(SMeshBufferLightMap* meshBuffer,
			SINT32 faceIndex, SINT32 patchTesselation, SINT32 storevertexcolor);

		struct S3DVertex2TCoords_64
		{
			Vector3 Pos;
			Vector3 Normal;
			ColourValue Color;
			Vector2 TCoords;
			Vector2 TCoords2;

			void copy(S3DVertex2TCoords &dest) const;

			S3DVertex2TCoords_64() {}
			S3DVertex2TCoords_64(const Vector3& pos, const Vector3& normal, const ColourValue& color,
				const Vector2& tcoords, const Vector2& tcoords2)
				: Pos(pos), Normal(normal), Color(color), TCoords(tcoords), TCoords2(tcoords2) {}

			S3DVertex2TCoords_64 getInterpolated_quadratic(const S3DVertex2TCoords_64& v2,
				const S3DVertex2TCoords_64& v3, const Real d) const
			{
				return S3DVertex2TCoords_64(
					Pos.getInterpolated_quadratic(v2.Pos, v3.Pos, d),
					Normal.getInterpolated_quadratic(v2.Normal, v3.Normal, d),
					Color.getInterpolated_quadratic(v2.Color, v3.Color, (FLOAT32)d),
					TCoords.getInterpolated_quadratic(v2.TCoords, v3.TCoords, d),
					TCoords2.getInterpolated_quadratic(v2.TCoords2, v3.TCoords2, d));
			}
		};

		inline void copy(S3DVertex2TCoords * dest, const tBSPVertex * source,
			SINT32 vertexcolor) const;
		void copy(S3DVertex2TCoords_64 * dest, const tBSPVertex * source, SINT32 vertexcolor) const;


		struct SBezier
		{
			SMeshBufferLightMap *Patch;
			S3DVertex2TCoords_64 control[9];

			void tesselate(SINT32 level);

		private:
			SINT32	Level;

			vector<S3DVertex2TCoords_64>::type column[3];

		};
		SBezier Bezier;

		Quake3::Q3LevelLoadParameter LoadParam;

		tBSPLump Lumps[kMaxLumps];

		tBSPTexture* Textures;
		SINT32 NumTextures;

		tBSPLightmap* LightMaps;
		SINT32 NumLightMaps;

		tBSPVertex* Vertices;
		SINT32 NumVertices;

		tBSPFace* Faces;
		SINT32 NumFaces;

		tBSPModel* Models;
		SINT32 NumModels;

		tBSPPlane* Planes;
		SINT32 NumPlanes;

		tBSPNode* Nodes;
		SINT32 NumNodes;

		tBSPLeaf* Leafs;
		SINT32 NumLeafs;

		SINT32 *LeafFaces;
		SINT32 NumLeafFaces;

		SINT32 *MeshVerts;           // The vertex offsets for a mesh
		SINT32 NumMeshVerts;

		tBSPBrush* Brushes;
		SINT32 NumBrushes;

		SMesh** BrushEntities;

		SMesh* Mesh[Quake3::E_Q3_MESH_SIZE];
		IVideoDriver* Driver;
		String LevelName;
		IFileSystem* FileSystem; // needs because there are no file extenstions stored in .bsp files.

		// Additional content
		ISceneManager* SceneManager;
		enum eToken
		{
			Q3_TOKEN_UNRESOLVED = 0,
			Q3_TOKEN_EOF = 1,
			Q3_TOKEN_START_LIST,
			Q3_TOKEN_END_LIST,
			Q3_TOKEN_ENTITY,
			Q3_TOKEN_TOKEN,
			Q3_TOKEN_EOL,
			Q3_TOKEN_COMMENT,
			Q3_TOKEN_MATH_DIVIDE,
			Q3_TOKEN_MATH_ADD,
			Q3_TOKEN_MATH_MULTIPY
		};
		struct SQ3Parser
		{
			const c8 *source;
			UINT32 sourcesize;
			UINT32 index;
			String token;
			eToken tokenresult;
		};
		SQ3Parser Parser;


		typedef void(CQ3LevelMesh::*tParserCallback) (Quake3::SVarGroupList *& groupList, eToken token);
		void parser_parse(const void * data, UINT32 size, tParserCallback callback);
		void parser_nextToken();

		void dumpVarGroup(const Quake3::SVarGroup * group, SINT32 stack) const;

		void scriptcallback_entity(Quake3::SVarGroupList *& grouplist, eToken token);
		void scriptcallback_shader(Quake3::SVarGroupList *& grouplist, eToken token);
		void scriptcallback_config(Quake3::SVarGroupList *& grouplist, eToken token);

		vector < Quake3::IShader >::type Shader;
		vector < Quake3::IShader >::type Entity;		//Quake3::tQ3EntityList Entity;


		Quake3::tStringList ShaderFile;
		void InitShader();
		void ReleaseShader();
		void ReleaseEntity();


		SINT32 setShaderMaterial(SMaterial & material, const tBSPFace * face) const;
		SINT32 setShaderFogMaterial(SMaterial &material, const tBSPFace * face) const;

		struct SToBuffer
		{
			SINT32 takeVertexColor;
			UINT32 index;
		};

		void cleanMeshes();
		void cleanMesh(SMesh *m, const bool texture0important = false);
		void cleanLoader();
		void calcBoundingBoxes();
		c8 buf[128];
		FLOAT32 FramesPerSecond;
	};
}

#endif