#include "SFML/Graphics.hpp"
#include "Core/Utils.h"
#include "Core/Logger.h"
#include "Renderer.h"
#include "IRenderBuffer.h"
#include "SFMLAPI/Rendering/Shader.h"
#include "SFMLAPI/Viewport/Viewport.h"
#include "SFMLAPI/Viewport/ViewportData.h"

#include "SFMLAPI/Rendering/Primitives/Primitive.h"

namespace LittleEngine
{
#if ENABLED(RENDER_STATS)
RenderData g_renderData;
#endif

Renderer::Renderer(Viewport& viewport) : m_pViewport(&viewport)
{
	m_bRendering.store(true);
}

Renderer::~Renderer() = default;

void Renderer::RenderFrame(IRenderBuffer& buffer, Fixed alpha)
{
	m_pViewport->clear();
#if ENABLED(RENDER_STATS)
	static Time fpsTime;
	static u32 frameCount = 0;
	u32 statics = 0;
	u32 drawCalls = 0;
	g_renderData._quadCount_Internal = 0;
#endif
	auto& matrix = buffer.ActiveRenderMatrix();

	for (auto& vec : matrix)
	{
		for (auto& pPrimitive : vec)
		{
			if (!pPrimitive->m_bDestroyed && pPrimitive->m_renderState.bEnabled)
			{
				pPrimitive->Draw(*m_pViewport, alpha);
#if ENABLED(RENDER_STATS)
				++drawCalls;
#endif
			}
#if ENABLED(RENDER_STATS)

			if (pPrimitive->IsEnabled() && pPrimitive->IsStatic())
			{
				++statics;
			}
#endif
		}
	}
#if ENABLED(RENDER_STATS)
	g_renderData.quadCount = g_renderData._quadCount_Internal;
	g_renderData.staticCount = statics;
	g_renderData.drawCallCount = drawCalls;
	g_renderData.dynamicCount = drawCalls - statics;
	++g_renderData.renderFrame;
	// Update FPS
	{
		++frameCount;
		g_renderData.lastRenderTime = Time::Now();
		if (Maths::Abs((fpsTime - g_renderData.lastRenderTime).AsSeconds()) >= 1.0f)
		{
			fpsTime = g_renderData.lastRenderTime;
			g_renderData.framesPerSecond = frameCount;
			frameCount = 0;
		}
	}
#endif
}

void Renderer::WaitForVsync()
{
	Assert(m_pViewport, "Viewport is null!");
	m_pViewport->display();
}
} // namespace LittleEngine
