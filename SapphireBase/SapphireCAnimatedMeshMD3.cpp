#include "SapphireCompileConifg.h"
#ifndef _SAPPHIRE _COMPILE_WITH_MD3_LOADER_

#include "SapphireCAnimatedMeshMD3.h"
#include "SapphireOS.h"
#include "SapphireCBlit.h"

namespace Sapphire
{

	// byte-align structures
#include "SapphirePack.h"

	//! General properties of a single animation frame.
	struct SMD3Frame
	{
		FLOAT32  mins[3];		// bounding box per frame
		FLOAT32  maxs[3];
		FLOAT32  position[3];	// position of bounding box
		FLOAT32  radius;		// radius of bounding sphere
		c8   creator[16];	// name of frame
	} PACK_STRUCT;


	//! An attachment point for another MD3 model.
	struct SMD3Tag
	{
		c8 Name[64];		//name of 'tag' as it's usually called in the md3 files try to see it as a sub-mesh/seperate mesh-part.
		FLOAT32 position[3];	//relative position of tag
		FLOAT32 rotationMatrix[9];	//3x3 rotation direction of tag
	} PACK_STRUCT;

	//!Shader
	struct SMD3Shader
	{
		c8 name[64];		// name of shader
		SINT32 shaderIndex;
	} PACK_STRUCT;

	// Default alignment
#include "SapphireUnpack.h"


	//! Constructor
	CAnimatedMeshMD3::CAnimatedMeshMD3()
		:Mesh(0), IPolShift(0), LoopMode(0), Scaling(1.f)//, FramesPerSecond(25.f)
	{
#ifdef _DEBUG
		setDebugName("CAnimatedMeshMD3");
#endif

		Mesh = new SMD3Mesh();
		MeshIPol = new SMesh();
		setInterpolationShift(0, 0);
	}


	//! Destructor
	CAnimatedMeshMD3::~CAnimatedMeshMD3()
	{
		if (Mesh)
			Mesh->drop();
		if (MeshIPol)
			MeshIPol->drop();
	}


	//! Returns the amount of frames in milliseconds. If the amount is 1, it is a static (=non animated) mesh.
	UINT32 CAnimatedMeshMD3::getFrameCount() const
	{
		return Mesh->MD3Header.numFrames << IPolShift;
	}


	//! Rendering Hint
	void CAnimatedMeshMD3::setInterpolationShift(UINT32 shift, UINT32 loopMode)
	{
		IPolShift = shift;
		LoopMode = loopMode;
	}


	//! returns amount of mesh buffers.
	UINT32 CAnimatedMeshMD3::getMeshBufferCount() const
	{
		return MeshIPol->getMeshBufferCount();
	}


	//! returns pointer to a mesh buffer
	IMeshBuffer* CAnimatedMeshMD3::getMeshBuffer(UINT32 nr) const
	{
		return MeshIPol->getMeshBuffer(nr);
	}


	//! Returns pointer to a mesh buffer which fits a material
	IMeshBuffer* CAnimatedMeshMD3::getMeshBuffer(const SMaterial &material) const
	{
		return MeshIPol->getMeshBuffer(material);
	}


	void CAnimatedMeshMD3::setMaterialFlag(E_MATERIAL_FLAG flag, bool newvalue)
	{
		MeshIPol->setMaterialFlag(flag, newvalue);
	}


	//! set the hardware mapping hint, for driver
	void CAnimatedMeshMD3::setHardwareMappingHint(E_HARDWARE_MAPPING newMappingHint,
		E_BUFFER_TYPE buffer)
	{
		MeshIPol->setHardwareMappingHint(newMappingHint, buffer);
	}


	//! flags the meshbuffer as changed, reloads hardware buffers
	void CAnimatedMeshMD3::setDirty(E_BUFFER_TYPE buffer)
	{
		MeshIPol->setDirty(buffer);
	}


	//! set user axis aligned bounding box
	void CAnimatedMeshMD3::setBoundingBox(const AxisAlignedBox& box)
	{
		MeshIPol->setBoundingBox(box);
	}


	//! Returns the animated tag list based on a detail level. 0 is the lowest, 255 the highest detail.
	SMD3QuaternionTagList *CAnimatedMeshMD3::getTagList(SINT32 frame, SINT32 detailLevel, SINT32 startFrameLoop, SINT32 endFrameLoop)
	{
		if (0 == Mesh)
			return 0;

		getMesh(frame, detailLevel, startFrameLoop, endFrameLoop);
		return &TagListIPol;
	}


	//! Returns the animated mesh based on a detail level. 0 is the lowest, 255 the highest detail.
	IMesh* CAnimatedMeshMD3::getMesh(SINT32 frame, SINT32 detailLevel, SINT32 startFrameLoop, SINT32 endFrameLoop)
	{
		if (0 == Mesh)
			return 0;

		//! check if we have the mesh in our private cache
		SCacheInfo candidate(frame, startFrameLoop, endFrameLoop);
		if (candidate == Current)
			return MeshIPol;

		startFrameLoop = SINT32_max(0, startFrameLoop >> IPolShift);
		endFrameLoop = if_c_a_else_b(endFrameLoop < 0, Mesh->MD3Header.numFrames - 1, endFrameLoop >> IPolShift);

		const UINT32 mask = 1 << IPolShift;

		SINT32 frameA;
		SINT32 frameB;
		FLOAT32 iPol;

		if (LoopMode)
		{
			// correct frame to "pixel center"
			frame -= mask >> 1;

			// interpolation
			iPol = FLOAT32(frame & (mask - 1)) * reciprocal(FLOAT32(mask));

			// wrap anim
			frame >>= IPolShift;
			frameA = if_c_a_else_b(frame < startFrameLoop, endFrameLoop, frame);
			frameB = if_c_a_else_b(frameA + 1 > endFrameLoop, startFrameLoop, frameA + 1);
		}
		else
		{
			// correct frame to "pixel center"
			frame -= mask >> 1;

			iPol = FLOAT32(frame & (mask - 1)) * reciprocal(FLOAT32(mask));

			// clamp anim
			frame >>= IPolShift;
			frameA = SINT32_clamp(frame, startFrameLoop, endFrameLoop);
			frameB = SINT32_min(frameA + 1, endFrameLoop);
		}

		// build current vertex
		for (UINT32 i = 0; i != Mesh->Buffer.size(); ++i)
		{
			buildVertexArray(frameA, frameB, iPol,
				Mesh->Buffer[i],
				(SMeshBufferLightMap*)MeshIPol->getMeshBuffer(i));
		}
		MeshIPol->recalculateBoundingBox();

		// build current tags
		buildTagArray(frameA, frameB, iPol);

		Current = candidate;
		return MeshIPol;
	}


	//! create a Irrlicht MeshBuffer for a MD3 MeshBuffer
	IMeshBuffer * CAnimatedMeshMD3::createMeshBuffer(const SMD3MeshBuffer* source,
		IFileSystem* fs, IVideoDriver * driver)
	{
		SMeshBufferLightMap * dest = new SMeshBufferLightMap();
		dest->Vertices.reserve(source->MeshHeader.numVertices);
		dest->Indices.reserve(source->Indices.size());

		UINT32 i;

		// fill in static face info
		for (i = 0; i < source->Indices.size(); i += 3)
		{
			dest->Indices[i + 0] = (UINT16)source->Indices[i + 0];
			dest->Indices[i + 1] = (UINT16)source->Indices[i + 1];
			dest->Indices[i + 2] = (UINT16)source->Indices[i + 2];
		}

		// fill in static vertex info
		for (i = 0; i != (UINT32)source->MeshHeader.numVertices; ++i)
		{
			S3DVertex2TCoords &v = dest->Vertices[i];
			v.Color = 0xFFFFFFFF;
			v.TCoords.x = source->Tex[i].u;
			v.TCoords.y = source->Tex[i].v;
			v.TCoords2.x = 0.f;
			v.TCoords2.y = 0.f;
		}

		// load static texture
		UINT32 pos = 0;
		Quake3::tTexArray textureArray;
		Quake3::getTextures(textureArray, source->Shader, pos, fs, driver);
		dest->Material.MaterialType = EMT_SOLID;
		dest->Material.setTexture(0, textureArray[0]);
		dest->Material.Lighting = false;

		return dest;
	}


	//! build final mesh's vertices from frames frameA and frameB with linear interpolation.
	void CAnimatedMeshMD3::buildVertexArray(UINT32 frameA, UINT32 frameB, FLOAT32 interpolate,
		const SMD3MeshBuffer* source,
		SMeshBufferLightMap* dest)
	{
		const UINT32 frameOffsetA = frameA * source->MeshHeader.numVertices;
		const UINT32 frameOffsetB = frameB * source->MeshHeader.numVertices;
		const FLOAT32 scale = (1.f / 64.f);

		for (SINT32 i = 0; i != source->MeshHeader.numVertices; ++i)
		{
			S3DVertex2TCoords &v = dest->Vertices[i];

			const SMD3Vertex &vA = source->Vertices[frameOffsetA + i];
			const SMD3Vertex &vB = source->Vertices[frameOffsetB + i];

			// position
			v.Pos.x = scale * (vA.position[0] + interpolate * (vB.position[0] - vA.position[0]));
			v.Pos.y = scale * (vA.position[2] + interpolate * (vB.position[2] - vA.position[2]));
			v.Pos.z = scale * (vA.position[1] + interpolate * (vB.position[1] - vA.position[1]));

			// normal
			const Vector3 nA(Quake3::getMD3Normal(vA.normal[0], vA.normal[1]));
			const Vector3 nB(Quake3::getMD3Normal(vB.normal[0], vB.normal[1]));

			v.Normal.x = nA.x + interpolate * (nB.x - nA.x);
			v.Normal.y = nA.z + interpolate * (nB.z - nA.z);
			v.Normal.z = nA.y + interpolate * (nB.y - nA.y);
		}

		dest->recalculateBoundingBox();
	}


	//! build final mesh's tag from frames frameA and frameB with linear interpolation.
	void CAnimatedMeshMD3::buildTagArray(UINT32 frameA, UINT32 frameB, FLOAT32 interpolate)
	{
		const UINT32 frameOffsetA = frameA * Mesh->MD3Header.numTags;
		const UINT32 frameOffsetB = frameB * Mesh->MD3Header.numTags;

		for (SINT32 i = 0; i != Mesh->MD3Header.numTags; ++i)
		{
			SMD3QuaternionTag &d = TagListIPol[i];

			const SMD3QuaternionTag &qA = Mesh->TagList[frameOffsetA + i];
			const SMD3QuaternionTag &qB = Mesh->TagList[frameOffsetB + i];

			// rotation
			d.rotation.Slerp(qA.rotation, qB.rotation, interpolate);

			// position
			d.position.x = qA.position.x + interpolate * (qB.position.x - qA.position.x);
			d.position.y = qA.position.y + interpolate * (qB.position.y - qA.position.y);
			d.position.z = qA.position.z + interpolate * (qB.position.z - qA.position.z);
		}
	}


	/*!
	loads a model
	*/
	bool CAnimatedMeshMD3::loadModelFile(UINT32 modelIndex, IReadFile* file,
		IFileSystem* fs, IVideoDriver* driver)
	{
		if (!file)
			return false;

		//! Check MD3Header
		{
			file->read(&Mesh->MD3Header, sizeof(SMD3Header));

			if (strncmp("IDP3", Mesh->MD3Header.headerID, 4))
			{
				Printer::log("MD3 Loader: invalid header");
				return false;
			}
		}

		//! store model name
		Mesh->Name = file->getFileName();

		UINT32 i;

		//! Frame Data (ignore)
#if 0
		SMD3Frame frameImport;
		file->seek(Mesh->MD3Header.frameStart);
		for (i = 0; i != Mesh->MD3Header.numFrames; ++i)
		{
			file->read(&frameImport, sizeof(frameImport));
		}
#endif

		//! Tag Data
		const UINT32 totalTags = Mesh->MD3Header.numTags * Mesh->MD3Header.numFrames;

		SMD3Tag import;

		file->seek(Mesh->MD3Header.tagStart);
		Mesh->TagList.set_used(totalTags);
		for (i = 0; i != totalTags; ++i)
		{
			file->read(&import, sizeof(import));

			SMD3QuaternionTag &exp = Mesh->TagList[i];

			//! tag name
			exp.Name = import.Name;

			//! position
			exp.position.x = import.position[0]; 
			exp.position.y = import.position[2];
			exp.position.z = import.position[1];

			//! construct quaternion from a RH 3x3 Matrix
			exp.rotation.set(import.rotationMatrix[7],
				0.f,
				-import.rotationMatrix[6],
				1 + import.rotationMatrix[8]);
			//exp.rotation.normalize();
			exp.rotation.normalise();
		}

		//! Meshes  
		UINT32 offset = Mesh->MD3Header.tagEnd;

		for (i = 0; i != (UINT32)Mesh->MD3Header.numMeshes; ++i)
		{
			//! construct a new mesh buffer
			SMD3MeshBuffer * buf = new SMD3MeshBuffer();

			// !read mesh header info
			SMD3MeshHeader &meshHeader = buf->MeshHeader;

			//! read mesh info
			file->seek(offset);
			file->read(&meshHeader, sizeof(SMD3MeshHeader));

			//! prepare memory
			buf->Vertices.reserve(meshHeader.numVertices * Mesh->MD3Header.numFrames);
			buf->Indices.reserve(meshHeader.numTriangles * 3);
			buf->Tex.reserve(meshHeader.numVertices);

			//! read skins (shaders). should be 1 per meshbuffer
			SMD3Shader skin;
			file->seek(offset + buf->MeshHeader.offset_shaders);
			for (SINT32 g = 0; g != buf->MeshHeader.numShader; ++g)
			{
				file->read(&skin, sizeof(skin));

				path name;
				cutFilenameExtension(name, skin.name);
				//name.replace('\\', '/');
				name = StringUtil::replaceAll(name, "\\", "/");
				buf->Shader = name;
			}

			//! read texture coordinates
			file->seek(offset + buf->MeshHeader.offset_st);
			//file->read(buf->Tex.pointer(), buf->MeshHeader.numVertices * sizeof(SMD3TexCoord));
			file->read(buf->Tex.begin()._Ptr, buf->MeshHeader.numVertices * sizeof(SMD3TexCoord));

			//! read vertices
			file->seek(offset + meshHeader.vertexStart);
			//file->read(buf->Vertices.pointer(), Mesh->MD3Header.numFrames * meshHeader.numVertices * sizeof(SMD3Vertex));
			file->read(buf->Vertices.begin()._Ptr, Mesh->MD3Header.numFrames * meshHeader.numVertices * sizeof(SMD3Vertex));

			//! read indices
			file->seek(offset + meshHeader.offset_triangles);
			//file->read(buf->Indices.pointer(), meshHeader.numTriangles * sizeof(SMD3Face));
			file->read(buf->Indices.begin()._Ptr, meshHeader.numTriangles * sizeof(SMD3Face));

			//! store meshBuffer
			Mesh->Buffer.push_back(buf);

			offset += meshHeader.offset_end;
		}

		// Init Mesh Interpolation
		for (i = 0; i != Mesh->Buffer.size(); ++i)
		{
			IMeshBuffer * buffer = createMeshBuffer(Mesh->Buffer[i], fs, driver);
			MeshIPol->addMeshBuffer(buffer);
			buffer->drop();
		}
		MeshIPol->recalculateBoundingBox();

		// Init Tag Interpolation
		for (i = 0; i != (UINT32)Mesh->MD3Header.numTags; ++i)
		{
			TagListIPol.push_back(Mesh->TagList[i]);
		}

		return true;
	}


	SMD3Mesh * CAnimatedMeshMD3::getOriginalMesh()
	{
		return Mesh;
	}


	//! Returns an axis aligned bounding box
	const AxisAlignedBox& CAnimatedMeshMD3::getBoundingBox() const
	{
		return MeshIPol->BoundingBox;
	}


	//! Returns the type of the animated mesh.
	E_ANIMATED_MESH_TYPE CAnimatedMeshMD3::getMeshType() const
	{
		return EAMT_MD3;
	}

}

#endif