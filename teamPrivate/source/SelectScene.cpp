#include "DxLib.h"
#include "SoundMng.h"
#include "SelectScene.h"
#include "SceneMng.h"
#include "GameScene.h"
#include "StageSelectScene.h"

// ��`
#define MARGIN_X				(90)	// ��ʗ]��X
#define MARGIN_Y				(50)	// ��ʗ]��Y
#define SELECT_SCREEN_SIZE_X	(460)	// �e��ڲ԰�̳���޳����X
#define SELECT_SCREEN_SIZE_Y	(310)	// �e��ڲ԰�̳���޳����Y
#define SELECT_ICON_X			(144)	// �I�𱲺݂̻���
#define SELECT_ICON_Y			(576)	// �I�𱲺݂̻���
#define CHAR_ICON_X				(180)	// ��׸������X
#define PLAYER_CURSOR			(76)	// ��ڲ԰�̶��ٻ���X
#define CURSOR_FARST_POS		((SCREEN_SIZE_Y - SELECT_ICON_Y)/2)	// ���ُ����ʒu
#define N_CFONT_SIZE_X			(140)	// �r���Q���\̫�Ă̎n�܂�̈ʒuX
#define N_CFONT_SIZE_Y			(150)	// �r���Q���\̫�Ă̎n�܂�̈ʒuY

#define PAGE_SIZE_X	1280
#define PAGE_SIZE_Y 720

#define DIR_SIZE_X	60					// �₶�邵�摜�̑傫��
#define DIR_SIZE_Y	60					// �₶�邵�摜�̑傫��

#define FADE_SPEED		180		// ̪��޲݂���������܂ł̎���(�ڰ�)

SelectScene::SelectScene(bool whiteInFlag)
{
	alpha = whiteInFlag ? 255 : 0;
	// �����x��FADE_SPEED(�ڰ�)�����čő�(255)�ɂ���
	alpha.set(0, FADE_SPEED);		
	joinPlayerNum = GetJoypadNum();
	Init();
}

SelectScene::~SelectScene()
{
}

unique_Base SelectScene::Update(unique_Base own, const GameController & controller)
{
	// �߯�ސڑ����̍X�V
	joinPlayerNum = GetJoypadNum();
	// ���۰װ���̊i�[
	this->controller = controller;

	if (alpha)
	{
		// �`�揈��
		Draw();

		return std::move(own);	// ��݌p��
	}

	if (mode == MODE_SCENE_NEXT)
	{
		if (PageDraw())
		{
			// ��ʑJ��
			return std::make_unique<StageSelectScene>(playerIdArray, SelectEndNum());
		}

		return std::move(own);	// ��݌p��
	}

	for (int k = 0; k < PLAYER_MAX; k++)
	{
		// �������݂ŎQ��
		if (controller.GetInputDown(PAD_INPUT_8, k))
		{
			// ��ڲ԰�̎Q��
			if (player[k].ID == ID_NON)
			{

				// SE�̍Đ�
				lpSoundMng.SetSound("�Q��SE");

				// �����͎P��
				player[k].ID = ID_Umb;
				player[k].posY = CURSOR_FARST_POS;
			}

		}

		// �I������
		if (player[k].ID != ID_NON && !player[k].flag)
		{
			if (controller.GetInputDown(PAD_INPUT_LEFT, k))
			{

				// �I��ׂ̕ύX
				player[k].ID =
					(CHAR_ID)(static_cast<int>(player[k].ID) <= static_cast<int>(CHAR_ID::ID_Umb)
						? static_cast<int>(CHAR_ID::ID_Rnd)
						: static_cast<int>(player[k].ID) - 1);

				if (player[k].ID != ID_Rnd)
				{
					// SE
					lpSoundMng.SetSound("����SE");
				}
				else
				{
					// SE
					lpSoundMng.SetSound("�h��");
				}

				//player[k].posY -= SELECT_ICON_X;
			}
			if (controller.GetInputDown(PAD_INPUT_RIGHT, k))
			{

				// ������
				// �I��ׂ̕ύX
				player[k].ID =
					(CHAR_ID)(static_cast<int>(player[k].ID) >= static_cast<int>(CHAR_ID::ID_Rnd)
						? static_cast<int>(CHAR_ID::ID_Umb)
						: static_cast<int>(player[k].ID) + 1);

				if (player[k].ID != ID_Rnd)
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
		// �g�̈ړ�
		if (player[k].ID != ID_NON)
		{
			//if (player[k].posY >= SCREEN_SIZE_Y - SELECT_ICON_X)
			//{
			//	player[k].posY = CURSOR_FARST_POS;
			//}
			//if (player[k].posY < CURSOR_FARST_POS)
			//{
			//	player[k].posY = SCREEN_SIZE_Y + CURSOR_FARST_POS - SELECT_ICON_X * 2;
			//}
			// ����
			if (controller.GetInputDown(GPAD_INPUT_A, k))
			{
				if (!player[k].flag)
				{
					if (player[k].ID != ID_Rnd)
					{
						// SE
						lpSoundMng.SetSound("����SE");
					}
					else
					{
						// SE
						lpSoundMng.SetSound("�h�h��");
					}
					// ��ڲ԰��׌���
					player[k].flag = true;
				}
			}
			// ������
			if (controller.GetInputDown(GPAD_INPUT_B, k))
			{
				if (player[k].flag)
				{
					// SE
					lpSoundMng.SetSound("��ݾ�SE");

					// ������
					player[k].flag = false;
				}
				else
				{
					// SE
					lpSoundMng.SetSound("��ݾ�SE");

					// ��ڲ԰��NON��(�s�Q��)
					player[k].ID = ID_NON;
				}
			}
		}

		// ��ׂ�I�����I������l����2�ȏ�̎��ɹްт��J�n�ł���(���̏�Ԃł̓r���Q�����ł���)
		// ���ޯ�ޒ��͏���
		 if (SelectEndNum() >= 2)
		{
			bool flag = true;
			for (int j = 0; j < PLAYER_MAX; j++)
			{
				if (player[j].ID != ID_NON && !player[j].flag)
				{
					flag = false;
					break;
				}
			}
			if (flag)
			{
				if (controller.GetInputDown(PAD_INPUT_8, k))
				{
					playerIdArray = {};
					for (int k = 0; k < PLAYER_MAX; k++)
					{
						if (player[k].ID == ID_NON) { continue; }
						playerIdArray[k] = player[k].ID;
					}

					int selectSceneImage = MakeScreen(PAGE_SIZE_X, PAGE_SIZE_Y);
					SetDrawScreen(selectSceneImage);
					DrawGraph(0, 0, lpSceneMng.GetTmpBuffer(), true);
					SetDrawScreen(DX_SCREEN_BACK);

					turnThePage = TurnThePage(VECTOR2(PAGE_SIZE_X, PAGE_SIZE_Y), VECTOR2(0, 0), selectSceneImage, lpImageMng.GetID("image/�L�����Z���p/�V�[���ڍs�p.png")[0], 1.0f);
					mode = MODE_SCENE_NEXT;
				}
			}
		}
	}

	// �`�揈��
	Draw();

	return std::move(own);	// ��݌p��
}

bool SelectScene::PageDraw(void)
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

bool SelectScene::Draw(void)
{
	ClsDrawScreen();

	// �w�i
	DrawGraph(0, 0, lpImageMng.GetID("image/�L�����Z���p/Select.png")[0], true);

	// �ڸı���
	DrawGraph((SCREEN_SIZE_X - SELECT_ICON_X) / 2, (SCREEN_SIZE_Y - SELECT_ICON_Y) / 2, lpImageMng.GetID("image/�L�����Z���p/CharaSelect.png")[0], true);

	for (int k = 0; k < PLAYER_MAX; k++)
	{
		// �_�ŗp����
		player[k].cnt++;

		if (player[k].ID != ID_NON)
		{
			// ÷��
			auto SelectText = [&](int color, CHAR_ID ID, const TCHAR* ruby, const TCHAR* name, const TCHAR* wepon, const TCHAR* comment, const TCHAR* specialRuby, const TCHAR* special, const TCHAR* specialText)
			{
				if (player[k].ID == ID)
				{
					// �Q���g
					DrawGraph(
						MARGIN_X + ((SCREEN_SIZE_X / 2) * (k % 2)),
						MARGIN_Y + ((SELECT_SCREEN_SIZE_Y) * (k / 2)),
						lpImageMng.GetID("image/�L�����Z���p/NowSelect.png")[0], true);

					if (player[k].ID != ID_Rnd)
					{
						DrawStringToHandle(
							(MARGIN_X + 700 + ((MARGIN_X * 9 + SELECT_SCREEN_SIZE_X + 10) * (k % 2)) - GetDrawStringWidthToHandle(static_cast<string>("�Ԃ�").c_str(), static_cast<int>(strlen(static_cast<const char*>("�Ԃ�"))), lpFontMng.SetFont("���ʂ�15"))) / 2,
							70 - 10 + ((SELECT_SCREEN_SIZE_Y) * (k / 2)),
							"�Ԃ�", 0x000000, lpFontMng.SetFont("���ʂ�15"));

						DrawStringToHandle(
							(MARGIN_X + 300 + ((MARGIN_X * 2 + SELECT_SCREEN_SIZE_X) * (k % 2))) - GetDrawStringWidthToHandle(static_cast<string>("�Ђ����킴").c_str(), static_cast<int>(strlen(static_cast<const char*>("�Ђ����킴"))), lpFontMng.SetFont("���ʂ�20��")) / 2,
							240 - 23 + ((SELECT_SCREEN_SIZE_Y) * (k / 2)),
							"�Ђ����킴", 0xff0000, lpFontMng.SetFont("���ʂ�20��"));
					}
					else
					{
						DrawVStringToHandle(
							(540 + ((MARGIN_X * 2 + SELECT_SCREEN_SIZE_X) * (k % 2))) - GetDrawStringWidthToHandle(static_cast<string>("�Ђ����킴").c_str(), static_cast<int>(strlen(static_cast<const char*>("�Ђ����킴"))), lpFontMng.SetFont("���ʂ�20��")) / 2,
							MARGIN_Y + 5 + ((SELECT_SCREEN_SIZE_Y) * (k / 2)),
							"�����͊���\n�@�n�܂��Ă���", 0xff0000, lpFontMng.SetFont("EP60"));
					}

					DrawStringToHandle(
						(MARGIN_X + 275 + ((MARGIN_X * 9 + SELECT_SCREEN_SIZE_X + 10) * (k % 2)) - GetDrawStringWidthToHandle(static_cast<string>(ruby).c_str(), static_cast<int>(strlen(static_cast<const char*>(ruby))), lpFontMng.SetFont("���ʂ�10"))) / 2,
						80 + ((SELECT_SCREEN_SIZE_Y) * (k / 2)),
						ruby, 0x000000, lpFontMng.SetFont("���ʂ�10"));

					DrawStringToHandle(
						(MARGIN_X + 275 + ((MARGIN_X * 9 + SELECT_SCREEN_SIZE_X + 10) * (k % 2)) - GetDrawStringWidthToHandle(static_cast<string>(name).c_str(), static_cast<int>(strlen(static_cast<const char*>(name))), lpFontMng.SetFont("���ʂ�20"))) / 2,
						90 + ((SELECT_SCREEN_SIZE_Y) * (k / 2)),
						name, 0x000000, lpFontMng.SetFont("���ʂ�20"));

					DrawStringToHandle(
						(MARGIN_X + 700 + ((MARGIN_X * 9 + SELECT_SCREEN_SIZE_X + 10) * (k % 2)) - GetDrawStringWidthToHandle(static_cast<string>(wepon).c_str(), static_cast<int>(strlen(static_cast<const char*>(wepon))), lpFontMng.SetFont("���ʂ�40"))) / 2,
						90 - 10 + ((SELECT_SCREEN_SIZE_Y) * (k / 2)),
						wepon, color, lpFontMng.SetFont("���ʂ�40"));

					DrawStringToHandle(
						MARGIN_X + 15 + (SELECT_SCREEN_SIZE_X / 3) + ((MARGIN_X * 2 + SELECT_SCREEN_SIZE_X) * (k % 2)),
						140 - 15 + ((SELECT_SCREEN_SIZE_Y) * (k / 2)),
						comment, 0x000000, lpFontMng.SetFont("���ʂ�18"));

					DrawStringToHandle(
						(MARGIN_X + 300 + ((MARGIN_X * 2 + SELECT_SCREEN_SIZE_X) * (k % 2))) - GetDrawStringWidthToHandle(static_cast<string>(specialRuby).c_str(), static_cast<int>(strlen(static_cast<const char*>(specialRuby))), lpFontMng.SetFont("���ʂ�20")) / 2,
						270 - 25 + ((SELECT_SCREEN_SIZE_Y) * (k / 2)),
						specialRuby, 0x000000, lpFontMng.SetFont("���ʂ�20"));

					DrawStringToHandle(
						(MARGIN_X + 300 + ((MARGIN_X * 2 + SELECT_SCREEN_SIZE_X) * (k % 2))) - GetDrawStringWidthToHandle(static_cast<string>(special).c_str(), static_cast<int>(strlen(static_cast<const char*>(special))), lpFontMng.SetFont("���ʂ�25")) / 2,
						290 - 25 + ((SELECT_SCREEN_SIZE_Y) * (k / 2)),
						special, color, lpFontMng.SetFont("���ʂ�25"));

					//DrawStringToHandle(
					//( MARGIN_X + 300 + ( (MARGIN_X * 2 + SELECT_SCREEN_SIZE_X) * (k % 2) ) ) - GetDrawStringWidthToHandle( static_cast<string>(specialText).c_str(), static_cast<int>( strlen( static_cast<const char*>(specialText) ) ), lpFontMng.SetFont("���ʂ�18") )/2 ,
					//320 -25 + ( (SELECT_SCREEN_SIZE_Y) * (k / 2) ),
					//specialText, 0x000000, lpFontMng.SetFont("���ʂ�18") );
					DrawStringToHandle(
						MARGIN_X + 15 + (SELECT_SCREEN_SIZE_X / 3) + ((MARGIN_X * 2 + SELECT_SCREEN_SIZE_X) * (k % 2)),
						320 - 25 + ((SELECT_SCREEN_SIZE_Y) * (k / 2)),
						specialText, 0x000000, lpFontMng.SetFont("���ʂ�18"));
				}
			};

			SelectText(0x0000ff,
				ID_Umb,
				"���߂� ������",
				"�J��~��",
				"����",
				"�X�^���_�[�h�ŁA���������₷��\n"
				"�������イ�ŏケ�������������\n"
				"�ӂ�ӂ킷���I\n"
				"���債�񂵂�ނ�",
				"���ꌎ",
				"�[The Moon Thrust�[",
				"�߂ɂ��Ƃ܂�ʂ͂₳�̂���\n"
				"�����Ă񂬂������\n"
				"(�O���̓G�ɑ�_���[�W)");

			SelectText(0xff00ff,
				ID_Bro,
				"������ �ق���",
				"�|���",
				"�ف[��",
				"���[�`�͂Ȃ�������\n"
				"����������傭�͂Ђ���\n"
				"�W�����v���P�񂨂����ł��邼�I"
				"\n",
				"�{��邨�{�l",
				"�[Rage of Chickens�[",
				"�܂ق�������������A��������\n"
				"����ɂ�Ƃ肳�ܣ���ӂ��Ă���\n"
				"(�O���̓G�ɑ��~�߁��_���[�W)");

			SelectText(0xff8000,
				ID_Lid,
				"�ӂ��� ������",
				"�o�t�u",
				"���݂΂��̂ӂ�",
				"�������������Â炢����\n"
				"���߂��������̂Ƃ����񂪂悢\n"
				"�Ђ����킴�Ń��e�L�ɂȂ�邼�I\n"
				"�΂����炵�������Ȃ��ɃI�X�X��",
				"�b�̙��K",
				"�[Beast Roar�[",
				"�������Ă��Ƀ��~�b�^�[���͂���\n"
				"�₹���̂�����������ق�����\n"
				"(��莞�Ԗ��G���X�e�[�^�X�㏸)");


			//SelectText( 0x000000,
			//			ID_Rnd,
			//			"��񂾂�",
			//			"�����_��",
			//			"�H�H�H",
			//			"\n"
			//			"��������񂽂������\n"
			//			"�����_���ŃL�����N�^�[��\n"
			//			"���܂��I",
			//			"�\�\�\",
			//			"�\�\�\�\�\",
			//			"");

			SelectText(0x000000,
				ID_Rnd,
				"",
				"",
				"",
				"\n"
				"\n"
				"\n"
				"",
				"",
				"",
				"");

			// ��ױ���	// ���I��������тłȂ��ꍇ
			if (player[k].ID != ID_NON && player[k].ID != ID_Rnd)
			{
				std::string iconPass = "image/" + charName[player[k].ID] + "/�A�C�R��/icon.png";
				DrawGraph(
					(SCREEN_SIZE_X / 4) - 215 + ((MARGIN_X * 2 + SELECT_SCREEN_SIZE_X) * (k % 2)),
					115 + ((SELECT_SCREEN_SIZE_Y) * (k / 2)),
					lpImageMng.GetID(iconPass)[0], true);
			}
			// ��ױ���	// ����ёI�������ꍇ  // ���ޯ�ޗp
			if (player[k].ID == ID_Rnd)
			{
				DrawGraph(
					MARGIN_X + 10 + ((MARGIN_X * 2 + SELECT_SCREEN_SIZE_X) * (k % 2)),
					150 + ((SELECT_SCREEN_SIZE_Y) * (k / 2)),
					lpImageMng.GetID("image/�L�����Z���p/�����_��.png")[0], true);
			}

			// 1P,2P�Ȃǂ̕\��
			//DrawRectGraph(
			//	((SCREEN_SIZE_X / 2) - PLAYER_CURSOR) + (PLAYER_CURSOR * (k % 2)),
			//	(PLAYER_CURSOR * (k / 2)) ,
			//	64 * k, 0,
			//	64, 32,
			//	lpImageMng.GetID("image/UI/1234.png")[0], true, false);
			DrawGraph(
				(MARGIN_X - 5) + ((SCREEN_SIZE_X / 2) * (k % 2)),
				(MARGIN_Y + 8) + ((SELECT_SCREEN_SIZE_Y) * (k / 2)),
				lpImageMng.GetID("image/UI/1234.png")[k], true);


			// �S�����s�Q����ԂłȂ��ꍇ��ɕ\������
			if (player[k].cnt / FLASING_LOW_CNT % 2 == 0)
			{
				DrawStringToHandle(1100, 5, "�a:���ǂ�", 0xffffff, lpFontMng.SetFont("���ʂ�30��"));
			}

			// ��׸�������߂��Ƃ�
			if (player[k].flag)
			{
				if (player[k].cnt / FLASING_HIGH_CNT % 2 == 0)
				{
					if (player[k].ID != ID_Rnd)
					{
						// ����÷��
						DrawStringToHandle(
							(MARGIN_X + 20) + ((SCREEN_SIZE_X / 2) * (k % 2)),
							(SCREEN_SIZE_Y / 2) - 40 + ((SELECT_SCREEN_SIZE_Y) * (k / 2)),
							"�����тn�j", 0xFF0000, lpFontMng.SetFont("���ʂ�20��"));
					}
					else
					{
						DrawStringToHandle(
							(MARGIN_X + 160) + ((SCREEN_SIZE_X / 2) * (k % 2)),
							(SCREEN_SIZE_Y / 2) - 40 + ((SELECT_SCREEN_SIZE_Y) * (k / 2)),
							"�o�劮���I", 0xFF0000, lpFontMng.SetFont("EP25"));
					}
				}
			}
			else
			{
				//// ����÷��
				//DrawStringToHandle(
				//	(SCREEN_SIZE_X / 5 - 30) + ((SCREEN_SIZE_X / 2) * (k % 2)),
				//	(SCREEN_SIZE_Y / 2) - 40 + ((SELECT_SCREEN_SIZE_Y) * (k / 2)),
				//	"�`�{�^���ł����Ă�", 0xFF0000, lpFontMng.SetFont("���ʂ�20"));

				// ���E�L�[���������u�Ԃ����摜��1.5�{�Ɋg�傷��
				double dirExtRateL = (controller.GetInputDown(PAD_INPUT_LEFT, k)) ? 1.1 : 1.0;
				double dirExtRateR = (controller.GetInputDown(PAD_INPUT_RIGHT, k)) ? 1.1 : 1.0;

				// ���L
				DrawRotaGraph(
					MARGIN_X - 45 + ((SCREEN_SIZE_X / 2) * (k % 2)) + DIR_SIZE_X / 2,
					MARGIN_Y - 30 + ((SELECT_SCREEN_SIZE_Y / 2)) + ((SELECT_SCREEN_SIZE_Y) * (k / 2)) + DIR_SIZE_Y / 2,
					dirExtRateL, 0.0,
					lpImageMng.GetID("image/�L�����Z���p/�₶�邵L.png")[0], true);
				// ���R
				DrawRotaGraph(
					MARGIN_X - 45 + ((SCREEN_SIZE_X / 2) * (k % 2)) + DIR_SIZE_X / 2 + 490,
					MARGIN_Y - 30 + ((SELECT_SCREEN_SIZE_Y / 2)) + ((SELECT_SCREEN_SIZE_Y) * (k / 2)) + DIR_SIZE_Y / 2,
					dirExtRateR, 0.0,
					lpImageMng.GetID("image/�L�����Z���p/�₶�邵R.png")[0], true);

				if (player[k].cnt / FLASING_LOW_CNT % 2 == 0)
				{
					// ÷��
					DrawStringToHandle(900, 5, "�`:�����Ă�", 0xffffff, lpFontMng.SetFont("���ʂ�30��"));
				}
			}
		}
		else
		{
			//// �s�Q���g
			//DrawGraph(
			//	MARGIN_X + ((SCREEN_SIZE_X / 2) * (k % 2)),
			//	MARGIN_Y + ((SELECT_SCREEN_SIZE_Y) /** (k / 2)*/),
			//	lpImageMng.GetID("image/NoSelect.png")[0], true);

			// �_�ŏ���
			if (player[k].cnt / FLASING_LOW_CNT % 2 == 0)
			{
				// �Q���\����
				DrawStringToHandle(
					N_CFONT_SIZE_X - 30 + ((SCREEN_SIZE_X / 2) * (k % 2)),
					N_CFONT_SIZE_Y + ((SELECT_SCREEN_SIZE_Y) * (k / 2)),
					"�X�^�[�g�{�^����", 0x000000, lpFontMng.SetFont("���ʂ�50"));

				DrawStringToHandle(
					N_CFONT_SIZE_X + ((SCREEN_SIZE_X / 2) * (k % 2)),
					N_CFONT_SIZE_Y + 60 + ((SELECT_SCREEN_SIZE_Y) * (k / 2)),
					"���񂩂ł����", 0x000000, lpFontMng.SetFont("���ʂ�50"));
			}
		}

		// ���ޯ��
		//DrawFormatString(
		//	N_CFONT_SIZE_X - 30 + ((SCREEN_SIZE_X / 2) * (k % 2)),
		//	N_CFONT_SIZE_Y + ((SELECT_SCREEN_SIZE_Y) * (k / 2)) - 80,
		//	0x000000,
		//	"flag:%d", player[k].flag);
		//DrawFormatString(
		//	N_CFONT_SIZE_X - 30 + ((SCREEN_SIZE_X / 2) * (k % 2)),
		//	N_CFONT_SIZE_Y + ((SELECT_SCREEN_SIZE_Y) * (k / 2)) - 60,
		//	0x000000,
		//	"ID  :%d", player[k].ID);
	}

	// ��ׂ�I�����I������l����2�ȏ�̎��ɕ\������
	if (SelectEndNum() >= 2)
	{
		bool flag = true;
		for (int j = 0; j < PLAYER_MAX; j++)
		{
			if (player[j].ID != ID_NON && !player[j].flag)
			{
				flag = false;
				break;
			}
		}

		if (flag)
		{
			flashDrawCnt++;
			// �_�ŏ���
			if (flashDrawCnt / FLASING_LOW_CNT % 2 == 0)
			{
				std::string StageName = ("�X�^�[�g�{�^���������ĂˁI");
				int len = static_cast<int>(strlen("�X�^�[�g�{�^���������ĂˁI"));

				DrawStringToHandle(
					(SCREEN_SIZE_X - GetDrawStringWidthToHandle(StageName.c_str(), len, lpFontMng.SetFont("���ʂ�60��"))) / 2,
					SCREEN_SIZE_Y / 2 - 30, "�X�^�[�g�{�^���������ĂˁI", 0xff0000, lpFontMng.SetFont("���ʂ�60��"));
			}
		}
	}
	else
	{
		flashDrawCnt = 0;
	}

	// debug-----
	//DrawFormatString(0, 0, 0xff0000, "�p�b�h�̐ڑ���:%d", joinPlayerNum);
	/*DrawFormatString(0, 0, 0xff0000, "2�l�ȏ�̎Q���ŃQ�[���X�^�[�g�ł��܂�");*/
	// debug-----

	DrawStringToHandle(10, 5, "�L�����N�^�[�Z���N�g", 0xff0000, lpFontMng.SetFont("���ʂ�40��"));

	// ̪��޲�
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(alpha));
	DrawBox(VECTOR2(0, 0), lpSceneMng.GetScreenSize(), 0xffffff, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	alpha.update();

	//---------------------------------------------------------------------------------------
	SetDrawScreen(DX_SCREEN_BACK);						// tmpBuffer�ɕ`�悵�Ă���(SceneMng��SysInit�Őݒ�)�̂�backScreen�ɖ߂�
	ClsDrawScreen();
	DrawGraph(0, 0, lpSceneMng.GetTmpBuffer(), true);	//backScreen�ɍ��܂ł̕`��(tmpBuffer)������ �����ŉ�ʌ��ʂ�ǉ��ł���
	SetDrawScreen(lpSceneMng.GetTmpBuffer());			// tmpBuffer�ɕ`�悷��悤�ɕύX														//--------------------------------------------------------------------------------------

	ScreenFlip();

	return true;
}

int SelectScene::SelectEndNum(void)
{
	int checkCnt = 0;
	for (int k = 0; k < PLAYER_MAX; k++)
	{
		if (player[k].flag)
		{
			checkCnt++;		// ��׌����ԂȂ綳�Ă�i�߂�
		}
	}

	return checkCnt;
}

int SelectScene::Init(void)
{
	VECTOR2 graphSize;
	VECTOR2 divCnt = { 4,1 };
	GetGraphSize(LoadGraph("image/UI/1234.png"), &graphSize.x, &graphSize.y);
	lpImageMng.GetID("image/UI/1234.png", graphSize / divCnt, divCnt);

	//SE�BGM�̓o�^
	lpSoundMng.SoundInit("sound/Select.mp3", "�ڸ�BGM");
	lpSoundMng.SoundInit("sound/SE��/DecisionSE.mp3", "����SE");
	lpSoundMng.SoundInit("sound/SE��/SelectSE.mp3", "����SE");
	lpSoundMng.SoundInit("sound/SE��/CancelSE.mp3", "��ݾ�SE");
	lpSoundMng.SoundInit("sound/SE��/Participation.mp3", "�Q��SE");
	lpSoundMng.SoundInit("sound/SE��/�L�����N�^�[�������ł�.mp3", "�����ł�");
	lpSoundMng.SoundInit("sound/SE��/����1.mp3", "�h��");
	lpSoundMng.SoundInit("sound/SE��/����2.mp3", "�h�h��");

	// �����ǉ�
	lpSoundMng.SoundInit("sound/SE��/Chicken1.mp3", "������");
	lpSoundMng.SoundInit("sound/SE��/Chicken2.mp3", "���");
	lpSoundMng.SoundInit("sound/SE��/Chicken3.mp3", "���");
	lpSoundMng.SoundInit("sound/SE��/Chicken4.mp3", "�������");
	lpSoundMng.SoundInit("sound/SE��/Bomb.mp3", "����", 85);
	lpSoundMng.SoundInit("sound/SE��/Dead.mp3", "���S");
	lpSoundMng.SoundInit("sound/SE��/Continue.mp3", "����", 85);

	if (!lpSoundMng.CheckPlaySound("�ڸ�BGM"))
	{
		// BGM�̍Đ�
		lpSoundMng.SetSound("�ڸ�BGM", true);
	}

	// ������
	for (int k = 0; k < PLAYER_MAX; k++)
	{
		// �ޮ�ފ��蓖��
		player[k].ID = ID_NON;

		// ���I��
		player[k].flag = false;

		// ���ݑI�����W
		player[k].posY = 0;

		// �_�ŗp����
		player[k].cnt = 0;
	}

	flashDrawCnt = 0;

	charName = {
		"�I���L�����Ȃ�",	//ID_NON
		"�J��~��"		,	//ID_Umb
		"�|���"		,	//ID_Bro
		"�o�t�u"		,	//ID_Lid
		"�G���[",	//ID_Rnd	
	};

	// ̫�Ă̓o�^
	lpFontMng.FontInit("font/TanukiMagic.ttf", "���ʂ������}�W�b�N", "���ʂ�10", 10, -1, false, false);
	lpFontMng.FontInit("font/TanukiMagic.ttf", "���ʂ������}�W�b�N", "���ʂ�15", 15, -1, false, false);
	lpFontMng.FontInit("font/TanukiMagic.ttf", "���ʂ������}�W�b�N", "���ʂ�18", 18, -1, false, false);
	lpFontMng.FontInit("font/TanukiMagic.ttf", "���ʂ������}�W�b�N", "���ʂ�20", 20, -1, false, false);
	lpFontMng.FontInit("font/TanukiMagic.ttf", "���ʂ������}�W�b�N", "���ʂ�25", 25, -1, false, false);
	lpFontMng.FontInit("font/TanukiMagic.ttf", "���ʂ������}�W�b�N", "���ʂ�30", 30, -1, false, false);
	lpFontMng.FontInit("font/TanukiMagic.ttf", "���ʂ������}�W�b�N", "���ʂ�40", 40, -1, false, false);
	lpFontMng.FontInit("font/TanukiMagic.ttf", "���ʂ������}�W�b�N", "���ʂ�50", 50, 9, false, false);
	lpFontMng.FontInit("font/TanukiMagic.ttf", "���ʂ������}�W�b�N", "���ʂ�20��", 20, -1, true, false);
	lpFontMng.FontInit("font/TanukiMagic.ttf", "���ʂ������}�W�b�N", "���ʂ�30��", 30, -1, true, false);
	lpFontMng.FontInit("font/TanukiMagic.ttf", "���ʂ������}�W�b�N", "���ʂ�40��", 40, 9, true, false);
	lpFontMng.FontInit("font/TanukiMagic.ttf", "���ʂ������}�W�b�N", "���ʂ�60��", 60, -1, true, false);
	lpFontMng.FontInit("font/TanukiMagic.ttf", "���ʂ������}�W�b�N", "���ʂ�80��", 80, 9, true, false);
	lpFontMng.FontInit("font/TanukiMagic.ttf", "���ʂ������}�W�b�N", "���ʂ�100��", 100, 9, true, false);
	lpFontMng.FontInit("font/EPGYOBLD.TTF", "EPSON ���s���̂a", "EP25", 25, 9, true, false);
	lpFontMng.FontInit("font/EPGYOBLD.TTF", "EPSON ���s���̂a", "EP50", 50, 9, true, false);
	lpFontMng.FontInit("font/EPGYOBLD.TTF", "@EPSON ���s���̂a", "EP60", 38, 9, true, false);
	lpFontMng.FontInit("font/EPGYOBLD.TTF", "EPSON ���s���̂a", "EP80", 60, 9, true, false);
	lpFontMng.FontInit("font/EPGYOBLD.TTF", "EPSON ���s���̂a", "EP100", 100, 9, true, false);

	lpFontMng.FontInit("font/AVOCADO.TTF", "�A�{�J�h", "�A�{�J�h", 10, 9, false, false);

	mode = MODE_SCENE_NOW;

	// BGM�̍Đ�
	lpSoundMng.SetSound("�����ł�");

	return 0;
}
