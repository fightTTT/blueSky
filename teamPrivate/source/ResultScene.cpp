#include <algorithm>
#include "ResultScene.h"
#include "ImageMng.h"
#include "GameController.h"
#include "SceneMng.h"
#include "SoundMng.h"
#include "SelectScene.h"

#define DRAW_TIME	(300)

#define BAR_MAX_LENGTH	360.0f

#define ICON_SIZE_X		152		// ��ױ��݉摜�̉�����
#define ICON_SIZE_Y		230		// ��ױ��݉摜�̏c����
#define CHAR_OFFSET		100		// ��ױ��݂�\������]��

#define GRAPH_SIZE_X	100		// ���̂̉�����
#define GRAPH_SIZE_Y	40		// ���̂̏c����
#define GRAPH_UNDER_POS	550		// ���̂̉��̍��W

#define GRAPH_ADD_SPEED	1.0f	// ���̂�L�΂�����

#define GRAY_OUT_TIME	360		// ��ڰ��Ľײ�ނ���ʂɂƂǂ܂鎞��(�ڰ�)

#define GRAY_OUT_SPEED	20		// ��ڰ��Ľײ�ނ�1�ڰт̗�

#define CHICKEN_POINT	20		// �ɂ�Ƃ�1����ɂ����Z����|�C���g��

#define AJUST_TIME		180		// 1�ʔ��\����ݑJ�ډ\�ɂȂ�܂ł̎���

#define FADE_SPEED		180		// ̪��޲݁E̪��ޱ�Ă���������܂ł̎���(�ڰ�)

ResultScene::ResultScene(RESULT_INFO resultInfo, int joinPlayerNum)
{
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	awardEndFlag = true;
	resEndFlag = false;
	awardCnt = 0;
	ajustCnt = 0;

	resultState = STATE_FADE_IN;	// ̪��޲ݏ��
	alpha = 255.0;					// �����x��ݒ�(DrawBox�̔��ɑ΂��Ă̓����x)
	
	// �����x��FADE_SPEED(�ڰ�)�����čŏ�(0)�ɂ���
	alpha.set(0, FADE_SPEED);

	grayOutCnt = 0;
	grayOut = GRAY_OUT_NON;

	graphAddPoint = {};

	awardState = static_cast<AWARD>(-1);

	this->resultInfo = resultInfo;


	SortByRank();		// resultInfo�̓��_���~���ɕ��ёւ��ArankInfo�Ɋi�[����


	// ���ʂ̊�b�_
	rankPointTbl = { 100, 50, 25, 0 };

	// �߲�ď����i�[
	for (int k = 0; k < PLAYER_MAX; k++)
	{
		pointInfo.totalGiveDamage[k]	= rankPointTbl[rankInfo.totalGiveDamage[k]];
		pointInfo.deathEnemyCnt[k]		= rankPointTbl[rankInfo.deathEnemyCnt[k]];
		pointInfo.deathOrder[k]			= rankPointTbl[rankInfo.deathOrder[k]];

		// �j���g������
		pointInfo.chickenCnt[k] = resultInfo.chickenCnt[k] * CHICKEN_POINT;
	}

	scoreMax = 0;		// ���v�ō����_
	for (int k = 0; k < PLAYER_MAX; k++)
	{
		int score = 0;
		score += pointInfo.totalGiveDamage[k];
		score += pointInfo.deathEnemyCnt[k];
		score += pointInfo.deathOrder[k];
		score += pointInfo.chickenCnt[k];

		if (score > scoreMax)
		{
			scoreMax = score;
		}
	}
	pointUnit = BAR_MAX_LENGTH / (float)scoreMax;

	blackFlag = true;

	PosXTbl = {
		CHAR_OFFSET + ICON_SIZE_X / 2,
		SCREEN_SIZE_X / 2 - CHAR_OFFSET - ICON_SIZE_X / 2,
		SCREEN_SIZE_X / 2 + CHAR_OFFSET + ICON_SIZE_X / 2,
		SCREEN_SIZE_X - CHAR_OFFSET - ICON_SIZE_X / 2
	};

	GraphColorTbl = { 0xff0000, 0x00bfff ,0x00ff00 ,0xffff00 };

	graphPoint = {};

	//SE�BGM�̓o�^
	lpSoundMng.SoundInit("sound/Result.mp3", "���U���gBGM");
	lpSoundMng.SoundInit("sound/SE��/���ʂ𔭕\���܂�.mp3", "���ʔ��\");
	lpSoundMng.SoundInit("sound/SE��/�h�������[��.mp3", "�h�������[��");
	lpSoundMng.SoundInit("sound/SE��/������������.mp3", "������");
	lpSoundMng.SoundInit("sound/SE��/�܂��V��ł�.mp3", "�܂��V��ł�");

	// BGM�̍Đ�
	lpSoundMng.SetSound("���U���gBGM", true);
	lpSoundMng.SetSound("���ʔ��\");

	// ̫�Ă̏�����
	lpFontMng.FontInit("font/TanukiMagic.ttf", "���ʂ������}�W�b�N", "���ʂ�35��", 35, -1, true, false);
	lpFontMng.FontInit("font/TanukiMagic.ttf", "���ʂ������}�W�b�N", "���ʂ�23��", 35, -1, true, false);

	winSeFlag = false;
	seCnt = 0;
	seFlag = true;
	seFlag2 = true;
	seFlag3 = true;
}


ResultScene::~ResultScene()
{
}

unique_Base ResultScene::Update(unique_Base own, const GameController & ctrl)
{
	switch (resultState)
	{
	case STATE_FADE_IN:
		StateFadeInFunc();
		break;

	case STATE_AWARD:
		StateAwardFunc(ctrl);
		break;

	case STATE_FADE_OUT:
		StateFadeOutFunc();
		break;

	case STATE_NEXT:
		lpSoundMng.StopSound("���U���gBGM");
		return std::make_unique<SelectScene>(true);

	case STATE_MAX:
		break;

	default:
		break;
	}

	resultCnt++;

	return std::move(own);	// ��݌p��
}

void ResultScene::StateFadeInFunc(void)
{
	// ̪��޲݂��I�������STATE_AWARD�ɂ���
	if (alpha <= 0)
	{
		resultState = STATE_AWARD;
		return;
	}

	alpha.update();
	// �`�揈��
	ResultDraw();
}

void ResultScene::StateAwardFunc(const GameController &ctrl)
{
	for (int k = 0; k < PLAYER_MAX; k++)
	{
		if (ctrl.GetInputDown(GPAD_INPUT_A, k))
		{
			// A�Ŏ��̕����
			if (awardEndFlag && awardState < AWARD_MAX - 1)
			{
				grayOut = GRAY_OUT_DOWN;
				awardState	 = (awardState < static_cast<AWARD>(AWARD_MAX - 1)) ? static_cast<AWARD>(awardState + 1) : awardState;
				awardEndFlag = false;
				awardCnt = 0;		// �����������
				seCnt	= 0;
				seFlag	= true;
				seFlag2 = true;
				seFlag3 = true;
				break;
			}

			// A��̪��ޱ�ĊJ�n
			if (resEndFlag && (ajustCnt > AJUST_TIME))
			{
				resultState = STATE_FADE_OUT;

				// �����x��FADE_SPEED(�ڰ�)�����čő�(255)�ɂ���
				alpha.set(255, FADE_SPEED);

				lpSoundMng.SetSound("�܂��V��ł�");
			}
		}
	}

	if (resEndFlag)
	{
		ajustCnt++;
	}

	bool checkFlag[4] = {};		// �e�����߲�ĉ��Z���I����true

	for (int k = 0; k < PLAYER_MAX; k++)
	{
		checkFlag[k] = AwardFunc(k, awardState);
	}
	awardCnt++;

	if (awardState >= AWARD_GIVE_DAMAGE)
	{
		awardEndFlag = true;	// ture�ŏ�����
		for (int k = 0; k < PLAYER_MAX; k++)
		{
			if (!checkFlag[k])
			{
				awardEndFlag = false;	// 1�l�ł��߲�ĉ��Z���I����ĂȂ����false
			}
		}
	}

	auto oldGrayOut = grayOut;

	GrayOutUpdate();

	for (int k = 0; k < PLAYER_MAX; k++)
	{
		if ((graphAddPoint[k] == scoreMax) && awardState >= AWARD_CHICKEN)
		{
			if (oldGrayOut == GRAY_OUT_UP && grayOut == GRAY_OUT_NON)
			{
				resEndFlag = true;		// �S����̖_���̉��Z�܂ŏI��
			}
		}
	}

	// �`�揈��
	ResultDraw();
}

void ResultScene::StateFadeOutFunc(void)
{
	if (alpha >= 255)
	{
		resultState = STATE_NEXT;
		return;
	}
	alpha.update();
	// �`�揈��
	ResultDraw();
}

bool ResultScene::AwardFunc(int pNum, AWARD awardState)
{
	if (awardCnt < (720 / GRAY_OUT_SPEED) + GRAY_OUT_TIME)
	{
		// ���̊Ԃ͏ڍו\����
		return false;
	}

	float arawdPoint;	// ������߲��
	switch (awardState)
	{
	case AWARD_GIVE_DAMAGE:
		arawdPoint = (float)(pointInfo.totalGiveDamage[pNum]);
		break;

	case AWARD_DEATH_ENEMY_CNT:
		arawdPoint = (float)(pointInfo.deathEnemyCnt[pNum]);
		break;

	case AWARD_DEATH_ODER:
		arawdPoint = (float)(pointInfo.deathOrder[pNum]);
		break;

	case AWARD_CHICKEN:
		arawdPoint = (float)(pointInfo.chickenCnt[pNum]);
		break;

	case AWARD_MAX:
		break;

	default:
		break;
	}

	if (graphPoint[pNum][awardState] < arawdPoint)
	{
		graphPoint[pNum][awardState] += GRAPH_ADD_SPEED;
		graphAddPoint[pNum] += (int)GRAPH_ADD_SPEED;

		if (seCnt % 20 == 0)
		{
			lpSoundMng.SetSound("�{��Q�[�W����");
		}
		seCnt++;

		return false;
	}

	return true;
}

void ResultScene::NoJoinPadNumPointInit(RESULT_INFO& c_resultInfo)
{
	// �s�Q������ڲԂ�-1�ɂ���
	for (int k = 0; k < PLAYER_MAX; k++)
	{
		if (resultInfo.charName[k] == "")
		{
			c_resultInfo.totalGiveDamage[k] = -1;
			c_resultInfo.deathEnemyCnt[k]	= -1;
			c_resultInfo.deathOrder[k]		= -1;
			c_resultInfo.chickenCnt[k]		= -1;
		}
	}
}

void ResultScene::SortByRank(void)
{
	RESULT_INFO c_resultInfo = resultInfo;		// ػ����ް���̺�߰��p��

	NoJoinPadNumPointInit(c_resultInfo);

	// ���ʍ~���Ń\�[�g�i1��2��3��4�ʁj
	sort(c_resultInfo.totalGiveDamage.begin(),	c_resultInfo.totalGiveDamage.end(), greater<int>());
	sort(c_resultInfo.deathEnemyCnt.begin(),	c_resultInfo.deathEnemyCnt.end(),	greater<int>());
	sort(c_resultInfo.deathOrder.begin(),		c_resultInfo.deathOrder.end(),		greater<int>());
	sort(c_resultInfo.chickenCnt.begin(),		c_resultInfo.chickenCnt.end(),		greater<int>());

	// ���ގ�
	using INFO = std::array<int, PLAYER_MAX>;
	auto ranking = [](int j, int k, INFO ori_info, INFO c_info, INFO& rank_info)
	{
		if (c_info[j] == ori_info[k])
		{
			rank_info[k] = j;
		}
	};

	// rankInfo�ɏ��ʂ�����
	for (int j = PLAYER_MAX - 1; j >= 0; j--)
	{
		for (int k = 0; k < PLAYER_MAX; k++)
		{
			ranking(j, k, resultInfo.totalGiveDamage,	c_resultInfo.totalGiveDamage,	rankInfo.totalGiveDamage);

			ranking(j, k, resultInfo.deathEnemyCnt,		c_resultInfo.deathEnemyCnt,		rankInfo.deathEnemyCnt);

			ranking(j, k, resultInfo.deathOrder,		c_resultInfo.deathOrder,		rankInfo.deathOrder);

			ranking(j, k, resultInfo.chickenCnt,		c_resultInfo.chickenCnt,		rankInfo.chickenCnt);
		}
	}
}

int ResultScene::Init(void)
{
	return 0;
}

void ResultScene::ResultDraw(void)
{
	ClsDrawScreen();
	DrawGraph(0, 0, lpImageMng.GetID("image/���U���g�p/result.png")[0], true);


	// ��ʉ��̉���
	DrawLine(
		0, GRAPH_UNDER_POS, SCREEN_SIZE_X / 2 - 50,
		GRAPH_UNDER_POS, 0x00000);
	DrawLine(
		SCREEN_SIZE_X / 2 + 50, GRAPH_UNDER_POS,
		1280, GRAPH_UNDER_POS, 0x00000);


	for (int k = 0; k < PLAYER_MAX; k++)
	{
		if (resultInfo.charName[k] == "")
		{
			continue;
		}

		float increasePosY = 0.0f;
		for (int j = awardState; j >= AWARD_GIVE_DAMAGE; j--)
		{
			increasePosY += graphPoint[k][j];

			// DrawBox�J�nY���W
			graphLength[k][j] = static_cast<int>(increasePosY * pointUnit);
			int posY = GRAPH_UNDER_POS - graphLength[k][j];

			// �_�O���t�`��
			// ����
			DrawBox(
				PosXTbl[k] - GRAPH_SIZE_X / 2, posY,
				PosXTbl[k] + GRAPH_SIZE_X / 2, posY + static_cast<int>(graphPoint[k][j] * pointUnit),
				GraphColorTbl[j], true);
		}
	}

	for (int k = 0; k < PLAYER_MAX; k++)
	{
		if (resultInfo.charName[k] == "")
		{
			continue;
		}

		std::string charIconPass = "image/" + resultInfo.charName[k] + "/�A�C�R��/icon.png";

		SetDrawMode(DX_DRAWMODE_BILINEAR);

		// ��ׂ̕`��
		DrawRotaGraph(
			PosXTbl[k] - ICON_SIZE_X / 2 + 75, 610,
			0.8, 0.0,
			lpImageMng.GetID(charIconPass)[0], true);

		SetDrawMode(DX_DRAWMODE_NEAREST);

		// ���ݖ_���̂ɕ\������Ă��鍇�v���_
		int offset = 80;
		if (graphAddPoint[k] < 100)
		{
			// ���_��100�ȉ��̏ꍇ�͕`��̾�Ēl��ύX
			offset = 60;
		}
		DrawFormatStringToHandle(
			PosXTbl[k] - offset, 430,
			0xffffff, lpFontMng.SetFont("���ʂ�60��"), "%dpt", graphAddPoint[k], true);
	}

	// ��ڰ��Ľײ�ނ̕\��
	DrawGrayOut();

	for (int k = 0; k < PLAYER_MAX; k++)
	{
		if (resultInfo.charName[k] == "")
		{
			continue;
		}

		std::string charIconPass = "image/" + resultInfo.charName[k] + "/�A�C�R��/icon.png";

		SetDrawMode(DX_DRAWMODE_BILINEAR);

		// ��ׂ̕`��
		DrawRotaGraph(
			PosXTbl[k] - ICON_SIZE_X / 2 + 75, 610,
			0.8, 0.0,
			lpImageMng.GetID(charIconPass)[0], true);

		SetDrawMode(DX_DRAWMODE_NEAREST);

		// 1P,2P�̕\��
		DrawGraph(PosXTbl[k] - 120, GRAPH_UNDER_POS - 35,lpImageMng.GetID("image/UI/1234.png")[k], true);
	}

	auto award = [&](int offsetX, const TCHAR* sentence, int color)
	{
		if (awardCnt >= (720 / GRAY_OUT_SPEED) + 60)
		{
			if (seFlag)
			{
				if (sentence != "�ɂ�Ƃ�̐���")
				{
					lpSoundMng.SetSound("�K�[�h");
					seFlag = false;
				}
				else
				{
					lpSoundMng.SetSound("������");
					seFlag = false;
				}
			}

			DrawStringToHandle(350 + offsetX, 50, sentence, GraphColorTbl[color], lpFontMng.SetFont("���ʂ�100��"));
		}
	};

	// ���ʂ̕`��
	auto DrawRank = [&](int k, std::array<int, PLAYER_MAX> info)
	{
		if (awardCnt >= (720 / GRAY_OUT_SPEED) + 180)
		{
			if (seFlag3)
			{
				if (awardState != AWARD_CHICKEN)
				{
					lpSoundMng.SetSound("�K�[�h");
					seFlag3 = false;
				}
				else
				{
					lpSoundMng.SetSound("���");
					seFlag3 = false;
				}
			}
			DrawFormatStringToHandle(
				PosXTbl[k] - 70, 320,
				GetRankColor(info[k] + 1), lpFontMng.SetFont("���ʂ�100��"), "%d��", info[k] + 1, true);
		}
	};

	// �l���|�C���g�̕`��iawardState == AWARD_CHICKEN�̏ꍇ�Ɍ����O������ture�ɂ��邱�Ɓj
	auto DrawPoint = [&](int k, std::array<int, PLAYER_MAX> ptInfo, std::array<int, PLAYER_MAX> RankInfo = {}, bool awardChicenFlg = false)
	{
		if (awardCnt >= (720 / GRAY_OUT_SPEED) + 180)
		{
			// awardState == AWARD_CHICKEN�̏ꍇ�̓|�C���g����łȂ����ʂ���F���擾����
			auto GetColor = [=]() { return (awardChicenFlg ? GetRankColor(RankInfo[k] + 1) : GetPointColor(ptInfo[k])); };

			DrawFormatStringToHandle(
				PosXTbl[k] - 50, 450,
				GetColor(), lpFontMng.SetFont("���ʂ�40"), "+%dpt", ptInfo[k], true);
		}
	};


	if (grayOutCnt >= 720 && awardCnt <= 1000)
	{
		if (awardState == AWARD_GIVE_DAMAGE)
		{
			// �^�����_���[�W�̕`��
			award(-60, "�^�����_���[�W", static_cast<int>(AWARD_GIVE_DAMAGE));
		}
		if (awardState == AWARD_DEATH_ENEMY_CNT)
		{
			// �|�����l��
			award(0, "�|�����l��", static_cast<int>(AWARD_DEATH_ENEMY_CNT));
		}
		if (awardState == AWARD_DEATH_ODER)
		{
			// ���񂾏���
			award(0, "�|���ꂽ����", static_cast<int>(AWARD_DEATH_ODER));
		}
		if (awardState == AWARD_CHICKEN)
		{
			// ���낵���ɂ�Ƃ�
			award(-80, "�ɂ�Ƃ�̐���", static_cast<int>(AWARD_CHICKEN));
		}

		for (int k = 0; k < PLAYER_MAX; k++)
		{
			if (resultInfo.charName[k] == "")
			{
				continue;
			}

			if (awardState == AWARD_GIVE_DAMAGE)
			{
				DrawRank(k, rankInfo.totalGiveDamage);

				DrawPoint(k, pointInfo.totalGiveDamage);

				if (awardCnt >= (720 / GRAY_OUT_SPEED) + 120)
				{
					if (seFlag2)
					{
						lpSoundMng.SetSound("�K�[�h");
						seFlag2 = false;
					}

					DrawFormatStringToHandle(
						PosXTbl[k] - 70, 200,
						0xffffff, lpFontMng.SetFont("���ʂ�100��"),
						"%d", resultInfo.totalGiveDamage[k], true);
				}
			}

			if (awardState == AWARD_DEATH_ENEMY_CNT)
			{
				DrawRank(k, rankInfo.deathEnemyCnt);

				DrawPoint(k, pointInfo.deathEnemyCnt);

				if (awardCnt >= (720 / GRAY_OUT_SPEED) + 120)
				{
					if (seFlag2)
					{
						lpSoundMng.SetSound("�K�[�h");
						seFlag2 = false;
					}

					DrawFormatStringToHandle(
						PosXTbl[k] - 60, 200,
						0xffffff, lpFontMng.SetFont("���ʂ�100��"),
						"%d�l", resultInfo.deathEnemyCnt[k], true);
				}
			}

			if (awardState == AWARD_DEATH_ODER)
			{
				DrawRank(k, rankInfo.deathOrder);

				DrawPoint(k, pointInfo.deathOrder);

				if (awardCnt >= (720 / GRAY_OUT_SPEED) + 120)
				{
					if (seFlag2)
					{
						lpSoundMng.SetSound("�K�[�h");
						seFlag2 = false;
					}
					// �|����Ă��Ȃ��Ȃ�[ - ]��\��
					if (resultInfo.deathOrder[k] == 4)
					{
						DrawStringToHandle(PosXTbl[k] - 60, 200, "�[", 0xffffff, lpFontMng.SetFont("���ʂ�100��"));
					}
					// �|���ꂽ�Ȃ珇�ʂ�\��
					else
					{
						DrawFormatStringToHandle(
							PosXTbl[k] - 60, 200,
							0xffffff, lpFontMng.SetFont("���ʂ�100��"),
							"%d�Ԗ�", resultInfo.deathOrder[k], true);
					}
				}
			}

			if (awardState == AWARD_CHICKEN)
			{
				DrawRank(k, rankInfo.chickenCnt);

				DrawPoint(k, pointInfo.chickenCnt, rankInfo.chickenCnt,true);

				if (awardCnt >= (720 / GRAY_OUT_SPEED) + 120)
				{
					if (seFlag2)
					{
						lpSoundMng.SetSound("���");
						seFlag2 = false;
					}
					if (resultInfo.chickenCnt[k] == 0)
					{
						DrawStringToHandle(PosXTbl[k] - 80, 200, "�[", 0xffffff, lpFontMng.SetFont("EP100"));
					}
					else if (resultInfo.chickenCnt[k] == 1)
					{
						DrawStringToHandle(PosXTbl[k] - 80, 200, "����", 0xffffff, lpFontMng.SetFont("EP100"));
					}
					else
					{
						DrawFormatStringToHandle(
							PosXTbl[k] - 80, 200,
							0xffffff, lpFontMng.SetFont("EP100"),
							"%d��", resultInfo.chickenCnt[k], true);
					}
				}
			}
		}
	}

	if (resultState == STATE_AWARD)
	{
		if (awardEndFlag && grayOut == GRAY_OUT_NON)
		{
			if (awardState != AWARD_CHICKEN)
			{
				if (resultCnt / 60 % 2)
				{
					DrawStringToHandle(1000, 20, "A�ł�����", 0xffffff, lpFontMng.SetFont("���ʂ�40��"));
				}
			}
		}
	}

	if (resEndFlag)
	{
		for (int k = 0; k < PLAYER_MAX; k++)
		{
			if (resultInfo.charName[k] == "")
			{
				continue;
			}

			if (graphAddPoint[k] != scoreMax)
			{
				continue;
			}

			if (resultCnt / 60 % 2)
			{
				if (!winSeFlag)
				{
					lpSoundMng.SetSound("����");
					lpSoundMng.SetSound("������");
					winSeFlag = true;
				}

				DrawRotaGraph(
					PosXTbl[k], static_cast<int>(GRAPH_UNDER_POS - BAR_MAX_LENGTH - 60),
					1.0, 0.0, lpImageMng.GetID("image/���U���g�p/oukan.png")[0], true, false);

				DrawStringToHandle(700, 20, "A�ŃL�����N�^�[�I���֖߂�", 0xffffff, lpFontMng.SetFont("���ʂ�40��"));
			}
		}
	}


	DrawStringToHandle(10, 5, "�������͂��҂�[", 0xff0000, lpFontMng.SetFont("���ʂ�40��"));

	// ̪��޲݁E̪��ޱ��
	DrawFade();
	
	//---------------------------------------------------------------------------------------
	SetDrawScreen(DX_SCREEN_BACK);						// tmpBuffer�ɕ`�悵�Ă���(SceneMng��SysInit�Őݒ�)�̂�backScreen�ɖ߂�
	ClsDrawScreen();
	DrawGraph(0, 0, lpSceneMng.GetTmpBuffer(), true);	//backScreen�ɍ��܂ł̕`��(tmpBuffer)������ �����ŉ�ʌ��ʂ�ǉ��ł���
	SetDrawScreen(lpSceneMng.GetTmpBuffer());			// tmpBuffer�ɕ`�悷��悤�ɕύX									//---------------------------------------------------------------------------------------

	ScreenFlip();
}

void ResultScene::DrawGrayOut(void)
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 220);
	DrawBox(VECTOR2(0, 0), VECTOR2(1280, grayOutCnt), 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}

void ResultScene::GrayOutUpdate(void)
{
	if (grayOut == GRAY_OUT_UP)
	{
		grayOutCnt -= GRAY_OUT_SPEED;
		if (grayOutCnt <= 0)
		{
			grayOut = GRAY_OUT_NON;
			resultCnt = 0;
		}
	}

	if (grayOut == GRAY_OUT_DOWN)
	{
		grayOutCnt += GRAY_OUT_SPEED;
		if (grayOutCnt >= 720)
		{
			grayOut = GRAY_OUT_NON;
		}
	}

	// ��ڰ��Ľײ�ނ�UP�ɂ������
	if (grayOut == GRAY_OUT_NON && awardCnt >= (720 / GRAY_OUT_SPEED) + GRAY_OUT_TIME)
	{
		if (grayOutCnt >= 0)
		{
			grayOut = GRAY_OUT_UP;
		}
	}
}

int ResultScene::GetRankColor(int rank)
{
	if (rank == 1)
	{
		// 1�ʂȂ��
		return 0xff0000;
	}
	if (rank == 2)
	{
		// 2�ʂȂ物
		return 0xffff00;
	}
	if (rank == 3)
	{
		// 3�ʂȂ��
		return 0x00bfff;
	}

	// 4�ʂ͔�
	return 0xffffff;
}

int ResultScene::GetPointColor(int point)
{
	if (point == rankPointTbl[0])
	{
		// 1�ʂȂ��
		return 0xff0000;
	}
	if (point == rankPointTbl[1])
	{
		// 2�ʂȂ物
		return 0xffff00;
	}
	if (point == rankPointTbl[2])
	{
		// 3�ʂȂ��
		return 0x00bfff;
	}

	// 3,4�ʂ͔�
	return 0xffffff;
}

void ResultScene::DrawFade(void)
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(alpha));
	DrawBox(VECTOR2(0, 0), lpSceneMng.GetScreenSize(), 0xffffff, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
