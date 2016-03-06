#ifndef _SAPPHIRE_C_SCENE_LOADER_
#define _SAPPHIRE_C_SCENE_LOADER_

#include "SapphireISceneLoader.h"
#include "SapphireIFileSystem.h"
#include "SapphireIXMLReader.h"

namespace Sapphire
{
	class ISceneManager;

	//! Class which can load a scene into the scene manager.
	class CSceneLoaderIrr : public virtual ISceneLoader
	{
	public:

		//! Constructor
		CSceneLoaderIrr(ISceneManager *smgr, IFileSystem* fs);

		//! Destructor
		virtual ~CSceneLoaderIrr();

		//! Returns true if the class might be able to load this file.
		virtual bool isALoadableFileExtension(const path& filename) const;

		//! Returns true if the class might be able to load this file.
		virtual bool isALoadableFileFormat(IReadFile *file) const;

		//! Loads the scene into the scene manager.
		virtual bool loadScene(IReadFile* file, ISceneUserDataSerializer* userDataSerializer = 0,
			ISceneNode* rootNode = 0);

	private:

		//! Recursively reads nodes from the xml file
		void readSceneNode(IXMLReader* reader, ISceneNode* parent,
			ISceneUserDataSerializer* userDataSerializer);

		//! read a node's materials
		void readMaterials(IXMLReader* reader, ISceneNode* node);

		//! read a node's animators
		void readAnimators(IXMLReader* reader, ISceneNode* node);

		//! read any other data into the user serializer
		void readUserData(IXMLReader* reader, ISceneNode* node,
			ISceneUserDataSerializer* userDataSerializer);

		ISceneManager   *SceneManager;
		IFileSystem *FileSystem;

		//! constants for reading and writing XML.
		//! Not made static due to portability problems.
		// TODO: move to own header
		const StringW SAPPHIRE_XML_FORMAT_SCENE;
		const StringW SAPPHIRE_XML_FORMAT_NODE;
		const StringW SAPPHIRE_XML_FORMAT_NODE_ATTR_TYPE;
		const StringW SAPPHIRE_XML_FORMAT_ATTRIBUTES;
		const StringW SAPPHIRE_XML_FORMAT_MATERIALS;
		const StringW SAPPHIRE_XML_FORMAT_ANIMATORS;
		const StringW SAPPHIRE_XML_FORMAT_USERDATA;
	};

}

#endif