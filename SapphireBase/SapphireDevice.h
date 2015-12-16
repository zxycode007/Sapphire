#ifndef _SAPPHIRE_DEVICE_
#define _SAPPHIRE_DEVICE_

#include "SapphirePrerequisites.h"
#include "SapphireEDriverTypes.h"
#include "SapphireEDeviceTypes.h"
#include "SapphireIEventRecevier.h"
#include "SapphireColorValue.h"




namespace Sapphire
{

	class IVideoDriver;
	class IFileSystem;
	class IGUIEnvironment;
	class ICursorControl;
	class ILogger;
	class IVideoModeList;
	class IOSOperator;
	class ITimer;
	class IRandomizer;
	class IEventReceiver;
	class SJoystickInfo;
	class ISceneManager;

	//! �豸. �����ͨ������createDevice() �� createDeviceEx()�����豸
	/** ������������Ҫ���ࡣ�������һ��������ʵ��ָ�룬��Ϳ��Է�����������ж�������������κ�ʱ��ֻ����һ��ʵ����
	*/
	class Device
	{
	public:

		//! ��������豸
		/** ͨ������ITimer����tick�������������ʱ��
		���ܹ�ͨ������ITimer::stop()��ֹ������ ITimer::start()֮ǰ����Device����run����֮��
		����豸��ɾ��������false�� ͨ������ķ���ʹ������

		while(device->run())
		{
		// draw everything here
		}
		�������Ҫ����ڴ��ڷǼ���״̬�豸�����κ����飨�Ƽ�������΢�޸��´��룬����isWindowActive()
		ע�⣺�������һ���ⲿ���Զ��崰�������У�����Device->run()���ᵼ�����涪��windows�ڲ���Ϣ
		�����Ҫ�����Լ��Զ���Ĵ������������棬�����Լ�ʵ�ֵ���Ϣѭ��GetMessage��DispatchMessage�Ͳ���ʲô�����������������
		����ע�⣺���治��ʰȡ�û����롣��irr::SIrrlichtCreationParameters::WindowId������Ϣ������
		*/
		virtual bool run() = 0;

		//! ��������豸��ʱ��ָͣ������������������
		/** �⽫���ʹ�����ʹ���ʶ����ᵼ��������Ҫ���ܶ�ʧ*/
		virtual void yield() = 0;

		//! ��ִͣ�в�����������������һ���ض�����ʱ��
		/** ����ȴ�������������ʱ�䣬Ȼ��˯�߽�����ϡ�
		\param timeMs: ˯��ʱ��ĺ�����
		\param pauseTimer: ���Ϊtrue����˯�ߵ�ʱ����ͣ�豸
		*/
		virtual void sleep(UINT32 timeMs, bool pauseTimer = false) = 0;

		//! �ṩ���ʻ���3D��2D���������Ƶ�豸
		/** \return ��Ƶ�豸��ָ��. */
		virtual IVideoDriver* getVideoDriver() = 0;

		//! �ṩ���������ļ�ϵͳ��ָ��
		/** \return �ļ�ϵͳ��ָ�� */
		virtual IFileSystem* getFileSystem() = 0;

		//! �ṩ2D GUI�����ķ���
		/** \return GUI������ָ�� */
		virtual IGUIEnvironment* getGUIEnvironment() = 0;

		//! �ṩ�����������ķ���
		/** \return ����������ָ�� */
		virtual ISceneManager* getSceneManager() = 0;

		//! �ṩ����������
		/** \return ����α���ƽӿ�ָ��*/
		virtual ICursorControl* getCursorControl() = 0;

		//! �ṩ��Χ��Ϣ��־�ӿ�
		/** \return ��־��ָ�� */
		virtual ILogger* getLogger() = 0;

		//! ��ȡ������Ƶģʽ�б�
		/** ����㲻�����������Ҫ��һ����Ƶģʽ����һ��Device֮ǰҪ��ȡ��Ƶģʽ�б�
		û�б�Ҫ��EDT_DIRECT3D8,EDT_OPENGL �� EDT_SOFTWARE������û�������ģ� EDT_NULL���ڡ�
		\return ͨ��gfx��������֧����Ƶģʽ�б��ָ��*/
		virtual IVideoModeList* getVideoModeList() = 0;

		//! �ṩ���ʲ���ϵͳ�Ĳ�������
		/** �ṩ����ϵͳһЩ�����ķ�������ȡϵͳ�ض���Ϣ����һЩϵͳ�ض�������
		���ü��а彻�����ݻ��߶�ȡ����ϵͳ�汾
		\return ����ϵͳ������ָ��*/
		virtual IOSOperator* getOSOperator() = 0;

		//! �ṩһ�����������timer
		/** ͨ����������������ܹ�����ϵͳʱ�䣬�����Ա�����
		\return ָ��ITimer�Ķ��� */
		virtual ITimer* getTimer() = 0;

		//! �ṩ�������浱ǰ�������������
		/** \return ָ��IRandomizer�����ָ�� */
		virtual IRandomizer* getRandomizer() const = 0;

		//! ����һ���µ�randomizer.
		/** \param r ָ��IRandomizer�����ָ�롣�������һ��grab()����ô����Ҫ�ͷ���ֻ��ͨ����һ��setRandomizer����
		���豸���� */
		virtual void setRandomizer(IRandomizer* r) = 0;

		//! ����һ���µ�Ĭ��randomizer
		/**
		\return ָ��Ĭ��IRandomizer�����ָ�� */
		virtual IRandomizer* createDefaultRandomizer() const = 0;

		//! ���ô��ڵı���
		/** \param text: ���ڱ���λ�� */
		virtual void setWindowCaption(const wchar_t* text) = 0;

		//! ������������Ƿ��Ǽ��� 
		/** ���������ڲ��Ǽ���ģ�����Ҫ���л��ơ��������������û�м���ʱ���㲻��Ҫ�����κλ��ƣ�
		ͨ���±߷���������Ļ���ѭ��
		
		while(device->run())
		{
		if (device->isWindowActive())
		{
		// draw everything here
		}
		else
		device->yield();
		}
		\return ������ڱ������true */
		virtual bool isWindowActive() const = 0;

		//! ����Irrlicht�����Ƿ��н���
		/** \return ��������н��㷵��true */
		virtual bool isWindowFocused() const = 0;

		//! ��ⴰ���Ƿ���С��
		/** \return �������С������true */
		virtual bool isWindowMinimized() const = 0;

		//! ��ⴰ���Ƿ�ȫ��Ļ
		/** \return �����ȫ��Ļ����true */
		virtual bool isFullscreen() const = 0;

		//! ��ȡ������ڵĵ�ǰ��ɫ��ʽ
		/** \return ���ڵ���ɫ��ʽ */
		virtual ECOLOR_FORMAT getColorFormat() const = 0;

		//! ֪ͨ�豸��Ӧ�ùر����Լ�
		/**Device::run() �� closeDevice()���ú�᷵��false . */
		virtual void closeDevice() = 0;

		//! ��ȡ����汾��Ϣ
		/** ���ص��ַ���
		*/
		virtual const String getVersion() const = 0;

		//! ����һ���µ��û��¼���������������������¼�
		/** 
		\param receiver Ҫ��ʹ���µ��¼�������. */
		virtual void setEventReceiver(IEventReceiver* receiver) = 0;

		//! �ṩ������ǰ�¼��Ľ�����
		/** \return ָ��ǰ�����������ָ�� */
		virtual IEventReceiver* getEventReceiver() = 0;

		//! ����һ���û����������絽����
		/** ��ͨ�����Ǳ���ģ�Ȼ���������ʹ���Զ��������⣬�����ֱ����룬����Ҫʹ������׳�key�����������¼������档
		�ڲ����������ֻ��ί������¼���һ����������������GUI����
		 */
		virtual bool postEventFromUser(const SEvent& event) = 0;

		//! ����������ܳ���������
		/** �������ΪNULL�� ����������������GetSceneManager�������أ�����������
		\param sceneManager Ҫʹ�õ��³���������. */
		virtual void setInputReceivingSceneManager(ISceneManager* sceneManager) = 0;

		//! �����Ƿ��ܹ����ڱ��ڴ���ģʽ��������С
		/** Ĭ����false���������ֻ�ܹ����ڴ���ģʽ
		 */
		virtual void setResizable(bool resize = false) = 0;

		//! ������ܴ�����С��
		virtual void minimizeWindow() = 0;

		//! ������ܴ������
		virtual void maximizeWindow() = 0;

		//! ������ָܻ�����������С
		virtual void restoreWindow() = 0;

		//! �����ֱ���Ϊ���Ǵ����¼�
		/** Ŀǰ�����ֱ�֧�֣���Ĭ������²��ܴ����ֱ��¼���Ҫ֧���ֱ���Ϣ��Ҫ��������֧�֡�����������������ֱ�֧�֣����ҽ���SJoystickEvent�¼�
		\param joystickInfo , ���ص��ֱ���Ϣ���⽫����ÿһ���ҵ��ͼ�����ֱ�����
		\return true ���������豸�ϱ�֧�ֺ�_IRR_COMPILE_WITH_JOYSTICK_EVENTS_�����壬����True
		*/
		virtual bool activateJoysticks(std::vector<SJoystickInfo>& joystickInfo) = 0;

		//! ������ʾ�ĵ�ǰ��gammaֵ
		virtual bool setGammaRamp(Real red, Real green, Real blue,
			Real relativebrightness, Real relativecontrast) = 0;

		//! ��ȡ��ǰ��ʾ��gammaֵ
		virtual bool getGammaRamp(Real &red, Real &green, Real &blue,
			Real &brightness, Real &contrast) = 0;

		//! �Ƴ���ϵͳ��Ϣѭ���д�������Ϣ
		/** �������ͨ��������Ϣ�������˺ܳ�һ��ʱ�䣬���磺����ȡ�˺ܴ�һ��������
		��������Ϣѭ����֤�û���GUI�ڼ��������������-�򰴼�������Դ����� 
		��ĿǰΪֹ���������Ϣ�ᱻ���
		Win32: ���еļ��̺������Ϣ 
		Linux: ���еļ��̺������Ϣ
		���������豸���������֧��
		 */
		virtual void clearSystemMessages() = 0;

		//! ��ȡ�豸������
		/** . */
		virtual E_DEVICE_TYPE getType() const = 0;

		//! ����������֧��һ����������
		/** �ж������Ƿ���ã����÷���true������false*/
		static bool isDriverSupported(E_DRIVER_TYPE driver)
		{
			switch (driver)
			{
			case EDT_NULL:
				return true;
			case EDT_SOFTWARE:
#if (SAPPHIRE_3D_API == SAPPHIRE_WITH_SOFTWAR)
				return true;
#else
				return false;
#endif
			case EDT_BURNINGSVIDEO:
#if (SAPPHIRE_3D_API==SAPPHIRE_WITH_BURNINGSVIDEO)
				return true;
#else
				return false;
#endif
			case EDT_DIRECT3D8:
#if (SAPPHIRE_3D_API == SAPPHIRE_WITH_DIRECT3D_8)
				return true;
#else
				return false;
#endif
			case EDT_DIRECT3D9:
#if (SAPPHIRE_3D_API == SAPPHIRE_DIRECT3D_9)
				return true;
#else
				return false;
#endif
			case EDT_OPENGL:
#if (SAPPHIRE_3D_API == SAPPHIRE_WITH_OPENGL)
				return true;
#else
				return false;
#endif
			default:
				return false;
			}
		}
	};

}

#endif