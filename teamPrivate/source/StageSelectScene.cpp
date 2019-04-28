#include "DxLib.h"
#include "SoundMng.h"
#include "StageSelectScene.h"
#include "SceneMng.h"
#include "GameScene.h"

#define PAGE_SIZE_X	1280
#define PAGE_SIZE_Y 720

#define PI 3.14159265258979

StageSelectScene::StageSelectScene(std::array<CHAR_ID, PLAYER_MAX>ID, int num)
{
	for (int n = 0; n < 4; n++)
	{
		this->ID[n] = ID[n];
	}

	this->num = num;

	Init();
}

StageSelectScene::~StageSelectScene()
{

}

unique_Base StageSelectScene::Update(unique_Base own, const GameController &ctrl)
{
	this->controller = ctrl;

	if (mode == MODE_SCENE_NEXT)
	{
		Draw();
		// �����������łł����ׯ���p�̶���
		flashCnt += flashCnt / 30;
		if (flashCnt >= 150)
		{
			return make_unique<GameScene>(ID, num, stageID);
		}
		return std::move(own);	// ��݌p��
	}

	if (mode == MODE_SCENE_BACK)
	{
		if (PageDraw())
		{
			// �߰�ނ��߂���I������缰ݑJ��
			return make_unique<SelectScene>();
		}

		return move(own);	// ��݌p��
	}


	for (int n = 0; n < 4; n++)
	{
		if (mode != MODE_SCENE_NOW)
		{
			break;
		}

		auto MovCursor = [&](int input, VECTOR2 move)
		{
			if ( ctrl.GetInputDown(input, n) )
			{
				auto modeEndPos = tblPos + move;
				if (modeEndPos.x >= 0 && modeEndPos.x < stageTbl[0].size()
				 && modeEndPos.y >= 0 && modeEndPos.y < stageTbl.size())
				{
					tblPos	= modeEndPos;
					stageID = stageTbl[tblPos.y][tblPos.x].id;
					cursor	= stageTbl[tblPos.y][tblPos.x].drawPos;
					stageDrawID = stageTbl[tblPos.y][tblPos.x].id;
					if (stageID != STAGE_RANDUM)
					{
						// SE
						lpSoundMng.SetSound("����SE");
					}
					else
					{
						// SE
						lpSoundMng.SetSound("�h��");
					}
				}
			}
		};

		if (!endFlag)
		{
			// �I�𶰿ق̈ړ�
			MovCursor(PAD_INPUT_UP,		VECTOR2(0, -1));
			MovCursor(PAD_INPUT_DOWN,	VECTOR2(0, +1));
			MovCursor(PAD_INPUT_LEFT,	VECTOR2(-1, 0));
			MovCursor(PAD_INPUT_RIGHT,	VECTOR2(+1, 0));

			// B���݂ŷ�׾ڂ֑J��
			if (ctrl.GetInputDown(GPAD_INPUT_B, n))
			{
				pageImage = MakeScreen(PAGE_SIZE_X, PAGE_SIZE_Y);
				SetDrawScreen(pageImage);
				DrawGraph(0, 0, lpSceneMng.GetTmpBuffer(), true);
				SetDrawScreen(DX_SCREEN_BACK);

				turnThePage = TurnThePage(VECTOR2(PAGE_SIZE_X, PAGE_SIZE_Y), VECTOR2(0, 0), pageImage, lpImageMng.GetID("image/�L�����Z���p/PageSerect.png")[0], 1.5f, true);

				mode = MODE_SCENE_BACK;
				// SE
				lpSoundMng.SetSound("��ݾ�SE");
				break;
			}
		}
		// ��������
		if (ctrl.GetInputDown(PAD_INPUT_8, n))
		{
			if (endFlag)
			{
				if (stageID == STAGE_RANDUM)
				{
					stageID = static_cast<STAGE_ID>((rand() % (static_cast<int>(STAGE_MAX) - 1)));
				}

				lpSoundMng.SetSound("���[�v");
				mode = MODE_SCENE_NEXT;
			}
		}
		// A����
		if (ctrl.GetInputDown(GPAD_INPUT_A, n))
		{
			if (!endFlag)
			{
				flashDrawCnt = 0;
				endFlag = true;

				if (stageID != STAGE_RANDUM)
				{
					// SE
					lpSoundMng.SetSound("����SE");
				}
				else
				{
					// SE
					lpSoundMng.SetSound("�h�h��");
				}
			}
		}
		if (ctrl.GetInputDown(GPAD_INPUT_B, n))
		{
			if (endFlag)
			{
				flashDrawCnt = 0;
				endFlag = false;

				// SE
				lpSoundMng.SetSound("��ݾ�SE");
			}
		}
	}

	Draw();

	return move(own);	// ��݌p��
}

bool StageSelectScene::PageDraw()
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

bool StageSelectScene::Draw(void)
{
	ClsDrawScreen();

	// �w�i
	DrawGraph(0, 0, lpImageMng.GetID("image/�X�e�[�W�Z���N�g�p/StageSelect.png")[0], true);
	
	DrawGraph(50, 60, lpImageMng.GetID("image/�X�e�[�W�Z���N�g�p/NamePlate.jpg")[0], true);

	DrawStringToHandle(10, 5, "�X�e�[�W�Z���N�g", 0xff0000, lpFontMng.SetFont("���ʂ�40��"));

	DrawExtendGraph(stageTbl[tblPos.y][tblPos.x].startPos.x,
		stageTbl[tblPos.y][tblPos.x].startPos.y,
		stageTbl[tblPos.y][tblPos.x].endPos.x,
		stageTbl[tblPos.y][tblPos.x].endPos.y,
		stageTbl[tblPos.y][tblPos.x].imageR, true);
	
	// �����̉摜
	DrawGraph(0, 210, stageTbl[tblPos.y][tblPos.x].imageL, true);

	DrawGraph(25, 507, lpImageMng.GetID("image/�X�e�[�W�Z���N�g�p/Comment.png")[0], true);

	// ����������荶�����ɔz�u

	auto SelectText = [&](STAGE_ID ID, const TCHAR* name, const TCHAR* text)
	{
		if (stageDrawID == ID)
		{
			if (stageDrawID != STAGE_RANDUM)
			{
				DrawStringToHandle(
					(600 - GetDrawStringWidthToHandle(static_cast<string>(name).c_str(), static_cast<int>(strlen(name)),
						lpFontMng.SetFont("���ʂ�60"))) / 2,
					95,
					name, 0x000000, lpFontMng.SetFont("���ʂ�60"));
				// ������
				DrawStringToHandle(
					40, 517,
					text, 0xffffff, lpFontMng.SetFont("���ʂ�25��"));

			}
			else
			{
				DrawGraph(
					149,
					70,
					lpImageMng.GetID("image/�L�����Z���p/�����_��.png")[0], true);
				// ������
				DrawStringToHandle(
					40, 557,
					"���}������I��", 0xff0000, lpFontMng.SetFont("EP80"));
			}
		}
	};

	SelectText( STAGE_SCHOOL,
				"�������[",
				"����ɂ񂪂�����Ă��邵�傤��������\n"
				"�z�[�����[��������10�Ղ�܂��ɂȂ��\n"
				"��������ɂ��Ȃ���������\n"
				"�����������񂹂��������Ă���");
	
	SelectText( STAGE_TREE_OF_LEGEND,
				"�`���̎�",
				"���������̂����Ă��ɂ͂��Ă��邫\n"
				"���̂��̂����łł����J�b�v����\n"
				"�ނ��΂��Ƃ������킳��\n"
				"��������Ȃ������胉�W�o���_���E�E�E");
	
	SelectText( STAGE_ARCADE,
				"����[�Ă񂪂�",
				"�����̂��������₨�₳��\n"
				"�����₳������������₳��\n"
				"�����Ăق������̂��ǂ�������\n"
				"�����ɂ񂫂̂������₳�񂪂���");

	SelectText( STAGE_RANDUM,
				"�����_��",
				"\n"
				"�v���C����X�e�[�W��\n"
				"�����_���ł��܂��I");

	// �ްѼ�݈ڍs���ɕ\�����Ȃ�
	if (mode != MODE_SCENE_NEXT)
	{
		// �_�ŏ���
		flashDrawCnt++;

		if (!endFlag)
		{
			// ���̕`��
			for (int i = 0; i <= 1; i++)
			{
				DrawRotaGraph(
					dirPosTbl[stageID][i].x + cursor.x,
					dirPosTbl[stageID][i].y + cursor.y,
					1.0, dirRotaTbl[stageID][i], lpImageMng.GetID("image/�L�����Z���p/�₶�邵R.png")[0], true);
			}
		}
		if (flashDrawCnt / FLASING_LOW_CNT % 2 == 0)
		{
			string str;
			str = (endFlag ? "�X�^�[�g�{�^���ŃQ�[���J�n" : "�X�^�[�g�{�^���ŃX�e�[�W����");
			/*DrawStringToHandle(270, 0, str.c_str(), 0xff0000, lpFontMng.SetFont("���ʂ�50��"));*/
			if (!endFlag)
			{
				DrawStringToHandle(900, 5, "�`:�����Ă�", 0xffffff, lpFontMng.SetFont("���ʂ�30��"));
			}
			else
			{
				std::string StageName = ("�X�^�[�g�{�^���Ńo�g���������I");
				int len = static_cast<int>(strlen("�X�^�[�g�{�^���Ńo�g���������I"));

				DrawStringToHandle(
					(SCREEN_SIZE_X - GetDrawStringWidthToHandle(StageName.c_str(), len, lpFontMng.SetFont("���ʂ�60��"))) / 2,
					SCREEN_SIZE_Y / 2 - 30,
					"�X�^�[�g�{�^���Ńo�g���������I", 0xff0000, lpFontMng.SetFont("���ʂ�60��"));
			}

			// ��ɕ\��
			DrawStringToHandle(1100, 5, "�a:���ǂ�", 0xffffff, lpFontMng.SetFont("���ʂ�30��"));
		}
	}

	// �ׯ��
	if (mode == MODE_SCENE_NEXT)
	{
		DrawRotaGraph(VECTOR2(cursor.x, cursor.y), flashCnt, flashCnt,
			lpImageMng.GetID("image/�X�e�[�W�Z���N�g�p/Flash.png")[0], true);
	}

	//---------------------------------------------------------------------------------------
	SetDrawScreen(DX_SCREEN_BACK);						// tmpBuffer�ɕ`�悵�Ă���(SceneMng��SysInit�Őݒ�)�̂�backScreen�ɖ߂�
	ClsDrawScreen();
	DrawGraph(0, 0, lpSceneMng.GetTmpBuffer(), true);	//backScreen�ɍ��܂ł̕`��(tmpBuffer)������ �����ŉ�ʌ��ʂ�ǉ��ł���
	SetDrawScreen(lpSceneMng.GetTmpBuffer());			// tmpBuffer�ɕ`�悷��悤�ɕύX														//--------------------------------------------------------------------------------------

	ScreenFlip();

	return true;
}

int StageSelectScene::Init(void)
{
	//SE�BGM�̓o�^
	lpSoundMng.SoundInit("sound/Select.mp3", "�ڸ�BGM");
	lpSoundMng.SoundInit("sound/SE��/Warp.mp3", "���[�v",80);
	lpSoundMng.SoundInit("sound/SE��/DecisionSE.mp3", "����SE");
	lpSoundMng.SoundInit("sound/SE��/SelectSE.mp3", "����SE");
	lpSoundMng.SoundInit("sound/SE��/CancelSE.mp3", "��ݾ�SE");
	lpSoundMng.SoundInit("sound/SE��/����1.mp3", "�h��");
	lpSoundMng.SoundInit("sound/SE��/����2.mp3", "�h�h��");

	if (!lpSoundMng.CheckPlaySound("�ڸ�BGM"))
	{
		// BGM�̍Đ�
		lpSoundMng.SetSound("�ڸ�BGM", true);
	}

	gameSceneImage = {
		lpImageMng.GetID("image/�Q�[���V�[���p/School.png")[0],
		lpImageMng.GetID("image/�Q�[���V�[���p/TreeOfLegend.png")[0],
		lpImageMng.GetID("image/�Q�[���V�[���p/Arcade.png")[0],
	};

	stageTbl[0][0] = {
		STAGE_SCHOOL,			
		VECTOR2(800,200),	
		lpImageMng.GetID("image/�X�e�[�W�Z���N�g�p/LeftSchool.png")[0],
		lpImageMng.GetID("image/�X�e�[�W�Z���N�g�p/SelectSchool.png")[0],
		VECTOR2(680, 30),
		VECTOR2(930, 380),
	}; 
	stageTbl[0][1] = {
		STAGE_TREE_OF_LEGEND,	
		VECTOR2(1110,190),	
		lpImageMng.GetID("image/�X�e�[�W�Z���N�g�p/LeftWood.png")[0],
		lpImageMng.GetID("image/�X�e�[�W�Z���N�g�p/SelectWood.png")[0],
		VECTOR2(930, 70),
		VECTOR2(1280, 320),
	};
	stageTbl[1][0] = {
		STAGE_ARCADE,			
		VECTOR2(850,520),	
		lpImageMng.GetID("image/�X�e�[�W�Z���N�g�p/LeftArcade.png")[0],
		lpImageMng.GetID("image/�X�e�[�W�Z���N�g�p/SelectArcade.png")[0],
		VECTOR2(680, 400),
		VECTOR2(1010, 630),
	};
	stageTbl[1][1] = {
		STAGE_RANDUM,			
		VECTOR2(1160,522),	
		lpImageMng.GetID("image/�X�e�[�W�Z���N�g�p/random.png")[0],
		lpImageMng.GetID("image/�X�e�[�W�Z���N�g�p/SelectRandom.png")[0],
		VECTOR2(1040, 350),
		VECTOR2(1270, 690),
	};

	dirPosTbl = {
		// X��				// Y��
		 VECTOR2(130,0),	VECTOR2(0,170),
		 VECTOR2(-170,0),	VECTOR2(0,125),
		 VECTOR2(160,0),	VECTOR2(0,-120),
		 VECTOR2(-120,0),	VECTOR2(0,-170) 
	};
	dirRotaTbl = {
		// X��				// Y��
		0.0,				PI / 2,
		PI,					PI / 2,
		0.0,			   -PI / 2,
		PI,				   -PI / 2,
	};

	mode			= MODE_SCENE_NOW;
	endFlag			= false; 
	flashDrawCnt	= 0;

	tblPos			= {};
	stageID			= stageTbl[tblPos.y][tblPos.x].id;
	cursor			= stageTbl[tblPos.y][tblPos.x].drawPos;

	stageDrawID = stageTbl[tblPos.y][tblPos.x].id;

	flashCnt = 0.01;

	// ̫�Ă̓o�^
	lpFontMng.FontInit("font/TanukiMagic.ttf", "���ʂ������}�W�b�N", "���ʂ�50", 50, 9, false, false);
	lpFontMng.FontInit("font/TanukiMagic.ttf", "���ʂ������}�W�b�N", "���ʂ�60", 60, 9, false, false);
	lpFontMng.FontInit("font/TanukiMagic.ttf", "���ʂ������}�W�b�N", "���ʂ�25��", 25, 9, true, false);
	lpFontMng.FontInit("font/TanukiMagic.ttf", "���ʂ������}�W�b�N", "���ʂ�40��", 40, 9, true, false);
	lpFontMng.FontInit("font/TanukiMagic.ttf", "���ʂ������}�W�b�N", "���ʂ�50��", 50, 9, true, false);
	lpFontMng.FontInit("font/TanukiMagic.ttf", "���ʂ������}�W�b�N", "���ʂ�60��", 60, -1, true, false);

	return 0;
}