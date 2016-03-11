#include "SapphireCompileConifg.h"
#ifdef _SAPPHIRE_COMPILE_WITH_IRR_MESH_LOADER_

#include "SapphireCMeshFileLoader.h"
#include "SapphireOS.h"
#include "SapphireIXMLReader.h"
#include "SapphireSAnimatedMesh.h"
#include "SapphireIReadFile.h"
#include "SapphireIAttributes.h"
#include "SapphireIMeshSceneNode.h"
#include "SapphireCDynamicMeshBuffer.h"
#include "SapphireSMeshBufferLightMap.h"

namespace Sapphire
{
	//! Constructor
	CMeshFileLoader::CMeshFileLoader(ISceneManager* smgr,
		IFileSystem* fs)
		: SceneManager(smgr), FileSystem(fs)
	{

#ifdef _DEBUG
		setDebugName("CMeshFileLoader");
#endif

	}


	//! Returns true if the file maybe is able to be loaded by this class.
	/** This decision should be based only on the file extension (e.g. ".cob") */
	bool CMeshFileLoader::isALoadableFileExtension(const path& filename) const
	{
		return hasFileExtension(filename, "xml", "irrmesh");
	}


	//! creates/loads an animated mesh from the file.
	//! \return Pointer to the created mesh. Returns 0 if loading failed.
	//! If you no longer need the mesh, you should call IAnimatedMesh::drop().
	//! See IReferenceCounted::drop() for more information.
	IAnimatedMesh* CMeshFileLoader::createMesh(IReadFile* file)
	{
		IXMLReader* reader = FileSystem->createXMLReader(file);
		if (!reader)
			return 0;

		// read until mesh section, skip other parts

		const String meshTagName = "mesh";
		IAnimatedMesh* mesh = 0;

		while (reader->read())
		{
			if (reader->getNodeType() == EXN_ELEMENT)
			{
				if (meshTagName == StringUtil::StringWToString(reader->getNodeName()))
				{
					mesh = readMesh(reader);
					break;
				}
				else
					skipSection(reader, true); // unknown section
			}
		}

		reader->drop();

		return mesh;
	}


	//! reads a mesh sections and creates a mesh from it
	IAnimatedMesh* CMeshFileLoader::readMesh(IXMLReader* reader)
	{
		SAnimatedMesh* animatedmesh = new SAnimatedMesh();
		SMesh* mesh = new SMesh();

		animatedmesh->addMesh(mesh);
		mesh->drop();

		String bbSectionName = "boundingBox";
		String bufferSectionName = "buffer";
		String meshSectionName = "mesh";

		if (!reader->isEmptyElement())
			while (reader->read())
			{
				if (reader->getNodeType() == EXN_ELEMENT)
				{
					const wchar_t* nodeName = reader->getNodeName();
					if (bbSectionName == StringUtil::StringWToString(nodeName))
					{
						// inside a bounding box, ignore it for now because
						// we are calculating this anyway ourselves later.
					}
					else
						if (bufferSectionName == StringUtil::StringWToString(nodeName))
						{
							// we've got a mesh buffer

							IMeshBuffer* buffer = readMeshBuffer(reader);
							if (buffer)
							{
								mesh->addMeshBuffer(buffer);
								buffer->drop();
							}
						}
						else
							skipSection(reader, true); // unknown section

				} // end if node type is element
				else
					if (reader->getNodeType() == EXN_ELEMENT_END)
					{
						if (meshSectionName == StringUtil::StringWToString(reader->getNodeName()))
						{
							// end of mesh section reached, cancel out
							break;
						}
					}
			} // end while reader->read();

		mesh->recalculateBoundingBox();
		animatedmesh->recalculateBoundingBox();

		return animatedmesh;
	}


	//! reads a mesh sections and creates a mesh buffer from it
	IMeshBuffer* CMeshFileLoader::readMeshBuffer(IXMLReader* reader)
	{
		CDynamicMeshBuffer* buffer = 0;

		String verticesSectionName = "vertices";
		String bbSectionName = "boundingBox";
		String materialSectionName = "material";
		String indicesSectionName = "indices";
		String bufferSectionName = "buffer";

		bool insideVertexSection = false;
		bool insideIndexSection = false;

		int vertexCount = 0;
		int indexCount = 0;

		SMaterial material;

		if (!reader->isEmptyElement())
			while (reader->read())
			{
				if (reader->getNodeType() == EXN_ELEMENT)
				{
					const wchar_t* nodeName = reader->getNodeName();
					if (bbSectionName == StringUtil::StringWToString(nodeName))
					{
						// inside a bounding box, ignore it for now because
						// we are calculating this anyway ourselves later.
					}
					else
						if (materialSectionName == StringUtil::StringWToString(nodeName))
						{
							//we've got a material

							IAttributes* attributes = FileSystem->createEmptyAttributes(SceneManager->getVideoDriver());
							attributes->read(reader, true, L"material");

							SceneManager->getVideoDriver()->fillMaterialStructureFromAttributes(material, attributes);
							attributes->drop();
						}
						else
							if (verticesSectionName == StringUtil::StringWToString(nodeName))
							{
								// vertices section

								const String vertexTypeName1 = "standard";
								const String vertexTypeName2 = "2tcoords";
								const String vertexTypeName3 = "tangents";

								const wchar_t* vertexType = reader->getAttributeValue(L"type");
								vertexCount = reader->getAttributeValueAsInt(L"vertexCount");

								insideVertexSection = true;

								E_INDEX_TYPE itype = (vertexCount > 65536) ? EIT_32BIT : EIT_16BIT;
								if (vertexTypeName1 == StringUtil::StringWToString(vertexType))
								{
									buffer = new CDynamicMeshBuffer(EVT_STANDARD, itype);

								}
								else
									if (vertexTypeName2 == StringUtil::StringWToString(vertexType))
									{
										buffer = new CDynamicMeshBuffer(EVT_2TCOORDS, itype);
									}
									else
										if (vertexTypeName3 == StringUtil::StringWToString(vertexType))
										{
											buffer = new CDynamicMeshBuffer(EVT_TANGENTS, itype);
										}
								buffer->getVertexBuffer().reallocate(vertexCount);
								buffer->Material = material;
							}
							else
								if (indicesSectionName == StringUtil::StringWToString(nodeName))
								{
									// indices section

									indexCount = reader->getAttributeValueAsInt(L"indexCount");
									insideIndexSection = true;
								}

				} // end if node type is element
				else
					if (reader->getNodeType() == EXN_TEXT)
					{
						// read vertex data
						if (insideVertexSection)
						{
							readMeshBuffer(reader, vertexCount, buffer);
							insideVertexSection = false;

						} // end reading vertex array
						else
							if (insideIndexSection)
							{
								readIndices(reader, indexCount, buffer->getIndexBuffer());
								insideIndexSection = false;
							}

					} // end if node type is text
					else
						if (reader->getNodeType() == EXN_ELEMENT_END)
						{
							if (bufferSectionName == StringUtil::StringWToString(reader->getNodeName()))
							{
								// end of buffer section reached, cancel out
								break;
							}
						}
			} // end while reader->read();

		if (buffer)
			buffer->recalculateBoundingBox();

		return buffer;
	}


	//! read indices
	void CMeshFileLoader::readIndices(IXMLReader* reader, int indexCount, IIndexBuffer& indices)
	{
		indices.reallocate(indexCount);

		String data = StringUtil::StringWToString(reader->getNodeData());
		const c8* p = &data[0];

		for (int i = 0; i<indexCount && *p; ++i)
		{
			findNextNoneWhiteSpace(&p);
			indices.push_back(readInt(&p));
		}
	}


	void CMeshFileLoader::readMeshBuffer(IXMLReader* reader, int vertexCount, CDynamicMeshBuffer* sbuffer)
	{
		String data = StringUtil::StringWToString(reader->getNodeData());
		const c8* p = &data[0];
		IVertexBuffer& Vertices = sbuffer->getVertexBuffer();
		E_VERTEX_TYPE vType = Vertices.getType();

		if (sbuffer)
		{
			for (int i = 0; i<vertexCount && *p; ++i)
			{
				switch (vType)
				{
				case EVT_STANDARD:
				{
					S3DVertex vtx;
					// position

					findNextNoneWhiteSpace(&p);
					vtx.Pos.x = readFloat(&p);
					findNextNoneWhiteSpace(&p);
					vtx.Pos.y = readFloat(&p);
					findNextNoneWhiteSpace(&p);
					vtx.Pos.z = readFloat(&p);

					// normal

					findNextNoneWhiteSpace(&p);
					vtx.Normal.x = readFloat(&p);
					findNextNoneWhiteSpace(&p);
					vtx.Normal.y = readFloat(&p);
					findNextNoneWhiteSpace(&p);
					vtx.Normal.z = readFloat(&p);

					// color

					UINT32 col;
					findNextNoneWhiteSpace(&p);
					sscanf(p, "%08x", &col);
					//vtx.Color.set(col);
					vtx.Color.setAsARGB(col);
					skipCurrentNoneWhiteSpace(&p);

					// tcoord1

					findNextNoneWhiteSpace(&p);
					vtx.TCoords.x = readFloat(&p);
					findNextNoneWhiteSpace(&p);
					vtx.TCoords.y = readFloat(&p);

					Vertices.push_back(vtx);
				}
				break;
				case EVT_2TCOORDS:
				{
					S3DVertex2TCoords vtx;
					// position

					findNextNoneWhiteSpace(&p);
					vtx.Pos.x = readFloat(&p);
					findNextNoneWhiteSpace(&p);
					vtx.Pos.y = readFloat(&p);
					findNextNoneWhiteSpace(&p);
					vtx.Pos.z = readFloat(&p);

					// normal

					findNextNoneWhiteSpace(&p);
					vtx.Normal.x = readFloat(&p);
					findNextNoneWhiteSpace(&p);
					vtx.Normal.y = readFloat(&p);
					findNextNoneWhiteSpace(&p);
					vtx.Normal.z = readFloat(&p);

					// color

					UINT32 col;
					findNextNoneWhiteSpace(&p);
					sscanf(p, "%08x", &col);
					//vtx.Color.set(col);
					vtx.Color.setAsARGB(col);
					skipCurrentNoneWhiteSpace(&p);

					// tcoord1

					findNextNoneWhiteSpace(&p);
					vtx.TCoords.x = readFloat(&p);
					findNextNoneWhiteSpace(&p);
					vtx.TCoords.y = readFloat(&p);

					// tcoord2

					findNextNoneWhiteSpace(&p);
					vtx.TCoords2.x = readFloat(&p);
					findNextNoneWhiteSpace(&p);
					vtx.TCoords2.y = readFloat(&p);

					Vertices.push_back(vtx);
				}
				break;

				case EVT_TANGENTS:
				{
					S3DVertexTangents vtx;
					// position

					findNextNoneWhiteSpace(&p);
					vtx.Pos.x = readFloat(&p);
					findNextNoneWhiteSpace(&p);
					vtx.Pos.y = readFloat(&p);
					findNextNoneWhiteSpace(&p);
					vtx.Pos.z = readFloat(&p);

					// normal

					findNextNoneWhiteSpace(&p);
					vtx.Normal.x = readFloat(&p);
					findNextNoneWhiteSpace(&p);
					vtx.Normal.y = readFloat(&p);
					findNextNoneWhiteSpace(&p);
					vtx.Normal.z = readFloat(&p);

					// color

					UINT32 col;
					findNextNoneWhiteSpace(&p);
					sscanf(p, "%08x", &col);
					//vtx.Color.set(col);
					vtx.Color.setAsARGB(col);
					skipCurrentNoneWhiteSpace(&p);

					// tcoord1

					findNextNoneWhiteSpace(&p);
					vtx.TCoords.x = readFloat(&p);
					findNextNoneWhiteSpace(&p);
					vtx.TCoords.y = readFloat(&p);

					// tangent

					findNextNoneWhiteSpace(&p);
					vtx.Tangent.x = readFloat(&p);
					findNextNoneWhiteSpace(&p);
					vtx.Tangent.y = readFloat(&p);
					findNextNoneWhiteSpace(&p);
					vtx.Tangent.z = readFloat(&p);

					// binormal

					findNextNoneWhiteSpace(&p);
					vtx.Binormal.x = readFloat(&p);
					findNextNoneWhiteSpace(&p);
					vtx.Binormal.y = readFloat(&p);
					findNextNoneWhiteSpace(&p);
					vtx.Binormal.z = readFloat(&p);

					Vertices.push_back(vtx);
				}
				break;
				};

			}
		}
	}


	//! skips an (unknown) section in the irrmesh document
	void CMeshFileLoader::skipSection(IXMLReader* reader, bool reportSkipping)
	{
#ifdef _DEBUG
		Printer::log("irrMesh skipping section", StringUtil::StringWToString(reader->getNodeName()).c_str());
#endif

		// skip if this element is empty anyway.
		if (reader->isEmptyElement())
			return;

		// read until we've reached the last element in this section
		UINT32 tagCounter = 1;

		while (tagCounter && reader->read())
		{
			if (reader->getNodeType() == EXN_ELEMENT &&
				!reader->isEmptyElement())
			{
#ifdef _DEBUG
				if (reportSkipping)
					Printer::log("irrMesh unknown element:", StringUtil::StringWToString(reader->getNodeName()));
#endif

				++tagCounter;
			}
			else
				if (reader->getNodeType() == EXN_ELEMENT_END)
					--tagCounter;
		}
	}


	//! parses a float from a char pointer and moves the pointer
	//! to the end of the parsed float
	inline FLOAT32 CMeshFileLoader::readFloat(const c8** p)
	{
		FLOAT32 ftmp;
		*p = fast_atof_move(*p, ftmp);
		return ftmp;
	}


	//! parses an int from a char pointer and moves the pointer to
	//! the end of the parsed float
	inline SINT32 CMeshFileLoader::readInt(const c8** p)
	{
		return (SINT32)readFloat(p);
	}


	//! places pointer to next begin of a token
	void CMeshFileLoader::skipCurrentNoneWhiteSpace(const c8** start)
	{
		const c8* p = *start;

		while (*p && !(*p == ' ' || *p == '\n' || *p == '\r' || *p == '\t'))
			++p;

		// TODO: skip comments <!-- -->

		*start = p;
	}

	//! places pointer to next begin of a token
	void CMeshFileLoader::findNextNoneWhiteSpace(const c8** start)
	{
		const c8* p = *start;

		while (*p && (*p == ' ' || *p == '\n' || *p == '\r' || *p == '\t'))
			++p;

		// TODO: skip comments <!-- -->

		*start = p;
	}


	//! reads floats from inside of xml element until end of xml element
	void CMeshFileLoader::readFloatsInsideElement(IXMLReader* reader, FLOAT32* floats, UINT32 count)
	{
		if (reader->isEmptyElement())
			return;

		while (reader->read())
		{
			// TODO: check for comments inside the element
			// and ignore them.

			if (reader->getNodeType() == EXN_TEXT)
			{
				// parse float data
				String data = StringUtil::StringWToString(reader->getNodeData());
				const c8* p = &data[0];

				for (UINT32 i = 0; i<count; ++i)
				{
					findNextNoneWhiteSpace(&p);
					if (*p)
						floats[i] = readFloat(&p);
					else
						floats[i] = 0.0f;
				}
			}
			else
				if (reader->getNodeType() == EXN_ELEMENT_END)
					break; // end parsing text
		}
	}
}


#endif