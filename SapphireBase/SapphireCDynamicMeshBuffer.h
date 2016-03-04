#ifndef _SAPPHIRE_C_DYNAMIC_MESH_BUFFER_
#define _SAPPHIRE_C_DYNAMIC_MESH_BUFFER_

#include "SapphireIDynamicMeshBuffer.h"

#include "SapphireCVertexBuffer.h"
#include "SapphireCIndexBuffer.h"

namespace Sapphire
{
	class CDynamicMeshBuffer : public IDynamicMeshBuffer
	{
	public:
		//! constructor
		CDynamicMeshBuffer(E_VERTEX_TYPE vertexType, E_INDEX_TYPE indexType)
		{
			VertexBuffer = new CVertexBuffer(vertexType);
			IndexBuffer = new CIndexBuffer(indexType);
		}

		//! destructor
		virtual ~CDynamicMeshBuffer()
		{
			if (VertexBuffer)
				VertexBuffer->drop();
			if (IndexBuffer)
				IndexBuffer->drop();
		}

		virtual IVertexBuffer& getVertexBuffer() const
		{
			return *VertexBuffer;
		}

		virtual IIndexBuffer& getIndexBuffer() const
		{
			return *IndexBuffer;
		}

		virtual void setVertexBuffer(IVertexBuffer *newVertexBuffer)
		{
			if (newVertexBuffer)
				newVertexBuffer->grab();
			if (VertexBuffer)
				VertexBuffer->drop();

			VertexBuffer = newVertexBuffer;
		}

		virtual void setIndexBuffer(IIndexBuffer *newIndexBuffer)
		{
			if (newIndexBuffer)
				newIndexBuffer->grab();
			if (IndexBuffer)
				IndexBuffer->drop();

			IndexBuffer = newIndexBuffer;
		}

		//! Get Material of this buffer.
		virtual const SMaterial& getMaterial() const
		{
			return Material;
		}

		//! Get Material of this buffer.
		virtual SMaterial& getMaterial()
		{
			return Material;
		}

		//! Get bounding box
		virtual const AxisAlignedBox& getBoundingBox() const
		{
			return BoundingBox;
		}

		//! Set bounding box
		virtual void setBoundingBox(const AxisAlignedBox& box)
		{
			BoundingBox = box;
		}

		//! Recalculate bounding box
		virtual void recalculateBoundingBox()
		{
			if (!getVertexBuffer().size())
				BoundingBox.reset(Vector3(0, 0, 0));
			else
			{
				BoundingBox.reset(getVertexBuffer()[0].Pos);
				for (UINT32 i = 1; i<getVertexBuffer().size(); ++i)
					BoundingBox.addInternalPoint(getVertexBuffer()[i].Pos);
			}
		}

		SMaterial Material;
		AxisAlignedBox BoundingBox;
	private:
		IVertexBuffer *VertexBuffer;
		IIndexBuffer *IndexBuffer;
	};
}

#endif