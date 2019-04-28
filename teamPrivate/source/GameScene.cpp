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

#define TIME_LIM				(180)	// 時間制限(秒)

#define GAME_START_WAIT_TIME	(180)	// 321の表示用
#define START_CHAR_TIME			(60)	// 60ﾌﾚｰﾑの間「すたーと！」の表示をする

#define CUT_IN_TIME				(180)

#define CHANG_SCENE_CNT			(60)

#define PAGE_SIZE_X				1280
#define PAGE_SIZE_Y				720

#define G_OUT_SPEED				7					// ｸﾞﾚｰｱｳﾄｽﾗｲﾄﾞの速さ
#define G_OUT_START_WAIT		(60 * G_OUT_SPEED)	// ｽﾀｰﾄﾎﾞﾀﾝ押下からｸﾞﾚｰｱｳﾄｽﾗｲﾄﾞが始まるまでの待ち時間
#define G_OUT_END_WAIT			(40 * G_OUT_SPEED)	// ｸﾞﾚｰｱｳﾄｽﾗｲﾄﾞが終り、321が始まるまでの待ち時間

#define FADE_SPEED				180		// ﾌｪｰﾄﾞｱｳﾄが完了するまでの時間(ﾌﾚｰﾑ)

// 仮です
#define EXT_RATE				2.1f		// ｽﾃｰｼﾞｾﾚｸﾄの左画像がﾌﾙｻｲｽﾞの何分の1なのか

GameScene::GameScene()
{
	Init();
}

GameScene::GameScene(CHAR_ID playerID[PLAYER_MAX], int joinPlayerNum, STAGE_ID ID)
{
	// static resultInfoの初期化
	Obj::ResultInfoInit();

	// ﾗﾝﾀﾞﾑｷｬﾗを選択した場合の処理
	for (int k = 0; k < PLAYER_MAX; k++)
	{
		if (playerID[k] == ID_Rnd)
		{
			playerID[k] = static_cast<CHAR_ID>((rand() % 3) + 1);
		}
	}

	// ｷｬﾗIDの受け渡し
	for (int k = 0; k < PLAYER_MAX; k++)
	{
		players[k] = playerID[k];
	}

	// ｹﾞｰﾑ参加人数
	this->joinPlayerNum = joinPlayerNum;

	stageID = ID;

	Init();

	// ﾃﾞﾊﾞｯｸ用------------------------------------
	//players[1] = ID_Bro;
	//players[2] = ID_Umb;
	//players[3] = ID_Lid;
	//this->joinPlayerNum = 4;
	// ﾃﾞﾊﾞｯｸ用------------------------------------
}

GameScene::~GameScene()
{
}

unique_Base GameScene::Update(unique_Base own, const GameController & controller)
{
	gameSceneCnt++;
	// ｺﾝﾄﾛｰﾗｰ情報をﾒﾝﾊﾞ変数に持たせる
	this->controller = controller;
	
	if (mode == MODE_SCENE_NEXT)
	{
		if (WhiteOut())
		{
			// 画面遷移
			return std::make_unique<ResultScene>(resultInfo, joinPlayerNum);
		}

		return std::move(own);	// ｼｰﾝ継続
	}

	if (mode == MODE_SCENE_BACK)
	{	
		if (!ExtendEndF)
		{
			// 毎ﾌﾚｰﾑの画像縮小値
			ExtRate -= 0.01f;

			// 画像縮小中の描画
			ClsDrawScreen();

			DrawGraph(0, 0, lpImageMng.GetID("image/ステージセレクト用/StageSelect.png")[0], true);

			float posX = 0, posY = 0;
			posX = (float)SCREEN_SIZE_X * ExtRate / 2.0f;
			posY = SCREEN_SIZE_Y / 2;

			SetDrawMode(DX_DRAWMODE_BILINEAR);
			DrawRotaGraph((int)posX, (int)posY, ExtRate, 0, captureImage, true, false);
			SetDrawMode(DX_DRAWMODE_NEAREST);
			BufferChange();

			// 縮小が完了した際の処理
			if (ExtRate <= (1.0f / (float)EXT_RATE))
			{
				ExtRate = 1.0f / (float)EXT_RATE;
				ExtendEndF = true;

				captureImage = MakeScreen(SCREEN_SIZE_X, SCREEN_SIZE_Y);
				SetDrawScreen(captureImage);
				DrawGraph(0, 0, lpSceneMng.GetTmpBuffer(), true);
				SetDrawScreen(lpSceneMng.GetTmpBuffer());

				turnThePage = TurnThePage(VECTOR2(SCREEN_SIZE_X, SCREEN_SIZE_Y), VECTOR2(0, 0), captureImage, lpImageMng.GetID("image/キャラセレ用/PageSerect.png")[0], 1.0f, true);
			}
		}
		else
		{
			// 画像縮小が終わり次第、ﾍﾟｰｼﾞめくり
			ClsDrawScreen();
			if (turnThePage.Draw())
			{
				return std::make_unique<SelectScene>();
			}
			BufferChange();
		}

		return std::move(own);	// ｼｰﾝ継続
	}

	switch (gameState)
	{
	case ENTRY:
		// ｴﾝﾄﾘｰ
		GameEntry();
		break;

	case TUTORIAL:
		// ﾁｭｰﾄﾘｱﾙ
		Tutorial();
		break;

	case GRAYOUT_SLIDE:
		// ｸﾞﾚｰｱｳﾄｽﾗｲﾄﾞ
		GrayOutSlide();
		break;

	case INSTRUCTION:
		// 操作説明の表示
		Instruction();

	case COUNT_DOWN:
		// 321のｶｳﾝﾄ
		CountDown();
		break;

	case PLAY:
		// ｹﾞｰﾑ中の処理
		GameFunc();
		break;

	case PAUSE:
		// ﾎﾟｰｽﾞ中の処理
		PauseFunc();
		break;

	case CUT_IN:
		// ｶｯﾄｲﾝ中の処理
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
		lpSoundMng.SetSound("笛");
	}
	if (gameCnt == 660)
	{
		if (!lpSoundMng.CheckPlaySound("エンドカウント"))
		{
			lpSoundMng.SetSound("エンドカウント");
		}
	}

	// 残り1人以下になった,もしくはﾀｲﾑｱｯﾌﾟでｽﾃｰﾄ変更
	if (gameState == PLAY)
	{
		if (CheckGameEnd())
		{
			if (resSceneCnt == CHANG_SCENE_CNT)
			{
				// BGMの停止
				//lpSoundMng.StopSound("バトル1BGM");
				//lpSoundMng.StopSound("バトル2BGM");
				//lpSoundMng.StopSound("バトル3BGM");
				//lpSoundMng.StopSound("死亡");

				//lpSoundMng.SetSound("そこまで");
				//lpSoundMng.SetSound("チャイム");
			}
			gameState = GAME_FINISH;
		}
	}

	// ﾘｻﾞﾙﾄｼｰﾝへ
	if (gameState == GAME_FINISH)
	{
		resSceneCnt--;

		if (resSceneCnt <= 0)
		{
			mode = MODE_SCENE_NEXT;
		}
	}

	// ｹﾞｰﾑｼｰﾝ中の描画
	if (gameState != GAME_FINISH)
	{
		GameDraw();
	}


	return std::move(own);	// ｼｰﾝ継続
}

void GameScene::GamePlay(bool entryFinish)
{
	// 状態更新
	for (auto floor : *floorList)
	{
		floor->Update();
	}

	// 状態更新とﾘｻﾞﾙﾄ情報の格納
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
			// 画面を揺らす
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
				lpSoundMng.StopSound("エンドカウント");

				SetDrawBright(140, 140, 140);	// ｶｯﾄｲﾝ中は画面を暗くする
				gameState = CUT_IN;
				break;
			}
		}
	}

	for (auto &exp : expOrbList)
	{
		exp->UpDate(objList);
	}

	// 当たり判定
	if (!gemeEndCnt)
	{
		for (auto &obj : *objList)
		{
			if (obj->GetAnim() != "死亡")
			{
				obj->CheckHit(objList, expOrbList, entryFinish);
			}
		}
		for (auto &chicken : *chickenList)
		{
			chicken->CheckHit(objList);
		}
	}
	// 叙述関数	ﾌﾟﾚﾃﾞｨｹｰﾄ
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
		// ﾃﾞﾊﾞｯｸﾞ用にけしてます
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

			lpSoundMng.SetSound("エントリー");
		}
	}
}

bool GameScene::CheckGameEnd(void)
{
	// 何人生存しているかを調べる
	int lifeCnt = 0;		// 何人生きているかのｶｳﾝﾀｰ
	for (auto &obj : *objList)
	{
		if (!(obj->CheckDeath()))
		{
			lifeCnt++;		// 生きている場合
		}
	}

	if ( (lifeCnt <= 1 || !gameCnt) && !gemeEndCnt)
	{
		gemeEndCnt = 240;

		// BGMの停止
		lpSoundMng.StopSound("バトル1BGM");
		lpSoundMng.StopSound("バトル2BGM");
		lpSoundMng.StopSound("バトル3BGM");
		lpSoundMng.StopSound("エンドカウント");

		if (!lpSoundMng.CheckPlaySound("そこまで"))
		{
			lpSoundMng.SetSound("そこまで");
		}
		if (!lpSoundMng.CheckPlaySound("チャイム"))
		{
			lpSoundMng.SetSound("チャイム");
		}

	}
	if (gemeEndCnt)
	{
		gemeEndCnt--;
	}

	// 一時停止解除時の音声の再生
	if (!lpSoundMng.CheckPlaySound("エンドカウント"))
	{
		if (gameCnt <= 660 && gameCnt - 80 > 0)
		{
			lpSoundMng.UnPausingSound("エンドカウント", false);
		}
	}
	// 生存人数が1人以下ならGameSceneを終わる
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
		// ｴﾝﾄﾘｰが終了しているとき
		return;
	}

	// ﾌﾟﾚｲﾔｰｲﾝｽﾀﾝｽ
	PlayerIns();

	// ｹﾞｰﾑﾌﾟﾚｲ
	GamePlay(false);

	for (int k = 0; k < PLAYER_MAX; k++)
	{
		if (players[k] == ID_NON) { continue; }
		if (!entryFlag[k])
		{
			return;	// 一人でもｴﾝﾄﾘｰしてないとき
		}
	}
	// 全員ｴﾝﾄﾘｰが完了しているならﾁｭｰﾄﾘｱﾙへ進む
	gameState = TUTORIAL;
}

void GameScene::Tutorial()
{
	if (gameState != TUTORIAL)
	{
		// ﾁｭｰﾄﾘｱﾙが終了している場合
		return;
	}


	// ｹﾞｰﾑﾌﾟﾚｲできるが当たり判定はない
	GamePlay(false);

	// 音声関係
	PreCnt++;
	if (PreCnt >= 3600 && !sleepFlag)
	{
		// SEの再生
		lpSoundMng.SetSound("あくび");
		sleepFlag = true;
		PreCnt = 0;
	}
	// 5秒毎に確認用SE
	if (!sleepFlag)
	{
		if (PreCnt % 600 == 0)
		{
			lpSoundMng.SetSound("確認");
		}
	}
	else
	{
		if (PreCnt % 600 == 0)
		{
			if (!lpSoundMng.CheckPlaySound("あくび"))
			{
				if (!chengFlag)
				{
					lpSoundMng.SetSound("むにゃ");
					chengFlag = !chengFlag;
				}
				else
				{
					lpSoundMng.SetSound("寝言");
					chengFlag = !chengFlag;
				}
			}
		}
	}

	for (int j = 0; j < PLAYER_MAX; j++)
	{
		// Bﾎﾞﾀﾝを押すと操作説明が出る
		if (controller.GetInputDown(GPAD_INPUT_B, j))
		{
			if (showInstruction && j == pausePadNum)
			{
				showInstruction = false;
				// SE
				lpSoundMng.SetSound("ｷｬﾝｾﾙSE");
			}
			else
			{
				if (!showInstruction)
				{
					showInstruction = true;
					pausePadNum = j;
					// SE
					lpSoundMng.SetSound("決定SE");
				}
			}
		}
	}


	if (showInstruction)
	{
		// 操作説明画面を表示している時はｹﾞｰﾑｽﾀｰﾄできない
		return;
	}


	for (int j = 0; j < PLAYER_MAX; j++)
	{
		// ｽﾀｰﾄﾎﾞﾀﾝを押すとｶｳﾝﾄﾀﾞｳﾝが始まる
		if (controller.GetInputDown(PAD_INPUT_8, j))
		{
			// BGMの停止
			lpSoundMng.StopSound("ｾﾚｸﾄBGM");
			lpSoundMng.StopSound("確認");
			lpSoundMng.StopSound("あくび");
			lpSoundMng.StopSound("むにゃ");
			lpSoundMng.StopSound("寝言");

			//// ﾗﾝﾀﾞﾑでBGMの再生
			//int battleBGM = (rand() % 3);
			//switch (battleBGM)
			//{
			//case 0:
			//	lpSoundMng.SetSound("バトル1BGM", true);	//ﾙｰﾌﾟ
			//	break;

			//case 1:
			//	lpSoundMng.SetSound("バトル2BGM", true);	//ﾙｰﾌﾟ
			//	break;

			//case 2:
			//	lpSoundMng.SetSound("バトル3BGM", true);	//ﾙｰﾌﾟ
			//	break;

			//default:
			//	break;
			//}
			switch (stageID)
			{
			case STAGE_SCHOOL:
				lpSoundMng.SetSound("バトル1BGM", true);	//ﾙｰﾌﾟ
				break;

			case STAGE_TREE_OF_LEGEND:
				lpSoundMng.SetSound("バトル3BGM", true);	//ﾙｰﾌﾟ
				break;

			case STAGE_ARCADE:
				lpSoundMng.SetSound("バトル2BGM", true);	//ﾙｰﾌﾟ
				break;

			default:
				break;
			}
			// SE
			lpSoundMng.SetSound("ぴんぽん");
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

	// ｹﾞｰﾑﾌﾟﾚｲできるが当たり判定はない
	GamePlay(false);

	grayOutCnt += G_OUT_SPEED;

	if (grayOutCnt >= (560 + G_OUT_END_WAIT))
	{
		gameState = COUNT_DOWN;
		lpSoundMng.SetSound("スタートカウント");
	}

	if (grayOutCnt == 0)
	{
		lpSoundMng.SetSound("サイレン");
	}
}

void GameScene::Instruction(void)
{
	if (gameState != INSTRUCTION)
	{
		return;
	}

	// Bﾎﾞﾀﾝを押すとﾎﾟｰｽﾞ画面に戻る
	if (controller.GetInputDown(GPAD_INPUT_B, pausePadNum))
	{
		gameState = PAUSE;
		// SE
		lpSoundMng.SetSound("ｷｬﾝｾﾙSE");
	}
}

void GameScene::CountDown(void)
{
	if (gameState != COUNT_DOWN)
	{
		// ｶｳﾝﾄﾀﾞｳﾝが終わったあとは処理しない
		return;
	}

	// ｹﾞｰﾑﾌﾟﾚｲできるが当たり判定はない
	if (!reStartFlag)
	{
		GamePlay(false);
	}

	gameStartCnt++;					// 3・2・1の表示

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
		return;		// ﾎﾟｰｽﾞﾓｰﾄﾞ中は処理しない
	}

	// ｹﾞｰﾑﾌﾟﾚｲ、当たり判定ありgameCnt
	GamePlay(true);

	gameCnt--;		// ｹﾞｰﾑ中の残り時間を減らす

	for (int k = 0; k < PLAYER_MAX; k++)
	{
		if (players[k] == ID_NON) { continue; }

		// ﾎﾟｰｽﾞﾓｰﾄﾞ切り替え
		if (!gemeEndCnt)
		{
			if (controller.GetInputDown(PAD_INPUT_8, k))
			{
				// SEの再生
				lpSoundMng.SetSound("ポーズ");
				lpSoundMng.StopSound("スタートカウント");
				lpSoundMng.StopSound("エンドカウント");

				pausePadNum = k;
				gameState = PAUSE;
				break;
			}
		}
		//// Bﾎﾞﾀﾝを押すと操作説明が出る
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

	// ｹﾞｰﾑｼｰﾝ・ｷｬﾗｾﾚｼｰﾝ・ﾘｻﾞﾙﾄｼｰﾝ・操作説明のｶｰｿﾙ選択

	if (controller.GetInputDown(PAD_INPUT_UP, pausePadNum))
	{
		// SE
		lpSoundMng.SetSound("ｶｰｿﾙSE");

		changeSnece = (changeSnece > CHANGE_SCENE_NON ? (CHANGE_SCENE)(changeSnece - 1) : CHANGE_SCENE_INSTRUCTION);
	}
	if (controller.GetInputDown(PAD_INPUT_DOWN, pausePadNum))
	{
		// SE
		lpSoundMng.SetSound("ｶｰｿﾙSE");

		changeSnece = (changeSnece < CHANGE_SCENE_INSTRUCTION ? (CHANGE_SCENE)(changeSnece + 1) : CHANGE_SCENE_NON);
	}

	// Bﾎﾞﾀﾝでｹﾞｰﾑに戻る
	if (controller.GetInputDown(GPAD_INPUT_B, pausePadNum)||controller.GetInputDown(PAD_INPUT_8, pausePadNum))
	{
		gameState = COUNT_DOWN;
		reStartFlag = true;		// 321のｶｳﾝﾄﾀﾞｳﾝ中はｷｬﾗ操作不可
		// SE
		lpSoundMng.SetSound("ｷｬﾝｾﾙSE");
		lpSoundMng.SetSound("スタートカウント");	// 321のSE再生

		// 次にﾎﾟｰｽﾞしたときにｶｰｿﾙが一番上に来るよう初期化
		changeSnece = CHANGE_SCENE_NON;
	}
	// Aボタンでｼｰﾝ遷移
	if (controller.GetInputDown(GPAD_INPUT_A, pausePadNum))
	{
		switch (changeSnece)
		{
		case CHANGE_SCENE_INSTRUCTION:
			// SE
			lpSoundMng.SetSound("決定SE");
			gameState = INSTRUCTION;
			break;

		case CHANGE_SCENE_CHAR_SEL:

			// BGMの停止
			lpSoundMng.StopSound("バトル1BGM");
			lpSoundMng.StopSound("バトル2BGM");
			lpSoundMng.StopSound("バトル3BGM");
			lpSoundMng.StopSound("死亡");
			// SE
			lpSoundMng.SetSound("決定SE");

			captureImage = MakeScreen(SCREEN_SIZE_X, SCREEN_SIZE_Y);
			SetDrawScreen(captureImage);
			DrawGraph(0, 0, lpSceneMng.GetTmpBuffer(), true);
			SetDrawScreen(lpSceneMng.GetTmpBuffer());

			mode = MODE_SCENE_BACK;
			break;

		case CHANGE_SCENE_RESULT:

			// BGMの停止
			lpSoundMng.StopSound("バトル1BGM");
			lpSoundMng.StopSound("バトル2BGM");
			lpSoundMng.StopSound("バトル3BGM");
			lpSoundMng.StopSound("死亡");
			// SE
			lpSoundMng.SetSound("決定SE");

			mode = MODE_SCENE_NEXT;
			break;

		case CHANGE_SCENE_NON:
			gameState = COUNT_DOWN;
			reStartFlag = true;		// 321のｶｳﾝﾄﾀﾞｳﾝ中はｷｬﾗ操作不可

			// SE
			lpSoundMng.SetSound("ｷｬﾝｾﾙSE");
			lpSoundMng.SetSound("スタートカウント");	// 321のSE再生

			break;

		case CHANGE_SCENE_MAX:
			// ｴﾗｰ
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

	// 画面を暗くする
	SetDrawBright(140, 140, 140);

	// ｶｯﾄｲﾝ中の処理
	cutInInfo.cnt++;
	if (cutInInfo.cnt >= CUT_IN_TIME)
	{
		if (gameCnt <= 660 && gameCnt - 80 > 0)
		{
			lpSoundMng.UnPausingSound("エンドカウント", false);
		}
		cutInInfo.flag = false;
		SetDrawBright(255, 255, 255);
		gameState = PLAY;
	}
}

bool GameScene::GameDraw()
{
	ClsDrawScreen();

	// 雲の座標更新
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

	// 素敵な背景
	DrawGraph(0, 0, lpImageMng.GetID("image/ゲームシーン用/back.png")[0], true);

	DrawGraph(cloudBackPos1, lpImageMng.GetID("image/ゲームシーン用/雲1.png")[0], true);
	DrawGraph(cloudBackPos2, lpImageMng.GetID("image/ゲームシーン用/雲1.png")[0], true);
	DrawGraph(cloudFrontPos1, lpImageMng.GetID("image/ゲームシーン用/雲2.png")[0], true);
	DrawGraph(cloudFrontPos2, lpImageMng.GetID("image/ゲームシーン用/雲2.png")[0], true);

	DrawGraph(0, 0, lpImageMng.GetID("image/ゲームシーン用/" + imageName[imageID] + ".png")[0], true);

	// ｻｶﾞの描画
	for (auto floor : *floorList)
	{
		floor->Draw();
	}

	// ﾁｭｰﾄﾘｱﾙ中の操作説明の描画
	if (gameState == TUTORIAL)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
		DrawBox(VECTOR2(0, 0), VECTOR2(1280, 560), 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

		// 操作説明画面を表示している時
		if (showInstruction)
		{
			DrawGraph(0, 0, lpImageMng.GetID("image/チュートリアル用/操作説明.png")[0], true);
			if (gameSceneCnt / 60 % 2)
			{
				DrawStringToHandle(1050, 20, "Ｂでとじる", 0xffffff, lpFontMng.SetFont("たぬき40縁"));
			}
		}
		// 操作説明画面を表示していない時
		else
		{
			if (gameSceneCnt / 60 % 2)
			{
				// 文字幅を取り左中央に配置
				StageName = ("スタートボタンではじめる");
				len = static_cast<int>(strlen("スタートボタンではじめる"));

				DrawStringToHandle(
					(SCREEN_SIZE_X - GetDrawStringWidthToHandle(StageName.c_str(), len, lpFontMng.SetFont("たぬき40縁"))) / 2,
					200, "スタートボタンではじめる", 0xffffff, lpFontMng.SetFont("たぬき40縁"));

				DrawStringToHandle(980, 50, "Ｂで操作説明", 0xffffff, lpFontMng.SetFont("たぬき40縁"));
			}
			// 文字幅を取り左中央に配置
			StageName = ("チュートリアル");
			len = static_cast<int>(strlen("チュートリアル"));

			DrawStringToHandle(
				(SCREEN_SIZE_X - GetDrawStringWidthToHandle(StageName.c_str(), len, lpFontMng.SetFont("たぬき80縁"))) / 2,
				70, "チュートリアル", 0xff0000, lpFontMng.SetFont("たぬき80縁"));

		}
	}

	// ｸﾞﾚｰｱｳﾄｽﾗｲﾄﾞ
	if (gameState == GRAYOUT_SLIDE)
	{
		int cnt = (grayOutCnt <= 0) ? 0 : grayOutCnt;
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
		DrawBox(VECTOR2(0, 0), VECTOR2(1280, 560 - cnt), 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}

	// Obj.cppの描画
	for (auto &data : *objList)
	{
		bool moveStopFlag = (gameState == PAUSE || gameState == INSTRUCTION);
		(*data).Draw(moveStopFlag, cutInInfo);
	}

	// ｹﾞｰｼﾞ玉の描画
	for (auto &orb : expOrbList)
	{
		orb->Draw();
	}

	for (auto &chicken : *chickenList)
	{
		chicken->Draw();
	}

	// 殺害鶏数の描画
	int t_c_KillCnt = 0;
	for (auto chickenKillCnt : resultInfo.chickenKillCnt)
	{
		t_c_KillCnt += chickenKillCnt;
	}
	if (t_c_KillCnt)
	{
		int drawLengh;
		drawLengh = GetDrawFormatStringWidthToHandle(lpFontMng.SetFont("たぬき50縁"), "× %d 羽", t_c_KillCnt);
		DrawGraph(SCREEN_SIZE_X - 72 - drawLengh, 10, lpImageMng.GetID("image/ゲームシーン用/Dead.png")[0], true);
		DrawFormatStringToHandle(SCREEN_SIZE_X - drawLengh, 72 / 2 - 5, 0xff0000, lpFontMng.SetFont("たぬき50縁"), "× %d 羽", t_c_KillCnt, true);
	}

	// 参加ﾎﾞﾀﾝを押してね的な描画
	if (gameState == ENTRY)
	{
		for (int k = 0; k < PLAYER_MAX; k++)
		{
			if (players[k] == ID_NON) { continue; }
			if (!entryFlag[k] && players[k] != ID_NON)
			{
				if ((gameSceneCnt / 30) % 2)
				{
					DrawStringToHandle(30 + 320 * k, 620, "STARTで参加", 0xff0000, lpFontMng.SetFont("たぬき40縁"));
				}
			}
		}
	}

	// ｶｳﾝﾄﾀﾞｳﾝ中の描画
	if (gameState == COUNT_DOWN)
	{
		// 3・2・1の描画
		if (gameStartCnt < GAME_START_WAIT_TIME)
		{
			DrawFormatStringToHandle(565, 100, 0xff0000, lpFontMng.SetFont("たぬき100縁"), "%d秒", 3 - (gameStartCnt / 60), true);
		}

		// 残り時間の描画(ここでは減算しない)
		DrawFormatStringToHandle(0, 0, 0xff0000, lpFontMng.SetFont("たぬき50縁"), "残り%d秒", gameCnt / 60, true);
	}

	// ｹﾞｰﾑ中の描画
	if (gameState == PLAY)
	{
		if ((TIME_LIM * 60 - gameCnt) < 60)
		{
			// 文字幅を取り左中央に配置
			StageName = ("すたーと！");
			len = static_cast<int>(strlen("すたーと！"));

			DrawStringToHandle(
				(SCREEN_SIZE_X - GetDrawStringWidthToHandle(StageName.c_str(), len, lpFontMng.SetFont("たぬき100縁"))) / 2,
				100, "すたーと！", 0xff0000, lpFontMng.SetFont("たぬき100縁"));
		}

		if (!gemeEndCnt)
		{
			DrawFormatStringToHandle(0, 0, 0xff0000, lpFontMng.SetFont("たぬき50縁"), "残り%d秒", gameCnt / 60, true);

			if (gameCnt <= 660)
			{
				DrawFormatStringToHandle(565, 100, 0xff0000, lpFontMng.SetFont("たぬき100縁"), "%d秒", gameCnt / 60, true);
			}
		}

	}

	if (gemeEndCnt)
	{
		DrawStringToHandle(430, 100, "そこまで！", 0xff0000, lpFontMng.SetFont("たぬき100縁"));
	}

	// ﾎﾟｰｽﾞﾓｰﾄﾞ中の描画
	if (gameState == PAUSE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
		DrawBox(VECTOR2(0, 0), VECTOR2(1280, 720), 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

		// 文字幅を取り左中央に配置
		StageName = ("ポーズ");
		len = static_cast<int>( strlen("ポーズ") );

		DrawStringToHandle(
			(SCREEN_SIZE_X - GetDrawStringWidthToHandle(StageName.c_str(), len, lpFontMng.SetFont("たぬき100縁"))) / 2,
			150, "ポーズ", 0xffffff, lpFontMng.SetFont("たぬき100縁"));

		DrawStringToHandle(450, 310, "ゲーム画面へ戻る",		0xffffff, lpFontMng.SetFont("たぬき40縁"));
		DrawStringToHandle(450, 390, "キャラクター選択画面へ",	0xffffff, lpFontMng.SetFont("たぬき40縁"));
		DrawStringToHandle(450, 470, "リザルト画面へ",			0xffffff, lpFontMng.SetFont("たぬき40縁"));
		DrawStringToHandle(450, 550, "操作説明を表示",			0xffffff, lpFontMng.SetFont("たぬき40縁"));

		DrawGraph(400, 310 + 80 * changeSnece, lpImageMng.GetID("image/ゲームシーン用/Abutton.png")[0], true);

		if (gameSceneCnt / 60 % 2)
		{
			DrawStringToHandle(1000, 20, "Ａ:けってい",			0xffffff, lpFontMng.SetFont("たぬき40縁"));
			DrawStringToHandle(786,	 70, "スタートかＢ:とじる", 0xffffff, lpFontMng.SetFont("たぬき40縁"));
		}
	}

	// 操作説明の描画
	if (gameState == INSTRUCTION)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
		DrawBox(VECTOR2(0, 0), VECTOR2(1280, 720), 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

		DrawGraph(0, 0, lpImageMng.GetID("image/チュートリアル用/操作説明.png")[0], true);
		if (gameSceneCnt / 60 % 2)
		{
			DrawStringToHandle(1000, 20, "Ｂ:とじる", 0xffffff, lpFontMng.SetFont("たぬき40縁"));
		}
	}

	// ﾎﾟｰｽﾞ画面 || 操作説明 を 開いているﾌﾟﾚｲﾔｰ番号の表示
	if ((gameState == INSTRUCTION) || (gameState == PAUSE) || ((gameState == TUTORIAL) && showInstruction))
	{
		VECTOR2 graphSize;
		GetGraphSize(lpImageMng.GetID("image/UI/1234.png")[pausePadNum], &graphSize.x, &graphSize.y);

		DrawRotaGraph(415 - graphSize.x/2, graphSize.y/2 + 320 + 80 * changeSnece, 1.0, 0.0,
			lpImageMng.GetID("image/UI/1234.png")[pausePadNum],true);
	}

	// ｶｯﾄｲﾝ中の描画
	if (gameState == CUT_IN)
	{
		SetDrawBright(255, 255, 255);	// 画面の明るさをもとに戻す

		if (cutInInfo.cnt < CUT_IN_TIME / 2)
		{
			int animCnt = cutInInfo.cnt / 6;	// ++されるよー 
			if (animCnt < 5 * 4)
			{
				DrawRectGraph(
					270, 0,
					720 * (animCnt % 5), 720 * (animCnt / 5 % 4),
					720, 720,
					lpImageMng.GetID("image/UI/その他11.png")[0], true, false);
			}
		}
		else
		{
			int animCnt = (cutInInfo.cnt - CUT_IN_TIME / 2) / 4;	// ++されるよー 
			if (animCnt < 5 * 4)
			{
				DrawRectGraph(
					cutInInfo.pos.x - 150 / 2, cutInInfo.pos.y - 150 / 2,
					150 * (animCnt % 5), 150 * (animCnt / 5 % 4),
					150, 150,
					lpImageMng.GetID("image/UI/その他3.png")[0], true, false);
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
		lastGameScreen = MakeScreen(SCREEN_SIZE_X, SCREEN_SIZE_Y);	// ﾌﾙｽｸﾘｰﾝｻｲｽﾞのﾊﾞｯﾌｧgameScreenを作成
		SetDrawScreen(lastGameScreen);								// 描画ﾊﾞｯﾌｧをgameScreenに設定
		DrawGraph(0, 0, lpSceneMng.GetTmpBuffer(), true);			// gameScreenに描画
		SetDrawScreen(lpSceneMng.GetTmpBuffer());
	}

	ClsDrawScreen();

	// ﾎﾜｲﾄｱｳﾄ
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
	SetDrawScreen(DX_SCREEN_BACK);						// tmpBufferに描画していた(SceneMngのSysInitで設定)のをbackScreenに戻す
	ClsDrawScreen();
	DrawGraph(0, 0, lpSceneMng.GetTmpBuffer(), true);	//backScreenに今までの描画(tmpBuffer)をする ここで画面効果を追加できる
	SetDrawScreen(lpSceneMng.GetTmpBuffer());			// tmpBufferに描画するように変更														//--------------------------------------------------------------------------------------

	ScreenFlip();

	return false;
}

void GameScene::PageInstance(void)
{
	lastGameScreen = MakeScreen(SCREEN_SIZE_X, SCREEN_SIZE_Y);	// ﾌﾙｽｸﾘｰﾝｻｲｽﾞのﾊﾞｯﾌｧgameScreenを作成
	SetDrawScreen(lastGameScreen);								// 描画ﾊﾞｯﾌｧをgameScreenに設定
	DrawGraph(0, 0, lpSceneMng.GetTmpBuffer(), true);			// gameScreenに描画

	int resultScreen = MakeScreen(SCREEN_SIZE_X, SCREEN_SIZE_Y);
	SetDrawScreen(resultScreen);
	DrawGraph(0, 0, lpImageMng.GetID("image/リザルト用/result.png")[0], true);
	DrawGraph(292, 170, lpImageMng.GetID("image/リザルト用/ジャパニカ128.png")[0], true);
	SetDrawScreen(lpSceneMng.GetTmpBuffer());

	turnThePage = TurnThePage(VECTOR2(SCREEN_SIZE_X, SCREEN_SIZE_Y), VECTOR2(0, 0), lastGameScreen, resultScreen, 1.0f);
}

void GameScene::BufferChange(void)
{
	SetDrawScreen(DX_SCREEN_BACK);
	ClsDrawScreen();
	DrawGraph(drawOffset, lpSceneMng.GetTmpBuffer(), true);	// ここで画面効果を追加できる
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
		lpSoundMng.SoundInit("sound/Battle.mp3", "バトル1BGM", 90);
		break;

	case STAGE_TREE_OF_LEGEND:
		lpSoundMng.SoundInit("sound/Battle3.mp3", "バトル3BGM", 90);
		break;

	case STAGE_ARCADE:
		AddFloorList()(floorList, std::make_shared<FloorCar>());
		lpSoundMng.SoundInit("sound/Battle2.mp3", "バトル2BGM");
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

	changeSnece = CHANGE_SCENE_NON;	// ﾎﾟｰｽﾞﾓｰﾄﾞﾒﾆｭｰ初期設定

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

	alpha = 0.0;					// 透明度を設定(DrawBoxの白に対しての透明度)
				
	alpha.set(255, FADE_SPEED);		// 透明度をFADE_SPEED(ﾌﾚｰﾑ)かけて最大(255)にする

	// BGM･SEの登録
	//lpSoundMng.SoundInit("sound/Battle.mp3", "バトル1BGM",90);
	//lpSoundMng.SoundInit("sound/Battle2.mp3", "バトル2BGM");
	//lpSoundMng.SoundInit("sound/Battle3.mp3", "バトル3BGM", 90);

	lpSoundMng.SoundInit("sound/SE類/StartCount.mp3", "スタートカウント");
	lpSoundMng.SoundInit("sound/SE類/BatteleStart.mp3", "笛");
	lpSoundMng.SoundInit("sound/SE類/EndCount.mp3", "エンドカウント");
	lpSoundMng.SoundInit("sound/SE類/Pause.mp3", "ポーズ");
	lpSoundMng.SoundInit("sound/SE類/Preparation.mp3", "確認");

	lpSoundMng.SoundInit("sound/SE類/Sleep1.mp3", "あくび");
	lpSoundMng.SoundInit("sound/SE類/Sleep2.mp3", "むにゃ");
	lpSoundMng.SoundInit("sound/SE類/Sleep3.mp3", "寝言");

	lpSoundMng.SoundInit("sound/SE類/チャイム.mp3", "チャイム");
	lpSoundMng.SoundInit("sound/SE類/そこまで.mp3", "そこまで");

	lpSoundMng.SoundInit("sound/SE類/サイレン.mp3", "サイレン");
	lpSoundMng.SoundInit("sound/SE類/PlayerIns.mp3", "エントリー");
	lpSoundMng.SoundInit("sound/SE類/ぴんぽん.mp3", "ぴんぽん");

	// SE用
	PreCnt = 0;
	sleepFlag = false;
	chengFlag = false;

	return 0;
}