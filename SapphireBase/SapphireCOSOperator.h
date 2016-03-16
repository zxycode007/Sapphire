#ifndef _SAPPHIRE_C_OS_OPERATOR_
#define _SAPPHIRE_C_OS_OPERATOR_

#include "SapphireIOSOperator.h"

namespace Sapphire
{
	class CIrrDeviceLinux;

	//! The Operating system operator provides operation system specific methods and informations.
	// ����ϵͳ�������ṩ����ϵͳ�ض��ķ�������Ϣ
	class COSOperator : public IOSOperator
	{
	public:

		// constructor
#if defined(_SAPPHIRE_COMPILE_WITH_X11_DEVICE_)
		COSOperator(const stringc& osversion, CIrrDeviceLinux* device);
#endif
		COSOperator(const String& osversion);

		//! returns the current operation system version as string.
		// ���ز���ϵͳ�汾�ַ���
		virtual const String& getOperatingSystemVersion() const;

		//! copies text to the clipboard
		//�������ֵ����а�
		virtual void copyToClipboard(const c8* text) const;

		//! gets text from the clipboard
		//�Ӽ��а��ȡ����
		//! \return Returns 0 if no string is in there.
		virtual const c8* getTextFromClipboard() const;

		//! gets the processor speed in megahertz
		//��ȡ�������ٶ�Ƶ��
		//! \param Mhz:
		//! \return Returns true if successful, false if not
		virtual bool getProcessorSpeedMHz(UINT32* MHz) const;

		//! gets the total and available system RAM in kB
		// ��ȡ���õ�ϵͳ�ڴ�   ��λKB
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