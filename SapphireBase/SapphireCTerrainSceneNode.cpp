#include "SapphireCTerrainSceneNode.h"
#include "SapphireCTerrianTriangleSelector.h"
#include "SapphireIVideoDriver.h"
#include "SapphireIScenesManager.h"
#include "SapphireICameraSceneNode.h"
#include "SapphireSViewFrustum.h"
#include "SapphireMath.h"
#include "SapphireOS.h"
#include "SapphireFont.h"
#include "SapphireIFileSystem.h"
#include "SapphireIReadFile.h"
#include "SapphireITextSceneNode.h"
#include "SapphireIAnimatedMesh.h"
#include "SapphireSMesh.h"
#include "SapphireCDynamicMeshBuffer.h"
#include "SapphireSceneParameters.h"


namespace Sapphire
{
	//! constructor
	CTerrainSceneNode::CTerrainSceneNode(ISceneNode* parent, ISceneManager* mgr,
		IFileSystem* fs, SINT32 id, SINT32 maxLOD, E_TERRAIN_PATCH_SIZE patchSize,
		const Vector3& position,
		const Vector3& rotation,
		const Vector3& scale)
		: ITerrainSceneNode(parent, mgr, id, position, rotation, scale),
		TerrainData(patchSize, maxLOD, position, rotation, scale), RenderBuffer(0),
		VerticesToRender(0), IndicesToRender(0), DynamicSelectorUpdate(false),
		OverrideDistanceThreshold(false), UseDefaultRotationPivot(true), ForceRecalculation(true),
		CameraMovementDelta(10.0f), CameraRotationDelta(1.0f), CameraFOVDelta(0.1f),
		TCoordScale1(1.0f), TCoordScale2(1.0f), SmoothFactor(0), FileSystem(fs)
	{
#ifdef _DEBUG
		setDebugName("CTerrainSceneNode");
#endif

		Mesh = new SMesh();
		RenderBuffer = new CDynamicMeshBuffer(EVT_2TCOORDS, EIT_16BIT);
		RenderBuffer->setHardwareMappingHint(EHM_STATIC, EBT_VERTEX);
		RenderBuffer->setHardwareMappingHint(EHM_DYNAMIC, EBT_INDEX);

		if (FileSystem)
			FileSystem->grab();

		setAutomaticCulling(EAC_OFF);
	}


	//! destructor
	CTerrainSceneNode::~CTerrainSceneNode()
	{
		delete[] TerrainData.Patches;

		if (FileSystem)
			FileSystem->drop();

		if (Mesh)
			Mesh->drop();

		if (RenderBuffer)
			RenderBuffer->drop();
	}


	//! Initializes the terrain data. Loads the vertices from the heightMapFile
	bool CTerrainSceneNode::loadHeightMap(IReadFile* file, ColourValue vertexColor,
		SINT32 smoothFactor)
	{
		if (!file)
			return false;

		Mesh->MeshBuffers.clear();
		const UINT32 startTime = Timer::getRealTime();
		IImage* heightMap = SceneManager->getVideoDriver()->createImageFromFile(file);

		if (!heightMap)
		{
			Printer::log("Unable to load heightmap.");
			return false;
		}

		HeightmapFile = file->getFileName();
		SmoothFactor = smoothFactor;

		// Get the dimension of the heightmap data
		TerrainData.Size = heightMap->getDimension().Width;

		switch (TerrainData.PatchSize)
		{
		case ETPS_9:
			if (TerrainData.MaxLOD > 3)
			{
				TerrainData.MaxLOD = 3;
			}
			break;
		case ETPS_17:
			if (TerrainData.MaxLOD > 4)
			{
				TerrainData.MaxLOD = 4;
			}
			break;
		case ETPS_33:
			if (TerrainData.MaxLOD > 5)
			{
				TerrainData.MaxLOD = 5;
			}
			break;
		case ETPS_65:
			if (TerrainData.MaxLOD > 6)
			{
				TerrainData.MaxLOD = 6;
			}
			break;
		case ETPS_129:
			if (TerrainData.MaxLOD > 7)
			{
				TerrainData.MaxLOD = 7;
			}
			break;
		}

		// --- Generate vertex data from heightmap ----
		// resize the vertex array for the mesh buffer one time (makes loading faster)
		CDynamicMeshBuffer *mb = 0;

		const UINT32 numVertices = TerrainData.Size * TerrainData.Size;
		if (numVertices <= 65536)
		{
			//small enough for 16bit buffers
			mb = new CDynamicMeshBuffer(EVT_2TCOORDS, EIT_16BIT);
			RenderBuffer->getIndexBuffer().setType(EIT_16BIT);
		}
		else
		{
			//we need 32bit buffers
			mb = new CDynamicMeshBuffer(EVT_2TCOORDS, EIT_32BIT);
			RenderBuffer->getIndexBuffer().setType(EIT_32BIT);
		}

		mb->getVertexBuffer().set_used(numVertices);

		// Read the heightmap to get the vertex data
		// Apply positions changes, scaling changes
		const FLOAT32 tdSize = 1.0f / (FLOAT32)(TerrainData.Size - 1);
		SINT32 index = 0;
		float fx = 0.f;
		float fx2 = 0.f;
		for (SINT32 x = 0; x < TerrainData.Size; ++x)
		{
			float fz = 0.f;
			float fz2 = 0.f;
			for (SINT32 z = 0; z < TerrainData.Size; ++z)
			{
				S3DVertex2TCoords& vertex = static_cast<S3DVertex2TCoords*>(mb->getVertexBuffer().pointer())[index++];
				vertex.Normal.set(0.0f, 1.0f, 0.0f);
				vertex.Color = vertexColor;
				vertex.Pos.x = fx;
				vertex.Pos.y = (FLOAT32)heightMap->getPixel(TerrainData.Size - x - 1, z).getLightness();
				vertex.Pos.z = fz;

				vertex.TCoords.x = vertex.TCoords2.x = 1.f - fx2;
				vertex.TCoords.y = vertex.TCoords2.y = fz2;

				++fz;
				fz2 += tdSize;
			}
			++fx;
			fx2 += tdSize;
		}

		// drop heightMap, no longer needed
		heightMap->drop();

		smoothTerrain(mb, smoothFactor);

		// calculate smooth normals for the vertices
		calculateNormals(mb);

		// add the MeshBuffer to the mesh
		Mesh->addMeshBuffer(mb);

		// We copy the data to the renderBuffer, after the normals have been calculated.
		RenderBuffer->getVertexBuffer().set_used(numVertices);

		for (UINT32 i = 0; i < numVertices; ++i)
		{
			RenderBuffer->getVertexBuffer()[i] = mb->getVertexBuffer()[i];
			RenderBuffer->getVertexBuffer()[i].Pos *= TerrainData.Scale;
			RenderBuffer->getVertexBuffer()[i].Pos += TerrainData.Position;
		}

		// We no longer need the mb
		mb->drop();

		// calculate all the necessary data for the patches and the terrain
		calculateDistanceThresholds();
		createPatches();
		calculatePatchData();

		// set the default rotation pivot point to the terrain nodes center
		TerrainData.RotationPivot = TerrainData.Center;

		// Rotate the vertices of the terrain by the rotation
		// specified. Must be done after calculating the terrain data,
		// so we know what the current center of the terrain is.
		setRotation(TerrainData.Rotation);

		// Pre-allocate memory for indices

		RenderBuffer->getIndexBuffer().set_used(
			TerrainData.PatchCount * TerrainData.PatchCount *
			TerrainData.CalcPatchSize * TerrainData.CalcPatchSize * 6);

		RenderBuffer->setDirty();

		const UINT32 endTime = Timer::getRealTime();

		c8 tmp[255];
		snprintf(tmp, 255, "Generated terrain data (%dx%d) in %.4f seconds",
			TerrainData.Size, TerrainData.Size, (endTime - startTime) / 1000.0f);
		Printer::log(tmp);

		return true;
	}


	//! Initializes the terrain data. Loads the vertices from the heightMapFile
	bool CTerrainSceneNode::loadHeightMapRAW(IReadFile* file,
		SINT32 bitsPerPixel, bool signedData, bool floatVals,
		SINT32 width, ColourValue vertexColor, SINT32 smoothFactor)
	{
		if (!file)
			return false;
		if (floatVals && bitsPerPixel != 32)
			return false;

		// start reading
		const UINT32 startTime = Timer::getTime();

		Mesh->MeshBuffers.clear();

		const SINT32 bytesPerPixel = bitsPerPixel / 8;

		// Get the dimension of the heightmap data
		const SINT32 filesize = file->getSize();
		if (!width)
			TerrainData.Size = floor32(sqrtf((FLOAT32)(filesize / bytesPerPixel)));
		else
		{
			if ((filesize - file->getPos()) / bytesPerPixel>width*width)
			{
				Printer::log("Error reading heightmap RAW file", "File is too small.");
				return false;
			}
			TerrainData.Size = width;
		}

		switch (TerrainData.PatchSize)
		{
		case ETPS_9:
			if (TerrainData.MaxLOD > 3)
			{
				TerrainData.MaxLOD = 3;
			}
			break;
		case ETPS_17:
			if (TerrainData.MaxLOD > 4)
			{
				TerrainData.MaxLOD = 4;
			}
			break;
		case ETPS_33:
			if (TerrainData.MaxLOD > 5)
			{
				TerrainData.MaxLOD = 5;
			}
			break;
		case ETPS_65:
			if (TerrainData.MaxLOD > 6)
			{
				TerrainData.MaxLOD = 6;
			}
			break;
		case ETPS_129:
			if (TerrainData.MaxLOD > 7)
			{
				TerrainData.MaxLOD = 7;
			}
			break;
		}

		// --- Generate vertex data from heightmap ----
		// resize the vertex array for the mesh buffer one time (makes loading faster)
		CDynamicMeshBuffer *mb = 0;
		const UINT32 numVertices = TerrainData.Size * TerrainData.Size;
		if (numVertices <= 65536)
		{
			//small enough for 16bit buffers
			mb = new CDynamicMeshBuffer(EVT_2TCOORDS, EIT_16BIT);
			RenderBuffer->getIndexBuffer().setType(EIT_16BIT);
		}
		else
		{
			//we need 32bit buffers
			mb = new CDynamicMeshBuffer(EVT_2TCOORDS, EIT_32BIT);
			RenderBuffer->getIndexBuffer().setType(EIT_32BIT);
		}

		mb->getVertexBuffer().reallocate(numVertices);

		S3DVertex2TCoords vertex;
		vertex.Normal.set(0.0f, 1.0f, 0.0f);
		vertex.Color = vertexColor;

		// Read the heightmap to get the vertex data
		// Apply positions changes, scaling changes
		const FLOAT32 tdSize = 1.0f / (FLOAT32)(TerrainData.Size - 1);
		float fx = 0.f;
		float fx2 = 0.f;
		for (SINT32 x = 0; x < TerrainData.Size; ++x)
		{
			float fz = 0.f;
			float fz2 = 0.f;
			for (SINT32 z = 0; z < TerrainData.Size; ++z)
			{
				bool failure = false;
				vertex.Pos.x = fx;
				if (floatVals)
				{
					if (file->read(&vertex.Pos.y, bytesPerPixel) != bytesPerPixel)
						failure = true;
				}
				else if (signedData)
				{
					switch (bytesPerPixel)
					{
					case 1:
					{
						s8 val;
						if (file->read(&val, bytesPerPixel) != bytesPerPixel)
							failure = true;
						vertex.Pos.y = val;
					}
					break;
					case 2:
					{
						SINT16 val;
						if (file->read(&val, bytesPerPixel) != bytesPerPixel)
							failure = true;
						vertex.Pos.y = val / 256.f;
					}
					break;
					case 4:
					{
						SINT32 val;
						if (file->read(&val, bytesPerPixel) != bytesPerPixel)
							failure = true;
						vertex.Pos.y = val / 16777216.f;
					}
					break;
					}
				}
				else
				{
					switch (bytesPerPixel)
					{
					case 1:
					{
						UINT8 val;
						if (file->read(&val, bytesPerPixel) != bytesPerPixel)
							failure = true;
						vertex.Pos.y = val;
					}
					break;
					case 2:
					{
						UINT16 val;
						if (file->read(&val, bytesPerPixel) != bytesPerPixel)
							failure = true;
						vertex.Pos.y = val / 256.f;
					}
					break;
					case 4:
					{
						UINT32 val;
						if (file->read(&val, bytesPerPixel) != bytesPerPixel)
							failure = true;
						vertex.Pos.y = val / 16777216.f;
					}
					break;
					}
				}
				if (failure)
				{
					Printer::log("Error reading heightmap RAW file.");
					mb->drop();
					return false;
				}
				vertex.Pos.z = fz;

				vertex.TCoords.x = vertex.TCoords2.x = 1.f - fx2;
				vertex.TCoords.y = vertex.TCoords2.y = fz2;

				mb->getVertexBuffer().push_back(vertex);
				++fz;
				fz2 += tdSize;
			}
			++fx;
			fx2 += tdSize;
		}

		smoothTerrain(mb, smoothFactor);

		// calculate smooth normals for the vertices
		calculateNormals(mb);

		// add the MeshBuffer to the mesh
		Mesh->addMeshBuffer(mb);
		const UINT32 vertexCount = mb->getVertexCount();

		// We copy the data to the renderBuffer, after the normals have been calculated.
		RenderBuffer->getVertexBuffer().set_used(vertexCount);

		for (UINT32 i = 0; i < vertexCount; i++)
		{
			RenderBuffer->getVertexBuffer()[i] = mb->getVertexBuffer()[i];
			RenderBuffer->getVertexBuffer()[i].Pos *= TerrainData.Scale;
			RenderBuffer->getVertexBuffer()[i].Pos += TerrainData.Position;
		}

		// We no longer need the mb
		mb->drop();

		// calculate all the necessary data for the patches and the terrain
		calculateDistanceThresholds();
		createPatches();
		calculatePatchData();

		// set the default rotation pivot point to the terrain nodes center
		TerrainData.RotationPivot = TerrainData.Center;

		// Rotate the vertices of the terrain by the rotation specified. Must be done
		// after calculating the terrain data, so we know what the current center of the
		// terrain is.
		setRotation(TerrainData.Rotation);

		// Pre-allocate memory for indices
		RenderBuffer->getIndexBuffer().set_used(
			TerrainData.PatchCount*TerrainData.PatchCount*
			TerrainData.CalcPatchSize*TerrainData.CalcPatchSize * 6);

		const UINT32 endTime = Timer::getTime();

		c8 tmp[255];
		snprintf(tmp, 255, "Generated terrain data (%dx%d) in %.4f seconds",
			TerrainData.Size, TerrainData.Size, (endTime - startTime) / 1000.0f);
		Printer::log(tmp);

		return true;
	}


	//! Returns the mesh
	IMesh* CTerrainSceneNode::getMesh() { return Mesh; }


	//! Returns the material based on the zero based index i.
	SMaterial& CTerrainSceneNode::getMaterial(UINT32 i)
	{
		return Mesh->getMeshBuffer(i)->getMaterial();
	}


	//! Returns amount of materials used by this scene node ( always 1 )
	UINT32 CTerrainSceneNode::getMaterialCount() const
	{
		return Mesh->getMeshBufferCount();
	}


	//! Sets the scale of the scene node.
	//! \param scale: New scale of the node
	void CTerrainSceneNode::setScale(const Vector3& scale)
	{
		TerrainData.Scale = scale;
		applyTransformation();
		calculateNormals(RenderBuffer);
		ForceRecalculation = true;
	}


	//! Sets the rotation of the node. This only modifies
	//! the relative rotation of the node.
	//! \param rotation: New rotation of the node in degrees.
	void CTerrainSceneNode::setRotation(const Vector3& rotation)
	{
		TerrainData.Rotation = rotation;
		applyTransformation();
		ForceRecalculation = true;
	}


	//! Sets the pivot point for rotation of this node. This is useful for the TiledTerrainManager to
	//! rotate all terrain tiles around a global world point.
	//! NOTE: The default for the RotationPivot will be the center of the individual tile.
	void CTerrainSceneNode::setRotationPivot(const Vector3& pivot)
	{
		UseDefaultRotationPivot = false;
		TerrainData.RotationPivot = pivot;
	}


	//! Sets the position of the node.
	//! \param newpos: New postition of the scene node.
	void CTerrainSceneNode::setPosition(const Vector3& newpos)
	{
		TerrainData.Position = newpos;
		applyTransformation();
		ForceRecalculation = true;
	}


	//! Apply transformation changes(scale, position, rotation)
	void CTerrainSceneNode::applyTransformation()
	{
		if (!Mesh->getMeshBufferCount())
			return;

		Matrix4 rotMatrix;
		rotMatrix.setRotationDegrees(TerrainData.Rotation);

		const SINT32 vtxCount = Mesh->getMeshBuffer(0)->getVertexCount();
		for (SINT32 i = 0; i < vtxCount; ++i)
		{
			RenderBuffer->getVertexBuffer()[i].Pos = Mesh->getMeshBuffer(0)->getPosition(i) * TerrainData.Scale + TerrainData.Position;

			RenderBuffer->getVertexBuffer()[i].Pos -= TerrainData.RotationPivot;
			rotMatrix.inverseRotateVect(RenderBuffer->getVertexBuffer()[i].Pos);
			RenderBuffer->getVertexBuffer()[i].Pos += TerrainData.RotationPivot;
		}

		calculateDistanceThresholds(true);
		calculatePatchData();

		RenderBuffer->setDirty(EBT_VERTEX);
	}


	//! Updates the scene nodes indices if the camera has moved or rotated by a certain
	//! threshold, which can be changed using the SetCameraMovementDeltaThreshold and
	//! SetCameraRotationDeltaThreshold functions. This also determines if a given patch
	//! for the scene node is within the view frustum and if it's not the indices are not
	//! generated for that patch.
	void CTerrainSceneNode::OnRegisterSceneNode()
	{
		if (!IsVisible || !SceneManager->getActiveCamera())
			return;

		SceneManager->registerNodeForRendering(this);

		preRenderCalculationsIfNeeded();

		// Do Not call ISceneNode::OnRegisterSceneNode(), this node should have no children (luke: is this comment still true, as ISceneNode::OnRegisterSceneNode() is called?)

		ISceneNode::OnRegisterSceneNode();
		ForceRecalculation = false;
	}

	void CTerrainSceneNode::preRenderCalculationsIfNeeded()
	{
		ICameraSceneNode * camera = SceneManager->getActiveCamera();
		if (!camera)
			return;

		// Determine the camera rotation, based on the camera direction.
		const Vector3 cameraPosition = camera->getAbsolutePosition();
		const Vector3 cameraRotation = line3d<FLOAT32>(cameraPosition, camera->getTarget()).getVector().getHorizontalAngle();
		Vector3 cameraUp = camera->getUpVector();
		cameraUp.normalize();
		const FLOAT32 CameraFOV = SceneManager->getActiveCamera()->getFOV();

		// Only check on the Camera's Y Rotation
		if (!ForceRecalculation)
		{
			if ((fabsf(cameraRotation.x - OldCameraRotation.x) < CameraRotationDelta) &&
				(fabsf(cameraRotation.y - OldCameraRotation.y) < CameraRotationDelta))
			{
				if ((fabs(cameraPosition.x - OldCameraPosition.x) < CameraMovementDelta) &&
					(fabs(cameraPosition.y - OldCameraPosition.y) < CameraMovementDelta) &&
					(fabs(cameraPosition.z - OldCameraPosition.z) < CameraMovementDelta))
				{
					if (fabs(CameraFOV - OldCameraFOV) < CameraFOVDelta &&
						cameraUp.dotProduct(OldCameraUp) > (1.f - (cos(DEGTORAD * CameraRotationDelta))))
					{
						return;
					}
				}
			}
		}

		//we need to redo calculations...

		OldCameraPosition = cameraPosition;
		OldCameraRotation = cameraRotation;
		OldCameraUp = cameraUp;
		OldCameraFOV = CameraFOV;

		preRenderLODCalculations();
		preRenderIndicesCalculations();
	}

	void CTerrainSceneNode::preRenderLODCalculations()
	{
		ICameraSceneNode * camera = SceneManager->getActiveCamera();

		if (!camera)
			return;

		const Vector3 cameraPosition = camera->getAbsolutePosition();

		const SViewFrustum* frustum = camera->getViewFrustum();

		// Determine each patches LOD based on distance from camera (and whether or not they are in
		// the view frustum).
		const SINT32 count = TerrainData.PatchCount * TerrainData.PatchCount;
		for (SINT32 j = 0; j < count; ++j)
		{
			if (frustum->getBoundingBox().intersects(TerrainData.Patches[j].BoundingBox))
			{
				const FLOAT32 distance = cameraPosition.squaredDistance(TerrainData.Patches[j].Center);

				TerrainData.Patches[j].CurrentLOD = 0;
				for (SINT32 i = TerrainData.MaxLOD - 1; i>0; --i)
				{
					if (distance >= TerrainData.LODDistanceThreshold[i])
					{
						TerrainData.Patches[j].CurrentLOD = i;
						break;
					}
				}
			}
			else
			{
				TerrainData.Patches[j].CurrentLOD = -1;
			}
		}
	}


	void CTerrainSceneNode::preRenderIndicesCalculations()
	{
		IIndexBuffer& indexBuffer = RenderBuffer->getIndexBuffer();
		IndicesToRender = 0;
		indexBuffer.set_used(0);

		SINT32 index = 0;
		// Then generate the indices for all patches that are visible.
		for (SINT32 i = 0; i < TerrainData.PatchCount; ++i)
		{
			for (SINT32 j = 0; j < TerrainData.PatchCount; ++j)
			{
				if (TerrainData.Patches[index].CurrentLOD >= 0)
				{
					SINT32 x = 0;
					SINT32 z = 0;

					// calculate the step we take this patch, based on the patches current LOD
					const SINT32 step = 1 << TerrainData.Patches[index].CurrentLOD;

					// Loop through patch and generate indices
					while (z < TerrainData.CalcPatchSize)
					{
						const SINT32 index11 = getIndex(j, i, index, x, z);
						const SINT32 index21 = getIndex(j, i, index, x + step, z);
						const SINT32 index12 = getIndex(j, i, index, x, z + step);
						const SINT32 index22 = getIndex(j, i, index, x + step, z + step);

						indexBuffer.push_back(index12);
						indexBuffer.push_back(index11);
						indexBuffer.push_back(index22);
						indexBuffer.push_back(index22);
						indexBuffer.push_back(index11);
						indexBuffer.push_back(index21);
						IndicesToRender += 6;

						// increment index position horizontally
						x += step;

						// we've hit an edge
						if (x >= TerrainData.CalcPatchSize)
						{
							x = 0;
							z += step;
						}
					}
				}
				++index;
			}
		}

		RenderBuffer->setDirty(EBT_INDEX);

		if (DynamicSelectorUpdate && TriangleSelector)
		{
			CTerrainTriangleSelector* selector = (CTerrainTriangleSelector*)TriangleSelector;
			selector->setTriangleData(this, -1);
		}
	}


	//! Render the scene node
	void CTerrainSceneNode::render()
	{
		if (!IsVisible || !SceneManager->getActiveCamera())
			return;

		if (!Mesh->getMeshBufferCount())
			return;

		IVideoDriver* driver = SceneManager->getVideoDriver();

		driver->setTransform(ETS_WORLD, Matrix4::IDENTITY);
		driver->setMaterial(Mesh->getMeshBuffer(0)->getMaterial());

		RenderBuffer->getIndexBuffer().set_used(IndicesToRender);

		// For use with geomorphing
		driver->drawMeshBuffer(RenderBuffer);

		RenderBuffer->getIndexBuffer().set_used(RenderBuffer->getIndexBuffer().allocated_size());

		// for debug purposes only:
		if (DebugDataVisible)
		{
			SMaterial m;
			m.Lighting = false;
			driver->setMaterial(m);
			if (DebugDataVisible & EDS_BBOX)
				driver->draw3DBox(TerrainData.BoundingBox, ColourValue::getColourValue(255, 255, 255, 255));

			const SINT32 count = TerrainData.PatchCount * TerrainData.PatchCount;
			SINT32 visible = 0;
			if (DebugDataVisible & EDS_BBOX_BUFFERS)
			{
				for (SINT32 j = 0; j < count; ++j)
				{
					driver->draw3DBox(TerrainData.Patches[j].BoundingBox, ColourValue::getColourValue(255, 255, 0, 0));
					visible += (TerrainData.Patches[j].CurrentLOD >= 0);
				}
			}

			if (DebugDataVisible & EDS_NORMALS)
			{
				// draw normals
				const FLOAT32 debugNormalLength = SceneManager->getParameters()->getAttributeAsFloat(DEBUG_NORMAL_LENGTH);
				const ColourValue debugNormalColor = SceneManager->getParameters()->getAttributeAsColor(DEBUG_NORMAL_COLOR);
				driver->drawMeshBufferNormals(RenderBuffer, debugNormalLength, debugNormalColor);
			}

			driver->setTransform(ETS_WORLD, AbsoluteTransformation);

			static UINT32 lastTime = 0;

			const UINT32 now = Timer::getRealTime();
			if (now - lastTime > 1000)
			{
				char buf[64];
				snprintf(buf, 64, "Count: %d, Visible: %d", count, visible);
				Printer::log(buf);

				lastTime = now;
			}
		}
	}


	//! Return the bounding box of the entire terrain.
	const AxisAlignedBox& CTerrainSceneNode::getBoundingBox() const
	{
		return TerrainData.BoundingBox;
	}


	//! Return the bounding box of a patch
	const AxisAlignedBox& CTerrainSceneNode::getBoundingBox(SINT32 patchX, SINT32 patchZ) const
	{
		return TerrainData.Patches[patchX * TerrainData.PatchCount + patchZ].BoundingBox;
	}


	//! Gets the meshbuffer data based on a specified Level of Detail.
	//! \param mb: A reference to an SMeshBuffer object
	//! \param LOD: The Level Of Detail you want the indices from.
	void CTerrainSceneNode::getMeshBufferForLOD(IDynamicMeshBuffer& mb, SINT32 LOD) const
	{
		if (!Mesh->getMeshBufferCount())
			return;

		LOD = Math::Clamp(LOD, 0, TerrainData.MaxLOD - 1);

		const UINT32 numVertices = Mesh->getMeshBuffer(0)->getVertexCount();
		mb.getVertexBuffer().reallocate(numVertices);
		S3DVertex2TCoords* vertices = (S3DVertex2TCoords*)Mesh->getMeshBuffer(0)->getVertices();

		for (UINT32 n = 0; n<numVertices; ++n)
			mb.getVertexBuffer().push_back(vertices[n]);

		mb.getIndexBuffer().setType(RenderBuffer->getIndexBuffer().getType());

		// calculate the step we take for all patches, since LOD is the same
		const SINT32 step = 1 << LOD;

		// Generate the indices for all patches at the specified LOD
		SINT32 index = 0;
		for (SINT32 i = 0; i<TerrainData.PatchCount; ++i)
		{
			for (SINT32 j = 0; j<TerrainData.PatchCount; ++j)
			{
				SINT32 x = 0;
				SINT32 z = 0;

				// Loop through patch and generate indices
				while (z < TerrainData.CalcPatchSize)
				{
					const SINT32 index11 = getIndex(j, i, index, x, z);
					const SINT32 index21 = getIndex(j, i, index, x + step, z);
					const SINT32 index12 = getIndex(j, i, index, x, z + step);
					const SINT32 index22 = getIndex(j, i, index, x + step, z + step);

					mb.getIndexBuffer().push_back(index12);
					mb.getIndexBuffer().push_back(index11);
					mb.getIndexBuffer().push_back(index22);
					mb.getIndexBuffer().push_back(index22);
					mb.getIndexBuffer().push_back(index11);
					mb.getIndexBuffer().push_back(index21);

					// increment index position horizontally
					x += step;

					if (x >= TerrainData.CalcPatchSize) // we've hit an edge
					{
						x = 0;
						z += step;
					}
				}
				++index;
			}
		}
	}


	//! Gets the indices for a specified patch at a specified Level of Detail.
	//! \param mb: A reference to an array of UINT32 indices.
	//! \param patchX: Patch x coordinate.
	//! \param patchZ: Patch z coordinate.
	//! \param LOD: The level of detail to get for that patch. If -1, then get
	//! the CurrentLOD. If the CurrentLOD is set to -1, meaning it's not shown,
	//! then it will retrieve the triangles at the highest LOD (0).
	//! \return: Number if indices put into the buffer.
	SINT32 CTerrainSceneNode::getIndicesForPatch(vector<UINT32>::type& indices, SINT32 patchX, SINT32 patchZ, SINT32 LOD)
	{
		if (patchX < 0 || patchX > TerrainData.PatchCount - 1 ||
			patchZ < 0 || patchZ > TerrainData.PatchCount - 1)
			return -1;

		if (LOD < -1 || LOD > TerrainData.MaxLOD - 1)
			return -1;

		vector<SINT32>::type cLODs;
		bool setLODs = false;

		// If LOD of -1 was passed in, use the CurrentLOD of the patch specified
		if (LOD == -1)
		{
			LOD = TerrainData.Patches[patchX * TerrainData.PatchCount + patchZ].CurrentLOD;
		}
		else
		{
			getCurrentLODOfPatches(cLODs);
			setCurrentLODOfPatches(LOD);
			setLODs = true;
		}

		if (LOD < 0)
			return -2; // Patch not visible, don't generate indices.

		// calculate the step we take for this LOD
		const SINT32 step = 1 << LOD;

		// Generate the indices for the specified patch at the specified LOD
		const SINT32 index = patchX * TerrainData.PatchCount + patchZ;

		SINT32 x = 0;
		SINT32 z = 0;

		//indices.set_used(TerrainData.PatchSize * TerrainData.PatchSize * 6);
		indices.reserve(TerrainData.PatchSize * TerrainData.PatchSize * 6);

		// Loop through patch and generate indices
		SINT32 rv = 0;
		while (z<TerrainData.CalcPatchSize)
		{
			const SINT32 index11 = getIndex(patchZ, patchX, index, x, z);
			const SINT32 index21 = getIndex(patchZ, patchX, index, x + step, z);
			const SINT32 index12 = getIndex(patchZ, patchX, index, x, z + step);
			const SINT32 index22 = getIndex(patchZ, patchX, index, x + step, z + step);

			indices[rv++] = index12;
			indices[rv++] = index11;
			indices[rv++] = index22;
			indices[rv++] = index22;
			indices[rv++] = index11;
			indices[rv++] = index21;

			// increment index position horizontally
			x += step;

			if (x >= TerrainData.CalcPatchSize) // we've hit an edge
			{
				x = 0;
				z += step;
			}
		}

		if (setLODs)
			setCurrentLODOfPatches(cLODs);

		return rv;
	}


	//! Populates an array with the CurrentLOD of each patch.
	//! \param LODs: A reference to a array<SINT32> to hold the values
	//! \return Returns the number of elements in the array
	SINT32 CTerrainSceneNode::getCurrentLODOfPatches(vector<SINT32>::type& LODs) const
	{
		SINT32 numLODs;
		LODs.clear();

		const SINT32 count = TerrainData.PatchCount * TerrainData.PatchCount;
		for (numLODs = 0; numLODs < count; numLODs++)
			LODs.push_back(TerrainData.Patches[numLODs].CurrentLOD);

		return LODs.size();
	}


	//! Manually sets the LOD of a patch
	//! \param patchX: Patch x coordinate.
	//! \param patchZ: Patch z coordinate.
	//! \param LOD: The level of detail to set the patch to.
	void CTerrainSceneNode::setLODOfPatch(SINT32 patchX, SINT32 patchZ, SINT32 LOD)
	{
		TerrainData.Patches[patchX * TerrainData.PatchCount + patchZ].CurrentLOD = LOD;
	}


	//! Override the default generation of distance thresholds for determining the LOD a patch
	//! is rendered at.
	bool CTerrainSceneNode::overrideLODDistance(SINT32 LOD, Real newDistance)
	{
		OverrideDistanceThreshold = true;

		if (LOD < 0 || LOD > TerrainData.MaxLOD - 1)
			return false;

		TerrainData.LODDistanceThreshold[LOD] = newDistance * newDistance;

		return true;
	}


	//! Creates a planar texture mapping on the terrain
	//! \param resolution: resolution of the planar mapping. This is the value
	//! specifying the relation between world space and texture coordinate space.
	void CTerrainSceneNode::scaleTexture(FLOAT32 resolution, FLOAT32 resolution2)
	{
		TCoordScale1 = resolution;
		TCoordScale2 = resolution2;

		const FLOAT32 resBySize = resolution / (FLOAT32)(TerrainData.Size - 1);
		const FLOAT32 res2BySize = resolution2 / (FLOAT32)(TerrainData.Size - 1);
		UINT32 index = 0;
		FLOAT32 xval = 0.f;
		FLOAT32 x2val = 0.f;
		for (SINT32 x = 0; x<TerrainData.Size; ++x)
		{
			FLOAT32 zval = 0.f;
			FLOAT32 z2val = 0.f;
			for (SINT32 z = 0; z<TerrainData.Size; ++z)
			{
				RenderBuffer->getVertexBuffer()[index].TCoords.x = 1.f - xval;
				RenderBuffer->getVertexBuffer()[index].TCoords.y = zval;

				if (RenderBuffer->getVertexType() == EVT_2TCOORDS)
				{
					if (resolution2 == 0)
					{
						((S3DVertex2TCoords&)RenderBuffer->getVertexBuffer()[index]).TCoords2 = RenderBuffer->getVertexBuffer()[index].TCoords;
					}
					else
					{
						((S3DVertex2TCoords&)RenderBuffer->getVertexBuffer()[index]).TCoords2.x = 1.f - x2val;
						((S3DVertex2TCoords&)RenderBuffer->getVertexBuffer()[index]).TCoords2.y = z2val;
					}
				}

				++index;
				zval += resBySize;
				z2val += res2BySize;
			}
			xval += resBySize;
			x2val += res2BySize;
		}

		RenderBuffer->setDirty(EBT_VERTEX);
	}


	//! used to get the indices when generating index data for patches at varying levels of detail.
	UINT32 CTerrainSceneNode::getIndex(const SINT32 PatchX, const SINT32 PatchZ,
		const SINT32 PatchIndex, UINT32 vX, UINT32 vZ) const
	{
		// top border
		if (vZ == 0)
		{
			if (TerrainData.Patches[PatchIndex].Top &&
				TerrainData.Patches[PatchIndex].CurrentLOD < TerrainData.Patches[PatchIndex].Top->CurrentLOD &&
				(vX % (1 << TerrainData.Patches[PatchIndex].Top->CurrentLOD)) != 0)
			{
				vX -= vX % (1 << TerrainData.Patches[PatchIndex].Top->CurrentLOD);
			}
		}
		else
			if (vZ == (UINT32)TerrainData.CalcPatchSize) // bottom border
			{
				if (TerrainData.Patches[PatchIndex].Bottom &&
					TerrainData.Patches[PatchIndex].CurrentLOD < TerrainData.Patches[PatchIndex].Bottom->CurrentLOD &&
					(vX % (1 << TerrainData.Patches[PatchIndex].Bottom->CurrentLOD)) != 0)
				{
					vX -= vX % (1 << TerrainData.Patches[PatchIndex].Bottom->CurrentLOD);
				}
			}

		// left border
		if (vX == 0)
		{
			if (TerrainData.Patches[PatchIndex].Left &&
				TerrainData.Patches[PatchIndex].CurrentLOD < TerrainData.Patches[PatchIndex].Left->CurrentLOD &&
				(vZ % (1 << TerrainData.Patches[PatchIndex].Left->CurrentLOD)) != 0)
			{
				vZ -= vZ % (1 << TerrainData.Patches[PatchIndex].Left->CurrentLOD);
			}
		}
		else
			if (vX == (UINT32)TerrainData.CalcPatchSize) // right border
			{
				if (TerrainData.Patches[PatchIndex].Right &&
					TerrainData.Patches[PatchIndex].CurrentLOD < TerrainData.Patches[PatchIndex].Right->CurrentLOD &&
					(vZ % (1 << TerrainData.Patches[PatchIndex].Right->CurrentLOD)) != 0)
				{
					vZ -= vZ % (1 << TerrainData.Patches[PatchIndex].Right->CurrentLOD);
				}
			}

		if (vZ >= (UINT32)TerrainData.PatchSize)
			vZ = TerrainData.CalcPatchSize;

		if (vX >= (UINT32)TerrainData.PatchSize)
			vX = TerrainData.CalcPatchSize;

		return (vZ + ((TerrainData.CalcPatchSize) * PatchZ)) * TerrainData.Size +
			(vX + ((TerrainData.CalcPatchSize) * PatchX));
	}


	//! smooth the terrain
	void CTerrainSceneNode::smoothTerrain(IDynamicMeshBuffer* mb, SINT32 smoothFactor)
	{
		for (SINT32 run = 0; run < smoothFactor; ++run)
		{
			SINT32 yd = TerrainData.Size;
			for (SINT32 y = 1; y < TerrainData.Size - 1; ++y)
			{
				for (SINT32 x = 1; x < TerrainData.Size - 1; ++x)
				{
					mb->getVertexBuffer()[x + yd].Pos.y =
						(mb->getVertexBuffer()[x - 1 + yd].Pos.y + //left
						mb->getVertexBuffer()[x + 1 + yd].Pos.y + //right
						mb->getVertexBuffer()[x + yd - TerrainData.Size].Pos.y + //above
						mb->getVertexBuffer()[x + yd + TerrainData.Size].Pos.y) * 0.25f; //below
				}
				yd += TerrainData.Size;
			}
		}
	}


	//! calculate smooth normals
	void CTerrainSceneNode::calculateNormals(IDynamicMeshBuffer* mb)
	{
		SINT32 count;
		Vector3 a, b, c, t;

		for (SINT32 x = 0; x<TerrainData.Size; ++x)
		{
			for (SINT32 z = 0; z<TerrainData.Size; ++z)
			{
				count = 0;
				Vector3 normal;

				// top left
				if (x>0 && z>0)
				{
					a = mb->getVertexBuffer()[(x - 1)*TerrainData.Size + z - 1].Pos;
					b = mb->getVertexBuffer()[(x - 1)*TerrainData.Size + z].Pos;
					c = mb->getVertexBuffer()[x*TerrainData.Size + z].Pos;
					b -= a;
					c -= a;
					t = b.crossProduct(c);
					t.normalize();
					normal += t;

					a = mb->getVertexBuffer()[(x - 1)*TerrainData.Size + z - 1].Pos;
					b = mb->getVertexBuffer()[x*TerrainData.Size + z - 1].Pos;
					c = mb->getVertexBuffer()[x*TerrainData.Size + z].Pos;
					b -= a;
					c -= a;
					t = b.crossProduct(c);
					t.normalize();
					normal += t;

					count += 2;
				}

				// top right
				if (x>0 && z<TerrainData.Size - 1)
				{
					a = mb->getVertexBuffer()[(x - 1)*TerrainData.Size + z].Pos;
					b = mb->getVertexBuffer()[(x - 1)*TerrainData.Size + z + 1].Pos;
					c = mb->getVertexBuffer()[x*TerrainData.Size + z + 1].Pos;
					b -= a;
					c -= a;
					t = b.crossProduct(c);
					t.normalize();
					normal += t;

					a = mb->getVertexBuffer()[(x - 1)*TerrainData.Size + z].Pos;
					b = mb->getVertexBuffer()[x*TerrainData.Size + z + 1].Pos;
					c = mb->getVertexBuffer()[x*TerrainData.Size + z].Pos;
					b -= a;
					c -= a;
					t = b.crossProduct(c);
					t.normalize();
					normal += t;

					count += 2;
				}

				// bottom right
				if (x<TerrainData.Size - 1 && z<TerrainData.Size - 1)
				{
					a = mb->getVertexBuffer()[x*TerrainData.Size + z + 1].Pos;
					b = mb->getVertexBuffer()[x*TerrainData.Size + z].Pos;
					c = mb->getVertexBuffer()[(x + 1)*TerrainData.Size + z + 1].Pos;
					b -= a;
					c -= a;
					t = b.crossProduct(c);
					t.normalize();
					normal += t;

					a = mb->getVertexBuffer()[x*TerrainData.Size + z + 1].Pos;
					b = mb->getVertexBuffer()[(x + 1)*TerrainData.Size + z + 1].Pos;
					c = mb->getVertexBuffer()[(x + 1)*TerrainData.Size + z].Pos;
					b -= a;
					c -= a;
					t = b.crossProduct(c);
					t.normalize();
					normal += t;

					count += 2;
				}

				// bottom left
				if (x<TerrainData.Size - 1 && z>0)
				{
					a = mb->getVertexBuffer()[x*TerrainData.Size + z - 1].Pos;
					b = mb->getVertexBuffer()[x*TerrainData.Size + z].Pos;
					c = mb->getVertexBuffer()[(x + 1)*TerrainData.Size + z].Pos;
					b -= a;
					c -= a;
					t = b.crossProduct(c);
					t.normalize();
					normal += t;

					a = mb->getVertexBuffer()[x*TerrainData.Size + z - 1].Pos;
					b = mb->getVertexBuffer()[(x + 1)*TerrainData.Size + z].Pos;
					c = mb->getVertexBuffer()[(x + 1)*TerrainData.Size + z - 1].Pos;
					b -= a;
					c -= a;
					t = b.crossProduct(c);
					t.normalize();
					normal += t;

					count += 2;
				}

				if (count != 0)
				{
					normal.normalize();
				}
				else
				{
					normal.set(0.0f, 1.0f, 0.0f);
				}

				mb->getVertexBuffer()[x * TerrainData.Size + z].Normal = normal;
			}
		}
	}


	//! create patches, stuff that needs to be done only once for patches goes here.
	void CTerrainSceneNode::createPatches()
	{
		TerrainData.PatchCount = (TerrainData.Size - 1) / (TerrainData.CalcPatchSize);

		if (TerrainData.Patches)
			delete[] TerrainData.Patches;

		TerrainData.Patches = new SPatch[TerrainData.PatchCount * TerrainData.PatchCount];
	}


	//! used to calculate the internal STerrainData structure both at creation and after scaling/position calls.
	void CTerrainSceneNode::calculatePatchData()
	{
		// Reset the Terrains Bounding Box for re-calculation
		TerrainData.BoundingBox.reset(RenderBuffer->getPosition(0));

		for (SINT32 x = 0; x < TerrainData.PatchCount; ++x)
		{
			for (SINT32 z = 0; z < TerrainData.PatchCount; ++z)
			{
				const SINT32 index = x * TerrainData.PatchCount + z;
				SPatch& patch = TerrainData.Patches[index];
				patch.CurrentLOD = 0;

				const SINT32 xstart = x*TerrainData.CalcPatchSize;
				const SINT32 xend = xstart + TerrainData.CalcPatchSize;
				const SINT32 zstart = z*TerrainData.CalcPatchSize;
				const SINT32 zend = zstart + TerrainData.CalcPatchSize;
				// For each patch, calculate the bounding box (mins and maxes)
				patch.BoundingBox.reset(RenderBuffer->getPosition(xstart*TerrainData.Size + zstart));

				for (SINT32 xx = xstart; xx <= xend; ++xx)
					for (SINT32 zz = zstart; zz <= zend; ++zz)
						patch.BoundingBox.addInternalPoint(RenderBuffer->getVertexBuffer()[xx * TerrainData.Size + zz].Pos);

				// Reconfigure the bounding box of the terrain as a whole
				TerrainData.BoundingBox.addInternalBox(patch.BoundingBox);

				// get center of Patch
				patch.Center = patch.BoundingBox.getCenter();

				// Assign Neighbours
				// Top
				if (x > 0)
					patch.Top = &TerrainData.Patches[(x - 1) * TerrainData.PatchCount + z];
				else
					patch.Top = 0;

				// Bottom
				if (x < TerrainData.PatchCount - 1)
					patch.Bottom = &TerrainData.Patches[(x + 1) * TerrainData.PatchCount + z];
				else
					patch.Bottom = 0;

				// Left
				if (z > 0)
					patch.Left = &TerrainData.Patches[x * TerrainData.PatchCount + z - 1];
				else
					patch.Left = 0;

				// Right
				if (z < TerrainData.PatchCount - 1)
					patch.Right = &TerrainData.Patches[x * TerrainData.PatchCount + z + 1];
				else
					patch.Right = 0;
			}
		}

		// get center of Terrain
		TerrainData.Center = TerrainData.BoundingBox.getCenter();

		// if the default rotation pivot is still being used, update it.
		if (UseDefaultRotationPivot)
		{
			TerrainData.RotationPivot = TerrainData.Center;
		}
	}


	//! used to calculate or recalculate the distance thresholds
	void CTerrainSceneNode::calculateDistanceThresholds(bool scalechanged)
	{
		// Only update the LODDistanceThreshold if it's not manually changed
		if (!OverrideDistanceThreshold)
		{
			//TerrainData.LODDistanceThreshold.set_used(0);
			TerrainData.LODDistanceThreshold.clear();
			// Determine new distance threshold for determining what LOD to draw patches at
			//TerrainData.LODDistanceThreshold.reallocate(TerrainData.MaxLOD);
			TerrainData.LODDistanceThreshold.reserve(TerrainData.MaxLOD);

			const Real size = TerrainData.PatchSize * TerrainData.PatchSize *
				TerrainData.Scale.x * TerrainData.Scale.z;
			for (SINT32 i = 0; i<TerrainData.MaxLOD; ++i)
			{
				TerrainData.LODDistanceThreshold.push_back(size * ((i + 1 + i / 2) * (i + 1 + i / 2)));
			}
		}
	}


	void CTerrainSceneNode::setCurrentLODOfPatches(SINT32 lod)
	{
		const SINT32 count = TerrainData.PatchCount * TerrainData.PatchCount;
		for (SINT32 i = 0; i< count; ++i)
			TerrainData.Patches[i].CurrentLOD = lod;
	}


	void CTerrainSceneNode::setCurrentLODOfPatches(const vector<SINT32>::type& lodarray)
	{
		const SINT32 count = TerrainData.PatchCount * TerrainData.PatchCount;
		for (SINT32 i = 0; i<count; ++i)
			TerrainData.Patches[i].CurrentLOD = lodarray[i];
	}


	//! Gets the height
	FLOAT32 CTerrainSceneNode::getHeight(FLOAT32 x, FLOAT32 z) const
	{
		if (!Mesh->getMeshBufferCount())
			return 0;

		Matrix4 rotMatrix;
		rotMatrix.setRotationDegrees(TerrainData.Rotation);
		Vector3 pos(x, 0.0f, z);
		//rotMatrix.rotateVect(pos);
		pos = rotMatrix.rotateVect(pos);
		pos -= TerrainData.Position;
		pos /= TerrainData.Scale;

		SINT32 X(floor32(pos.x));
		SINT32 Z(floor32(pos.z));

		FLOAT32 height = -FLT_MAX;
		if (X >= 0 && X < TerrainData.Size - 1 &&
			Z >= 0 && Z < TerrainData.Size - 1)
		{
			const S3DVertex2TCoords* Vertices = (const S3DVertex2TCoords*)Mesh->getMeshBuffer(0)->getVertices();
			const Vector3& a = Vertices[X * TerrainData.Size + Z].Pos;
			const Vector3& b = Vertices[(X + 1) * TerrainData.Size + Z].Pos;
			const Vector3& c = Vertices[X * TerrainData.Size + (Z + 1)].Pos;
			const Vector3& d = Vertices[(X + 1) * TerrainData.Size + (Z + 1)].Pos;

			// offset from integer position
			const FLOAT32 dx = pos.x - X;
			const FLOAT32 dz = pos.z - Z;

			if (dx > dz)
				height = a.y + (d.y - b.y)*dz + (b.y - a.y)*dx;
			else
				height = a.y + (d.y - c.y)*dx + (c.y - a.y)*dz;

			height *= TerrainData.Scale.y;
			height += TerrainData.Position.y;
		}

		return height;
	}


	//! Writes attributes of the scene node.
	void CTerrainSceneNode::serializeAttributes(IAttributes* out,
		SAttributeReadWriteOptions* options) const
	{
		ISceneNode::serializeAttributes(out, options);

		out->addString("Heightmap", HeightmapFile.c_str());
		out->addFloat("TextureScale1", TCoordScale1);
		out->addFloat("TextureScale2", TCoordScale2);
		out->addInt("SmoothFactor", SmoothFactor);
	}


	//! Reads attributes of the scene node.
	void CTerrainSceneNode::deserializeAttributes(IAttributes* in,
		SAttributeReadWriteOptions* options)
	{
		path newHeightmap = in->getAttributeAsString("Heightmap");
		FLOAT32 tcoordScale1 = in->getAttributeAsFloat("TextureScale1");
		FLOAT32 tcoordScale2 = in->getAttributeAsFloat("TextureScale2");
		SINT32 smoothFactor = in->getAttributeAsInt("SmoothFactor");

		// set possible new heightmap

		if (newHeightmap.size() != 0 && newHeightmap != HeightmapFile)
		{
			IReadFile* file = FileSystem->createAndOpenFile(newHeightmap.c_str());
			if (file)
			{
				loadHeightMap(file, ColourValue::getColourValue(255, 255, 255, 255), smoothFactor);
				file->drop();
			}
			else
				Printer::log("could not open heightmap", newHeightmap.c_str());
		}

		// set possible new scale

		if (Math::equals(tcoordScale1, 0.f))
			tcoordScale1 = 1.0f;

		if (Math::equals(tcoordScale2, 0.f))
			tcoordScale2 = 1.0f;

		if (!Math::equals(tcoordScale1, TCoordScale1) ||
			!Math::equals(tcoordScale2, TCoordScale2))
		{
			scaleTexture(tcoordScale1, tcoordScale2);
		}

		ISceneNode::deserializeAttributes(in, options);
	}


	//! Creates a clone of this scene node and its children.
	ISceneNode* CTerrainSceneNode::clone(ISceneNode* newParent, ISceneManager* newManager)
	{
		if (!newParent)
			newParent = Parent;
		if (!newManager)
			newManager = SceneManager;

		CTerrainSceneNode* nb = new CTerrainSceneNode(
			newParent, newManager, FileSystem, ID,
			4, ETPS_17, getPosition(), getRotation(), getScale());

		nb->cloneMembers(this, newManager);

		// instead of cloning the data structures, recreate the terrain.
		// (temporary solution)

		// load file

		IReadFile* file = FileSystem->createAndOpenFile(HeightmapFile.c_str());
		if (file)
		{
			nb->loadHeightMap(file, ColourValue::getColourValue(255, 255, 255, 255), 0);
			file->drop();
		}

		// scale textures

		nb->scaleTexture(TCoordScale1, TCoordScale2);

		// copy materials

		for (unsigned int m = 0; m<Mesh->getMeshBufferCount(); ++m)
		{
			if (nb->Mesh->getMeshBufferCount()>m &&
				nb->Mesh->getMeshBuffer(m) &&
				Mesh->getMeshBuffer(m))
			{
				nb->Mesh->getMeshBuffer(m)->getMaterial() =
					Mesh->getMeshBuffer(m)->getMaterial();
			}
		}

		nb->RenderBuffer->getMaterial() = RenderBuffer->getMaterial();

		// finish

		if (newParent)
			nb->drop();
		return nb;
	}
}