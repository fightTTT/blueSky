#include "DxLib.h"
#include "SoundMng.h"
#include "SelectScene.h"
#include "SceneMng.h"
#include "GameScene.h"
#include "StageSelectScene.h"

// 定義
#define MARGIN_X				(90)	// 画面余白X
#define MARGIN_Y				(50)	// 画面余白Y
#define SELECT_SCREEN_SIZE_X	(460)	// 各ﾌﾟﾚｲﾔｰのｳｨﾝﾄﾞｳｻｲｽﾞX
#define SELECT_SCREEN_SIZE_Y	(310)	// 各ﾌﾟﾚｲﾔｰのｳｨﾝﾄﾞｳｻｲｽﾞY
#define SELECT_ICON_X			(144)	// 選択ｱｲｺﾝのｻｲｽﾞ
#define SELECT_ICON_Y			(576)	// 選択ｱｲｺﾝのｻｲｽﾞ
#define CHAR_ICON_X				(180)	// ｷｬﾗｸﾀｰｻｲｽﾞX
#define PLAYER_CURSOR			(76)	// ﾌﾟﾚｲﾔｰのｶｰｿﾙｻｲｽﾞX
#define CURSOR_FARST_POS		((SCREEN_SIZE_Y - SELECT_ICON_Y)/2)	// ｶｰｿﾙ初期位置
#define N_CFONT_SIZE_X			(140)	// 途中参加可能ﾌｫﾝﾄの始まりの位置X
#define N_CFONT_SIZE_Y			(150)	// 途中参加可能ﾌｫﾝﾄの始まりの位置Y

#define PAGE_SIZE_X	1280
#define PAGE_SIZE_Y 720

#define DIR_SIZE_X	60					// やじるし画像の大きさ
#define DIR_SIZE_Y	60					// やじるし画像の大きさ

#define FADE_SPEED		180		// ﾌｪｰﾄﾞｲﾝが完了するまでの時間(ﾌﾚｰﾑ)

SelectScene::SelectScene(bool whiteInFlag)
{
	alpha = whiteInFlag ? 255 : 0;
	// 透明度をFADE_SPEED(ﾌﾚｰﾑ)かけて最大(255)にする
	alpha.set(0, FADE_SPEED);		
	joinPlayerNum = GetJoypadNum();
	Init();
}

SelectScene::~SelectScene()
{
}

unique_Base SelectScene::Update(unique_Base own, const GameController & controller)
{
	// ﾊﾟｯﾄﾞ接続数の更新
	joinPlayerNum = GetJoypadNum();
	// ｺﾝﾄﾛｰﾗｰ情報の格納
	this->controller = controller;

	if (alpha)
	{
		// 描画処理
		Draw();

		return std::move(own);	// ｼｰﾝ継続
	}

	if (mode == MODE_SCENE_NEXT)
	{
		if (PageDraw())
		{
			// 画面遷移
			return std::make_unique<StageSelectScene>(playerIdArray, SelectEndNum());
		}

		return std::move(own);	// ｼｰﾝ継続
	}

	for (int k = 0; k < PLAYER_MAX; k++)
	{
		// ｽﾀｰﾄﾎﾞﾀﾝで参加
		if (controller.GetInputDown(PAD_INPUT_8, k))
		{
			// ﾌﾟﾚｲﾔｰの参加
			if (player[k].ID == ID_NON)
			{

				// SEの再生
				lpSoundMng.SetSound("参戦SE");

				// 初期は傘に
				player[k].ID = ID_Umb;
				player[k].posY = CURSOR_FARST_POS;
			}

		}

		// 選択処理
		if (player[k].ID != ID_NON && !player[k].flag)
		{
			if (controller.GetInputDown(PAD_INPUT_LEFT, k))
			{

				// 選択ｷｬﾗの変更
				player[k].ID =
					(CHAR_ID)(static_cast<int>(player[k].ID) <= static_cast<int>(CHAR_ID::ID_Umb)
						? static_cast<int>(CHAR_ID::ID_Rnd)
						: static_cast<int>(player[k].ID) - 1);

				if (player[k].ID != ID_Rnd)
				{
					// SE
					lpSoundMng.SetSound("ｶｰｿﾙSE");
				}
				else
				{
					// SE
					lpSoundMng.SetSound("ドン");
				}

				//player[k].posY -= SELECT_ICON_X;
			}
			if (controller.GetInputDown(PAD_INPUT_RIGHT, k))
			{

				// 下方向
				// 選択ｷｬﾗの変更
				player[k].ID =
					(CHAR_ID)(static_cast<int>(player[k].ID) >= static_cast<int>(CHAR_ID::ID_Rnd)
						? static_cast<int>(CHAR_ID::ID_Umb)
						: static_cast<int>(player[k].ID) + 1);

				if (player[k].ID != ID_Rnd)
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
		// 枠の移動
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
			// 決定
			if (controller.GetInputDown(GPAD_INPUT_A, k))
			{
				if (!player[k].flag)
				{
					if (player[k].ID != ID_Rnd)
					{
						// SE
						lpSoundMng.SetSound("決定SE");
					}
					else
					{
						// SE
						lpSoundMng.SetSound("ドドン");
					}
					// ﾌﾟﾚｲﾔｰｷｬﾗ決定
					player[k].flag = true;
				}
			}
			// 取り消し
			if (controller.GetInputDown(GPAD_INPUT_B, k))
			{
				if (player[k].flag)
				{
					// SE
					lpSoundMng.SetSound("ｷｬﾝｾﾙSE");

					// 未決定
					player[k].flag = false;
				}
				else
				{
					// SE
					lpSoundMng.SetSound("ｷｬﾝｾﾙSE");

					// ﾌﾟﾚｲﾔｰをNONに(不参加)
					player[k].ID = ID_NON;
				}
			}
		}

		// ｷｬﾗを選択し終わった人数が2以上の時にｹﾞｰﾑを開始できる(この状態での途中参加もできる)
		// ﾃﾞﾊﾞｯｸﾞ中は消す
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

					turnThePage = TurnThePage(VECTOR2(PAGE_SIZE_X, PAGE_SIZE_Y), VECTOR2(0, 0), selectSceneImage, lpImageMng.GetID("image/キャラセレ用/シーン移行用.png")[0], 1.0f);
					mode = MODE_SCENE_NEXT;
				}
			}
		}
	}

	// 描画処理
	Draw();

	return std::move(own);	// ｼｰﾝ継続
}

bool SelectScene::PageDraw(void)
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

bool SelectScene::Draw(void)
{
	ClsDrawScreen();

	// 背景
	DrawGraph(0, 0, lpImageMng.GetID("image/キャラセレ用/Select.png")[0], true);

	// ｾﾚｸﾄｱｲｺﾝ
	DrawGraph((SCREEN_SIZE_X - SELECT_ICON_X) / 2, (SCREEN_SIZE_Y - SELECT_ICON_Y) / 2, lpImageMng.GetID("image/キャラセレ用/CharaSelect.png")[0], true);

	for (int k = 0; k < PLAYER_MAX; k++)
	{
		// 点滅用ｶｳﾝﾄ
		player[k].cnt++;

		if (player[k].ID != ID_NON)
		{
			// ﾃｷｽﾄ
			auto SelectText = [&](int color, CHAR_ID ID, const TCHAR* ruby, const TCHAR* name, const TCHAR* wepon, const TCHAR* comment, const TCHAR* specialRuby, const TCHAR* special, const TCHAR* specialText)
			{
				if (player[k].ID == ID)
				{
					// 参加枠
					DrawGraph(
						MARGIN_X + ((SCREEN_SIZE_X / 2) * (k % 2)),
						MARGIN_Y + ((SELECT_SCREEN_SIZE_Y) * (k / 2)),
						lpImageMng.GetID("image/キャラセレ用/NowSelect.png")[0], true);

					if (player[k].ID != ID_Rnd)
					{
						DrawStringToHandle(
							(MARGIN_X + 700 + ((MARGIN_X * 9 + SELECT_SCREEN_SIZE_X + 10) * (k % 2)) - GetDrawStringWidthToHandle(static_cast<string>("ぶき").c_str(), static_cast<int>(strlen(static_cast<const char*>("ぶき"))), lpFontMng.SetFont("たぬき15"))) / 2,
							70 - 10 + ((SELECT_SCREEN_SIZE_Y) * (k / 2)),
							"ぶき", 0x000000, lpFontMng.SetFont("たぬき15"));

						DrawStringToHandle(
							(MARGIN_X + 300 + ((MARGIN_X * 2 + SELECT_SCREEN_SIZE_X) * (k % 2))) - GetDrawStringWidthToHandle(static_cast<string>("ひっさつわざ").c_str(), static_cast<int>(strlen(static_cast<const char*>("ひっさつわざ"))), lpFontMng.SetFont("たぬき20縁")) / 2,
							240 - 23 + ((SELECT_SCREEN_SIZE_Y) * (k / 2)),
							"ひっさつわざ", 0xff0000, lpFontMng.SetFont("たぬき20縁"));
					}
					else
					{
						DrawVStringToHandle(
							(540 + ((MARGIN_X * 2 + SELECT_SCREEN_SIZE_X) * (k % 2))) - GetDrawStringWidthToHandle(static_cast<string>("ひっさつわざ").c_str(), static_cast<int>(strlen(static_cast<const char*>("ひっさつわざ"))), lpFontMng.SetFont("たぬき20縁")) / 2,
							MARGIN_Y + 5 + ((SELECT_SCREEN_SIZE_Y) * (k / 2)),
							"勝負は既に\n　始まっている", 0xff0000, lpFontMng.SetFont("EP60"));
					}

					DrawStringToHandle(
						(MARGIN_X + 275 + ((MARGIN_X * 9 + SELECT_SCREEN_SIZE_X + 10) * (k % 2)) - GetDrawStringWidthToHandle(static_cast<string>(ruby).c_str(), static_cast<int>(strlen(static_cast<const char*>(ruby))), lpFontMng.SetFont("たぬき10"))) / 2,
						80 + ((SELECT_SCREEN_SIZE_Y) * (k / 2)),
						ruby, 0x000000, lpFontMng.SetFont("たぬき10"));

					DrawStringToHandle(
						(MARGIN_X + 275 + ((MARGIN_X * 9 + SELECT_SCREEN_SIZE_X + 10) * (k % 2)) - GetDrawStringWidthToHandle(static_cast<string>(name).c_str(), static_cast<int>(strlen(static_cast<const char*>(name))), lpFontMng.SetFont("たぬき20"))) / 2,
						90 + ((SELECT_SCREEN_SIZE_Y) * (k / 2)),
						name, 0x000000, lpFontMng.SetFont("たぬき20"));

					DrawStringToHandle(
						(MARGIN_X + 700 + ((MARGIN_X * 9 + SELECT_SCREEN_SIZE_X + 10) * (k % 2)) - GetDrawStringWidthToHandle(static_cast<string>(wepon).c_str(), static_cast<int>(strlen(static_cast<const char*>(wepon))), lpFontMng.SetFont("たぬき40"))) / 2,
						90 - 10 + ((SELECT_SCREEN_SIZE_Y) * (k / 2)),
						wepon, color, lpFontMng.SetFont("たぬき40"));

					DrawStringToHandle(
						MARGIN_X + 15 + (SELECT_SCREEN_SIZE_X / 3) + ((MARGIN_X * 2 + SELECT_SCREEN_SIZE_X) * (k % 2)),
						140 - 15 + ((SELECT_SCREEN_SIZE_Y) * (k / 2)),
						comment, 0x000000, lpFontMng.SetFont("たぬき18"));

					DrawStringToHandle(
						(MARGIN_X + 300 + ((MARGIN_X * 2 + SELECT_SCREEN_SIZE_X) * (k % 2))) - GetDrawStringWidthToHandle(static_cast<string>(specialRuby).c_str(), static_cast<int>(strlen(static_cast<const char*>(specialRuby))), lpFontMng.SetFont("たぬき20")) / 2,
						270 - 25 + ((SELECT_SCREEN_SIZE_Y) * (k / 2)),
						specialRuby, 0x000000, lpFontMng.SetFont("たぬき20"));

					DrawStringToHandle(
						(MARGIN_X + 300 + ((MARGIN_X * 2 + SELECT_SCREEN_SIZE_X) * (k % 2))) - GetDrawStringWidthToHandle(static_cast<string>(special).c_str(), static_cast<int>(strlen(static_cast<const char*>(special))), lpFontMng.SetFont("たぬき25")) / 2,
						290 - 25 + ((SELECT_SCREEN_SIZE_Y) * (k / 2)),
						special, color, lpFontMng.SetFont("たぬき25"));

					//DrawStringToHandle(
					//( MARGIN_X + 300 + ( (MARGIN_X * 2 + SELECT_SCREEN_SIZE_X) * (k % 2) ) ) - GetDrawStringWidthToHandle( static_cast<string>(specialText).c_str(), static_cast<int>( strlen( static_cast<const char*>(specialText) ) ), lpFontMng.SetFont("たぬき18") )/2 ,
					//320 -25 + ( (SELECT_SCREEN_SIZE_Y) * (k / 2) ),
					//specialText, 0x000000, lpFontMng.SetFont("たぬき18") );
					DrawStringToHandle(
						MARGIN_X + 15 + (SELECT_SCREEN_SIZE_X / 3) + ((MARGIN_X * 2 + SELECT_SCREEN_SIZE_X) * (k % 2)),
						320 - 25 + ((SELECT_SCREEN_SIZE_Y) * (k / 2)),
						specialText, 0x000000, lpFontMng.SetFont("たぬき18"));
				}
			};

			SelectText(0x0000ff,
				ID_Umb,
				"あめの こうた",
				"雨野降太",
				"かさ",
				"スタンダードで、たたかいやすい\n"
				"くうちゅうで上こうげきをすると\n"
				"ふわふわするよ！\n"
				"しょしんしゃむけ",
				"乱れ月",
				"ーThe Moon Thrustー",
				"めにもとまらぬはやさのつきと\n"
				"かいてんぎりをする\n"
				"(前方の敵に大ダメージ)");

			SelectText(0xff00ff,
				ID_Bro,
				"たけべ ほうき",
				"竹部箒",
				"ほーき",
				"リーチはながいけど\n"
				"こうげきりょくはひくい\n"
				"ジャンプが１回おおくできるぞ！"
				"\n",
				"怒れるお鶏様",
				"ーRage of Chickensー",
				"まほうじんをえがき、そこから\n"
				"｢おにわとりさま｣がふってくる\n"
				"(前方の敵に足止め＆ダメージ)");

			SelectText(0xff8000,
				ID_Lid,
				"ふたば あおと",
				"双葉青砥",
				"ごみばこのふた",
				"すこしあつかいづらいけど\n"
				"ためこうげきのとっしんがつよい\n"
				"ひっさつわざでムテキになれるぞ！\n"
				"ばをあらしたいあなたにオススメ",
				"獣の咆哮",
				"ーBeast Roarー",
				"いちじてきにリミッターをはずし\n"
				"やせいのちからをかいほうする\n"
				"(一定時間無敵＆ステータス上昇)");


			//SelectText( 0x000000,
			//			ID_Rnd,
			//			"らんだむ",
			//			"ランダム",
			//			"？？？",
			//			"\n"
			//			"これをせんたくすると\n"
			//			"ランダムでキャラクターが\n"
			//			"きまるよ！",
			//			"―――",
			//			"―――――",
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

			// ｷｬﾗｱｲｺﾝ	// 未選択かﾗﾝﾀﾞﾑでない場合
			if (player[k].ID != ID_NON && player[k].ID != ID_Rnd)
			{
				std::string iconPass = "image/" + charName[player[k].ID] + "/アイコン/icon.png";
				DrawGraph(
					(SCREEN_SIZE_X / 4) - 215 + ((MARGIN_X * 2 + SELECT_SCREEN_SIZE_X) * (k % 2)),
					115 + ((SELECT_SCREEN_SIZE_Y) * (k / 2)),
					lpImageMng.GetID(iconPass)[0], true);
			}
			// ｷｬﾗｱｲｺﾝ	// ﾗﾝﾀﾞﾑ選択した場合  // ﾃﾞﾊﾞｯｸﾞ用
			if (player[k].ID == ID_Rnd)
			{
				DrawGraph(
					MARGIN_X + 10 + ((MARGIN_X * 2 + SELECT_SCREEN_SIZE_X) * (k % 2)),
					150 + ((SELECT_SCREEN_SIZE_Y) * (k / 2)),
					lpImageMng.GetID("image/キャラセレ用/ランダム.png")[0], true);
			}

			// 1P,2Pなどの表示
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


			// 全員が不参加状態でない場合常に表示する
			if (player[k].cnt / FLASING_LOW_CNT % 2 == 0)
			{
				DrawStringToHandle(1100, 5, "Ｂ:もどる", 0xffffff, lpFontMng.SetFont("たぬき30縁"));
			}

			// ｷｬﾗｸﾀｰを決めたとき
			if (player[k].flag)
			{
				if (player[k].cnt / FLASING_HIGH_CNT % 2 == 0)
				{
					if (player[k].ID != ID_Rnd)
					{
						// 決定ﾃｷｽﾄ
						DrawStringToHandle(
							(MARGIN_X + 20) + ((SCREEN_SIZE_X / 2) * (k % 2)),
							(SCREEN_SIZE_Y / 2) - 40 + ((SELECT_SCREEN_SIZE_Y) * (k / 2)),
							"じゅんびＯＫ", 0xFF0000, lpFontMng.SetFont("たぬき20縁"));
					}
					else
					{
						DrawStringToHandle(
							(MARGIN_X + 160) + ((SCREEN_SIZE_X / 2) * (k % 2)),
							(SCREEN_SIZE_Y / 2) - 40 + ((SELECT_SCREEN_SIZE_Y) * (k / 2)),
							"覚悟完了！", 0xFF0000, lpFontMng.SetFont("EP25"));
					}
				}
			}
			else
			{
				//// 決定ﾃｷｽﾄ
				//DrawStringToHandle(
				//	(SCREEN_SIZE_X / 5 - 30) + ((SCREEN_SIZE_X / 2) * (k % 2)),
				//	(SCREEN_SIZE_Y / 2) - 40 + ((SELECT_SCREEN_SIZE_Y) * (k / 2)),
				//	"Ａボタンでけってい", 0xFF0000, lpFontMng.SetFont("たぬき20"));

				// 左右キーを押した瞬間だけ画像を1.5倍に拡大する
				double dirExtRateL = (controller.GetInputDown(PAD_INPUT_LEFT, k)) ? 1.1 : 1.0;
				double dirExtRateR = (controller.GetInputDown(PAD_INPUT_RIGHT, k)) ? 1.1 : 1.0;

				// 矢印L
				DrawRotaGraph(
					MARGIN_X - 45 + ((SCREEN_SIZE_X / 2) * (k % 2)) + DIR_SIZE_X / 2,
					MARGIN_Y - 30 + ((SELECT_SCREEN_SIZE_Y / 2)) + ((SELECT_SCREEN_SIZE_Y) * (k / 2)) + DIR_SIZE_Y / 2,
					dirExtRateL, 0.0,
					lpImageMng.GetID("image/キャラセレ用/やじるしL.png")[0], true);
				// 矢印R
				DrawRotaGraph(
					MARGIN_X - 45 + ((SCREEN_SIZE_X / 2) * (k % 2)) + DIR_SIZE_X / 2 + 490,
					MARGIN_Y - 30 + ((SELECT_SCREEN_SIZE_Y / 2)) + ((SELECT_SCREEN_SIZE_Y) * (k / 2)) + DIR_SIZE_Y / 2,
					dirExtRateR, 0.0,
					lpImageMng.GetID("image/キャラセレ用/やじるしR.png")[0], true);

				if (player[k].cnt / FLASING_LOW_CNT % 2 == 0)
				{
					// ﾃｷｽﾄ
					DrawStringToHandle(900, 5, "Ａ:けってい", 0xffffff, lpFontMng.SetFont("たぬき30縁"));
				}
			}
		}
		else
		{
			//// 不参加枠
			//DrawGraph(
			//	MARGIN_X + ((SCREEN_SIZE_X / 2) * (k % 2)),
			//	MARGIN_Y + ((SELECT_SCREEN_SIZE_Y) /** (k / 2)*/),
			//	lpImageMng.GetID("image/NoSelect.png")[0], true);

			// 点滅処理
			if (player[k].cnt / FLASING_LOW_CNT % 2 == 0)
			{
				// 参加可能文字
				DrawStringToHandle(
					N_CFONT_SIZE_X - 30 + ((SCREEN_SIZE_X / 2) * (k % 2)),
					N_CFONT_SIZE_Y + ((SELECT_SCREEN_SIZE_Y) * (k / 2)),
					"スタートボタンで", 0x000000, lpFontMng.SetFont("たぬき50"));

				DrawStringToHandle(
					N_CFONT_SIZE_X + ((SCREEN_SIZE_X / 2) * (k % 2)),
					N_CFONT_SIZE_Y + 60 + ((SELECT_SCREEN_SIZE_Y) * (k / 2)),
					"さんかできるよ", 0x000000, lpFontMng.SetFont("たぬき50"));
			}
		}

		// ﾃﾞﾊﾞｯｸﾞ
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

	// ｷｬﾗを選択し終わった人数が2以上の時に表示する
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
			// 点滅処理
			if (flashDrawCnt / FLASING_LOW_CNT % 2 == 0)
			{
				std::string StageName = ("スタートボタンをおしてね！");
				int len = static_cast<int>(strlen("スタートボタンをおしてね！"));

				DrawStringToHandle(
					(SCREEN_SIZE_X - GetDrawStringWidthToHandle(StageName.c_str(), len, lpFontMng.SetFont("たぬき60縁"))) / 2,
					SCREEN_SIZE_Y / 2 - 30, "スタートボタンをおしてね！", 0xff0000, lpFontMng.SetFont("たぬき60縁"));
			}
		}
	}
	else
	{
		flashDrawCnt = 0;
	}

	// debug-----
	//DrawFormatString(0, 0, 0xff0000, "パッドの接続数:%d", joinPlayerNum);
	/*DrawFormatString(0, 0, 0xff0000, "2人以上の参加でゲームスタートできます");*/
	// debug-----

	DrawStringToHandle(10, 5, "キャラクターセレクト", 0xff0000, lpFontMng.SetFont("たぬき40縁"));

	// ﾌｪｰﾄﾞｲﾝ
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(alpha));
	DrawBox(VECTOR2(0, 0), lpSceneMng.GetScreenSize(), 0xffffff, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	alpha.update();

	//---------------------------------------------------------------------------------------
	SetDrawScreen(DX_SCREEN_BACK);						// tmpBufferに描画していた(SceneMngのSysInitで設定)のをbackScreenに戻す
	ClsDrawScreen();
	DrawGraph(0, 0, lpSceneMng.GetTmpBuffer(), true);	//backScreenに今までの描画(tmpBuffer)をする ここで画面効果を追加できる
	SetDrawScreen(lpSceneMng.GetTmpBuffer());			// tmpBufferに描画するように変更														//--------------------------------------------------------------------------------------

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
			checkCnt++;		// ｷｬﾗ決定状態ならｶｳﾝﾄを進める
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

	//SE･BGMの登録
	lpSoundMng.SoundInit("sound/Select.mp3", "ｾﾚｸﾄBGM");
	lpSoundMng.SoundInit("sound/SE類/DecisionSE.mp3", "決定SE");
	lpSoundMng.SoundInit("sound/SE類/SelectSE.mp3", "ｶｰｿﾙSE");
	lpSoundMng.SoundInit("sound/SE類/CancelSE.mp3", "ｷｬﾝｾﾙSE");
	lpSoundMng.SoundInit("sound/SE類/Participation.mp3", "参戦SE");
	lpSoundMng.SoundInit("sound/SE類/キャラクターをえらんでね.mp3", "えらんでね");
	lpSoundMng.SoundInit("sound/SE類/太鼓1.mp3", "ドン");
	lpSoundMng.SoundInit("sound/SE類/太鼓2.mp3", "ドドン");

	// 鳴き声追加
	lpSoundMng.SoundInit("sound/SE類/Chicken1.mp3", "ｺｹｺｯｺｰ");
	lpSoundMng.SoundInit("sound/SE類/Chicken2.mp3", "ｺｹｰ");
	lpSoundMng.SoundInit("sound/SE類/Chicken3.mp3", "ｺｯｺ");
	lpSoundMng.SoundInit("sound/SE類/Chicken4.mp3", "ｺｯｺ歩き");
	lpSoundMng.SoundInit("sound/SE類/Bomb.mp3", "爆発", 85);
	lpSoundMng.SoundInit("sound/SE類/Dead.mp3", "死亡");
	lpSoundMng.SoundInit("sound/SE類/Continue.mp3", "復活", 85);

	if (!lpSoundMng.CheckPlaySound("ｾﾚｸﾄBGM"))
	{
		// BGMの再生
		lpSoundMng.SetSound("ｾﾚｸﾄBGM", true);
	}

	// 初期化
	for (int k = 0; k < PLAYER_MAX; k++)
	{
		// ｼﾞｮﾌﾞ割り当て
		player[k].ID = ID_NON;

		// 未選択
		player[k].flag = false;

		// ｱｲｺﾝ選択座標
		player[k].posY = 0;

		// 点滅用ｶｳﾝﾄ
		player[k].cnt = 0;
	}

	flashDrawCnt = 0;

	charName = {
		"選択キャラなし",	//ID_NON
		"雨野降太"		,	//ID_Umb
		"竹部箒"		,	//ID_Bro
		"双葉青砥"		,	//ID_Lid
		"エラー",	//ID_Rnd	
	};

	// ﾌｫﾝﾄの登録
	lpFontMng.FontInit("font/TanukiMagic.ttf", "たぬき油性マジック", "たぬき10", 10, -1, false, false);
	lpFontMng.FontInit("font/TanukiMagic.ttf", "たぬき油性マジック", "たぬき15", 15, -1, false, false);
	lpFontMng.FontInit("font/TanukiMagic.ttf", "たぬき油性マジック", "たぬき18", 18, -1, false, false);
	lpFontMng.FontInit("font/TanukiMagic.ttf", "たぬき油性マジック", "たぬき20", 20, -1, false, false);
	lpFontMng.FontInit("font/TanukiMagic.ttf", "たぬき油性マジック", "たぬき25", 25, -1, false, false);
	lpFontMng.FontInit("font/TanukiMagic.ttf", "たぬき油性マジック", "たぬき30", 30, -1, false, false);
	lpFontMng.FontInit("font/TanukiMagic.ttf", "たぬき油性マジック", "たぬき40", 40, -1, false, false);
	lpFontMng.FontInit("font/TanukiMagic.ttf", "たぬき油性マジック", "たぬき50", 50, 9, false, false);
	lpFontMng.FontInit("font/TanukiMagic.ttf", "たぬき油性マジック", "たぬき20縁", 20, -1, true, false);
	lpFontMng.FontInit("font/TanukiMagic.ttf", "たぬき油性マジック", "たぬき30縁", 30, -1, true, false);
	lpFontMng.FontInit("font/TanukiMagic.ttf", "たぬき油性マジック", "たぬき40縁", 40, 9, true, false);
	lpFontMng.FontInit("font/TanukiMagic.ttf", "たぬき油性マジック", "たぬき60縁", 60, -1, true, false);
	lpFontMng.FontInit("font/TanukiMagic.ttf", "たぬき油性マジック", "たぬき80縁", 80, 9, true, false);
	lpFontMng.FontInit("font/TanukiMagic.ttf", "たぬき油性マジック", "たぬき100縁", 100, 9, true, false);
	lpFontMng.FontInit("font/EPGYOBLD.TTF", "EPSON 太行書体Ｂ", "EP25", 25, 9, true, false);
	lpFontMng.FontInit("font/EPGYOBLD.TTF", "EPSON 太行書体Ｂ", "EP50", 50, 9, true, false);
	lpFontMng.FontInit("font/EPGYOBLD.TTF", "@EPSON 太行書体Ｂ", "EP60", 38, 9, true, false);
	lpFontMng.FontInit("font/EPGYOBLD.TTF", "EPSON 太行書体Ｂ", "EP80", 60, 9, true, false);
	lpFontMng.FontInit("font/EPGYOBLD.TTF", "EPSON 太行書体Ｂ", "EP100", 100, 9, true, false);

	lpFontMng.FontInit("font/AVOCADO.TTF", "アボカド", "アボカド", 10, 9, false, false);

	mode = MODE_SCENE_NOW;

	// BGMの再生
	lpSoundMng.SetSound("えらんでね");

	return 0;
}
