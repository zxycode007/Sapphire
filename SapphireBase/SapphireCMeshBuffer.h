#ifndef _SAPPHIRE_C_MESH_BUFFER_
#define _SAPPHIRE_C_MESH_BUFFER_
#include "SapphirePrerequisites.h"
#include "SapphireIMeshBuffer.h"
namespace Sapphire
{
	//! Template implementation of the IMeshBuffer interface
	template <class T>
	class CMeshBuffer : public IMeshBuffer
	{
	public:
		//! Default constructor for empty meshbuffer
		CMeshBuffer() :ChangedID_Vertex(1), ChangedID_Index(1), MappingHint_Vertex(EHM_NEVER), MappingHint_Index(EHM_NEVER)
		{
#ifdef _DEBUG
			setDebugName("SMeshBuffer");
#endif
		}


		//! Get material of this meshbuffer
		/** \return Material of this buffer */
		virtual const SMaterial& getMaterial() const
		{
			return Material;
		}


		//! Get material of this meshbuffer
		/** \return Material of this buffer */
		virtual SMaterial& getMaterial()
		{
			
			return Material;
		}


		//! Get pointer to vertices
		/** \return Pointer to vertices. */
		virtual const void* getVertices() const
		{
			return (void*)Vertices.begin()._Ptr;
		}


		//! Get pointer to vertices
		/** \return Pointer to vertices. */
		virtual void* getVertices()
		{
			return (void*)Vertices.begin()._Ptr;
		}


		//! Get number of vertices
		/** \return Number of vertices. */
		virtual UINT32 getVertexCount() const
		{
			return Vertices.size();
		}

		//! Get type of index data which is stored in this meshbuffer.
		/** \return Index type of this buffer. */
		virtual E_INDEX_TYPE getIndexType() const
		{
			return EIT_16BIT;
		}

		//! Get pointer to indices
		/** \return Pointer to indices. */
		virtual const UINT16* getIndices() const
		{
			return (UINT16*)Indices.begin()._Ptr;
		}


		//! Get pointer to indices
		/** \return Pointer to indices. */
		virtual UINT16* getIndices()
		{
			return (UINT16*)Indices.begin()._Ptr;;
		}


		//! Get number of indices
		/** \return Number of indices. */
		virtual UINT32 getIndexCount() const
		{
			return Indices.size();
		}


		//! Get the axis aligned bounding box
		/** \return Axis aligned bounding box of this buffer. */
		virtual const AxisAlignedBox& getBoundingBox() const
		{
			return BoundingBox;
		}


		//! Set the axis aligned bounding box
		/** \param box New axis aligned bounding box for this buffer. */
		//! set user axis aligned bounding box
		virtual void setBoundingBox(const AxisAlignedBox& box)
		{
			BoundingBox = box;
		}


		//! Recalculate the bounding box.
		/** should be called if the mesh changed. */
		virtual void recalculateBoundingBox()
		{
			if (Vertices.empty())
				BoundingBox.reset(Vector3(0, 0, 0));
			else
			{
				BoundingBox.reset(Vertices[0].Pos);
				for (UINT32 i = 1; i<Vertices.size(); ++i)
					BoundingBox.addInternalPoint(Vertices[i].Pos);
			}
		}


		//! Get type of vertex data stored in this buffer.
		/** \return Type of vertex data. */
		virtual E_VERTEX_TYPE getVertexType() const
		{
			return T().getType();
		}

		//! returns position of vertex i
		virtual const Vector3& getPosition(UINT32 i) const
		{
			return Vertices[i].Pos;
		}

		//! returns position of vertex i
		virtual Vector3& getPosition(UINT32 i)
		{
			return Vertices[i].Pos;
		}

		//! returns normal of vertex i
		virtual const Vector3& getNormal(UINT32 i) const
		{
			return Vertices[i].Normal;
		}

		//! returns normal of vertex i
		virtual Vector3& getNormal(UINT32 i)
		{
			return Vertices[i].Normal;
		}

		//! returns texture coord of vertex i
		virtual const Vector2& getTCoords(UINT32 i) const
		{
			return Vertices[i].TCoords;
		}

		//! returns texture coord of vertex i
		virtual Vector2& getTCoords(UINT32 i)
		{
			return Vertices[i].TCoords;
		}


		//! Append the vertices and indices to the current buffer
		/** Only works for compatible types, i.e. either the same type
		or the main buffer is of standard type. Otherwise, behavior is
		undefined.
		*/
		virtual void append(const void* const vertices, UINT32 numVertices, const UINT16* const indices, UINT32 numIndices)
		{
			if (vertices == getVertices())
				return;

			const UINT32 vertexCount = getVertexCount();
			UINT32 i;

			//Vertices.reallocate(vertexCount + numVertices);
			for (i = 0; i<numVertices; ++i)
			{
				Vertices.push_back(reinterpret_cast<const T*>(vertices)[i]);
				BoundingBox.addInternalPoint(reinterpret_cast<const T*>(vertices)[i].Pos);
			}

			//Indices.reallocate(getIndexCount() + numIndices);
			for (i = 0; i<numIndices; ++i)
			{
				Indices.push_back(indices[i] + vertexCount);
			}
		}


		//! Append the meshbuffer to the current buffer
		/** Only works for compatible types, i.e. either the same type
		or the main buffer is of standard type. Otherwise, behavior is
		undefined.
		\param other Meshbuffer to be appended to this one.
		*/
		virtual void append(const IMeshBuffer* const other)
		{
			/*
			if (this==other)
			return;

			const UINT32 vertexCount = getVertexCount();
			UINT32 i;

			Vertices.reallocate(vertexCount+other->getVertexCount());
			for (i=0; i<other->getVertexCount(); ++i)
			{
			Vertices.push_back(reinterpret_cast<const T*>(other->getVertices())[i]);
			}

			Indices.reallocate(getIndexCount()+other->getIndexCount());
			for (i=0; i<other->getIndexCount(); ++i)
			{
			Indices.push_back(other->getIndices()[i]+vertexCount);
			}
			BoundingBox.addInternalBox(other->getBoundingBox());
			*/
		}


		//! get the current hardware mapping hint
		virtual E_HARDWARE_MAPPING getHardwareMappingHint_Vertex() const
		{
			return MappingHint_Vertex;
		}

		//! get the current hardware mapping hint
		virtual E_HARDWARE_MAPPING getHardwareMappingHint_Index() const
		{
			return MappingHint_Index;
		}

		//! set the hardware mapping hint, for driver
		virtual void setHardwareMappingHint(E_HARDWARE_MAPPING NewMappingHint, E_BUFFER_TYPE Buffer = EBT_VERTEX_AND_INDEX)
		{
			if (Buffer == EBT_VERTEX_AND_INDEX || Buffer == EBT_VERTEX)
				MappingHint_Vertex = NewMappingHint;
			if (Buffer == EBT_VERTEX_AND_INDEX || Buffer == EBT_INDEX)
				MappingHint_Index = NewMappingHint;
		}


		//! flags the mesh as changed, reloads hardware buffers
		virtual void setDirty(E_BUFFER_TYPE Buffer = EBT_VERTEX_AND_INDEX)
		{
			if (Buffer == EBT_VERTEX_AND_INDEX || Buffer == EBT_VERTEX)
				++ChangedID_Vertex;
			if (Buffer == EBT_VERTEX_AND_INDEX || Buffer == EBT_INDEX)
				++ChangedID_Index;
		}

		//! Get the currently used ID for identification of changes.
		/** This shouldn't be used for anything outside the VideoDriver. */
		virtual UINT32 getChangedID_Vertex() const { return ChangedID_Vertex; }

		//! Get the currently used ID for identification of changes.
		/** This shouldn't be used for anything outside the VideoDriver. */
		virtual UINT32 getChangedID_Index() const { return ChangedID_Index; }

		UINT32 ChangedID_Vertex;
		UINT32 ChangedID_Index;

		//! hardware mapping hint
		E_HARDWARE_MAPPING MappingHint_Vertex;
		E_HARDWARE_MAPPING MappingHint_Index;

		//! Material for this meshbuffer.
		SMaterial Material;
		//! Vertices of this buffer
		//vector<T>::type Vertices;
		typedef typename vector<T>::type vectorT;
		vectorT Vertices;
		//vector<T>::type Vertices;
		//! Indices into the vertices of this buffer.
		vector<UINT16>::type Indices;
		//! Bounding box of this meshbuffer.
		AxisAlignedBox BoundingBox;
	};

	//! Standard meshbuffer
	typedef CMeshBuffer<S3DVertex> SMeshBuffer;
	//! Meshbuffer with two texture coords per vertex, e.g. for lightmaps
	typedef CMeshBuffer<S3DVertex2TCoords> SMeshBufferLightMap;
	//! Meshbuffer with vertices having tangents stored, e.g. for normal mapping
	typedef CMeshBuffer<S3DVertexTangents> SMeshBufferTangents;
}

#endif