#include "Core/Logger.h"
#include "SFMLAPI/Rendering/Primitives/SFRect.h"
#include "SFMLAPI/Rendering/Primitives/SFText.h"
#include "UIWidget.h"
#include "Model/GameManager.h"
#include "Model/UI/UIGameStyle.h"
#include "Model/UI/UITransform.h"
#include "UIContext.h"

namespace LittleEngine
{
UIWidget::UIWidget() = default;
UIWidget::~UIWidget() = default;

UIWidgetStyle& UIWidget::GetStyle()
{
	return m_style;
}

UIElement* UIWidget::Root() const
{
	return m_pRoot;
}

void UIWidget::SetStyle(const UIWidgetStyle& style)
{
	m_style = style;
}

void UIWidget::SetInteractable(bool bInteractable)
{
	SetState(bInteractable ? UIWidgetState::NotSelected : UIWidgetState::Uninteractable);
	OnSetInteractable(bInteractable);
}

bool UIWidget::IsInteractable() const
{
	return m_state != UIWidgetState::Uninteractable;
}

void UIWidget::SetState(UIWidgetState state)
{
	m_prevState = m_state == UIWidgetState::Interacting ? state : m_state;
	m_state = state;
}

bool UIWidget::IsPointInBounds(Vector2 point) const
{
	return Root()->Rect()->RenderBounds().IsPointIn(point);
}

void UIWidget::OnCreated() {}

void UIWidget::Tick(Time dt)
{
	if (m_bDestroyed)
	{
		return;
	}
	for (auto& uUIelement : m_uiElements)
	{
		uUIelement->Tick(dt);
	}
}

void UIWidget::Select()
{
	if (IsInteractable())
	{
		SetState(UIWidgetState::Selected);
		OnSelected();
	}
}

void UIWidget::Deselect()
{
	if (IsInteractable())
	{
		SetState(UIWidgetState::NotSelected);
		OnDeselected();
	}
}

void UIWidget::InteractStart()
{
	if (IsInteractable())
	{
		SetState(UIWidgetState::Interacting);
		OnInteractStart();
	}
}

void UIWidget::InteractEnd(bool bInteract)
{
	if (IsInteractable())
	{
		SetState(UIWidgetState::Selected);
		OnInteractEnd(bInteract);
	}
}

void UIWidget::OnCreate(String name, UIContext& owner, UIWidgetStyle* pStyleToCopy)
{
	SetNameAndType(std::move(name), "UIWidget");
	m_pOwner = &owner;
	m_style = pStyleToCopy ? *pStyleToCopy : UIGameStyle::GetStyle("");
	m_pRoot = AddElement<UIElement>(String(NameStr()) + "_Root");
	m_pRoot->m_transform.size = m_style.widgetSize;
	OnCreated();
	SetInteractable(true);
}

void UIWidget::InitElement(String name, UIElement* pNewElement, UITransform* pParent)
{
	if (!pParent)
	{
		pParent = m_pOwner ? &m_pOwner->Root()->m_transform : nullptr;
	}
	pNewElement->OnCreate(*g_pGameManager->Context(), std::move(name), pParent, m_style.pFont);
}

LayerID UIWidget::MaxLayer() const
{
	LayerID maxLayer = LAYER_ZERO;
	for (const auto& uElement : m_uiElements)
	{
		if (uElement->m_layer > maxLayer)
		{
			maxLayer = uElement->m_layer;
		}
	}
	return maxLayer;
}
} // namespace LittleEngine
