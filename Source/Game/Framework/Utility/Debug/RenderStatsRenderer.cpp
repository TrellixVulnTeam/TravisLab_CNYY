#include "Core/Version.h"
#include "RenderStatsRenderer.h"
#if ENABLED(RENDER_STATS)
#include "SFMLAPI/Rendering/Renderer.h"
#include "SFMLAPI/Rendering/Primitives.h"
#include "Engine/Debug/Tweakable.h"
#include "Engine/Context/LEContext.h"
#include "Engine/Renderer/LERenderer.h"
#include "Engine/Repository/LERepository.h"

namespace LittleEngine::Debug
{
namespace
{
u16 textSize = 18;
} // namespace

bool RenderStatsRenderer::s_bConsoleRenderStatsEnabled = false;
TweakBool(renderStats, &RenderStatsRenderer::s_bConsoleRenderStatsEnabled);

RenderStatsRenderer::RenderStatsRenderer(LEContext& context)
{
	Fixed nY(-55, 100);
	Fixed x(68, 100);
	Fixed dx(17, 100);
	Vector2 size(300, 220);
	Vector2 pad(-40, 40);
	Colour bg(20, 20, 20, 230);
	LERenderer* pRenderer = context.Renderer();
	LayerID layer = static_cast<LayerID>(LAYER_TOP + 2);
	auto pFont = g_pRepository->DefaultFont();

	m_pBG = pRenderer->New<SFRect>(static_cast<LayerID>(layer - 1));
	Vector2 pos = pRenderer->Project({1, -1}, false) + pad;
	m_pBG->SetSize(size)
		->SetPivot({1, -1}, true)
		->SetPosition(pos, true)
		->SetPrimaryColour(bg, true)
		->SetStatic(true)
		->SetEnabled(s_bConsoleRenderStatsEnabled);

	m_pTitles = pRenderer->New<SFText>(layer);
	m_pTitles->SetText("Quads\nDraw Calls\nDynamic\nStatic\nTicks/s\nFPS\nGame Frame\nRender Frame")
		->SetSize(textSize)
		->SetFont(*pFont)
		->SetPivot({-1, 1}, true)
		->SetPrimaryColour(g_logTextColour, true)
		->SetPosition(pRenderer->Project({x, nY}, false), true)
		->SetStatic(true)
		->SetEnabled(s_bConsoleRenderStatsEnabled);

	m_pValues = pRenderer->New<SFText>(layer);
	m_pValues->SetFont(*pFont)
		->SetSize(textSize)
		->SetPivot({-1, 1}, true)
		->SetPrimaryColour(g_logTextColour, true)
		->SetPosition(pRenderer->Project({x + dx, nY}, false), true)
		->SetEnabled(s_bConsoleRenderStatsEnabled);
}

RenderStatsRenderer::~RenderStatsRenderer()
{
	m_pBG->Destroy();
	m_pTitles->Destroy();
	m_pValues->Destroy();
}

void RenderStatsRenderer::Tick(Time /*dt*/)
{
	m_pBG->SetEnabled(s_bConsoleRenderStatsEnabled);
	m_pTitles->SetEnabled(s_bConsoleRenderStatsEnabled);
	m_pValues->SetEnabled(s_bConsoleRenderStatsEnabled);
	if (s_bConsoleRenderStatsEnabled)
	{
		u32 ticksPerSec = static_cast<u32>(1.0f / g_renderData.tickRate.AsSeconds());
		static char buf[256];
		SPRINTF(buf, sizeof(buf), "%u\n%u\n%u\n%u\n%u\n%u/%u\n%u\n%u", g_renderData.quadCount, g_renderData.drawCallCount,
				g_renderData.dynamicCount, g_renderData.staticCount, ticksPerSec, g_renderData.framesPerSecond, g_renderData.fpsMax,
				g_renderData.gameFrame, g_renderData.renderFrame);
		m_pValues->SetText(buf);
	}
}

#if defined(DEBUGGING)
VersionRenderer::VersionRenderer(LEContext& context)
{
	LERenderer* pRenderer = context.Renderer();
	auto pFont = g_pRepository->Load<FontAsset>("Fonts/UIFont.ttf");
	m_pBuildVersion = pRenderer->New<SFText>(LAYER_TOP);
	m_pBuildVersion->SetText(Core::Version::szBUILD_VERSION_WITH_COMMIT)
		->SetSize(textSize + 5U)
		->SetFont(pFont ? *pFont : *g_pRepository->DefaultFont())
		->SetPivot({-1, 0})
		->SetPosition(pRenderer->Project({-Fixed(0.95f), -Fixed(0.90f)}, false))
		->SetPrimaryColour(g_logTextColour)
		->SetEnabled(true);
}

VersionRenderer::~VersionRenderer()
{
	m_pBuildVersion->Destroy();
}
#endif
} // namespace LittleEngine::Debug
#endif
