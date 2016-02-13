#include "SapphireCSceneNodeAnimatorFollowSpline.h"


namespace Sapphire
{

	//! constructor
	CSceneNodeAnimatorFollowSpline::CSceneNodeAnimatorFollowSpline(UINT32 time,
		const vector<Vector3>::type& points, FLOAT32 speed,
		FLOAT32 tightness, bool loop, bool pingpong)
		: ISceneNodeAnimatorFinishing(0), Points(points), Speed(speed), Tightness(tightness), StartTime(time)
		, Loop(loop), PingPong(pingpong)
	{
#ifdef _DEBUG
		setDebugName("CSceneNodeAnimatorFollowSpline");
#endif
	}


	inline SINT32 CSceneNodeAnimatorFollowSpline::clamp(SINT32 idx, SINT32 size)
	{
		return (idx<0 ? size + idx : (idx >= size ? idx - size : idx));
	}


	//! animates a scene node
	void CSceneNodeAnimatorFollowSpline::animateNode(ISceneNode* node, UINT32 timeMs)
	{
		if (!node)
			return;

		const UINT32 pSize = Points.size();
		if (pSize == 0)
		{
			if (!Loop)
				HasFinished = true;
			return;
		}
		if (pSize == 1)
		{
			if (timeMs > StartTime)
			{
				node->setPosition(Points[0]);
				if (!Loop)
					HasFinished = true;
			}
			return;
		}

		const FLOAT32 dt = ((timeMs - StartTime) * Speed * 0.001f);
		const SINT32 unwrappedIdx = floor32(dt);
		if (!Loop && unwrappedIdx >= (SINT32)pSize - 1)
		{
			node->setPosition(Points[pSize - 1]);
			HasFinished = true;
			return;
		}
		const bool pong = PingPong && (unwrappedIdx / (pSize - 1)) % 2;
		const FLOAT32 u = pong ? 1.f - fract(dt) : fract(dt);
		const SINT32 idx = pong ? (pSize - 2) - (unwrappedIdx % (pSize - 1))
			: (PingPong ? unwrappedIdx % (pSize - 1)
			: unwrappedIdx % pSize);
		//const FLOAT32 u = 0.001f * fmodf( dt, 1000.0f );

		const Vector3& p0 = Points[clamp(idx - 1, pSize)];
		const Vector3& p1 = Points[clamp(idx + 0, pSize)]; // starting point
		const Vector3& p2 = Points[clamp(idx + 1, pSize)]; // end point
		const Vector3& p3 = Points[clamp(idx + 2, pSize)];

		// hermite polynomials
		const FLOAT32 h1 = 2.0f * u * u * u - 3.0f * u * u + 1.0f;
		const FLOAT32 h2 = -2.0f * u * u * u + 3.0f * u * u;
		const FLOAT32 h3 = u * u * u - 2.0f * u * u + u;
		const FLOAT32 h4 = u * u * u - u * u;

		// tangents
		const Vector3 t1 = (p2 - p0) * Tightness;
		const Vector3 t2 = (p3 - p1) * Tightness;

		// interpolated point
		node->setPosition(p1 * h1 + p2 * h2 + t1 * h3 + t2 * h4);
	}


	//! Writes attributes of the scene node animator.
	void CSceneNodeAnimatorFollowSpline::serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const
	{
		out->addFloat("Speed", Speed);
		out->addFloat("Tightness", Tightness);
		out->addBool("Loop", Loop);
		out->addBool("PingPong", PingPong);

		UINT32 count = Points.size();

		if (options && (options->Flags & EARWF_FOR_EDITOR))
		{
			// add one point in addition when serializing for editors
			// to make it easier to add points quickly
			count += 1;
		}

		for (UINT32 i = 0; i<count; ++i)
		{
			String tname = "Point";
			tname += (int)(i + 1);

			out->addVector3d(tname.c_str(), i<Points.size() ? Points[i] : Vector3(0, 0, 0));
		}
	}


	//! Reads attributes of the scene node animator.
	void CSceneNodeAnimatorFollowSpline::deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options)
	{
		Speed = in->getAttributeAsFloat("Speed");
		Tightness = in->getAttributeAsFloat("Tightness");
		Loop = in->getAttributeAsBool("Loop");
		PingPong = in->getAttributeAsBool("PingPong");
		Points.clear();

		for (UINT32 i = 1; true; ++i)
		{
			String pname = "Point";
			pname += i;

			if (in->existsAttribute(pname.c_str()))
				Points.push_back(in->getAttributeAsVector3d(pname.c_str()));
			else
				break;
		}

		// remove last point if double entry from editor
		if (options && (options->Flags & EARWF_FOR_EDITOR) &&
			//Points.size() > 2 && Points.getLast() == Vector3(0, 0, 0))
			Points.size() > 2 && Points.back() == Vector3(0, 0, 0))
		{
			//Points.erase(Points.size() - 1);
			Points.erase(Points.end() - 1);

			//if (Points.size() > 2 && Points.getLast() == Vector3(0, 0, 0))
			if (Points.size() > 2 && Points.back() == Vector3(0, 0, 0))
				//Points.erase(Points.size() - 1);
				Points.erase(Points.end() - 1);
		}
	}


	ISceneNodeAnimator* CSceneNodeAnimatorFollowSpline::createClone(ISceneNode* node, ISceneManager* newManager)
	{
		CSceneNodeAnimatorFollowSpline * newAnimator =
			new CSceneNodeAnimatorFollowSpline(StartTime, Points, Speed, Tightness);

		return newAnimator;
	}
}