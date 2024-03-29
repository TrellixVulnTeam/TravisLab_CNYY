#pragma once
#include "Core/CoreTypes.h"
#include "SFML/Graphics.hpp"
#include "SFMLAPI/Input/InputStateMachine.h"
#include "SFMLAPI/Rendering/Colour.h"
#include "SFMLAPI/Viewport/ViewportData.h"

namespace LittleEngine
{
f32 Cast(Fixed fixed);
Fixed Cast(f32 value);

sf::Vector2f Cast(Vector2 vec2);
template <typename T>
Vector2 Cast(const sf::Vector2<T>& vec2);

sf::Color Cast(Colour colour);
Colour Cast(const sf::Color& sfColor);

sf::Uint8 Cast(ViewportStyle style);

KeyType Cast(sf::Mouse::Button sfButton);

Vector2 WorldToViewport(Vector2 worldPoint);
Fixed WorldToViewport(Fixed worldOrientation);
Vector2 ViewportToWorld(Vector2 vpPoint);
Fixed ViewportToWorld(Fixed vpOrientation);

inline Vector2 Cast(const sf::Vector2<u32> vec2u)
{
	return Vector2(static_cast<s32>(vec2u.x), static_cast<s32>(vec2u.y));
}

inline Vector2 Cast(const sf::Vector2i& vec2)
{
	return Vector2(vec2.x, vec2.y);
}

inline Vector2 Cast(const sf::Vector2f& vec2f)
{
	return Vector2(Fixed(vec2f.x), Fixed(vec2f.y));
}
} // namespace LittleEngine
