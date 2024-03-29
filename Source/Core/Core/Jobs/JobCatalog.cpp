#include "JobManager.h"
#include "JobCatalog.h"
#include "Core/Logger.h"
#include "Core/Utils.h"

namespace Core
{
JobCatalog::JobCatalog(JobManager& manager, String name) : m_logName("[" + std::move(name) + "]"), m_pManager(&manager) {}

void JobCatalog::AddJob(Task job, String name)
{
	if (name.empty())
	{
		name = "JobCatalog_" + Strings::ToString(m_subJobs.size());
	}
	m_subJobs.emplace_back(std::move(name), std::move(job));
}

void JobCatalog::StartJobs(Task onComplete)
{
	LOG_D("%s started. Running and monitoring %d jobs", LogNameStr(), m_subJobs.size());
	m_onComplete = onComplete;
	m_bCompleted = false;
	m_startTime = Time::Now();
	for (auto& job : m_subJobs)
	{
		m_pendingJobs.push_back(m_pManager->Enqueue(std::move(job.second), std::move(job.first)));
	}
}

Fixed JobCatalog::Progress() const
{
	u32 done = static_cast<u32>(m_subJobs.size() - m_pendingJobs.size());
	return Fixed(static_cast<s32>(done), static_cast<s32>(m_subJobs.size()));
}

void JobCatalog::Update()
{
	auto iter = m_pendingJobs.begin();
	while (iter != m_pendingJobs.end())
	{
		const auto& subJob = *iter;
		if (subJob->HasCompleted())
		{
#if ENABLED(DEBUG_LOGGING)
			auto id = subJob->ID();
#endif
			iter = m_pendingJobs.erase(iter);
			LOG_D("%s Job %d completed. %d jobs remaining", LogNameStr(), id, m_pendingJobs.size());
		}
		else
		{
			++iter;
		}
	}

	if (m_pendingJobs.empty() && !m_bCompleted)
	{
		if (m_onComplete)
		{
			m_onComplete();
			m_onComplete = nullptr;
		}
		m_bCompleted = true;
#if ENABLED(DEBUG_LOGGING)
		f32 secs = (Time::Now() - m_startTime).AsSeconds();
		LOG_D("%s completed %d jobs in %.2fs", LogNameStr(), m_subJobs.size(), secs);
#endif
	}
}

const char* JobCatalog::LogNameStr() const
{
	return m_logName.c_str();
}
} // namespace Core
