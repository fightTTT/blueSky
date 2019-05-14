#include "DxLib.h"
#include "SceneMng.h"
#include "ImageMng.h"
#include "GameCtrl.h"
#include "SelectScene.h"
#include "TitleScene.h"

TitleScene::TitleScene()
{
	Init();
}

TitleScene::~TitleScene()
{
}

unique_Base TitleScene::UpDate(unique_Base own, const GameCtrl & controller)
{
	if (!m_selFlag)
	{
		if (controller.GetPadDataTrg(PAD_1, BUTTON_START) || controller.GetPadDataTrg(PAD_2, BUTTON_START))
		{
			m_selFlag = true;
			mode = MODE_1PLAYER;
		}
	}
	else
	{
		if (controller.GetPadDataTrg(PAD_1, BUTTON_START) || controller.GetPadDataTrg(PAD_1, BUTTON_A)
		 || controller.GetPadDataTrg(PAD_2, BUTTON_START) || controller.GetPadDataTrg(PAD_2, BUTTON_A))
		{
			lpSceneMng.SetMode(mode);
			return std::make_unique<SelectScene>();
		}

		if (controller.GetPadDataTrg(PAD_1, THUMB_L_UP) || controller.GetPadDataTrg(PAD_1, BUTTON_UP)
		 || controller.GetPadDataTrg(PAD_2, THUMB_L_UP) || controller.GetPadDataTrg(PAD_2, BUTTON_UP))
		{
			mode = MODE_1PLAYER;
		}
		else if (controller.GetPadDataTrg(PAD_1, THUMB_L_DOWN) || controller.GetPadDataTrg(PAD_1, BUTTON_DOWN)
			  || controller.GetPadDataTrg(PAD_2, THUMB_L_DOWN) || controller.GetPadDataTrg(PAD_2, BUTTON_DOWN))
		{
			mode = MODE_2PLAYER;
		}

		if (controller.GetPadDataTrg(PAD_1, BUTTON_B) || controller.GetPadDataTrg(PAD_2, BUTTON_B))
		{
			m_selFlag = false;
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

	posTbl = { VECTOR2(210,420),
			   VECTOR2(210,520) };
	return 0;
}

void TitleScene::TitleDraw()
{
	DrawBox(0, 0, ssize.x, ssize.y, 0x5c2d87, true);
	DrawGraph(200, 20, IMAGE_ID("image/タイトル用/StickHuman_Red.png")[0], true);
	DrawGraph((ssize.x / 2), 20, IMAGE_ID("image/タイトル用/StickHuman_Blue.png")[0], true);
	DrawGraph(160, 110, IMAGE_ID("image/タイトル用/StickHuman.png")[0], true);
	DrawGraph(200, 180, IMAGE_ID("image/タイトル用/Fighters.png")[0], true);

	if (!m_selFlag)
	{
		DrawGraph(300, 460, IMAGE_ID("image/タイトル用/PressStartButton.png")[0], true);
	}
	else
	{
		DrawGraph(290, 420, IMAGE_ID("image/タイトル用/1PlayerMode.png")[0], true);
		DrawGraph(290, 520, IMAGE_ID("image/タイトル用/2PlayerMode.png")[0], true);

		DrawGraph(posTbl[mode].x, posTbl[mode].y, IMAGE_ID("image/タイトル用/ModeSelectCursor.png")[0], true);
	}
}
