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
		// 酔った勢いでできたﾌﾗｯｼｭ用のｶｳﾝﾄ
		flashCnt += flashCnt / 30;
		if (flashCnt >= 150)
		{
			return make_unique<GameScene>(ID, num, stageID);
		}
		return std::move(own);	// ｼｰﾝ継続
	}

	if (mode == MODE_SCENE_BACK)
	{
		if (PageDraw())
		{
			// ﾍﾟｰｼﾞをめくり終わったらｼｰﾝ遷移
			return make_unique<SelectScene>();
		}

		return move(own);	// ｼｰﾝ継続
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
						lpSoundMng.SetSound("ｶｰｿﾙSE");
					}
					else
					{
						// SE
						lpSoundMng.SetSound("ドン");
					}
				}
			}
		};

		if (!endFlag)
		{
			// 選択ｶｰｿﾙの移動
			MovCursor(PAD_INPUT_UP,		VECTOR2(0, -1));
			MovCursor(PAD_INPUT_DOWN,	VECTOR2(0, +1));
			MovCursor(PAD_INPUT_LEFT,	VECTOR2(-1, 0));
			MovCursor(PAD_INPUT_RIGHT,	VECTOR2(+1, 0));

			// Bﾎﾞﾀﾝでｷｬﾗｾﾚへ遷移
			if (ctrl.GetInputDown(GPAD_INPUT_B, n))
			{
				pageImage = MakeScreen(PAGE_SIZE_X, PAGE_SIZE_Y);
				SetDrawScreen(pageImage);
				DrawGraph(0, 0, lpSceneMng.GetTmpBuffer(), true);
				SetDrawScreen(DX_SCREEN_BACK);

				turnThePage = TurnThePage(VECTOR2(PAGE_SIZE_X, PAGE_SIZE_Y), VECTOR2(0, 0), pageImage, lpImageMng.GetID("image/キャラセレ用/PageSerect.png")[0], 1.5f, true);

				mode = MODE_SCENE_BACK;
				// SE
				lpSoundMng.SetSound("ｷｬﾝｾﾙSE");
				break;
			}
		}
		// ｽﾀｰﾄﾎﾞﾀﾝ
		if (ctrl.GetInputDown(PAD_INPUT_8, n))
		{
			if (endFlag)
			{
				if (stageID == STAGE_RANDUM)
				{
					stageID = static_cast<STAGE_ID>((rand() % (static_cast<int>(STAGE_MAX) - 1)));
				}

				lpSoundMng.SetSound("ワープ");
				mode = MODE_SCENE_NEXT;
			}
		}
		// Aﾎﾞﾀﾝ
		if (ctrl.GetInputDown(GPAD_INPUT_A, n))
		{
			if (!endFlag)
			{
				flashDrawCnt = 0;
				endFlag = true;

				if (stageID != STAGE_RANDUM)
				{
					// SE
					lpSoundMng.SetSound("決定SE");
				}
				else
				{
					// SE
					lpSoundMng.SetSound("ドドン");
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
				lpSoundMng.SetSound("ｷｬﾝｾﾙSE");
			}
		}
	}

	Draw();

	return move(own);	// ｼｰﾝ継続
}

bool StageSelectScene::PageDraw()
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

bool StageSelectScene::Draw(void)
{
	ClsDrawScreen();

	// 背景
	DrawGraph(0, 0, lpImageMng.GetID("image/ステージセレクト用/StageSelect.png")[0], true);
	
	DrawGraph(50, 60, lpImageMng.GetID("image/ステージセレクト用/NamePlate.jpg")[0], true);

	DrawStringToHandle(10, 5, "ステージセレクト", 0xff0000, lpFontMng.SetFont("たぬき40縁"));

	DrawExtendGraph(stageTbl[tblPos.y][tblPos.x].startPos.x,
		stageTbl[tblPos.y][tblPos.x].startPos.y,
		stageTbl[tblPos.y][tblPos.x].endPos.x,
		stageTbl[tblPos.y][tblPos.x].endPos.y,
		stageTbl[tblPos.y][tblPos.x].imageR, true);
	
	// 左側の画像
	DrawGraph(0, 210, stageTbl[tblPos.y][tblPos.x].imageL, true);

	DrawGraph(25, 507, lpImageMng.GetID("image/ステージセレクト用/Comment.png")[0], true);

	// 文字幅を取り左中央に配置

	auto SelectText = [&](STAGE_ID ID, const TCHAR* name, const TCHAR* text)
	{
		if (stageDrawID == ID)
		{
			if (stageDrawID != STAGE_RANDUM)
			{
				DrawStringToHandle(
					(600 - GetDrawStringWidthToHandle(static_cast<string>(name).c_str(), static_cast<int>(strlen(name)),
						lpFontMng.SetFont("たぬき60"))) / 2,
					95,
					name, 0x000000, lpFontMng.SetFont("たぬき60"));
				// 説明文
				DrawStringToHandle(
					40, 517,
					text, 0xffffff, lpFontMng.SetFont("たぬき25縁"));

			}
			else
			{
				DrawGraph(
					149,
					70,
					lpImageMng.GetID("image/キャラセレ用/ランダム.png")[0], true);
				// 説明文
				DrawStringToHandle(
					40, 557,
					"ロマン溢れる選択", 0xff0000, lpFontMng.SetFont("EP80"));
			}
		}
	};

	SelectText( STAGE_SCHOOL,
				"がっこー",
				"さんにんがかよっているしょうがっこう\n"
				"ホームルームかいし10ぷんまえになると\n"
				"こうもんにしないをもった\n"
				"たけうちせんせいがたっている");
	
	SelectText( STAGE_TREE_OF_LEGEND,
				"伝説の樹",
				"がっこうのこうていにはえているき\n"
				"このきのしたでできたカップルは\n"
				"むすばれるといううわさが\n"
				"あったりなかったりラジバンダリ・・・");
	
	SelectText( STAGE_ARCADE,
				"しょーてんがい",
				"こえのおおきいやおやさん\n"
				"いつもやさしいおもちゃやさん\n"
				"そしてほうかごのこどもたちに\n"
				"だいにんきのだがしやさんがある");

	SelectText( STAGE_RANDUM,
				"ランダム",
				"\n"
				"プレイするステージが\n"
				"ランダムできまるよ！");

	// ｹﾞｰﾑｼｰﾝ移行時に表示しない
	if (mode != MODE_SCENE_NEXT)
	{
		// 点滅処理
		flashDrawCnt++;

		if (!endFlag)
		{
			// 矢印の描画
			for (int i = 0; i <= 1; i++)
			{
				DrawRotaGraph(
					dirPosTbl[stageID][i].x + cursor.x,
					dirPosTbl[stageID][i].y + cursor.y,
					1.0, dirRotaTbl[stageID][i], lpImageMng.GetID("image/キャラセレ用/やじるしR.png")[0], true);
			}
		}
		if (flashDrawCnt / FLASING_LOW_CNT % 2 == 0)
		{
			string str;
			str = (endFlag ? "スタートボタンでゲーム開始" : "スタートボタンでステージ決定");
			/*DrawStringToHandle(270, 0, str.c_str(), 0xff0000, lpFontMng.SetFont("たぬき50縁"));*/
			if (!endFlag)
			{
				DrawStringToHandle(900, 5, "Ａ:けってい", 0xffffff, lpFontMng.SetFont("たぬき30縁"));
			}
			else
			{
				std::string StageName = ("スタートボタンでバトルかいし！");
				int len = static_cast<int>(strlen("スタートボタンでバトルかいし！"));

				DrawStringToHandle(
					(SCREEN_SIZE_X - GetDrawStringWidthToHandle(StageName.c_str(), len, lpFontMng.SetFont("たぬき60縁"))) / 2,
					SCREEN_SIZE_Y / 2 - 30,
					"スタートボタンでバトルかいし！", 0xff0000, lpFontMng.SetFont("たぬき60縁"));
			}

			// 常に表示
			DrawStringToHandle(1100, 5, "Ｂ:もどる", 0xffffff, lpFontMng.SetFont("たぬき30縁"));
		}
	}

	// ﾌﾗｯｼｭ
	if (mode == MODE_SCENE_NEXT)
	{
		DrawRotaGraph(VECTOR2(cursor.x, cursor.y), flashCnt, flashCnt,
			lpImageMng.GetID("image/ステージセレクト用/Flash.png")[0], true);
	}

	//---------------------------------------------------------------------------------------
	SetDrawScreen(DX_SCREEN_BACK);						// tmpBufferに描画していた(SceneMngのSysInitで設定)のをbackScreenに戻す
	ClsDrawScreen();
	DrawGraph(0, 0, lpSceneMng.GetTmpBuffer(), true);	//backScreenに今までの描画(tmpBuffer)をする ここで画面効果を追加できる
	SetDrawScreen(lpSceneMng.GetTmpBuffer());			// tmpBufferに描画するように変更														//--------------------------------------------------------------------------------------

	ScreenFlip();

	return true;
}

int StageSelectScene::Init(void)
{
	//SE･BGMの登録
	lpSoundMng.SoundInit("sound/Select.mp3", "ｾﾚｸﾄBGM");
	lpSoundMng.SoundInit("sound/SE類/Warp.mp3", "ワープ",80);
	lpSoundMng.SoundInit("sound/SE類/DecisionSE.mp3", "決定SE");
	lpSoundMng.SoundInit("sound/SE類/SelectSE.mp3", "ｶｰｿﾙSE");
	lpSoundMng.SoundInit("sound/SE類/CancelSE.mp3", "ｷｬﾝｾﾙSE");
	lpSoundMng.SoundInit("sound/SE類/太鼓1.mp3", "ドン");
	lpSoundMng.SoundInit("sound/SE類/太鼓2.mp3", "ドドン");

	if (!lpSoundMng.CheckPlaySound("ｾﾚｸﾄBGM"))
	{
		// BGMの再生
		lpSoundMng.SetSound("ｾﾚｸﾄBGM", true);
	}

	gameSceneImage = {
		lpImageMng.GetID("image/ゲームシーン用/School.png")[0],
		lpImageMng.GetID("image/ゲームシーン用/TreeOfLegend.png")[0],
		lpImageMng.GetID("image/ゲームシーン用/Arcade.png")[0],
	};

	stageTbl[0][0] = {
		STAGE_SCHOOL,			
		VECTOR2(800,200),	
		lpImageMng.GetID("image/ステージセレクト用/LeftSchool.png")[0],
		lpImageMng.GetID("image/ステージセレクト用/SelectSchool.png")[0],
		VECTOR2(680, 30),
		VECTOR2(930, 380),
	}; 
	stageTbl[0][1] = {
		STAGE_TREE_OF_LEGEND,	
		VECTOR2(1110,190),	
		lpImageMng.GetID("image/ステージセレクト用/LeftWood.png")[0],
		lpImageMng.GetID("image/ステージセレクト用/SelectWood.png")[0],
		VECTOR2(930, 70),
		VECTOR2(1280, 320),
	};
	stageTbl[1][0] = {
		STAGE_ARCADE,			
		VECTOR2(850,520),	
		lpImageMng.GetID("image/ステージセレクト用/LeftArcade.png")[0],
		lpImageMng.GetID("image/ステージセレクト用/SelectArcade.png")[0],
		VECTOR2(680, 400),
		VECTOR2(1010, 630),
	};
	stageTbl[1][1] = {
		STAGE_RANDUM,			
		VECTOR2(1160,522),	
		lpImageMng.GetID("image/ステージセレクト用/random.png")[0],
		lpImageMng.GetID("image/ステージセレクト用/SelectRandom.png")[0],
		VECTOR2(1040, 350),
		VECTOR2(1270, 690),
	};

	dirPosTbl = {
		// X軸				// Y軸
		 VECTOR2(130,0),	VECTOR2(0,170),
		 VECTOR2(-170,0),	VECTOR2(0,125),
		 VECTOR2(160,0),	VECTOR2(0,-120),
		 VECTOR2(-120,0),	VECTOR2(0,-170) 
	};
	dirRotaTbl = {
		// X軸				// Y軸
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

	// ﾌｫﾝﾄの登録
	lpFontMng.FontInit("font/TanukiMagic.ttf", "たぬき油性マジック", "たぬき50", 50, 9, false, false);
	lpFontMng.FontInit("font/TanukiMagic.ttf", "たぬき油性マジック", "たぬき60", 60, 9, false, false);
	lpFontMng.FontInit("font/TanukiMagic.ttf", "たぬき油性マジック", "たぬき25縁", 25, 9, true, false);
	lpFontMng.FontInit("font/TanukiMagic.ttf", "たぬき油性マジック", "たぬき40縁", 40, 9, true, false);
	lpFontMng.FontInit("font/TanukiMagic.ttf", "たぬき油性マジック", "たぬき50縁", 50, 9, true, false);
	lpFontMng.FontInit("font/TanukiMagic.ttf", "たぬき油性マジック", "たぬき60縁", 60, -1, true, false);

	return 0;
}