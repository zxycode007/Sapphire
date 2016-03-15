#ifndef _SAPPHIRE_IEVENT_RECEVIER_
#define _SAPPHIRE_IEVENT_RECEVIER_

#include "SapphireKeyMap.h"
#include "SapphireKeyCodes.h"
#include "SapphirePrerequisites.h"
#include "SapphireLog.h"

namespace Sapphire
{
	//! 对于所有事件类型的枚举
	enum EEVENT_TYPE
	{
		//! 一个GUI事件
		/** GUI 事件由GUI环境或GUI元素对鼠标或键盘事件的响应。
		当GUI元素接受到一个事件，它不是处理它并且返回true，就是传递这个事件到它的父节点。如果一个事件没在它到达根元素前被吸收，
		那么它将被传递给用户的接收器
		 */
		EET_GUI_EVENT = 0,

		//! 一个鼠标输入事件
		/** 鼠标事件由设备创建并且响应到来自操作系统的鼠标输入并传递到Device::postEventFromUser
		鼠标事件第一个传递到用户的接收器，然后给GUI环境和它的元素，最后输入由场景管理器接收，在哪儿它会传递给激活的相机
		*/
		EET_MOUSE_INPUT_EVENT,

		//! 一个键盘输入事件
		/** 类似于鼠标事件，键盘事件由设备创建并且传递给Device::postEventFromUser
		. 它和鼠标事件路径相同*/
		EET_KEY_INPUT_EVENT,

		//! 一个操纵杆输入事件
		/** 
		操纵杆事件通过轮询所有连接的操作杆的，同时每个设备都run（）并且传递事件给Device::postEventFromUser.
		它们和鼠标事件路径相同
		Windows, SDL: 实现
		Linux: 实现 
		MacOS / Other ：没实现
		*/
		EET_JOYSTICK_INPUT_EVENT,

		//! 一个日志事件
		/** 日志事件只传递给用户接收器，如果有的话。 如果它们通过用户接收器接收，那么没有文本会发送到控制台*/
		EET_LOG_TEXT_EVENT,

		//! 一个使用用户数据的用户事件
		/** 这是引擎不会使用的，并且用于通过系统发送用户特定的数据。
		sapphire 的‘windows handle’由Device::getExposedVideoData()获得
		使用和行为具体依赖于操作系统：
		Windows: 发送一个WM_USER消息给Sapphire Window; 这
		wParam 和 lParam 将用于填入SUserEvent的UserData1和UserData2成员
		Linux: 通过XSendEvent发送一个ClientMessage给Sapphire窗口；其中data.l[0] 和 data.l[1]变量是由UserData1 和 UserData2.转换到的SINT32数据
		MacOS: 没有实现
		*/
		EET_USER_EVENT,

		//! 这个不用
		//EGUIET_FORCE_32_BIT = 0x7fffffff

	};


	//! 所有鼠标输入事件枚举
	enum EMOUSE_INPUT_EVENT
	{
		//! 左键按下
		EMIE_LMOUSE_PRESSED_DOWN = 0,

		//!右键按下
		EMIE_RMOUSE_PRESSED_DOWN,

		//! 中键按下
		EMIE_MMOUSE_PRESSED_DOWN,

		//! 左键弹起
		EMIE_LMOUSE_LEFT_UP,

		//! 右键弹起
		EMIE_RMOUSE_LEFT_UP,

		//! 中键弹起
		EMIE_MMOUSE_LEFT_UP,

		//! 鼠标指针位置改变
		EMIE_MOUSE_MOVED,

		//! 鼠标滚轮滚动，在事件数据中使用滚轮值可以找出方向和速度
		EMIE_MOUSE_WHEEL,

		//! 鼠标左键双击
		//! 这个事件是在第二次EMIE_LMOUSE_PRESSED_DOWN创建之后
		EMIE_LMOUSE_DOUBLE_CLICK,

		//! 鼠标右键双击
		//! 这个事件是在第二次EMIE_RMOUSE_PRESSED_DOWN创建之后
		EMIE_RMOUSE_DOUBLE_CLICK,

		//! 鼠标中键双击
		//!  这个事件是在第二次EMIE_MMOUSE_PRESSED_DOWN创建之后
		EMIE_MMOUSE_DOUBLE_CLICK,

		//! 鼠标左键三击
		//!  这个事件是在第三次EMIE_LMOUSE_PRESSED_DOWN创建之后 
		EMIE_LMOUSE_TRIPLE_CLICK,

		//! 鼠标右键三击
		//!  这个事件是在第三次EMIE_RMOUSE_PRESSED_DOWN创建之后 
		EMIE_RMOUSE_TRIPLE_CLICK,

		//! 鼠标中键三击
		//!  这个事件是在第三次EMIE_MMOUSE_PRESSED_DOWN创建之后 
		EMIE_MMOUSE_TRIPLE_CLICK,

		 
		EMIE_COUNT
	};

	//! 鼠标按键状态掩码
	enum E_MOUSE_BUTTON_STATE_MASK
	{
		EMBSM_LEFT = 0x01,
		EMBSM_RIGHT = 0x02,
		EMBSM_MIDDLE = 0x04,

		//!当前仅仅在窗口上
		EMBSM_EXTRA1 = 0x08,

		//! 当前仅仅在窗口上
		EMBSM_EXTRA2 = 0x10,

		EMBSM_FORCE_32_BIT = 0x7fffffff
	};

	class IGUIElement;

	//! 对于由GUI系统发送的所有事件的枚举
	enum EGUI_EVENT_TYPE
	{
		//! 一个GUI元素失去它的焦点
		/** GUIEvent.Caller 失去了GUIEvent.Element的焦点
		如果这个事件被接收，那么焦点将会改变 */
		EGET_ELEMENT_FOCUS_LOST = 0,

		//! 一个GUI元素获得焦点
		/** 如果这个事件被接收，那么焦点将会改变 */
		EGET_ELEMENT_FOCUSED,

		//! 鼠标指针覆盖到一个GUI元素上方
		/** 如果一个元素有子元素，对于这些子元素你也可以获得这个消息*/
		EGET_ELEMENT_HOVERED,

		//! 鼠标指针离开覆盖的元素
		/**如果一个元素有子元素，对于这些子元素你也可以获得这个消息 */
		EGET_ELEMENT_LEFT,

		//! 一个元素将要被关闭
		/** 窗口和上下文菜单使用这个事件，当它们即将被关闭，可以通过接受这个事件来取消 */
		EGET_ELEMENT_CLOSED,

		//! 一个按钮被按下
		EGET_BUTTON_CLICKED,

		//! 一个滚动条改变位置
		EGET_SCROLL_BAR_CHANGED,

		//! 一个checkbox改变状态
		EGET_CHECKBOX_CHANGED,

		//! 一个listbox的新条目被选择
		/** NOTE: 在当条目被再一次点击后大于500ms，你能够获得这个事件*/
		EGET_LISTBOX_CHANGED,

		//! 一个listbox的新条目被选择，它已经被选择过了
		/** NOTE: 只有在如果这个条目被再一次点击500ms或者通过"enter"或“space”选定，你才能获得当前这个事件
		 */
		EGET_LISTBOX_SELECTED_AGAIN,

		//! 一个文件在文件对话框中被选择
		EGET_FILE_SELECTED,

		//! 一个目录在文件对话框中旋转
		EGET_DIRECTORY_SELECTED,

		//! 一个文件打开对话框没有选择文件就关闭
		EGET_FILE_CHOOSE_DIALOG_CANCELLED,

		//! 在一个消息窗口中'Yes'被点击
		EGET_MESSAGEBOX_YES,

		//! 在一个消息窗口中'No'被点击
		EGET_MESSAGEBOX_NO,

		//! 在一个消息窗口中'OK'被点击
		EGET_MESSAGEBOX_OK,

		//! 在一个消息窗口中'Cancel'被点击
		EGET_MESSAGEBOX_CANCEL,

		//! 在编辑盒子中，'ENTER' 被按下
		EGET_EDITBOX_ENTER,

		//! 边界盒子中文本改变。这不包括在文本分解中的自动改变
		EGET_EDITBOX_CHANGED,

		//! 在编辑盒子的标记区域被改变
		EGET_EDITBOX_MARKING_CHANGED,

		//! 在一个tab 控制的tab被改变
		EGET_TAB_CHANGED,

		//! 在一个上下文菜单一个菜单项被选择
		EGET_MENU_ITEM_SELECTED,

		//! 组合盒子被选择
		EGET_COMBO_BOX_CHANGED,

		//! spin盒子的值被改变
		EGET_SPINBOX_CHANGED,

		//! 一个表被改变
		EGET_TABLE_CHANGED,
		EGET_TABLE_HEADER_CHANGED,
		EGET_TABLE_SELECTED_AGAIN,

		//! 一个树观察节点失去选择，见IGUITreeView::getLastEventNode().
		EGET_TREEVIEW_NODE_DESELECT,

		//! 一个树观察节点被选择，见IGUITreeView::getLastEventNode().
		EGET_TREEVIEW_NODE_SELECT,

		//! 一个树观察节点被展开，IGUITreeView::getLastEventNode().
		EGET_TREEVIEW_NODE_EXPAND,

		//! 一个树观察节点被折叠。 见IGUITreeView::getLastEventNode().
		EGET_TREEVIEW_NODE_COLLAPSE,

		//! 废弃 - 用 EGET_TREEVIEW_NODE_COLLAPSE 代替
		EGET_TREEVIEW_NODE_COLLAPS = EGET_TREEVIEW_NODE_COLLAPSE,

		//事件数
		EGET_COUNT
	};


	//! SEvents 保存事件的信息。见IEventReceiver对于事件的细节
	struct SEvent
	{
		//! 任何GUI类型的事件
		struct SGUIEvent
		{
			//! 调用事件的IGUIElement
			IGUIElement* Caller;

			//! 如果这个事件有些事情需要和另外的GUI元素协同，它将被保存在这儿
			IGUIElement* Element;

			//! GUI事件的类型
			EGUI_EVENT_TYPE EventType;

		};

		//! 任何类型鼠标事件
		struct SMouseInput
		{
			//! 鼠标指针的X位置
			SINT32 X;

			//! 鼠标指针的Y位置
			SINT32 Y;

			//! 鼠标滚轮增量，通常在1.0或-1.0，但是也为其它值 < 0.f 或 > 0.f;
			/** 只在事件为EMIE_MOUSE_WHEEL时有效 */
			Real Wheel;

			//!如果shift被按下，为true
			bool Shift : 1;

			//! 如果ctrl被按下为true
			bool Control : 1;

			//! 一个按钮的位图状态。你可以用isButtonPressed()来检测
			//! 如果一个按钮按下或者没有按下
			//! 如果事件是EMIE_MOUSE_MOVED，当前才有效
			UINT32 ButtonStates;

			//! 左键按下？
			bool isLeftPressed() const { return 0 != (ButtonStates & EMBSM_LEFT); }

			//!  右键按下？
			bool isRightPressed() const { return 0 != (ButtonStates & EMBSM_RIGHT); }

			//! 中键按下
			bool isMiddlePressed() const { return 0 != (ButtonStates & EMBSM_MIDDLE); }

			//! 鼠标事件
			EMOUSE_INPUT_EVENT Event;
		};

		//! 各种键盘事件
		struct SKeyInput
		{
			//! 匹配的键盘字符， 0则没有
			wchar_t Char;

			//! 哪一个键被按下或者释放
			EKEY_CODE Key;

			//! 如果不为true，那么这个键弹起
			bool PressedDown : 1;

			//! 如果为true，shift被按下 
			bool Shift : 1;

			//! 如果为true ctrl键被按下
			bool Control : 1;
		};

		//! 一个手柄事件
		/** 和其它事件不一样，操纵杆事件表示在设备每次run()的时候轮询每一个连接的操作杆的结果。
		操作杆事件默认将不会创建。如果操作杆对于激活设备的支持是有效。_SAPPHIRE_COMPILE_WITH_JOYSTICK_EVENTS_ 会被定义，
		并且Device::activateJoysticks()会被调用，一个这个类型的事件将在每一个操作杆设备每次Device::run()时创建，而不管操作杆
		的状态实际上有没有改变。
		 */
		struct SJoystickEvent
		{
			enum
			{
				NUMBER_OF_BUTTONS = 32,

				AXIS_X = 0, // e.g. 遥感1左到右
				AXIS_Y,		// e.g. 遥感1上到下
				AXIS_Z,		// e.g. 油门，或模拟2遥感2 左到右
				AXIS_R,		// e.g. 舵, 或模拟2遥感2 上到下
				AXIS_U,
				AXIS_V,
				NUMBER_OF_AXES
			};

			/** 按钮位状态的位图，你可以用 IsButtonPressed()来检查从0到(NUMBER_OF_BUTTONS - 1)每一个按钮状态
		   */
			SINT32 ButtonStates;

			/** 对于 AXIS_X, AXIS_Y, AXIS_Z, AXIS_R, AXIS_U 和 AXIS_V
			值的范围是在-32768 to 32767，0表示中心位置。你将接收从操作杆接收原始值，并且将通常要实现一个环绕中心范围的死区。
			轴不不支持操作杆的0值。在Linux上，POV hat表示为轴，通常最后两个激活轴
			*/
			SINT16 Axis[NUMBER_OF_AXES];

			/** 
			POV表示POV hat的角度*100的角度，从0~35900。值65535表示POV hat在中心
	        这个值只在windows中支持。在linux中，POV hat 有2个轴代替
			*/
			UINT16 POV;

			//! 创建这个事件的操作杆ID
			/** 这是引擎内部索引；它不能直接映射到任何特别的硬件操作杆
			*/
			UINT8 Joystick;

			//! 如果helper函数检测按键被按下
			bool IsButtonPressed(UINT32 button) const
			{
				if (button >= (UINT32)NUMBER_OF_BUTTONS)
					return false;

				return (ButtonStates & (1 << button)) ? true : false;
			}
		};


		//! 各种类型的log
		struct SLogEvent
		{
			//! 文本指针
			const c8* Text;

			//! Log级别
			LogMessageLevel Level;
		};

		//! 各种类型用户事件
		struct SUserEvent
		{
			//! 用户指定的数据作为int
			SINT32 UserData1;

			//! 用户指定的数据作为int
			SINT32 UserData2;
		};

		EEVENT_TYPE EventType;
		union
		{
			struct SGUIEvent GUIEvent;
			struct SMouseInput MouseInput;
			struct SKeyInput KeyInput;
			struct SJoystickEvent JoystickEvent;
			struct SLogEvent LogEvent;
			struct SUserEvent UserEvent;
		};

	};



	//! 一个能够接收事件的接口
	/** 引擎许多的类都继承IEventReceiver，所以他们可以处理事件。事件通常开始于postEventFromUser函数并且
	通过一个事件接收者链来传递，直到OnEvent返回true而结束。 看EEVENT_TYPE对于每一个事件类型的描述，并且通过系统所需路径.
	  */
	class IEventReceiver
	{
	public:

		 
		virtual ~IEventReceiver() {}

		//! 如果事件发生调用
		/** 请注意：当你要阻止引擎更进一步的处理事件，只需要返回true。
		所以true意味着一个事件已经处理完成。
		* 因此你返回值对应所以未处理的事件都应该是false.
		\return 如果为true，则事件被处理了
		*/
		virtual bool OnEvent(const SEvent& event) = 0;
	};


	//! 一个操作杆信息  返回自Device::activateJoysticks()
	struct SJoystickInfo
	{
		//! 操作杆ID 
		/** 这是引擎内部索引；它不能直接映射到任何特别的硬件操作杆 
		它与SJoystickEvent Joystick ID一致
	    */
		UINT8				Joystick;

		//! 手柄用于标示自己的名字
		String	Name;

		//! 手柄拥有的按键数
		UINT32				Buttons;

		//! 操作杆的轴数X, Y, Z, R, U, V.
		/** Note: 对于Linux 设备, POV hat用两个轴. 这些在数量中会被包括*/
		UINT32				Axes;

		//! 一个直到操作杆是否有一个POV hat
		/** 一个Windows设备将标识存在或不存在这个POV hat。一个Linux设备不能，并且只返回POV_HAT_UNKNOWN*/
		enum
		{
			//目前定义了一个hat
			POV_HAT_PRESENT,

			//! 一个hat目前没有定义
			POV_HAT_ABSENT,

			//! 没有检测到hat
			POV_HAT_UNKNOWN
		} PovHat;
	};  

}

#endif