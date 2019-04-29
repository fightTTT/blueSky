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
		return dataOld;
	}

	return data;
}

bool GameCtrl::GetPadData(PAD_ID padNum, BUTTON_TYPE button) const
{
	return padData[padNum][button];
}

bool GameCtrl::UpDate(void)
{
	dataOld = data;
	GetHitKeyStateAll(&data[0]);

	padDataOld = padData;

	XINPUT_STATE pad[PAD_MAX];
	GetJoypadXInputState(DX_INPUT_PAD1, &pad[0]);
	GetJoypadXInputState(DX_INPUT_PAD2, &pad[1]);

	for (int i = 0; i < PAD_MAX; ++i)
	{
		padData[i][THUMB_L_UP] = pad[i].ThumbLY > 20000;
		padData[i][THUMB_L_DOWN] = pad[i].ThumbLY < -20000;
		padData[i][THUMB_L_RIGHT] = pad[i].ThumbLX > 20000;
		padData[i][THUMB_L_LEFT] = pad[i].ThumbLX < -20000;
	}


	return true;
}
