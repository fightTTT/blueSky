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

bool GameCtrl::UpDate(void)
{
	dataOld = data;
	GetHitKeyStateAll(&data[0]);
	return true;
}
