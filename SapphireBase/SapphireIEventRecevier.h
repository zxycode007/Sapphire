#ifndef _SAPPHIRE_IEVENT_RECEVIER_
#define _SAPPHIRE_IEVENT_RECEVIER_

#include "SapphireKeyMap.h"
#include "SapphireKeyCodes.h"
#include "SapphirePrerequisites.h"
#include "SapphireLog.h"

namespace Sapphire
{
	//! ���������¼����͵�ö��
	enum EEVENT_TYPE
	{
		//! һ��GUI�¼�
		/** GUI �¼���GUI������GUIԪ�ض���������¼�����Ӧ��
		��GUIԪ�ؽ��ܵ�һ���¼��������Ǵ��������ҷ���true�����Ǵ�������¼������ĸ��ڵ㡣���һ���¼�û���������Ԫ��ǰ�����գ�
		��ô���������ݸ��û��Ľ�����
		 */
		EET_GUI_EVENT = 0,

		//! һ����������¼�
		/** ����¼����豸����������Ӧ�����Բ���ϵͳ��������벢���ݵ�Device::postEventFromUser
		����¼���һ�����ݵ��û��Ľ�������Ȼ���GUI����������Ԫ�أ���������ɳ������������գ����Ķ����ᴫ�ݸ���������
		*/
		EET_MOUSE_INPUT_EVENT,

		//! һ�����������¼�
		/** ����������¼��������¼����豸�������Ҵ��ݸ�Device::postEventFromUser
		. ��������¼�·����ͬ*/
		EET_KEY_INPUT_EVENT,

		//! һ�����ݸ������¼�
		/** 
		���ݸ��¼�ͨ����ѯ�������ӵĲ����˵ģ�ͬʱÿ���豸��run�������Ҵ����¼���Device::postEventFromUser.
		���Ǻ�����¼�·����ͬ
		Windows, SDL: ʵ��
		Linux: ʵ�� 
		MacOS / Other ��ûʵ��
		*/
		EET_JOYSTICK_INPUT_EVENT,

		//! һ����־�¼�
		/** ��־�¼�ֻ���ݸ��û�������������еĻ��� �������ͨ���û����������գ���ôû���ı��ᷢ�͵�����̨*/
		EET_LOG_TEXT_EVENT,

		//! һ��ʹ���û����ݵ��û��¼�
		/** �������治��ʹ�õģ���������ͨ��ϵͳ�����û��ض������ݡ�
		sapphire �ġ�windows handle����Device::getExposedVideoData()���
		ʹ�ú���Ϊ���������ڲ���ϵͳ��
		Windows: ����һ��WM_USER��Ϣ��Sapphire Window; ��
		wParam �� lParam ����������SUserEvent��UserData1��UserData2��Ա
		Linux: ͨ��XSendEvent����һ��ClientMessage��Sapphire���ڣ�����data.l[0] �� data.l[1]��������UserData1 �� UserData2.ת������SINT32����
		MacOS: û��ʵ��
		*/
		EET_USER_EVENT,

		//! �������
		//EGUIET_FORCE_32_BIT = 0x7fffffff

	};


	//! ������������¼�ö��
	enum EMOUSE_INPUT_EVENT
	{
		//! �������
		EMIE_LMOUSE_PRESSED_DOWN = 0,

		//!�Ҽ�����
		EMIE_RMOUSE_PRESSED_DOWN,

		//! �м�����
		EMIE_MMOUSE_PRESSED_DOWN,

		//! �������
		EMIE_LMOUSE_LEFT_UP,

		//! �Ҽ�����
		EMIE_RMOUSE_LEFT_UP,

		//! �м�����
		EMIE_MMOUSE_LEFT_UP,

		//! ���ָ��λ�øı�
		EMIE_MOUSE_MOVED,

		//! �����ֹ��������¼�������ʹ�ù���ֵ�����ҳ�������ٶ�
		EMIE_MOUSE_WHEEL,

		//! ������˫��
		//! ����¼����ڵڶ���EMIE_LMOUSE_PRESSED_DOWN����֮��
		EMIE_LMOUSE_DOUBLE_CLICK,

		//! ����Ҽ�˫��
		//! ����¼����ڵڶ���EMIE_RMOUSE_PRESSED_DOWN����֮��
		EMIE_RMOUSE_DOUBLE_CLICK,

		//! ����м�˫��
		//!  ����¼����ڵڶ���EMIE_MMOUSE_PRESSED_DOWN����֮��
		EMIE_MMOUSE_DOUBLE_CLICK,

		//! ����������
		//!  ����¼����ڵ�����EMIE_LMOUSE_PRESSED_DOWN����֮�� 
		EMIE_LMOUSE_TRIPLE_CLICK,

		//! ����Ҽ�����
		//!  ����¼����ڵ�����EMIE_RMOUSE_PRESSED_DOWN����֮�� 
		EMIE_RMOUSE_TRIPLE_CLICK,

		//! ����м�����
		//!  ����¼����ڵ�����EMIE_MMOUSE_PRESSED_DOWN����֮�� 
		EMIE_MMOUSE_TRIPLE_CLICK,

		 
		EMIE_COUNT
	};

	//! ��갴��״̬����
	enum E_MOUSE_BUTTON_STATE_MASK
	{
		EMBSM_LEFT = 0x01,
		EMBSM_RIGHT = 0x02,
		EMBSM_MIDDLE = 0x04,

		//!��ǰ�����ڴ�����
		EMBSM_EXTRA1 = 0x08,

		//! ��ǰ�����ڴ�����
		EMBSM_EXTRA2 = 0x10,

		EMBSM_FORCE_32_BIT = 0x7fffffff
	};

	class IGUIElement;

	//! ������GUIϵͳ���͵������¼���ö��
	enum EGUI_EVENT_TYPE
	{
		//! һ��GUIԪ��ʧȥ���Ľ���
		/** GUIEvent.Caller ʧȥ��GUIEvent.Element�Ľ���
		�������¼������գ���ô���㽫��ı� */
		EGET_ELEMENT_FOCUS_LOST = 0,

		//! һ��GUIԪ�ػ�ý���
		/** �������¼������գ���ô���㽫��ı� */
		EGET_ELEMENT_FOCUSED,

		//! ���ָ�븲�ǵ�һ��GUIԪ���Ϸ�
		/** ���һ��Ԫ������Ԫ�أ�������Щ��Ԫ����Ҳ���Ի�������Ϣ*/
		EGET_ELEMENT_HOVERED,

		//! ���ָ���뿪���ǵ�Ԫ��
		/**���һ��Ԫ������Ԫ�أ�������Щ��Ԫ����Ҳ���Ի�������Ϣ */
		EGET_ELEMENT_LEFT,

		//! һ��Ԫ�ؽ�Ҫ���ر�
		/** ���ں������Ĳ˵�ʹ������¼��������Ǽ������رգ�����ͨ����������¼���ȡ�� */
		EGET_ELEMENT_CLOSED,

		//! һ����ť������
		EGET_BUTTON_CLICKED,

		//! һ���������ı�λ��
		EGET_SCROLL_BAR_CHANGED,

		//! һ��checkbox�ı�״̬
		EGET_CHECKBOX_CHANGED,

		//! һ��listbox������Ŀ��ѡ��
		/** NOTE: �ڵ���Ŀ����һ�ε�������500ms�����ܹ��������¼�*/
		EGET_LISTBOX_CHANGED,

		//! һ��listbox������Ŀ��ѡ�����Ѿ���ѡ�����
		/** NOTE: ֻ������������Ŀ����һ�ε��500ms����ͨ��"enter"��space��ѡ��������ܻ�õ�ǰ����¼�
		 */
		EGET_LISTBOX_SELECTED_AGAIN,

		//! һ���ļ����ļ��Ի����б�ѡ��
		EGET_FILE_SELECTED,

		//! һ��Ŀ¼���ļ��Ի�������ת
		EGET_DIRECTORY_SELECTED,

		//! һ���ļ��򿪶Ի���û��ѡ���ļ��͹ر�
		EGET_FILE_CHOOSE_DIALOG_CANCELLED,

		//! ��һ����Ϣ������'Yes'�����
		EGET_MESSAGEBOX_YES,

		//! ��һ����Ϣ������'No'�����
		EGET_MESSAGEBOX_NO,

		//! ��һ����Ϣ������'OK'�����
		EGET_MESSAGEBOX_OK,

		//! ��һ����Ϣ������'Cancel'�����
		EGET_MESSAGEBOX_CANCEL,

		//! �ڱ༭�����У�'ENTER' ������
		EGET_EDITBOX_ENTER,

		//! �߽�������ı��ı䡣�ⲻ�������ı��ֽ��е��Զ��ı�
		EGET_EDITBOX_CHANGED,

		//! �ڱ༭���ӵı�����򱻸ı�
		EGET_EDITBOX_MARKING_CHANGED,

		//! ��һ��tab ���Ƶ�tab���ı�
		EGET_TAB_CHANGED,

		//! ��һ�������Ĳ˵�һ���˵��ѡ��
		EGET_MENU_ITEM_SELECTED,

		//! ��Ϻ��ӱ�ѡ��
		EGET_COMBO_BOX_CHANGED,

		//! spin���ӵ�ֵ���ı�
		EGET_SPINBOX_CHANGED,

		//! һ�����ı�
		EGET_TABLE_CHANGED,
		EGET_TABLE_HEADER_CHANGED,
		EGET_TABLE_SELECTED_AGAIN,

		//! һ�����۲�ڵ�ʧȥѡ�񣬼�IGUITreeView::getLastEventNode().
		EGET_TREEVIEW_NODE_DESELECT,

		//! һ�����۲�ڵ㱻ѡ�񣬼�IGUITreeView::getLastEventNode().
		EGET_TREEVIEW_NODE_SELECT,

		//! һ�����۲�ڵ㱻չ����IGUITreeView::getLastEventNode().
		EGET_TREEVIEW_NODE_EXPAND,

		//! һ�����۲�ڵ㱻�۵��� ��IGUITreeView::getLastEventNode().
		EGET_TREEVIEW_NODE_COLLAPSE,

		//! ���� - �� EGET_TREEVIEW_NODE_COLLAPSE ����
		EGET_TREEVIEW_NODE_COLLAPS = EGET_TREEVIEW_NODE_COLLAPSE,

		//�¼���
		EGET_COUNT
	};


	//! SEvents �����¼�����Ϣ����IEventReceiver�����¼���ϸ��
	struct SEvent
	{
		//! �κ�GUI���͵��¼�
		struct SGUIEvent
		{
			//! �����¼���IGUIElement
			IGUIElement* Caller;

			//! �������¼���Щ������Ҫ�������GUIԪ��Эͬ�����������������
			IGUIElement* Element;

			//! GUI�¼�������
			EGUI_EVENT_TYPE EventType;

		};

		//! �κ���������¼�
		struct SMouseInput
		{
			//! ���ָ���Xλ��
			SINT32 X;

			//! ���ָ���Yλ��
			SINT32 Y;

			//! ������������ͨ����1.0��-1.0������ҲΪ����ֵ < 0.f �� > 0.f;
			/** ֻ���¼�ΪEMIE_MOUSE_WHEELʱ��Ч */
			Real Wheel;

			//!���shift�����£�Ϊtrue
			bool Shift : 1;

			//! ���ctrl������Ϊtrue
			bool Control : 1;

			//! һ����ť��λͼ״̬���������isButtonPressed()�����
			//! ���һ����ť���»���û�а���
			//! ����¼���EMIE_MOUSE_MOVED����ǰ����Ч
			UINT32 ButtonStates;

			//! ������£�
			bool isLeftPressed() const { return 0 != (ButtonStates & EMBSM_LEFT); }

			//!  �Ҽ����£�
			bool isRightPressed() const { return 0 != (ButtonStates & EMBSM_RIGHT); }

			//! �м�����
			bool isMiddlePressed() const { return 0 != (ButtonStates & EMBSM_MIDDLE); }

			//! ����¼�
			EMOUSE_INPUT_EVENT Event;
		};

		//! ���ּ����¼�
		struct SKeyInput
		{
			//! ƥ��ļ����ַ��� 0��û��
			wchar_t Char;

			//! ��һ���������»����ͷ�
			EKEY_CODE Key;

			//! �����Ϊtrue����ô���������
			bool PressedDown : 1;

			//! ���Ϊtrue��shift������ 
			bool Shift : 1;

			//! ���Ϊtrue ctrl��������
			bool Control : 1;
		};

		//! һ���ֱ��¼�
		/** �������¼���һ�������ݸ��¼���ʾ���豸ÿ��run()��ʱ����ѯÿһ�����ӵĲ����˵Ľ����
		�������¼�Ĭ�Ͻ����ᴴ������������˶��ڼ����豸��֧������Ч��_SAPPHIRE_COMPILE_WITH_JOYSTICK_EVENTS_ �ᱻ���壬
		����Device::activateJoysticks()�ᱻ���ã�һ��������͵��¼�����ÿһ���������豸ÿ��Device::run()ʱ�����������ܲ�����
		��״̬ʵ������û�иı䡣
		 */
		struct SJoystickEvent
		{
			enum
			{
				NUMBER_OF_BUTTONS = 32,

				AXIS_X = 0, // e.g. ң��1����
				AXIS_Y,		// e.g. ң��1�ϵ���
				AXIS_Z,		// e.g. ���ţ���ģ��2ң��2 ����
				AXIS_R,		// e.g. ��, ��ģ��2ң��2 �ϵ���
				AXIS_U,
				AXIS_V,
				NUMBER_OF_AXES
			};

			/** ��ťλ״̬��λͼ��������� IsButtonPressed()������0��(NUMBER_OF_BUTTONS - 1)ÿһ����ť״̬
		   */
			SINT32 ButtonStates;

			/** ���� AXIS_X, AXIS_Y, AXIS_Z, AXIS_R, AXIS_U �� AXIS_V
			ֵ�ķ�Χ����-32768 to 32767��0��ʾ����λ�á��㽫���մӲ����˽���ԭʼֵ�����ҽ�ͨ��Ҫʵ��һ���������ķ�Χ��������
			�᲻��֧�ֲ����˵�0ֵ����Linux�ϣ�POV hat��ʾΪ�ᣬͨ���������������
			*/
			SINT16 Axis[NUMBER_OF_AXES];

			/** 
			POV��ʾPOV hat�ĽǶ�*100�ĽǶȣ���0~35900��ֵ65535��ʾPOV hat������
	        ���ֵֻ��windows��֧�֡���linux�У�POV hat ��2�������
			*/
			UINT16 POV;

			//! ��������¼��Ĳ�����ID
			/** ���������ڲ�������������ֱ��ӳ�䵽�κ��ر��Ӳ��������
			*/
			UINT8 Joystick;

			//! ���helper������ⰴ��������
			bool IsButtonPressed(UINT32 button) const
			{
				if (button >= (UINT32)NUMBER_OF_BUTTONS)
					return false;

				return (ButtonStates & (1 << button)) ? true : false;
			}
		};


		//! �������͵�log
		struct SLogEvent
		{
			//! �ı�ָ��
			const c8* Text;

			//! Log����
			LogMessageLevel Level;
		};

		//! ���������û��¼�
		struct SUserEvent
		{
			//! �û�ָ����������Ϊint
			SINT32 UserData1;

			//! �û�ָ����������Ϊint
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



	//! һ���ܹ������¼��Ľӿ�
	/** ���������඼�̳�IEventReceiver���������ǿ��Դ����¼����¼�ͨ����ʼ��postEventFromUser��������
	ͨ��һ���¼��������������ݣ�ֱ��OnEvent����true�������� ��EEVENT_TYPE����ÿһ���¼����͵�����������ͨ��ϵͳ����·��.
	  */
	class IEventReceiver
	{
	public:

		 
		virtual ~IEventReceiver() {}

		//! ����¼���������
		/** ��ע�⣺����Ҫ��ֹ�������һ���Ĵ����¼���ֻ��Ҫ����true��
		����true��ζ��һ���¼��Ѿ�������ɡ�
		* ����㷵��ֵ��Ӧ����δ������¼���Ӧ����false.
		\return ���Ϊtrue�����¼���������
		*/
		virtual bool OnEvent(const SEvent& event) = 0;
	};


	//! һ����������Ϣ  ������Device::activateJoysticks()
	struct SJoystickInfo
	{
		//! ������ID 
		/** ���������ڲ�������������ֱ��ӳ�䵽�κ��ر��Ӳ�������� 
		����SJoystickEvent Joystick IDһ��
	    */
		UINT8				Joystick;

		//! �ֱ����ڱ�ʾ�Լ�������
		String	Name;

		//! �ֱ�ӵ�еİ�����
		UINT32				Buttons;

		//! �����˵�����X, Y, Z, R, U, V.
		/** Note: ����Linux �豸, POV hat��������. ��Щ�������лᱻ����*/
		UINT32				Axes;

		//! һ��ֱ���������Ƿ���һ��POV hat
		/** һ��Windows�豸����ʶ���ڻ򲻴������POV hat��һ��Linux�豸���ܣ�����ֻ����POV_HAT_UNKNOWN*/
		enum
		{
			//Ŀǰ������һ��hat
			POV_HAT_PRESENT,

			//! һ��hatĿǰû�ж���
			POV_HAT_ABSENT,

			//! û�м�⵽hat
			POV_HAT_UNKNOWN
		} PovHat;
	};  

}

#endif