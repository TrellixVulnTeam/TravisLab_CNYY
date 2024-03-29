#pragma once
#include <memory>
#include "SFML/Graphics/RenderWindow.hpp"
#include "ViewportData.h"
#include "Core/CoreTypes.h"

namespace LittleEngine
{
// \brief: Derives from sf::RenderWindow
// Implementation provides SFML libs for linker via pragma
class Viewport final : public sf::RenderWindow
{
private:
	ViewportData m_data;
	Rect2 m_viewBounds;
	Vector2 m_viewSize;

public:
	static ViewportSize MaxSize();

public:
	Viewport();
	~Viewport() override;

	void SetData(ViewportData data);
	void Create(u32 framerateLimit = 300);
	void Destroy();
	void OverrideData(ViewportRecreateData data);

	Vector2 ViewSize() const;
	// Projects unit Rect to viewport
	Vector2 Project(Vector2 nPos, bool bPreClamp) const;
	Vector2 ViewportToWorld(s32 vpX, s32 vpY) const;
};
} // namespace LittleEngine
