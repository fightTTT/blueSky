#include "ExplanationScene.h"
#include "GameScene.h"

#include "DxLib.h"

ExplanationScene::ExplanationScene()
{
	Init();
}

ExplanationScene::~ExplanationScene()
{
}

unique_Base ExplanationScene::UpDate(unique_Base own, const GameCtrl & controller)
{
	if (imageHandle.size() == imageIdx)
	{
		if (controller.GetPadDataTrg(PAD_1, BUTTON_START) || controller.GetPadDataTrg(PAD_2, BUTTON_START))
		{
			return std::make_unique<GameScene>();
		}
	}

	if (controller.GetPadDataTrg(PAD_1, BUTTON_B) || controller.GetPadDataTrg(PAD_2, BUTTON_B))
	{
		if (imageIdx > 0)
		{
			imageIdx--;
		}
	}
	else if (controller.GetPadDataTrg(PAD_1, BUTTON_A) || controller.GetPadDataTrg(PAD_2, BUTTON_A))
	{
		if (imageIdx < imageHandle.size())
		{
			imageIdx++;
		}
	}

	ExplanationDraw();

	return std::move(own);
}

int ExplanationScene::Init()
{
	imageIdx = 0;
	imageHandle.resize(3);

	return 0;
}

void ExplanationScene::ExplanationDraw()
{
	DrawFormatString(500, 500, 0xffffff, "idx : %d", imageIdx);
}
