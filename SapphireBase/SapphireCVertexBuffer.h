#ifndef _SAPPHIRE_C_VERTEX_BUFFER_
#define _SAPPHIRE_C_VERTEX_BUFFER_

#include "SapphirePrerequisites.h"
#include "SapphireIVertexBuffer.h"

namespace Sapphire
{
	class CVertexBuffer : public IVertexBuffer
	{
		class IVertexList
		{
		public:
			virtual ~IVertexList(){};

			virtual UINT32 stride() const = 0;

			virtual UINT32 size() const = 0;

			virtual void push_back(const S3DVertex &element) = 0;
			virtual S3DVertex& operator [](const UINT32 index) const = 0;
			virtual S3DVertex& getLast() = 0;
			virtual void set_used(UINT32 usedNow) = 0;
			virtual void reallocate(UINT32 new_size) = 0;
			virtual UINT32 allocated_size() const = 0;
			virtual S3DVertex* pointer() = 0;
			virtual E_VERTEX_TYPE getType() const = 0;
		};

		template <class T>
		class CSpecificVertexList : public IVertexList
		{
		public:
			typedef typename Sapphire::vector<T>::type array;
			array Vertices;

			virtual UINT32 stride() const { return sizeof(T); }

			virtual UINT32 size() const { return Vertices.size(); }

			virtual void push_back(const S3DVertex &element)
			{
				Vertices.push_back((T&)element);
			}

			virtual S3DVertex& operator [](const UINT32 index) const
			{
				return (S3DVertex&)Vertices[index];
			} 

			virtual S3DVertex& getLast()
			{
				return (S3DVertex&)Vertices.back();
			}

			virtual void set_used(UINT32 usedNow)
			{
				Vertices.reserve(usedNow);
			}

			virtual void reallocate(UINT32 new_size)
			{
				//Vertices.reallocate(new_size);
				Vertices.reserve(new_size);
			}

			virtual UINT32 allocated_size() const
			{
				//return Vertices.allocated_size();
				return Vertices.size();
			}

			virtual S3DVertex* pointer() { return Vertices.begin()._Ptr; }

			virtual E_VERTEX_TYPE getType() const { return T().getType(); }
		};

	public:
		IVertexList *Vertices;

		CVertexBuffer(E_VERTEX_TYPE vertexType) : Vertices(0),
			MappingHint(EHM_NEVER), ChangedID(1)
		{
			setType(vertexType);
		}

		CVertexBuffer(const IVertexBuffer &VertexBufferCopy) :
			Vertices(0), MappingHint(EHM_NEVER),
			ChangedID(1)
		{
			setType(VertexBufferCopy.getType());
			reallocate(VertexBufferCopy.size());

			for (UINT32 n = 0; n<VertexBufferCopy.size(); ++n)
				push_back(VertexBufferCopy[n]);
		}

		virtual ~CVertexBuffer()
		{
			delete Vertices;
		}


		virtual void setType(E_VERTEX_TYPE vertexType)
		{
			IVertexList *NewVertices = 0;

			switch (vertexType)
			{
			case EVT_STANDARD:
			{
				NewVertices = new CSpecificVertexList<S3DVertex>;
				break;
			}
			case EVT_2TCOORDS:
			{
				NewVertices = new CSpecificVertexList<S3DVertex2TCoords>;
				break;
			}
			case EVT_TANGENTS:
			{
				NewVertices = new CSpecificVertexList<S3DVertexTangents>;
				break;
			}
			}
			if (Vertices)
			{
				NewVertices->reallocate(Vertices->size());

				for (UINT32 n = 0; n<Vertices->size(); ++n)
					NewVertices->push_back((*Vertices)[n]);

				delete Vertices;
			}

			Vertices = NewVertices;
		}

		virtual void* getData() { return Vertices->pointer(); }

		virtual E_VERTEX_TYPE getType() const { return Vertices->getType(); }

		virtual UINT32 stride() const { return Vertices->stride(); }

		virtual UINT32 size() const
		{
			return Vertices->size();
		}

		virtual void push_back(const S3DVertex &element)
		{
			Vertices->push_back(element);
		}

		virtual S3DVertex& operator [](const UINT32 index) const
		{
			return (*Vertices)[index];
		}

		virtual S3DVertex& getLast()
		{
			return Vertices->getLast();
		}

		virtual void set_used(UINT32 usedNow)
		{
			Vertices->set_used(usedNow);
		}

		virtual void reallocate(UINT32 new_size)
		{
			Vertices->reallocate(new_size);
		}

		virtual UINT32 allocated_size() const
		{
			return Vertices->allocated_size();
		}

		virtual S3DVertex* pointer()
		{
			return Vertices->pointer();
		}

		//! get the current hardware mapping hint
		virtual E_HARDWARE_MAPPING getHardwareMappingHint() const
		{
			return MappingHint;
		}

		//! set the hardware mapping hint, for driver
		virtual void setHardwareMappingHint(E_HARDWARE_MAPPING NewMappingHint)
		{
			MappingHint = NewMappingHint;
		}

		//! flags the mesh as changed, reloads hardware buffers
		virtual void setDirty()
		{
			++ChangedID;
		}

		//! Get the currently used ID for identification of changes.
		/** This shouldn't be used for anything outside the VideoDriver. */
		virtual UINT32 getChangedID() const { return ChangedID; }

		E_HARDWARE_MAPPING MappingHint;
		UINT32 ChangedID;
	};
}

#endif