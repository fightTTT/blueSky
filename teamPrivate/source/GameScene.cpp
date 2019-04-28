#include "DxLib.h"
#include "SceneMng.h"
#include "GameController.h"
#include "GameScene.h"
#include "Umbrella.h"
#include "Lid.h"
#include "Broom.h"
#include "FloorNormal.h"
#include "FloorMove.h"
#include "FloorCar.h"
#include "ResultScene.h"
#include "SoundMng.h"
#include "time.h"

#define TIME_LIM				(180)	// ���Ԑ���(�b)

#define GAME_START_WAIT_TIME	(180)	// 321�̕\���p
#define START_CHAR_TIME			(60)	// 60�ڰт̊ԁu�����[�ƁI�v�̕\��������

#define CUT_IN_TIME				(180)

#define CHANG_SCENE_CNT			(60)

#define PAGE_SIZE_X				1280
#define PAGE_SIZE_Y				720

#define G_OUT_SPEED				7					// ��ڰ��Ľײ�ނ̑���
#define G_OUT_START_WAIT		(60 * G_OUT_SPEED)	// �������݉��������ڰ��Ľײ�ނ��n�܂�܂ł̑҂�����
#define G_OUT_END_WAIT			(40 * G_OUT_SPEED)	// ��ڰ��Ľײ�ނ��I��A321���n�܂�܂ł̑҂�����

#define FADE_SPEED				180		// ̪��ޱ�Ă���������܂ł̎���(�ڰ�)

// ���ł�
#define EXT_RATE				2.1f		// �ð�޾ڸĂ̍��摜���ٻ��ނ̉�����1�Ȃ̂�

GameScene::GameScene()
{
	Init();
}

GameScene::GameScene(CHAR_ID playerID[PLAYER_MAX], int joinPlayerNum, STAGE_ID ID)
{
	// static resultInfo�̏�����
	Obj::ResultInfoInit();

	// ����ѷ�ׂ�I�������ꍇ�̏���
	for (int k = 0; k < PLAYER_MAX; k++)
	{
		if (playerID[k] == ID_Rnd)
		{
			playerID[k] = static_cast<CHAR_ID>((rand() % 3) + 1);
		}
	}

	// ���ID�̎󂯓n��
	for (int k = 0; k < PLAYER_MAX; k++)
	{
		players[k] = playerID[k];
	}

	// �ްюQ���l��
	this->joinPlayerNum = joinPlayerNum;

	stageID = ID;

	Init();

	// ���ޯ��p------------------------------------
	//players[1] = ID_Bro;
	//players[2] = ID_Umb;
	//players[3] = ID_Lid;
	//this->joinPlayerNum = 4;
	// ���ޯ��p------------------------------------
}

GameScene::~GameScene()
{
}

unique_Base GameScene::Update(unique_Base own, const GameController & controller)
{
	gameSceneCnt++;
	// ���۰װ�������ޕϐ��Ɏ�������
	this->controller = controller;
	
	if (mode == MODE_SCENE_NEXT)
	{
		if (WhiteOut())
		{
			// ��ʑJ��
			return std::make_unique<ResultScene>(resultInfo, joinPlayerNum);
		}

		return std::move(own);	// ��݌p��
	}

	if (mode == MODE_SCENE_BACK)
	{	
		if (!ExtendEndF)
		{
			// ���ڰт̉摜�k���l
			ExtRate -= 0.01f;

			// �摜�k�����̕`��
			ClsDrawScreen();

			DrawGraph(0, 0, lpImageMng.GetID("image/�X�e�[�W�Z���N�g�p/StageSelect.png")[0], true);

			float posX = 0, posY = 0;
			posX = (float)SCREEN_SIZE_X * ExtRate / 2.0f;
			posY = SCREEN_SIZE_Y / 2;

			SetDrawMode(DX_DRAWMODE_BILINEAR);
			DrawRotaGraph((int)posX, (int)posY, ExtRate, 0, captureImage, true, false);
			SetDrawMode(DX_DRAWMODE_NEAREST);
			BufferChange();

			// �k�������������ۂ̏���
			if (ExtRate <= (1.0f / (float)EXT_RATE))
			{
				ExtRate = 1.0f / (float)EXT_RATE;
				ExtendEndF = true;

				captureImage = MakeScreen(SCREEN_SIZE_X, SCREEN_SIZE_Y);
				SetDrawScreen(captureImage);
				DrawGraph(0, 0, lpSceneMng.GetTmpBuffer(), true);
				SetDrawScreen(lpSceneMng.GetTmpBuffer());

				turnThePage = TurnThePage(VECTOR2(SCREEN_SIZE_X, SCREEN_SIZE_Y), VECTOR2(0, 0), captureImage, lpImageMng.GetID("image/�L�����Z���p/PageSerect.png")[0], 1.0f, true);
			}
		}
		else
		{
			// �摜�k�����I��莟��A�߰�ނ߂���
			ClsDrawScreen();
			if (turnThePage.Draw())
			{
				return std::make_unique<SelectScene>();
			}
			BufferChange();
		}

		return std::move(own);	// ��݌p��
	}

	switch (gameState)
	{
	case ENTRY:
		// ���ذ
		GameEntry();
		break;

	case TUTORIAL:
		// ����ر�
		Tutorial();
		break;

	case GRAYOUT_SLIDE:
		// ��ڰ��Ľײ��
		GrayOutSlide();
		break;

	case INSTRUCTION:
		// ��������̕\��
		Instruction();

	case COUNT_DOWN:
		// 321�̶���
		CountDown();
		break;

	case PLAY:
		// �ްђ��̏���
		GameFunc();
		break;

	case PAUSE:
		// �߰�ޒ��̏���
		PauseFunc();
		break;

	case CUT_IN:
		// ��Ĳݒ��̏���
		CutInFunc();
		break;

	case GAME_FINISH:
		//GamePlay(false);
		break;

	default:
		break;
	}

	// SE
	if (gameStartCnt == GAME_START_WAIT_TIME)
	{
		lpSoundMng.SetSound("�J");
	}
	if (gameCnt == 660)
	{
		if (!lpSoundMng.CheckPlaySound("�G���h�J�E���g"))
		{
			lpSoundMng.SetSound("�G���h�J�E���g");
		}
	}

	// �c��1�l�ȉ��ɂȂ���,����������ѱ��߂ŽðĕύX
	if (gameState == PLAY)
	{
		if (CheckGameEnd())
		{
			if (resSceneCnt == CHANG_SCENE_CNT)
			{
				// BGM�̒�~
				//lpSoundMng.StopSound("�o�g��1BGM");
				//lpSoundMng.StopSound("�o�g��2BGM");
				//lpSoundMng.StopSound("�o�g��3BGM");
				//lpSoundMng.StopSound("���S");

				//lpSoundMng.SetSound("�����܂�");
				//lpSoundMng.SetSound("�`���C��");
			}
			gameState = GAME_FINISH;
		}
	}

	// ػ��ļ�݂�
	if (gameState == GAME_FINISH)
	{
		resSceneCnt--;

		if (resSceneCnt <= 0)
		{
			mode = MODE_SCENE_NEXT;
		}
	}

	// �ްѼ�ݒ��̕`��
	if (gameState != GAME_FINISH)
	{
		GameDraw();
	}


	return std::move(own);	// ��݌p��
}

void GameScene::GamePlay(bool entryFinish)
{
	// ��ԍX�V
	for (auto floor : *floorList)
	{
		floor->Update();
	}

	// ��ԍX�V��ػ��ď��̊i�[
	for (auto &obj : *objList)
	{
		obj->UpDate(controller, objList, chickenList, entryFinish);
		this->resultInfo = obj->GetResultInfo();
	}
	for (auto &chicken : *chickenList)
	{
		chicken->UpDate(controller, chickenList);
	}
	for (auto &chicken : *chickenList)
	{
		if (chicken->CheckExpCnt())
		{
			// ��ʂ�h�炷
			int shake = 20;
			drawOffset = { (rand() % shake) - shake / 2, (rand() % shake) - shake / 2 };
			break;
		}
	}

	for (auto &obj : *objList)
	{
		if (!cutInInfo.flag)
		{
			cutInInfo = obj->CheckSpecialCutIn();
			if (cutInInfo.flag)
			{
				lpSoundMng.StopSound("�G���h�J�E���g");

				SetDrawBright(140, 140, 140);	// ��Ĳݒ��͉�ʂ��Â�����
				gameState = CUT_IN;
				break;
			}
		}
	}

	for (auto &exp : expOrbList)
	{
		exp->UpDate(objList);
	}

	// �����蔻��
	if (!gemeEndCnt)
	{
		for (auto &obj : *objList)
		{
			if (obj->GetAnim() != "���S")
			{
				obj->CheckHit(objList, expOrbList, entryFinish);
			}
		}
		for (auto &chicken : *chickenList)
		{
			chicken->CheckHit(objList);
		}
	}
	// ���q�֐�	����ި���
	for (auto itr = expOrbList.begin(); itr != expOrbList.end(); )
	{
		if ((*itr)->CheckArrive())
		{
			(*itr)->DeathPrc(objList);
			itr = expOrbList.erase(itr);
		}
		else
		{
			itr++;
		}
	}
	objList->remove_if([](uniqueObj&obj) {return obj->CheckDeath(); });
}

void GameScene::PlayerIns()
{
	if (gameState != ENTRY)
	{
		return;
	}

	for (int k = 0; k < PLAYER_MAX; k++)
	{
		if (players[k] == ID_NON) { continue; }
		// ���ޯ�ޗp�ɂ����Ă܂�
		 //if (controller.GetInputDown(PAD_INPUT_8, k) && !entryFlag[k])
		{
			entryFlag[k] = true;

			switch (players[k])
			{
			case ID_Umb:
				AddObjList()(objList, std::make_unique<Umbrella>(floorList, gameStartPos[k], k));
				break;

			case ID_Lid:
				AddObjList()(objList, std::make_unique<Lid>(floorList, gameStartPos[k], k));
				break;

			case ID_Bro:
				AddObjList()(objList, std::make_unique<Broom>(floorList, gameStartPos[k], k));
				break;

				//case ID_Wgun:
			case ID_NON:
			default:
				break;
			}

			lpSoundMng.SetSound("�G���g���[");
		}
	}
}

bool GameScene::CheckGameEnd(void)
{
	// ���l�������Ă��邩�𒲂ׂ�
	int lifeCnt = 0;		// ���l�����Ă��邩�̶����
	for (auto &obj : *objList)
	{
		if (!(obj->CheckDeath()))
		{
			lifeCnt++;		// �����Ă���ꍇ
		}
	}

	if ( (lifeCnt <= 1 || !gameCnt) && !gemeEndCnt)
	{
		gemeEndCnt = 240;

		// BGM�̒�~
		lpSoundMng.StopSound("�o�g��1BGM");
		lpSoundMng.StopSound("�o�g��2BGM");
		lpSoundMng.StopSound("�o�g��3BGM");
		lpSoundMng.StopSound("�G���h�J�E���g");

		if (!lpSoundMng.CheckPlaySound("�����܂�"))
		{
			lpSoundMng.SetSound("�����܂�");
		}
		if (!lpSoundMng.CheckPlaySound("�`���C��"))
		{
			lpSoundMng.SetSound("�`���C��");
		}

	}
	if (gemeEndCnt)
	{
		gemeEndCnt--;
	}

	// �ꎞ��~�������̉����̍Đ�
	if (!lpSoundMng.CheckPlaySound("�G���h�J�E���g"))
	{
		if (gameCnt <= 660 && gameCnt - 80 > 0)
		{
			lpSoundMng.UnPausingSound("�G���h�J�E���g", false);
		}
	}
	// �����l����1�l�ȉ��Ȃ�GameScene���I���
	if ((lifeCnt <= 1 || gameCnt <= 0) && !gemeEndCnt)
	{
		return true;
	}

	return false;
}

void GameScene::GameEntry(void)
{
	if (gameState != ENTRY)
	{
		// ���ذ���I�����Ă���Ƃ�
		return;
	}

	// ��ڲ԰�ݽ�ݽ
	PlayerIns();

	// �ް���ڲ
	GamePlay(false);

	for (int k = 0; k < PLAYER_MAX; k++)
	{
		if (players[k] == ID_NON) { continue; }
		if (!entryFlag[k])
		{
			return;	// ��l�ł���ذ���ĂȂ��Ƃ�
		}
	}
	// �S�����ذ���������Ă���Ȃ�����رق֐i��
	gameState = TUTORIAL;
}

void GameScene::Tutorial()
{
	if (gameState != TUTORIAL)
	{
		// ����رق��I�����Ă���ꍇ
		return;
	}


	// �ް���ڲ�ł��邪�����蔻��͂Ȃ�
	GamePlay(false);

	// �����֌W
	PreCnt++;
	if (PreCnt >= 3600 && !sleepFlag)
	{
		// SE�̍Đ�
		lpSoundMng.SetSound("������");
		sleepFlag = true;
		PreCnt = 0;
	}
	// 5�b���Ɋm�F�pSE
	if (!sleepFlag)
	{
		if (PreCnt % 600 == 0)
		{
			lpSoundMng.SetSound("�m�F");
		}
	}
	else
	{
		if (PreCnt % 600 == 0)
		{
			if (!lpSoundMng.CheckPlaySound("������"))
			{
				if (!chengFlag)
				{
					lpSoundMng.SetSound("�ނɂ�");
					chengFlag = !chengFlag;
				}
				else
				{
					lpSoundMng.SetSound("�Q��");
					chengFlag = !chengFlag;
				}
			}
		}
	}

	for (int j = 0; j < PLAYER_MAX; j++)
	{
		// B���݂������Ƒ���������o��
		if (controller.GetInputDown(GPAD_INPUT_B, j))
		{
			if (showInstruction && j == pausePadNum)
			{
				showInstruction = false;
				// SE
				lpSoundMng.SetSound("��ݾ�SE");
			}
			else
			{
				if (!showInstruction)
				{
					showInstruction = true;
					pausePadNum = j;
					// SE
					lpSoundMng.SetSound("����SE");
				}
			}
		}
	}


	if (showInstruction)
	{
		// ���������ʂ�\�����Ă��鎞�͹ްѽ��Ăł��Ȃ�
		return;
	}


	for (int j = 0; j < PLAYER_MAX; j++)
	{
		// �������݂������ƶ����޳݂��n�܂�
		if (controller.GetInputDown(PAD_INPUT_8, j))
		{
			// BGM�̒�~
			lpSoundMng.StopSound("�ڸ�BGM");
			lpSoundMng.StopSound("�m�F");
			lpSoundMng.StopSound("������");
			lpSoundMng.StopSound("�ނɂ�");
			lpSoundMng.StopSound("�Q��");

			//// ����т�BGM�̍Đ�
			//int battleBGM = (rand() % 3);
			//switch (battleBGM)
			//{
			//case 0:
			//	lpSoundMng.SetSound("�o�g��1BGM", true);	//ٰ��
			//	break;

			//case 1:
			//	lpSoundMng.SetSound("�o�g��2BGM", true);	//ٰ��
			//	break;

			//case 2:
			//	lpSoundMng.SetSound("�o�g��3BGM", true);	//ٰ��
			//	break;

			//default:
			//	break;
			//}
			switch (stageID)
			{
			case STAGE_SCHOOL:
				lpSoundMng.SetSound("�o�g��1BGM", true);	//ٰ��
				break;

			case STAGE_TREE_OF_LEGEND:
				lpSoundMng.SetSound("�o�g��3BGM", true);	//ٰ��
				break;

			case STAGE_ARCADE:
				lpSoundMng.SetSound("�o�g��2BGM", true);	//ٰ��
				break;

			default:
				break;
			}
			// SE
			lpSoundMng.SetSound("�҂�ۂ�");
			PreCnt = 0;

			gameState = GRAYOUT_SLIDE;
		}
	}
}

void GameScene::GrayOutSlide(void)
{
	if (gameState != GRAYOUT_SLIDE)
	{
		return;
	}

	// �ް���ڲ�ł��邪�����蔻��͂Ȃ�
	GamePlay(false);

	grayOutCnt += G_OUT_SPEED;

	if (grayOutCnt >= (560 + G_OUT_END_WAIT))
	{
		gameState = COUNT_DOWN;
		lpSoundMng.SetSound("�X�^�[�g�J�E���g");
	}

	if (grayOutCnt == 0)
	{
		lpSoundMng.SetSound("�T�C����");
	}
}

void GameScene::Instruction(void)
{
	if (gameState != INSTRUCTION)
	{
		return;
	}

	// B���݂��������߰�މ�ʂɖ߂�
	if (controller.GetInputDown(GPAD_INPUT_B, pausePadNum))
	{
		gameState = PAUSE;
		// SE
		lpSoundMng.SetSound("��ݾ�SE");
	}
}

void GameScene::CountDown(void)
{
	if (gameState != COUNT_DOWN)
	{
		// �����޳݂��I��������Ƃ͏������Ȃ�
		return;
	}

	// �ް���ڲ�ł��邪�����蔻��͂Ȃ�
	if (!reStartFlag)
	{
		GamePlay(false);
	}

	gameStartCnt++;					// 3�E2�E1�̕\��

	if (gameStartCnt > GAME_START_WAIT_TIME)
	{
		gameState = PLAY;
		gameStartCnt = 0;
		reStartFlag = false;
	}
}

void GameScene::GameFunc()
{
	if (gameState != PLAY)
	{
		return;		// �߰��Ӱ�ޒ��͏������Ȃ�
	}

	// �ް���ڲ�A�����蔻�肠��gameCnt
	GamePlay(true);

	gameCnt--;		// �ްђ��̎c�莞�Ԃ����炷

	for (int k = 0; k < PLAYER_MAX; k++)
	{
		if (players[k] == ID_NON) { continue; }

		// �߰��Ӱ�ސ؂�ւ�
		if (!gemeEndCnt)
		{
			if (controller.GetInputDown(PAD_INPUT_8, k))
			{
				// SE�̍Đ�
				lpSoundMng.SetSound("�|�[�Y");
				lpSoundMng.StopSound("�X�^�[�g�J�E���g");
				lpSoundMng.StopSound("�G���h�J�E���g");

				pausePadNum = k;
				gameState = PAUSE;
				break;
			}
		}
		//// B���݂������Ƒ���������o��
		//if (controller.GetInputDown(GPAD_INPUT_B, pausePadNum))
		//{
		//	showInstruction = !showInstruction;
		//	break;
		//}
	}
}

void GameScene::PauseFunc(void)
{
	if (gameState != PAUSE)
	{
		return;
	}

	// �ްѼ�݁E��׾ڼ�݁Eػ��ļ�݁E��������̶��ّI��

	if (controller.GetInputDown(PAD_INPUT_UP, pausePadNum))
	{
		// SE
		lpSoundMng.SetSound("����SE");

		changeSnece = (changeSnece > CHANGE_SCENE_NON ? (CHANGE_SCENE)(changeSnece - 1) : CHANGE_SCENE_INSTRUCTION);
	}
	if (controller.GetInputDown(PAD_INPUT_DOWN, pausePadNum))
	{
		// SE
		lpSoundMng.SetSound("����SE");

		changeSnece = (changeSnece < CHANGE_SCENE_INSTRUCTION ? (CHANGE_SCENE)(changeSnece + 1) : CHANGE_SCENE_NON);
	}

	// B���݂Źްтɖ߂�
	if (controller.GetInputDown(GPAD_INPUT_B, pausePadNum)||controller.GetInputDown(PAD_INPUT_8, pausePadNum))
	{
		gameState = COUNT_DOWN;
		reStartFlag = true;		// 321�̶����޳ݒ��ͷ�ב���s��
		// SE
		lpSoundMng.SetSound("��ݾ�SE");
		lpSoundMng.SetSound("�X�^�[�g�J�E���g");	// 321��SE�Đ�

		// �����߰�ނ����Ƃ��ɶ��ق���ԏ�ɗ���悤������
		changeSnece = CHANGE_SCENE_NON;
	}
	// A�{�^���ż�ݑJ��
	if (controller.GetInputDown(GPAD_INPUT_A, pausePadNum))
	{
		switch (changeSnece)
		{
		case CHANGE_SCENE_INSTRUCTION:
			// SE
			lpSoundMng.SetSound("����SE");
			gameState = INSTRUCTION;
			break;

		case CHANGE_SCENE_CHAR_SEL:

			// BGM�̒�~
			lpSoundMng.StopSound("�o�g��1BGM");
			lpSoundMng.StopSound("�o�g��2BGM");
			lpSoundMng.StopSound("�o�g��3BGM");
			lpSoundMng.StopSound("���S");
			// SE
			lpSoundMng.SetSound("����SE");

			captureImage = MakeScreen(SCREEN_SIZE_X, SCREEN_SIZE_Y);
			SetDrawScreen(captureImage);
			DrawGraph(0, 0, lpSceneMng.GetTmpBuffer(), true);
			SetDrawScreen(lpSceneMng.GetTmpBuffer());

			mode = MODE_SCENE_BACK;
			break;

		case CHANGE_SCENE_RESULT:

			// BGM�̒�~
			lpSoundMng.StopSound("�o�g��1BGM");
			lpSoundMng.StopSound("�o�g��2BGM");
			lpSoundMng.StopSound("�o�g��3BGM");
			lpSoundMng.StopSound("���S");
			// SE
			lpSoundMng.SetSound("����SE");

			mode = MODE_SCENE_NEXT;
			break;

		case CHANGE_SCENE_NON:
			gameState = COUNT_DOWN;
			reStartFlag = true;		// 321�̶����޳ݒ��ͷ�ב���s��

			// SE
			lpSoundMng.SetSound("��ݾ�SE");
			lpSoundMng.SetSound("�X�^�[�g�J�E���g");	// 321��SE�Đ�

			break;

		case CHANGE_SCENE_MAX:
			// �װ
			break;
		default:
			break;
		}
	}
}

void GameScene::CutInFunc(void)
{
	if (gameState != CUT_IN)
	{
		return;
	}

	// ��ʂ��Â�����
	SetDrawBright(140, 140, 140);

	// ��Ĳݒ��̏���
	cutInInfo.cnt++;
	if (cutInInfo.cnt >= CUT_IN_TIME)
	{
		if (gameCnt <= 660 && gameCnt - 80 > 0)
		{
			lpSoundMng.UnPausingSound("�G���h�J�E���g", false);
		}
		cutInInfo.flag = false;
		SetDrawBright(255, 255, 255);
		gameState = PLAY;
	}
}

bool GameScene::GameDraw()
{
	ClsDrawScreen();

	// �_�̍��W�X�V
	auto checkCloudePos = [=](VECTOR2& pos, int speed) {
		pos.x += (gameSceneCnt % 2) * speed;
		if (pos.x >= SCREEN_SIZE_X)
		{
			pos.x = -SCREEN_SIZE_X;
		}
	};

	checkCloudePos(cloudBackPos1, 1);
	checkCloudePos(cloudBackPos2, 1);
	checkCloudePos(cloudFrontPos1, 2);
	checkCloudePos(cloudFrontPos2, 2);

	// �f�G�Ȕw�i
	DrawGraph(0, 0, lpImageMng.GetID("image/�Q�[���V�[���p/back.png")[0], true);

	DrawGraph(cloudBackPos1, lpImageMng.GetID("image/�Q�[���V�[���p/�_1.png")[0], true);
	DrawGraph(cloudBackPos2, lpImageMng.GetID("image/�Q�[���V�[���p/�_1.png")[0], true);
	DrawGraph(cloudFrontPos1, lpImageMng.GetID("image/�Q�[���V�[���p/�_2.png")[0], true);
	DrawGraph(cloudFrontPos2, lpImageMng.GetID("image/�Q�[���V�[���p/�_2.png")[0], true);

	DrawGraph(0, 0, lpImageMng.GetID("image/�Q�[���V�[���p/" + imageName[imageID] + ".png")[0], true);

	// ��ނ̕`��
	for (auto floor : *floorList)
	{
		floor->Draw();
	}

	// ����رْ��̑�������̕`��
	if (gameState == TUTORIAL)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
		DrawBox(VECTOR2(0, 0), VECTOR2(1280, 560), 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

		// ���������ʂ�\�����Ă��鎞
		if (showInstruction)
		{
			DrawGraph(0, 0, lpImageMng.GetID("image/�`���[�g���A���p/�������.png")[0], true);
			if (gameSceneCnt / 60 % 2)
			{
				DrawStringToHandle(1050, 20, "�a�łƂ���", 0xffffff, lpFontMng.SetFont("���ʂ�40��"));
			}
		}
		// ���������ʂ�\�����Ă��Ȃ���
		else
		{
			if (gameSceneCnt / 60 % 2)
			{
				// ����������荶�����ɔz�u
				StageName = ("�X�^�[�g�{�^���ł͂��߂�");
				len = static_cast<int>(strlen("�X�^�[�g�{�^���ł͂��߂�"));

				DrawStringToHandle(
					(SCREEN_SIZE_X - GetDrawStringWidthToHandle(StageName.c_str(), len, lpFontMng.SetFont("���ʂ�40��"))) / 2,
					200, "�X�^�[�g�{�^���ł͂��߂�", 0xffffff, lpFontMng.SetFont("���ʂ�40��"));

				DrawStringToHandle(980, 50, "�a�ő������", 0xffffff, lpFontMng.SetFont("���ʂ�40��"));
			}
			// ����������荶�����ɔz�u
			StageName = ("�`���[�g���A��");
			len = static_cast<int>(strlen("�`���[�g���A��"));

			DrawStringToHandle(
				(SCREEN_SIZE_X - GetDrawStringWidthToHandle(StageName.c_str(), len, lpFontMng.SetFont("���ʂ�80��"))) / 2,
				70, "�`���[�g���A��", 0xff0000, lpFontMng.SetFont("���ʂ�80��"));

		}
	}

	// ��ڰ��Ľײ��
	if (gameState == GRAYOUT_SLIDE)
	{
		int cnt = (grayOutCnt <= 0) ? 0 : grayOutCnt;
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
		DrawBox(VECTOR2(0, 0), VECTOR2(1280, 560 - cnt), 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}

	// Obj.cpp�̕`��
	for (auto &data : *objList)
	{
		bool moveStopFlag = (gameState == PAUSE || gameState == INSTRUCTION);
		(*data).Draw(moveStopFlag, cutInInfo);
	}

	// �ް�ދʂ̕`��
	for (auto &orb : expOrbList)
	{
		orb->Draw();
	}

	for (auto &chicken : *chickenList)
	{
		chicken->Draw();
	}

	// �E�Q�{���̕`��
	int t_c_KillCnt = 0;
	for (auto chickenKillCnt : resultInfo.chickenKillCnt)
	{
		t_c_KillCnt += chickenKillCnt;
	}
	if (t_c_KillCnt)
	{
		int drawLengh;
		drawLengh = GetDrawFormatStringWidthToHandle(lpFontMng.SetFont("���ʂ�50��"), "�~ %d �H", t_c_KillCnt);
		DrawGraph(SCREEN_SIZE_X - 72 - drawLengh, 10, lpImageMng.GetID("image/�Q�[���V�[���p/Dead.png")[0], true);
		DrawFormatStringToHandle(SCREEN_SIZE_X - drawLengh, 72 / 2 - 5, 0xff0000, lpFontMng.SetFont("���ʂ�50��"), "�~ %d �H", t_c_KillCnt, true);
	}

	// �Q�����݂������Ă˓I�ȕ`��
	if (gameState == ENTRY)
	{
		for (int k = 0; k < PLAYER_MAX; k++)
		{
			if (players[k] == ID_NON) { continue; }
			if (!entryFlag[k] && players[k] != ID_NON)
			{
				if ((gameSceneCnt / 30) % 2)
				{
					DrawStringToHandle(30 + 320 * k, 620, "START�ŎQ��", 0xff0000, lpFontMng.SetFont("���ʂ�40��"));
				}
			}
		}
	}

	// �����޳ݒ��̕`��
	if (gameState == COUNT_DOWN)
	{
		// 3�E2�E1�̕`��
		if (gameStartCnt < GAME_START_WAIT_TIME)
		{
			DrawFormatStringToHandle(565, 100, 0xff0000, lpFontMng.SetFont("���ʂ�100��"), "%d�b", 3 - (gameStartCnt / 60), true);
		}

		// �c�莞�Ԃ̕`��(�����ł͌��Z���Ȃ�)
		DrawFormatStringToHandle(0, 0, 0xff0000, lpFontMng.SetFont("���ʂ�50��"), "�c��%d�b", gameCnt / 60, true);
	}

	// �ްђ��̕`��
	if (gameState == PLAY)
	{
		if ((TIME_LIM * 60 - gameCnt) < 60)
		{
			// ����������荶�����ɔz�u
			StageName = ("�����[�ƁI");
			len = static_cast<int>(strlen("�����[�ƁI"));

			DrawStringToHandle(
				(SCREEN_SIZE_X - GetDrawStringWidthToHandle(StageName.c_str(), len, lpFontMng.SetFont("���ʂ�100��"))) / 2,
				100, "�����[�ƁI", 0xff0000, lpFontMng.SetFont("���ʂ�100��"));
		}

		if (!gemeEndCnt)
		{
			DrawFormatStringToHandle(0, 0, 0xff0000, lpFontMng.SetFont("���ʂ�50��"), "�c��%d�b", gameCnt / 60, true);

			if (gameCnt <= 660)
			{
				DrawFormatStringToHandle(565, 100, 0xff0000, lpFontMng.SetFont("���ʂ�100��"), "%d�b", gameCnt / 60, true);
			}
		}

	}

	if (gemeEndCnt)
	{
		DrawStringToHandle(430, 100, "�����܂ŁI", 0xff0000, lpFontMng.SetFont("���ʂ�100��"));
	}

	// �߰��Ӱ�ޒ��̕`��
	if (gameState == PAUSE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
		DrawBox(VECTOR2(0, 0), VECTOR2(1280, 720), 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

		// ����������荶�����ɔz�u
		StageName = ("�|�[�Y");
		len = static_cast<int>( strlen("�|�[�Y") );

		DrawStringToHandle(
			(SCREEN_SIZE_X - GetDrawStringWidthToHandle(StageName.c_str(), len, lpFontMng.SetFont("���ʂ�100��"))) / 2,
			150, "�|�[�Y", 0xffffff, lpFontMng.SetFont("���ʂ�100��"));

		DrawStringToHandle(450, 310, "�Q�[����ʂ֖߂�",		0xffffff, lpFontMng.SetFont("���ʂ�40��"));
		DrawStringToHandle(450, 390, "�L�����N�^�[�I����ʂ�",	0xffffff, lpFontMng.SetFont("���ʂ�40��"));
		DrawStringToHandle(450, 470, "���U���g��ʂ�",			0xffffff, lpFontMng.SetFont("���ʂ�40��"));
		DrawStringToHandle(450, 550, "���������\��",			0xffffff, lpFontMng.SetFont("���ʂ�40��"));

		DrawGraph(400, 310 + 80 * changeSnece, lpImageMng.GetID("image/�Q�[���V�[���p/Abutton.png")[0], true);

		if (gameSceneCnt / 60 % 2)
		{
			DrawStringToHandle(1000, 20, "�`:�����Ă�",			0xffffff, lpFontMng.SetFont("���ʂ�40��"));
			DrawStringToHandle(786,	 70, "�X�^�[�g���a:�Ƃ���", 0xffffff, lpFontMng.SetFont("���ʂ�40��"));
		}
	}

	// ��������̕`��
	if (gameState == INSTRUCTION)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
		DrawBox(VECTOR2(0, 0), VECTOR2(1280, 720), 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

		DrawGraph(0, 0, lpImageMng.GetID("image/�`���[�g���A���p/�������.png")[0], true);
		if (gameSceneCnt / 60 % 2)
		{
			DrawStringToHandle(1000, 20, "�a:�Ƃ���", 0xffffff, lpFontMng.SetFont("���ʂ�40��"));
		}
	}

	// �߰�މ�� || ������� �� �J���Ă�����ڲ԰�ԍ��̕\��
	if ((gameState == INSTRUCTION) || (gameState == PAUSE) || ((gameState == TUTORIAL) && showInstruction))
	{
		VECTOR2 graphSize;
		GetGraphSize(lpImageMng.GetID("image/UI/1234.png")[pausePadNum], &graphSize.x, &graphSize.y);

		DrawRotaGraph(415 - graphSize.x/2, graphSize.y/2 + 320 + 80 * changeSnece, 1.0, 0.0,
			lpImageMng.GetID("image/UI/1234.png")[pausePadNum],true);
	}

	// ��Ĳݒ��̕`��
	if (gameState == CUT_IN)
	{
		SetDrawBright(255, 255, 255);	// ��ʂ̖��邳�����Ƃɖ߂�

		if (cutInInfo.cnt < CUT_IN_TIME / 2)
		{
			int animCnt = cutInInfo.cnt / 6;	// ++������[ 
			if (animCnt < 5 * 4)
			{
				DrawRectGraph(
					270, 0,
					720 * (animCnt % 5), 720 * (animCnt / 5 % 4),
					720, 720,
					lpImageMng.GetID("image/UI/���̑�11.png")[0], true, false);
			}
		}
		else
		{
			int animCnt = (cutInInfo.cnt - CUT_IN_TIME / 2) / 4;	// ++������[ 
			if (animCnt < 5 * 4)
			{
				DrawRectGraph(
					cutInInfo.pos.x - 150 / 2, cutInInfo.pos.y - 150 / 2,
					150 * (animCnt % 5), 150 * (animCnt / 5 % 4),
					150, 150,
					lpImageMng.GetID("image/UI/���̑�3.png")[0], true, false);
			}
		}
	}

	BufferChange();

	return true;
}

bool GameScene::WhiteOut(void)
{
	if (!lastGameScreen)
	{
		lastGameScreen = MakeScreen(SCREEN_SIZE_X, SCREEN_SIZE_Y);	// �ٽ�ذݻ��ނ��ޯ̧gameScreen���쐬
		SetDrawScreen(lastGameScreen);								// �`���ޯ̧��gameScreen�ɐݒ�
		DrawGraph(0, 0, lpSceneMng.GetTmpBuffer(), true);			// gameScreen�ɕ`��
		SetDrawScreen(lpSceneMng.GetTmpBuffer());
	}

	ClsDrawScreen();

	// �ܲı��
	DrawGraph(0, 0, lastGameScreen, true);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(alpha));
	DrawBox(VECTOR2(0, 0), lpSceneMng.GetScreenSize(), 0xffffff, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	alpha.update();

	if (alpha >= 255)
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

void GameScene::PageInstance(void)
{
	lastGameScreen = MakeScreen(SCREEN_SIZE_X, SCREEN_SIZE_Y);	// �ٽ�ذݻ��ނ��ޯ̧gameScreen���쐬
	SetDrawScreen(lastGameScreen);								// �`���ޯ̧��gameScreen�ɐݒ�
	DrawGraph(0, 0, lpSceneMng.GetTmpBuffer(), true);			// gameScreen�ɕ`��

	int resultScreen = MakeScreen(SCREEN_SIZE_X, SCREEN_SIZE_Y);
	SetDrawScreen(resultScreen);
	DrawGraph(0, 0, lpImageMng.GetID("image/���U���g�p/result.png")[0], true);
	DrawGraph(292, 170, lpImageMng.GetID("image/���U���g�p/�W���p�j�J128.png")[0], true);
	SetDrawScreen(lpSceneMng.GetTmpBuffer());

	turnThePage = TurnThePage(VECTOR2(SCREEN_SIZE_X, SCREEN_SIZE_Y), VECTOR2(0, 0), lastGameScreen, resultScreen, 1.0f);
}

void GameScene::BufferChange(void)
{
	SetDrawScreen(DX_SCREEN_BACK);
	ClsDrawScreen();
	DrawGraph(drawOffset, lpSceneMng.GetTmpBuffer(), true);	// �����ŉ�ʌ��ʂ�ǉ��ł���
	SetDrawScreen(lpSceneMng.GetTmpBuffer());
	drawOffset = {};

	ScreenFlip();
}

int GameScene::Init(void)
{
	if (!objList)
	{
		objList = std::make_shared<uniqueObjList>();
	}
	objList->clear();

	if (!floorList)
	{
		floorList = std::make_shared<sharedFloorList>();
	}
	floorList->clear();

	if (!chickenList)
	{
		chickenList = std::make_shared<uniqueChickenList>();
	}
	chickenList->clear();

	if (stageID == STAGE_RANDUM)
	{
		stageID = static_cast<STAGE_ID>((rand() % (static_cast<int>(STAGE_MAX) - 1)));
	}

	switch (stageID)
	{
	case STAGE_SCHOOL:
		AddFloorList()(floorList, std::make_shared<FloorNormal>(VECTOR2(-20, 360), VECTOR2(500, 5)));
		AddFloorList()(floorList, std::make_shared<FloorNormal>(VECTOR2(800, 360), VECTOR2(500, 5)));
		lpSoundMng.SoundInit("sound/Battle.mp3", "�o�g��1BGM", 90);
		break;

	case STAGE_TREE_OF_LEGEND:
		lpSoundMng.SoundInit("sound/Battle3.mp3", "�o�g��3BGM", 90);
		break;

	case STAGE_ARCADE:
		AddFloorList()(floorList, std::make_shared<FloorCar>());
		lpSoundMng.SoundInit("sound/Battle2.mp3", "�o�g��2BGM");
		break;

	default:
		break;
	}

	imageID = static_cast<IMAGE_ID>(stageID);

	gameStartPos = { VECTOR2(400,0),VECTOR2(880,0), VECTOR2(300,0), VECTOR2(980,0) };

	imageName = {
		"School"	,
		"TreeOfLegend",
		"Arcade"
	};

	gameState = ENTRY;

	pausePadNum = 0;

	entryFlag = {};

	drawOffset = {};

	gameCnt = TIME_LIM * 60;

	gameStartCnt = 0;

	grayOutCnt = -G_OUT_START_WAIT;

	gemeEndCnt = 0;

	changeSnece = CHANGE_SCENE_NON;	// �߰��Ӱ���ƭ������ݒ�

	cutInInfo = {};


	cloudFrontPos1	= {};
	cloudFrontPos2	= VECTOR2(-lpSceneMng.GetScreenSize().x,0);
	cloudBackPos1	= {};
	cloudBackPos2	= VECTOR2(-lpSceneMng.GetScreenSize().x,0);

	showInstruction = false;

	resSceneCnt = CHANG_SCENE_CNT;

	reStartFlag = false;
	lastGameScreen = 0;
	mode = MODE_SCENE_NOW;
	ExtRate = 1.0f;
	ExtendEndF = false;

	alpha = 0.0;					// �����x��ݒ�(DrawBox�̔��ɑ΂��Ă̓����x)
				
	alpha.set(255, FADE_SPEED);		// �����x��FADE_SPEED(�ڰ�)�����čő�(255)�ɂ���

	// BGM�SE�̓o�^
	//lpSoundMng.SoundInit("sound/Battle.mp3", "�o�g��1BGM",90);
	//lpSoundMng.SoundInit("sound/Battle2.mp3", "�o�g��2BGM");
	//lpSoundMng.SoundInit("sound/Battle3.mp3", "�o�g��3BGM", 90);

	lpSoundMng.SoundInit("sound/SE��/StartCount.mp3", "�X�^�[�g�J�E���g");
	lpSoundMng.SoundInit("sound/SE��/BatteleStart.mp3", "�J");
	lpSoundMng.SoundInit("sound/SE��/EndCount.mp3", "�G���h�J�E���g");
	lpSoundMng.SoundInit("sound/SE��/Pause.mp3", "�|�[�Y");
	lpSoundMng.SoundInit("sound/SE��/Preparation.mp3", "�m�F");

	lpSoundMng.SoundInit("sound/SE��/Sleep1.mp3", "������");
	lpSoundMng.SoundInit("sound/SE��/Sleep2.mp3", "�ނɂ�");
	lpSoundMng.SoundInit("sound/SE��/Sleep3.mp3", "�Q��");

	lpSoundMng.SoundInit("sound/SE��/�`���C��.mp3", "�`���C��");
	lpSoundMng.SoundInit("sound/SE��/�����܂�.mp3", "�����܂�");

	lpSoundMng.SoundInit("sound/SE��/�T�C����.mp3", "�T�C����");
	lpSoundMng.SoundInit("sound/SE��/PlayerIns.mp3", "�G���g���[");
	lpSoundMng.SoundInit("sound/SE��/�҂�ۂ�.mp3", "�҂�ۂ�");

	// SE�p
	PreCnt = 0;
	sleepFlag = false;
	chengFlag = false;

	return 0;
}