#include "SapphireCVideoModelList.h"
#include "SapphireMath.h"

namespace Sapphire
{
	//! constructor
	CVideoModeList::CVideoModeList()
	{
#ifdef _DEBUG
		setDebugName("CVideoModeList");
#endif

		Desktop.depth = 0;
		Desktop.size = dimension2d<UINT32>(0, 0);
	}


	void CVideoModeList::setDesktop(SINT32 desktopDepth, const dimension2d<UINT32>& desktopSize)
	{
		Desktop.depth = desktopDepth;
		Desktop.size = desktopSize;
	}


	//! Gets amount of video modes in the list.
	SINT32 CVideoModeList::getVideoModeCount() const
	{
		return (SINT32)VideoModes.size();
	}


	//! Returns the screen size of a video mode in pixels.
	dimension2d<UINT32> CVideoModeList::getVideoModeResolution(SINT32 modeNumber) const
	{
		if (modeNumber < 0 || modeNumber >(SINT32)VideoModes.size())
			return dimension2d<UINT32>(0, 0);

		return VideoModes[modeNumber].size;
	}


	dimension2d<UINT32> CVideoModeList::getVideoModeResolution(
		const dimension2d<UINT32>& minSize,
		const dimension2d<UINT32>& maxSize) const
	{
		UINT32 best = VideoModes.size();
		// if only one or no mode
		if (best<2)
			return getVideoModeResolution(0);

		UINT32 i;
		for (i = 0; i<VideoModes.size(); ++i)
		{
			if (VideoModes[i].size.Width >= minSize.Width &&
				VideoModes[i].size.Height >= minSize.Height &&
				VideoModes[i].size.Width <= maxSize.Width &&
				VideoModes[i].size.Height <= maxSize.Height)
				best = i;
		}
		// we take the last one found, the largest one fitting
		if (best<VideoModes.size())
			return VideoModes[best].size;
		const UINT32 minArea = minSize.getArea();
		const UINT32 maxArea = maxSize.getArea();
		UINT32 minDist = 0xffffffff;
		best = 0;
		for (i = 0; i<VideoModes.size(); ++i)
		{
			const UINT32 area = VideoModes[i].size.getArea();
			const UINT32 dist = Math::_min(abs(int(minArea - area)), abs(int(maxArea - area)));
			if (dist<minDist)
			{
				minDist = dist;
				best = i;
			}
		}
		return VideoModes[best].size;
	}


	//! Returns the pixel depth of a video mode in bits.
	SINT32 CVideoModeList::getVideoModeDepth(SINT32 modeNumber) const
	{
		if (modeNumber < 0 || modeNumber >(SINT32)VideoModes.size())
			return 0;

		return VideoModes[modeNumber].depth;
	}


	//! Returns current desktop screen resolution.
	const dimension2d<UINT32>& CVideoModeList::getDesktopResolution() const
	{
		return Desktop.size;
	}


	//! Returns the pixel depth of a video mode in bits.
	SINT32 CVideoModeList::getDesktopDepth() const
	{
		return Desktop.depth;
	}


	//! adds a new mode to the list
	void CVideoModeList::addMode(const dimension2d<UINT32>& size, SINT32 depth)
	{
		SVideoMode m;
		m.depth = depth;
		m.size = size;

		for (UINT32 i = 0; i<VideoModes.size(); ++i)
		{
			if (VideoModes[i] == m)
				return;
		}

		VideoModes.push_back(m);
		std::stable_sort(VideoModes.begin(), VideoModes.end());
		//VideoModes.sort();
	}
}
