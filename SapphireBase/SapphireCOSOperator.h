#ifndef _SAPPHIRE_C_OS_OPERATOR_
#define _SAPPHIRE_C_OS_OPERATOR_

#include "SapphireIOSOperator.h"

namespace Sapphire
{
	class CIrrDeviceLinux;

	//! The Operating system operator provides operation system specific methods and informations.
	// 操作系统操作器提供操作系统特定的方法和信息
	class COSOperator : public IOSOperator
	{
	public:

		// constructor
#if defined(_SAPPHIRE_COMPILE_WITH_X11_DEVICE_)
		COSOperator(const stringc& osversion, CIrrDeviceLinux* device);
#endif
		COSOperator(const String& osversion);

		//! returns the current operation system version as string.
		// 返回操作系统版本字符串
		virtual const String& getOperatingSystemVersion() const;

		//! copies text to the clipboard
		//复制文字到剪切板
		virtual void copyToClipboard(const c8* text) const;

		//! gets text from the clipboard
		//从剪切板获取文字
		//! \return Returns 0 if no string is in there.
		virtual const c8* getTextFromClipboard() const;

		//! gets the processor speed in megahertz
		//获取处理器速度频率
		//! \param Mhz:
		//! \return Returns true if successful, false if not
		virtual bool getProcessorSpeedMHz(UINT32* MHz) const;

		//! gets the total and available system RAM in kB
		// 获取可用的系统内存   单位KB
		//! \param Total: will contain the total system memory
		//! \param Avail: will contain the available memory
		//! \return Returns true if successful, false if not
		virtual bool getSystemMemory(UINT32* Total, UINT32* Avail) const;

	private:

		String OperatingSystem;

#if defined(_SAPPHIRE_COMPILE_WITH_X11_DEVICE_)
		CSapphireDeviceLinux * SapphireDeviceLinux;
#endif

	};
}

#endif