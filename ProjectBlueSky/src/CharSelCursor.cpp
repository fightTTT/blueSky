#include "CharSelCursor.h"
#include "SceneMng.h"
#include "DxLib.h"
#include "SelectScene.h"
#include "ImageMng.h"

#define BOX_SIZE_X (100)		// ｷｬﾗｱｲｺﾝのX軸ｻｲｽﾞ
#define BOX_SIZE_Y (100)		// ｷｬﾗｱｲｺﾝのY軸ｻｲｽﾞ
#define PI2 (3.141592654f*6)	// ｶｰｿﾙの回転用

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
	mCount++;

	if (!decidFlag)
	{
		/* ｶｰｿﾙの移動 */
		//PL1
		PAD_ID tmpEnemyId;
		if (lpSceneMng.GetMode() == MODE_2PLAYER)
		{
			if (padID == PAD_1)
			{
				tmpEnemyId = PAD_2;
			}
			else if (padID == PAD_2)
			{
				tmpEnemyId = PAD_1;
			}
			else
			{
				// なにもしない
			}
		}

		if (ctl.GetPadDataTrg(padID, THUMB_L_UP) || ctl.GetPadDataTrg(padID, BUTTON_UP))
		{
			if (charID >= 4)
			{
				charID -= 4;
				if (lpSceneMng.GetMode() == MODE_2PLAYER)
				{
					if (charID == lpSceneMng.GetCharID(tmpEnemyId))
					{
						charID += 4;
					}
				}
			}
		}
		if (ctl.GetPadDataTrg(padID, THUMB_L_RIGHT) || ctl.GetPadDataTrg(padID, BUTTON_RIGHT))
		{
			if ((charID % 4) != 3)
			{
				if (lpSceneMng.GetMode() == MODE_1PLAYER)
				{
					charID += 1;
				}
				else if (lpSceneMng.GetMode() == MODE_2PLAYER)
				{
					if ((charID + 1) == (lpSceneMng.GetCharID(tmpEnemyId)) && (lpSceneMng.GetCharID(tmpEnemyId) % 4) != 3)
					{
						charID += 1;
					}
					if ((charID + 1) != (lpSceneMng.GetCharID(tmpEnemyId)))
					{
						charID += 1;
					}
				}
				else 
				{
					// なにもしない
				}
			}
		}
		if (ctl.GetPadDataTrg(padID, THUMB_L_DOWN) || ctl.GetPadDataTrg(padID, BUTTON_DOWN))
		{
			if (charID < 4)
			{
				charID += 4;
				if (lpSceneMng.GetMode() == MODE_2PLAYER)
				{
					if (charID == lpSceneMng.GetCharID(tmpEnemyId))
					{
						charID -= 4;
					}
				}
			}
		}
		if (ctl.GetPadDataTrg(padID, THUMB_L_LEFT) || ctl.GetPadDataTrg(padID, BUTTON_LEFT))
		{
			if ((charID % 4) != 0)
			{
				if (lpSceneMng.GetMode() == MODE_1PLAYER)
				{
					charID -= 1;
				}
				else if (lpSceneMng.GetMode() == MODE_2PLAYER)
				{
					if ((charID - 1) == (lpSceneMng.GetCharID(tmpEnemyId)) && (lpSceneMng.GetCharID(tmpEnemyId) % 4) != 0)
					{
						charID -= 1;
					}
					if ((charID - 1) != (lpSceneMng.GetCharID(tmpEnemyId)))
					{
						charID -= 1;
					}
				}
				else
				{
					// なにもしない
				}
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

	mCount = 0;
	mMask = LoadMask("image/selected_mask.png");
	return 0;
}

void CharSelCursor::Draw(void)
{
	//DrawGraph(posTbl[charID].x, posTbl[charID].y, IMAGE_DIV_ID("image/キャラセレ用/frame.png", VECTOR2(100, 100), VECTOR2(PLAYER_CNT_MAX, 1))[padID], true);		// 選択用ｶｰｿﾙを描画

	CreateMaskScreen();     // ﾏｽｸ開始
	DrawMask(posTbl[charID].x-40, posTbl[charID].y-40, mMask, DX_MASKTRANS_BLACK); // 黒色の場所だけ描画
	SetDrawBlendMode(DX_BLENDMODE_ADD, 255);               // 加算ブレンドに設定
	DrawRotaGraph2((posTbl[charID].x + 50), (posTbl[charID].y + 50), 50, 50, 1.0, PI2 / 240 * mCount, IMAGE_ID("image/menu_back_over.png")[0], true);			// ｶｰｿﾙをﾏｽの中心で画像の中心を軸に回転
	DrawRotaGraph2((posTbl[charID].x + 50), (posTbl[charID].y + 50), 50, 50, 1.0, PI2 / 240 * (mCount - 120), IMAGE_ID("image/menu_back_over.png")[0], true);	// 対角線上にもう一つ
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);     // ﾌﾞﾚﾝﾄﾞをﾘｾｯﾄ
	DeleteMaskScreen(); // ﾏｽｸ終了
}

