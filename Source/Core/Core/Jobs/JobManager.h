#pragma once
#include <optional>
#include "Core/CoreTypes.h"
#include "JobHandle.h"

namespace Core
{
class JobManager final
{
public:
	static constexpr s32 INVALID_ID = -1;

private:
	class Job
	{
	private:
		std::promise<void> m_promise;
		Array<char, 256> szLogName;

	public:
		JobHandle m_sHandle;
		Task m_task;
		s64 m_id;
		bool m_bSilent = false;
		const char* m_szException = nullptr;

	public:
		Job();
		Job(s64 id, Task task, String name, bool bSilent);
		Job(Job&& rhs) = default;
		Job& operator=(Job&& rhs) = default;

		const char* ToStr() const;

		void Run();
		void Fulfil();
	};

private:
	Vec<UPtr<class JobWorker>> m_jobWorkers;
	List<UPtr<class JobCatalog>> m_catalogs;
	List<Job> m_jobQueue;
	mutable std::mutex m_queueMutex;
	s64 m_nextGameJobID = 0;

public:
	JobManager(u32 workerCount);
	~JobManager();

public:
	JobHandle Enqueue(Task task, String name = "", bool bSilent = false);
	JobCatalog* CreateCatalog(String name);
	void ForEach(std::function<void(size_t)> indexedTask, size_t iterationCount, size_t iterationsPerJob, size_t startIdx = 0);

	void Update();
	bool AreWorkersIdle() const;

private:
	std::optional<Job> Lock_PopJob();

	friend class JobWorker;
};
} // namespace Core
