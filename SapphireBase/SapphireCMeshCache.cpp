#include "SapphireCMeshCache.h"
#include "SapphireIAnimatedMesh.h"
#include "SapphireIMesh.h"


namespace Sapphire
{

	
	static const SNamedPath emptyNamedPath;


	CMeshCache::~CMeshCache()
	{
		clear();
	}


	//! adds a mesh to the list
	void CMeshCache::addMesh(const path& filename, IAnimatedMesh* mesh)
	{
		mesh->grab();

		MeshEntry e(filename);
		e.Mesh = mesh;

		Meshes.push_back(e);
	}


	//! Removes a mesh from the cache.
	void CMeshCache::removeMesh(const IMesh* const mesh)
	{
		if (!mesh)
			return;
		for (UINT32 i = 0; i<Meshes.size(); ++i)
		{
			if (Meshes[i].Mesh == mesh || (Meshes[i].Mesh && Meshes[i].Mesh->getMesh(0) == mesh))
			{
				Meshes[i].Mesh->drop();
				Meshes.erase(Meshes.begin()+i);
				return;
			}
		}
	}


	//! Returns amount of loaded meshes
	UINT32 CMeshCache::getMeshCount() const
	{
		return Meshes.size();
	}


	//! Returns current number of the mesh
	SINT32 CMeshCache::getMeshIndex(const IMesh* const mesh) const
	{
		for (UINT32 i = 0; i<Meshes.size(); ++i)
		{
			if (Meshes[i].Mesh == mesh || (Meshes[i].Mesh && Meshes[i].Mesh->getMesh(0) == mesh))
				return (SINT32)i;
		}

		return -1;
	}


	//! Returns a mesh based on its index number
	IAnimatedMesh* CMeshCache::getMeshByIndex(UINT32 number)
	{
		if (number >= Meshes.size())
			return 0;

		return Meshes[number].Mesh;
	}


	//! Returns a mesh based on its name.
	IAnimatedMesh* CMeshCache::getMeshByName(const path& name)
	{
		MeshEntry e(name);
		//SINT32 id = Meshes.binary_search(e);
		SINT32 id = std::distance(Meshes.begin(), std::find(Meshes.begin(), Meshes.end(), e));
		return (id != -1) ? Meshes[id].Mesh : 0;
	}


	//! Get the name of a loaded mesh, based on its index.
	const SNamedPath& CMeshCache::getMeshName(UINT32 index) const
	{
		if (index >= Meshes.size())
			return emptyNamedPath;

		return Meshes[index].NamedPath;
	}


	//! Get the name of a loaded mesh, if there is any.
	const SNamedPath& CMeshCache::getMeshName(const IMesh* const mesh) const
	{
		if (!mesh)
			return emptyNamedPath;

		for (UINT32 i = 0; i<Meshes.size(); ++i)
		{
			if (Meshes[i].Mesh == mesh || (Meshes[i].Mesh && Meshes[i].Mesh->getMesh(0) == mesh))
				return Meshes[i].NamedPath;
		}

		return emptyNamedPath;
	}

	//! Renames a loaded mesh.
	bool CMeshCache::renameMesh(UINT32 index, const path& name)
	{
		if (index >= Meshes.size())
			return false;

		Meshes[index].NamedPath.setPath(name);
		//Meshes.sort();
		std::stable_sort(Meshes.begin(), Meshes.end());
		return true;
	}


	//! Renames a loaded mesh.
	bool CMeshCache::renameMesh(const IMesh* const mesh, const path& name)
	{
		for (UINT32 i = 0; i<Meshes.size(); ++i)
		{
			if (Meshes[i].Mesh == mesh || (Meshes[i].Mesh && Meshes[i].Mesh->getMesh(0) == mesh))
			{
				Meshes[i].NamedPath.setPath(name);
				//Meshes.sort();
				std::stable_sort(Meshes.begin(), Meshes.end());
				return true;
			}
		}

		return false;
	}


	//! returns if a mesh already was loaded
	bool CMeshCache::isMeshLoaded(const path& name)
	{
		return getMeshByName(name) != 0;
	}


	//! Clears the whole mesh cache, removing all meshes.
	void CMeshCache::clear()
	{
		for (UINT32 i = 0; i<Meshes.size(); ++i)
			Meshes[i].Mesh->drop();

		Meshes.clear();
	}

	//! Clears all meshes that are held in the mesh cache but not used anywhere else.
	void CMeshCache::clearUnusedMeshes()
	{
		for (UINT32 i = 0; i<Meshes.size(); ++i)
		{
			if (Meshes[i].Mesh->getReferenceCount() == 1)
			{
				Meshes[i].Mesh->drop();
				Meshes.erase(Meshes.begin()+i);
				--i;
			}
		}
	}
}