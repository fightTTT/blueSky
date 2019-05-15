#include "CharSelCursor.h"
#include "SceneMng.h"
#include "DxLib.h"
#include "SelectScene.h"
#include "ImageMng.h"

#define BOX_SIZE_X (100)		// ｷｬﾗｱｲｺﾝのX軸ｻｲｽﾞ
#define BOX_SIZE_Y (100)		// ｷｬﾗｱｲｺﾝのY軸ｻｲｽﾞ


CharSelCursor::CharSelCursor(PAD_ID padId)
{
	padID = padId;
	Init();
}


CharSelCursor::~CharSelCursor()
{
}

void CharSelCursor::SetMove(const GameCtrl & ctl, weekListObj objList)
{
	if (!decidFlag)
	{
		/* ｶｰｿﾙの移動 */
		//PL1
		if (ctl.GetPadDataTrg(padID, THUMB_L_UP) || ctl.GetPadDataTrg(padID, BUTTON_UP))
		{
			if (charID >= 4)
			{
				charID -= 4;
			}
		}
		if (ctl.GetPadDataTrg(padID, THUMB_L_RIGHT) || ctl.GetPadDataTrg(padID, BUTTON_RIGHT))
		{
			if ((charID % 4) != 3)
			{
				charID += 1;
			}
		}
		if (ctl.GetPadDataTrg(padID, THUMB_L_DOWN) || ctl.GetPadDataTrg(padID, BUTTON_DOWN))
		{
			if (charID < 4)
			{
				charID += 4;
			}
		}
		if (ctl.GetPadDataTrg(padID, THUMB_L_LEFT) || ctl.GetPadDataTrg(padID, BUTTON_LEFT))
		{
			if ((charID % 4) != 0)
			{
				charID -= 1;
			}
		}
		lpSceneMng.SetCharID(padID, charID);

		// 決定
		if (ctl.GetPadDataTrg(padID, BUTTON_A))
		{
			decidFlag = true;
			lpSceneMng.SetDecidFlag(padID, decidFlag);
		}
	}
	else
	{
		// 決定取り消し確認
		if (ctl.GetPadDataTrg(padID, BUTTON_B))
		{
			decidFlag = false;
			lpSceneMng.SetDecidFlag(padID, false);
		}
	}


}

bool CharSelCursor::CheckObjType(OBJ_TYPE type)
{
	return false;
}

int CharSelCursor::Init(void)
{
	sSize = lpSceneMng.GetScreenSize();
	decidFlag = false;
	if (padID == PAD_1)
	{
		charID = 0;		// PL1のｷｬﾗID
	}
	else
	{
		charID = 3;		// PL2のｷｬﾗID
	}
	lpSceneMng.SetCharID(padID, charID);

	// ﾃｰﾌﾞﾙ
	posTbl = { VECTOR2((sSize.x / 2) - (BOX_SIZE_X * 2),	(sSize.y * 3 / 5)),
			   VECTOR2((sSize.x / 2) - (BOX_SIZE_X),		(sSize.y * 3 / 5)),
			   VECTOR2((sSize.x / 2),						(sSize.y * 3 / 5)),
			   VECTOR2((sSize.x / 2) + BOX_SIZE_X,			(sSize.y * 3 / 5)),
			   VECTOR2((sSize.x / 2) - (BOX_SIZE_X * 2),	(sSize.y * 3 / 5) + BOX_SIZE_Y),
			   VECTOR2((sSize.x / 2) - (BOX_SIZE_X),		(sSize.y * 3 / 5) + BOX_SIZE_Y),
			   VECTOR2((sSize.x / 2),						(sSize.y * 3 / 5) + BOX_SIZE_Y),
			   VECTOR2((sSize.x / 2) + BOX_SIZE_X,			(sSize.y * 3 / 5) + BOX_SIZE_Y) };

	colorTbl = {0xff0000, 0x00ffff};
	return 0;
}

void CharSelCursor::Draw(void)
{
	DrawGraph(posTbl[charID].x, posTbl[charID].y, IMAGE_DIV_ID("image/キャラセレ用/frame.png", VECTOR2(100, 100), VECTOR2(PLAYER_CNT_MAX, 1))[padID], true);		// ｷｬﾗのｱｲｺﾝを描画
}

