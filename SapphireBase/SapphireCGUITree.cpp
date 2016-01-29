#include "SapphireCGUITree.h"


#ifdef _SAPPHIRE_COMPILE_WITH_GUI_
#include "SapphireIGUISkin.h"
#include "SapphireIGUIEnvironment.h"
#include "SapphireIVideoDriver.h"
#include "SapphireFont.h"
#include "SapphireCGUIScrollBar.h"
#include "SapphireOS.h"
namespace Sapphire
{

	CGUITreeViewNode::CGUITreeViewNode(CGUITreeView* owner, CGUITreeViewNode* parent)
		: Owner(owner), Parent(parent), ImageIndex(-1), SelectedImageIndex(-1),
		Data(0), Data2(0), Expanded(false)
	{
#ifdef _DEBUG
		setDebugName("CGUITreeView");
#endif
	}

	CGUITreeViewNode::~CGUITreeViewNode()
	{
		if (Owner && this == Owner->getSelected())
		{
			setSelected(false);
		}

		clearChildren();

		if (Data2)
		{
			Data2->drop();
		}
	}

	IGUITreeView* CGUITreeViewNode::getOwner() const
	{
		return Owner;
	}

	IGUITreeViewNode* CGUITreeViewNode::getParent() const
	{
		return Parent;
	}

	void CGUITreeViewNode::setText(const wchar_t* text)
	{
		Text = text;
	}

	void CGUITreeViewNode::setIcon(const wchar_t* icon)
	{
		Icon = icon;
	}

	void CGUITreeViewNode::clearChildren()
	{
		list<CGUITreeViewNode*>::iterator	it;

		for (it = Children.begin(); it != Children.end(); it++)
		{
			(*it)->drop();
		}
		Children.clear();
	}

	IGUITreeViewNode* CGUITreeViewNode::addChildBack(
		const wchar_t*		text,
		const wchar_t*		icon /*= 0*/,
		SINT32					imageIndex /*= -1*/,
		SINT32					selectedImageIndex /*= -1*/,
		void*					data /*= 0*/,
		IReferenceCounter*			data2 /*= 0*/)
	{
		CGUITreeViewNode*	newChild = new CGUITreeViewNode(Owner, this);

		Children.push_back(newChild);
		newChild->Text = text;
		newChild->Icon = icon;
		newChild->ImageIndex = imageIndex;
		newChild->SelectedImageIndex = selectedImageIndex;
		newChild->Data = data;
		newChild->Data2 = data2;
		if (data2)
		{
			data2->grab();
		}
		return newChild;
	}

	IGUITreeViewNode* CGUITreeViewNode::addChildFront(
		const wchar_t*		text,
		const wchar_t*		icon /*= 0*/,
		SINT32					imageIndex /*= -1*/,
		SINT32					selectedImageIndex /*= -1*/,
		void*					data /*= 0*/,
		IReferenceCounter*			data2 /*= 0*/)
	{
		CGUITreeViewNode*	newChild = new CGUITreeViewNode(Owner, this);

		Children.push_front(newChild);
		newChild->Text = text;
		newChild->Icon = icon;
		newChild->ImageIndex = imageIndex;
		newChild->SelectedImageIndex = selectedImageIndex;
		newChild->Data = data;
		newChild->Data2 = data2;
		if (data2)
		{
			data2->grab();
		}
		return newChild;
	}

	IGUITreeViewNode* CGUITreeViewNode::insertChildAfter(
		IGUITreeViewNode*	other,
		const wchar_t*		text,
		const wchar_t*		icon /*= 0*/,
		SINT32					imageIndex /*= -1*/,
		SINT32					selectedImageIndex /*= -1*/,
		void*					data /*= 0*/,
		IReferenceCounter*			data2/* = 0*/)
	{
		list<CGUITreeViewNode*>::iterator	itOther;
		CGUITreeViewNode*									newChild = 0;

		for (itOther = Children.begin(); itOther != Children.end(); itOther++)
		{
			if (other == *itOther)
			{
				newChild = new CGUITreeViewNode(Owner, this);
				newChild->Text = text;
				newChild->Icon = icon;
				newChild->ImageIndex = imageIndex;
				newChild->SelectedImageIndex = selectedImageIndex;
				newChild->Data = data;
				newChild->Data2 = data2;
				if (data2)
				{
					data2->grab();
				}
				list<CGUITreeViewNode*>::iterator it = itOther;
				it++;
				//Children.insert_after(itOther, newChild);
				Children.insert(it, newChild);
				break;
			}
		}
		return newChild;
	}

	IGUITreeViewNode* CGUITreeViewNode::insertChildBefore(
		IGUITreeViewNode*	other,
		const wchar_t*		text,
		const wchar_t*		icon /*= 0*/,
		SINT32					imageIndex /*= -1*/,
		SINT32					selectedImageIndex /*= -1*/,
		void*					data /*= 0*/,
		IReferenceCounter*			data2/* = 0*/)
	{
		list<CGUITreeViewNode*>::iterator	itOther;
		CGUITreeViewNode*									newChild = 0;

		for (itOther = Children.begin(); itOther != Children.end(); itOther++)
		{
			if (other == *itOther)
			{
				newChild = new CGUITreeViewNode(Owner, this);
				newChild->Text = text;
				newChild->Icon = icon;
				newChild->ImageIndex = imageIndex;
				newChild->SelectedImageIndex = selectedImageIndex;
				newChild->Data = data;
				newChild->Data2 = data2;
				if (data2)
				{
					data2->grab();
				}
				//Children.insert_before(itOther, newChild);
				Children.insert(itOther, newChild);
				break;
			}
		}
		return newChild;
	}

	IGUITreeViewNode* CGUITreeViewNode::getFirstChild() const
	{
		if (Children.empty())
		{
			return 0;
		}
		else
		{
			return *(Children.begin());
		}
	}

	IGUITreeViewNode* CGUITreeViewNode::getLastChild() const
	{
		if (Children.empty())
		{
			return 0;
		}
		else
		{
			return Children.back();
		}
	}

	IGUITreeViewNode* CGUITreeViewNode::getPrevSibling() const
	{
		list<CGUITreeViewNode*>::iterator	itThis;
		list<CGUITreeViewNode*>::iterator	itOther;
		CGUITreeViewNode*									other = 0;

		if (Parent)
		{
			for (itThis = Parent->Children.begin(); itThis != Parent->Children.end(); itThis++)
			{
				if (this == *itThis)
				{
					if (itThis != Parent->Children.begin())
					{
						other = *itOther;
					}
					break;
				}
				itOther = itThis;
			}
		}
		return other;
	}

	IGUITreeViewNode* CGUITreeViewNode::getNextSibling() const
	{
		list<CGUITreeViewNode*>::iterator	itThis;
		CGUITreeViewNode*									other = 0;

		if (Parent)
		{
			for (itThis = Parent->Children.begin(); itThis != Parent->Children.end(); itThis++)
			{
				if (this == *itThis)
				{
					if (itThis != (--Parent->Children.end()))//if (itThis != Parent->Children.getLast())
					{
						other = *(++itThis);
					}
					break;
				}
			}
		}
		return other;
	}

	IGUITreeViewNode* CGUITreeViewNode::getNextVisible() const
	{
		IGUITreeViewNode*	next = 0;
		IGUITreeViewNode*	node = 0;

		node = const_cast<CGUITreeViewNode*>(this);

		if (node->getExpanded() && node->hasChildren())
		{
			next = node->getFirstChild();
		}
		else
		{
			next = node->getNextSibling();
		}
		while (!next && node->getParent())
		{
			next = node->getParent()->getNextSibling();
			if (!next)
			{
				node = node->getParent();
			}
		}

		return next;
	}

	bool CGUITreeViewNode::deleteChild(IGUITreeViewNode* child)
	{
		list<CGUITreeViewNode*>::iterator	itChild;
		bool	deleted = false;

		for (itChild = Children.begin(); itChild != Children.end(); itChild++)
		{
			if (child == *itChild)
			{
				child->drop();
				Children.erase(itChild);
				deleted = true;
				break;
			}
		}
		return deleted;
	}

	bool CGUITreeViewNode::moveChildUp(IGUITreeViewNode* child)
	{
		list<CGUITreeViewNode*>::iterator	itChild;
		list<CGUITreeViewNode*>::iterator	itOther;
		CGUITreeViewNode*									nodeTmp;
		bool													moved = false;

		for (itChild = Children.begin(); itChild != Children.end(); itChild++)
		{
			if (child == *itChild)
			{
				if (itChild != Children.begin())
				{
					nodeTmp = *itChild;
					*itChild = *itOther;
					*itOther = nodeTmp;
					moved = true;
				}
				break;
			}
			itOther = itChild;
		}
		return moved;
	}

	bool CGUITreeViewNode::moveChildDown(IGUITreeViewNode* child)
	{
		list<CGUITreeViewNode*>::iterator	itChild;
		list<CGUITreeViewNode*>::iterator	itOther;
		CGUITreeViewNode*									nodeTmp;
		bool													moved = false;

		for (itChild = Children.begin(); itChild != Children.end(); itChild++)
		{
			if (child == *itChild)
			{
				if (itChild != (--Children.end()))//if (itChild != Children.getLast())
				{
					itOther = itChild;
					++itOther;
					nodeTmp = *itChild;
					*itChild = *itOther;
					*itOther = nodeTmp;
					moved = true;
				}
				break;
			}
		}
		return moved;
	}

	void CGUITreeViewNode::setExpanded(bool expanded)
	{
		Expanded = expanded;
	}

	void CGUITreeViewNode::setSelected(bool selected)
	{
		if (Owner)
		{
			if (selected)
			{
				Owner->Selected = this;
			}
			else
			{
				if (Owner->Selected == this)
				{
					Owner->Selected = 0;
				}
			}
		}
	}

	bool CGUITreeViewNode::getSelected() const
	{
		if (Owner)
		{
			return Owner->Selected == (IGUITreeViewNode*)this;
		}
		else
		{
			return false;
		}
	}

	bool CGUITreeViewNode::isRoot() const
	{
		return (Owner && (this == Owner->Root));
	}

	SINT32 CGUITreeViewNode::getLevel() const
	{
		if (Parent)
		{
			return Parent->getLevel() + 1;
		}
		else
		{
			return 0;
		}
	}

	bool CGUITreeViewNode::isVisible() const
	{
		if (Parent)
		{
			return Parent->getExpanded() && Parent->isVisible();
		}
		else
		{
			return true;
		}
	}


	//! constructor
	CGUITreeView::CGUITreeView(IGUIEnvironment* environment, IGUIElement* parent,
		SINT32 id, rect<SINT32> rectangle, bool clip,
		bool drawBack, bool scrollBarVertical, bool scrollBarHorizontal)
		: IGUITreeView(environment, parent, id, rectangle),
		Root(0), Selected(0),
		ItemHeight(0),
		IndentWidth(0),
		TotalItemHeight(0),
		TotalItemWidth(0),
		Font(0),
		IconFont(0),
		ScrollBarH(0),
		ScrollBarV(0),
		ImageList(0),
		LastEventNode(0),
		LinesVisible(true),
		Selecting(false),
		Clip(clip),
		DrawBack(drawBack),
		ImageLeftOfIcon(true)
	{
#ifdef _DEBUG
		setDebugName("CGUITreeView");
#endif

		IGUISkin* skin = Environment->getSkin();
		SINT32 s = skin->getSize(EGDS_SCROLLBAR_SIZE);

		if (scrollBarVertical)
		{
			ScrollBarV = new CGUIScrollBar(false, Environment, this, 0,
				rect<SINT32>(RelativeRect.getWidth() - s,
				0,
				RelativeRect.getWidth(),
				RelativeRect.getHeight() - (scrollBarHorizontal ? s : 0)
				),
				!clip);
			ScrollBarV->drop();

			ScrollBarV->setSubElement(true);
			ScrollBarV->setPos(0);
			ScrollBarV->grab();
		}

		if (scrollBarHorizontal)
		{
			ScrollBarH = new CGUIScrollBar(true, Environment, this, 1,
				rect<SINT32>(0, RelativeRect.getHeight() - s, RelativeRect.getWidth() - s, RelativeRect.getHeight()),
				!clip);
			ScrollBarH->drop();

			ScrollBarH->setSubElement(true);
			ScrollBarH->setPos(0);
			ScrollBarH->grab();
		}

		Root = new CGUITreeViewNode(this, 0);
		Root->Expanded = true;

		recalculateItemHeight();
	}


	//! destructor
	CGUITreeView::~CGUITreeView()
	{
		if (ScrollBarV)
		{
			ScrollBarV->drop();
		}

		if (ScrollBarH)
		{
			ScrollBarH->drop();
		}

		if (Font)
		{
			Font->drop();
		}

		if (IconFont)
		{
			IconFont->drop();
		}

		if (ImageList)
		{
			ImageList->drop();
		}

		if (Root)
		{
			Root->drop();
		}
	}

	void CGUITreeView::recalculateItemHeight()
	{
		IGUISkin*		skin = Environment->getSkin();
		IGUITreeViewNode*	node;

		if (Font != skin->getFont())
		{
			if (Font)
			{
				Font->drop();
			}

			Font = skin->getFont();
			ItemHeight = 0;

			if (Font)
			{
				ItemHeight = Font->getDimension(L"A").Height + 4;
				Font->grab();
			}

			if (IconFont)
			{
				SINT32 height = IconFont->getDimension(L" ").Height;
				if (height > ItemHeight)
				{
					ItemHeight = height;
				}
			}
			if (ImageList)
			{
				if (ImageList->getImageSize().Height + 1 > ItemHeight)
				{
					ItemHeight = ImageList->getImageSize().Height + 1;
				}
			}
		}

		IndentWidth = ItemHeight;
		if (IndentWidth < 9)
		{
			IndentWidth = 9;
		}
		else if (IndentWidth > 15)
		{
			IndentWidth = 15;
		}
		else
		{
			if (((IndentWidth >> 1) << 1) - IndentWidth == 0)
			{
				--IndentWidth;
			}
		}

		TotalItemHeight = 0;
		TotalItemWidth = AbsoluteRect.getWidth() * 2;
		node = Root->getFirstChild();
		while (node)
		{
			TotalItemHeight += ItemHeight;
			node = node->getNextVisible();
		}

		if (ScrollBarV)
			ScrollBarV->setMax(Math::_max(0, TotalItemHeight - AbsoluteRect.getHeight()));

		if (ScrollBarH)
			ScrollBarH->setMax(Math::_max(0, TotalItemWidth - AbsoluteRect.getWidth()));

	}

	//! called if an event happened.
	bool CGUITreeView::OnEvent(const SEvent &event)
	{
		if (isEnabled())
		{
			switch (event.EventType)
			{
			case EET_GUI_EVENT:
				switch (event.GUIEvent.EventType)
				{
				case EGET_SCROLL_BAR_CHANGED:
					if (event.GUIEvent.Caller == ScrollBarV || event.GUIEvent.Caller == ScrollBarH)
					{
						//SINT32 pos = ( ( IGUIScrollBar* )event.GUIEvent.Caller )->getPos();
						return true;
					}
					break;
				case EGET_ELEMENT_FOCUS_LOST:
				{
					Selecting = false;
					return false;
				}
				break;
				default:
					break;
				}
				break;
			case EET_MOUSE_INPUT_EVENT:
			{
				Position2d p(event.MouseInput.X, event.MouseInput.Y);

				switch (event.MouseInput.Event)
				{
				case EMIE_MOUSE_WHEEL:
					if (ScrollBarV)
						ScrollBarV->setPos(ScrollBarV->getPos() + (event.MouseInput.Wheel < 0 ? -1 : 1) * -10);
					return true;
					break;

				case EMIE_LMOUSE_PRESSED_DOWN:

					if (Environment->hasFocus(this) && !AbsoluteClippingRect.isPointInside(p))
					{
						Environment->removeFocus(this);
						return false;
					}

					if (Environment->hasFocus(this) &&
						((ScrollBarV && ScrollBarV->getAbsolutePosition().isPointInside(p) && ScrollBarV->OnEvent(event)) ||
						(ScrollBarH && ScrollBarH->getAbsolutePosition().isPointInside(p) && ScrollBarH->OnEvent(event))
						)
						)
					{
						return true;
					}

					Selecting = true;
					Environment->setFocus(this);
					return true;
					break;

				case EMIE_LMOUSE_LEFT_UP:
					if (Environment->hasFocus(this) &&
						((ScrollBarV && ScrollBarV->getAbsolutePosition().isPointInside(p) && ScrollBarV->OnEvent(event)) ||
						(ScrollBarH && ScrollBarH->getAbsolutePosition().isPointInside(p) && ScrollBarH->OnEvent(event))
						)
						)
					{
						return true;
					}

					Selecting = false;
					Environment->removeFocus(this);
					mouseAction(event.MouseInput.X, event.MouseInput.Y);
					return true;
					break;

				case EMIE_MOUSE_MOVED:
					if (Selecting)
					{
						if (getAbsolutePosition().isPointInside(p))
						{
							mouseAction(event.MouseInput.X, event.MouseInput.Y, true);
							return true;
						}
					}
					break;
				default:
					break;
				}
			}
			break;
			default:
				break;
			}
		}

		return Parent ? Parent->OnEvent(event) : false;
	}

	/*!
	*/
	void CGUITreeView::mouseAction(SINT32 xpos, SINT32 ypos, bool onlyHover /*= false*/)
	{
		IGUITreeViewNode*		oldSelected = Selected;
		IGUITreeViewNode*		hitNode = 0;
		SINT32						selIdx = -1;
		SINT32						n;
		IGUITreeViewNode*		node;
		SEvent					event;

		event.EventType = EET_GUI_EVENT;
		event.GUIEvent.Caller = this;
		event.GUIEvent.Element = 0;

		xpos -= AbsoluteRect.UpperLeftCorner.x;
		ypos -= AbsoluteRect.UpperLeftCorner.y;

		// find new selected item.
		if (ItemHeight != 0 && ScrollBarV)
		{
			selIdx = ((ypos - 1) + ScrollBarV->getPos()) / ItemHeight;
		}

		hitNode = 0;
		node = Root->getFirstChild();
		n = 0;
		while (node)
		{
			if (selIdx == n)
			{
				hitNode = node;
				break;
			}
			node = node->getNextVisible();
			++n;
		}

		if (hitNode && xpos > hitNode->getLevel() * IndentWidth)
		{
			Selected = hitNode;
		}

		if (hitNode && !onlyHover
			&& xpos < hitNode->getLevel() * IndentWidth
			&& xpos >(hitNode->getLevel() - 1) * IndentWidth
			&& hitNode->hasChildren())
		{
			hitNode->setExpanded(!hitNode->getExpanded());

			// post expand/collaps news
			if (hitNode->getExpanded())
			{
				event.GUIEvent.EventType = EGET_TREEVIEW_NODE_EXPAND;
			}
			else
			{
				event.GUIEvent.EventType = EGET_TREEVIEW_NODE_COLLAPS;
			}
			LastEventNode = hitNode;
			Parent->OnEvent(event);
			LastEventNode = 0;
		}

		if (Selected && !Selected->isVisible())
		{
			Selected = 0;
		}

		// post selection news

		if (Parent && !onlyHover && Selected != oldSelected)
		{
			if (oldSelected)
			{
				event.GUIEvent.EventType = EGET_TREEVIEW_NODE_DESELECT;
				LastEventNode = oldSelected;
				Parent->OnEvent(event);
				LastEventNode = 0;
			}
			if (Selected)
			{
				event.GUIEvent.EventType = EGET_TREEVIEW_NODE_SELECT;
				LastEventNode = Selected;
				Parent->OnEvent(event);
				LastEventNode = 0;
			}
		}
	}


	//! draws the element and its children
	void CGUITreeView::draw()
	{
		if (!IsVisible)
		{
			return;
		}

		recalculateItemHeight(); // if the font changed

		IGUISkin* skin = Environment->getSkin();
		IVideoDriver* driver = Environment->getVideoDriver();

		rect<SINT32>* clipRect = 0;
		if (Clip)
		{
			clipRect = &AbsoluteClippingRect;
		}

		// draw background
		rect<SINT32> frameRect(AbsoluteRect);

		if (DrawBack)
		{
			driver->draw2DRectangle(skin->getColor(EGDC_3D_HIGH_LIGHT), frameRect,
				clipRect);
		}

		// draw the border
		frameRect.LowerRightCorner.y = frameRect.UpperLeftCorner.y + 1;
		driver->draw2DRectangle(skin->getColor(EGDC_3D_SHADOW), frameRect,
			clipRect);

		frameRect.LowerRightCorner.y = AbsoluteRect.LowerRightCorner.y;
		frameRect.LowerRightCorner.x = frameRect.UpperLeftCorner.x + 1;
		driver->draw2DRectangle(skin->getColor(EGDC_3D_SHADOW), frameRect,
			clipRect);

		frameRect = AbsoluteRect;
		frameRect.UpperLeftCorner.x = frameRect.LowerRightCorner.x - 1;
		driver->draw2DRectangle(skin->getColor(EGDC_3D_HIGH_LIGHT), frameRect,
			clipRect);

		frameRect = AbsoluteRect;
		frameRect.UpperLeftCorner.y = AbsoluteRect.LowerRightCorner.y - 1;
		frameRect.LowerRightCorner.y = AbsoluteRect.LowerRightCorner.y;
		driver->draw2DRectangle(skin->getColor(EGDC_3D_HIGH_LIGHT), frameRect,
			clipRect);


		// draw items

		rect<SINT32> clientClip(AbsoluteRect);
		clientClip.UpperLeftCorner.y += 1;
		clientClip.UpperLeftCorner.x += 1;
		clientClip.LowerRightCorner.x = AbsoluteRect.LowerRightCorner.x;
		clientClip.LowerRightCorner.y -= 1;

		if (ScrollBarV)
			clientClip.LowerRightCorner.x -= skin->getSize(EGDS_SCROLLBAR_SIZE);
		if (ScrollBarH)
			clientClip.LowerRightCorner.y -= skin->getSize(EGDS_SCROLLBAR_SIZE);

		if (clipRect)
		{
			clientClip.clipAgainst(*clipRect);
		}

		frameRect = AbsoluteRect;
		frameRect.LowerRightCorner.x = AbsoluteRect.LowerRightCorner.x - skin->getSize(EGDS_SCROLLBAR_SIZE);
		frameRect.LowerRightCorner.y = AbsoluteRect.UpperLeftCorner.y + ItemHeight;

		if (ScrollBarV)
		{
			frameRect.UpperLeftCorner.y -= ScrollBarV->getPos();
			frameRect.LowerRightCorner.y -= ScrollBarV->getPos();
		}

		if (ScrollBarH)
		{
			frameRect.UpperLeftCorner.x -= ScrollBarH->getPos();
			frameRect.LowerRightCorner.x -= ScrollBarH->getPos();
		}

		IGUITreeViewNode* node = Root->getFirstChild();
		while (node)
		{
			frameRect.UpperLeftCorner.x = AbsoluteRect.UpperLeftCorner.x + 1 + node->getLevel() * IndentWidth;

			if (frameRect.LowerRightCorner.y >= AbsoluteRect.UpperLeftCorner.y
				&& frameRect.UpperLeftCorner.y <= AbsoluteRect.LowerRightCorner.y)
			{
				if (node == Selected)
				{
					driver->draw2DRectangle(skin->getColor(EGDC_HIGH_LIGHT), frameRect, &clientClip);
				}

				if (node->hasChildren())
				{
					rect<SINT32> rc;
					rect<SINT32> expanderRect;

					expanderRect.UpperLeftCorner.x = frameRect.UpperLeftCorner.x - IndentWidth + 2;
					expanderRect.UpperLeftCorner.y = frameRect.UpperLeftCorner.y + ((frameRect.getHeight() - (IndentWidth - 4)) >> 1);
					expanderRect.LowerRightCorner.x = expanderRect.UpperLeftCorner.x + IndentWidth - 4;
					expanderRect.LowerRightCorner.y = expanderRect.UpperLeftCorner.y + IndentWidth - 4;

					// box upper line
					rc.UpperLeftCorner.x = expanderRect.UpperLeftCorner.x;
					rc.UpperLeftCorner.y = expanderRect.UpperLeftCorner.y;
					rc.LowerRightCorner.x = expanderRect.LowerRightCorner.x;
					rc.LowerRightCorner.y = rc.UpperLeftCorner.y + 1;
					driver->draw2DRectangle(skin->getColor(EGDC_3D_SHADOW), rc,
						clipRect);

					// box left line
					rc.UpperLeftCorner.x = expanderRect.UpperLeftCorner.x;
					rc.UpperLeftCorner.y = expanderRect.UpperLeftCorner.y;
					rc.LowerRightCorner.x = rc.UpperLeftCorner.x + 1;
					rc.LowerRightCorner.y = expanderRect.LowerRightCorner.y;
					driver->draw2DRectangle(skin->getColor(EGDC_3D_SHADOW), rc,
						clipRect);

					// box right line
					rc.UpperLeftCorner.x = expanderRect.LowerRightCorner.x - 1;
					rc.UpperLeftCorner.y = expanderRect.UpperLeftCorner.y;
					rc.LowerRightCorner.x = rc.UpperLeftCorner.x + 1;
					rc.LowerRightCorner.y = expanderRect.LowerRightCorner.y;
					driver->draw2DRectangle(skin->getColor(EGDC_3D_SHADOW), rc,
						clipRect);

					// box bottom line
					rc.UpperLeftCorner.x = expanderRect.UpperLeftCorner.x;
					rc.UpperLeftCorner.y = expanderRect.LowerRightCorner.y - 1;
					rc.LowerRightCorner.x = expanderRect.LowerRightCorner.x;
					rc.LowerRightCorner.y = rc.UpperLeftCorner.y + 1;
					driver->draw2DRectangle(skin->getColor(EGDC_3D_SHADOW), rc,
						clipRect);

					// horizontal '-' line
					rc.UpperLeftCorner.x = expanderRect.UpperLeftCorner.x + 2;
					rc.UpperLeftCorner.y = expanderRect.UpperLeftCorner.y + (expanderRect.getHeight() >> 1);
					rc.LowerRightCorner.x = rc.UpperLeftCorner.x + expanderRect.getWidth() - 4;
					rc.LowerRightCorner.y = rc.UpperLeftCorner.y + 1;
					driver->draw2DRectangle(skin->getColor(EGDC_BUTTON_TEXT), rc,
						clipRect);

					if (!node->getExpanded())
					{
						// vertical '+' line
						rc.UpperLeftCorner.x = expanderRect.UpperLeftCorner.x + (expanderRect.getWidth() >> 1);
						rc.UpperLeftCorner.y = expanderRect.UpperLeftCorner.y + 2;
						rc.LowerRightCorner.x = rc.UpperLeftCorner.x + 1;
						rc.LowerRightCorner.y = rc.UpperLeftCorner.y + expanderRect.getHeight() - 4;
						driver->draw2DRectangle(skin->getColor(EGDC_BUTTON_TEXT), rc,
							clipRect);
					}
				}

				rect<SINT32> textRect = frameRect;

				if (Font)
				{
					EGUI_DEFAULT_COLOR textCol = EGDC_GRAY_TEXT;
					if (isEnabled())
						textCol = (node == Selected) ? EGDC_HIGH_LIGHT_TEXT : EGDC_BUTTON_TEXT;

					SINT32 iconWidth = 0;
					for (SINT32 n = 0; n < 2; ++n)
					{
						SINT32 index = node->getImageIndex();
						if ((ImageList && index >= 0)
							&& ((ImageLeftOfIcon && n == 0)
							|| (!ImageLeftOfIcon && n == 1)))
						{
							index = node->getSelectedImageIndex();
							if (node != Selected || index < 0)
							{
								index = node->getImageIndex();
							}
							ImageList->draw(
								index,
								Position2d(
								textRect.UpperLeftCorner.x,
								textRect.UpperLeftCorner.y + ((textRect.getHeight() - ImageList->getImageSize().Height) >> 1)),
								&clientClip);
							iconWidth += ImageList->getImageSize().Width + 3;
							textRect.UpperLeftCorner.x += ImageList->getImageSize().Width + 3;
						}
						else if ((IconFont && reinterpret_cast<CGUITreeViewNode*>(node)->Icon.size())
							&& ((ImageLeftOfIcon && n == 1)
							|| (!ImageLeftOfIcon && n == 0)))
						{
							IconFont->draw(node->getIcon(), textRect, skin->getColor(textCol), false, true, &clientClip);
							iconWidth += IconFont->getDimension(node->getIcon()).Width + 3;
							textRect.UpperLeftCorner.x += IconFont->getDimension(node->getIcon()).Width + 3;
						}
					}

					Font->draw(node->getText(), textRect, skin->getColor(textCol), false, true, &clientClip);

					textRect.UpperLeftCorner.x -= iconWidth;
				}

				// draw the lines if neccessary
				if (LinesVisible)
				{
					rect<SINT32> rc;

					// horizontal line
					rc.UpperLeftCorner.x = frameRect.UpperLeftCorner.x - IndentWidth - (IndentWidth >> 1) - 1;
					rc.UpperLeftCorner.y = frameRect.UpperLeftCorner.y + ((frameRect.getHeight()) >> 1);
					if (node->hasChildren())
					{
						rc.LowerRightCorner.x = frameRect.UpperLeftCorner.x - IndentWidth;
					}
					else
					{
						rc.LowerRightCorner.x = frameRect.UpperLeftCorner.x - 2;
					}
					rc.LowerRightCorner.y = rc.UpperLeftCorner.y + 1;
					driver->draw2DRectangle(skin->getColor(EGDC_3D_SHADOW), rc,
						clipRect);

					if (node->getParent() != Root)
					{
						// vertical line
						if (node == node->getParent()->getFirstChild())
						{
							rc.UpperLeftCorner.y = frameRect.UpperLeftCorner.y - ((frameRect.getHeight() - IndentWidth) >> 1);
						}
						else
						{
							rc.UpperLeftCorner.y = frameRect.UpperLeftCorner.y - (frameRect.getHeight() >> 1);
						}
						rc.LowerRightCorner.x = rc.UpperLeftCorner.x + 1;
						driver->draw2DRectangle(skin->getColor(EGDC_3D_SHADOW), rc,
							clipRect);

						// the vertical lines of all parents
						IGUITreeViewNode* nodeTmp = node->getParent();
						rc.UpperLeftCorner.y = frameRect.UpperLeftCorner.y - (frameRect.getHeight() >> 1);
						for (SINT32 n = 0; n < node->getLevel() - 2; ++n)
						{
							rc.UpperLeftCorner.x -= IndentWidth;
							rc.LowerRightCorner.x -= IndentWidth;
							if (nodeTmp != nodeTmp->getParent()->getLastChild())
							{
								driver->draw2DRectangle(skin->getColor(EGDC_3D_SHADOW), rc,
									clipRect);
							}
							nodeTmp = nodeTmp->getParent();
						}
					}
				}
			}

			frameRect.UpperLeftCorner.y += ItemHeight;
			frameRect.LowerRightCorner.y += ItemHeight;

			node = node->getNextVisible();
		}

		IGUIElement::draw();
	}

	//! Sets the font which should be used as icon font. This font is set to the Irrlicht engine
	//! built-in-font by default. Icons can be displayed in front of every list item.
	//! An icon is a string, displayed with the icon font. When using the build-in-font of the
	//! Irrlicht engine as icon font, the icon strings defined in GUIIcons.h can be used.
	void CGUITreeView::setIconFont(IGUIFont* font)
	{
		SINT32	height;

		if (font)
			font->grab();
		if (IconFont)
		{
			IconFont->drop();
		}

		IconFont = font;
		if (IconFont)
		{
			height = IconFont->getDimension(L" ").Height;
			if (height > ItemHeight)
			{
				ItemHeight = height;
			}
		}
	}

	//! Sets the image list which should be used for the image and selected image of every node.
	//! The default is 0 (no images).
	void CGUITreeView::setImageList(IGUIImageList* imageList)
	{
		if (imageList)
			imageList->grab();
		if (ImageList)
		{
			ImageList->drop();
		}

		ImageList = imageList;
		if (ImageList)
		{
			if (ImageList->getImageSize().Height + 1 > ItemHeight)
			{
				ItemHeight = ImageList->getImageSize().Height + 1;
			}
		}
	}
}

#endif