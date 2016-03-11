#include "SapphireCompileConifg.h"
#ifdef _SAPPHIRE_COMPILE_WITH_BSP_LOADER_
#include "SapphireCQ3LevelMesh.h"
#include "SapphireIScenesManager.h"
#include "SapphireOS.h"
#include "SapphireSMeshBufferLightMap.h"
#include "SapphireILightSceneNode.h"
#include "SapphireIQ3Shader.h"
#include "SapphireIFileList.h"

namespace Sapphire
{
	using namespace Quake3;

	//! constructor
	CQ3LevelMesh::CQ3LevelMesh(IFileSystem* fs, ISceneManager* smgr,
		const Q3LevelLoadParameter &loadParam)
		: LoadParam(loadParam), Textures(0), NumTextures(0), LightMaps(0), NumLightMaps(0),
		Vertices(0), NumVertices(0), Faces(0), NumFaces(0), Models(0), NumModels(0),
		Planes(0), NumPlanes(0), Nodes(0), NumNodes(0), Leafs(0), NumLeafs(0),
		LeafFaces(0), NumLeafFaces(0), MeshVerts(0), NumMeshVerts(0),
		Brushes(0), NumBrushes(0), BrushEntities(0), FileSystem(fs),
		SceneManager(smgr), FramesPerSecond(25.f)
	{
#ifdef _DEBUG
		IReferenceCounter::setDebugName("CQ3LevelMesh");
#endif

		for (SINT32 i = 0; i != E_Q3_MESH_SIZE; ++i)
		{
			Mesh[i] = 0;
		}

		Driver = smgr ? smgr->getVideoDriver() : 0;
		if (Driver)
			Driver->grab();

		if (FileSystem)
			FileSystem->grab();

		// load default shaders
		InitShader();
	}


	//! destructor
	CQ3LevelMesh::~CQ3LevelMesh()
	{
		cleanLoader();

		if (Driver)
			Driver->drop();

		if (FileSystem)
			FileSystem->drop();

		SINT32 i;

		for (i = 0; i != E_Q3_MESH_SIZE; ++i)
		{
			if (Mesh[i])
			{
				Mesh[i]->drop();
				Mesh[i] = 0;
			}
		}

		for (i = 1; i < NumModels; i++)
		{
			BrushEntities[i]->drop();
		}
		delete[] BrushEntities; BrushEntities = 0;

		ReleaseShader();
		ReleaseEntity();
	}


	//! loads a level from a .bsp-File. Also tries to load all needed textures. Returns true if successful.
	bool CQ3LevelMesh::loadFile(IReadFile* file)
	{
		if (!file)
			return false;

		LevelName = file->getFileName();

		file->read(&header, sizeof(tBSPHeader));

#ifdef __BIG_ENDIAN__
		header.strID = Byteswap::byteswap(header.strID);
		header.version = Byteswap::byteswap(header.version);
#endif

		if ((header.strID != 0x50534249 ||			// IBSP
			(header.version != 0x2e			// Quake3
			&& header.version != 0x2f			// rtcw
			)
			)
			&&
			(header.strID != 0x50534252 || header.version != 1) // RBSP, starwars jedi, sof
			)
		{
			Printer::log("Could not load .bsp file, unknown header.", file->getFileName(), LML_ERROR);
			return false;
		}

#if 0
		if (header.strID == 0x50534252)	// RBSP Raven
		{
			LoadParam.swapHeader = 1;
		}
#endif

		// now read lumps
		file->read(&Lumps[0], sizeof(tBSPLump)*kMaxLumps);

		SINT32 i;
		if (LoadParam.swapHeader)
		{
			for (i = 0; i < kMaxLumps; ++i)
			{
				Lumps[i].offset = Byteswap::byteswap(Lumps[i].offset);
				Lumps[i].length = Byteswap::byteswap(Lumps[i].length);
			}
		}

		ReleaseEntity();

		// load everything
		loadEntities(&Lumps[kEntities], file);			// load the entities
		loadTextures(&Lumps[kShaders], file);			// Load the textures
		loadLightmaps(&Lumps[kLightmaps], file);		// Load the lightmaps
		loadVerts(&Lumps[kVertices], file);				// Load the vertices
		loadFaces(&Lumps[kFaces], file);				// Load the faces
		loadPlanes(&Lumps[kPlanes], file);				// Load the Planes of the BSP
		loadNodes(&Lumps[kNodes], file);				// load the Nodes of the BSP
		loadLeafs(&Lumps[kLeafs], file);				// load the Leafs of the BSP
		loadLeafFaces(&Lumps[kLeafFaces], file);		// load the Faces of the Leafs of the BSP
		loadVisData(&Lumps[kVisData], file);			// load the visibility data of the clusters
		loadModels(&Lumps[kModels], file);				// load the models
		loadMeshVerts(&Lumps[kMeshVerts], file);		// load the mesh vertices
		loadBrushes(&Lumps[kBrushes], file);			// load the brushes of the BSP
		loadBrushSides(&Lumps[kBrushSides], file);		// load the brushsides of the BSP
		loadLeafBrushes(&Lumps[kLeafBrushes], file);	// load the brushes of the leaf
		loadFogs(&Lumps[kFogs], file);					// load the fogs

		loadTextures();
		constructMesh();
		solveTJunction();

		cleanMeshes();
		calcBoundingBoxes();
		cleanLoader();

		return true;
	}

	/*!
	*/
	void CQ3LevelMesh::cleanLoader()
	{
		delete[] Textures; Textures = 0;
		delete[] LightMaps; LightMaps = 0;
		delete[] Vertices; Vertices = 0;
		delete[] Faces; Faces = 0;
		delete[] Models; Models = 0;
		delete[] Planes; Planes = 0;
		delete[] Nodes; Nodes = 0;
		delete[] Leafs; Leafs = 0;
		delete[] LeafFaces; LeafFaces = 0;
		delete[] MeshVerts; MeshVerts = 0;
		delete[] Brushes; Brushes = 0;

		Lightmap.clear();
		Tex.clear();
	}

	//! returns the amount of frames in milliseconds. If the amount is 1, it is a static (=non animated) mesh.
	UINT32 CQ3LevelMesh::getFrameCount() const
	{
		return 1;
	}


	//! returns the animated mesh based on a detail level. 0 is the lowest, 255 the highest detail. Note, that some Meshes will ignore the detail level.
	IMesh* CQ3LevelMesh::getMesh(SINT32 frameInMs, SINT32 detailLevel, SINT32 startFrameLoop, SINT32 endFrameLoop)
	{
		return Mesh[frameInMs];
	}


	void CQ3LevelMesh::loadTextures(tBSPLump* l, IReadFile* file)
	{
		NumTextures = l->length / sizeof(tBSPTexture);
		if (!NumTextures)
			return;
		Textures = new tBSPTexture[NumTextures];

		file->seek(l->offset);
		file->read(Textures, l->length);

		if (LoadParam.swapHeader)
		{
			for (SINT32 i = 0; i < NumTextures; ++i)
			{
				Textures[i].flags = Byteswap::byteswap(Textures[i].flags);
				Textures[i].contents = Byteswap::byteswap(Textures[i].contents);
				//Printer::log("Loaded texture", Textures[i].strName, LML_NORMAL);
			}
		}
	}


	void CQ3LevelMesh::loadLightmaps(tBSPLump* l, IReadFile* file)
	{
		NumLightMaps = l->length / sizeof(tBSPLightmap);
		if (!NumLightMaps)
			return;
		LightMaps = new tBSPLightmap[NumLightMaps];

		file->seek(l->offset);
		file->read(LightMaps, l->length);
	}

	/*!
	*/
	void CQ3LevelMesh::loadVerts(tBSPLump* l, IReadFile* file)
	{
		NumVertices = l->length / sizeof(tBSPVertex);
		if (!NumVertices)
			return;
		Vertices = new tBSPVertex[NumVertices];

		file->seek(l->offset);
		file->read(Vertices, l->length);

		if (LoadParam.swapHeader)
			for (SINT32 i = 0; i < NumVertices; i++)
			{
				Vertices[i].vPosition[0] = Byteswap::byteswap(Vertices[i].vPosition[0]);
				Vertices[i].vPosition[1] = Byteswap::byteswap(Vertices[i].vPosition[1]);
				Vertices[i].vPosition[2] = Byteswap::byteswap(Vertices[i].vPosition[2]);
				Vertices[i].vTextureCoord[0] = Byteswap::byteswap(Vertices[i].vTextureCoord[0]);
				Vertices[i].vTextureCoord[1] = Byteswap::byteswap(Vertices[i].vTextureCoord[1]);
				Vertices[i].vLightmapCoord[0] = Byteswap::byteswap(Vertices[i].vLightmapCoord[0]);
				Vertices[i].vLightmapCoord[1] = Byteswap::byteswap(Vertices[i].vLightmapCoord[1]);
				Vertices[i].vNormal[0] = Byteswap::byteswap(Vertices[i].vNormal[0]);
				Vertices[i].vNormal[1] = Byteswap::byteswap(Vertices[i].vNormal[1]);
				Vertices[i].vNormal[2] = Byteswap::byteswap(Vertices[i].vNormal[2]);
			}
	}


	/*!
	*/
	void CQ3LevelMesh::loadFaces(tBSPLump* l, IReadFile* file)
	{
		NumFaces = l->length / sizeof(tBSPFace);
		if (!NumFaces)
			return;
		Faces = new tBSPFace[NumFaces];

		file->seek(l->offset);
		file->read(Faces, l->length);

		if (LoadParam.swapHeader)
		{
			for (SINT32 i = 0; i < NumFaces; i++)
			{
				Faces[i].textureID = Byteswap::byteswap(Faces[i].textureID);
				Faces[i].fogNum = Byteswap::byteswap(Faces[i].fogNum);
				Faces[i].type = Byteswap::byteswap(Faces[i].type);
				Faces[i].vertexIndex = Byteswap::byteswap(Faces[i].vertexIndex);
				Faces[i].numOfVerts = Byteswap::byteswap(Faces[i].numOfVerts);
				Faces[i].meshVertIndex = Byteswap::byteswap(Faces[i].meshVertIndex);
				Faces[i].numMeshVerts = Byteswap::byteswap(Faces[i].numMeshVerts);
				Faces[i].lightmapID = Byteswap::byteswap(Faces[i].lightmapID);
				Faces[i].lMapCorner[0] = Byteswap::byteswap(Faces[i].lMapCorner[0]);
				Faces[i].lMapCorner[1] = Byteswap::byteswap(Faces[i].lMapCorner[1]);
				Faces[i].lMapSize[0] = Byteswap::byteswap(Faces[i].lMapSize[0]);
				Faces[i].lMapSize[1] = Byteswap::byteswap(Faces[i].lMapSize[1]);
				Faces[i].lMapPos[0] = Byteswap::byteswap(Faces[i].lMapPos[0]);
				Faces[i].lMapPos[1] = Byteswap::byteswap(Faces[i].lMapPos[1]);
				Faces[i].lMapPos[2] = Byteswap::byteswap(Faces[i].lMapPos[2]);
				Faces[i].lMapBitsets[0][0] = Byteswap::byteswap(Faces[i].lMapBitsets[0][0]);
				Faces[i].lMapBitsets[0][1] = Byteswap::byteswap(Faces[i].lMapBitsets[0][1]);
				Faces[i].lMapBitsets[0][2] = Byteswap::byteswap(Faces[i].lMapBitsets[0][2]);
				Faces[i].lMapBitsets[1][0] = Byteswap::byteswap(Faces[i].lMapBitsets[1][0]);
				Faces[i].lMapBitsets[1][1] = Byteswap::byteswap(Faces[i].lMapBitsets[1][1]);
				Faces[i].lMapBitsets[1][2] = Byteswap::byteswap(Faces[i].lMapBitsets[1][2]);
				Faces[i].vNormal[0] = Byteswap::byteswap(Faces[i].vNormal[0]);
				Faces[i].vNormal[1] = Byteswap::byteswap(Faces[i].vNormal[1]);
				Faces[i].vNormal[2] = Byteswap::byteswap(Faces[i].vNormal[2]);
				Faces[i].size[0] = Byteswap::byteswap(Faces[i].size[0]);
				Faces[i].size[1] = Byteswap::byteswap(Faces[i].size[1]);
			}
		}
	}


	/*!
	*/
	void CQ3LevelMesh::loadPlanes(tBSPLump* l, IReadFile* file)
	{
		// ignore
	}


	/*!
	*/
	void CQ3LevelMesh::loadNodes(tBSPLump* l, IReadFile* file)
	{
		// ignore
	}


	/*!
	*/
	void CQ3LevelMesh::loadLeafs(tBSPLump* l, IReadFile* file)
	{
		// ignore
	}


	/*!
	*/
	void CQ3LevelMesh::loadLeafFaces(tBSPLump* l, IReadFile* file)
	{
		// ignore
	}


	/*!
	*/
	void CQ3LevelMesh::loadVisData(tBSPLump* l, IReadFile* file)
	{
		// ignore
	}


	/*!
	*/
	void CQ3LevelMesh::loadEntities(tBSPLump* l, IReadFile* file)
	{
		vector<UINT8>::type entity;
		entity.reserve(l->length + 2);
		entity[l->length + 1] = 0;

		file->seek(l->offset);
		file->read(entity.begin()._Ptr, l->length);

		parser_parse(entity.begin()._Ptr, l->length, &CQ3LevelMesh::scriptcallback_entity);
	}


	/*!
	load fog brushes
	*/
	void CQ3LevelMesh::loadFogs(tBSPLump* l, IReadFile* file)
	{
		UINT32 files = l->length / sizeof(tBSPFog);

		file->seek(l->offset);
		tBSPFog fog;
		const IShader *shader;
		STexShader t;
		for (UINT32 i = 0; i != files; ++i)
		{
			file->read(&fog, sizeof(fog));

			shader = getShader(fog.shader);
			t.Texture = 0;
			t.ShaderID = shader ? shader->ID : -1;

			FogMap.push_back(t);
		}
	}


	/*!
	load models named in bsp
	*/
	void CQ3LevelMesh::loadModels(tBSPLump* l, IReadFile* file)
	{
		NumModels = l->length / sizeof(tBSPModel);
		Models = new tBSPModel[NumModels];

		file->seek(l->offset);
		file->read(Models, l->length);

		if (LoadParam.swapHeader)
		{
			for (SINT32 i = 0; i < NumModels; i++)
			{
				Models[i].min[0] = Byteswap::byteswap(Models[i].min[0]);
				Models[i].min[1] = Byteswap::byteswap(Models[i].min[1]);
				Models[i].min[2] = Byteswap::byteswap(Models[i].min[2]);
				Models[i].max[0] = Byteswap::byteswap(Models[i].max[0]);
				Models[i].max[1] = Byteswap::byteswap(Models[i].max[1]);
				Models[i].max[2] = Byteswap::byteswap(Models[i].max[2]);

				Models[i].faceIndex = Byteswap::byteswap(Models[i].faceIndex);
				Models[i].numOfFaces = Byteswap::byteswap(Models[i].numOfFaces);
				Models[i].brushIndex = Byteswap::byteswap(Models[i].brushIndex);
				Models[i].numOfBrushes = Byteswap::byteswap(Models[i].numOfBrushes);
			}
		}

		BrushEntities = new SMesh*[NumModels];
	}

	/*!
	*/
	void CQ3LevelMesh::loadMeshVerts(tBSPLump* l, IReadFile* file)
	{
		NumMeshVerts = l->length / sizeof(SINT32);
		if (!NumMeshVerts)
			return;
		MeshVerts = new SINT32[NumMeshVerts];

		file->seek(l->offset);
		file->read(MeshVerts, l->length);

		if (LoadParam.swapHeader)
		{
			for (int i = 0; i < NumMeshVerts; i++)
				MeshVerts[i] = Byteswap::byteswap(MeshVerts[i]);
		}
	}

	/*!
	*/
	void CQ3LevelMesh::loadBrushes(tBSPLump* l, IReadFile* file)
	{
		// ignore
	}

	/*!
	*/
	void CQ3LevelMesh::loadBrushSides(tBSPLump* l, IReadFile* file)
	{
		// ignore
	}

	/*!
	*/
	void CQ3LevelMesh::loadLeafBrushes(tBSPLump* l, IReadFile* file)
	{
		// ignore
	}

	/*!
	*/
	inline bool isQ3WhiteSpace(const UINT8 symbol)
	{
		return symbol == ' ' || symbol == '\t' || symbol == '\r';
	}

	/*!
	*/
	inline bool isQ3ValidName(const UINT8 symbol)
	{
		return	(symbol >= 'a' && symbol <= 'z') ||
			(symbol >= 'A' && symbol <= 'Z') ||
			(symbol >= '0' && symbol <= '9') ||
			(symbol == '/' || symbol == '_' || symbol == '.');
	}

	/*!
	*/
	void CQ3LevelMesh::parser_nextToken()
	{
		UINT8 symbol;

		Parser.token = "";
		Parser.tokenresult = Q3_TOKEN_UNRESOLVED;

		// skip white space
		do
		{
			if (Parser.index >= Parser.sourcesize)
			{
				Parser.tokenresult = Q3_TOKEN_EOF;
				return;
			}

			symbol = Parser.source[Parser.index];
			Parser.index += 1;
		} while (isQ3WhiteSpace(symbol));

		// first symbol, one symbol
		switch (symbol)
		{
		case 0:
			Parser.tokenresult = Q3_TOKEN_EOF;
			return;

		case '/':
			// comment or divide
			if (Parser.index >= Parser.sourcesize)
			{
				Parser.tokenresult = Q3_TOKEN_EOF;
				return;
			}
			symbol = Parser.source[Parser.index];
			Parser.index += 1;
			if (isQ3WhiteSpace(symbol))
			{
				Parser.tokenresult = Q3_TOKEN_MATH_DIVIDE;
				return;
			}
			else
				if (symbol == '*')
				{
					// C-style comment in quake?
				}
				else
					if (symbol == '/')
					{
						// skip to eol
						do
						{
							if (Parser.index >= Parser.sourcesize)
							{
								Parser.tokenresult = Q3_TOKEN_EOF;
								return;
							}
							symbol = Parser.source[Parser.index];
							Parser.index += 1;
						} while (symbol != '\n');
						Parser.tokenresult = Q3_TOKEN_COMMENT;
						return;
					}
			// take /[name] as valid token..?!?!?. mhmm, maybe
			break;

		case '\n':
			Parser.tokenresult = Q3_TOKEN_EOL;
			return;
		case '{':
			Parser.tokenresult = Q3_TOKEN_START_LIST;
			return;
		case '}':
			Parser.tokenresult = Q3_TOKEN_END_LIST;
			return;

		case '"':
			// string literal
			do
			{
				if (Parser.index >= Parser.sourcesize)
				{
					Parser.tokenresult = Q3_TOKEN_EOF;
					return;
				}
				symbol = Parser.source[Parser.index];
				Parser.index += 1;
				if (symbol != '"')
					Parser.token += symbol;//Parser.token.append(symbol);
			} while (symbol != '"');
			Parser.tokenresult = Q3_TOKEN_ENTITY;
			return;
		}

		// user identity
		Parser.token += symbol;//Parser.token.append(symbol);

		// continue till whitespace
		bool validName = true;
		do
		{
			if (Parser.index >= Parser.sourcesize)
			{
				Parser.tokenresult = Q3_TOKEN_EOF;
				return;
			}
			symbol = Parser.source[Parser.index];

			validName = isQ3ValidName(symbol);
			if (validName)
			{
				Parser.token += symbol;//Parser.token.append(symbol);
				Parser.index += 1;
			}
		} while (validName);

		Parser.tokenresult = Q3_TOKEN_TOKEN;
		return;
	}


	/*
	parse entity & shader
	calls callback on content in {}
	*/
	void CQ3LevelMesh::parser_parse(const void * data, const UINT32 size, CQ3LevelMesh::tParserCallback callback)
	{
		Parser.source = static_cast<const c8*>(data);
		Parser.sourcesize = size;
		Parser.index = 0;

		SVarGroupList *groupList;

		SINT32 active;
		SINT32 last;

		SVariable entity("");

		groupList = new SVarGroupList();

		groupList->VariableGroup.push_back(SVarGroup());
		active = last = 0;

		do
		{
			parser_nextToken();

			switch (Parser.tokenresult)
			{
			case Q3_TOKEN_START_LIST:
			{
				//stack = min_( stack + 1, 7 );

				groupList->VariableGroup.push_back(SVarGroup());
				last = active;
				active = groupList->VariableGroup.size() - 1;
				entity.clear();
			}  break;

			// a unregisterd variable is finished
			case Q3_TOKEN_EOL:
			{
				if (entity.isValid())
				{
					groupList->VariableGroup[active].Variable.push_back(entity);
					entity.clear();
				}
			} break;

			case Q3_TOKEN_TOKEN:
			case Q3_TOKEN_ENTITY:
			{
				//Parser.token.make_lower();
				StringUtil::toLowerCase(Parser.token);

				// store content based on line-delemiter
				if (0 == entity.isValid())
				{
					entity.name = Parser.token;
					entity.content = "";

				}
				else
				{
					if (entity.content.size())
					{
						entity.content += " ";
					}
					entity.content += Parser.token;
				}
			} break;

			case Q3_TOKEN_END_LIST:
			{
				//stack = max_( stack - 1, 0 );

				// close tag for first
				if (active == 1)
				{
					(this->*callback)(groupList, Q3_TOKEN_END_LIST);

					// new group
					groupList->drop();
					groupList = new SVarGroupList();
					groupList->VariableGroup.push_back(SVarGroup());
					last = 0;
				}

				active = last;
				entity.clear();

			} break;

			default:
				break;
			}

		} while (Parser.tokenresult != Q3_TOKEN_EOF);

		(this->*callback)(groupList, Q3_TOKEN_EOF);

		groupList->drop();
	}


	/*
	this loader applies only textures for stage 1 & 2
	*/
	SINT32 CQ3LevelMesh::setShaderFogMaterial(SMaterial &material, const tBSPFace * face) const
	{
		material.MaterialType = EMT_SOLID;
		material.Wireframe = false;
		material.Lighting = false;
		material.BackfaceCulling = false;
		material.setTexture(0, 0);
		material.setTexture(1, 0);
		material.setTexture(2, 0);
		material.setTexture(3, 0);
		material.ZBuffer = ECFN_LESSEQUAL;
		material.ZWriteEnable = false;
		material.MaterialTypeParam = 0.f;

		SINT32 shaderState = -1;

		if ((UINT32)face->fogNum < FogMap.size())
		{
			material.setTexture(0, FogMap[face->fogNum].Texture);
			shaderState = FogMap[face->fogNum].ShaderID;
		}

		return shaderState;

	}
	/*
	this loader applies only textures for stage 1 & 2
	*/
	SINT32 CQ3LevelMesh::setShaderMaterial(SMaterial &material, const tBSPFace * face) const
	{
		material.MaterialType = EMT_SOLID;
		material.Wireframe = false;
		material.Lighting = false;
		material.BackfaceCulling = true;
		material.setTexture(0, 0);
		material.setTexture(1, 0);
		material.setTexture(2, 0);
		material.setTexture(3, 0);
		material.ZBuffer = ECFN_LESSEQUAL;
		material.ZWriteEnable = true;
		material.MaterialTypeParam = 0.f;

		SINT32 shaderState = -1;

		if (face->textureID >= 0 && face->textureID < (SINT32)Tex.size())
		{
			material.setTexture(0, Tex[face->textureID].Texture);
			shaderState = Tex[face->textureID].ShaderID;
		}

		if (face->lightmapID >= 0 && face->lightmapID < (SINT32)Lightmap.size())
		{
			material.setTexture(1, Lightmap[face->lightmapID]);
			material.MaterialType = LoadParam.defaultLightMapMaterial;
		}

		// store shader ID
		material.MaterialTypeParam2 = (FLOAT32)shaderState;

		const IShader *shader = getShader(shaderState);
		if (0 == shader)
			return shaderState;

		return shaderState;

#if 0
		const SVarGroup *group;


		// generic
		group = shader->getGroup(1);
		if (group)
		{
			material.BackfaceCulling = getCullingFunction(group->get("cull"));

			if (group->isDefined("surfaceparm", "nolightmap"))
			{
				material.MaterialType = EMT_SOLID;
				material.setTexture(1, 0);
			}

		}

		// try to get the best of the 8 texture stages..

		// texture 1, texture 2
		UINT32 startPos;
		for (SINT32 g = 2; g <= 3; ++g)
		{
			group = shader->getGroup(g);
			if (0 == group)
				continue;

			startPos = 0;

			if (group->isDefined("depthwrite"))
			{
				material.zWriteEnable = true;
			}

			SBlendFunc blendfunc(LoadParam.defaultModulate);
			getBlendFunc(group->get("blendfunc"), blendfunc);
			getBlendFunc(group->get("alphafunc"), blendfunc);

			if (0 == LoadParam.alpharef &&
				(blendfunc.type == EMT_TRANSPARENT_ALPHA_CHANNEL ||
				blendfunc.type == EMT_TRANSPARENT_ALPHA_CHANNEL_REF
				)
				)
			{
				blendfunc.type = EMT_TRANSPARENT_ALPHA_CHANNEL;
				blendfunc.param0 = 0.f;
			}

			material.MaterialType = blendfunc.type;
			material.MaterialTypeParam = blendfunc.param0;

			// try if we can match better
			shaderState |= (material.MaterialType == EMT_SOLID) ? 0x00020000 : 0;
		}

		//material.BackfaceCulling = false;

		if (shader->VarGroup->VariableGroup.size() <= 4)
		{
			shaderState |= 0x00010000;
		}

		material.MaterialTypeParam2 = (FLOAT32)shaderState;
		return shaderState;
#endif
	}

	/*!
	Internal function to build a mesh.
	*/
	SMesh** CQ3LevelMesh::buildMesh(SINT32 num)
	{
		SMesh** newmesh = new SMesh *[Quake3::E_Q3_MESH_SIZE];

		SINT32 i, j, k, s;

		for (i = 0; i < E_Q3_MESH_SIZE; i++)
		{
			newmesh[i] = new SMesh();
		}

		SINT32 *index;

		S3DVertex2TCoords temp[3];
		SMaterial material;
		SMaterial material2;

		SToBuffer item[E_Q3_MESH_SIZE];
		UINT32 itemSize;

		for (i = Models[num].faceIndex; i < Models[num].numOfFaces + Models[num].faceIndex; ++i)
		{
			const tBSPFace * face = Faces + i;

			SINT32 shaderState = setShaderMaterial(material, face);
			itemSize = 0;

			const IShader *shader = getShader(shaderState);

			if (face->fogNum >= 0)
			{
				setShaderFogMaterial(material2, face);
				item[itemSize].index = E_Q3_MESH_FOG;
				item[itemSize].takeVertexColor = 1;
				itemSize += 1;
			}

			switch (face->type)
			{
			case 1: // normal polygons
			case 2: // patches
			case 3: // meshes
				if (0 == shader)
				{
					if (LoadParam.cleanUnResolvedMeshes || material.getTexture(0))
					{
						item[itemSize].takeVertexColor = 1;
						item[itemSize].index = E_Q3_MESH_GEOMETRY;
						itemSize += 1;
					}
					else
					{
						item[itemSize].takeVertexColor = 1;
						item[itemSize].index = E_Q3_MESH_UNRESOLVED;
						itemSize += 1;
					}
				}
				else
				{
					item[itemSize].takeVertexColor = 1;
					item[itemSize].index = E_Q3_MESH_ITEMS;
					itemSize += 1;
				}
				break;

			case 4: // billboards
				//item[itemSize].takeVertexColor = 1;
				//item[itemSize].index = E_Q3_MESH_ITEMS;
				//itemSize += 1;
				break;

			}

			for (UINT32 g = 0; g != itemSize; ++g)
			{
				SMeshBufferLightMap* buffer = 0;

				if (item[g].index == E_Q3_MESH_GEOMETRY)
				{
					if (0 == item[g].takeVertexColor)
					{
						item[g].takeVertexColor = material.getTexture(0) == 0 || material.getTexture(1) == 0;
					}

					if (Faces[i].lightmapID < -1 || Faces[i].lightmapID > NumLightMaps - 1)
					{
						Faces[i].lightmapID = -1;
					}

#if 0
					// there are lightmapsids and textureid with -1
					const SINT32 tmp_index = ((Faces[i].lightmapID + 1) * (NumTextures + 1)) + (Faces[i].textureID + 1);
					buffer = (SMeshBufferLightMap*)newmesh[E_Q3_MESH_GEOMETRY]->getMeshBuffer(tmp_index);
					buffer->setHardwareMappingHint(EHM_STATIC);
					buffer->getMaterial() = material;
#endif
				}

				// Construct a unique mesh for each shader or combine meshbuffers for same shader
				if (0 == buffer)
				{

					if (LoadParam.mergeShaderBuffer == 1)
					{
						// combine
						buffer = (SMeshBufferLightMap*)newmesh[item[g].index]->getMeshBuffer(
							item[g].index != E_Q3_MESH_FOG ? material : material2);
					}

					// create a seperate mesh buffer
					if (0 == buffer)
					{
						buffer = new SMeshBufferLightMap();
						newmesh[item[g].index]->addMeshBuffer(buffer);
						buffer->drop();
						buffer->getMaterial() = item[g].index != E_Q3_MESH_FOG ? material : material2;
						if (item[g].index == E_Q3_MESH_GEOMETRY)
							buffer->setHardwareMappingHint(EHM_STATIC);
					}
				}


				switch (Faces[i].type)
				{
				case 4: // billboards
					break;
				case 2: // patches
					createCurvedSurface_bezier(buffer, i,
						LoadParam.patchTesselation,
						item[g].takeVertexColor
						);
					break;

				case 1: // normal polygons
				case 3: // mesh vertices
					index = MeshVerts + face->meshVertIndex;
					k = buffer->getVertexCount();

					// reallocate better if many small meshes are used
					s = buffer->getIndexCount() + face->numMeshVerts;
					//if (buffer->Indices.allocated_size() < (UINT32)s)
					if (buffer->Indices.capacity() < (UINT32)s)
					{
						if (buffer->Indices.capacity() > 0 &&
							face->numMeshVerts < 20 && NumFaces > 1000
							)
						{
							s = buffer->getIndexCount() + (NumFaces >> 3 * face->numMeshVerts);
						}
						buffer->Indices.reserve(s);
					}

					for (j = 0; j < face->numMeshVerts; ++j)
					{
						buffer->Indices.push_back(k + index[j]);
					}

					s = k + face->numOfVerts;
					if (buffer->Vertices.capacity() < (UINT32)s)
					{
						if (buffer->Indices.capacity() > 0 &&
							face->numOfVerts < 20 && NumFaces > 1000
							)
						{
							s = buffer->getIndexCount() + (NumFaces >> 3 * face->numOfVerts);
						}
						buffer->Vertices.reserve(s);
					}
					for (j = 0; j != face->numOfVerts; ++j)
					{
						copy(&temp[0], &Vertices[j + face->vertexIndex], item[g].takeVertexColor);
						buffer->Vertices.push_back(temp[0]);
					}
					break;

				} // end switch
			}
		}

		return newmesh;
	}

	/*!
	*/
	void CQ3LevelMesh::solveTJunction()
	{
	}

	/*!
	constructs a mesh from the quake 3 level file.
	*/
	void CQ3LevelMesh::constructMesh()
	{
		if (LoadParam.verbose > 0)
		{
			LoadParam.startTime = Timer::getRealTime();

			if (LoadParam.verbose > 1)
			{
				snprintf(buf, sizeof(buf),
					"Quake3::constructMesh start to create %d faces, %d vertices,%d mesh vertices",
					NumFaces,
					NumVertices,
					NumMeshVerts
					);
				Printer::log(buf, LML_NORMAL);
			}

		}

		SINT32 i, j;

		// First the main level
		SMesh **tmp = buildMesh(0);

		for (i = 0; i < E_Q3_MESH_SIZE; i++)
		{
			Mesh[i] = tmp[i];
		}
		delete[] tmp;

		// Then the brush entities

		for (i = 1; i < NumModels; i++)
		{
			tmp = buildMesh(i);
			BrushEntities[i] = tmp[0];

			// We only care about the main geometry here
			for (j = 1; j < E_Q3_MESH_SIZE; j++)
			{
				tmp[j]->drop();
			}
			delete[] tmp;
		}

		if (LoadParam.verbose > 0)
		{
			LoadParam.endTime = Timer::getRealTime();

			snprintf(buf, sizeof(buf),
				"Quake3::constructMesh needed %04d ms to create %d faces, %d vertices,%d mesh vertices",
				LoadParam.endTime - LoadParam.startTime,
				NumFaces,
				NumVertices,
				NumMeshVerts
				);
			Printer::log(buf, LML_NORMAL);
		}

	}


	void CQ3LevelMesh::S3DVertex2TCoords_64::copy(S3DVertex2TCoords &dest) const
	{
#if defined (TJUNCTION_SOLVER_ROUND)
		dest.Pos.x = round_((FLOAT32)Pos.x);
		dest.Pos.y = round_((FLOAT32)Pos.y);
		dest.Pos.z = round_((FLOAT32)Pos.z);
#elif defined (TJUNCTION_SOLVER_0125)
		dest.Pos.x = (FLOAT32)(floor(Pos.x * 8.f + 0.5) * 0.125);
		dest.Pos.y = (FLOAT32)(floor(Pos.y * 8.f + 0.5) * 0.125);
		dest.Pos.z = (FLOAT32)(floor(Pos.z * 8.f + 0.5) * 0.125);
#else
		dest.Pos.x = (FLOAT32)Pos.x;
		dest.Pos.y = (FLOAT32)Pos.y;
		dest.Pos.z = (FLOAT32)Pos.z;
#endif

		dest.Normal.x = (FLOAT32)Normal.x;
		dest.Normal.y = (FLOAT32)Normal.y;
		dest.Normal.z = (FLOAT32)Normal.z;
		dest.Normal.normalize();

		dest.Color = Color;

		dest.TCoords.x = (FLOAT32)TCoords.x;
		dest.TCoords.y = (FLOAT32)TCoords.y;

		dest.TCoords2.x = (FLOAT32)TCoords2.x;
		dest.TCoords2.y = (FLOAT32)TCoords2.y;
	}


	void CQ3LevelMesh::copy(S3DVertex2TCoords_64 * dest, const tBSPVertex * source, SINT32 vertexcolor) const
	{
#if defined (TJUNCTION_SOLVER_ROUND)
		dest->Pos.x = round_(source->vPosition[0]);
		dest->Pos.y = round_(source->vPosition[2]);
		dest->Pos.z = round_(source->vPosition[1]);
#elif defined (TJUNCTION_SOLVER_0125)
		dest->Pos.x = (FLOAT32)(floor(source->vPosition[0] * 8.f + 0.5) * 0.125);
		dest->Pos.y = (FLOAT32)(floor(source->vPosition[2] * 8.f + 0.5) * 0.125);
		dest->Pos.z = (FLOAT32)(floor(source->vPosition[1] * 8.f + 0.5) * 0.125);
#else
		dest->Pos.x = source->vPosition[0];
		dest->Pos.y = source->vPosition[2];
		dest->Pos.z = source->vPosition[1];
#endif

		dest->Normal.x = source->vNormal[0];
		dest->Normal.y = source->vNormal[2];
		dest->Normal.z = source->vNormal[1];
		dest->Normal.normalize();

		dest->TCoords.x = source->vTextureCoord[0];
		dest->TCoords.y = source->vTextureCoord[1];
		dest->TCoords2.x = source->vLightmapCoord[0];
		dest->TCoords2.y = source->vLightmapCoord[1];

		if (vertexcolor)
		{
			//UINT32 a = SINT32_min( source->color[3] * LoadParam.defaultModulate, 255 );
			UINT32 a = source->color[3];
			UINT32 r = SINT32_min(source->color[0] * LoadParam.defaultModulate, 255);
			UINT32 g = SINT32_min(source->color[1] * LoadParam.defaultModulate, 255);
			UINT32 b = SINT32_min(source->color[2] * LoadParam.defaultModulate, 255);

			dest->Color = ColourValue(r * 1.f / 255.f,
				g * 1.f / 255.f, b * 1.f / 255.f, a * 1.f / 255.f);
		}
		else
		{
			dest->Color = ColourValue(1.f, 1.f, 1.f, 1.f);
		}
	}


	inline void CQ3LevelMesh::copy(S3DVertex2TCoords * dest, const tBSPVertex * source, SINT32 vertexcolor) const
	{
#if defined (TJUNCTION_SOLVER_ROUND)
		dest->Pos.x = round_(source->vPosition[0]);
		dest->Pos.y = round_(source->vPosition[2]);
		dest->Pos.z = round_(source->vPosition[1]);
#elif defined (TJUNCTION_SOLVER_0125)
		dest->Pos.x = (FLOAT32)(floor(source->vPosition[0] * 8.f + 0.5) * 0.125);
		dest->Pos.y = (FLOAT32)(floor(source->vPosition[2] * 8.f + 0.5) * 0.125);
		dest->Pos.z = (FLOAT32)(floor(source->vPosition[1] * 8.f + 0.5) * 0.125);
#else
		dest->Pos.x = source->vPosition[0];
		dest->Pos.y = source->vPosition[2];
		dest->Pos.z = source->vPosition[1];
#endif

		dest->Normal.x = source->vNormal[0];
		dest->Normal.y = source->vNormal[2];
		dest->Normal.z = source->vNormal[1];
		dest->Normal.normalize();

		dest->TCoords.x = source->vTextureCoord[0];
		dest->TCoords.y = source->vTextureCoord[1];
		dest->TCoords2.x = source->vLightmapCoord[0];
		dest->TCoords2.y = source->vLightmapCoord[1];

		if (vertexcolor)
		{
			//UINT32 a = SINT32_min( source->color[3] * LoadParam.defaultModulate, 255 );
			UINT32 a = source->color[3];
			UINT32 r = SINT32_min(source->color[0] * LoadParam.defaultModulate, 255);
			UINT32 g = SINT32_min(source->color[1] * LoadParam.defaultModulate, 255);
			UINT32 b = SINT32_min(source->color[2] * LoadParam.defaultModulate, 255);

			dest->Color.setAsARGB(a << 24 | r << 16 | g << 8 | b);
		}
		else
		{
			dest->Color.setAsARGB(0xFFFFFFFF);
		}
	}


	void CQ3LevelMesh::SBezier::tesselate(SINT32 level)
	{
		//Calculate how many vertices across/down there are
		SINT32 j, k;

		column[0].reserve(level + 1);
		column[1].reserve(level + 1);
		column[2].reserve(level + 1);

		const Real w = 0.0 + (1.0 / (Real)level);

		//Tesselate along the columns
		for (j = 0; j <= level; ++j)
		{
			const Real f = w * (Real)j;

			column[0][j] = control[0].getInterpolated_quadratic(control[3], control[6], f);
			column[1][j] = control[1].getInterpolated_quadratic(control[4], control[7], f);
			column[2][j] = control[2].getInterpolated_quadratic(control[5], control[8], f);
		}

		const UINT32 idx = Patch->Vertices.size();
		Patch->Vertices.reserve(idx + level*level);
		//Tesselate across the rows to get final vertices
		S3DVertex2TCoords v;
		S3DVertex2TCoords_64 f;
		for (j = 0; j <= level; ++j)
		{
			for (k = 0; k <= level; ++k)
			{
				f = column[0][j].getInterpolated_quadratic(column[1][j], column[2][j], w * (Real)k);
				f.copy(v);
				Patch->Vertices.push_back(v);
			}
		}

		Patch->Indices.reserve(Patch->Indices.size() + 6 * level*level);
		// connect
		for (j = 0; j < level; ++j)
		{
			for (k = 0; k < level; ++k)
			{
				const SINT32 inx = idx + (k * (level + 1)) + j;

				Patch->Indices.push_back(inx + 0);
				Patch->Indices.push_back(inx + (level + 1) + 0);
				Patch->Indices.push_back(inx + (level + 1) + 1);

				Patch->Indices.push_back(inx + 0);
				Patch->Indices.push_back(inx + (level + 1) + 1);
				Patch->Indices.push_back(inx + 1);
			}
		}
	}


	/*!
	no subdivision
	*/
	void CQ3LevelMesh::createCurvedSurface_nosubdivision(SMeshBufferLightMap* meshBuffer,
		SINT32 faceIndex,
		SINT32 patchTesselation,
		SINT32 storevertexcolor)
	{
		tBSPFace * face = &Faces[faceIndex];
		UINT32 j, k, m;

		// number of control points across & up
		const UINT32 controlWidth = face->size[0];
		const UINT32 controlHeight = face->size[1];
		if (0 == controlWidth || 0 == controlHeight)
			return;

		S3DVertex2TCoords v;

		m = meshBuffer->Vertices.size();
		meshBuffer->Vertices.reserve(m + controlHeight * controlWidth);
		for (j = 0; j != controlHeight * controlWidth; ++j)
		{
			copy(&v, &Vertices[face->vertexIndex + j], storevertexcolor);
			meshBuffer->Vertices.push_back(v);
		}

		meshBuffer->Indices.reserve(meshBuffer->Indices.size() + 6 * (controlHeight - 1) * (controlWidth - 1));
		for (j = 0; j != controlHeight - 1; ++j)
		{
			for (k = 0; k != controlWidth - 1; ++k)
			{
				meshBuffer->Indices.push_back(m + k + 0);
				meshBuffer->Indices.push_back(m + k + controlWidth + 0);
				meshBuffer->Indices.push_back(m + k + controlWidth + 1);

				meshBuffer->Indices.push_back(m + k + 0);
				meshBuffer->Indices.push_back(m + k + controlWidth + 1);
				meshBuffer->Indices.push_back(m + k + 1);
			}
			m += controlWidth;
		}
	}


	/*!
	*/
	void CQ3LevelMesh::createCurvedSurface_bezier(SMeshBufferLightMap* meshBuffer,
		SINT32 faceIndex,
		SINT32 patchTesselation,
		SINT32 storevertexcolor)
	{

		tBSPFace * face = &Faces[faceIndex];
		UINT32 j, k;

		// number of control points across & up
		const UINT32 controlWidth = face->size[0];
		const UINT32 controlHeight = face->size[1];

		if (0 == controlWidth || 0 == controlHeight)
			return;

		// number of biquadratic patches
		const UINT32 biquadWidth = (controlWidth - 1) / 2;
		const UINT32 biquadHeight = (controlHeight - 1) / 2;

		if (LoadParam.verbose > 1)
		{
			LoadParam.startTime = Timer::getRealTime();
		}

		// Create space for a temporary array of the patch's control points
		vector<S3DVertex2TCoords_64>::type controlPoint;
		controlPoint.reserve(controlWidth * controlHeight);

		for (j = 0; j < controlPoint.size(); ++j)
		{
			copy(&controlPoint[j], &Vertices[face->vertexIndex + j], storevertexcolor);
		}

		// create a temporary patch
		Bezier.Patch = new SMeshBufferLightMap();

		//Loop through the biquadratic patches
		for (j = 0; j < biquadHeight; ++j)
		{
			for (k = 0; k < biquadWidth; ++k)
			{
				// set up this patch
				const SINT32 inx = j*controlWidth * 2 + k * 2;

				// setup bezier control points for this patch
				Bezier.control[0] = controlPoint[inx + 0];
				Bezier.control[1] = controlPoint[inx + 1];
				Bezier.control[2] = controlPoint[inx + 2];
				Bezier.control[3] = controlPoint[inx + controlWidth + 0];
				Bezier.control[4] = controlPoint[inx + controlWidth + 1];
				Bezier.control[5] = controlPoint[inx + controlWidth + 2];
				Bezier.control[6] = controlPoint[inx + controlWidth * 2 + 0];
				Bezier.control[7] = controlPoint[inx + controlWidth * 2 + 1];
				Bezier.control[8] = controlPoint[inx + controlWidth * 2 + 2];

				Bezier.tesselate(patchTesselation);
			}
		}

		// stitch together with existing geometry
		// TODO: only border needs to be checked
		const UINT32 bsize = Bezier.Patch->getVertexCount();
		const UINT32 msize = meshBuffer->getVertexCount();
		/*
		for ( j = 0; j!= bsize; ++j )
		{
		const vector3df &v = Bezier.Patch->Vertices[j].Pos;

		for ( k = 0; k!= msize; ++k )
		{
		const vector3df &m = meshBuffer->Vertices[k].Pos;

		if ( !v.equals( m, tolerance ) )
		continue;

		meshBuffer->Vertices[k].Pos = v;
		//Bezier.Patch->Vertices[j].Pos = m;
		}
		}
		*/

		// add Patch to meshbuffer
		meshBuffer->Vertices.reserve(msize + bsize);
		for (j = 0; j != bsize; ++j)
		{
			meshBuffer->Vertices.push_back(Bezier.Patch->Vertices[j]);
		}

		// add indices to meshbuffer
		meshBuffer->Indices.reserve(meshBuffer->getIndexCount() + Bezier.Patch->getIndexCount());
		for (j = 0; j != Bezier.Patch->getIndexCount(); ++j)
		{
			meshBuffer->Indices.push_back(msize + Bezier.Patch->Indices[j]);
		}

		delete Bezier.Patch;

		if (LoadParam.verbose > 1)
		{
			LoadParam.endTime = Timer::getRealTime();

			snprintf(buf, sizeof(buf),
				"Quake3::createCurvedSurface_bezier needed %04d ms to create bezier patch.(%dx%d)",
				LoadParam.endTime - LoadParam.startTime,
				biquadWidth,
				biquadHeight
				);
			Printer::log(buf, LML_NORMAL);
		}

	}



	/*!
	Loads entities from file
	*/
	void CQ3LevelMesh::getConfiguration(IReadFile* file)
	{
		tBSPLump l;
		l.offset = file->getPos();
		l.length = file->getSize();

		vector<UINT8>::type entity;
		entity.reserve(l.length + 2);
		entity[l.length + 1] = 0;

		file->seek(l.offset);
		file->read(entity.begin()._Ptr, l.length);

		parser_parse(entity.begin()._Ptr, l.length, &CQ3LevelMesh::scriptcallback_config);

		if (Entity.size())
			Entity.back().name = file->getFileName();
	}


	//! get's an interface to the entities
	tQ3EntityList & CQ3LevelMesh::getEntityList()
	{
		//	Entity.sort();
		return Entity;
	}

	//! returns the requested brush entity
	IMesh* CQ3LevelMesh::getBrushEntityMesh(SINT32 num) const
	{
		if (num < 1 || num >= NumModels)
			return 0;

		return BrushEntities[num];
	}

	//! returns the requested brush entity
	IMesh* CQ3LevelMesh::getBrushEntityMesh(Quake3::IEntity &ent) const
	{
		// This is a helper function to parse the entity,
		// so you don't have to.

		SINT32 num;

		const Quake3::SVarGroup* group = ent.getGroup(1);
		const String& modnum = group->get("model");

		if (!group->isDefined("model"))
			return 0;

		const char *temp = modnum.c_str() + 1; // We skip the first character.
		num = strtol10(temp);

		return getBrushEntityMesh(num);
	}


	/*!
	*/
	const IShader * CQ3LevelMesh::getShader(UINT32 index) const
	{
		index &= 0xFFFF;

		if (index < Shader.size())
		{
			return &Shader[index];
		}

		return 0;
	}


	/*!
	loads the shader definition
	*/
	const IShader* CQ3LevelMesh::getShader(const c8 * filename, bool fileNameIsValid)
	{
		String searchName(filename);

		IShader search;
		search.name = searchName;
		//search.name.replace('\\', '/');
		search.name = StringUtil::replaceAll(search.name, "\\", "/");
		//search.name.make_lower();
		StringUtil::toLowerCase(search.name);


		String message;
		SINT32 index;

		//! is Shader already in cache?
		{
			//index = Shader.linear_search(search);
			vector<Quake3::IShader>::iterator it = find(Shader.begin(), Shader.end(), search);
			if (it != Shader.end())
			{
				if (LoadParam.verbose > 1)
				{
					message = searchName + " found " + it->name;
					Printer::log("Quake3:getShader", message.c_str(), LML_NORMAL);
				}

				//return &Shader[index];
				return it._Ptr;
			}
		}


		path loadFile;

		if (!fileNameIsValid)
		{
			// extract the shader name from the last path component in filename
			// "scripts/[name].shader"
			String cut(search.name);

			//SINT32 end = cut.findLast('/');
			SINT32 end = cut.rfind('/');
			//SINT32 start = cut.findLast('/', end - 1);
			SINT32 start = cut.rfind('/', end - 1);

			loadFile = LoadParam.scriptDir;
			loadFile.append(cut.substr(start, end - start));
			loadFile.append(".shader");
		}
		else
		{
			loadFile = search.name;
		}

		{
			// already loaded the file ?
			//index = ShaderFile.binary_search(loadFile);
			vector< String >::iterator it = std::find(ShaderFile.begin(), ShaderFile.end(), loadFile);

			if (it == ShaderFile.end())
				return 0;
		}
		// add file to loaded files
		ShaderFile.push_back(loadFile);

		if (!FileSystem->existFile(loadFile.c_str()))
		{
			if (LoadParam.verbose > 1)
			{
				message = loadFile + " for " + searchName + " failed ";
				Printer::log("Quake3:getShader", message.c_str(), LML_NORMAL);
			}
			return 0;
		}

		if (LoadParam.verbose)
		{
			message = loadFile + " for " + searchName;
			Printer::log("Quake3:getShader Load shader", message.c_str(), LML_NORMAL);
		}


		IReadFile *file = FileSystem->createAndOpenFile(loadFile.c_str());
		if (file)
		{
			getShader(file);
			file->drop();
		}


		{
			// search again
			//index = Shader.linear_search(search);
			vector < Quake3::IShader >::iterator it = find(Shader.begin(), Shader.end(), search);
			if (it == Shader.end())
			{
				return 0;
			}
			else
			{
				return it._Ptr;
			}

		}
		//return index >= 0 ? &Shader[index] : 0;

		//return index >= 0 ? &Shader[index] : 0;
	}

	/*!
	loads the shader definition
	*/
	void CQ3LevelMesh::getShader(IReadFile* file)
	{
		if (0 == file)
			return;

		// load script
		vector<UINT8>::type script;
		const long len = file->getSize();

		script.reserve(len + 2);

		file->seek(0);
		file->read(script.begin()._Ptr, len);
		script[len + 1] = 0;

		// start a parser instance
		parser_parse(script.begin()._Ptr, len, &CQ3LevelMesh::scriptcallback_shader);
	}


	//! adding default shaders
	void CQ3LevelMesh::InitShader()
	{
		ReleaseShader();

		IShader element;

		SVarGroup group;
		SVariable variable("noshader");

		group.Variable.push_back(variable);

		element.VarGroup = new SVarGroupList();
		element.VarGroup->VariableGroup.push_back(group);
		element.VarGroup->VariableGroup.push_back(SVarGroup());
		element.name = element.VarGroup->VariableGroup[0].Variable[0].name;
		element.ID = Shader.size();
		Shader.push_back(element);

		if (LoadParam.loadAllShaders)
		{
			EFileSystemType current = FileSystem->setFileListSystem(FILESYSTEM_VIRTUAL);
			path save = FileSystem->getWorkingDirectory();

			path newDir;
			newDir = "/";
			newDir += LoadParam.scriptDir;
			newDir += "/";
			FileSystem->changeWorkingDirectoryTo(newDir.c_str());

			String s;
			IFileList *fileList = FileSystem->createFileList();
			for (UINT32 i = 0; i < fileList->getFileCount(); ++i)
			{
				s = fileList->getFullFileName(i);
				if (s.find(".shader") >= 0)
				{
					if (0 == LoadParam.loadSkyShader && s.find("sky.shader") >= 0)
					{
					}
					else
					{
						getShader(s.c_str());
					}
				}
			}
			fileList->drop();

			FileSystem->changeWorkingDirectoryTo(save);
			FileSystem->setFileListSystem(current);
		}
	}


	//! script callback for shaders
	//! i'm having troubles with the reference counting, during callback.. resorting..
	void CQ3LevelMesh::ReleaseShader()
	{
		for (UINT32 i = 0; i != Shader.size(); ++i)
		{
			Shader[i].VarGroup->drop();
		}
		Shader.clear();
		ShaderFile.clear();
	}


	/*!
	*/
	void CQ3LevelMesh::ReleaseEntity()
	{
		for (UINT32 i = 0; i != Entity.size(); ++i)
		{
			Entity[i].VarGroup->drop();
		}
		Entity.clear();
	}


	// config in simple (Quake3) and advanced style
	void CQ3LevelMesh::scriptcallback_config(SVarGroupList *& grouplist, eToken token)
	{
		IShader element;

		if (token == Q3_TOKEN_END_LIST)
		{
			if (0 == grouplist->VariableGroup[0].Variable.size())
				return;

			element.name = grouplist->VariableGroup[0].Variable[0].name;
		}
		else
		{
			if (grouplist->VariableGroup.size() != 2)
				return;

			element.name = "configuration";
		}

		grouplist->grab();
		element.VarGroup = grouplist;
		element.ID = Entity.size();
		Entity.push_back(element);
	}


	// entity only has only one valid level.. and no assoziative name..
	void CQ3LevelMesh::scriptcallback_entity(SVarGroupList *& grouplist, eToken token)
	{
		if (token != Q3_TOKEN_END_LIST || grouplist->VariableGroup.size() != 2)
			return;

		grouplist->grab();

		IEntity element;
		element.VarGroup = grouplist;
		element.ID = Entity.size();
		element.name = grouplist->VariableGroup[1].get("classname");


		Entity.push_back(element);
	}


	//!. script callback for shaders
	void CQ3LevelMesh::scriptcallback_shader(SVarGroupList *& grouplist, eToken token)
	{
		if (token != Q3_TOKEN_END_LIST || grouplist->VariableGroup[0].Variable.size() == 0)
			return;


		IShader element;

		grouplist->grab();
		element.VarGroup = grouplist;
		element.name = element.VarGroup->VariableGroup[0].Variable[0].name;
		element.ID = Shader.size();
		/*
		String s;
		dumpShader ( s, &element );
		printf ( s.c_str () );
		*/
		Shader.push_back(element);
	}


	/*!
	delete all buffers without geometry in it.
	*/
	void CQ3LevelMesh::cleanMeshes()
	{
		if (0 == LoadParam.cleanUnResolvedMeshes)
			return;

		SINT32 i;

		// First the main level
		for (i = 0; i < E_Q3_MESH_SIZE; i++)
		{
			bool texture0important = (i == 0);

			cleanMesh(Mesh[i], texture0important);
		}

		// Then the brush entities
		for (i = 1; i < NumModels; i++)
		{
			cleanMesh(BrushEntities[i], true);
		}
	}

	void CQ3LevelMesh::cleanMesh(SMesh *m, const bool texture0important)
	{
		// delete all buffers without geometry in it.
		UINT32 run = 0;
		UINT32 remove = 0;

		IMeshBuffer *b;

		run = 0;
		remove = 0;

		if (LoadParam.verbose > 0)
		{
			LoadParam.startTime = Timer::getRealTime();
			if (LoadParam.verbose > 1)
			{
				snprintf(buf, sizeof(buf),
					"Quake3::cleanMeshes start for %d meshes",
					m->MeshBuffers.size()
					);
				Printer::log(buf, LML_NORMAL);
			}
		}

		UINT32 i = 0;
		SINT32 blockstart = -1;
		SINT32 blockcount = 0;

		while (i < m->MeshBuffers.size())
		{
			run += 1;

			b = m->MeshBuffers[i];

			if (b->getVertexCount() == 0 || b->getIndexCount() == 0 ||
				(texture0important && b->getMaterial().getTexture(0) == 0)
				)
			{
				if (blockstart < 0)
				{
					blockstart = i;
					blockcount = 0;
				}
				blockcount += 1;
				i += 1;

				// delete Meshbuffer
				i -= 1;
				remove += 1;
				b->drop();
				m->MeshBuffers.erase(m->MeshBuffers.begin() + i);
			}
			else
			{
				// clean blockwise
				if (blockstart >= 0)
				{
					if (LoadParam.verbose > 1)
					{
						snprintf(buf, sizeof(buf),
							"Quake3::cleanMeshes cleaning mesh %d %d size",
							blockstart,
							blockcount
							);
						Printer::log(buf, LML_NORMAL);
					}
					blockstart = -1;
				}
				i += 1;
			}
		}

		if (LoadParam.verbose > 0)
		{
			LoadParam.endTime = Timer::getRealTime();
			snprintf(buf, sizeof(buf),
				"Quake3::cleanMeshes needed %04d ms to clean %d of %d meshes",
				LoadParam.endTime - LoadParam.startTime,
				remove,
				run
				);
			Printer::log(buf, LML_NORMAL);
		}
	}


	// recalculate bounding boxes
	void CQ3LevelMesh::calcBoundingBoxes()
	{
		if (LoadParam.verbose > 0)
		{
			LoadParam.startTime = Timer::getRealTime();

			if (LoadParam.verbose > 1)
			{
				snprintf(buf, sizeof(buf),
					"Quake3::calcBoundingBoxes start create %d textures and %d lightmaps",
					NumTextures,
					NumLightMaps
					);
				Printer::log(buf, LML_NORMAL);
			}
		}

		SINT32 g;

		// create bounding box
		for (g = 0; g != E_Q3_MESH_SIZE; ++g)
		{
			for (UINT32 j = 0; j < Mesh[g]->MeshBuffers.size(); ++j)
			{
				((SMeshBufferLightMap*)Mesh[g]->MeshBuffers[j])->recalculateBoundingBox();
			}

			Mesh[g]->recalculateBoundingBox();
			// Mesh[0] is the main bbox
			if (g != 0)
				Mesh[0]->BoundingBox.addInternalBox(Mesh[g]->getBoundingBox());
		}

		for (g = 1; g < NumModels; g++)
		{
			for (UINT32 j = 0; j < BrushEntities[g]->MeshBuffers.size(); ++j)
			{
				((SMeshBufferLightMap*)BrushEntities[g]->MeshBuffers[j])->
					recalculateBoundingBox();
			}

			BrushEntities[g]->recalculateBoundingBox();
		}

		if (LoadParam.verbose > 0)
		{
			LoadParam.endTime = Timer::getRealTime();

			snprintf(buf, sizeof(buf),
				"Quake3::calcBoundingBoxes needed %04d ms to create %d textures and %d lightmaps",
				LoadParam.endTime - LoadParam.startTime,
				NumTextures,
				NumLightMaps
				);
			Printer::log(buf, LML_NORMAL);
		}
	}


	//! loads the textures
	void CQ3LevelMesh::loadTextures()
	{
		if (!Driver)
			return;

		if (LoadParam.verbose > 0)
		{
			LoadParam.startTime = Timer::getRealTime();

			if (LoadParam.verbose > 1)
			{
				snprintf(buf, sizeof(buf),
					"Quake3::loadTextures start create %d textures and %d lightmaps",
					NumTextures,
					NumLightMaps
					);
				Printer::log(buf, LML_NORMAL);
			}
		}

		c8 lightmapname[255];
		SINT32 t;

		// load lightmaps.
		Lightmap.reserve(NumLightMaps);

		/*
		bool oldMipMapState = Driver->getTextureCreationFlag(ETCF_CREATE_MIP_MAPS);
		Driver->setTextureCreationFlag(ETCF_CREATE_MIP_MAPS, false);
		*/
		dimension2d<UINT32> lmapsize(128, 128);

		IImage* lmapImg;
		for (t = 0; t < NumLightMaps; ++t)
		{
			sprintf(lightmapname, "%s.lightmap.%d", LevelName.c_str(), t);

			// lightmap is a CTexture::R8G8B8 format
			lmapImg = Driver->createImageFromData(
				ECF_R8G8B8, lmapsize,
				LightMaps[t].imageBits, false, true);

			Lightmap[t] = Driver->addTexture(lightmapname, lmapImg);
			lmapImg->drop();
		}

		//	Driver->setTextureCreationFlag(ETCF_CREATE_MIP_MAPS, oldMipMapState);

		// load textures
		Tex.reserve(NumTextures);

		const IShader* shader;

		String list;
		path check;
		tTexArray textureArray;

		// pre-load shaders
		for (t = 0; t < NumTextures; ++t)
		{
			shader = getShader(Textures[t].strName, false);
		}

		for (t = 0; t < NumTextures; ++t)
		{
			Tex[t].ShaderID = -1;
			Tex[t].Texture = 0;

			list = "";

			// get a shader ( if one exists )
			shader = getShader(Textures[t].strName, false);
			if (shader)
			{
				Tex[t].ShaderID = shader->ID;

				// if texture name == stage1 Texture map
				const SVarGroup * group;

				group = shader->getGroup(2);
				if (group)
				{
					if (cutFilenameExtension(check, group->get("map")) == Textures[t].strName)
					{
						list += check;
					}
					else
						if (check == "$lightmap")
						{
							// we check if lightmap is in stage 1 and texture in stage 2
							group = shader->getGroup(3);
							if (group)
								list += group->get("map");
						}
				}
			}
			else
			{
				// no shader, take it
				list += Textures[t].strName;
			}

			UINT32 pos = 0;
			getTextures(textureArray, list, pos, FileSystem, Driver);

			Tex[t].Texture = textureArray[0];
		}

		if (LoadParam.verbose > 0)
		{
			LoadParam.endTime = Timer::getRealTime();

			snprintf(buf, sizeof(buf),
				"Quake3::loadTextures needed %04d ms to create %d textures and %d lightmaps",
				LoadParam.endTime - LoadParam.startTime,
				NumTextures,
				NumLightMaps
				);
			Printer::log(buf, LML_NORMAL);
		}
	}


	//! Returns an axis aligned bounding box of the mesh.
	const AxisAlignedBox& CQ3LevelMesh::getBoundingBox() const
	{
		return Mesh[0]->getBoundingBox();
	}


	void CQ3LevelMesh::setBoundingBox(const AxisAlignedBox& box)
	{
		Mesh[0]->setBoundingBox(box);
	}


	//! Returns the type of the animated mesh.
	E_ANIMATED_MESH_TYPE CQ3LevelMesh::getMeshType() const
	{
		return EAMT_BSP;
	}

}

#endif