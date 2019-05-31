#include "CharSelCursor.h"
#include "SceneMng.h"
#include "DxLib.h"
#include "GameCtrl.h"
#include "SelectScene.h"
#include "ImageMng.h"
#include "TitleScene.h"
#include "SoundMng.h"

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
	/* ｷｬﾗを未選択のとき */
	if (!decidFlag)
	{
		if (!lpSceneMng.GetSceneBackFlag())
		{
			/* ﾀｲﾄﾙ遷移ｳｨﾝﾄﾞｳが起動してないとき */

			/* BﾎﾞﾀﾝでｳｨﾝﾄﾞｳﾌﾗｸﾞをON */
			if (ctl.GetPadDataTrg(padID, BUTTON_B))
			{
				PlaySoundMem(SOUND_ID("se/ui/cansell.mp3"), DX_PLAYTYPE_BACK);

				lpSceneMng.SetOpenBackWindowPadID(padID);
				lpSceneMng.SetSceneBackFlag(true);
			}

			/*-----ｶｰｿﾙの移動-----*/
			/* 相手のIDを取得 */
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
			
			/* 上移動 */
			if (ctl.GetPadDataTrg(padID, THUMB_L_UP) || ctl.GetPadDataTrg(padID, BUTTON_UP))
			{
				if (charID >= 4)
				{
					PlaySoundMem(SOUND_ID("se/ui/cursor.mp3"), DX_PLAYTYPE_BACK);

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
			/* 右移動 */
			if (ctl.GetPadDataTrg(padID, THUMB_L_RIGHT) || ctl.GetPadDataTrg(padID, BUTTON_RIGHT))
			{
				if ((charID % 4) != 3)
				{
					PlaySoundMem(SOUND_ID("se/ui/cursor.mp3"), DX_PLAYTYPE_BACK);

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
			/* 下移動 */
			if (ctl.GetPadDataTrg(padID, THUMB_L_DOWN) || ctl.GetPadDataTrg(padID, BUTTON_DOWN))
			{
				if (charID < 4)
				{
					PlaySoundMem(SOUND_ID("se/ui/cursor.mp3"), DX_PLAYTYPE_BACK);

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
			/* 左移動 */
			if (ctl.GetPadDataTrg(padID, THUMB_L_LEFT) || ctl.GetPadDataTrg(padID, BUTTON_LEFT))
			{
				if ((charID % 4) != 0)
				{
					PlaySoundMem(SOUND_ID("se/ui/cursor.mp3"), DX_PLAYTYPE_BACK);

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
			/* ﾊﾟｯﾄﾞIDとｷｬﾗIDを紐づけ */
			lpSceneMng.SetCharID(padID, charID);

			/* ｷｬﾗを決定 */
			if (ctl.GetPadDataTrg(padID, BUTTON_A) && !lpSceneMng.GetSceneBackFlag())
			{
				PlaySoundMem(SOUND_ID("se/ui/check.mp3"), DX_PLAYTYPE_BACK);

				decidFlag = true;
				lpSceneMng.SetDecidFlag(padID, decidFlag);
			}

		}
		else
		{
			/* ｳｨﾝﾄﾞｳが起動しているとき */

			if (ctl.GetPadDataTrg(padID, THUMB_L_RIGHT) || ctl.GetPadDataTrg(padID, BUTTON_RIGHT))
			{
				if (backCurID != 1)
				{
					PlaySoundMem(SOUND_ID("se/ui/cursor.mp3"), DX_PLAYTYPE_BACK);

					backCurID += 1;
				}
			}
			if (ctl.GetPadDataTrg(padID, THUMB_L_LEFT) || ctl.GetPadDataTrg(padID, BUTTON_LEFT))
			{
				if (backCurID != 0)
				{
					PlaySoundMem(SOUND_ID("se/ui/cursor.mp3"), DX_PLAYTYPE_BACK);

					backCurID -= 1;
				}
			}

			/* Aﾎﾞﾀﾝでﾀｲﾄﾙｼｰﾝに戻るﾌﾗｸﾞを立てる */
			if (ctl.GetPadDataTrg(lpSceneMng.GetOpenBackWindowPadID(), BUTTON_A))
			{
				if (backCurID == 0)
				{
					PlaySoundMem(SOUND_ID("se/ui/check.mp3"), DX_PLAYTYPE_BACK);

					StopSoundMem(SOUND_ID("bgm/select.mp3"));

					lpSceneMng.SetTitleChangeFlag(true);
				}
				else
				{
					PlaySoundMem(SOUND_ID("se/ui/cansell.mp3"), DX_PLAYTYPE_BACK);

					lpSceneMng.SetSceneBackFlag(false);
				}
			}

			/* Bﾎﾞﾀﾝでｳｨﾝﾄﾞｳを閉じる */
			if (ctl.GetPadDataTrg(padID, BUTTON_B))
			{
				PlaySoundMem(SOUND_ID("se/ui/cansell.mp3"), DX_PLAYTYPE_BACK);

				lpSceneMng.SetSceneBackFlag(false);
			}
		}
	}

	/* ｷｬﾗを選択してるとき */
	else
	{
		// 決定取り消し確認
		if (ctl.GetPadDataTrg(padID, BUTTON_B))
		{
			PlaySoundMem(SOUND_ID("se/ui/cansell.mp3"), DX_PLAYTYPE_BACK);

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
	padIdFlag = true;
	if (padID == PAD_1)
	{
		charID = 0;		// PL1のｷｬﾗID
	}
	else
	{
		charID = 3;		// PL2のｷｬﾗID
	}
	lpSceneMng.SetCharID(padID, charID);

	backCurID = 0;

	// ﾃｰﾌﾞﾙ
	charSelPosTbl = { VECTOR2((sSize.x / 2) - (BOX_SIZE_X * 2),	(sSize.y * 3 / 5)),
			   VECTOR2((sSize.x / 2) - (BOX_SIZE_X),		(sSize.y * 3 / 5)),
			   VECTOR2((sSize.x / 2),						(sSize.y * 3 / 5)),
			   VECTOR2((sSize.x / 2) + BOX_SIZE_X,			(sSize.y * 3 / 5)),
			   VECTOR2((sSize.x / 2) - (BOX_SIZE_X * 2),	(sSize.y * 3 / 5) + BOX_SIZE_Y),
			   VECTOR2((sSize.x / 2) - (BOX_SIZE_X),		(sSize.y * 3 / 5) + BOX_SIZE_Y),
			   VECTOR2((sSize.x / 2),						(sSize.y * 3 / 5) + BOX_SIZE_Y),
			   VECTOR2((sSize.x / 2) + BOX_SIZE_X,			(sSize.y * 3 / 5) + BOX_SIZE_Y) };

	sceneCurPosTbl = { VECTOR2((sSize.x / 2) - 300, (sSize.y / 2) + 50),
					  VECTOR2((sSize.x / 2) + 50, (sSize.y / 2) + 50) };

	colorTbl = {0xff0000, 0x00ffff};

	mCount = 0;
	charCurMask = LoadMask("image/キャラセレ用/selected_mask.png");
	sceneCurMask = LoadMask("image/キャラセレ用/back_mask.png");

	return 0;
}

void CharSelCursor::Draw(void)
{
	if (!lpSceneMng.GetSceneBackFlag())
	{
		DrawGraph(charSelPosTbl[charID].x, charSelPosTbl[charID].y, IMAGE_DIV_ID("image/キャラセレ用/frame.png", VECTOR2(100, 100), VECTOR2(PLAYER_CNT_MAX, 1))[padID], true);		// 選択用ｶｰｿﾙを描画
		CreateMaskScreen();     // ﾏｽｸ開始
		DrawMask(charSelPosTbl[charID].x - 40, charSelPosTbl[charID].y - 40, charCurMask, DX_MASKTRANS_BLACK); // 黒色の場所だけ描画
		SetDrawBlendMode(DX_BLENDMODE_ADD, 255);               // 加算ブレンドに設定
		DrawRotaGraph2((charSelPosTbl[charID].x + 50), (charSelPosTbl[charID].y + 50), 50, 50, 1.0, PI2 / 240 * mCount, IMAGE_DIV_ID("image/キャラセレ用/selected.png", VECTOR2(130, 61), VECTOR2(PLAYER_CNT_MAX, 1))[padID], true);			// ｶｰｿﾙをﾏｽの中心で画像の中心を軸に回転
		DrawRotaGraph2((charSelPosTbl[charID].x + 50), (charSelPosTbl[charID].y + 50), 50, 50, 1.0, PI2 / 240 * (mCount - 120), IMAGE_DIV_ID("image/キャラセレ用/selected.png", VECTOR2(130, 61), VECTOR2(PLAYER_CNT_MAX, 1))[padID], true);	// 対角線上にもう一つ
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);     // ﾌﾞﾚﾝﾄﾞをﾘｾｯﾄ
		DeleteMaskScreen();		// ﾏｽｸ終了
	}
	else
	{
		/* 自分がﾊﾞｯｸｳｨﾝﾄﾞｳを開いたとき */
		if (lpSceneMng.GetOpenBackWindowPadID() == padID)
		{
			CreateMaskScreen();     // ﾏｽｸ開始
			DrawMask(sceneCurPosTbl[backCurID].x - 40, sceneCurPosTbl[backCurID].y - 100, sceneCurMask, DX_MASKTRANS_BLACK); // 黒色の場所だけ描画
			SetDrawBlendMode(DX_BLENDMODE_ADD, 255);               // 加算ブレンドに設定
			DrawRotaGraph2((sceneCurPosTbl[backCurID].x + 120), (sceneCurPosTbl[backCurID].y + 50), 0, 35, 1.0, PI2 / 240 * mCount, IMAGE_DIV_ID("image/キャラセレ用/backCur.png", VECTOR2(150, 70), VECTOR2(PLAYER_CNT_MAX, 1))[padID], true);			// ｶｰｿﾙをﾏｽの中心で画像の中心を軸に回転
			DrawRotaGraph2((sceneCurPosTbl[backCurID].x + 120), (sceneCurPosTbl[backCurID].y + 50), 0, 35, 1.0, PI2 / 240 * (mCount - 120), IMAGE_DIV_ID("image/キャラセレ用/backCur.png", VECTOR2(150, 70), VECTOR2(PLAYER_CNT_MAX, 1))[padID], true);	// 対角線上にもう一つ
			DrawRotaGraph2((sceneCurPosTbl[backCurID].x + 120), (sceneCurPosTbl[backCurID].y + 50), 0, 35, 1.0, PI2 / 240 * (mCount - 60), IMAGE_DIV_ID("image/キャラセレ用/backCur.png", VECTOR2(150, 70), VECTOR2(PLAYER_CNT_MAX, 1))[padID], true);	// 対角線上にもう一つ
			DrawRotaGraph2((sceneCurPosTbl[backCurID].x + 120), (sceneCurPosTbl[backCurID].y + 50), 0, 35, 1.0, PI2 / 240 * (mCount - 180), IMAGE_DIV_ID("image/キャラセレ用/backCur.png", VECTOR2(150, 70), VECTOR2(PLAYER_CNT_MAX, 1))[padID], true);	// 対角線上にもう一つ
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);     // ﾌﾞﾚﾝﾄﾞをﾘｾｯﾄ
			DeleteMaskScreen();		// ﾏｽｸ終了
		}
	}
}

