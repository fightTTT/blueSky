#include "Dxlib.h"
#include "GameCtrl.h"

GameCtrl::GameCtrl()
{
}

GameCtrl::~GameCtrl()
{
}


const KEY_ARRAY & GameCtrl::GetCtrl(KEY_TYPE type) const
{
	if (type == KEY_TYPE_OLD)
	{
		return keyDataOld;
	}

	return keyData;
}

bool GameCtrl::GetPadData(PAD_ID padNum, BUTTON_TYPE button) const
{
	return padData[padNum][button];
}

bool GameCtrl::GetPadDataTrg(PAD_ID padNum, BUTTON_TYPE button) const
{
	return padData[padNum][button] && !padDataOld[padNum][button];
}

bool GameCtrl::UpDate(void)
{
	keyDataOld = keyData;
	GetHitKeyStateAll(&keyData[0]);

	padDataOld = padData;

	XINPUT_STATE pad[PAD_MAX];
	GetJoypadXInputState(DX_INPUT_PAD1, &pad[0]);
	GetJoypadXInputState(DX_INPUT_PAD2, &pad[1]);

	for (int i = 0; i < PAD_MAX; ++i)
	{
		padData[i][THUMB_L_UP] = pad[i].ThumbLY > 20000 || keyData[KEY_INPUT_W];
		padData[i][THUMB_L_DOWN] = pad[i].ThumbLY < -20000 || keyData[KEY_INPUT_S];
		padData[i][THUMB_L_RIGHT] = pad[i].ThumbLX > 20000 || keyData[KEY_INPUT_D];
		padData[i][THUMB_L_LEFT] = pad[i].ThumbLX < -20000 || keyData[KEY_INPUT_A];
		padData[i][BUTTON_A] = pad[i].Buttons[XINPUT_BUTTON_A] > 0 || keyData[KEY_INPUT_NUMPAD1];
		padData[i][BUTTON_B] = pad[i].Buttons[XINPUT_BUTTON_B] > 0;
		padData[i][BUTTON_X] = pad[i].Buttons[XINPUT_BUTTON_X] > 0;
		padData[i][BUTTON_Y] = pad[i].Buttons[XINPUT_BUTTON_Y] > 0;
		padData[i][BUTTON_DOWN] = pad[i].Buttons[XINPUT_BUTTON_DPAD_DOWN] > 0 || keyData[KEY_INPUT_DOWN];
		padData[i][BUTTON_LEFT] = pad[i].Buttons[XINPUT_BUTTON_DPAD_LEFT] > 0 || keyData[KEY_INPUT_LEFT];
		padData[i][BUTTON_RIGHT] = pad[i].Buttons[XINPUT_BUTTON_DPAD_RIGHT] > 0 || keyData[KEY_INPUT_RIGHT];
		padData[i][BUTTON_UP] = pad[i].Buttons[XINPUT_BUTTON_DPAD_UP] > 0 || keyData[KEY_INPUT_UP];
		padData[i][BUTTON_START] = pad[i].Buttons[XINPUT_BUTTON_START] > 0 || keyData[KEY_INPUT_SPACE];
	}


	return true;
}
