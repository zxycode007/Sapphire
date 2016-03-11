#include "SapphireCompileConifg.h"
#ifndef _SAPPHIRE_COMPILE_WITH_IRR_WRITER_

#include "SapphireCMeshFileWriter.h"
#include "SapphireOS.h"
#include "SapphireIWriteFile.h"
#include "SapphireIXMLWriter.h"
#include "SapphireIMesh.h"
#include "SapphireIAttributes.h"

namespace Sapphire
{

	CMeshWriter::CMeshWriter(IVideoDriver* driver,
		IFileSystem* fs)
		: FileSystem(fs), VideoDriver(driver), Writer(0)
	{
#ifdef _DEBUG
		setDebugName("CMeshWriter");
#endif

		if (VideoDriver)
			VideoDriver->grab();

		if (FileSystem)
			FileSystem->grab();
	}


	CMeshWriter::~CMeshWriter()
	{
		if (VideoDriver)
			VideoDriver->drop();

		if (FileSystem)
			FileSystem->drop();
	}


	//! Returns the type of the mesh writer
	EMESH_WRITER_TYPE CMeshWriter::getType() const
	{
		return EMWT_IRR_MESH;
	}


	//! writes a mesh
	bool CMeshWriter::writeMesh(IWriteFile* file, IMesh* mesh, SINT32 flags)
	{
		if (!file)
			return false;

		Writer = FileSystem->createXMLWriter(file);

		if (!Writer)
		{
			Printer::log("Could not write file", file->getFileName());
			return false;
		}

		Printer::log("Writing mesh", file->getFileName());

		// write IRR MESH header

		Writer->writeXMLHeader();

		Writer->writeElement(L"mesh", false,
			L"xmlns", L"http://irrlicht.sourceforge.net/IRRMESH_09_2007",
			L"version", L"1.0");
		Writer->writeLineBreak();

		// add some informational comment. Add a space after and before the comment
		// tags so that some braindead xml parsers (AS anyone?) are able to parse this too.

		StringW infoComment = L" This file contains a static mesh in the Irrlicht Engine format with ";
		//infoComment += StringW(mesh->getMeshBufferCount());
		infoComment += StringUtil::int32ToStringW(mesh->getMeshBufferCount());
		infoComment += L" materials.";

		Writer->writeComment(infoComment.c_str());
		Writer->writeLineBreak();

		// write mesh bounding box

		writeBoundingBox(mesh->getBoundingBox());
		Writer->writeLineBreak();

		// write mesh buffers

		for (int i = 0; i<(int)mesh->getMeshBufferCount(); ++i)
		{
			IMeshBuffer* buffer = mesh->getMeshBuffer(i);
			if (buffer)
			{
				writeMeshBuffer(buffer);
				Writer->writeLineBreak();
			}
		}

		Writer->writeClosingTag(L"mesh");

		Writer->drop();
		return true;
	}


	void CMeshWriter::writeBoundingBox(const AxisAlignedBox& box)
	{
		Writer->writeElement(L"boundingBox", true,
			L"minEdge", getVectorAsStringLine(box.getMinimum()).c_str(),
			L"maxEdge", getVectorAsStringLine(box.getMaximum()).c_str());
	}


	StringW CMeshWriter::getVectorAsStringLine(const Vector3& v) const
	{
		StringW str;

		//str = StringW(v.x);
		str = StringUtil::int32ToStringW(v.x);
		str += L" ";
		//str += StringW(v.y);
		str += StringUtil::int32ToStringW(v.y);
		str += L" ";
		//str += StringW(v.z);
		str += StringUtil::int32ToStringW(v.z);
		return str;
	}


	StringW CMeshWriter::getVectorAsStringLine(const Vector2& v) const
	{
		StringW str;

		//str = StringW(v.x);
		str = StringUtil::int32ToStringW(v.x);
		str += L" ";
		//str += StringW(v.y);
		str += StringUtil::int32ToStringW(v.y);

		return str;
	}


	void CMeshWriter::writeMeshBuffer(const IMeshBuffer* buffer)
	{
		Writer->writeElement(L"buffer", false);
		Writer->writeLineBreak();

		// write bounding box

		writeBoundingBox(buffer->getBoundingBox());
		Writer->writeLineBreak();

		// write material

		writeMaterial(buffer->getMaterial());

		// write vertices

		const StringW vertexTypeStr = StringUtil::StringToStringW(sBuiltInVertexTypeNames[buffer->getVertexType()]);

		Writer->writeElement(L"vertices", false,
			L"type", vertexTypeStr.c_str(),
			L"vertexCount", StringUtil::int32ToStringW(buffer->getVertexCount()).c_str());

		Writer->writeLineBreak();

		UINT32 vertexCount = buffer->getVertexCount();

		switch (buffer->getVertexType())
		{
		caseEVT_STANDARD:
		{
			S3DVertex* vtx = (S3DVertex*)buffer->getVertices();
			for (UINT32 j = 0; j<vertexCount; ++j)
			{
				StringW str = getVectorAsStringLine(vtx[j].Pos);
				str += L" ";
				str += getVectorAsStringLine(vtx[j].Normal);

				char tmp[12];
				sprintf(tmp, " %02x%02x%02x%02x ", vtx[j].Color.getAlpha(), vtx[j].Color.getRed(), vtx[j].Color.getGreen(), vtx[j].Color.getBlue());
				//str += tmp;
				str += StringUtil::StringToStringW(tmp);

				str += getVectorAsStringLine(vtx[j].TCoords);

				Writer->writeText(str.c_str());
				Writer->writeLineBreak();
			}
		}
		break;
		caseEVT_2TCOORDS:
		{
			S3DVertex2TCoords* vtx = (S3DVertex2TCoords*)buffer->getVertices();
			for (UINT32 j = 0; j<vertexCount; ++j)
			{
				StringW str = getVectorAsStringLine(vtx[j].Pos);
				str += L" ";
				str += getVectorAsStringLine(vtx[j].Normal);

				char tmp[12];
				sprintf(tmp, " %02x%02x%02x%02x ", vtx[j].Color.getAlpha(), vtx[j].Color.getRed(), vtx[j].Color.getGreen(), vtx[j].Color.getBlue());
				//str += tmp;
				str += StringUtil::StringToStringW(tmp);

				str += getVectorAsStringLine(vtx[j].TCoords);
				str += L" ";
				str += getVectorAsStringLine(vtx[j].TCoords2);

				Writer->writeText(str.c_str());
				Writer->writeLineBreak();
			}
		}
		break;
		caseEVT_TANGENTS:
		{
			S3DVertexTangents* vtx = (S3DVertexTangents*)buffer->getVertices();
			for (UINT32 j = 0; j<vertexCount; ++j)
			{
				StringW str = getVectorAsStringLine(vtx[j].Pos);
				str += L" ";
				str += getVectorAsStringLine(vtx[j].Normal);

				char tmp[12];
				sprintf(tmp, " %02x%02x%02x%02x ", vtx[j].Color.getAlpha(), vtx[j].Color.getRed(), vtx[j].Color.getGreen(), vtx[j].Color.getBlue());
				//str += tmp;
				str += StringUtil::StringToStringW(tmp);

				str += getVectorAsStringLine(vtx[j].TCoords);
				str += L" ";
				str += getVectorAsStringLine(vtx[j].Tangent);
				str += L" ";
				str += getVectorAsStringLine(vtx[j].Binormal);

				Writer->writeText(str.c_str());
				Writer->writeLineBreak();
			}
		}
		break;
		}

		Writer->writeClosingTag(L"vertices");
		Writer->writeLineBreak();

		// write indices

		Writer->writeElement(L"indices", false,
			L"indexCount", StringUtil::int32ToStringW(buffer->getIndexCount()).c_str());//L"indexCount", StringW(buffer->getIndexCount()).c_str());

		Writer->writeLineBreak();

		int indexCount = (int)buffer->getIndexCount();

		E_INDEX_TYPE iType = buffer->getIndexType();

		const UINT16* idx16 = buffer->getIndices();
		const UINT32* idx32 = (UINT32*)buffer->getIndices();
		const int maxIndicesPerLine = 25;

		for (int i = 0; i<indexCount; ++i)
		{
			if (iType ==EIT_16BIT)
			{
				//StringW str((int)idx16[i]);
				StringW str = StringUtil::int32ToStringW((int)idx16[i]);
				Writer->writeText(str.c_str());
			}
			else
			{
				//StringW str((int)idx32[i]);
				StringW str = StringUtil::int32ToStringW((int)idx32[i]);
				Writer->writeText(str.c_str());
			}

			if (i % maxIndicesPerLine != maxIndicesPerLine)
			{
				if (i % maxIndicesPerLine == maxIndicesPerLine - 1)
					Writer->writeLineBreak();
				else
					Writer->writeText(L" ");
			}
		}

		if ((indexCount - 1) % maxIndicesPerLine != maxIndicesPerLine - 1)
			Writer->writeLineBreak();

		Writer->writeClosingTag(L"indices");
		Writer->writeLineBreak();

		// close buffer tag

		Writer->writeClosingTag(L"buffer");
	}


	void CMeshWriter::writeMaterial(const SMaterial& material)
	{
		// simply use irrlichts built-in attribute serialization capabilities here:

		IAttributes* attributes =
			VideoDriver->createAttributesFromMaterial(material);

		if (attributes)
		{
			attributes->write(Writer, false, L"material");
			attributes->drop();
		}
	}
}

#endif