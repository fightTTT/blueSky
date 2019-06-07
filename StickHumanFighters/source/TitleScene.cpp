#include "DxLib.h"
#include "SceneMng.h"
#include "ImageMng.h"
#include "GameCtrl.h"
#include "SelectScene.h"
#include "TitleScene.h"

#define DEF_RED_POS_X (200)
#define DEF_RED_POS_Y (20)

#define DEF_BLUE_POS_X (ssize.x / 2)
#define DEF_BLUE_POS_Y (20)

#define SCENE_CHANGE_TIME  (120)
#define SCENE_CHANGE_FRAME (5)

#define FLASHING_SPEED	   (30)

TitleScene::TitleScene()
{
	Init();
}

TitleScene::~TitleScene()
{
}

unique_Base TitleScene::UpDate(unique_Base own, const GameCtrl & controller)
{
	frame++;
	if (sceneChangeFlag)
	{
		frame += 3;
		if ((frame > 780) && ((frame - 4) / FLASHING_SPEED % 2))
		{
			WaitTimer(750);
			return std::make_unique<SelectScene>();
		}
	}
	else
	{
		if (cutInFlag >= 3)
		{
			if (!m_selFlag)
			{
				if (controller.GetPadDataTrg(PAD_1, BUTTON_START) || controller.GetPadDataTrg(PAD_2, BUTTON_START))
				{
					m_selFlag = true;
					mode = MODE_1PLAYER;
					frame = FLASHING_SPEED;
				}
			}
			else
			{
				if (controller.GetPadDataTrg(PAD_1, BUTTON_START) || controller.GetPadDataTrg(PAD_1, BUTTON_A)
					|| controller.GetPadDataTrg(PAD_2, BUTTON_START) || controller.GetPadDataTrg(PAD_2, BUTTON_A))
				{
					lpSceneMng.SetMode(mode);
					sceneChangeFlag = true;
				}

				if (controller.GetPadDataTrg(PAD_1, THUMB_L_UP) || controller.GetPadDataTrg(PAD_1, BUTTON_UP)
					|| controller.GetPadDataTrg(PAD_2, THUMB_L_UP) || controller.GetPadDataTrg(PAD_2, BUTTON_UP))
				{
					mode = MODE_1PLAYER;
					frame = FLASHING_SPEED;
				}
				else if (controller.GetPadDataTrg(PAD_1, THUMB_L_DOWN) || controller.GetPadDataTrg(PAD_1, BUTTON_DOWN)
					|| controller.GetPadDataTrg(PAD_2, THUMB_L_DOWN) || controller.GetPadDataTrg(PAD_2, BUTTON_DOWN))
				{
					mode = MODE_2PLAYER;
					frame = FLASHING_SPEED;
				}

				if (controller.GetPadDataTrg(PAD_1, BUTTON_B) || controller.GetPadDataTrg(PAD_2, BUTTON_B))
				{
					m_selFlag = false;
					frame = FLASHING_SPEED;
				}
			}
		}
		else
		{
			if (controller.GetPadDataTrg(PAD_1, BUTTON_START) || controller.GetPadDataTrg(PAD_2, BUTTON_START))
			{
				cutInFlag = 3;
				redManPos = { DEF_RED_POS_X , DEF_RED_POS_Y };
				blueManPos = { DEF_BLUE_POS_X , DEF_BLUE_POS_Y };
				frame = FLASHING_SPEED;
			}

			switch (cutInFlag)
			{
			case 0:
				if (frame > 60)
				{
					cutInFlag++;
					frame = 0;
				}
				else
				{
					redManPos += {-20, -20};
				}
				break;
			case 1:
				if (frame > 60)
				{
					if (frame == 140 || frame == 160 || frame == 220)
					{
						flashFlag = true;
					}
					if (frame == 141 || frame == 161)
					{
						flashFlag = false;
					}
					if (frame == 360)
					{
						cutInFlag++;
						frame = 0;
					}
				}
				else
				{
					blueManPos += {20, -20};
				}
				break;
			case 2:
				if (frame == 1)
				{
					flashFlag = false;
				}
				if (frame > 60)
				{
					cutInFlag++;
					frame = FLASHING_SPEED;
				}
				break;
			default:
				break;
			}
		}
	}

	TitleDraw();

	return std::move(own);
}

int TitleScene::Init()
{
	ssize = lpSceneMng.GetScreenSize();
	m_selFlag = false;
	mode = MODE_1PLAYER;
	frame = 0;
	sceneChangeFlag = false;
	sceneChangeTime = SCENE_CHANGE_TIME;
	cutInFlag = 0;
	flashFlag = false;
	redManPos = { DEF_RED_POS_X + (60 * 20) , DEF_RED_POS_Y + (60 * 20) };
	blueManPos = { DEF_BLUE_POS_X - (60 * 20) , DEF_BLUE_POS_Y + (60 * 20) };

	posTbl = { VECTOR2(210,420),
			   VECTOR2(210,520) };
	lpSceneMng.SetTitleChangeFlag(false);
	lpSceneMng.SetSceneBackFlag(false);
	return 0;
}

void TitleScene::TitleDraw()
{
	if (cutInFlag >= 2)
	{
		DrawBox(0, 0, ssize.x, ssize.y, 0x5c2d87, true);
	}

	if (cutInFlag >= 0)
	{
		DrawGraph(redManPos.x, redManPos.y, IMAGE_ID("image/タイトル用/StickHuman_Red.png")[0], true);
		if (cutInFlag >= 1)
		{
			DrawGraph(blueManPos.x, blueManPos.y, IMAGE_ID("image/タイトル用/StickHuman_Blue.png")[0], true);
			if (cutInFlag >= 2)
			{
				DrawGraph(160, 110, IMAGE_ID("image/タイトル用/StickHuman.png")[0], true);
				DrawGraph(200, 180, IMAGE_ID("image/タイトル用/Fighters.png")[0], true);
			}
		}
	}

	if (flashFlag)
	{
		if (frame >= 220)
		{
			int blend = (frame - 220) * 3;
			if (blend > 255)
			{
				blend = 255;
			}
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, blend);
		}
		DrawBox(0, 0, ssize.x, ssize.y, 0xeeeeee, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	if (cutInFlag >= 3)
	{
		if (!m_selFlag)
		{
			if (frame / FLASHING_SPEED % 2)
			{
				DrawGraph(300, 460, IMAGE_ID("image/タイトル用/PressStartButton.png")[0], true);
			}
		}
		else
		{
			if (mode == MODE_1PLAYER)
			{
				if (frame / FLASHING_SPEED % 2)
				{
					DrawGraph(290, 420, IMAGE_ID("image/タイトル用/1PlayerMode.png")[0], true);
				}
				DrawGraph(290, 520, IMAGE_ID("image/タイトル用/2PlayerMode.png")[0], true);
			}
			else
			{
				DrawGraph(290, 420, IMAGE_ID("image/タイトル用/1PlayerMode.png")[0], true);
				if (frame / FLASHING_SPEED % 2)
				{
					DrawGraph(290, 520, IMAGE_ID("image/タイトル用/2PlayerMode.png")[0], true);
				}
			}

			DrawGraph(posTbl[mode].x, posTbl[mode].y, IMAGE_ID("image/タイトル用/ModeSelectCursor.png")[0], true);
		}
	}
}
