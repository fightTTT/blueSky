#pragma once

#include <array>
#include "VECTOR2.h"

class InputManager
{
public:
	InputManager();
	~InputManager();

	void Update();

	const VECTOR2& MousePos() const { return mousePos; }

	// 押されたキー情報取得
	bool KeyStateDown(int keyCode);
	bool MouseStateDown(int keyCode);

	// 離されたキー情報取得
	bool MouseStateUp(int keyCode);

private:

	// マウス情報
	VECTOR2 mousePos;
	int mouseData;
	int mouseDataOld;

	// キー情報
	std::array<char, 256> keyData;
	std::array<char, 256> keyDataOld;
};

