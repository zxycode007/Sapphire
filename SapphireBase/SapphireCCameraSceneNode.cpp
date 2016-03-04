#include "SapphireCCameraSceneNode.h"
#include "SapphireIScenesManager.h"
#include "SapphireIVideoDriver.h"
#include "SapphireOS.h"

namespace Sapphire
{

	//! constructor
	CCameraSceneNode::CCameraSceneNode(ISceneNode* parent, ISceneManager* mgr, SINT32 id,
		const Vector3& position, const Vector3& lookat)
		: ICameraSceneNode(parent, mgr, id, position),
		Target(lookat), UpVector(0.0f, 1.0f, 0.0f), ZNear(1.0f), ZFar(3000.0f),
		InputReceiverEnabled(true), TargetAndRotationAreBound(false)
	{
#ifdef _DEBUG
		setDebugName("CCameraSceneNode");
#endif

		// set default projection
		Fovy = PI / 2.5f;	// Field of view, in radians.

		const IVideoDriver* const d = mgr ? mgr->getVideoDriver() : 0;
		if (d)
			Aspect = (FLOAT32)d->getCurrentRenderTargetSize().Width /
			(FLOAT32)d->getCurrentRenderTargetSize().Height;
		else
			Aspect = 4.0f / 3.0f;	// Aspect ratio.

		recalculateProjectionMatrix();
		recalculateViewArea();
	}


	//! Disables or enables the camera to get key or mouse inputs.
	void CCameraSceneNode::setInputReceiverEnabled(bool enabled)
	{
		InputReceiverEnabled = enabled;
	}


	//! Returns if the input receiver of the camera is currently enabled.
	bool CCameraSceneNode::isInputReceiverEnabled() const
	{
		
		return InputReceiverEnabled;
	}


	//! Sets the projection matrix of the camera.
	/** The Matrix4 class has some methods
	to build a projection matrix. e.g: Matrix4::buildProjectionMatrixPerspectiveFovLH
	\param projection: The new projection matrix of the camera. */
	void CCameraSceneNode::setProjectionMatrix(const Matrix4& projection, bool isOrthogonal)
	{
		IsOrthogonal = isOrthogonal;
		ViewArea.getTransform(ETS_PROJECTION) = projection;
	}


	//! Gets the current projection matrix of the camera
	//! \return Returns the current projection matrix of the camera.
	const Matrix4& CCameraSceneNode::getProjectionMatrix() const
	{
		return ViewArea.getTransform(ETS_PROJECTION);
	}


	//! Gets the current view matrix of the camera
	//! \return Returns the current view matrix of the camera.
	const Matrix4& CCameraSceneNode::getViewMatrix() const
	{
		return ViewArea.getTransform(ETS_VIEW);
	}


	//! Sets a custom view matrix affector. The matrix passed here, will be
	//! multiplied with the view matrix when it gets updated.
	//! This allows for custom camera setups like, for example, a reflection camera.
	/** \param affector: The affector matrix. */
	void CCameraSceneNode::setViewMatrixAffector(const Matrix4& affector)
	{
		Affector = affector;
	}


	//! Gets the custom view matrix affector.
	const Matrix4& CCameraSceneNode::getViewMatrixAffector() const
	{
		return Affector;
	}


	//! It is possible to send mouse and key events to the camera. Most cameras
	//! may ignore this input, but camera scene nodes which are created for
	//! example with scene::ISceneManager::addMayaCameraSceneNode or
	//! scene::ISceneManager::addFPSCameraSceneNode, may want to get this input
	//! for changing their position, look at target or whatever.
	bool CCameraSceneNode::OnEvent(const SEvent& event)
	{
		if (!InputReceiverEnabled)
			return false;

		// send events to event receiving animators

		ISceneNodeAnimatorList::iterator ait = Animators.begin();

		for (; ait != Animators.end(); ++ait)
			if ((*ait)->isEventReceiverEnabled() && (*ait)->OnEvent(event))
				return true;

		// if nobody processed the event, return false
		return false;
	}


	//! sets the look at target of the camera
	//! \param pos: Look at target of the camera.
	void CCameraSceneNode::setTarget(const Vector3& pos)
	{
		Target = pos;

		if (TargetAndRotationAreBound)
		{
			const Vector3 toTarget = Target - getAbsolutePosition();
			ISceneNode::setRotation(toTarget.getHorizontalAngle());
		}
	}


	//! Sets the rotation of the node.
	/** This only modifies the relative rotation of the node.
	If the camera's target and rotation are bound ( @see bindTargetAndRotation() )
	then calling this will also change the camera's target to match the rotation.
	\param rotation New rotation of the node in degrees. */
	void CCameraSceneNode::setRotation(const Vector3& rotation)
	{
		if (TargetAndRotationAreBound)
			Target = getAbsolutePosition() + rotation.rotationToDirection();

		ISceneNode::setRotation(rotation);
	}


	//! Gets the current look at target of the camera
	//! \return Returns the current look at target of the camera
	const Vector3& CCameraSceneNode::getTarget() const
	{
		return Target;
	}


	//! sets the up vector of the camera
	//! \param pos: New upvector of the camera.
	void CCameraSceneNode::setUpVector(const Vector3& pos)
	{
		UpVector = pos;
	}


	//! Gets the up vector of the camera.
	//! \return Returns the up vector of the camera.
	const Vector3& CCameraSceneNode::getUpVector() const
	{
		return UpVector;
	}


	Real CCameraSceneNode::getNearValue() const
	{
		return ZNear;
	}


	Real CCameraSceneNode::getFarValue() const
	{
		return ZFar;
	}


	Real CCameraSceneNode::getAspectRatio() const
	{
		return Aspect;
	}


	Real CCameraSceneNode::getFOV() const
	{
		return Fovy;
	}


	void CCameraSceneNode::setNearValue(Real f)
	{
		ZNear = f;
		recalculateProjectionMatrix();
	}


	void CCameraSceneNode::setFarValue(Real f)
	{
		ZFar = f;
		recalculateProjectionMatrix();
	}


	void CCameraSceneNode::setAspectRatio(Real f)
	{
		Aspect = f;
		recalculateProjectionMatrix();
	}


	void CCameraSceneNode::setFOV(Real f)
	{
		Fovy = f;
		recalculateProjectionMatrix();
	}


	void CCameraSceneNode::recalculateProjectionMatrix()
	{
		ViewArea.getTransform(ETS_PROJECTION).buildProjectionMatrixPerspectiveFovLH(Fovy, Aspect, ZNear, ZFar);
	}


	//! prerender
	void CCameraSceneNode::OnRegisterSceneNode()
	{
		if (SceneManager->getActiveCamera() == this)
			SceneManager->registerNodeForRendering(this, ESNRP_CAMERA);

		ISceneNode::OnRegisterSceneNode();
	}


	//! render
	void CCameraSceneNode::render()
	{
		Vector3 pos = getAbsolutePosition();
		Vector3 tgtv = Target - pos;
		tgtv.normalize();

		// if upvector and vector to the target are the same, we have a
		// problem. so solve this problem:
		Vector3 up = UpVector;
		up.normalize();

		FLOAT32 dp = tgtv.dotProduct(up);

		if (Math::equals(Math::Abs(dp), 1.f))
		{
			up.x += 0.5f;
		}

		ViewArea.getTransform(ETS_VIEW).buildCameraLookAtMatrixLH(pos, Target, up);
		//ViewArea.getTransform(ETS_VIEW) *= Affector;
		ViewArea.getTransform(ETS_VIEW) = Affector * ViewArea.getTransform(ETS_VIEW);
		recalculateViewArea();

		IVideoDriver* driver = SceneManager->getVideoDriver();
		if (driver)
		{
			driver->setTransform(ETS_PROJECTION, ViewArea.getTransform(ETS_PROJECTION));
			driver->setTransform(ETS_VIEW, ViewArea.getTransform(ETS_VIEW));
		}
	}


	//! returns the axis aligned bounding box of this node
	const AxisAlignedBox& CCameraSceneNode::getBoundingBox() const
	{
		return ViewArea.getBoundingBox();
	}


	//! returns the view frustum. needed sometimes by bsp or lod render nodes.
	const SViewFrustum* CCameraSceneNode::getViewFrustum() const
	{
		return &ViewArea;
	}


	void CCameraSceneNode::recalculateViewArea()
	{
		ViewArea.cameraPosition = getAbsolutePosition();

		Matrix4 m(Matrix4::ZERO);
		m.setbyproduct_nocheck(ViewArea.getTransform(ETS_PROJECTION),
			ViewArea.getTransform(ETS_VIEW));
		ViewArea.setFrom(m);
	}


	//! Writes attributes of the scene node.
	void CCameraSceneNode::serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const
	{
		ICameraSceneNode::serializeAttributes(out, options);

		out->addVector3d("Target", Target);
		out->addVector3d("UpVector", UpVector);
		out->addFloat("Fovy", Fovy);
		out->addFloat("Aspect", Aspect);
		out->addFloat("ZNear", ZNear);
		out->addFloat("ZFar", ZFar);
		out->addBool("Binding", TargetAndRotationAreBound);
		out->addBool("ReceiveInput", InputReceiverEnabled);
	}

	//! Reads attributes of the scene node.
	void CCameraSceneNode::deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options)
	{
		ICameraSceneNode::deserializeAttributes(in, options);

		Target = in->getAttributeAsVector3d("Target");
		UpVector = in->getAttributeAsVector3d("UpVector");
		Fovy = in->getAttributeAsFloat("Fovy");
		Aspect = in->getAttributeAsFloat("Aspect");
		ZNear = in->getAttributeAsFloat("ZNear");
		ZFar = in->getAttributeAsFloat("ZFar");
		TargetAndRotationAreBound = in->getAttributeAsBool("Binding");
		if (in->findAttribute("ReceiveInput"))
			InputReceiverEnabled = in->getAttributeAsBool("InputReceiverEnabled");

		recalculateProjectionMatrix();
		recalculateViewArea();
	}


	//! Set the binding between the camera's rotation adn target.
	void CCameraSceneNode::bindTargetAndRotation(bool bound)
	{
		TargetAndRotationAreBound = bound;
	}


	//! Gets the binding between the camera's rotation and target.
	bool CCameraSceneNode::getTargetAndRotationBinding(void) const
	{
		return TargetAndRotationAreBound;
	}


	//! Creates a clone of this scene node and its children.
	ISceneNode* CCameraSceneNode::clone(ISceneNode* newParent, ISceneManager* newManager)
	{
		ICameraSceneNode::clone(newParent, newManager);

		if (!newParent)
			newParent = Parent;
		if (!newManager)
			newManager = SceneManager;

		CCameraSceneNode* nb = new CCameraSceneNode(newParent,
			newManager, ID, RelativeTranslation, Target);

		nb->ISceneNode::cloneMembers(this, newManager);
		nb->ICameraSceneNode::cloneMembers(this);

		nb->Target = Target;
		nb->UpVector = UpVector;
		nb->Fovy = Fovy;
		nb->Aspect = Aspect;
		nb->ZNear = ZNear;
		nb->ZFar = ZFar;
		nb->ViewArea = ViewArea;
		nb->Affector = Affector;
		nb->InputReceiverEnabled = InputReceiverEnabled;
		nb->TargetAndRotationAreBound = TargetAndRotationAreBound;

		if (newParent)
			nb->drop();
		return nb;
	}
}