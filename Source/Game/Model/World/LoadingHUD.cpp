#include "Core/Logger.h"
#include "SFMLAPI/System/Assets.h"
#include "SFMLAPI/Rendering/Primitives/Quad.h"
#include "SFMLAPI/Rendering/Primitives/SFRect.h"
#include "SFMLAPI/Rendering/Primitives/SFText.h"
#include "Engine/Context/LEContext.h"
#include "Engine/Renderer/LERenderer.h"
#include "Model/GameManager.h"
#include "Model/UI/UIElement.h"
#include "LoadingHUD.h"

namespace LittleEngine
{
namespace
{
Vector2 progressBarSize;
} // namespace

LoadingHUD::LoadingHUD()
{
	Vector2 viewSize = g_pGameManager->Renderer()->ViewSize();
	Vector2 halfView = viewSize * Fixed::OneHalf;
	LayerID bg = LAYER_TOP;
	LayerID hud = static_cast<LayerID>(bg + 1);
	auto pMainFont = g_pRepository->DefaultFont();
	auto pTitleFont = PreloadAsset<FontAsset>("Fonts/Sunscreen.otf");
	auto pSubtitleFont = PreloadAsset<FontAsset>("Fonts/UIFont.ttf");
	auto pTexture = PreloadAsset<TextureAsset>("Textures/LoadingIcon.png");

	m_pBG = g_pGameManager->Renderer()->New<Quad>(bg);
	m_pBG->SetModel(Rect2::SizeCentre(viewSize));
	m_pBG->SetPrimaryColour(Colour(20, 5, 30, 0))->SetEnabled(true);

	m_pTitle = g_pGameManager->Renderer()->New<SFText>(hud);
	m_pSubtitle = g_pGameManager->Renderer()->New<SFText>(hud);
	m_pTitle->SetFont(pTitleFont ? *pTitleFont : *pMainFont);
	m_pSubtitle->SetFont(pSubtitleFont ? *pSubtitleFont : pTitleFont ? *pTitleFont : *pMainFont);
	m_pTitle->SetPosition({0, 70})->SetEnabled(true);
	m_pSubtitle->SetPosition({0, -30})->SetEnabled(true);

	progressBarSize = {viewSize.x, 10};
	m_pProgressBar = g_pGameManager->Renderer()->New<SFRect>(hud);
	m_pProgressBar->SetSize({Fixed::Zero, progressBarSize.y})->SetPivot({-1, 0})->SetPrimaryColour(Colour::White)->SetEnabled(true);
	Fixed yPad = progressBarSize.y * Fixed::OneHalf;
	m_pProgressBar->SetPosition({-halfView.x, -halfView.y + yPad}, true);

	Vector2 pos = halfView - Vector2(100, 100);
	m_pRotator = g_pGameManager->Renderer()->New<Quad>(hud);
	m_pRotator->SetModel(Rect2::SizeCentre({75, 75}))->SetPosition(pos, true)->SetEnabled(true);
	if (pTexture)
	{
		m_pRotator->SetTexture(*pTexture);
	}

	SetEnabled(false);
	LOG_D("[Loading UI] constructed");
}

LoadingHUD::~LoadingHUD()
{
	m_pProgressBar->Destroy();
	m_pBG->Destroy();
	m_pTitle->Destroy();
	m_pSubtitle->Destroy();
	m_pRotator->Destroy();
	LOG_D("[Loading UI] destroyed");
}

SFText& LoadingHUD::Title() const
{
	return *m_pTitle;
}

SFText* LoadingHUD::Subtitle() const
{
	return m_pSubtitle;
}

Quad* LoadingHUD::Spinner() const
{
	return m_pRotator;
}

void LoadingHUD::Tick(Time dt, Fixed progress)
{
	m_alpha = Maths::Lerp(m_alpha, m_alphaTarget, Fixed(dt.AsSeconds() * 8));
	m_pProgressBar->SetSize({progress * progressBarSize.x, progressBarSize.y}, false);

	m_elapsed += dt;
	m_angle -= Fixed(dt.AsSeconds() * 200.0f);
	m_pRotator->SetOrientation(Vector2::ToOrientation(m_angle));

	Update(false);
}

void LoadingHUD::Reset()
{
	auto pTexture = g_pRepository->Load<TextureAsset>("Textures/LoadingIcon.png");
	if (pTexture)
	{
		m_pRotator->SetTexture(*pTexture);
	}
	m_pProgressBar->SetSize({Fixed::Zero, progressBarSize.y});
}

void LoadingHUD::SetEnabled(bool bEnabled)
{
	if (bEnabled)
	{
		m_alpha = m_alphaTarget = Fixed::One;
	}
	else
	{
		m_alphaTarget = Fixed::Zero;
	}
	Update(true);
}

void LoadingHUD::Update(bool bImmediate)
{
	UByte alpha(static_cast<u8>((m_alpha * 255).ToU32()));
	if (alpha.rawValue < 5)
	{
		alpha.rawValue = 0;
	}
	else if (alpha.rawValue > 250)
	{
		alpha.rawValue = 255;
	}
	m_pBG->SetPrimaryAlpha(alpha, bImmediate);
	m_pProgressBar->SetPrimaryAlpha(alpha, bImmediate);
	m_pTitle->SetPrimaryAlpha(alpha, bImmediate);
	m_pSubtitle->SetPrimaryAlpha(alpha, bImmediate);
	m_pRotator->SetPrimaryAlpha(alpha, bImmediate);
}
} // namespace LittleEngine
