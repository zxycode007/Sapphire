#ifndef _SAPPHIRE_C_INDEX_BUFFER_
#define _SAPPHIRE_C_INDEX_BUFFER_

#include "SapphireIIndexBuffer.h"

namespace Sapphire
{
	class CIndexBuffer : public IIndexBuffer
	{

		class IIndexList
		{
		public:
			virtual ~IIndexList(){};

			virtual UINT32 stride() const = 0;
			virtual UINT32 size() const = 0;
			virtual void push_back(const UINT32 &element) = 0;
			virtual UINT32 operator [](UINT32 index) const = 0;
			virtual UINT32 getLast() = 0;
			virtual void setValue(UINT32 index, UINT32 value) = 0;
			virtual void set_used(UINT32 usedNow) = 0;
			virtual void reallocate(UINT32 new_size) = 0;
			virtual UINT32 allocated_size() const = 0;
			virtual void* pointer() = 0;
			virtual E_INDEX_TYPE getType() const = 0;
		};

		template <class T>
		class CSpecificIndexList : public IIndexList
		{
		public:
			typedef typename Sapphire::vector<T>::type array;
			array Indices;

			virtual UINT32 stride() const { return sizeof(T); }

			virtual UINT32 size() const { return Indices.size(); }

			virtual void push_back(const UINT32 &element)
			{
				// push const ref due to compiler problem with gcc 4.6, big endian
				Indices.push_back((const T&)element);
			}

			virtual UINT32 operator [](UINT32 index) const
			{
				return (UINT32)(Indices[index]);
			}

			virtual UINT32 getLast() { return (UINT32)Indices.back(); }

			virtual void setValue(UINT32 index, UINT32 value)
			{
				Indices[index] = (T)value;
			}

			virtual void set_used(UINT32 usedNow)
			{
				//Indices.set_used(usedNow);
				Indices.reserve(usedNow);
			}

			virtual void reallocate(UINT32 new_size)
			{
				//Indices.reallocate(new_size);
				Indices.reserve(new_size);
			}

			virtual UINT32 allocated_size() const
			{
				//return Indices.allocated_size();
				return Indices.size();
			}

			virtual void* pointer() { return Indices.begin()._Ptr; }

			virtual E_INDEX_TYPE getType() const
			{
				if (sizeof(T) == sizeof(UINT16))
					return EIT_16BIT;
				else
					return EIT_32BIT;
			}
		};

	public:
		IIndexList *Indices;

		CIndexBuffer(E_INDEX_TYPE IndexType) :Indices(0), MappingHint(EHM_NEVER), ChangedID(1)
		{
			setType(IndexType);
		}

		CIndexBuffer(const IIndexBuffer &IndexBufferCopy) :Indices(0), MappingHint(EHM_NEVER), ChangedID(1)
		{
			setType(IndexBufferCopy.getType());
			reallocate(IndexBufferCopy.size());

			for (UINT32 n = 0; n<IndexBufferCopy.size(); ++n)
				push_back(IndexBufferCopy[n]);
		}

		virtual ~CIndexBuffer()
		{
			delete Indices;
		}

		//virtual void setType(E_INDEX_TYPE IndexType);
		virtual void setType(E_INDEX_TYPE IndexType)
		{
			IIndexList *NewIndices = 0;

			switch (IndexType)
			{
			case EIT_16BIT:
			{
				NewIndices = new CSpecificIndexList<UINT16>;
				break;
			}
			case EIT_32BIT:
			{
				NewIndices = new CSpecificIndexList<UINT32>;
				break;
			}
			}

			if (Indices)
			{
				NewIndices->reallocate(Indices->size());

				for (UINT32 n = 0; n<Indices->size(); ++n)
					NewIndices->push_back((*Indices)[n]);

				delete Indices;
			}

			Indices = NewIndices;
		}

		virtual void* getData() { return Indices->pointer(); }

		virtual E_INDEX_TYPE getType() const { return Indices->getType(); }

		virtual UINT32 stride() const { return Indices->stride(); }

		virtual UINT32 size() const
		{
			return Indices->size();
		}

		virtual void push_back(const UINT32 &element)
		{
			Indices->push_back(element);
		}

		virtual UINT32 operator [](UINT32 index) const
		{
			return (*Indices)[index];
		}

		virtual UINT32 getLast()
		{
			return Indices->getLast();
		}

		virtual void setValue(UINT32 index, UINT32 value)
		{
			Indices->setValue(index, value);
		}

		virtual void set_used(UINT32 usedNow)
		{
			Indices->set_used(usedNow);
		}

		virtual void reallocate(UINT32 new_size)
		{
			Indices->reallocate(new_size);
		}

		virtual UINT32 allocated_size() const
		{
			return Indices->allocated_size();
		}

		virtual void* pointer()
		{
			return Indices->pointer();
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