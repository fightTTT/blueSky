#include "DxLib.h"
#include "SoundMng.h"
#include "FontMng.h"
#include "SceneMng.h"
#include "GameController.h"
#include "SelectScene.h"
#include "TitleScene.h"
//#include "PlayerNumSelectScene.h"

//��`
#define CHANGE_SCENE	(30)	// ��ʑJ�ڂ܂ł̶���

TitleScene::TitleScene()
{
	Init();
}


TitleScene::~TitleScene()
{
}

unique_Base TitleScene::Update(unique_Base own, const GameController & controller)
{
	if (mode == MODE_SCENE_NEXT)
	{
		if (PageDraw())
		{
			//// BGM�̒�~
			lpSoundMng.StopSound("����BGM");
			return std::make_unique<SelectScene>();
		}
		return std::move(own);	// ��݌p��
	}

	// Start��
	int Start_KYE = PAD_INPUT_8;

	for (int k = 0; k < PLAYER_MAX; k++)
	{
		if (controller.GetInputDown(KEY_INPUT_SPACE) || (controller.GetInputDown(Start_KYE, k)))
		{
			if (!ChangeSceneFlag)
			{
				// SE
				lpSoundMng.SetSound("����SE");

				flasing = FLASING_HIGH_CNT;
				Cnt = 0;
				ChangeSceneFlag = true;

				// �ڸļ�ݗp�ϐ�
				startPadNum = k;
			}
		}
	}

	if (ChangeSceneFlag)
	{
		if (Cnt >= CHANGE_SCENE)
		{
			int titleSceneImage = MakeScreen(SCREEN_SIZE_X, SCREEN_SIZE_Y);
			SetDrawScreen(titleSceneImage);
			DrawGraph(0, 0, lpSceneMng.GetTmpBuffer(), true);
			//DrawGraph(0, 0, lpImageMng.GetID("image/�L�����Z���p/�W���p�j�J.png")[0], true);
			SetDrawScreen(DX_SCREEN_BACK);

			turnThePage = TurnThePage(VECTOR2(SCREEN_SIZE_X, SCREEN_SIZE_Y), VECTOR2(0, 0), titleSceneImage, lpImageMng.GetID("image/�L�����Z���p/PageSerect.png")[0], 1.0f);
			mode = MODE_SCENE_NEXT;
		}
	}

	TitleDraw();


	return std::move(own);	// ��݌p��
}

bool TitleScene::TitleDraw(void)
{
	Cnt++;

	// �w�i
	DrawGraph(0, 0, lpImageMng.GetID("image/�^�C�g���p/Title.png")[0], true);
	DrawStringToHandle(870,125,"�ő�S�l�ΐ�", 0x000000, lpFontMng.SetFont("���ʂ�40"));
	DrawStringToHandle(800, 162, "�h�^�o�^�I�A�N�V�����o�g��!!", 0x000000, lpFontMng.SetFont("���ʂ�27"));
	if (Cnt / flasing % 2 == 0)
	{
		DrawStringToHandle(
			700,
			550,
			"�X�^�[�g�{�^���������Ă�", 0xff0000, lpFontMng.SetFont("���ʂ�45��"));
	}

	//---------------------------------------------------------------------------------------
	SetDrawScreen(DX_SCREEN_BACK);						// tmpBuffer�ɕ`�悵�Ă���(SceneMng��SysInit�Őݒ�)�̂�backScreen�ɖ߂�
	ClsDrawScreen();
	DrawGraph(0, 0, lpSceneMng.GetTmpBuffer(), true);	//backScreen�ɍ��܂ł̕`��(tmpBuffer)������ �����ŉ�ʌ��ʂ�ǉ��ł���
	SetDrawScreen(lpSceneMng.GetTmpBuffer());			// tmpBuffer�ɕ`�悷��悤�ɕύX
	//---------------------------------------------------------------------------------------

	ScreenFlip();

	return true;
}

bool TitleScene::PageDraw(void)
{
	ClsDrawScreen();
	if (turnThePage.Draw())
	{
		return true;
	}
	//---------------------------------------------------------------------------------------
	SetDrawScreen(DX_SCREEN_BACK);						// tmpBuffer�ɕ`�悵�Ă���(SceneMng��SysInit�Őݒ�)�̂�backScreen�ɖ߂�
	ClsDrawScreen();
	DrawGraph(0, 0, lpSceneMng.GetTmpBuffer(), true);	//backScreen�ɍ��܂ł̕`��(tmpBuffer)������ �����ŉ�ʌ��ʂ�ǉ��ł���
	SetDrawScreen(lpSceneMng.GetTmpBuffer());			// tmpBuffer�ɕ`�悷��悤�ɕύX														//--------------------------------------------------------------------------------------

	ScreenFlip();

	return false;
}

int TitleScene::Init(void)
{
	if (!objList)
	{
		objList = std::make_shared<uniqueObjList>();
	}
	objList->clear();

	//SE�BGM�̓o�^
	lpSoundMng.SoundInit("sound/Title.mp3", "����BGM");
	lpSoundMng.SoundInit("sound/Select.mp3", "�ڸ�BGM");

	lpSoundMng.SoundInit("sound/Battle.mp3", "�o�g��1BGM");
	lpSoundMng.SoundInit("sound/Battle2.mp3", "�o�g��2BGM");
	lpSoundMng.SoundInit("sound/Battle3.mp3", "�o�g��3BGM", 90);

	lpSoundMng.SoundInit("sound/SE��/DecisionSE.mp3", "����SE");
	
	// BGM�̍Đ�
	lpSoundMng.SetSound("����BGM", true);
	
	// ̫�Ă̓o�^
	lpFontMng.FontInit("font/TanukiMagic.ttf", "���ʂ������}�W�b�N", "���ʂ�27", 27, -1, false, false);
	lpFontMng.FontInit("font/TanukiMagic.ttf", "���ʂ������}�W�b�N", "���ʂ�40",35, -1, false, false);
	lpFontMng.FontInit("font/TanukiMagic.ttf", "���ʂ������}�W�b�N", "���ʂ�45��", 45, -1, true,false);
	lpFontMng.FontInit("font/AVOCADO.TTF", "�A�{�J�h", "�A�{�J�h��27", 27, -1, true, false);

	lpFontMng.FontInit("font/Molot.otf", "Molot", "Molot40���Α�", 35, -1, true, true);

	lpFontMng.FontInit("font/Square.ttf", "SquareFont", "�X�N�G�A28���Α�", 28, -1, true,true);
	lpFontMng.FontInit("font/Square.ttf", "SquareFont", "�X�N�G�A18���Α�", 18, -1, true, true);
	lpFontMng.FontInit("font/Square.ttf", "SquareFont", "�X�N�G�A20���Α�", 20, -1, true, true);
	// ������
	Cnt = 0;
	flasing = FLASING_LOW_CNT;
	ChangeSceneFlag = false;
	startPadNum = 0;

	mode = MODE_SCENE_NOW;

	return 0;
}
