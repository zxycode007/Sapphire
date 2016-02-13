#include "SapphireCSceneNodeAnimatorFlyStraight.h"


namespace Sapphire
{

	//! constructor
	CSceneNodeAnimatorFlyStraight::CSceneNodeAnimatorFlyStraight(const Vector3& startPoint,
		const Vector3& endPoint, UINT32 timeForWay,
		bool loop, UINT32 now, bool pingpong)
		: ISceneNodeAnimatorFinishing(now + timeForWay),
		Start(startPoint), End(endPoint), TimeFactor(0.0f), StartTime(now),
		TimeForWay(timeForWay), Loop(loop), PingPong(pingpong)
	{
#ifdef _DEBUG
		setDebugName("CSceneNodeAnimatorFlyStraight");
#endif

		recalculateIntermediateValues();
	}


	void CSceneNodeAnimatorFlyStraight::recalculateIntermediateValues()
	{
		Vector = End - Start;
		TimeFactor = (FLOAT32)Vector.length() / TimeForWay;
		Vector.normalise();
	}


	//! animates a scene node
	void CSceneNodeAnimatorFlyStraight::animateNode(ISceneNode* node, UINT32 timeMs)
	{
		if (!node)
			return;

		UINT32 t = (timeMs - StartTime);

		Vector3 pos;

		if (!Loop && !PingPong && t >= TimeForWay)
		{
			pos = End;
			HasFinished = true;
		}
		else if (!Loop && PingPong && t >= TimeForWay * 2.f)
		{
			pos = Start;
			HasFinished = true;
		}
		else
		{
			FLOAT32 phase = fmodf((FLOAT32)t, (FLOAT32)TimeForWay);
			Vector3 rel = Vector * phase * TimeFactor;
			const bool pong = PingPong && fmodf((FLOAT32)t, (FLOAT32)TimeForWay*2.f) >= TimeForWay;

			if (!pong)
			{
				pos += Start + rel;
			}
			else
			{
				pos = End - rel;
			}
		}

		node->setPosition(pos);
	}


	//! Writes attributes of the scene node animator.
	void CSceneNodeAnimatorFlyStraight::serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const
	{
		out->addVector3d("Start", Start);
		out->addVector3d("End", End);
		out->addInt("TimeForWay", TimeForWay);
		out->addBool("Loop", Loop);
		out->addBool("PingPong", PingPong);
	}


	//! Reads attributes of the scene node animator.
	void CSceneNodeAnimatorFlyStraight::deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options)
	{
		Start = in->getAttributeAsVector3d("Start");
		End = in->getAttributeAsVector3d("End");
		TimeForWay = in->getAttributeAsInt("TimeForWay");
		Loop = in->getAttributeAsBool("Loop");
		PingPong = in->getAttributeAsBool("PingPong");

		recalculateIntermediateValues();
	}


	ISceneNodeAnimator* CSceneNodeAnimatorFlyStraight::createClone(ISceneNode* node, ISceneManager* newManager)
	{
		CSceneNodeAnimatorFlyStraight * newAnimator =
			new CSceneNodeAnimatorFlyStraight(Start, End, TimeForWay, Loop, StartTime, PingPong);

		return newAnimator;
	}
}