#include "SapphireCompileConifg.h"

#ifdef _SAPPHIRE_COMPILE_WITH_OPENGL_


#include "SapphireCOpenGLGLSLMaterialRenderer.h"
#include "SapphireIGPUProgramServices.h"
#include "SapphireIShaderConstantSetCallBack.h"
#include "SapphireIMaterialRendererServices.h"
#include "SapphireIVideoDriver.h"
#include "SapphireOS.h"
#include "SapphireCOpenGLDriver.h"


namespace Sapphire
{
	//! Constructor
	COpenGLSLMaterialRenderer::COpenGLSLMaterialRenderer(COpenGLDriver* driver,
		SINT32& outMaterialTypeNr, const c8* vertexShaderProgram,
		const c8* vertexShaderEntryPointName,
		E_VERTEX_SHADER_TYPE vsCompileTarget,
		const c8* pixelShaderProgram,
		const c8* pixelShaderEntryPointName,
		E_PIXEL_SHADER_TYPE psCompileTarget,
		const c8* geometryShaderProgram,
		const c8* geometryShaderEntryPointName,
		E_GEOMETRY_SHADER_TYPE gsCompileTarget,
		E_PRIMITIVE_TYPE inType, E_PRIMITIVE_TYPE outType,
		UINT32 verticesOut,
		IShaderConstantSetCallBack* callback,
		IMaterialRenderer* baseMaterial,
		SINT32 userData)
		: Driver(driver), CallBack(callback), BaseMaterial(baseMaterial),
		Program(0), Program2(0), UserData(userData)
	{
#ifdef _DEBUG
		setDebugName("COpenGLSLMaterialRenderer");
#endif

		//entry points must always be main, and the compile target isn't selectable
		//it is fine to ignore what has been asked for, as the compiler should spot anything wrong
		//just check that GLSL is available
		//入口点必须为main， 并且编译目标不可选

		if (BaseMaterial)
			BaseMaterial->grab();

		if (CallBack)
			CallBack->grab();

		if (!Driver->queryFeature(EVDF_ARB_GLSL))
			return;
		//初始化
		init(outMaterialTypeNr, vertexShaderProgram, pixelShaderProgram, geometryShaderProgram);
	}


	//! constructor only for use by derived classes who want to
	//! create a fall back material for example.
	// 只用于例如想要创建一个返回材质的派生类的构造函数
	COpenGLSLMaterialRenderer::COpenGLSLMaterialRenderer(COpenGLDriver* driver,
		IShaderConstantSetCallBack* callback,
		IMaterialRenderer* baseMaterial, SINT32 userData)
		: Driver(driver), CallBack(callback), BaseMaterial(baseMaterial),
		Program(0), Program2(0), UserData(userData)
	{
		if (BaseMaterial)
			BaseMaterial->grab();

		if (CallBack)
			CallBack->grab();
	}


	//! Destructor
	COpenGLSLMaterialRenderer::~COpenGLSLMaterialRenderer()
	{
		if (CallBack)
			CallBack->drop();

		if (Program)
		{
			GLhandleARB shaders[8];
			GLint count;
			Driver->extGlGetAttachedObjects(Program, 8, &count, shaders);
			// avoid bugs in some drivers, which return larger numbers
			//避免某些驱动bug，返回最大数目
			count = Math::min_(count, 8);
			for (GLint i = 0; i<count; ++i)
				Driver->extGlDeleteObject(shaders[i]);   //删除OBJ
			Driver->extGlDeleteObject(Program);        //最后删除Program
			Program = 0;
		}

		if (Program2)
		{
			GLuint shaders[8];
			GLint count;
			Driver->extGlGetAttachedShaders(Program2, 8, &count, shaders);
			// avoid bugs in some drivers, which return larger numbers
			//避免某些驱动bug，返回最大数目
			count = Math::min_(count, 8);
			for (GLint i = 0; i<count; ++i)
				Driver->extGlDeleteShader(shaders[i]);
			Driver->extGlDeleteProgram(Program2);
			Program2 = 0;
		}

		UniformInfo.clear();

		if (BaseMaterial)
			BaseMaterial->drop();
	}

	//初始化
	void COpenGLSLMaterialRenderer::init(SINT32& outMaterialTypeNr,
		const c8* vertexShaderProgram,
		const c8* pixelShaderProgram,
		const c8* geometryShaderProgram,
		E_PRIMITIVE_TYPE inType, E_PRIMITIVE_TYPE outType,
		UINT32 verticesOut)
	{
		outMaterialTypeNr = -1;
		//创建Program
		if (!createProgram())
			return;
//创建Vertex Shader和Pixel Shader
#if defined(GL_ARB_vertex_shader) && defined (GL_ARB_fragment_shader)
		if (vertexShaderProgram)
			if (!createShader(GL_VERTEX_SHADER_ARB, vertexShaderProgram))
				return;

		if (pixelShaderProgram)
			if (!createShader(GL_FRAGMENT_SHADER_ARB, pixelShaderProgram))
				return;
#endif

#if defined(GL_ARB_geometry_shader4) || defined(GL_EXT_geometry_shader4) || defined(GL_NV_geometry_program4) || defined(GL_NV_geometry_shader4)
		if (geometryShaderProgram && Driver->queryFeature(EVDF_GEOMETRY_SHADER))
		{
			if (!createShader(GL_GEOMETRY_SHADER_EXT, geometryShaderProgram))
				return;
#if defined(GL_ARB_geometry_shader4) || defined(GL_EXT_geometry_shader4) || defined(GL_NV_geometry_shader4)
			if (Program2) // Geometry shaders are supported only in OGL2.x+ drivers.
			{
				Driver->extGlProgramParameteri(Program2, GL_GEOMETRY_INPUT_TYPE_EXT, Driver->primitiveTypeToGL(inType));
				Driver->extGlProgramParameteri(Program2, GL_GEOMETRY_OUTPUT_TYPE_EXT, Driver->primitiveTypeToGL(outType));
				if (verticesOut == 0)
					Driver->extGlProgramParameteri(Program2, GL_GEOMETRY_VERTICES_OUT_EXT, Driver->MaxGeometryVerticesOut);
				else
					Driver->extGlProgramParameteri(Program2, GL_GEOMETRY_VERTICES_OUT_EXT, Math::min_(verticesOut, Driver->MaxGeometryVerticesOut));
			}
#elif defined(GL_NV_geometry_program4)
			if (verticesOut == 0)
				Driver->extGlProgramVertexLimit(GL_GEOMETRY_PROGRAM_NV, Driver->MaxGeometryVerticesOut);
			else
				Driver->extGlProgramVertexLimit(GL_GEOMETRY_PROGRAM_NV, min_(verticesOut, Driver->MaxGeometryVerticesOut));
#endif
		}
#endif
		//将shader obj 链接到Program上
		if (!linkProgram())
			return;

		// register myself as new material
		// 将自己注册为一个驱动的新材质
		outMaterialTypeNr = Driver->addMaterialRenderer(this);
	}


	bool COpenGLSLMaterialRenderer::OnRender(IMaterialRendererServices* service,
		E_VERTEX_TYPE vtxtype)
	{
		// call callback to set shader constants
		if (CallBack && (Program || Program2))
			CallBack->OnSetConstants(this, UserData);

		return true;
	}


	void COpenGLSLMaterialRenderer::OnSetMaterial(const SMaterial& material,
		const SMaterial& lastMaterial,
		bool resetAllRenderstates,
		IMaterialRendererServices* services)
	{
		if (material.MaterialType != lastMaterial.MaterialType || resetAllRenderstates)
		{
			if (Program2)
				Driver->extGlUseProgram(Program2);
			else if (Program)
				Driver->extGlUseProgramObject(Program);

			if (BaseMaterial)
				BaseMaterial->OnSetMaterial(material, material, true, this);
		}

		//let callback know used material
		if (CallBack)
			CallBack->OnSetMaterial(material);

		for (UINT32 i = 0; i<MATERIAL_MAX_TEXTURES; ++i)
			Driver->setActiveTexture(i, material.getTexture(i));
		Driver->setBasicRenderStates(material, lastMaterial, resetAllRenderstates);
	}


	void COpenGLSLMaterialRenderer::OnUnsetMaterial()
	{
		if (Program)
			Driver->extGlUseProgramObject(0);
		if (Program2)
			Driver->extGlUseProgram(0);

		if (BaseMaterial)
			BaseMaterial->OnUnsetMaterial();
	}


	//! Returns if the material is transparent.
	bool COpenGLSLMaterialRenderer::isTransparent() const
	{
		return BaseMaterial ? BaseMaterial->isTransparent() : false;
	}


	bool COpenGLSLMaterialRenderer::createProgram()
	{
		if (Driver->Version >= 200)
			Program2 = Driver->extGlCreateProgram();
		else
			Program = Driver->extGlCreateProgramObject();
		return true;
	}


	bool COpenGLSLMaterialRenderer::createShader(GLenum shaderType, const char* shader)
	{
		if (Program2)
		{
			GLuint shaderHandle = Driver->extGlCreateShader(shaderType);
			Driver->extGlShaderSource(shaderHandle, 1, &shader, NULL);
			Driver->extGlCompileShader(shaderHandle);

			GLint status = 0;

#ifdef GL_VERSION_2_0
			//查询编译状态
			Driver->extGlGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &status);
#endif
			//如果编译不成功
			if (status != GL_TRUE)
			{
				Printer::log("GLSL shader failed to compile", LML_ERROR);
				// check error message and log it
				GLint maxLength = 0;
				GLint length;
#ifdef GL_VERSION_2_0
				//驱动消息日志长度
				Driver->extGlGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH,
					&maxLength);
#endif
				if (maxLength)
				{
					GLchar *infoLog = new GLchar[maxLength];
					//获取消息日志
					Driver->extGlGetShaderInfoLog(shaderHandle, maxLength, &length, infoLog);
					Printer::log(reinterpret_cast<const c8*>(infoLog), LML_ERROR);
					delete[] infoLog;
				}

				return false;
			}
			//关联shader
			Driver->extGlAttachShader(Program2, shaderHandle);
		}
		else
		{
			GLhandleARB shaderHandle = Driver->extGlCreateShaderObject(shaderType);

			Driver->extGlShaderSourceARB(shaderHandle, 1, &shader, NULL);
			Driver->extGlCompileShaderARB(shaderHandle);

			GLint status = 0;

#ifdef GL_ARB_shader_objects
			//获取编译OBJ参数
			Driver->extGlGetObjectParameteriv(shaderHandle, GL_OBJECT_COMPILE_STATUS_ARB, &status);
#endif

			if (!status)
			{
				Printer::log("GLSL shader failed to compile", LML_ERROR);
				// check error message and log it
				GLint maxLength = 0;
				GLsizei length;
#ifdef GL_ARB_shader_objects
				Driver->extGlGetObjectParameteriv(shaderHandle,
					GL_OBJECT_INFO_LOG_LENGTH_ARB, &maxLength);
#endif
				if (maxLength)
				{
					GLcharARB *infoLog = new GLcharARB[maxLength];
					Driver->extGlGetInfoLog(shaderHandle, maxLength, &length, infoLog);
					Printer::log(reinterpret_cast<const c8*>(infoLog), LML_ERROR);
					delete[] infoLog;
				}

				return false;
			}
			//编译成功，关联对象
			Driver->extGlAttachObject(Program, shaderHandle);
		}
		return true;
	}

	//链接Program
	bool COpenGLSLMaterialRenderer::linkProgram()
	{
		if (Program2)
		{
			Driver->extGlLinkProgram(Program2);

			GLint status = 0;

#ifdef GL_VERSION_2_0
			Driver->extGlGetProgramiv(Program2, GL_LINK_STATUS, &status);
#endif
			//判断是否有错误
			if (!status)
			{
				Printer::log("GLSL shader program failed to link", LML_ERROR);
				// check error message and log it
				GLint maxLength = 0;
				GLsizei length;
#ifdef GL_VERSION_2_0
				Driver->extGlGetProgramiv(Program2, GL_INFO_LOG_LENGTH, &maxLength);
#endif
				if (maxLength)
				{
					GLchar *infoLog = new GLchar[maxLength];
					Driver->extGlGetProgramInfoLog(Program2, maxLength, &length, infoLog);
					Printer::log(reinterpret_cast<const c8*>(infoLog), LML_ERROR);
					delete[] infoLog;
				}

				return false;
			}

			// get uniforms information

			GLint num = 0;
#ifdef GL_VERSION_2_0
			//获取活动的Uniform数量
			Driver->extGlGetProgramiv(Program2, GL_ACTIVE_UNIFORMS, &num);
#endif

			if (num == 0)
			{
				// no uniforms
				return true;
			}

			GLint maxlen = 0;
#ifdef GL_VERSION_2_0
			//获取Uniform名字字符最大长度
			Driver->extGlGetProgramiv(Program2, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxlen);
#endif

			if (maxlen == 0)
			{
				Printer::log("GLSL: failed to retrieve uniform information", LML_ERROR);
				return false;
			}

			// seems that some implementations use an extra null terminator
			++maxlen;
			c8 *buf = new c8[maxlen];

			UniformInfo.clear();
			//UniformInfo.reallocate(num);
			UniformInfo.reserve(num);

			for (GLint i = 0; i < num; ++i)
			{
				SUniformInfo ui;
				memset(buf, 0, maxlen);

				GLint size;
				//依次获取Uniform属性(名字和类型)
				Driver->extGlGetActiveUniform(Program2, i, maxlen, 0, &size, &ui.type, reinterpret_cast<GLchar*>(buf));
				ui.name = buf;

				UniformInfo.push_back(ui);
			}

			delete[] buf;
		}
		else
		{
			Driver->extGlLinkProgramARB(Program);

			GLint status = 0;

#ifdef GL_ARB_shader_objects
			Driver->extGlGetObjectParameteriv(Program, GL_OBJECT_LINK_STATUS_ARB, &status);
#endif

			if (!status)
			{
				Printer::log("GLSL shader program failed to link", LML_ERROR);
				// check error message and log it
				GLint maxLength = 0;
				GLsizei length;
#ifdef GL_ARB_shader_objects
				Driver->extGlGetObjectParameteriv(Program,
					GL_OBJECT_INFO_LOG_LENGTH_ARB, &maxLength);
#endif
				if (maxLength)
				{
					GLcharARB *infoLog = new GLcharARB[maxLength];
					Driver->extGlGetInfoLog(Program, maxLength, &length, infoLog);
					Printer::log(reinterpret_cast<const c8*>(infoLog), LML_ERROR);
					delete[] infoLog;
				}

				return false;
			}

			// get uniforms information

			GLint num = 0;
#ifdef GL_ARB_shader_objects
			Driver->extGlGetObjectParameteriv(Program, GL_OBJECT_ACTIVE_UNIFORMS_ARB, &num);
#endif

			if (num == 0)
			{
				// no uniforms
				return true;
			}

			GLint maxlen = 0;
#ifdef GL_ARB_shader_objects
			Driver->extGlGetObjectParameteriv(Program, GL_OBJECT_ACTIVE_UNIFORM_MAX_LENGTH_ARB, &maxlen);
#endif

			if (maxlen == 0)
			{
				Printer::log("GLSL: failed to retrieve uniform information", LML_ERROR);
				return false;
			}

			// seems that some implementations use an extra null terminator
			++maxlen;
			c8 *buf = new c8[maxlen];

			UniformInfo.clear();
			//UniformInfo.reallocate(num);
			UniformInfo.reserve(num);

			for (int i = 0; i < num; ++i)
			{
				SUniformInfo ui;
				memset(buf, 0, maxlen);

				GLint size;
				Driver->extGlGetActiveUniformARB(Program, i, maxlen, 0, &size, &ui.type, reinterpret_cast<GLcharARB*>(buf));
				ui.name = buf;

				UniformInfo.push_back(ui);
			}

			delete[] buf;
		}

		return true;
	}

	//设置基本状态
	void COpenGLSLMaterialRenderer::setBasicRenderStates(const SMaterial& material,
		const SMaterial& lastMaterial,
		bool resetAllRenderstates)
	{
		// forward
		Driver->setBasicRenderStates(material, lastMaterial, resetAllRenderstates);
	}

	//设置VS Constant
	bool COpenGLSLMaterialRenderer::setVertexShaderConstant(const c8* name, const FLOAT32* floats, int count)
	{
		return setPixelShaderConstant(name, floats, count);
	}

	bool COpenGLSLMaterialRenderer::setVertexShaderConstant(const c8* name, const bool* bools, int count)
	{
		return setPixelShaderConstant(name, bools, count);
	}

	bool COpenGLSLMaterialRenderer::setVertexShaderConstant(const c8* name, const SINT32* ints, int count)
	{
		return setPixelShaderConstant(name, ints, count);
	}

	void COpenGLSLMaterialRenderer::setVertexShaderConstant(const FLOAT32* data, SINT32 startRegister, SINT32 constantAmount)
	{
		Printer::log("Cannot set constant, please use high level shader call instead.", LML_WARNING);
	}
	//设置PS Constant
	bool COpenGLSLMaterialRenderer::setPixelShaderConstant(const c8* name, const FLOAT32* floats, int count)
	{
		UINT32 i;
		const UINT32 num = UniformInfo.size();

		for (i = 0; i < num; ++i)
		{
			if (UniformInfo[i].name == name)
				break;
		}

		if (i == num)
			return false;

#if defined(GL_VERSION_2_0)||defined(GL_ARB_shader_objects)
		GLint Location = 0;
		if (Program2)
			Location = Driver->extGlGetUniformLocation(Program2, name);
		else
			Location = Driver->extGlGetUniformLocationARB(Program, name);

		bool status = true;

		switch (UniformInfo[i].type)
		{
		case GL_FLOAT:
			Driver->extGlUniform1fv(Location, count, floats);
			break;
		case GL_FLOAT_VEC2:
			Driver->extGlUniform2fv(Location, count / 2, floats);
			break;
		case GL_FLOAT_VEC3:
			Driver->extGlUniform3fv(Location, count / 3, floats);
			break;
		case GL_FLOAT_VEC4:
			Driver->extGlUniform4fv(Location, count / 4, floats);
			break;
		case GL_FLOAT_MAT2:
			Driver->extGlUniformMatrix2fv(Location, count / 4, false, floats);
			break;
		case GL_FLOAT_MAT3:
			Driver->extGlUniformMatrix3fv(Location, count / 9, false, floats);
			break;
		case GL_FLOAT_MAT4:
			Driver->extGlUniformMatrix4fv(Location, count / 16, false, floats);
			break;
		case GL_SAMPLER_1D:
		case GL_SAMPLER_2D:
		case GL_SAMPLER_3D:
		case GL_SAMPLER_CUBE:
		case GL_SAMPLER_1D_SHADOW:
		case GL_SAMPLER_2D_SHADOW:
		{
			const GLint id = static_cast<GLint>(*floats);
			Driver->extGlUniform1iv(Location, 1, &id);
		}
		break;
		default:
			status = false;
			break;
		}
		return status;
#else
		return false;
#endif
	}

	bool COpenGLSLMaterialRenderer::setPixelShaderConstant(const c8* name, const bool* bools, int count)
	{
		UINT32 i;
		const UINT32 num = UniformInfo.size();

		for (i = 0; i < num; ++i)
		{
			if (UniformInfo[i].name == name)
				break;
		}

		if (i == num)
			return false;

#if defined(GL_VERSION_2_0)||defined(GL_ARB_shader_objects)
		GLint Location = 0;
		if (Program2)
			Location = Driver->extGlGetUniformLocation(Program2, name);
		else
			Location = Driver->extGlGetUniformLocationARB(Program, name);

		bool status = true;

		switch (UniformInfo[i].type)
		{
		case GL_BOOL:
			Driver->extGlUniform1iv(Location, count, (GLint*)bools);
			break;
		case GL_BOOL_VEC2:
			Driver->extGlUniform2iv(Location, count / 2, (GLint*)bools);
			break;
		case GL_BOOL_VEC3:
			Driver->extGlUniform3iv(Location, count / 3, (GLint*)bools);
			break;
		case GL_BOOL_VEC4:
			Driver->extGlUniform4iv(Location, count / 4, (GLint*)bools);
			break;
		default:
			status = false;
			break;
		}
		return status;
#else
		return false;
#endif
	}

	bool COpenGLSLMaterialRenderer::setPixelShaderConstant(const c8* name, const SINT32* ints, int count)
	{
		UINT32 i;
		const UINT32 num = UniformInfo.size();

		for (i = 0; i < num; ++i)
		{
			if (UniformInfo[i].name == name)
				break;
		}

		if (i == num)
			return false;

#if defined(GL_VERSION_2_0)||defined(GL_ARB_shader_objects)
		GLint Location = 0;
		if (Program2)
			Location = Driver->extGlGetUniformLocation(Program2, name);
		else
			Location = Driver->extGlGetUniformLocationARB(Program, name);

		bool status = true;

		switch (UniformInfo[i].type)
		{
		case GL_INT:
			Driver->extGlUniform1iv(Location, count, ints);
			break;
		case GL_INT_VEC2:
			Driver->extGlUniform2iv(Location, count / 2, ints);
			break;
		case GL_INT_VEC3:
			Driver->extGlUniform3iv(Location, count / 3, ints);
			break;
		case GL_INT_VEC4:
			Driver->extGlUniform4iv(Location, count / 4, ints);
			break;
		case GL_SAMPLER_1D:
		case GL_SAMPLER_2D:
		case GL_SAMPLER_3D:
		case GL_SAMPLER_CUBE:
		case GL_SAMPLER_1D_SHADOW:
		case GL_SAMPLER_2D_SHADOW:
			Driver->extGlUniform1iv(Location, 1, ints);
			break;
		default:
			status = false;
			break;
		}
		return status;
#else
		return false;
#endif
	}

	void COpenGLSLMaterialRenderer::setPixelShaderConstant(const FLOAT32* data, SINT32 startRegister, SINT32 constantAmount)
	{
		Printer::log("Cannot set constant, use high level shader call.", LML_WARNING);
	}

	IVideoDriver* COpenGLSLMaterialRenderer::getVideoDriver()
	{
		return Driver;
	}
}


#endif