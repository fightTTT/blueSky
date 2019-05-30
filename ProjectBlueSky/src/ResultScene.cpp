#include "DxLib.h"
#include "ResultScene.h"
#include "TitleScene.h"
#include "ImageMng.h"
#include "SceneMng.h"
#include "VECTOR2.h"
#include <string>

ResultScene::ResultScene()
{
}

ResultScene::ResultScene(PAD_ID padData)
{
	victoryPad = padData;
	
	switch (victoryPad)
	{
	case PAD_1:
		if (lpSceneMng.GetMode() == MODE_1PLAYER)
		{
			defeatPad = PAD_AI;
		}
		else
		{
			defeatPad = PAD_2;
		}
		break;
	case PAD_2:
		defeatPad = PAD_1;
		break;
	case PAD_AI:
		defeatPad = PAD_1;
		break;
	default:
		break;
	}

	Init();
}

ResultScene::~ResultScene()
{
}

unique_Base ResultScene::UpDate(unique_Base own, const GameCtrl & controller)
{
	if (controller.GetPadDataTrg(PAD_1, BUTTON_START) || controller.GetPadDataTrg(PAD_1, BUTTON_A)
		|| controller.GetPadDataTrg(PAD_2, BUTTON_START) || controller.GetPadDataTrg(PAD_2, BUTTON_A))
	{
		return std::make_unique<TitleScene>();
	}

	ResultDraw();
	animCnt++;
	return std::move(own);
}

int ResultScene::Init()
{
	victoryCharID = lpSceneMng.GetCharID(victoryPad);
	defeatCharID  = lpSceneMng.GetCharID(defeatPad);
	

	charName = { "棒人間",
				"棒人間_黒",
				"棒人間_赤",
				"棒人間_青",
				"棒人間_緑",
				"棒人間_黄",
				"棒人間_ピンク",
				"棒人間_水色" };

	imagePass[0].resize(5);

	// player(キャラ)のpass
	for (unsigned int i = 0; i < imagePass[0].size(); i++)
	{
		sprintf_s(tmpImagePass, "image/%s/勝ち/win_%d.png", charName[lpSceneMng.GetCharID(victoryPad)].c_str(), i);
		imagePass[0][i] = tmpImagePass;
	}
	imagePass[1].resize(9);
	for (unsigned int i = 0; i < imagePass[1].size(); i++)
	{
		sprintf_s(tmpImagePass, "image/%s/負け/lose_%d.png", charName[lpSceneMng.GetCharID(defeatPad)].c_str(), i);
		imagePass[1][i] = tmpImagePass;
	}

	// player(文字)のpass
	if (victoryPad == PAD_AI || defeatPad == PAD_AI)
	{
		for (unsigned int i = 0; i < playerfontPass.size(); i++)
		{
			sprintf_s(tmpImagePass, "image/リザルトシーン用/playerまとめ_%d.png", i);
			playerfontPass[i] = tmpImagePass;
		}
	}
	else
	{
		for (unsigned int i = 0; i < playerfontPass.size(); i++)
		{
			sprintf_s(tmpImagePass, "image/リザルトシーン用/playerまとめ_%d.png", (i + 2));
			playerfontPass[i] = tmpImagePass;
		}
	}

	// 勝敗の文字のpass
	for(unsigned int i = 0; i < victoryPass.size(); i++)
	{
		sprintf_s(tmpImagePass, "image/リザルトシーン用/victory_%d.png",i);
		victoryPass[i] = tmpImagePass;
	}

	animCnt = 0;

	return 0;
}

void ResultScene::ResultDraw()
{
	// 背景の描画処理
	sprintf_s(tmpImagePass, "image/リザルトシーン用/背景.png");
	DrawRotaGraph(1280 / 2, 720 / 2, 1.0, 0.0, lpImageMng.GetID(tmpImagePass)[0], true, false);

	DrawString(500, 350, "STARTボタンでタイトルへ遷移", 0xffffff);

	animFrame = {4 - abs(((animCnt / 4) % 9) - 4)  ,animCnt / 8 % 9 };

	int charDrawPos_x;
	bool turnFlag;

	for (int num = 0; num < 2; num++)
	{
		charDrawPos_x = 1280 / 4 * (1 + (num * 2));
		turnFlag = num;

		// 文字の描画
		if (victoryPad == PAD_2 || defeatPad == PAD_2)
		{
			DrawRotaGraph(charDrawPos_x, 720 / 4 * 2, 1.0, 0.0, lpImageMng.GetID(playerfontPass[num], VECTOR2(261, 94), VECTOR2(2, 4))[lpSceneMng.GetCharID(PAD_ID(num))], true, false);
		}
		else
		{
			DrawRotaGraph(1280 / 4, 720 / 4 * 2, 1.0, 0.0, lpImageMng.GetID(playerfontPass[0], VECTOR2(261, 94), VECTOR2(2, 4))[lpSceneMng.GetCharID(PAD_ID(0))], true, false);
			DrawRotaGraph(1280 / 4 * 3, 720 / 4 * 2, 1.0, 0.0, lpImageMng.GetID(playerfontPass[1])[0], true, false);
		}

		if(victoryPad != PAD_1)
		{
			charDrawPos_x = 1280 / 4 * (1 + ((1 - num) * 2));
			turnFlag = 1 - num;
		}

		// 勝ち負けの描画
		DrawRotaGraph(charDrawPos_x, 720 / 4, 1.0, 0.0, lpImageMng.GetID(victoryPass[num])[0], true, false);
		// キャラの描画
		DrawRotaGraph(charDrawPos_x, 720 / 4 * 3 + 30, 1.0, 0.0, lpImageMng.GetID(imagePass[num][animFrame[num]])[0], true, turnFlag);
	}
}
