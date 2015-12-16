#ifndef _SAPPHIRE_IVIDEO_MODE_LIST_
#define _SAPPHIRE_IVIDEO_MODE_LIST_

#include "SapphirePrerequisites.h"
#include "SapphireIReferenceCounter.h"
#include "SapphireDimension2D.h"

namespace Sapphire
{
	//! 一个所有可以视频模式的列表
	/** 你可以通过Device::getVideoModeList()方法来获取一个列表。如果你现在搞不清楚，
	因为在用视频模式创建Device之前，要能够获取视频列表。
	 */
	class IVideoModeList : public virtual IReferenceCounter
	{
	public:

		//! 获取列表的视频模式的数量
		/** \return 返回视频模式的数量 */
		virtual SINT32 getVideoModeCount() const = 0;

		//! 获取视频模式的窗口的像素大小
		/** \param modeNumber: 视频模式索引基于0开始
		\return 视频模式的窗口的像素大小*/
		virtual  dimension2d<UINT32> getVideoModeResolution(SINT32 modeNumber) const = 0;

		//! 获取一个某些条件约束的屏幕大小
		/** \param minSize: 允许的最小尺寸
		\param maxSize: 允许的最大尺寸
		\return 匹配这些条件的的视频模式的窗口的像素大小*/
		virtual  dimension2d<UINT32> getVideoModeResolution(const  dimension2d<UINT32>& minSize, const  dimension2d<UINT32>& maxSize) const = 0;

		//! 获取视频模式的像素位深
		/** \param modeNumber: 视频模式索引基于0开始
		\return 指定视频模式的每个像素色位深 */
		virtual SINT32 getVideoModeDepth(SINT32 modeNumber) const = 0;

		//! 获取当前桌面屏幕分辨率
		/** \return 当前桌面视频模式的像素大小*/
		virtual const  dimension2d<UINT32>& getDesktopResolution() const = 0;

		//! 获取当前视频模式像素位深
		/** \return指定当前桌面视频模式的每个像素色位深 */
		virtual SINT32 getDesktopDepth() const = 0;
	};
}

#endif