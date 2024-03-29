#include "Core/Logger.h"
#include "Core/Utils.h"
#include "SFMLAPI/System/Assets.h"
#include "Engine/Audio/LEAudio.h"
#include "Engine/Context/LEContext.h"
#include "Engine/Repository/ManifestLoader.h"
#include "Engine/Repository/LERepository.h"
#include "World.h"
#include "WorldClock.h"
#include "WorldStateMachine.h"
#include "Model/GameManager.h"
#include "Model/UI/UIManager.h"

namespace LittleEngine
{
World::World(String name) : GameObject(std::move(name), "World")
{
	m_uWorldClock = MakeUnique<WorldClock>();
	m_manifestID = m_name + ".amf";
}

World::~World() = default;

bool World::LoadWorld(WorldID id)
{
	Assert(m_pWSM, "WSM is null!");
	return m_pWSM->LoadWorld(id);
}

void World::Quit()
{
	g_pGameManager->Quit();
}

Time World::WorldTime() const
{
	return m_uWorldClock->Elapsed();
}

LEInput* World::Input() const
{
	Assert(m_pWSM, "WSM is null!");
	return m_pWSM->m_pContext->Input();
}

void World::Init(WorldStateMachine& wsm)
{
	m_pWSM = &wsm;
	m_state = State::Inactive;
}

void World::Activate()
{
	Assert(m_pWSM, "WSM is null!");
	m_state = State::Active;
	m_uWorldClock->Restart();
	OnActivated();
	LOG_D("%s Activated", LogNameStr());
}

void World::Deactivate()
{
	m_tokenHandler.Clear();
	OnDeactivating();
	m_state = State::Inactive;
	LOG_D("%s Deactivated", LogNameStr());
}
} // namespace LittleEngine
