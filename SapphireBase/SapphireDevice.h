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

	//! 设备. 你可以通过函数createDevice() 或 createDeviceEx()创建设备
	/** 这是引擎最重要的类。如果你有一个这个类的实例指针，你就可以访问引擎的所有东西。这个类在任何时刻只能有一个实例。
	*/
	class Device
	{
	public:

		//! 运行这个设备
		/** 通过调用ITimer：：tick（）增加虚拟计时器
		你能够通过调用ITimer::stop()阻止它；在 ITimer::start()之前，在Device：：run（）之后。
		如果设备被删除，返回false。 通过下面的方法使用它：

		while(device->run())
		{
		// draw everything here
		}
		如果你想要如果在窗口非激活状态设备不做任何事情（推荐），稍微修改下代码，见用isWindowActive()
		注意：如果你在一个外部的自定义窗口中运行：调用Device->run()将会导致引擎丢弃windows内部消息
		如果你要在你自己自定义的窗口中运行引擎，用你自己实现的消息循环GetMessage和DispatchMessage和不管什么都不能用这个方法。
		但是注意：引擎不会拾取用户输入。见irr::SIrrlichtCreationParameters::WindowId更多信息见代码
		*/
		virtual bool run() = 0;

		//! 导致这个设备暂时暂停指向并且让其他进程允许
		/** 这将降低处理器使用率而不会导致引擎主要性能丢失*/
		virtual void yield() = 0;

		//! 暂停执行并且让其他进程运行一个特定量的时间
		/** 它会等待到给定的完整时间，然后睡眠将被打断。
		\param timeMs: 睡眠时间的毫秒数
		\param pauseTimer: 如果为true，当睡眠的时候暂停设备
		*/
		virtual void sleep(UINT32 timeMs, bool pauseTimer = false) = 0;

		//! 提供访问绘制3D和2D几何体的视频设备
		/** \return 视频设备的指针. */
		virtual IVideoDriver* getVideoDriver() = 0;

		//! 提供访问虚拟文件系统的指针
		/** \return 文件系统的指针 */
		virtual IFileSystem* getFileSystem() = 0;

		//! 提供2D GUI环境的访问
		/** \return GUI环境的指针 */
		virtual IGUIEnvironment* getGUIEnvironment() = 0;

		//! 提供场景管理器的访问
		/** \return 场景管理器指针 */
		virtual ISceneManager* getSceneManager() = 0;

		//! 提供访问鼠标控制
		/** \return 鼠标游标控制接口指针*/
		virtual ICursorControl* getCursorControl() = 0;

		//! 提供范围消息日志接口
		/** \return 日志器指针 */
		virtual ILogger* getLogger() = 0;

		//! 获取可用视频模式列表
		/** 如果搞不清楚，由于你要用一个视频模式创建一个Device之前要获取视频模式列表。
		没有必要用EDT_DIRECT3D8,EDT_OPENGL 或 EDT_SOFTWARE：对于没有驱动的， EDT_NULL存在。
		\return 通过gfx适配器的支持视频模式列表的指针*/
		virtual IVideoModeList* getVideoModeList() = 0;

		//! 提供访问操作系统的操作对象
		/** 提供操作系统一些操作的方法：获取系统特定信息和做一些系统特定操作，
		如用剪切板交换数据或者读取操作系统版本
		\return 操作系统操作的指针*/
		virtual IOSOperator* getOSOperator() = 0;

		//! 提供一个访问引擎的timer
		/** 通过它这个虚拟世界能够返回系统时间，它可以被操作
		\return 指向ITimer的对象 */
		virtual ITimer* getTimer() = 0;

		//! 提供访问引擎当前的随机数发生器
		/** \return 指向IRandomizer对象的指针 */
		virtual IRandomizer* getRandomizer() const = 0;

		//! 设置一个新的randomizer.
		/** \param r 指向IRandomizer对象的指针。这个对象一旦grab()，那么引擎要释放它只有通过下一次setRandomizer调用
		或设备销毁 */
		virtual void setRandomizer(IRandomizer* r) = 0;

		//! 创建一个新的默认randomizer
		/**
		\return 指向默认IRandomizer对象的指针 */
		virtual IRandomizer* createDefaultRandomizer() const = 0;

		//! 设置窗口的标题
		/** \param text: 窗口标题位置 */
		virtual void setWindowCaption(const wchar_t* text) = 0;

		//! 返回这个窗口是否是激活 
		/** 如果这个窗口不是激活的，不需要进行绘制。所以如果当窗口没有激活时，你不需要进行任何绘制，
		通过下边方法创建你的绘制循环
		
		while(device->run())
		{
		if (device->isWindowActive())
		{
		// draw everything here
		}
		else
		device->yield();
		}
		\return 如果窗口被激活返回true */
		virtual bool isWindowActive() const = 0;

		//! 剪裁Irrlicht窗口是否有焦点
		/** \return 如果窗口有交点返回true */
		virtual bool isWindowFocused() const = 0;

		//! 检测窗口是否最小化
		/** \return 如果最最小化返回true */
		virtual bool isWindowMinimized() const = 0;

		//! 检测窗口是否全屏幕
		/** \return 如果最全屏幕返回true */
		virtual bool isFullscreen() const = 0;

		//! 获取这个窗口的当前颜色格式
		/** \return 窗口的颜色格式 */
		virtual ECOLOR_FORMAT getColorFormat() const = 0;

		//! 通知设备它应该关闭它自己
		/**Device::run() 在 closeDevice()调用后会返回false . */
		virtual void closeDevice() = 0;

		//! 获取引擎版本信息
		/** 返回的字符串
		*/
		virtual const String getVersion() const = 0;

		//! 设置一个新的用户事件接收器，它由引擎接受事件
		/** 
		\param receiver 要被使用新的事件接收器. */
		virtual void setEventReceiver(IEventReceiver* receiver) = 0;

		//! 提供方法当前事件的接收器
		/** \return 指向当前世界接收器的指针 */
		virtual IEventReceiver* getEventReceiver() = 0;

		//! 发送一个用户创建的世界到引擎
		/** 这通常不是必须的，然而，如果你使用自定义的输入库，比如手柄输入，你需要使用这个抛出key和鼠标的输入事件到引擎。
		内部，这个方法只是委托这个事件进一步到场景管理器到GUI环境
		 */
		virtual bool postEventFromUser(const SEvent& event) = 0;

		//! 设置输入接受场景管理器
		/** 如果设置为NULL， 主场景管理器（由GetSceneManager（）返回）将接收输入
		\param sceneManager 要使用的新场景管理器. */
		virtual void setInputReceivingSceneManager(ISceneManager* sceneManager) = 0;

		//! 设置是否能够窗口被在窗口模式被调整大小
		/** 默认是false，这个方法只能工作在窗口模式
		 */
		virtual void setResizable(bool resize = false) = 0;

		//! 如果可能窗口最小化
		virtual void minimizeWindow() = 0;

		//! 如果可能窗口最大化
		virtual void maximizeWindow() = 0;

		//! 如果可能恢复窗口正常大小
		virtual void restoreWindow() = 0;

		//! 激活手柄，为他们创建事件
		/** 目前包含手柄支持，但默认情况下不能创建手柄事件，要支持手柄信息需要第三方库支持。调用这个方法激活手柄支持，并且接收SJoystickEvent事件
		\param joystickInfo , 返回的手柄信息，这将包含每一个找到和激活的手柄数组
		\return true 如果在这个设备上被支持和_IRR_COMPILE_WITH_JOYSTICK_EVENTS_被定义，返回True
		*/
		virtual bool activateJoysticks(std::vector<SJoystickInfo>& joystickInfo) = 0;

		//! 设置显示的当前的gamma值
		virtual bool setGammaRamp(Real red, Real green, Real blue,
			Real relativebrightness, Real relativecontrast) = 0;

		//! 获取当前显示的gamma值
		virtual bool getGammaRamp(Real &red, Real &green, Real &blue,
			Real &brightness, Real &contrast) = 0;

		//! 移除在系统消息循环中待定的消息
		/** 这个函数通常用做消息被缓冲了很长一段时间，例如：当读取了很大一个场景。
		清空这个消息循环保证用户在GUI期间的意见动作像鼠标-或按键点击得以触发。 
		到目前为止，下面的消息会被清空
		Win32: 所有的键盘和鼠标信息 
		Linux: 所有的键盘和鼠标信息
		所有其它设备在这儿不被支持
		 */
		virtual void clearSystemMessages() = 0;

		//! 获取设备的类型
		/** . */
		virtual E_DEVICE_TYPE getType() const = 0;

		//! 检测如果引擎支持一个驱动类型
		/** 判断渠道是否可用，可用返回true，否则false*/
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