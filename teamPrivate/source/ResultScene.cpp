#include <algorithm>
#include "ResultScene.h"
#include "ImageMng.h"
#include "GameController.h"
#include "SceneMng.h"
#include "SoundMng.h"
#include "SelectScene.h"

#define DRAW_TIME	(300)

#define BAR_MAX_LENGTH	360.0f

#define ICON_SIZE_X		152		// ｷｬﾗｱｲｺﾝ画像の横ｻｲｽﾞ
#define ICON_SIZE_Y		230		// ｷｬﾗｱｲｺﾝ画像の縦ｻｲｽﾞ
#define CHAR_OFFSET		100		// ｷｬﾗｱｲｺﾝを表示する余白

#define GRAPH_SIZE_X	100		// ｸﾞﾗﾌの横ｻｲｽﾞ
#define GRAPH_SIZE_Y	40		// ｸﾞﾗﾌの縦ｻｲｽﾞ
#define GRAPH_UNDER_POS	550		// ｸﾞﾗﾌの下の座標

#define GRAPH_ADD_SPEED	1.0f	// ｸﾞﾗﾌを伸ばす速さ

#define GRAY_OUT_TIME	360		// ｸﾞﾚｰｱｳﾄｽﾗｲﾄﾞが画面にとどまる時間(ﾌﾚｰﾑ)

#define GRAY_OUT_SPEED	20		// ｸﾞﾚｰｱｳﾄｽﾗｲﾄﾞの1ﾌﾚｰﾑの量

#define CHICKEN_POINT	20		// にわとり1世代につき加算するポイント量

#define AJUST_TIME		180		// 1位発表→ｼｰﾝ遷移可能になるまでの時間

#define FADE_SPEED		180		// ﾌｪｰﾄﾞｲﾝ・ﾌｪｰﾄﾞｱｳﾄが完了するまでの時間(ﾌﾚｰﾑ)

ResultScene::ResultScene(RESULT_INFO resultInfo, int joinPlayerNum)
{
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	awardEndFlag = true;
	resEndFlag = false;
	awardCnt = 0;
	ajustCnt = 0;

	resultState = STATE_FADE_IN;	// ﾌｪｰﾄﾞｲﾝ状態
	alpha = 255.0;					// 透明度を設定(DrawBoxの白に対しての透明度)
	
	// 透明度をFADE_SPEED(ﾌﾚｰﾑ)かけて最小(0)にする
	alpha.set(0, FADE_SPEED);

	grayOutCnt = 0;
	grayOut = GRAY_OUT_NON;

	graphAddPoint = {};

	awardState = static_cast<AWARD>(-1);

	this->resultInfo = resultInfo;


	SortByRank();		// resultInfoの得点を降順に並び替え、rankInfoに格納する


	// 順位の基礎点
	rankPointTbl = { 100, 50, 25, 0 };

	// ﾎﾟｲﾝﾄ情報を格納
	for (int k = 0; k < PLAYER_MAX; k++)
	{
		pointInfo.totalGiveDamage[k]	= rankPointTbl[rankInfo.totalGiveDamage[k]];
		pointInfo.deathEnemyCnt[k]		= rankPointTbl[rankInfo.deathEnemyCnt[k]];
		pointInfo.deathOrder[k]			= rankPointTbl[rankInfo.deathOrder[k]];

		// ニワトリ世代
		pointInfo.chickenCnt[k] = resultInfo.chickenCnt[k] * CHICKEN_POINT;
	}

	scoreMax = 0;		// 合計最高得点
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

	//SE･BGMの登録
	lpSoundMng.SoundInit("sound/Result.mp3", "リザルトBGM");
	lpSoundMng.SoundInit("sound/SE類/結果を発表します.mp3", "結果発表");
	lpSoundMng.SoundInit("sound/SE類/ドラムロール.mp3", "ドラムロール");
	lpSoundMng.SoundInit("sound/SE類/すごいすごい.mp3", "すごい");
	lpSoundMng.SoundInit("sound/SE類/また遊んでね.mp3", "また遊んでね");

	// BGMの再生
	lpSoundMng.SetSound("リザルトBGM", true);
	lpSoundMng.SetSound("結果発表");

	// ﾌｫﾝﾄの初期化
	lpFontMng.FontInit("font/TanukiMagic.ttf", "たぬき油性マジック", "たぬき35縁", 35, -1, true, false);
	lpFontMng.FontInit("font/TanukiMagic.ttf", "たぬき油性マジック", "たぬき23縁", 35, -1, true, false);

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
		lpSoundMng.StopSound("リザルトBGM");
		return std::make_unique<SelectScene>(true);

	case STATE_MAX:
		break;

	default:
		break;
	}

	resultCnt++;

	return std::move(own);	// ｼｰﾝ継続
}

void ResultScene::StateFadeInFunc(void)
{
	// ﾌｪｰﾄﾞｲﾝが終わったらSTATE_AWARDにする
	if (alpha <= 0)
	{
		resultState = STATE_AWARD;
		return;
	}

	alpha.update();
	// 描画処理
	ResultDraw();
}

void ResultScene::StateAwardFunc(const GameController &ctrl)
{
	for (int k = 0; k < PLAYER_MAX; k++)
	{
		if (ctrl.GetInputDown(GPAD_INPUT_A, k))
		{
			// Aで次の部門へ
			if (awardEndFlag && awardState < AWARD_MAX - 1)
			{
				grayOut = GRAY_OUT_DOWN;
				awardState	 = (awardState < static_cast<AWARD>(AWARD_MAX - 1)) ? static_cast<AWARD>(awardState + 1) : awardState;
				awardEndFlag = false;
				awardCnt = 0;		// ｶｳﾝﾀｰ初期化
				seCnt	= 0;
				seFlag	= true;
				seFlag2 = true;
				seFlag3 = true;
				break;
			}

			// Aでﾌｪｰﾄﾞｱｳﾄ開始
			if (resEndFlag && (ajustCnt > AJUST_TIME))
			{
				resultState = STATE_FADE_OUT;

				// 透明度をFADE_SPEED(ﾌﾚｰﾑ)かけて最大(255)にする
				alpha.set(255, FADE_SPEED);

				lpSoundMng.SetSound("また遊んでね");
			}
		}
	}

	if (resEndFlag)
	{
		ajustCnt++;
	}

	bool checkFlag[4] = {};		// 各部門ﾎﾟｲﾝﾄ加算が終わればtrue

	for (int k = 0; k < PLAYER_MAX; k++)
	{
		checkFlag[k] = AwardFunc(k, awardState);
	}
	awardCnt++;

	if (awardState >= AWARD_GIVE_DAMAGE)
	{
		awardEndFlag = true;	// tureで初期化
		for (int k = 0; k < PLAYER_MAX; k++)
		{
			if (!checkFlag[k])
			{
				awardEndFlag = false;	// 1人でもﾎﾟｲﾝﾄ加算が終わってなければfalse
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
				resEndFlag = true;		// 全部門の棒ｸﾞﾗﾌ加算まで終了
			}
		}
	}

	// 描画処理
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
	// 描画処理
	ResultDraw();
}

bool ResultScene::AwardFunc(int pNum, AWARD awardState)
{
	if (awardCnt < (720 / GRAY_OUT_SPEED) + GRAY_OUT_TIME)
	{
		// この間は詳細表示中
		return false;
	}

	float arawdPoint;	// 部門のﾎﾟｲﾝﾄ
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
			lpSoundMng.SetSound("怒りゲージ溜め");
		}
		seCnt++;

		return false;
	}

	return true;
}

void ResultScene::NoJoinPadNumPointInit(RESULT_INFO& c_resultInfo)
{
	// 不参加のﾌﾟﾚｲﾔは-1にする
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
	RESULT_INFO c_resultInfo = resultInfo;		// ﾘｻﾞﾙﾄﾃﾞｰﾀｰのｺﾋﾟｰを用意

	NoJoinPadNumPointInit(c_resultInfo);

	// 順位降順でソート（1位2位3位4位）
	sort(c_resultInfo.totalGiveDamage.begin(),	c_resultInfo.totalGiveDamage.end(), greater<int>());
	sort(c_resultInfo.deathEnemyCnt.begin(),	c_resultInfo.deathEnemyCnt.end(),	greater<int>());
	sort(c_resultInfo.deathOrder.begin(),		c_resultInfo.deathOrder.end(),		greater<int>());
	sort(c_resultInfo.chickenCnt.begin(),		c_resultInfo.chickenCnt.end(),		greater<int>());

	// ﾗﾑﾀﾞ式
	using INFO = std::array<int, PLAYER_MAX>;
	auto ranking = [](int j, int k, INFO ori_info, INFO c_info, INFO& rank_info)
	{
		if (c_info[j] == ori_info[k])
		{
			rank_info[k] = j;
		}
	};

	// rankInfoに順位を入れる
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
	DrawGraph(0, 0, lpImageMng.GetID("image/リザルト用/result.png")[0], true);


	// 画面下の横線
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

			// DrawBox開始Y座標
			graphLength[k][j] = static_cast<int>(increasePosY * pointUnit);
			int posY = GRAPH_UNDER_POS - graphLength[k][j];

			// 棒グラフ描画
			// 正面
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

		std::string charIconPass = "image/" + resultInfo.charName[k] + "/アイコン/icon.png";

		SetDrawMode(DX_DRAWMODE_BILINEAR);

		// ｷｬﾗの描画
		DrawRotaGraph(
			PosXTbl[k] - ICON_SIZE_X / 2 + 75, 610,
			0.8, 0.0,
			lpImageMng.GetID(charIconPass)[0], true);

		SetDrawMode(DX_DRAWMODE_NEAREST);

		// 現在棒ｸﾞﾗﾌに表示されている合計得点
		int offset = 80;
		if (graphAddPoint[k] < 100)
		{
			// 得点が100以下の場合は描画ｵﾌｾｯﾄ値を変更
			offset = 60;
		}
		DrawFormatStringToHandle(
			PosXTbl[k] - offset, 430,
			0xffffff, lpFontMng.SetFont("たぬき60縁"), "%dpt", graphAddPoint[k], true);
	}

	// ｸﾞﾚｰｱｳﾄｽﾗｲﾄﾞの表示
	DrawGrayOut();

	for (int k = 0; k < PLAYER_MAX; k++)
	{
		if (resultInfo.charName[k] == "")
		{
			continue;
		}

		std::string charIconPass = "image/" + resultInfo.charName[k] + "/アイコン/icon.png";

		SetDrawMode(DX_DRAWMODE_BILINEAR);

		// ｷｬﾗの描画
		DrawRotaGraph(
			PosXTbl[k] - ICON_SIZE_X / 2 + 75, 610,
			0.8, 0.0,
			lpImageMng.GetID(charIconPass)[0], true);

		SetDrawMode(DX_DRAWMODE_NEAREST);

		// 1P,2Pの表示
		DrawGraph(PosXTbl[k] - 120, GRAPH_UNDER_POS - 35,lpImageMng.GetID("image/UI/1234.png")[k], true);
	}

	auto award = [&](int offsetX, const TCHAR* sentence, int color)
	{
		if (awardCnt >= (720 / GRAY_OUT_SPEED) + 60)
		{
			if (seFlag)
			{
				if (sentence != "にわとりの世代")
				{
					lpSoundMng.SetSound("ガード");
					seFlag = false;
				}
				else
				{
					lpSoundMng.SetSound("ｺｹｺｯｺｰ");
					seFlag = false;
				}
			}

			DrawStringToHandle(350 + offsetX, 50, sentence, GraphColorTbl[color], lpFontMng.SetFont("たぬき100縁"));
		}
	};

	// 順位の描画
	auto DrawRank = [&](int k, std::array<int, PLAYER_MAX> info)
	{
		if (awardCnt >= (720 / GRAY_OUT_SPEED) + 180)
		{
			if (seFlag3)
			{
				if (awardState != AWARD_CHICKEN)
				{
					lpSoundMng.SetSound("ガード");
					seFlag3 = false;
				}
				else
				{
					lpSoundMng.SetSound("ｺｹｰ");
					seFlag3 = false;
				}
			}
			DrawFormatStringToHandle(
				PosXTbl[k] - 70, 320,
				GetRankColor(info[k] + 1), lpFontMng.SetFont("たぬき100縁"), "%d位", info[k] + 1, true);
		}
	};

	// 獲得ポイントの描画（awardState == AWARD_CHICKENの場合に限り第三引数をtureにすること）
	auto DrawPoint = [&](int k, std::array<int, PLAYER_MAX> ptInfo, std::array<int, PLAYER_MAX> RankInfo = {}, bool awardChicenFlg = false)
	{
		if (awardCnt >= (720 / GRAY_OUT_SPEED) + 180)
		{
			// awardState == AWARD_CHICKENの場合はポイントからでなく順位から色を取得する
			auto GetColor = [=]() { return (awardChicenFlg ? GetRankColor(RankInfo[k] + 1) : GetPointColor(ptInfo[k])); };

			DrawFormatStringToHandle(
				PosXTbl[k] - 50, 450,
				GetColor(), lpFontMng.SetFont("たぬき40"), "+%dpt", ptInfo[k], true);
		}
	};


	if (grayOutCnt >= 720 && awardCnt <= 1000)
	{
		if (awardState == AWARD_GIVE_DAMAGE)
		{
			// 与えたダメージの描画
			award(-60, "与えたダメージ", static_cast<int>(AWARD_GIVE_DAMAGE));
		}
		if (awardState == AWARD_DEATH_ENEMY_CNT)
		{
			// 倒した人数
			award(0, "倒した人数", static_cast<int>(AWARD_DEATH_ENEMY_CNT));
		}
		if (awardState == AWARD_DEATH_ODER)
		{
			// 死んだ順番
			award(0, "倒された順番", static_cast<int>(AWARD_DEATH_ODER));
		}
		if (awardState == AWARD_CHICKEN)
		{
			// ころしたにわとり
			award(-80, "にわとりの世代", static_cast<int>(AWARD_CHICKEN));
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
						lpSoundMng.SetSound("ガード");
						seFlag2 = false;
					}

					DrawFormatStringToHandle(
						PosXTbl[k] - 70, 200,
						0xffffff, lpFontMng.SetFont("たぬき100縁"),
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
						lpSoundMng.SetSound("ガード");
						seFlag2 = false;
					}

					DrawFormatStringToHandle(
						PosXTbl[k] - 60, 200,
						0xffffff, lpFontMng.SetFont("たぬき100縁"),
						"%d人", resultInfo.deathEnemyCnt[k], true);
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
						lpSoundMng.SetSound("ガード");
						seFlag2 = false;
					}
					// 倒されていないなら[ - ]を表示
					if (resultInfo.deathOrder[k] == 4)
					{
						DrawStringToHandle(PosXTbl[k] - 60, 200, "ー", 0xffffff, lpFontMng.SetFont("たぬき100縁"));
					}
					// 倒されたなら順位を表示
					else
					{
						DrawFormatStringToHandle(
							PosXTbl[k] - 60, 200,
							0xffffff, lpFontMng.SetFont("たぬき100縁"),
							"%d番目", resultInfo.deathOrder[k], true);
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
						lpSoundMng.SetSound("ｺｹｰ");
						seFlag2 = false;
					}
					if (resultInfo.chickenCnt[k] == 0)
					{
						DrawStringToHandle(PosXTbl[k] - 80, 200, "ー", 0xffffff, lpFontMng.SetFont("EP100"));
					}
					else if (resultInfo.chickenCnt[k] == 1)
					{
						DrawStringToHandle(PosXTbl[k] - 80, 200, "初代", 0xffffff, lpFontMng.SetFont("EP100"));
					}
					else
					{
						DrawFormatStringToHandle(
							PosXTbl[k] - 80, 200,
							0xffffff, lpFontMng.SetFont("EP100"),
							"%d代", resultInfo.chickenCnt[k], true);
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
					DrawStringToHandle(1000, 20, "Aですすむ", 0xffffff, lpFontMng.SetFont("たぬき40縁"));
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
					lpSoundMng.SetSound("復活");
					lpSoundMng.SetSound("すごい");
					winSeFlag = true;
				}

				DrawRotaGraph(
					PosXTbl[k], static_cast<int>(GRAPH_UNDER_POS - BAR_MAX_LENGTH - 60),
					1.0, 0.0, lpImageMng.GetID("image/リザルト用/oukan.png")[0], true, false);

				DrawStringToHandle(700, 20, "Aでキャラクター選択へ戻る", 0xffffff, lpFontMng.SetFont("たぬき40縁"));
			}
		}
	}


	DrawStringToHandle(10, 5, "けっかはっぴょー", 0xff0000, lpFontMng.SetFont("たぬき40縁"));

	// ﾌｪｰﾄﾞｲﾝ・ﾌｪｰﾄﾞｱｳﾄ
	DrawFade();
	
	//---------------------------------------------------------------------------------------
	SetDrawScreen(DX_SCREEN_BACK);						// tmpBufferに描画していた(SceneMngのSysInitで設定)のをbackScreenに戻す
	ClsDrawScreen();
	DrawGraph(0, 0, lpSceneMng.GetTmpBuffer(), true);	//backScreenに今までの描画(tmpBuffer)をする ここで画面効果を追加できる
	SetDrawScreen(lpSceneMng.GetTmpBuffer());			// tmpBufferに描画するように変更									//---------------------------------------------------------------------------------------

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

	// ｸﾞﾚｰｱｳﾄｽﾗｲﾄﾞをUPにする条件
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
		// 1位なら赤
		return 0xff0000;
	}
	if (rank == 2)
	{
		// 2位なら黄
		return 0xffff00;
	}
	if (rank == 3)
	{
		// 3位なら青
		return 0x00bfff;
	}

	// 4位は白
	return 0xffffff;
}

int ResultScene::GetPointColor(int point)
{
	if (point == rankPointTbl[0])
	{
		// 1位なら赤
		return 0xff0000;
	}
	if (point == rankPointTbl[1])
	{
		// 2位なら黄
		return 0xffff00;
	}
	if (point == rankPointTbl[2])
	{
		// 3位なら青
		return 0x00bfff;
	}

	// 3,4位は白
	return 0xffffff;
}

void ResultScene::DrawFade(void)
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(alpha));
	DrawBox(VECTOR2(0, 0), lpSceneMng.GetScreenSize(), 0xffffff, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
