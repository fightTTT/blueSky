#include "DxLib.h"
#include "ResultScene.h"
#include "TitleScene.h"
#include "ImageMng.h"
#include "SceneMng.h"
#include <string>

ResultScene::ResultScene()
{
}

ResultScene::ResultScene(PAD_ID padData,bool DrawFlag)
{
	victoryPad = padData;
	drawflag = DrawFlag;
	
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
		//return std::make_unique<TitleScene>();
		returnTitle = true;
	}

	if (itvCnt > 120)
	{
		return std::make_unique<TitleScene>();
	}
	ResultDraw();
	animCnt++;
	if (returnTitle)
	{
		itvCnt++;
	}
	return std::move(own);
}

int ResultScene::Init()
{
	victoryCharID = lpSceneMng.GetCharID(victoryPad);
	defeatCharID  = lpSceneMng.GetCharID(defeatPad);
	

	charName = { "�_�l��",
				"�_�l��_��",
				"�_�l��_��",
				"�_�l��_��",
				"�_�l��_��",
				"�_�l��_��",
				"�_�l��_�s���N",
				"�_�l��_���F" };

	
	animOffSet = { VECTOR2(0,15),VECTOR2(0,0) };

	// player(����)��pass
	if (victoryPad == PAD_AI || defeatPad == PAD_AI)
	{
		for (unsigned int i = 0; i < playerfontPass.size(); i++)
		{
			sprintf_s(tmpImagePass, "image/���U���g�V�[���p/player�܂Ƃ�_%d.png", i);
			playerfontPass[i] = tmpImagePass;
		}
	}
	else
	{
		for (unsigned int i = 0; i < playerfontPass.size(); i++)
		{
			sprintf_s(tmpImagePass, "image/���U���g�V�[���p/player�܂Ƃ�_%d.png", (i + 2));
			playerfontPass[i] = tmpImagePass;
		}
	}

	for (unsigned int i = 0; i < shadowPass.size(); i++)
	{
		sprintf_s(tmpImagePass, "image/���U���g�V�[���p/�e.png");
		shadowPass[i] = tmpImagePass;
	}

	if (!drawflag)
	{
		imagePass[0].resize(5);
		// player(�L����)��pass
		for (unsigned int i = 0; i < imagePass[0].size(); i++)
		{
			sprintf_s(tmpImagePass, "image/%s/����/win_%d.png", charName[lpSceneMng.GetCharID(victoryPad)].c_str(), i);
			imagePass[0][i] = tmpImagePass;
		}
		imagePass[1].resize(9);
		for (unsigned int i = 0; i < imagePass[1].size(); i++)
		{
			sprintf_s(tmpImagePass, "image/%s/����/lose_%d.png", charName[lpSceneMng.GetCharID(defeatPad)].c_str(), i);
			imagePass[1][i] = tmpImagePass;
		}

		// ���s�̕�����pass
		for (unsigned int i = 0; i < victoryPass.size(); i++)
		{
			sprintf_s(tmpImagePass, "image/���U���g�V�[���p/victory_%d.png", i);
			victoryPass[i] = tmpImagePass;
		}
	}
	else
	{
		// ���������̎�
		for (int a = 0; a < 2; a++)
		{
			imagePass[a].resize(5);
			// player(�L����)��pass
			for (unsigned int i = 0; i < imagePass[a].size(); i++)
			{
				sprintf_s(tmpImagePass, "image/%s/����/win_%d.png", charName[lpSceneMng.GetCharID(PAD_ID(a))].c_str(), i);
				imagePass[a][i] = tmpImagePass;
			}

			// ���s�̕�����pass
			sprintf_s(tmpImagePass, "image/���U���g�V�[���p/draw.png");
			victoryPass[0] = tmpImagePass;
		}
	}
	animCnt = 0;
	itvCnt = 0;
	returnTitle = false;

	return 0;
}

void ResultScene::ResultDraw()
{
	// �w�i�̕`�揈��
	sprintf_s(tmpImagePass, "image/���U���g�V�[���p/�w�i.png");
	DrawRotaGraph(1280 / 2, 720 / 2, 1.0, 0.0, lpImageMng.GetID(tmpImagePass)[0], true, false);
	sprintf_s(tmpImagePass, "image/���U���g�V�[���p/winnercup.png");
	DrawRotaGraph(1280 / 2, 720 / 3 * 2, 1.0, 0.0, lpImageMng.GetID(tmpImagePass)[0], true, false);
	
	sprintf_s(tmpImagePass, "image/���U���g�V�[���p/start�{�^��.png");

	if (returnTitle)
	{
		if ((animCnt / 4) % 2)
		{
			DrawRotaGraph(1280 / 2, 300, 1.0, 0.0, lpImageMng.GetID(tmpImagePass)[0], true, false);
		}
	}
	else
	{
		if ((animCnt / 30) % 2)
		{
			DrawRotaGraph(1280 / 2, 300, 1.0, 0.0, lpImageMng.GetID(tmpImagePass)[0], true, false);
		}
	}

	animFrame = {4 - abs(((animCnt / 4) % 9) - 4)  ,animCnt / 8 % 9 };

	int charDrawPos_x;
	bool turnFlag;


	for (int num = 0; num < 2; num++)
	{
		charDrawPos_x = 1280 / 4 * (1 + (num * 2));
		turnFlag = num;

		// �����̕`��
		if (victoryPad == PAD_2 || defeatPad == PAD_2)
		{
			DrawRotaGraph(charDrawPos_x, 720 / 4 * 2, 1.0, 0.0, lpImageMng.GetID(playerfontPass[num], VECTOR2(261, 94), VECTOR2(2, 4))[lpSceneMng.GetCharID(PAD_ID(num))], true, false);
		}
		else
		{
			DrawRotaGraph(1280 / 4, 720 / 4 * 2, 1.0, 0.0, lpImageMng.GetID(playerfontPass[0], VECTOR2(261, 94), VECTOR2(2, 4))[lpSceneMng.GetCharID(PAD_ID(0))], true, false);
			DrawRotaGraph(1280 / 4 * 3, 720 / 4 * 2, 1.0, 0.0, lpImageMng.GetID(playerfontPass[1])[0], true, false);
		}

		

		if (!drawflag)
		{
			if (victoryPad != PAD_1)
			{
				charDrawPos_x = 1280 / 4 * (1 + ((1 - num) * 2));
				turnFlag = 1 - num;
			}
			// �e�̕`��
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
			DrawOvalAA(float(charDrawPos_x), float(720 / 4 * 3 + 140), 70.f, 15.f, 12, 0x000000, true);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

			// ���������̕`��
			DrawRotaGraph(charDrawPos_x, 720 / 4, 1.0, 0.0, lpImageMng.GetID(victoryPass[num])[0], true, false);
			// �L�����̕`��
			DrawRotaGraph(charDrawPos_x, 720 / 4 * 3 + 30 + animOffSet[num].y, 1.0, 0.0, lpImageMng.GetID(imagePass[num][animFrame[num]])[0], true, turnFlag);
		}
		else
		{
			// ���������̎�
			// �e�̕`��
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
			DrawOvalAA(float(charDrawPos_x), float(720 / 4 * 3 + 140), 70.f, 15.f, 12, 0x000000, true);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

			// ���������̕`��
			DrawRotaGraph(1280 / 2, 720 / 4, 1.0, 0.0, lpImageMng.GetID(victoryPass[0])[0], true, false);
			// �L�����̕`��
			DrawRotaGraph(charDrawPos_x, 720 / 4 * 3 + 30 + animOffSet[0].y, 1.0, 0.0, lpImageMng.GetID(imagePass[num][animFrame[0]])[0], true, turnFlag);
		}
		
	}
}
