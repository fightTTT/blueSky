#pragma once

#include <array>
#include "Vector.h"

class InputManager
{
public:
	InputManager();
	~InputManager();

	void Update();

	const Vector2& MousePos() const { return mousePos; }

	// �����ꂽ�L�[���擾
	bool KeyStateDown(int keyCode);
	bool MouseStateDown(int keyCode);

	// �����ꂽ�L�[���擾
	bool MouseStateUp(int keyCode);

private:

	// �}�E�X���
	Vector2 mousePos;
	int mouseData;
	int mouseDataOld;

	// �L�[���
	std::array<char, 256> keyData;
	std::array<char, 256> keyDataOld;
};

