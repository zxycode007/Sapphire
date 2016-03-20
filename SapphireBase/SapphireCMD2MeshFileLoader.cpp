#include "SapphireCompileConifg.h"
#ifdef _SAPPHIRE_COMPILE_WITH_MD2_LOADER_

#include "SapphireCMD2MeshFileLoader.h"
#include "SapphireCAnimatedMeshMD2.h"
#include "SapphireOS.h"

namespace Sapphire
{
	// structs needed to load the md2-format

	const SINT32 MD2_MAGIC_NUMBER = 844121161;
	const SINT32 MD2_VERSION = 8;
	const SINT32 MD2_MAX_VERTS = 2048;

	// byte-align structures
#include "SapphirePack.h"

	struct SMD2Header
	{
		SINT32 magic;           // four character code "IDP2"
		SINT32 version;         // must be 8
		SINT32 skinWidth;	     // width of the texture
		SINT32 skinHeight;      // height of the texture
		SINT32 frameSize;       // size in bytes of an animation frame
		SINT32 numSkins;        // number of textures
		SINT32 numVertices;     // total number of vertices
		SINT32 numTexcoords;    // number of vertices with texture coords
		SINT32 numTriangles;    // number of triangles
		SINT32 numGlCommands;   // number of opengl commands (triangle strip or triangle fan)
		SINT32 numFrames;       // animation keyframe count
		SINT32 offsetSkins;     // offset in bytes to 64 character skin names
		SINT32 offsetTexcoords; // offset in bytes to texture coordinate list
		SINT32 offsetTriangles; // offset in bytes to triangle list
		SINT32 offsetFrames;    // offset in bytes to frame list
		SINT32 offsetGlCommands;// offset in bytes to opengl commands
		SINT32 offsetEnd;       // offset in bytes to end of file
	} PACK_STRUCT;

	struct SMD2Vertex
	{
		UINT8 vertex[3];        // [0] = X, [1] = Z, [2] = Y
		UINT8 lightNormalIndex; // index in the normal table
	} PACK_STRUCT;

	struct SMD2Frame
	{
		FLOAT32	scale[3];           // first scale the vertex position
		FLOAT32	translate[3];       // then translate the position
		c8	name[16];           // the name of the animation that this key belongs to
		SMD2Vertex vertices[1]; // vertex 1 of SMD2Header.numVertices
	} PACK_STRUCT;

	struct SMD2Triangle
	{
		UINT16 vertexIndices[3];
		UINT16 textureIndices[3];
	} PACK_STRUCT;

	struct SMD2TextureCoordinate
	{
		SINT16 s;
		SINT16 t;
	} PACK_STRUCT;

	struct SMD2GLCommand
	{
		FLOAT32 s, t;
		SINT32 vertexIndex;
	} PACK_STRUCT;

	// Default alignment
#include "SapphireUnpack.h"

	//! Constructor
	CMD2MeshFileLoader::CMD2MeshFileLoader()
	{
#ifdef _DEBUG
		setDebugName("CMD2MeshFileLoader");
#endif
	}


	//! returns true if the file maybe is able to be loaded by this class
	//! based on the file extension (e.g. ".bsp")
	bool CMD2MeshFileLoader::isALoadableFileExtension(const path& filename) const
	{
		return hasFileExtension(filename, "md2");
	}


	//! creates/loads an animated mesh from the file.
	//! \return Pointer to the created mesh. Returns 0 if loading failed.
	//! If you no longer need the mesh, you should call IAnimatedMesh::drop().
	//! See IReferenceCounted::drop() for more information.
	IAnimatedMesh* CMD2MeshFileLoader::createMesh(IReadFile* file)
	{
		IAnimatedMesh* msh = new CAnimatedMeshMD2();
		if (msh)
		{
			if (loadFile(file, (CAnimatedMeshMD2*)msh))
				return msh;

			msh->drop();
		}

		return 0;
	}

	//! loads an md2 file
	bool CMD2MeshFileLoader::loadFile(IReadFile* file, CAnimatedMeshMD2* mesh)
	{
		if (!file)
			return false;

		SMD2Header header;

		file->read(&header, sizeof(SMD2Header));

#if  SAPPHIRE_CONFIG_ENDIAN == SAPPHIRE_BIG_ENDIAN
		header.magic = Byteswap::byteswap(header.magic);
		header.version = Byteswap::byteswap(header.version);
		header.skinWidth = Byteswap::byteswap(header.skinWidth);
		header.skinHeight = Byteswap::byteswap(header.skinHeight);
		header.frameSize = Byteswap::byteswap(header.frameSize);
		header.numSkins = Byteswap::byteswap(header.numSkins);
		header.numVertices = Byteswap::byteswap(header.numVertices);
		header.numTexcoords = Byteswap::byteswap(header.numTexcoords);
		header.numTriangles = Byteswap::byteswap(header.numTriangles);
		header.numGlCommands = Byteswap::byteswap(header.numGlCommands);
		header.numFrames = Byteswap::byteswap(header.numFrames);
		header.offsetSkins = Byteswap::byteswap(header.offsetSkins);
		header.offsetTexcoords = Byteswap::byteswap(header.offsetTexcoords);
		header.offsetTriangles = Byteswap::byteswap(header.offsetTriangles);
		header.offsetFrames = Byteswap::byteswap(header.offsetFrames);
		header.offsetGlCommands = Byteswap::byteswap(header.offsetGlCommands);
		header.offsetEnd = Byteswap::byteswap(header.offsetEnd);
#endif

		if (header.magic != MD2_MAGIC_NUMBER || header.version != MD2_VERSION)
		{
			Printer::log("MD2 Loader: Wrong file header", file->getFileName(), LML_WARNING);
			return false;
		}

		//
		// prepare mesh and allocate memory
		//

		mesh->FrameCount = header.numFrames;

		// create keyframes
		//mesh->FrameTransforms.set_used(header.numFrames);
		mesh->FrameTransforms.resize(header.numFrames);

		// create vertex arrays for each keyframe
		if (mesh->FrameList)
			delete[] mesh->FrameList;
		mesh->FrameList = new vector<CAnimatedMeshMD2::SMD2Vert>::type[header.numFrames];

		// allocate space in vertex arrays
		SINT32 i;
		for (i = 0; i<header.numFrames; ++i)
			mesh->FrameList[i].reserve(header.numVertices);//mesh->FrameList[i].reallocate(header.numVertices);

		// allocate interpolation buffer vertices
		//mesh->InterpolationBuffer->Vertices.set_used(header.numTriangles * 3);
		mesh->InterpolationBuffer->Vertices.resize(header.numTriangles * 3);

		// populate triangles
		//mesh->InterpolationBuffer->Indices.reallocate(header.numTriangles * 3);
		mesh->InterpolationBuffer->Indices.reserve(header.numTriangles * 3);
		const SINT32 count = header.numTriangles * 3;
		for (i = 0; i<count; i += 3)
		{
			mesh->InterpolationBuffer->Indices.push_back(i);
			mesh->InterpolationBuffer->Indices.push_back(i + 1);
			mesh->InterpolationBuffer->Indices.push_back(i + 2);
		}

		//
		// read texture coordinates
		//

		file->seek(header.offsetTexcoords);
		SMD2TextureCoordinate* textureCoords = new SMD2TextureCoordinate[header.numTexcoords];

		if (!file->read(textureCoords, sizeof(SMD2TextureCoordinate)*header.numTexcoords))
		{
			delete[] textureCoords;
			Printer::log("MD2 Loader: Error reading TextureCoords.", file->getFileName(), LML_ERROR);
			return false;
		}

#if SAPPHIRE_CONFIG_ENDIAN == SAPPHIRE_BIG_ENDIAN
		for (i = 0; i<header.numTexcoords; ++i)
		{
			textureCoords[i].s = Byteswap::byteswap(textureCoords[i].s);
			textureCoords[i].t = Byteswap::byteswap(textureCoords[i].t);
		}
#endif

		// read Triangles

		file->seek(header.offsetTriangles);

		SMD2Triangle *triangles = new SMD2Triangle[header.numTriangles];
		if (!file->read(triangles, header.numTriangles *sizeof(SMD2Triangle)))
		{
			delete[] triangles;
			delete[] textureCoords;

			Printer::log("MD2 Loader: Error reading triangles.", file->getFileName(), LML_ERROR);
			return false;
		}

#if SAPPHIRE_CONFIG_ENDIAN == SAPPHIRE_BIG_ENDIAN
		for (i = 0; i<header.numTriangles; ++i)
		{
			triangles[i].vertexIndices[0] = Byteswap::byteswap(triangles[i].vertexIndices[0]);
			triangles[i].vertexIndices[1] = Byteswap::byteswap(triangles[i].vertexIndices[1]);
			triangles[i].vertexIndices[2] = Byteswap::byteswap(triangles[i].vertexIndices[2]);
			triangles[i].textureIndices[0] = Byteswap::byteswap(triangles[i].textureIndices[0]);
			triangles[i].textureIndices[1] = Byteswap::byteswap(triangles[i].textureIndices[1]);
			triangles[i].textureIndices[2] = Byteswap::byteswap(triangles[i].textureIndices[2]);
		}
#endif

		// read Vertices

		UINT8 buffer[MD2_MAX_VERTS * 4 + 128];
		SMD2Frame* frame = (SMD2Frame*)buffer;

		file->seek(header.offsetFrames);

		for (i = 0; i<header.numFrames; ++i)
		{
			// read vertices

			file->read(frame, header.frameSize);

#if SAPPHIRE_CONFIG_ENDIAN == SAPPHIRE_BIG_ENDIAN
			frame->scale[0] = Byteswap::byteswap(frame->scale[0]);
			frame->scale[1] = Byteswap::byteswap(frame->scale[1]);
			frame->scale[2] = Byteswap::byteswap(frame->scale[2]);
			frame->translate[0] = Byteswap::byteswap(frame->translate[0]);
			frame->translate[1] = Byteswap::byteswap(frame->translate[1]);
			frame->translate[2] = Byteswap::byteswap(frame->translate[2]);
#endif
			//
			// store frame data
			//

			CAnimatedMeshMD2::SAnimationData adata;
			adata.begin = i;
			adata.end = i;
			adata.fps = 7;

			// Add new named animation if necessary
			if (frame->name[0])
			{
				// get animation name
				for (SINT32 s = 0; s < 16 && frame->name[s] != 0 && (frame->name[s] < '0' || frame->name[s] > '9'); ++s)
				{
					adata.name += frame->name[s];
				}

				// Does this keyframe have the same animation name as the current animation?
				if (!mesh->AnimationData.empty() && mesh->AnimationData[mesh->AnimationData.size() - 1].name == adata.name)
				{
					// Increase the length of the animation
					++mesh->AnimationData[mesh->AnimationData.size() - 1].end;
				}
				else
				{
					// Add the new animation
					mesh->AnimationData.push_back(adata);
				}
			}

			// save keyframe scale and translation

			mesh->FrameTransforms[i].scale.x = frame->scale[0];
			mesh->FrameTransforms[i].scale.z = frame->scale[1];
			mesh->FrameTransforms[i].scale.y = frame->scale[2];
			mesh->FrameTransforms[i].translate.x = frame->translate[0];
			mesh->FrameTransforms[i].translate.z = frame->translate[1];
			mesh->FrameTransforms[i].translate.y = frame->translate[2];

			// add vertices
			for (SINT32 j = 0; j<header.numTriangles; ++j)
			{
				for (UINT32 ti = 0; ti<3; ++ti)
				{
					CAnimatedMeshMD2::SMD2Vert v;
					UINT32 num = triangles[j].vertexIndices[ti];
					v.Pos.x = frame->vertices[num].vertex[0];
					v.Pos.z = frame->vertices[num].vertex[1];
					v.Pos.y = frame->vertices[num].vertex[2];
					v.NormalIdx = frame->vertices[num].lightNormalIndex;

					mesh->FrameList[i].push_back(v);
				}
			}

			// calculate bounding boxes
			if (header.numVertices)
			{
				AxisAlignedBox box;
				Vector3 pos;
				pos.x = FLOAT32(mesh->FrameList[i][0].Pos.x) * mesh->FrameTransforms[i].scale.x + mesh->FrameTransforms[i].translate.x;
				pos.y = FLOAT32(mesh->FrameList[i][0].Pos.y) * mesh->FrameTransforms[i].scale.y + mesh->FrameTransforms[i].translate.y;
				pos.z = FLOAT32(mesh->FrameList[i][0].Pos.z) * mesh->FrameTransforms[i].scale.z + mesh->FrameTransforms[i].translate.z;

				box.reset(pos);

				for (SINT32 j = 1; j<header.numTriangles * 3; ++j)
				{
					pos.x = FLOAT32(mesh->FrameList[i][j].Pos.x) * mesh->FrameTransforms[i].scale.x + mesh->FrameTransforms[i].translate.x;
					pos.y = FLOAT32(mesh->FrameList[i][j].Pos.y) * mesh->FrameTransforms[i].scale.y + mesh->FrameTransforms[i].translate.y;
					pos.z = FLOAT32(mesh->FrameList[i][j].Pos.z) * mesh->FrameTransforms[i].scale.z + mesh->FrameTransforms[i].translate.z;

					box.addInternalPoint(pos);
				}
				mesh->BoxList.push_back(box);
			}
		}

		// populate interpolation buffer with texture coordinates and colors
		if (header.numFrames)
		{
			FLOAT32 dmaxs = 1.0f / (header.skinWidth);
			FLOAT32 dmaxt = 1.0f / (header.skinHeight);

			for (SINT32 t = 0; t<header.numTriangles; ++t)
			{
				for (SINT32 n = 0; n<3; ++n)
				{
					mesh->InterpolationBuffer->Vertices[t * 3 + n].TCoords.x = (textureCoords[triangles[t].textureIndices[n]].s + 0.5f) * dmaxs;
					mesh->InterpolationBuffer->Vertices[t * 3 + n].TCoords.y = (textureCoords[triangles[t].textureIndices[n]].t + 0.5f) * dmaxt;
					mesh->InterpolationBuffer->Vertices[t * 3 + n].Color = ColourValue(255, 255, 255, 255);
				}
			}
		}

		// clean up
		delete[] triangles;
		delete[] textureCoords;

		// init buffer with start frame.
		mesh->getMesh(0);
		return true;
	}
}



#include "SapphireCMD2MeshFileLoader.h"
#endif

