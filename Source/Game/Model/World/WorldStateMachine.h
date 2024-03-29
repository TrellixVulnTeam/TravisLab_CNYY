#pragma once
#include "Core/Delegate.h"
#include "World.h"

namespace LittleEngine
{
class WorldStateMachine final
{
private:
	Core::Delegate<>::Token m_onSubmitToken;
	LEInput::Token m_inputToken;
	class LEContext* m_pContext;
	static Vec<UPtr<class World>> s_createdWorlds;

#ifdef DEBUGGING
public:
	static bool s_bRunning;
#endif

public:
	template <typename T>
	static WorldID CreateWorld();
	template <typename T>
	static void CreateWorlds();
	template <typename T, typename U, typename... V>
	static void CreateWorlds();

public:
	WorldStateMachine(LEContext& context);
	~WorldStateMachine();

	void Start(String coreManifestID = "", String gameStyleID = "", Task onManifestLoaded = nullptr);
	void Tick(Time dt, bool& bYieldIntegration);
	bool LoadWorld(WorldID id);
	void SetLoadingHUD(UPtr<class ILoadingHUD> uLoadingHUD);

	World* ActiveWorld() const;
	WorldID ActiveWorldID() const;
	Vec<WorldID> AllWorldIDs() const;

private:
	void ChangeState();

	void UnloadActiveWorld();
	void Quit();

	friend class GameManager;
	friend class World;
};

template <typename T>
WorldID WorldStateMachine::CreateWorld()
{
	WorldID id = -1;
	static_assert(IsDerived<World, T>(), "T must derive from World!");
	UPtr<T> uWorld = MakeUnique<T>();
	if (uWorld)
	{
		id = static_cast<s32>(s_createdWorlds.size());
		uWorld->m_id = id;
		s_createdWorlds.push_back(std::move(uWorld));
	}
	return id;
}

template <typename T>
void WorldStateMachine::CreateWorlds()
{
	CreateWorld<T>();
}

template <typename T, typename U, typename... V>
void WorldStateMachine::CreateWorlds()
{
	CreateWorld<T>();
	CreateWorlds<U, V...>();
}
} // namespace LittleEngine
