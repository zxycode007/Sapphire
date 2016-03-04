#ifndef _SAPPHIRE_OCTREE_
#define _SAPPHIRE_OCTREE_

#include "SapphirePrerequisites.h"
#include "SapphireSViewFrustum.h"
#include "SapphireVertex.h"
#include "SapphireAxisAlignedBox.h"
#include "SapphireCMeshBuffer.h"

/**
Flags for Octree
*/
//! use meshbuffer for drawing, enables VBO usage
#define OCTREE_USE_HARDWARE	false
//! use visibility information together with VBOs
#define OCTREE_USE_VISIBILITY true
//! use bounding box or frustum for calculate polys
#define OCTREE_BOX_BASED true
//! bypass full invisible/visible test
#define OCTREE_PARENTTEST

namespace Sapphire
{

	//! template octree.
	/** T must be a vertex type which has a member
	called .Pos, which is a vertex3df position. */
	template <class T>
	class Octree
	{
	public:

		

		struct SMeshChunk : public  CMeshBuffer<T>
		{
			SMeshChunk()
				: CMeshBuffer<T>(), MaterialId(0)
			{
				CMeshBuffer<T>::grab();
			}

			virtual ~SMeshChunk()
			{
				//removeAllHardwareBuffers
			}

			SINT32 MaterialId;
		};

		
		
		
 
		struct SIndexChunk
		{
			vector<UINT16>::type Indices;
			SINT32 MaterialId;
		};

		typedef typename Sapphire::vector<SMeshChunk>::type SMeshChunkArray;
		typedef typename Sapphire::vector<SMeshChunk>::iterator SMeshChunkIterator;
		typedef typename Sapphire::vector<SMeshChunk>::const_iterator SMeshChunkCIterator;
		typedef typename Sapphire::vector<SIndexChunk>::type SIndexChunkArray;
		typedef typename Sapphire::vector<SIndexChunk>::iterator SIndexChunkIterator;
		typedef typename Sapphire::vector<SIndexChunk>::const_iterator SIndexChunkCIterator;

		struct SIndexData
		{
			UINT16* Indices;
			SINT32 CurrentSize;
			SINT32 MaxSize;
		};

		
		//! Constructor
		Octree(const SMeshChunkArray& meshes, SINT32 minimalPolysPerNode = 128) :
			IndexData(0), IndexDataCount(meshes.size()), NodeCount(0)
		{
			IndexData = new SIndexData[IndexDataCount];
			// construct array of all indices
			 
			
			

			SIndexChunkArray indexChunks;
			indexChunks.reserve(meshes.size());
			for (UINT32 i = 0; i != meshes.size(); ++i)
			{
				IndexData[i].CurrentSize = 0;
				IndexData[i].MaxSize = meshes[i].Indices.size();
				IndexData[i].Indices = new UINT16[IndexData[i].MaxSize];

				indexChunks.push_back(SIndexChunk());
				//SIndexChunk& tic = indexChunks->getLast();
				SIndexChunk& tic = indexChunks.back();

				tic.MaterialId = meshes[i].MaterialId;
				tic.Indices = meshes[i].Indices;
			}

			// create tree
			Root = new OctreeNode(NodeCount, 0, meshes, indexChunks, minimalPolysPerNode);
		}

		//! returns all ids of polygons partially or fully enclosed
		//! by this bounding box.
		void calculatePolys(const AxisAlignedBox& box)
		{
			for (UINT32 i = 0; i != IndexDataCount; ++i)
				IndexData[i].CurrentSize = 0;

			Root->getPolys(box, IndexData, 0);
		}

		//! returns all ids of polygons partially or fully enclosed
		//! by a view frustum.
		void calculatePolys(const SViewFrustum& frustum)
		{
			for (UINT32 i = 0; i != IndexDataCount; ++i)
				IndexData[i].CurrentSize = 0;

			Root->getPolys(frustum, IndexData, 0);
		}

		const SIndexData* getIndexData() const
		{
			return IndexData;
		}

		UINT32 getIndexDataCount() const
		{
			return IndexDataCount;
		}

		UINT32 getNodeCount() const
		{
			return NodeCount;
		}

		//! for debug purposes only, collects the bounding boxes of the tree
		void getBoundingBoxes(const AxisAlignedBox& box,
			vector< const AxisAlignedBox* >::type&outBoxes) const
		{
			Root->getBoundingBoxes(box, outBoxes);
		}

		//! destructor
		~Octree()
		{
			for (UINT32 i = 0; i<IndexDataCount; ++i)
				delete[] IndexData[i].Indices;

			delete[] IndexData;
			delete Root;
		}

	private:
		// private inner class
		class OctreeNode
		{
		public:

			// constructor
			OctreeNode(UINT32& nodeCount, UINT32 currentdepth,
				const SMeshChunkArray& allmeshdata,
				SIndexChunkArray indices,
				SINT32 minimalPolysPerNode) : IndexData(0),
				Depth(currentdepth + 1)
			{
				++nodeCount;

				UINT32 i; // new ISO for scoping problem with different compilers

				for (i = 0; i != 8; ++i)
					Children[i] = 0;

				if (indices.empty())
				{
					//delete indices;
					return;
				}

				bool found = false;

				// find first point for bounding box

				for (SIndexChunkIterator it = indices.begin(); it<indices.end(); it++)
				{
					if (!it->Indices.empty())//if (!(*indices)[i].Indices.empty())
					{
						//Box.reset(allmeshdata[i].Vertices[(*indices)[i].Indices[0]].Pos);
						Box.reset(allmeshdata[i].Vertices[it->Indices[0]].Pos);
						found = true;
						break;
					}
				}

				if (!found)
				{
					//delete indices;
					return;
				}

				SINT32 totalPrimitives = 0;

				// now lets calculate our bounding box
				//for (i = 0; i<indices.size(); ++i)
				for (SIndexChunkIterator it = indices.begin(); it<indices.end(); it++)
				{
					//totalPrimitives += (*indices)[i].Indices.size();
					totalPrimitives += it->Indices.size();
					//for (UINT32 j = 0; j < (*indices)[i].Indices.size(); ++j)
					for (UINT32 j = 0; j < it->Indices.size(); ++j)
					{
						
						Box.addInternalPoint(allmeshdata[i].Vertices[it->Indices[j]].Pos);
					}
				}

				Vector3 middle = Box.getCenter();
				//取得盒子所有顶点
				const Vector3* edges = Box.getAllCorners();
				

				// calculate all children
				AxisAlignedBox box;
				vector<UINT16>::type keepIndices;

				if (totalPrimitives > minimalPolysPerNode && !Box.isNull())
					for (UINT32 ch = 0; ch != 8; ++ch)
					{
						box.reset(middle);
						box.addInternalPoint(edges[ch]);

						// create indices for child
						bool added = false;
						SIndexChunkArray cindexChunks;
						cindexChunks.reserve(allmeshdata.size());
						for (i = 0; i<allmeshdata.size(); ++i)
						{
							cindexChunks.push_back(SIndexChunk());
							SIndexChunk& tic = cindexChunks.back();
							tic.MaterialId = allmeshdata[i].MaterialId;

							//for (UINT32 t = 0; t<(*indices)[i].Indices.size(); t += 3)
							for (UINT32 t = 0; t<indices[i].Indices.size(); t += 3)
							{
								if (box.contains(allmeshdata[i].Vertices[indices[i].Indices[t]].Pos) &&//box.isPointInside(allmeshdata[i].Vertices[indices[i].Indices[t]].Pos) &&
									box.contains(allmeshdata[i].Vertices[indices[i].Indices[t + 1]].Pos) &&//box.isPointInside(allmeshdata[i].Vertices[indices[i].Indices[t + 1]].Pos) &&
									box.contains(allmeshdata[i].Vertices[indices[i].Indices[t + 2]].Pos))//box.isPointInside(allmeshdata[i].Vertices[indices[i].Indices[t + 2]].Pos))
								{
									tic.Indices.push_back(indices[i].Indices[t]);
									tic.Indices.push_back(indices[i].Indices[t + 1]);
									tic.Indices.push_back(indices[i].Indices[t + 2]);

									added = true;
								}
								else
								{
									keepIndices.push_back(indices[i].Indices[t]);
									keepIndices.push_back(indices[i].Indices[t + 1]);
									keepIndices.push_back(indices[i].Indices[t + 2]);
								}
							}

							indices[i].Indices.reserve(keepIndices.size());
							memcpy(indices[i].Indices.begin()._Ptr, keepIndices.begin()._Ptr, keepIndices.size()*sizeof(UINT16));
							//keepIndices.set_used(0);
							keepIndices.clear();
						}

						if (added)
							Children[ch] = new OctreeNode(nodeCount, Depth,
							allmeshdata, cindexChunks, minimalPolysPerNode);
						else
						{
							//delete cindexChunks;
						}

					} // end for all possible children

				IndexData = indices;
			}

			// destructor
			~OctreeNode()
			{
				//delete IndexData;

				for (UINT32 i = 0; i < 8; ++i)
				{
					//delete Children[i];
				}
					
			}

			// returns all ids of polygons partially or full enclosed
			// by this bounding box.
			void getPolys(const AxisAlignedBox& box, SIndexData* idxdata, UINT32 parentTest) const
			{
#if defined (OCTREE_PARENTTEST )
				// if not full inside
				if (parentTest != 2)
				{
					// partially inside ?
					if (!Box.intersects(box))
						return;

					// fully inside ?
					//parentTest = Box.isFullInside(box) ? 2 : 1;
					parentTest = box.contains(Box) ? 2 : 1;
				}
#else
				if (Box.intersects(box))
#endif
				{
					const UINT32 cnt = IndexData.size();
					UINT32 i; // new ISO for scoping problem in some compilers

					for (i = 0; i<cnt; ++i)
					{
						const SINT32 idxcnt = IndexData[i].Indices.size();

						if (idxcnt)
						{
							memcpy(&idxdata[i].Indices[idxdata[i].CurrentSize],
								&IndexData[i].Indices[0], idxcnt * sizeof(SINT16));
							idxdata[i].CurrentSize += idxcnt;
						}
					}

					for (i = 0; i != 8; ++i)
						if (Children[i])
							Children[i]->getPolys(box, idxdata, parentTest);
			}
		}

			// returns all ids of polygons partially or full enclosed
			// by the view frustum.
			void getPolys(const SViewFrustum& frustum, SIndexData* idxdata, UINT32 parentTest) const
			{
				UINT32 i; // new ISO for scoping problem in some compilers

				// if parent is fully inside, no further check for the children is needed
#if defined (OCTREE_PARENTTEST )
				if (parentTest != 2)
#endif
				{
#if defined (OCTREE_PARENTTEST )
					parentTest = 2;
#endif
					for (i = 0; i != SViewFrustum::VF_PLANE_COUNT; ++i)
					{
						EIntersectionRelation3D r = Box.classifyPlaneRelation(frustum.planes[i]);
						if (r == ISREL3D_FRONT)
							return;
#if defined (OCTREE_PARENTTEST )
						if (r == ISREL3D_CLIPPED)
							parentTest = 1;	// must still check children
#endif
					}
				}


				const UINT32 cnt = IndexData.size();

				for (i = 0; i != cnt; ++i)
				{
					SINT32 idxcnt = IndexData[i].Indices.size();

					if (idxcnt)
					{
						memcpy(&idxdata[i].Indices[idxdata[i].CurrentSize],
							&IndexData[i].Indices[0], idxcnt * sizeof(SINT16));
						idxdata[i].CurrentSize += idxcnt;
					}
				}

				for (i = 0; i != 8; ++i)
					if (Children[i])
						Children[i]->getPolys(frustum, idxdata, parentTest);
			}

			//! for debug purposes only, collects the bounding boxes of the node
			void getBoundingBoxes(const AxisAlignedBox& box,
				vector< const AxisAlignedBox* >::type& outBoxes) const
			{
				if (Box.intersects(box))
				{
					outBoxes.push_back(&Box);

					for (UINT32 i = 0; i != 8; ++i)
						if (Children[i])
							Children[i]->getBoundingBoxes(box, outBoxes);
				}
			}

		private:

			AxisAlignedBox Box;
			//array<SIndexChunk>* IndexData;
			SIndexChunkArray IndexData;
			OctreeNode* Children[8];
			UINT32 Depth;
	};

		OctreeNode* Root;
		SIndexData* IndexData;
		UINT32 IndexDataCount;
		UINT32 NodeCount;
};

}

#endif