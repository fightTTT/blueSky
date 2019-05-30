#include "CharSelCursor.h"
#include "SceneMng.h"
#include "DxLib.h"
#include "GameCtrl.h"
#include "SelectScene.h"
#include "ImageMng.h"
#include "TitleScene.h"
#include "SoundMng.h"

#define BOX_SIZE_X (100)		// ��ױ��݂�X������
#define BOX_SIZE_Y (100)		// ��ױ��݂�Y������
#define PI2 (3.141592654f*6)	// ���ق̉�]�p

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
	/* ��ׂ𖢑I���̂Ƃ� */
	if (!decidFlag)
	{
		if (!lpSceneMng.GetSceneBackFlag())
		{
			/* ���ّJ�ڳ���޳���N�����ĂȂ��Ƃ� */

			/* B���݂ų���޳�׸ނ�ON */
			if (ctl.GetPadDataTrg(padID, BUTTON_B))
			{
				PlaySoundMem(SOUND_ID("se/ui/cansell.mp3"), DX_PLAYTYPE_BACK);

				lpSceneMng.SetOpenBackWindowPadID(padID);
				lpSceneMng.SetSceneBackFlag(true);
			}

			/*-----���ق̈ړ�-----*/
			/* �����ID���擾 */
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
					// �Ȃɂ����Ȃ�
				}
			}
			
			/* ��ړ� */
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
			/* �E�ړ� */
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
						// �Ȃɂ����Ȃ�
					}
				}
			}
			/* ���ړ� */
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
			/* ���ړ� */
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
						// �Ȃɂ����Ȃ�
					}
				}
			}
			/* �߯��ID�Ʒ��ID��R�Â� */
			lpSceneMng.SetCharID(padID, charID);

			/* ��ׂ����� */
			if (ctl.GetPadDataTrg(padID, BUTTON_A) && !lpSceneMng.GetSceneBackFlag())
			{
				PlaySoundMem(SOUND_ID("se/ui/check.mp3"), DX_PLAYTYPE_BACK);

				decidFlag = true;
				lpSceneMng.SetDecidFlag(padID, decidFlag);
			}

		}
		else
		{
			/* ����޳���N�����Ă���Ƃ� */

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

			/* A���݂����ټ�݂ɖ߂��׸ނ𗧂Ă� */
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

			/* B���݂ų���޳����� */
			if (ctl.GetPadDataTrg(padID, BUTTON_B))
			{
				PlaySoundMem(SOUND_ID("se/ui/cansell.mp3"), DX_PLAYTYPE_BACK);

				lpSceneMng.SetSceneBackFlag(false);
			}
		}
	}

	/* ��ׂ�I�����Ă�Ƃ� */
	else
	{
		// ����������m�F
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
		charID = 0;		// PL1�̷��ID
	}
	else
	{
		charID = 3;		// PL2�̷��ID
	}
	lpSceneMng.SetCharID(padID, charID);

	backCurID = 0;

	// ð���
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
	charCurMask = LoadMask("image/�L�����Z���p/selected_mask.png");
	sceneCurMask = LoadMask("image/�L�����Z���p/back_mask.png");

	return 0;
}

void CharSelCursor::Draw(void)
{
	if (!lpSceneMng.GetSceneBackFlag())
	{
		DrawGraph(charSelPosTbl[charID].x, charSelPosTbl[charID].y, IMAGE_DIV_ID("image/�L�����Z���p/frame.png", VECTOR2(100, 100), VECTOR2(PLAYER_CNT_MAX, 1))[padID], true);		// �I��p���ق�`��
		CreateMaskScreen();     // Ͻ��J�n
		DrawMask(charSelPosTbl[charID].x - 40, charSelPosTbl[charID].y - 40, charCurMask, DX_MASKTRANS_BLACK); // ���F�̏ꏊ�����`��
		SetDrawBlendMode(DX_BLENDMODE_ADD, 255);               // ���Z�u�����h�ɐݒ�
		DrawRotaGraph2((charSelPosTbl[charID].x + 50), (charSelPosTbl[charID].y + 50), 50, 50, 1.0, PI2 / 240 * mCount, IMAGE_DIV_ID("image/�L�����Z���p/selected.png", VECTOR2(130, 61), VECTOR2(PLAYER_CNT_MAX, 1))[padID], true);			// ���ق�Ͻ�̒��S�ŉ摜�̒��S�����ɉ�]
		DrawRotaGraph2((charSelPosTbl[charID].x + 50), (charSelPosTbl[charID].y + 50), 50, 50, 1.0, PI2 / 240 * (mCount - 120), IMAGE_DIV_ID("image/�L�����Z���p/selected.png", VECTOR2(130, 61), VECTOR2(PLAYER_CNT_MAX, 1))[padID], true);	// �Ίp����ɂ������
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);     // �����ނ�ؾ��
		DeleteMaskScreen();		// Ͻ��I��
	}
	else
	{
		/* �������ޯ�����޳���J�����Ƃ� */
		if (lpSceneMng.GetOpenBackWindowPadID() == padID)
		{
			CreateMaskScreen();     // Ͻ��J�n
			DrawMask(sceneCurPosTbl[backCurID].x - 40, sceneCurPosTbl[backCurID].y - 100, sceneCurMask, DX_MASKTRANS_BLACK); // ���F�̏ꏊ�����`��
			SetDrawBlendMode(DX_BLENDMODE_ADD, 255);               // ���Z�u�����h�ɐݒ�
			DrawRotaGraph2((sceneCurPosTbl[backCurID].x + 120), (sceneCurPosTbl[backCurID].y + 50), 0, 35, 1.0, PI2 / 240 * mCount, IMAGE_DIV_ID("image/�L�����Z���p/backCur.png", VECTOR2(150, 70), VECTOR2(PLAYER_CNT_MAX, 1))[padID], true);			// ���ق�Ͻ�̒��S�ŉ摜�̒��S�����ɉ�]
			DrawRotaGraph2((sceneCurPosTbl[backCurID].x + 120), (sceneCurPosTbl[backCurID].y + 50), 0, 35, 1.0, PI2 / 240 * (mCount - 120), IMAGE_DIV_ID("image/�L�����Z���p/backCur.png", VECTOR2(150, 70), VECTOR2(PLAYER_CNT_MAX, 1))[padID], true);	// �Ίp����ɂ������
			DrawRotaGraph2((sceneCurPosTbl[backCurID].x + 120), (sceneCurPosTbl[backCurID].y + 50), 0, 35, 1.0, PI2 / 240 * (mCount - 60), IMAGE_DIV_ID("image/�L�����Z���p/backCur.png", VECTOR2(150, 70), VECTOR2(PLAYER_CNT_MAX, 1))[padID], true);	// �Ίp����ɂ������
			DrawRotaGraph2((sceneCurPosTbl[backCurID].x + 120), (sceneCurPosTbl[backCurID].y + 50), 0, 35, 1.0, PI2 / 240 * (mCount - 180), IMAGE_DIV_ID("image/�L�����Z���p/backCur.png", VECTOR2(150, 70), VECTOR2(PLAYER_CNT_MAX, 1))[padID], true);	// �Ίp����ɂ������
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);     // �����ނ�ؾ��
			DeleteMaskScreen();		// Ͻ��I��
		}
	}
}

