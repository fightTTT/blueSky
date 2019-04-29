
#include "InputManager.h"
#include <DxLib.h>

InputManager::InputManager()
{
}

InputManager::~InputManager()
{
}

void InputManager::Update()
{
	// マウスデータ更新
	mouseDataOld = mouseData;
	mouseData = GetMouseInput();
	GetMousePoint(&mousePos.x, &mousePos.y);

	// キーデータ更新
	keyDataOld = keyData;
	GetHitKeyStateAll(keyData.data());
}

bool InputManager::KeyStateDown(int keyCode)
{
	if ( keyData[keyCode] & (~(keyDataOld[keyCode])) )
	{
		return true;
	}

	return false;
}

bool InputManager::MouseStateDown(int keyCode)
{
	if ((mouseData & keyCode) & (~(mouseDataOld & keyCode)) )
	{
		return true;
	}

	return false;
}

bool InputManager::MouseStateUp(int keyCode)
{
	if ( (~(mouseData & keyCode)) & (mouseDataOld & keyCode) )
	{
		return true;
	}

	return false;
}
