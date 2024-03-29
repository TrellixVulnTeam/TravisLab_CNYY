#include "Core/Logger.h"
#include "UIManager.h"
#include "Model/GameManager.h"

namespace LittleEngine
{
UIManager::UIManager()
{
	LOG_D("[UIManager] constructed");
}

UIManager::~UIManager()
{
	Reset();
	m_inactive.clear();
	LOG_D("[UIManager] destroyed");
}

UIContext* UIManager::Active() const
{
	return m_contexts.empty() ? nullptr : m_contexts.back().get();
}

void UIManager::Reset()
{
	UIContext* pActive = Active();
	if (pActive)
	{
		if (pActive->IsPersistent())
		{
			DisableContext(*pActive);
			m_inactive[pActive->m_name] = std::move(m_contexts.back());
		}
	}
	m_contexts.clear();
}

void UIManager::Tick(Time dt)
{
	UIContext* pActive = Active();
	if (pActive)
	{
		if (pActive->m_bDestroyed)
		{
			if (pActive->IsPersistent())
			{
				DisableContext(*pActive);
				m_inactive[pActive->m_name] = std::move(m_contexts.back());
			}
			m_contexts.pop_back();
			if (!m_contexts.empty())
			{
				auto& uContext = m_contexts.back();
				uContext->SetEnabled(true);
				uContext->SetActive(true);
			}
		}
	}

	auto iter = m_contexts.begin();
	while (iter != m_contexts.end())
	{
		UIContext* pContext = iter->get();
		if (pContext->m_bDestroyed)
		{
			if (pContext->IsPersistent())
			{
				DisableContext(*pContext);
				m_inactive[pContext->m_name] = std::move(*iter);
			}
			iter = m_contexts.erase(iter);
		}
		else
		{
			pContext->Tick(dt);
			++iter;
		}
	}
}

void UIManager::InitContext(UIContext& context, String id, LayerID baseLayer)
{
	context.OnCreate(std::move(id), baseLayer);
}

void UIManager::DisableContext(UIContext& context)
{
	context.SetActive(false);
	context.SetEnabled(false);
}
} // namespace LittleEngine
