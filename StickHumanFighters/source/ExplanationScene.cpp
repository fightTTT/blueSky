#include "DxLib.h"
#include "ImageMng.h"
#include "GameScene.h"
#include "ExplanationScene.h"

#define SCENE_CHANGE_TIME  (120)
#define SCENE_CHANGE_FRAME (5)

#define FLASHING_SPEED	   (30)

ExplanationScene::ExplanationScene()
{
	Init();
}

ExplanationScene::~ExplanationScene()
{
}

unique_Base ExplanationScene::UpDate(unique_Base own, const GameCtrl & controller)
{
	if (!sceneChangeFlag)
	{
		if (controller.GetPadDataTrg(PAD_1, BUTTON_START) || controller.GetPadDataTrg(PAD_2, BUTTON_START))
		{
			sceneChangeFlag = true;
		}

		if (controller.GetPadDataTrg(PAD_1, BUTTON_B) || controller.GetPadDataTrg(PAD_2, BUTTON_B))
		{
			page--;

			if (page < 0)
			{
				page = 0;
			}
		}

		if (controller.GetPadDataTrg(PAD_1, BUTTON_A) || controller.GetPadDataTrg(PAD_2, BUTTON_A))
		{
			page++;

			if (page > (int(imageHandle.size()) - 1))
			{
				page = (int(imageHandle.size()) - 1);
			}
		}
	}
	else
	{
		sceneChangeTime--;

		if ((sceneChangeTime < 0) && (((frame - SCENE_CHANGE_FRAME) / FLASHING_SPEED) % 2))
		{
			WaitTimer(750);
			return std::make_unique<GameScene>();
		}

		frame += (SCENE_CHANGE_FRAME - 1);
	}

	ExplanationDraw();

	frame++;

	return std::move(own);
}

int ExplanationScene::Init()
{
	char tmpPass[30];

	imageHandle.resize(3);
	for (unsigned int i = 0; i < imageHandle.size(); i++)
	{
		sprintf_s(tmpPass, "image/説明用/説明_%d.png", i);

		imageHandle[i] = IMAGE_ID(tmpPass)[0];
	}

	frame = FLASHING_SPEED;
	page = 0;
	sceneChangeFlag = false;
	sceneChangeTime = SCENE_CHANGE_TIME;

	return 0;
}

void ExplanationScene::ExplanationDraw()
{
	DrawGraph(0, 0, imageHandle[page], true);

	if ((frame / FLASHING_SPEED) % 2)
	{
		DrawGraph(290, 640, IMAGE_ID("image/説明用/スタートボタンでゲーム開始.png")[0], true);
	}
}
