#include "SapphireCompileConifg.h"
#ifdef _SAPPHIRE_COMPILE_WITH_OBJ_LOADER_
#include "SapphireCOBJMeshFileLoader.h"
#include "SapphireIMeshManipulator.h"
#include "SapphireIVideoDriver.h"
#include "SapphireSMesh.h"
#include "SapphireSMeshBuffer.h"
#include "SapphireSAnimatedMesh.h"
#include "SapphireIReadFile.h"
#include "SapphireIAttributes.h"
#include "SapphireCoreUtil.h"
#include "SapphireOS.h"
#include "SapphireSceneParameters.h"
#include <consoleapi.h>
namespace Sapphire
{

#ifdef _DEBUG
#define _SAPPHIRE_DEBUG_OBJ_LOADER_
#endif

	static const UINT32 WORD_BUFFER_LENGTH = 512;

	//! Constructor
	COBJMeshFileLoader::COBJMeshFileLoader(ISceneManager* smgr, IFileSystem* fs)
		: SceneManager(smgr), FileSystem(fs)
	{
#ifdef _DEBUG
		setDebugName("COBJMeshFileLoader");
#endif
		
		if (FileSystem)
			FileSystem->grab();
	}


	//! destructor
	COBJMeshFileLoader::~COBJMeshFileLoader()
	{
		if (FileSystem)
			FileSystem->drop();
	}


	//! returns true if the file maybe is able to be loaded by this class
	//! based on the file extension (e.g. ".bsp")
	// 如果文件可以被这个类加载，返回true
	bool COBJMeshFileLoader::isALoadableFileExtension(const path& filename) const
	{
		return hasFileExtension(filename, "obj");
	}


	//! creates/loads an animated mesh from the file.
	//! \return Pointer to the created mesh. Returns 0 if loading failed.
	//! If you no longer need the mesh, you should call IAnimatedMesh::drop().
	//! See IReferenceCounted::drop() for more information.
	// 从这个文件加载一个动画网格
	// \return 指向创建网格的指针。如果失败返回0
	// 如果你长时间不需要这个网格，你可以调用IAnimatedMesh::drop()
	IAnimatedMesh* COBJMeshFileLoader::createMesh(IReadFile* file)
	{
		//获取文件大小
		const long filesize = file->getSize();
		if (!filesize)
			return 0;

		//字缓冲长度
		const UINT32 WORD_BUFFER_LENGTH = 512;

		//顶点缓冲区
		vector<Vector3>::type vertexBuffer;
		//法线缓冲区
		vector<Vector3>::type normalsBuffer;
		//纹理坐标缓冲区
		vector<Vector2>::type textureCoordBuffer;

		//OBj模型的Mtl文件对象（材质描述）
		//当前材质
		SObjMtl * currMtl = new SObjMtl();
		Materials.push_back(currMtl);
		//平滑组
		UINT32 smoothingGroup = 0;
		//获取完整文件名
		const path fullName = file->getFileName();
		//真实路径
		const path relPath = FileSystem->getFileDir(fullName) + "/";

		//字节缓冲区
		c8* buf = new c8[filesize];
		//清零
		memset(buf, 0, filesize);
		//将文件内容读入到缓冲区中
		file->read((void*)buf, filesize);
		//文件结束指针地址
		const c8* const bufEnd = buf + filesize;

		// Process obj information
		//下面处理OBJ信息
		//buf读取指针
		const c8* bufPtr = buf;
		//MTL名
		String grpName, mtlName;
		bool mtlChanged = false;
		bool useGroups = !SceneManager->getParameters()->getAttributeAsBool(OBJ_LOADER_IGNORE_GROUPS);
		bool useMaterials = !SceneManager->getParameters()->getAttributeAsBool(OBJ_LOADER_IGNORE_MATERIAL_FILES);
		while (bufPtr != bufEnd)
		{
			switch (bufPtr[0])
			{
			case 'm':	// mtllib (material)
			{
				if (useMaterials)
				{
					//名字缓冲区
					c8 name[WORD_BUFFER_LENGTH];
					bufPtr = goAndCopyNextWord(name, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
#ifdef _SAPPHIRE_DEBUG_OBJ_LOADER_
					Printer::log("Reading material file", name);
#endif
					//按照MTL名，读取mtl文件
					readMTL(name, relPath);
				}
			}
			break;
			//区分顶点，法线，纹理坐标.....
			case 'v':               // v, vn, vt
				switch (bufPtr[1])
				{
				case ' ':          // vertex
				{
					Vector3 vec;
					bufPtr = readVec3(bufPtr, vec, bufEnd);
					vertexBuffer.push_back(vec);
				}
				break;

				case 'n':       // normal
				{
					Vector3 vec;
					bufPtr = readVec3(bufPtr, vec, bufEnd);
					normalsBuffer.push_back(vec);
				}
				break;

				case 't':       // texcoord
				{
					Vector2 vec;
					bufPtr = readUV(bufPtr, vec, bufEnd);
					textureCoordBuffer.push_back(vec);
				}
				break;
				}
				break;

			case 'g': // group name  组名
			{
				c8 grp[WORD_BUFFER_LENGTH];
				bufPtr = goAndCopyNextWord(grp, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
#ifdef _SAPPHIRE_DEBUG_OBJ_LOADER_
				Printer::log("Loaded group start", grp, LML_DEBUG);
#endif
				if (useGroups)
				{
					if (0 != grp[0])
						grpName = grp;
					else
						grpName = "default";
				}
				mtlChanged = true;
			}
			break;

			case 's': // smoothing can be a group or off (equiv. to 0)
			{
				c8 smooth[WORD_BUFFER_LENGTH];
				bufPtr = goAndCopyNextWord(smooth, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
#ifdef _SAPPHIRE_DEBUG_OBJ_LOADER_
				Printer::log("Loaded smoothing group start", smooth, LML_DEBUG);
#endif
				if (String("off") == smooth)
					smoothingGroup = 0;
				else
					smoothingGroup = strtoul10(smooth);
			}
			break;

			case 'u': // usemtl
				// get name of material
			{
				c8 matName[WORD_BUFFER_LENGTH];
				bufPtr = goAndCopyNextWord(matName, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
#ifdef _SAPPHIRE_DEBUG_OBJ_LOADER_
				Printer::log("Loaded material start", matName, LML_DEBUG);
#endif
				mtlName = matName;
				mtlChanged = true;
			}
			break;

			case 'f':               // face  多边形面
			{
				c8 vertexWord[WORD_BUFFER_LENGTH]; // 用于接收顶点数据 for retrieving vertex data
				//顶点对象
				S3DVertex v;
				// Assign vertex color from currently active material's diffuse color
				// 从激活的材质的漫反射的颜色分配顶点颜色
				if (mtlChanged)
				{
					// retrieve the material
					// 接收这个材质
					SObjMtl *useMtl = findMtl(mtlName, grpName);  //查找对应的材质
					// only change material if we found it
					// 只有找到这个材质才替换它
					if (useMtl)
						currMtl = useMtl;
					mtlChanged = false;
				}
				if (currMtl)
					v.Color = currMtl->Meshbuffer->Material.DiffuseColor;   //设置顶点颜色为当前材质的漫反射颜色

				// get all vertices data in this face (current line of obj file) 
				// 获取这个面的所有顶点数据(obj文件的当前行)
				const String wordBuffer = copyLine(bufPtr, bufEnd);
				const c8* linePtr = wordBuffer.c_str();
				const c8* const endPtr = linePtr + wordBuffer.size();

				vector<int>::type faceCorners;
				faceCorners.reserve(32);//faceCorners.reallocate(32); // should be large enough

				// read in all vertices
				//读取所有顶点
				linePtr = goNextWord(linePtr, endPtr);
#ifdef SAPPHIRE_DEBUG
				int fcount = 0; int round = 0;
#endif
				while (0 != linePtr[0])
				{
					// Array to communicate with retrieveVertexIndices()
					// sends the buffer sizes and gets the actual indices
					// if index not set returns -1
					// 数组要与retrieveVertexIndices（）通信，发送缓冲区大小和实际的索引
					SINT32 Idx[3];   //存放顶点坐标/纹理坐标/法线向量的索引
					Idx[1] = Idx[2] = -1;

					// read in next vertex's data
					// 读入下一个顶点数据到vertexWord
					UINT32 wlength = copyWord(vertexWord, linePtr, WORD_BUFFER_LENGTH, endPtr);
					// this function will also convert obj's 1-based index to c++'s 0-based index
					//这个函数将转换obj 基于1的起始索引到C++基于0的索引
					retrieveVertexIndices(vertexWord, Idx, vertexWord + wlength + 1, vertexBuffer.size(), textureCoordBuffer.size(), normalsBuffer.size());
					v.Pos = vertexBuffer[Idx[0]];
					if (-1 != Idx[1])  //判断有无纹理坐标
						v.TCoords = textureCoordBuffer[Idx[1]];
					else
						v.TCoords.set(0.0f, 0.0f);
					if (-1 != Idx[2])   //判断有无法线向量
						v.Normal = normalsBuffer[Idx[2]];
					else
					{
						v.Normal.set(0.0f, 0.0f, 0.0f);
						currMtl->RecalculateNormals = true;
					}

					int vertLocation;
					//map<S3DVertex, int>::Node* n = currMtl->VertMap.find(v);
					//到当前顶点Map查找这个顶点
					order_map<S3DVertex, int>::Node* n = currMtl->VertMap.find(v);
					if (n)
					{

						 
#ifdef SAPPHIRE_DEBUG
						fcount++;
#endif
						//vertLocation = n->getValue();
						//取出它顶点对应位置
						vertLocation = n->getValue();
					}
					else
					{
						//没有找到的话
						currMtl->Meshbuffer->Vertices.push_back(v);
						vertLocation = currMtl->Meshbuffer->Vertices.size() - 1;
						//currMtl->VertMap.insert(v, vertLocation);
						currMtl->VertMap.insert(v, vertLocation);
					}

					faceCorners.push_back(vertLocation);

					// go to next vertex
					// 下一个顶点
					linePtr = goNextWord(linePtr, endPtr);
#ifdef SAPPHIRE_DEBUG
					round++;
#endif
				}

#ifdef SAPPHIRE_DEBUG
				Printer::log(String("FCOUNT ==") + StringUtil::int32ToString(fcount), LML_DEBUG);
				Printer::log(String("ROUND ==") + StringUtil::int32ToString(round), LML_DEBUG);
#endif 
				// triangulate the face
				// 面的三角形化
				for (UINT32 i = 1; i < faceCorners.size() - 1; ++i)
				{
					// Add a triangle
					// 添加一个三角形
					currMtl->Meshbuffer->Indices.push_back(faceCorners[i + 1]);
					currMtl->Meshbuffer->Indices.push_back(faceCorners[i]);
					currMtl->Meshbuffer->Indices.push_back(faceCorners[0]);
				}
				faceCorners.clear(); //faceCorners.set_used(0); // fast clear
				faceCorners.reserve(32);
			}
			break;

			case '#': // comment 注释 不用管
			default:
				break;
			}	// end switch(bufPtr[0])
			// eat up rest of line
			// 读取下一行
			bufPtr = goNextLine(bufPtr, bufEnd);
		}	// end while(bufPtr && (bufPtr-buf<filesize)) 
#ifdef SAPPHIRE_DEBUG
		int cvmap_size = currMtl->VertMap.size();
		Printer::log(String("CurrMTL->VertMapSize ==") + StringUtil::int32ToString(cvmap_size), LML_DEBUG);
#if SAPPHIRE_PLATFORM == SAPPHIRE_PLATFORM_WIN32
         
	
#endif
		
#endif
		int cvmap_size = currMtl->VertMap.size();
		SMesh* mesh = new SMesh();

		// Combine all the groups (meshbuffers) into the mesh
		// 合并所有的组（网格缓冲区）到一个网格
		for (UINT32 m = 0; m < Materials.size(); ++m)
		{
			if (Materials[m]->Meshbuffer->getIndexCount() > 0)
			{
				Materials[m]->Meshbuffer->recalculateBoundingBox();
				if (Materials[m]->RecalculateNormals)
					SceneManager->getMeshManipulator()->recalculateNormals(Materials[m]->Meshbuffer);
				if (Materials[m]->Meshbuffer->Material.MaterialType == EMT_PARALLAX_MAP_SOLID)
				{
					SMesh tmp;
					tmp.addMeshBuffer(Materials[m]->Meshbuffer);
					IMesh* tangentMesh = SceneManager->getMeshManipulator()->createMeshWithTangents(&tmp);
					mesh->addMeshBuffer(tangentMesh->getMeshBuffer(0));
					tangentMesh->drop();
				}
				else
					mesh->addMeshBuffer(Materials[m]->Meshbuffer);
			}
		}

		// Create the Animated mesh if there's anything in the mesh
		//创建这个动画网格
		SAnimatedMesh* animMesh = 0;
		if (0 != mesh->getMeshBufferCount())
		{
			mesh->recalculateBoundingBox();
			animMesh = new SAnimatedMesh();
			animMesh->Type = EAMT_OBJ;
			animMesh->addMesh(mesh);
			animMesh->recalculateBoundingBox();
		}

		// Clean up the allocate obj file contents
		//清空文件缓冲区
		delete[] buf;
		// more cleaning up
		cleanUp();
		mesh->drop();

		return animMesh;
	}


	const c8* COBJMeshFileLoader::readTextures(const c8* bufPtr, const c8* const bufEnd, SObjMtl* currMaterial, const path& relPath)
	{
		UINT8 type = 0; // map_Kd - diffuse color texture map
		// map_Ks - specular color texture map
		// map_Ka - ambient color texture map
		// map_Ns - shininess texture map
		if ((!strncmp(bufPtr, "map_bump", 8)) || (!strncmp(bufPtr, "bump", 4)))
			type = 1; // normal map
		else if ((!strncmp(bufPtr, "map_d", 5)) || (!strncmp(bufPtr, "map_opacity", 11)))
			type = 2; // opacity map
		else if (!strncmp(bufPtr, "map_refl", 8))
			type = 3; // reflection map
		// extract new material's name
		c8 textureNameBuf[WORD_BUFFER_LENGTH];
		bufPtr = goAndCopyNextWord(textureNameBuf, bufPtr, WORD_BUFFER_LENGTH, bufEnd);

		FLOAT32 bumpiness = 6.0f;
		bool clamp = false;
		// handle options
		while (textureNameBuf[0] == '-')
		{
			if (!strncmp(bufPtr, "-bm", 3))
			{
				bufPtr = goAndCopyNextWord(textureNameBuf, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
				currMaterial->Meshbuffer->Material.MaterialTypeParam = fast_atof(textureNameBuf);
				bufPtr = goAndCopyNextWord(textureNameBuf, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
				continue;
			}
			else
				if (!strncmp(bufPtr, "-blendu", 7))
					bufPtr = goAndCopyNextWord(textureNameBuf, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
				else
					if (!strncmp(bufPtr, "-blendv", 7))
						bufPtr = goAndCopyNextWord(textureNameBuf, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
					else
						if (!strncmp(bufPtr, "-cc", 3))
							bufPtr = goAndCopyNextWord(textureNameBuf, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
						else
							if (!strncmp(bufPtr, "-clamp", 6))
								bufPtr = readBool(bufPtr, clamp, bufEnd);
							else
								if (!strncmp(bufPtr, "-texres", 7))
									bufPtr = goAndCopyNextWord(textureNameBuf, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
								else
									if (!strncmp(bufPtr, "-type", 5))
										bufPtr = goAndCopyNextWord(textureNameBuf, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
									else
										if (!strncmp(bufPtr, "-mm", 3))
										{
											bufPtr = goAndCopyNextWord(textureNameBuf, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
											bufPtr = goAndCopyNextWord(textureNameBuf, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
										}
										else
											if (!strncmp(bufPtr, "-o", 2)) // texture coord translation
											{
												bufPtr = goAndCopyNextWord(textureNameBuf, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
												// next parameters are optional, so skip rest of loop if no number is found
												bufPtr = goAndCopyNextWord(textureNameBuf, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
												if (!isdigit(textureNameBuf[0]))
													continue;
												bufPtr = goAndCopyNextWord(textureNameBuf, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
												if (!isdigit(textureNameBuf[0]))
													continue;
											}
											else
												if (!strncmp(bufPtr, "-s", 2)) // texture coord scale
												{
													bufPtr = goAndCopyNextWord(textureNameBuf, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
													// next parameters are optional, so skip rest of loop if no number is found
													bufPtr = goAndCopyNextWord(textureNameBuf, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
													if (!isdigit(textureNameBuf[0]))
														continue;
													bufPtr = goAndCopyNextWord(textureNameBuf, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
													if (!isdigit(textureNameBuf[0]))
														continue;
												}
												else
													if (!strncmp(bufPtr, "-t", 2))
													{
														bufPtr = goAndCopyNextWord(textureNameBuf, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
														// next parameters are optional, so skip rest of loop if no number is found
														bufPtr = goAndCopyNextWord(textureNameBuf, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
														if (!isdigit(textureNameBuf[0]))
															continue;
														bufPtr = goAndCopyNextWord(textureNameBuf, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
														if (!isdigit(textureNameBuf[0]))
															continue;
													}
			// get next word
			bufPtr = goAndCopyNextWord(textureNameBuf, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
		}

		if ((type == 1) && (isdigit(textureNameBuf[0])))
		{
			currMaterial->Meshbuffer->Material.MaterialTypeParam = fast_atof(textureNameBuf);
			bufPtr = goAndCopyNextWord(textureNameBuf, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
		}
		if (clamp)
			currMaterial->Meshbuffer->Material.setFlag(EMF_TEXTURE_WRAP, ETC_CLAMP);

		path texname(textureNameBuf);
		//texname.replace('\\', '/');
		texname = StringUtil::replaceAll(texname, "\\", "/");

		ITexture * texture = 0;
		bool newTexture = false;
		if (texname.size())
		{
			path texnameWithUserPath(SceneManager->getParameters()->getAttributeAsString(OBJ_TEXTURE_PATH));
			if (texnameWithUserPath.size())
			{
				texnameWithUserPath += '/';
				texnameWithUserPath += texname;
			}
			if (FileSystem->existFile(texnameWithUserPath))
				texture = SceneManager->getVideoDriver()->getTexture(texnameWithUserPath);
			else if (FileSystem->existFile(texname))
			{
				newTexture = SceneManager->getVideoDriver()->findTexture(texname) == 0;
				texture = SceneManager->getVideoDriver()->getTexture(texname);
			}
			else
			{
				newTexture = SceneManager->getVideoDriver()->findTexture(relPath + texname) == 0;
				// try to read in the relative path, the .obj is loaded from
				texture = SceneManager->getVideoDriver()->getTexture(relPath + texname);
			}
		}
		if (texture)
		{
			if (type == 0)
				currMaterial->Meshbuffer->Material.setTexture(0, texture);
			else if (type == 1)
			{
				if (newTexture)
					SceneManager->getVideoDriver()->makeNormalMapTexture(texture, bumpiness);
				currMaterial->Meshbuffer->Material.setTexture(1, texture);
				currMaterial->Meshbuffer->Material.MaterialType = EMT_PARALLAX_MAP_SOLID;
				currMaterial->Meshbuffer->Material.MaterialTypeParam = 0.035f;
			}
			else if (type == 2)
			{
				currMaterial->Meshbuffer->Material.setTexture(0, texture);
				currMaterial->Meshbuffer->Material.MaterialType = EMT_TRANSPARENT_ADD_COLOR;
			}
			else if (type == 3)
			{
				//						currMaterial->Meshbuffer->Material.Textures[1] = texture;
				//						currMaterial->Meshbuffer->Material.MaterialType=EMT_REFLECTION_2_LAYER;
			}
			// Set diffuse material color to white so as not to affect texture color
			// Because Maya set diffuse color Kd to black when you use a diffuse color map
			// But is this the right thing to do?
			currMaterial->Meshbuffer->Material.DiffuseColor = ColourValue::getColourValue(
				currMaterial->Meshbuffer->Material.DiffuseColor.getAlpha(), 255, 255, 255);
		}
		return bufPtr;
	}


	void COBJMeshFileLoader::readMTL(const c8* fileName, const path& relPath)
	{
		const path realFile(fileName);
		IReadFile * mtlReader;

		if (FileSystem->existFile(realFile))
			mtlReader = FileSystem->createAndOpenFile(realFile);
		else if (FileSystem->existFile(relPath + realFile))
			mtlReader = FileSystem->createAndOpenFile(relPath + realFile);
		else if (FileSystem->existFile(FileSystem->getFileBasename(realFile)))
			mtlReader = FileSystem->createAndOpenFile(FileSystem->getFileBasename(realFile));
		else
			mtlReader = FileSystem->createAndOpenFile(relPath + FileSystem->getFileBasename(realFile));
		if (!mtlReader)	// fail to open and read file
		{
			Printer::log("Could not open material file", realFile, LML_WARNING);
			return;
		}

		const long filesize = mtlReader->getSize();
		if (!filesize)
		{
			Printer::log("Skipping empty material file", realFile, LML_WARNING);
			mtlReader->drop();
			return;
		}

		c8* buf = new c8[filesize];
		mtlReader->read((void*)buf, filesize);
		const c8* bufEnd = buf + filesize;

		SObjMtl* currMaterial = 0;

		const c8* bufPtr = buf;
		while (bufPtr != bufEnd)
		{
			switch (*bufPtr)
			{
			case 'n': // newmtl
			{
				// if there's an existing material, store it first
				if (currMaterial)
					Materials.push_back(currMaterial);

				// extract new material's name
				c8 mtlNameBuf[WORD_BUFFER_LENGTH];
				bufPtr = goAndCopyNextWord(mtlNameBuf, bufPtr, WORD_BUFFER_LENGTH, bufEnd);

				currMaterial = new SObjMtl;
				currMaterial->Name = mtlNameBuf;
			}
			break;
			case 'i': // illum - illumination
				if (currMaterial)
				{
					const UINT32 COLOR_BUFFER_LENGTH = 16;
					c8 illumStr[COLOR_BUFFER_LENGTH];

					bufPtr = goAndCopyNextWord(illumStr, bufPtr, COLOR_BUFFER_LENGTH, bufEnd);
					currMaterial->Illumination = (c8)atol(illumStr);
				}
				break;
			case 'N':
				if (currMaterial)
				{
					switch (bufPtr[1])
					{
					case 's': // Ns - shininess
					{
						const UINT32 COLOR_BUFFER_LENGTH = 16;
						c8 nsStr[COLOR_BUFFER_LENGTH];

						bufPtr = goAndCopyNextWord(nsStr, bufPtr, COLOR_BUFFER_LENGTH, bufEnd);
						FLOAT32 shininessValue = fast_atof(nsStr);

						// wavefront shininess is from [0, 1000], so scale for OpenGL
						shininessValue *= 0.128f;
						currMaterial->Meshbuffer->Material.Shininess = shininessValue;
					}
					break;
					case 'i': // Ni - refraction index
					{
						c8 tmpbuf[WORD_BUFFER_LENGTH];
						bufPtr = goAndCopyNextWord(tmpbuf, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
					}
					break;
					}
				}
				break;
			case 'K':
				if (currMaterial)
				{
					switch (bufPtr[1])
					{
					case 'd':		// Kd = diffuse
					{
						bufPtr = readColor(bufPtr, currMaterial->Meshbuffer->Material.DiffuseColor, bufEnd);

					}
					break;

					case 's':		// Ks = specular
					{
						bufPtr = readColor(bufPtr, currMaterial->Meshbuffer->Material.SpecularColor, bufEnd);
					}
					break;

					case 'a':		// Ka = ambience
					{
						bufPtr = readColor(bufPtr, currMaterial->Meshbuffer->Material.AmbientColor, bufEnd);
					}
					break;
					case 'e':		// Ke = emissive
					{
						bufPtr = readColor(bufPtr, currMaterial->Meshbuffer->Material.EmissiveColor, bufEnd);
					}
					break;
					}	// end switch(bufPtr[1])
				}	// end case 'K': if ( 0 != currMaterial )...
				break;
			case 'b': // bump
			case 'm': // texture maps
				if (currMaterial)
				{
					bufPtr = readTextures(bufPtr, bufEnd, currMaterial, relPath);
				}
				break;
			case 'd': // d - transparency
				if (currMaterial)
				{
					const UINT32 COLOR_BUFFER_LENGTH = 16;
					c8 dStr[COLOR_BUFFER_LENGTH];

					bufPtr = goAndCopyNextWord(dStr, bufPtr, COLOR_BUFFER_LENGTH, bufEnd);
					FLOAT32 dValue = fast_atof(dStr);

					currMaterial->Meshbuffer->Material.DiffuseColor.setAlpha((SINT32)(dValue * 255));
					if (dValue<1.0f)
						currMaterial->Meshbuffer->Material.MaterialType = EMT_TRANSPARENT_VERTEX_ALPHA;
				}
				break;
			case 'T':
				if (currMaterial)
				{
					switch (bufPtr[1])
					{
					case 'f':		// Tf - Transmitivity
						const UINT32 COLOR_BUFFER_LENGTH = 16;
						c8 redStr[COLOR_BUFFER_LENGTH];
						c8 greenStr[COLOR_BUFFER_LENGTH];
						c8 blueStr[COLOR_BUFFER_LENGTH];

						bufPtr = goAndCopyNextWord(redStr, bufPtr, COLOR_BUFFER_LENGTH, bufEnd);
						bufPtr = goAndCopyNextWord(greenStr, bufPtr, COLOR_BUFFER_LENGTH, bufEnd);
						bufPtr = goAndCopyNextWord(blueStr, bufPtr, COLOR_BUFFER_LENGTH, bufEnd);

						FLOAT32 transparency = (fast_atof(redStr) + fast_atof(greenStr) + fast_atof(blueStr)) / 3;

						currMaterial->Meshbuffer->Material.DiffuseColor.setAlpha((SINT32)(transparency * 255));
						if (transparency < 1.0f)
							currMaterial->Meshbuffer->Material.MaterialType = EMT_TRANSPARENT_VERTEX_ALPHA;
					}
				}
				break;
			default: // comments or not recognised
				break;
			} // end switch(bufPtr[0])
			// go to next line
			bufPtr = goNextLine(bufPtr, bufEnd);
		}	// end while (bufPtr)

		// end of file. if there's an existing material, store it
		if (currMaterial)
			Materials.push_back(currMaterial);

		delete[] buf;
		mtlReader->drop();
	}


	//! Read RGB color
	const c8* COBJMeshFileLoader::readColor(const c8* bufPtr, ColourValue& color, const c8* const bufEnd)
	{
		const UINT32 COLOR_BUFFER_LENGTH = 16;
		c8 colStr[COLOR_BUFFER_LENGTH];

		color.setAlpha(255);
		bufPtr = goAndCopyNextWord(colStr, bufPtr, COLOR_BUFFER_LENGTH, bufEnd);
		color.setRed((SINT32)(fast_atof(colStr) * 255.0f));
		bufPtr = goAndCopyNextWord(colStr, bufPtr, COLOR_BUFFER_LENGTH, bufEnd);
		color.setGreen((SINT32)(fast_atof(colStr) * 255.0f));
		bufPtr = goAndCopyNextWord(colStr, bufPtr, COLOR_BUFFER_LENGTH, bufEnd);
		color.setBlue((SINT32)(fast_atof(colStr) * 255.0f));
		return bufPtr;
	}


	//! Read 3d vector of floats
	// 读取3d浮点向量
	const c8* COBJMeshFileLoader::readVec3(const c8* bufPtr, Vector3& vec, const c8* const bufEnd)
	{
		const UINT32 WORD_BUFFER_LENGTH = 256;
		c8 wordBuffer[WORD_BUFFER_LENGTH];

		bufPtr = goAndCopyNextWord(wordBuffer, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
		vec.x = -fast_atof(wordBuffer); // change handedness
		bufPtr = goAndCopyNextWord(wordBuffer, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
		vec.y = fast_atof(wordBuffer);
		bufPtr = goAndCopyNextWord(wordBuffer, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
		vec.z = fast_atof(wordBuffer);
		return bufPtr;
	}


	//! Read 2d vector of floats
	const c8* COBJMeshFileLoader::readUV(const c8* bufPtr, Vector2& vec, const c8* const bufEnd)
	{
		const UINT32 WORD_BUFFER_LENGTH = 256;
		c8 wordBuffer[WORD_BUFFER_LENGTH];

		bufPtr = goAndCopyNextWord(wordBuffer, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
		vec.x = fast_atof(wordBuffer);
		bufPtr = goAndCopyNextWord(wordBuffer, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
		vec.y = 1 - fast_atof(wordBuffer); // change handedness
		return bufPtr;
	}


	//! Read boolean value represented as 'on' or 'off'
	const c8* COBJMeshFileLoader::readBool(const c8* bufPtr, bool& tf, const c8* const bufEnd)
	{
		const UINT32 BUFFER_LENGTH = 8;
		c8 tfStr[BUFFER_LENGTH];

		bufPtr = goAndCopyNextWord(tfStr, bufPtr, BUFFER_LENGTH, bufEnd);
		tf = strcmp(tfStr, "off") != 0;
		return bufPtr;
	}


	COBJMeshFileLoader::SObjMtl* COBJMeshFileLoader::findMtl(const String& mtlName, const String& grpName)
	{
		COBJMeshFileLoader::SObjMtl* defMaterial = 0;
		// search existing Materials for best match
		// exact match does return immediately, only name match means a new group
		for (UINT32 i = 0; i < Materials.size(); ++i)
		{
			if (Materials[i]->Name == mtlName)
			{
				if (Materials[i]->Group == grpName)
					return Materials[i];
				else
					defMaterial = Materials[i];
			}
		}
		// we found a partial match
		if (defMaterial)
		{
			Materials.push_back(new SObjMtl(*defMaterial));
			Materials.back()->Group = grpName;
			return Materials.back();
		}
		// we found a new group for a non-existant material
		else if (grpName.size())
		{
			Materials.push_back(new SObjMtl(*Materials[0]));
			Materials.back()->Group = grpName;
			return Materials.back();
		}
		return 0;
	}


	//! skip space characters and stop on first non-space
	const c8* COBJMeshFileLoader::goFirstWord(const c8* buf, const c8* const bufEnd, bool acrossNewlines)
	{
		// skip space characters
		if (acrossNewlines)
			while ((buf != bufEnd) && isspace(*buf))
				++buf;
		else
			while ((buf != bufEnd) && isspace(*buf) && (*buf != '\n'))
				++buf;

		return buf;
	}


	//! skip current word and stop at beginning of next one
	// 跳过当前的字并且在下一个之前停止
	const c8* COBJMeshFileLoader::goNextWord(const c8* buf, const c8* const bufEnd, bool acrossNewlines)
	{
		// skip current word
		// 跳过当前的字
		while ((buf != bufEnd) && !isspace(*buf))
			++buf;

		return goFirstWord(buf, bufEnd, acrossNewlines);
	}


	//! Read until line break is reached and stop at the next non-space character
	const c8* COBJMeshFileLoader::goNextLine(const c8* buf, const c8* const bufEnd)
	{
		// look for newline characters
		while (buf != bufEnd)
		{
			// found it, so leave
			if (*buf == '\n' || *buf == '\r')
				break;
			++buf;
		}
		return goFirstWord(buf, bufEnd);
	}


	UINT32 COBJMeshFileLoader::copyWord(c8* outBuf, const c8* const inBuf, UINT32 outBufLength, const c8* const bufEnd)
	{
		if (!outBufLength)
			return 0;
		if (!inBuf)
		{
			*outBuf = 0;
			return 0;
		}

		UINT32 i = 0;
		while (inBuf[i])
		{
			if (isspace(inBuf[i]) || &(inBuf[i]) == bufEnd)
				break;
			++i;
		}

		UINT32 length = Math::min_(i, outBufLength - 1);
		for (UINT32 j = 0; j<length; ++j)
			outBuf[j] = inBuf[j];

		outBuf[length] = 0;
		return length;
	}


	String COBJMeshFileLoader::copyLine(const c8* inBuf, const c8* bufEnd)
	{
		if (!inBuf)
			return String();

		const c8* ptr = inBuf;
		while (ptr<bufEnd)
		{
			if (*ptr == '\n' || *ptr == '\r')
				break;
			++ptr;
		}
		// we must avoid the +1 in case the vector is used up
		return String(inBuf, (UINT32)(ptr - inBuf + ((ptr < bufEnd) ? 1 : 0)));
	}


	const c8* COBJMeshFileLoader::goAndCopyNextWord(c8* outBuf, const c8* inBuf, UINT32 outBufLength, const c8* bufEnd)
	{
		inBuf = goNextWord(inBuf, bufEnd, false);
		copyWord(outBuf, inBuf, outBufLength, bufEnd);
		return inBuf;
	}


	bool COBJMeshFileLoader::retrieveVertexIndices(c8* vertexData, SINT32* idx, const c8* bufEnd, UINT32 vbsize, UINT32 vtsize, UINT32 vnsize)
	{
		c8 word[16] = "";
		const c8* p = goFirstWord(vertexData, bufEnd);
		UINT32 idxType = 0;	// 0 = posIdx, 1 = texcoordIdx, 2 = normalIdx

		UINT32 i = 0;
		while (p != bufEnd)
		{
			if ((isdigit(*p)) || (*p == '-'))
			{
				// build up the number
				word[i++] = *p;
			}
			else if (*p == '/' || *p == ' ' || *p == '\0')
			{
				// number is completed. Convert and store it
				word[i] = '\0';
				// if no number was found index will become 0 and later on -1 by decrement
				idx[idxType] = strtol10(word);
				if (idx[idxType]<0)
				{
					switch (idxType)
					{
					case 0:
						idx[idxType] += vbsize;
						break;
					case 1:
						idx[idxType] += vtsize;
						break;
					case 2:
						idx[idxType] += vnsize;
						break;
					}
				}
				else
					idx[idxType] -= 1;

				// reset the word
				word[0] = '\0';
				i = 0;

				// go to the next kind of index type
				if (*p == '/')
				{
					if (++idxType > 2)
					{
						// error checking, shouldn't reach here unless file is wrong
						idxType = 0;
					}
				}
				else
				{
					// set all missing values to disable (=-1)
					while (++idxType < 3)
						idx[idxType] = -1;
					++p;
					break; // while
				}
			}

			// go to the next char
			++p;
		}

		return true;
	}


	void COBJMeshFileLoader::cleanUp()
	{
		for (UINT32 i = 0; i < Materials.size(); ++i)
		{
			Materials[i]->Meshbuffer->drop();
			delete Materials[i];
		}

		Materials.clear();
	}
}


#endif