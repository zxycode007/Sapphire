#ifndef _SAPPHIRE_C_VIDEO_MODEL_LIST_
#define _SAPPHIRE_C_VIDEO_MODEL_LIST_

#include "SapphireIVideoModeList.h"
#include "SapphireDimension2D.h"


namespace Sapphire
{
	class CVideoModeList : public IVideoModeList
	{
	public:

		//! constructor
		CVideoModeList();

		//! Gets amount of video modes in the list.
		virtual SINT32 getVideoModeCount() const;

		//! Returns the screen size of a video mode in pixels.
		virtual dimension2d<UINT32> getVideoModeResolution(SINT32 modeNumber) const;

		//! Returns the screen size of an optimal video mode in pixels.
		virtual dimension2d<UINT32> getVideoModeResolution(const dimension2d<UINT32>& minSize, const dimension2d<UINT32>& maxSize) const;

		//! Returns the pixel depth of a video mode in bits.
		virtual SINT32 getVideoModeDepth(SINT32 modeNumber) const;

		//! Returns current desktop screen resolution.
		virtual const dimension2d<UINT32>& getDesktopResolution() const;

		//! Returns the pixel depth of a video mode in bits.
		virtual SINT32 getDesktopDepth() const;

		//! adds a new mode to the list
		void addMode(const dimension2d<UINT32>& size, SINT32 depth);

		void setDesktop(SINT32 desktopDepth, const dimension2d<UINT32>& desktopSize);

	private:

		struct SVideoMode
		{
			dimension2d<UINT32> size;
			SINT32 depth;

			bool operator==(const SVideoMode& other) const
			{
				return size == other.size && depth == other.depth;
			}

			bool operator <(const SVideoMode& other) const
			{
				return (size.Width < other.size.Width ||
					(size.Width == other.size.Width &&
					size.Height < other.size.Height) ||
					(size.Width == other.size.Width &&
					size.Height == other.size.Height &&
					depth < other.depth));
			}
		};

		vector<SVideoMode>::type VideoModes;
		SVideoMode Desktop;
	};
}

#endif