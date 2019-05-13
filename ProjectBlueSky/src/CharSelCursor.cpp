#include "CharSelCursor.h"
#include "SceneMng.h"
#include "DxLib.h"
#include "SelectScene.h"

#define BOX_SIZE_X (100)		// ·¬×±²ºÝ‚ÌXŽ²»²½Þ
#define BOX_SIZE_Y (100)		// ·¬×±²ºÝ‚ÌYŽ²»²½Þ


CharSelCursor::CharSelCursor()
{
	Init();
}


CharSelCursor::~CharSelCursor()
{
}

void CharSelCursor::SetMove(const GameCtrl & ctl, weekListObj objList)
{
	/* ¶°¿Ù‚ÌˆÚ“® */
	if (ctl.GetPadDataTrg(PAD_1, THUMB_L_UP))
	{
		if (charID >= 4)
		{
			charID -= 4;
		}
	}
	if (ctl.GetPadDataTrg(PAD_1, THUMB_L_RIGHT))
	{
		if ( (charID % 4) != 3)
		{
			charID += 1;
		}
	}
	if (ctl.GetPadDataTrg(PAD_1, THUMB_L_DOWN))
	{
		if (charID < 4)
		{
			charID += 4;
		}
	}
	if (ctl.GetPadDataTrg(PAD_1, THUMB_L_LEFT))
	{
		if ((charID % 4) != 0)
		{
			charID -= 1;
		}
	}

	lpSceneMng.SetCharID(charID);
}

bool CharSelCursor::CheckObjType(OBJ_TYPE type)
{
	return false;
}

int CharSelCursor::Init(void)
{
	sSize = lpSceneMng.GetScreenSize();
	charID = 0;
	lpSceneMng.SetCharID(charID);

	// Ã°ÌÞÙ
	posTbl = { VECTOR2((sSize.x / 2) - (BOX_SIZE_X * 2),	(sSize.y * 3 / 5)),
			   VECTOR2((sSize.x / 2) - (BOX_SIZE_X),		(sSize.y * 3 / 5)),
			   VECTOR2((sSize.x / 2),						(sSize.y * 3 / 5)),
			   VECTOR2((sSize.x / 2) + BOX_SIZE_X,			(sSize.y * 3 / 5)),
			   VECTOR2((sSize.x / 2) - (BOX_SIZE_X * 2),	(sSize.y * 3 / 5) + BOX_SIZE_Y),
			   VECTOR2((sSize.x / 2) - (BOX_SIZE_X),		(sSize.y * 3 / 5) + BOX_SIZE_Y),
			   VECTOR2((sSize.x / 2),						(sSize.y * 3 / 5) + BOX_SIZE_Y),
			   VECTOR2((sSize.x / 2) + BOX_SIZE_X,			(sSize.y * 3 / 5) + BOX_SIZE_Y) };

	return 0;
}

void CharSelCursor::Draw(void)
{
	DrawBox(posTbl[charID].x, posTbl[charID].y, posTbl[charID].x + BOX_SIZE_X, posTbl[charID].y+ BOX_SIZE_Y, 0xff0000, false);		// ¶°¿Ù‚Ì•`‰æ
	DrawBox(posTbl[charID].x+1, posTbl[charID].y+1, posTbl[charID].x + BOX_SIZE_X+1, posTbl[charID].y + BOX_SIZE_Y+1, 0xff0000, false);		// ¶°¿Ù‚Ì•â••`‰æ
}

