#include "SapphireCSceneLoader.h"
#include "SapphireISceneNodeAnimatorFactory.h"
#include "SapphireIScenesManager.h"
#include "SapphireIVideoDriver.h"
#include "SapphireIFileSystem.h"
#include "SapphireOS.h"
#include "SapphireSceneParameters.h"


namespace Sapphire
{
	//! Constructor
	CSceneLoaderIrr::CSceneLoaderIrr(ISceneManager *smgr, IFileSystem* fs)
		: SceneManager(smgr), FileSystem(fs),
		SAPPHIRE_XML_FORMAT_SCENE(L"irr_scene"), SAPPHIRE_XML_FORMAT_NODE(L"node"), SAPPHIRE_XML_FORMAT_NODE_ATTR_TYPE(L"type"),
		SAPPHIRE_XML_FORMAT_ATTRIBUTES(L"attributes"), SAPPHIRE_XML_FORMAT_MATERIALS(L"materials"),
		SAPPHIRE_XML_FORMAT_ANIMATORS(L"animators"), SAPPHIRE_XML_FORMAT_USERDATA(L"userData")
	{

	}

	//! Destructor
	CSceneLoaderIrr::~CSceneLoaderIrr()
	{

	}

	//! Returns true if the class might be able to load this file.
	bool CSceneLoaderIrr::isALoadableFileExtension(const path& filename) const
	{
		return hasFileExtension(filename, "irr");
	}

	//! Returns true if the class might be able to load this file.
	bool CSceneLoaderIrr::isALoadableFileFormat(IReadFile *file) const
	{
		// todo: check inside the file
		return true;
	}

	//! Loads the scene into the scene manager.
	bool CSceneLoaderIrr::loadScene(IReadFile* file, ISceneUserDataSerializer* userDataSerializer,
		ISceneNode* rootNode)
	{
		if (!file)
		{
			Printer::log("Unable to open scene file", LML_ERROR);
			return false;
		}

		IXMLReader* reader = FileSystem->createXMLReader(file);
		if (!reader)
		{
			Printer::log("Scene is not a valid XML file", file->getFileName().c_str(), LML_ERROR);
			return false;
		}

		// TODO: COLLADA_CREATE_SCENE_INSTANCES can be removed when the COLLADA loader is a scene loader
		bool oldColladaSingleMesh = SceneManager->getParameters()->getAttributeAsBool(COLLADA_CREATE_SCENE_INSTANCES);
		SceneManager->getParameters()->setAttribute(COLLADA_CREATE_SCENE_INSTANCES, false);

		// read file
		while (reader->read())
		{
			readSceneNode(reader, rootNode, userDataSerializer);
		}

		// restore old collada parameters
		SceneManager->getParameters()->setAttribute(COLLADA_CREATE_SCENE_INSTANCES, oldColladaSingleMesh);

		// clean up
		reader->drop();
		return true;
	}


	//! Reads the next node
	void CSceneLoaderIrr::readSceneNode(IXMLReader* reader, ISceneNode* parent,
		ISceneUserDataSerializer* userDataSerializer)
	{
		if (!reader)
			return;

		ISceneNode* node = 0;

		if (!parent && SAPPHIRE_XML_FORMAT_SCENE == reader->getNodeName())
			node = SceneManager->getRootSceneNode();
		else if (parent && SAPPHIRE_XML_FORMAT_NODE == reader->getNodeName())
		{
			// find node type and create it
			String attrName = StringUtil::StringWToString(reader->getAttributeValue(SAPPHIRE_XML_FORMAT_NODE_ATTR_TYPE.c_str()));

			node = SceneManager->addSceneNode(attrName.c_str(), parent);

			if (!node)
				Printer::log("Could not create scene node of unknown type", attrName.c_str());
		}
		else
			node = parent;

		// read attributes
		while (reader->read())
		{
			bool endreached = false;

			const wchar_t* name = reader->getNodeName();

			switch (reader->getNodeType())
			{
			case EXN_ELEMENT_END:
				if ((SAPPHIRE_XML_FORMAT_NODE == name) ||
					(SAPPHIRE_XML_FORMAT_SCENE == name))
				{
					endreached = true;
				}
				break;
			case EXN_ELEMENT:
				if (SAPPHIRE_XML_FORMAT_ATTRIBUTES == name)
				{
					// read attributes
					IAttributes* attr = FileSystem->createEmptyAttributes(SceneManager->getVideoDriver());
					attr->read(reader, true);

					if (node)
						node->deserializeAttributes(attr);

					attr->drop();
				}
				else
					if (SAPPHIRE_XML_FORMAT_MATERIALS == name)
						readMaterials(reader, node);
					else
						if (SAPPHIRE_XML_FORMAT_ANIMATORS == name)
							readAnimators(reader, node);
						else
							if (SAPPHIRE_XML_FORMAT_USERDATA == name)
								readUserData(reader, node, userDataSerializer);
							else
								if ((SAPPHIRE_XML_FORMAT_NODE == name) ||
									(SAPPHIRE_XML_FORMAT_SCENE == name))
								{
									readSceneNode(reader, node, userDataSerializer);
								}
								else
								{
									Printer::log("Found unknown element in irrlicht scene file",
										StringUtil::StringWToString(name).c_str());
								}
				break;
			default:
				break;
			}

			if (endreached)
				break;
		}
		if (node && userDataSerializer)
			userDataSerializer->OnCreateNode(node);
	}

	//! reads materials of a node
	void CSceneLoaderIrr::readMaterials(IXMLReader* reader, ISceneNode* node)
	{
		UINT32 nr = 0;

		while (reader->read())
		{
			const wchar_t* name = reader->getNodeName();

			switch (reader->getNodeType())
			{
			case EXN_ELEMENT_END:
				if (SAPPHIRE_XML_FORMAT_MATERIALS == name)
					return;
				break;
			case EXN_ELEMENT:
				if (SAPPHIRE_XML_FORMAT_ATTRIBUTES == name)
				{
					// read materials from attribute list
					IAttributes* attr = FileSystem->createEmptyAttributes(SceneManager->getVideoDriver());
					attr->read(reader);

					if (node && node->getMaterialCount() > nr)
					{
						SceneManager->getVideoDriver()->fillMaterialStructureFromAttributes(
							node->getMaterial(nr), attr);
					}

					attr->drop();
					++nr;
				}
				break;
			default:
				break;
			}
		}
	}


	//! reads animators of a node
	void CSceneLoaderIrr::readAnimators(IXMLReader* reader, ISceneNode* node)
	{
		while (reader->read())
		{
			const wchar_t* name = reader->getNodeName();

			switch (reader->getNodeType())
			{
			case EXN_ELEMENT_END:
				if (SAPPHIRE_XML_FORMAT_ANIMATORS == name)
					return;
				break;
			case EXN_ELEMENT:
				if (SAPPHIRE_XML_FORMAT_ATTRIBUTES == name)
				{
					// read animator data from attribute list
					IAttributes* attr = FileSystem->createEmptyAttributes(SceneManager->getVideoDriver());
					attr->read(reader);

					if (node)
					{
						String typeName = attr->getAttributeAsString("Type");
						ISceneNodeAnimator* anim = SceneManager->createSceneNodeAnimator(typeName.c_str(), node);

						if (anim)
						{
							anim->deserializeAttributes(attr);
							anim->drop();
						}
					}

					attr->drop();
				}
				break;
			default:
				break;
			}
		}
	}


	//! reads user data of a node
	void CSceneLoaderIrr::readUserData(IXMLReader* reader, ISceneNode* node, ISceneUserDataSerializer* userDataSerializer)
	{
		while (reader->read())
		{
			const wchar_t* name = reader->getNodeName();

			switch (reader->getNodeType())
			{
			case EXN_ELEMENT_END:
				if (SAPPHIRE_XML_FORMAT_USERDATA == name)
					return;
				break;
			case EXN_ELEMENT:
				if (SAPPHIRE_XML_FORMAT_ATTRIBUTES == name)
				{
					// read user data from attribute list
					IAttributes* attr = FileSystem->createEmptyAttributes(SceneManager->getVideoDriver());
					attr->read(reader);

					if (node && userDataSerializer)
					{
						userDataSerializer->OnReadUserData(node, attr);
					}

					attr->drop();
				}
				break;
			default:
				break;
			}
		}
	}
}