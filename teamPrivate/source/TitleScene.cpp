#include "DxLib.h"
#include "SoundMng.h"
#include "FontMng.h"
#include "SceneMng.h"
#include "GameController.h"
#include "SelectScene.h"
#include "TitleScene.h"
//#include "PlayerNumSelectScene.h"

//定義
#define CHANGE_SCENE	(30)	// 画面遷移までのｶｳﾝﾄ

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
			//// BGMの停止
			lpSoundMng.StopSound("ﾀｲﾄﾙBGM");
			return std::make_unique<SelectScene>();
		}
		return std::move(own);	// ｼｰﾝ継続
	}

	// Startｷｰ
	int Start_KYE = PAD_INPUT_8;

	for (int k = 0; k < PLAYER_MAX; k++)
	{
		if (controller.GetInputDown(KEY_INPUT_SPACE) || (controller.GetInputDown(Start_KYE, k)))
		{
			if (!ChangeSceneFlag)
			{
				// SE
				lpSoundMng.SetSound("決定SE");

				flasing = FLASING_HIGH_CNT;
				Cnt = 0;
				ChangeSceneFlag = true;

				// ｾﾚｸﾄｼｰﾝ用変数
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
			//DrawGraph(0, 0, lpImageMng.GetID("image/キャラセレ用/ジャパニカ.png")[0], true);
			SetDrawScreen(DX_SCREEN_BACK);

			turnThePage = TurnThePage(VECTOR2(SCREEN_SIZE_X, SCREEN_SIZE_Y), VECTOR2(0, 0), titleSceneImage, lpImageMng.GetID("image/キャラセレ用/PageSerect.png")[0], 1.0f);
			mode = MODE_SCENE_NEXT;
		}
	}

	TitleDraw();


	return std::move(own);	// ｼｰﾝ継続
}

bool TitleScene::TitleDraw(void)
{
	Cnt++;

	// 背景
	DrawGraph(0, 0, lpImageMng.GetID("image/タイトル用/Title.png")[0], true);
	DrawStringToHandle(870,125,"最大４人対戦", 0x000000, lpFontMng.SetFont("たぬき40"));
	DrawStringToHandle(800, 162, "ドタバタ！アクションバトル!!", 0x000000, lpFontMng.SetFont("たぬき27"));
	if (Cnt / flasing % 2 == 0)
	{
		DrawStringToHandle(
			700,
			550,
			"スタートボタンをおしてね", 0xff0000, lpFontMng.SetFont("たぬき45縁"));
	}

	//---------------------------------------------------------------------------------------
	SetDrawScreen(DX_SCREEN_BACK);						// tmpBufferに描画していた(SceneMngのSysInitで設定)のをbackScreenに戻す
	ClsDrawScreen();
	DrawGraph(0, 0, lpSceneMng.GetTmpBuffer(), true);	//backScreenに今までの描画(tmpBuffer)をする ここで画面効果を追加できる
	SetDrawScreen(lpSceneMng.GetTmpBuffer());			// tmpBufferに描画するように変更
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
	SetDrawScreen(DX_SCREEN_BACK);						// tmpBufferに描画していた(SceneMngのSysInitで設定)のをbackScreenに戻す
	ClsDrawScreen();
	DrawGraph(0, 0, lpSceneMng.GetTmpBuffer(), true);	//backScreenに今までの描画(tmpBuffer)をする ここで画面効果を追加できる
	SetDrawScreen(lpSceneMng.GetTmpBuffer());			// tmpBufferに描画するように変更														//--------------------------------------------------------------------------------------

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

	//SE･BGMの登録
	lpSoundMng.SoundInit("sound/Title.mp3", "ﾀｲﾄﾙBGM");
	lpSoundMng.SoundInit("sound/Select.mp3", "ｾﾚｸﾄBGM");

	lpSoundMng.SoundInit("sound/Battle.mp3", "バトル1BGM");
	lpSoundMng.SoundInit("sound/Battle2.mp3", "バトル2BGM");
	lpSoundMng.SoundInit("sound/Battle3.mp3", "バトル3BGM", 90);

	lpSoundMng.SoundInit("sound/SE類/DecisionSE.mp3", "決定SE");
	
	// BGMの再生
	lpSoundMng.SetSound("ﾀｲﾄﾙBGM", true);
	
	// ﾌｫﾝﾄの登録
	lpFontMng.FontInit("font/TanukiMagic.ttf", "たぬき油性マジック", "たぬき27", 27, -1, false, false);
	lpFontMng.FontInit("font/TanukiMagic.ttf", "たぬき油性マジック", "たぬき40",35, -1, false, false);
	lpFontMng.FontInit("font/TanukiMagic.ttf", "たぬき油性マジック", "たぬき45縁", 45, -1, true,false);
	lpFontMng.FontInit("font/AVOCADO.TTF", "アボカド", "アボカド縁27", 27, -1, true, false);

	lpFontMng.FontInit("font/Molot.otf", "Molot", "Molot40縁斜体", 35, -1, true, true);

	lpFontMng.FontInit("font/Square.ttf", "SquareFont", "スクエア28縁斜体", 28, -1, true,true);
	lpFontMng.FontInit("font/Square.ttf", "SquareFont", "スクエア18縁斜体", 18, -1, true, true);
	lpFontMng.FontInit("font/Square.ttf", "SquareFont", "スクエア20縁斜体", 20, -1, true, true);
	// 初期化
	Cnt = 0;
	flasing = FLASING_LOW_CNT;
	ChangeSceneFlag = false;
	startPadNum = 0;

	mode = MODE_SCENE_NOW;

	return 0;
}
