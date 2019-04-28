#include "Gamecontroller.h"

GameController::GameController()
{
	OldMouseInputFlag = 0;
	NowMouseInputFlag = 0;
	GetMousePoint(&MousePos.x, &MousePos.y);

	PadArray = {
		&HitPad0_NowArray , &HitPad0_OldArray,
		&HitPad1_NowArray , &HitPad1_OldArray,
		&HitPad2_NowArray , &HitPad2_OldArray,
		&HitPad3_NowArray , &HitPad3_OldArray,
	};
}


GameController::~GameController()
{
}

const HitKeyArray& GameController::GetNowInput() const
{
	return HitKeyNowArray;
}

const HitKeyArray& GameController::GetOldInput() const
{
	return HitKeyOldArray;
}

const int & GameController::GetNowMouseInputFlag() const
{
	return NowMouseInputFlag;
}

const int & GameController::GetOldMouseInputFlag() const
{
	return OldMouseInputFlag;
}

const VECTOR2 & GameController::GetMousePos() const
{
	return MousePos;
}

void GameController::Update()
{
	// Keyの入力を更新する
	RefreshInput();
}

const bool GameController::GetNowInputKey(int InputKeyCode) const
{
	return HitKeyNowArray[InputKeyCode] == 1;
}

const bool GameController::GetOldInputKey(int InputKeyCode) const
{
	return HitKeyOldArray[InputKeyCode] == 1;
}

bool GameController::GetInputDown(int InputKeyCode) const
{
	// 前フレームが0で、現フレームが1の場合ボタンが押し込まれていると判断できる
	// どのキーを判別するかは引数のKeyCodeを用いる
	if (HitKeyNowArray[InputKeyCode] & ~HitKeyOldArray[InputKeyCode])
	{
		return true;
	}

	return false;
}

bool GameController::GetInputHold(int InputKeyCode) const
{
	// 前フレームが1で、現フレームが1の場合ボタンが押し込まれ続けていると判断できる
	// どのキーを判別するかは引数のKeyCodeを用いる
	if (HitKeyNowArray[InputKeyCode] & HitKeyOldArray[InputKeyCode])
	{
		return true;
	}
	return false;
}

bool GameController::GetInputUp(int InputKeyCode) const
{
	// 前フレームが1で、現フレームが0の場合ボタンが離された瞬間と判断できる
	// どのキーを判別するかは引数のKeyCodeを用いる
	if ( ~HitKeyNowArray[InputKeyCode] & HitKeyOldArray[InputKeyCode])
	{
		return true;
	}
	return false;
}

bool GameController::GetMouseInputDown(int InputKeyCode) const
{
	// 前フレームが0で、現フレームが1の場合ボタンが押し込まれていると判断できる
	// どのキーを判別するかは引数のKeyCodeを用いる
	if ( (NowMouseInputFlag & InputKeyCode) & ~(OldMouseInputFlag & InputKeyCode) )
	{
		return true;
	}
	return false;
}

bool GameController::GetMouseInputHold(int InputKeyCode) const
{
	// 前フレームが1で、現フレームが1の場合ボタンが押し込まれ続けていると判断できる
	// どのキーを判別するかは引数のKeyCodeを用いる
	if ( (NowMouseInputFlag & InputKeyCode) & (OldMouseInputFlag & InputKeyCode) )
	{
		return true;
	}
	return false;
}

bool GameController::GetMouseInputUp(int InputKeyCode) const
{
	// 前フレームが1で、現フレームが0の場合ボタンが離された瞬間と判断できる
	// どのキーを判別するかは引数のKeyCodeを用いる
	if ( ~(NowMouseInputFlag & InputKeyCode) & (OldMouseInputFlag & InputKeyCode) )
	{
		return true;
	}
	return false;
}

bool GameController::GetInputDown(int InputPadCode, int PadNum) const
{
	// 前フレームが0で、現フレームが1の場合ボタンが押し込まれていると判断できる
	// どのキーを判別するかは引数のPadCodeを用いる
	if (~(*PadArray[PadNum][1] & InputPadCode) & (*PadArray[PadNum][0] & InputPadCode) )
	{
		return true;
	}
	return false;
}

bool GameController::GetInputHold(int InputPadCode, int PadNum) const
{
	// 前フレームが1で、現フレームが1の場合ボタンが押し込まれ続けていると判断できる
	// どのキーを判別するかは引数のPadCodeを用いる
	if ( (*PadArray[PadNum][1] & InputPadCode) & (*PadArray[PadNum][0] & InputPadCode) )
	{
		return true;
	}
	return false;
}

bool GameController::GetInputUp(int InputPadCode, int PadNum) const
{
	// 前フレームが1で、現フレームが0の場合ボタンが離された瞬間と判断できる
	// どのキーを判別するかは引数のPadCodeを用いる
	if ( (*PadArray[PadNum][1] & InputPadCode) & ~(*PadArray[PadNum][0] & InputPadCode))
	{
		return true;
	}
	return false;
}

void GameController::RefreshInput(void)
{
	// KeyArrayの更新
	HitKeyOldArray = HitKeyNowArray;
	GetHitKeyStateAll(&HitKeyNowArray[0]);
	GetMousePoint(&MousePos.x, &MousePos.y);
	// MouseInputFlagの更新
	OldMouseInputFlag = NowMouseInputFlag;
	NowMouseInputFlag = GetMouseInput();
	// PadArrayの更新
	HitPad0_OldArray = HitPad0_NowArray;
	HitPad1_OldArray = HitPad1_NowArray;
	HitPad2_OldArray = HitPad2_NowArray;
	HitPad3_OldArray = HitPad3_NowArray;
	HitPad0_NowArray = GetJoypadInputState(DX_INPUT_PAD1);
	//HitPad1_NowArray = GetJoypadInputState(DX_INPUT_KEY);	// 開発環境ではPAD2をｷｰ入力対応してます。コントローラーが1個しかないからね
	HitPad1_NowArray = GetJoypadInputState(DX_INPUT_PAD2);
	HitPad2_NowArray = GetJoypadInputState(DX_INPUT_PAD3);
	HitPad3_NowArray = GetJoypadInputState(DX_INPUT_PAD4);
}