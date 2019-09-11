#pragma once
#include "Core/StdTypes.h"
#include "Engine/Input/LEInput.h"

namespace LittleEngine
{
struct LiveLine
{
	String liveString;
	size_t cursorIdx;

	LiveLine();

	void Set(String newInput);
	void Append(String newInput);
	bool Backspace();
	bool Delete();
	void Clear();
	bool MoveCursor(s8 units);
	void MoveCursorToExtreme(bool bStart);

	Fixed CursorNPos() const;
};

class KeyboardInput final
{
public:
	LiveLine m_liveLine;
	bool m_bClearOnEscape = true;

public:
	void Update(const LEInput::Frame& frame);
	void Clear();
	String LiveString() const;
};
} // namespace LittleEngine
