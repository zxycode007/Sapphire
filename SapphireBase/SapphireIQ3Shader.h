#ifndef _SAPPHIRE_I_Q3_SHADER_
#define _SAPPHIRE_I_Q3_SHADER_

#include "SapphirePrerequisites.h"
#include "SapphireString.h"
#include "SapphireIFileSystem.h"
#include "SapphireIVideoDriver.h"
#include "SapphireIImage.h"
#include "SapphireCoreUtil.h"

namespace Sapphire
{
	namespace Quake3
	{
		static String irrEmptyStringc("");

		//! 为getMesh保存不同的Mesh类型
		enum eQ3MeshIndex
		{
			E_Q3_MESH_GEOMETRY = 0,
			E_Q3_MESH_ITEMS,
			E_Q3_MESH_BILLBOARD,
			E_Q3_MESH_FOG,
			E_Q3_MESH_UNRESOLVED,
			E_Q3_MESH_SIZE
		};

		/*! 用于自定义Quake3 BSP 加载器
		*/

		struct Q3LevelLoadParameter
		{
			Q3LevelLoadParameter()
				:defaultLightMapMaterial(EMT_LIGHTMAP_M4),
				defaultModulate(EMFN_MODULATE_4X),
				defaultFilter(EMF_BILINEAR_FILTER),
				patchTesselation(8),
				verbose(0),
				startTime(0), endTime(0),
				mergeShaderBuffer(1),
				cleanUnResolvedMeshes(1),
				loadAllShaders(0),
				loadSkyShader(0),
				alpharef(1),
				swapLump(0),
#ifdef __BIG_ENDIAN__
				swapHeader(1)
#else
				swapHeader(0)
#endif
			{
				memcpy(scriptDir, "scripts\x0", 8);
			}

			E_MATERIAL_TYPE defaultLightMapMaterial;
			E_MODULATE_FUNC defaultModulate;
			E_MATERIAL_FLAG defaultFilter;
			SINT32 patchTesselation;
			SINT32 verbose;
			UINT32 startTime;
			UINT32 endTime;
			SINT32 mergeShaderBuffer;
			SINT32 cleanUnResolvedMeshes;
			SINT32 loadAllShaders;
			SINT32 loadSkyShader;
			SINT32 alpharef;
			SINT32 swapLump;
			SINT32 swapHeader;
			c8 scriptDir[64];
		};

		// 某些有用的定义
		typedef std::vector< String > tStringList;
		typedef std::vector< ITexture* > tTexArray;

		 
		inline SINT16 isEqual(const String &string, UINT32 &pos, const c8 *list[], UINT16 listSize)
		{
			const char * in = string.c_str() + pos;

			for (UINT16 i = 0; i != listSize; ++i)
			{
				if (string.size() < pos)
					return -2;
				UINT32 len = (UINT32)strlen(list[i]);
				if (string.size() < pos + len)
					continue;
				if (in[len] != 0 && in[len] != ' ')
					continue;
				if (strncmp(in, list[i], len))
					continue;

				pos += len + 1;
				return (SINT16)i;
			}
			return -2;
		}
		//！ 将字符串转换为浮点数
		inline Real getAsFloat(const String &string, UINT32 &pos)
		{
			const char * in = string.c_str() + pos;

			Real value = 0.f;
			pos += (UINT32)(fast_atof_move(in, value) - in) + 1;
			return value;
		}

		//! 获取一个quake3 向量转换到引擎的位置(x,-z,y)
		inline Vector3 getAsVector3df(const String &string, UINT32 &pos)
		{
			Vector3 v;

			v.x = getAsFloat(string, pos);
			v.z = getAsFloat(string, pos);
			v.y = getAsFloat(string, pos);

			return v;
		}


		/*
		提取子串
		*/
		inline void getAsStringList(tStringList &list, SINT32 max, const String &string, UINT32 &startPos)
		{
			list.clear();

			SINT32 finish = 0;
			SINT32 endPos;
			/*
			do
			{
				endPos = string.findNext(' ', startPos);
				if (endPos == -1)
				{
					finish = 1;
					endPos = string.size();
				}

				list.push_back(string.substr(startPos, endPos - startPos));
				startPos = endPos + 1;

				if (list.size() >= (UINT32)max)
					finish = 1;

			} while (!finish);
			*/
			do
			{
				endPos = string.find(' ', startPos);
				if (endPos == -1)
				{
					finish = 1;
					endPos = string.size();
				}

				list.push_back(string.substr(startPos, endPos - startPos));
				startPos = endPos + 1;

				if (list.size() >= (UINT32)max)
					finish = 1;

			} while (!finish);
		}

		//! 对于q3 shader 的混合函数
		struct SBlendFunc
		{
			SBlendFunc(E_MODULATE_FUNC mod)
				: type(EMT_SOLID), modulate(mod),
				param0(0.f),
				isTransparent(0) {}

			E_MATERIAL_TYPE type;
			E_MODULATE_FUNC modulate;

			Real param0;
			UINT32 isTransparent;
		};

		// 解析变量cull的内容
		inline bool getCullingFunction(const String &cull)
		{
			if (cull.size() == 0)
				return true;

			bool ret = true;
			static const c8 * funclist[] = { "none", "disable", "twosided" };

			UINT32 pos = 0;
			switch (isEqual(cull, pos, funclist, 3))
			{
			case 0:
			case 1:
			case 2:
				ret = false;
				break;
			}
			return ret;
		}

		// 解析变量depthfunc的内容
		// 返回一个z-test
		inline UINT8 getDepthFunction(const String &string)
		{
			UINT8 ret = ECFN_LESSEQUAL;

			if (string.size() == 0)
				return ret;

			static const c8 * funclist[] = { "lequal", "equal" };

			UINT32 pos = 0;
			switch (isEqual(string, pos, funclist, 2))
			{
			case 0:
				ret = ECFN_LESSEQUAL;
				break;
			case 1:
				ret = ECFN_EQUAL;
				break;
			}
			return ret;
		}


		/*!
		解析Variable blendfunc,alphafunc的内容
		它为渲染透明或实体节点做一个提示

		我们假设一个典型的quake节点看起来这样//
		1) 大型静态表面 Big Static Mesh ( solid )
		2) 静态场景项目static scene item ( 可能会透明 ) 但是在实体渲染通道
		3) 附加透明项目在透明通道 

		它不是100%精确
		*/
		inline static void getBlendFunc(const String &string, SBlendFunc &blendfunc)
		{
			if (string.size() == 0)
				return;

			//E_BLEND_FACTOR的映射
			static const c8 * funclist[] =
			{
				"gl_zero",
				"gl_one",
				"gl_dst_color",
				"gl_one_minus_dst_color",
				"gl_src_color",
				"gl_one_minus_src_color",
				"gl_src_alpha",
				"gl_one_minus_src_alpha",
				"gl_dst_alpha",
				"gl_one_minus_dst_alpha",
				"gl_src_alpha_sat",

				"add",
				"filter",
				"blend",

				"ge128",
				"gt0",
			};


			UINT32 pos = 0;
			SINT32 srcFact = isEqual(string, pos, funclist, 16);

			if (srcFact < 0)
				return;

			UINT32 resolved = 0;
			SINT32 dstFact = isEqual(string, pos, funclist, 16);

			switch (srcFact)
			{
			case EBF_ZERO:
				switch (dstFact)
				{
					// gl_zero gl_src_color == gl_dst_color gl_zero
				case EBF_SRC_COLOR:
					blendfunc.type = EMT_ONETEXTURE_BLEND;
					blendfunc.param0 = pack_textureBlendFunc(EBF_DST_COLOR, EBF_ZERO, blendfunc.modulate);
					blendfunc.isTransparent = 1;
					resolved = 1;
					break;
				} break;

			case EBF_ONE:
				switch (dstFact)
				{
					// gl_one gl_zero
				case EBF_ZERO:
					blendfunc.type = EMT_SOLID;
					blendfunc.isTransparent = 0;
					resolved = 1;
					break;

					// gl_one gl_one
				case EBF_ONE:
					blendfunc.type = EMT_TRANSPARENT_ADD_COLOR;
					blendfunc.isTransparent = 1;
					resolved = 1;
					break;
				} break;

			case EBF_SRC_ALPHA:
				switch (dstFact)
				{
					// gl_src_alpha gl_one_minus_src_alpha
				case EBF_ONE_MINUS_SRC_ALPHA:
					blendfunc.type = EMT_TRANSPARENT_ALPHA_CHANNEL;
					blendfunc.param0 = 1.f / 255.f;
					blendfunc.isTransparent = 1;
					resolved = 1;
					break;
				} break;

			case 11:
				// add
				blendfunc.type = EMT_TRANSPARENT_ADD_COLOR;
				blendfunc.isTransparent = 1;
				resolved = 1;
				break;
			case 12:
				// filter = gl_dst_color gl_zero or gl_zero gl_src_color
				blendfunc.type = EMT_ONETEXTURE_BLEND;
				blendfunc.param0 = pack_textureBlendFunc(EBF_DST_COLOR, EBF_ZERO, blendfunc.modulate);
				blendfunc.isTransparent = 1;
				resolved = 1;
				break;
			case 13:
				// blend = gl_src_alpha gl_one_minus_src_alpha
				blendfunc.type = EMT_TRANSPARENT_ALPHA_CHANNEL;
				blendfunc.param0 = 1.f / 255.f;
				blendfunc.isTransparent = 1;
				resolved = 1;
				break;
			case 14:
				// alphafunc ge128
				blendfunc.type = EMT_TRANSPARENT_ALPHA_CHANNEL;
				blendfunc.param0 = 0.5f;
				blendfunc.isTransparent = 1;
				resolved = 1;
				break;
			case 15:
				// alphafunc gt0
				blendfunc.type = EMT_TRANSPARENT_ALPHA_CHANNEL;
				blendfunc.param0 = 1.f / 255.f;
				blendfunc.isTransparent = 1;
				resolved = 1;
				break;

			}

			//使用同一的混合器
			if (0 == resolved)
			{
				blendfunc.type = EMT_ONETEXTURE_BLEND;
				blendfunc.param0 = pack_textureBlendFunc(
					(E_BLEND_FACTOR) srcFact,
					(E_BLEND_FACTOR) dstFact,
					blendfunc.modulate);

				blendfunc.isTransparent = 1;
			}
		}

		// 随机噪声 [-1;1]
		struct Noiser
		{
			static Real get()
			{
				static UINT32 RandomSeed = 0x69666966;
				RandomSeed = (RandomSeed * 3631 + 1);

				Real value = ((Real)(RandomSeed & 0x7FFF) * (1.0f / (Real)(0x7FFF >> 1))) - 1.f;
				return value;
			}
		};

		enum eQ3ModifierFunction
		{
			TCMOD = 0,
			DEFORMVERTEXES = 1,
			RGBGEN = 2,
			TCGEN = 3,
			MAP = 4,
			ALPHAGEN = 5,

			FUNCTION2 = 0x10,
			SCROLL = FUNCTION2 + 1,
			SCALE = FUNCTION2 + 2,
			ROTATE = FUNCTION2 + 3,
			STRETCH = FUNCTION2 + 4,
			TURBULENCE = FUNCTION2 + 5,
			WAVE = FUNCTION2 + 6,

			IDENTITY = FUNCTION2 + 7,
			VERTEX = FUNCTION2 + 8,
			TEXTURE = FUNCTION2 + 9,
			LIGHTMAP = FUNCTION2 + 10,
			ENVIRONMENT = FUNCTION2 + 11,
			DOLLAR_LIGHTMAP = FUNCTION2 + 12,
			BULGE = FUNCTION2 + 13,
			AUTOSPRITE = FUNCTION2 + 14,
			AUTOSPRITE2 = FUNCTION2 + 15,
			TRANSFORM = FUNCTION2 + 16,
			EXACTVERTEX = FUNCTION2 + 17,
			CONSTANT = FUNCTION2 + 18,
			LIGHTINGSPECULAR = FUNCTION2 + 19,
			MOVE = FUNCTION2 + 20,
			NORMAL = FUNCTION2 + 21,
			IDENTITYLIGHTING = FUNCTION2 + 22,

			WAVE_MODIFIER_FUNCTION = 0x30,
			SINUS = WAVE_MODIFIER_FUNCTION + 1,
			COSINUS = WAVE_MODIFIER_FUNCTION + 2,
			SQUARE = WAVE_MODIFIER_FUNCTION + 3,
			TRIANGLE = WAVE_MODIFIER_FUNCTION + 4,
			SAWTOOTH = WAVE_MODIFIER_FUNCTION + 5,
			SAWTOOTH_INVERSE = WAVE_MODIFIER_FUNCTION + 6,
			NOISE = WAVE_MODIFIER_FUNCTION + 7,


			UNKNOWN = -2

		};

		struct SModifierFunction
		{
			SModifierFunction()
				: masterfunc0(UNKNOWN), masterfunc1(UNKNOWN), func(SINUS),
				tcgen(TEXTURE), rgbgen(IDENTITY), alphagen(UNKNOWN),
				base(0), amp (1), phase(0), frequency(1),
				wave(1),
				x(0), y(0), z(0), count(0) {}

			// "tcmod","deformvertexes","rgbgen", "tcgen"
			eQ3ModifierFunction masterfunc0;
			// depends
			eQ3ModifierFunction masterfunc1;
			// depends
			eQ3ModifierFunction func;

			eQ3ModifierFunction tcgen;
			eQ3ModifierFunction rgbgen;
			eQ3ModifierFunction alphagen;

			union
			{
				Real base;
				Real bulgewidth;
			};

			union
			{
				Real amp;
				Real bulgeheight;
			};

			Real phase;

			union
			{
				Real frequency;
				Real bulgespeed;
			};

			union
			{
				Real wave;
				Real div;
			};

			Real x;
			Real y;
			Real z;
			UINT32 count;

			Real evaluate(Real dt) const
			{
				// phase in 0 and 1..
				Real x = fract((dt + phase) * frequency);
				Real y = 0.f;

				switch (func)
				{
				case SINUS:
					y = sinf(x * PI * 2.f);
					break;
				case COSINUS:
					y = cosf(x * PI * 2.f);
					break;
				case SQUARE:
					y = x < 0.5f ? 1.f : -1.f;
					break;
				case TRIANGLE:
					y = x < 0.5f ? (4.f * x) - 1.f : (-4.f * x) + 3.f;
					break;
				case SAWTOOTH:
					y = x;
					break;
				case SAWTOOTH_INVERSE:
					y = 1.f - x;
					break;
				case NOISE:
					y = Noiser::get();
					break;
				default:
					break;
				}

				return base + (y * amp);
			}


		};

		inline Vector3 getMD3Normal(UINT32 i, UINT32 j)
		{
			const Real lng = i * 2.0f *  PI / 255.0f;
			const Real lat = j * 2.0f *  PI / 255.0f;
			return Vector3(cosf(lat) * sinf(lng),
				sinf(lat) * sinf(lng),
				cosf(lng));
		}

		//
		inline void getModifierFunc(SModifierFunction& fill, const String &string, UINT32 &pos)
		{
			if (string.size() == 0)
				return;

			static const c8 * funclist[] =
			{
				"sin", "cos", "square",
				"triangle", "sawtooth", "inversesawtooth", "noise"
			};

			fill.func = (eQ3ModifierFunction)isEqual(string, pos, funclist, 7);
			fill.func = fill.func == UNKNOWN ? SINUS : (eQ3ModifierFunction)((UINT32)fill.func + WAVE_MODIFIER_FUNCTION + 1);

			fill.base = getAsFloat(string, pos);
			fill.amp = getAsFloat(string, pos);
			fill.phase = getAsFloat(string, pos);
			fill.frequency = getAsFloat(string, pos);
		}


		// name = "a b c .."
		struct SVariable
		{
			String name;
			String content;

			SVariable(const c8 * n, const c8 *c = 0) : name(n), content(c) {}
			virtual ~SVariable() {}

			void clear()
			{
				name = "";
				content = "";
			}

			SINT32 isValid() const
			{
				return name.size();
			}

			bool operator == (const SVariable &other) const
			{
				return 0 == strcmp(name.c_str(), other.name.c_str());
			}

			bool operator < (const SVariable &other) const
			{
				return 0 > strcmp(name.c_str(), other.name.c_str());
			}

		};


		// 字符串数据库 "a" = "Hello", "b" = "1234.6"
		struct SVarGroup
		{
			SVarGroup() { /*Variable.setAllocStrategy ( core::ALLOC_STRATEGY_SAFE ); */ }
			virtual ~SVarGroup() {}

			UINT32 isDefined(const c8 * name, const c8 * content = 0) const
			{
				for (UINT32 i = 0; i != Variable.size(); ++i)
				{
					if (0 == strcmp(Variable[i].name.c_str(), name) &&
						(0 == content || strstr(Variable[i].content.c_str(), content))
						)
					{
						return i + 1;
					}
				}
				return 0;
			}

			// 搜索变量名返回内容
			// 如果变量没有找到一个引用，返回一个空字符串
			const String &get(const c8 * name) const
			{
				SVariable search(name);
				//SINT32 index = Variable.linear_search(search);
				std::vector<SVariable>::const_iterator it = find(Variable.begin(), Variable.end(), search);
				SINT32 index = std::distance(Variable.begin(), it);
				if (index < 0)
					return irrEmptyStringc;

				return Variable[index].content;
			}

			// 设置变量名
			void set(const c8 * name, const c8 * content = 0)
			{
				UINT32 index = isDefined(name, 0);
				if (0 == index)
				{
					Variable.push_back(SVariable(name, content));
				}
				else
				{
					Variable[index].content = content;
				}
			}


			std::vector < SVariable > Variable;
		};

		//! 保存一个变量组
		struct SVarGroupList : public IReferenceCounter
		{
			SVarGroupList()
			{
				//VariableGroup.setAllocStrategy(core::ALLOC_STRATEGY_SAFE);
			}
			virtual ~SVarGroupList() {}

			std::vector < SVarGroup > VariableGroup;
		};


		//! 一个解析后的Shader保存在组中的变量顺序
		struct IShader
		{
			IShader()
				: ID(0), VarGroup(0)  {}
			virtual ~IShader() {}

			void operator = (const IShader &other)
			{
				ID = other.ID;
				VarGroup = other.VarGroup;
				name = other.name;
			}

			bool operator == (const IShader &other) const
			{
				return 0 == strcmp(name.c_str(), other.name.c_str());
				//return name == other.name;
			}

			bool operator < (const IShader &other) const
			{
				return strcmp(name.c_str(), other.name.c_str()) < 0;
				//return name < other.name;
			}

			UINT32 getGroupSize() const
			{
				if (0 == VarGroup)
					return 0;
				return VarGroup->VariableGroup.size();
			}

			const SVarGroup * getGroup(UINT32 stage) const
			{
				if (0 == VarGroup || stage >= VarGroup->VariableGroup.size())
					return 0;

				return &VarGroup->VariableGroup[stage];
			}

			// id
			SINT32 ID;
			SVarGroupList *VarGroup; // reference

			// Shader: shader name ( also first variable in first Vargroup )
			// Entity: classname ( variable in Group(1) )
			String name;
		};

		typedef IShader IEntity;

		typedef std::vector < IEntity > tQ3EntityList;

		/*
		按源布局dump shader, 不考虑内部数据有没有递归
		*/
		inline void dumpVarGroup(String &dest, const SVarGroup * group, SINT32 stack)
		{
			String buf;
			SINT32 i;


			if (stack > 0)
			{
				buf = "";
				for (i = 0; i < stack - 1; ++i)
					buf += '\t';

				buf += "{\n";
				dest.append(buf);
			}

			for (UINT32 g = 0; g != group->Variable.size(); ++g)
			{
				buf = "";
				for (i = 0; i < stack; ++i)
					buf += '\t';

				buf += group->Variable[g].name;
				buf += " ";
				buf += group->Variable[g].content;
				buf += "\n";
				dest.append(buf);
			}

			if (stack > 1)
			{
				buf = "";
				for (i = 0; i < stack - 1; ++i)
					buf += '\t';

				buf += "}\n";
				dest.append(buf);
			}

		}

		/*!
		dump 一个 Shader 或一个 Entity
		*/
		inline String & dumpShader(String &dest, const IShader * shader, bool entity = false)
		{
			if (0 == shader)
				return dest;

			const SVarGroup * group;

			const UINT32 size = shader->VarGroup->VariableGroup.size();
			for (UINT32 i = 0; i != size; ++i)
			{
				group = &shader->VarGroup->VariableGroup[i];
				dumpVarGroup(dest, group, Math::Clamp((int)i, 0, 2));
			}

			if (!entity)
			{
				if (size <= 1)
				{
					dest.append("{\n");
				}
				dest.append("}\n");
			}
			return dest;
		}


		/*
		quake3 不区分tga和jpg
		加载一个或多个文件保存在纹理数组中以startPos开始的位置，如果纹理没有被加载，会添加0（没有找到纹理）
		*/
		inline void getTextures(tTexArray &textures,
			const String &name, UINT32 &startPos,
			IFileSystem *fileSystem,
			IVideoDriver* driver)
		{
			static const char* extension[] =
			{
				".jpg",
				".jpeg",
				".png",
				".dds",
				".tga",
				".bmp",
				".pcx"
			};

			tStringList stringList;
			getAsStringList(stringList, -1, name, startPos);

			textures.clear();

			path loadFile;
			for (UINT32 i = 0; i != stringList.size(); ++i)
			{
				ITexture* texture = 0;
				for (UINT32 g = 0; g != 7; ++g)
				{
					cutFilenameExtension(loadFile, stringList[i]);

					if (loadFile == "$whiteimage")
					{
						texture = driver->getTexture("$whiteimage");
						if (0 == texture)
						{
							dimension2du s(2, 2);
							UINT32 image[4] = { 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF };
							IImage* w = driver->createImageFromData(ECF_A8R8G8B8, s, &image);
							texture = driver->addTexture("$whiteimage", w);
							w->drop();
						}

					}
					else
						if (loadFile == "$redimage")
						{
							texture = driver->getTexture("$redimage");
							if (0 == texture)
							{
								dimension2du s(2, 2);
								UINT32 image[4] = { 0xFFFF0000, 0xFFFF0000, 0xFFFF0000, 0xFFFF0000 };
								IImage* w = driver->createImageFromData(ECF_A8R8G8B8, s, &image);
								texture = driver->addTexture("$redimage", w);
								w->drop();
							}
						}
						else
							if (loadFile == "$blueimage")
							{
								texture = driver->getTexture("$blueimage");
								if (0 == texture)
								{
									dimension2du s(2, 2);
									UINT32 image[4] = { 0xFF0000FF, 0xFF0000FF, 0xFF0000FF, 0xFF0000FF };
									IImage* w = driver->createImageFromData(ECF_A8R8G8B8, s, &image);
									texture = driver->addTexture("$blueimage", w);
									w->drop();
								}
							}
							else
								if (loadFile == "$checkerimage")
								{
									texture = driver->getTexture("$checkerimage");
									if (0 == texture)
									{
										dimension2du s(2, 2);
										UINT32 image[4] = { 0xFFFFFFFF, 0xFF000000, 0xFF000000, 0xFFFFFFFF };
										IImage* w = driver->createImageFromData(ECF_A8R8G8B8, s, &image);
										texture = driver->addTexture("$checkerimage", w);
										w->drop();
									}
								}
								else
									if (loadFile == "$lightmap")
									{
										texture = 0;
									}
									else
									{
										loadFile.append(extension[g]);
									}

					if (fileSystem->existFile(loadFile))
					{
						texture = driver->getTexture(loadFile);
						if (texture)
							break;
						texture = 0;
					}
				}
				// take 0 Texture
				textures.push_back(texture);
			}
		}


		//! 关联不同的Quake3 shader类型
		class IShaderManager : public IReferenceCounter
		{
		};
	}
}

#endif