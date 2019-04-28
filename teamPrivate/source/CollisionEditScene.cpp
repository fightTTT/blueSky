#include "DxLib.h"
#include "SceneMng.h"
#include "GameScene.h"
#include "SelectScene.h"
#include "Gamecontroller.h"
#include "CollisionEditScene.h"
#include "TitleScene.h"

#define COL_OFFSET_X	600
#define COL_OFFSET_Y	350

#define PLAYER_SIZE_X	64
#define PLAYER_SIZE_Y	64

CollisionEditScene::CollisionEditScene()
{
	Init();
}

CollisionEditScene::~CollisionEditScene()
{
}

unique_Base CollisionEditScene::Update(unique_Base own, const GameController & controller)
{
	editCnt++;
	// ｽﾍﾟｰｽｷ-orｽﾀｰﾄﾎﾞﾀﾝでｼｰﾝ遷移
	if (controller.GetInputDown(KEY_INPUT_SPACE)||(controller.GetInputDown(PAD_INPUT_8,0)))
	{
		return std::make_unique<SelectScene>();
		//return std::make_unique<TitleScene>();
	}

	// ｴﾃﾞｨｯﾄ画像変更中
	if (changePictureFlag)
	{
		// 方向ｷｰのいずれかを押したらtrue
		bool put = false;

		// ｷｬﾗ名かｱﾆﾒｰｼｮﾝ名を変更	例(ユニティちゃん->野獣先輩)
		if (controller.GetInputDown(KEY_INPUT_DOWN))
		{
			int cnt = 0;
			for (auto itr : animNameTbl)
			{
				if (itr == NowAnimName)
				{
					tblCnt = cnt;
					break;
				}
				cnt++;
			}
			changeCnt = (changeCnt >= 2 ? 2 : changeCnt + 1);
			put = true;
		}
		if (controller.GetInputUp(KEY_INPUT_UP))
		{
			int cnt = 0;
			for (auto itr : charNameTbl)
			{
				if (itr == NowCharName)
				{
					tblCnt = cnt;
					break;
				}
				cnt++;
			}
			changeCnt = (changeCnt <= 0	? 0 : changeCnt - 1);
			put = true;
		}

		// 種類を変更				例(待機->必殺技)
		if (controller.GetInputDown(KEY_INPUT_LEFT))
		{
			tblCnt = (tblCnt <= 0 ? maxTblPtn[changeCnt] - 1 : tblCnt-1);
			put = true;
		}
		if (controller.GetInputDown(KEY_INPUT_RIGHT))
		{
			tblCnt = (tblCnt >= maxTblPtn[changeCnt] - 1 ? 0 : tblCnt+1);
			put = true;
		}

		// 方向ｷｰのいずれかを押したら
		if (put)
		{
			switch (changeCnt)
			{

			// ｷｬﾗ名変更時の処理
			case 0:
				NowCharName = charNameTbl[tblCnt];
				break;

			// ｱﾆﾒｰｼｮﾝ名変更時の処理
			case 1:
				NowAnimName = animNameTbl[tblCnt];
				NowUseDataName = dataNameTbl[tblCnt];
				break;

			// 変更終了時の処理
			case 2:
			default:
				changePictureFlag	= false;		// ｴﾃﾞｨｯﾄ画像変更を終了する
				changeCnt	= 0;
				break;
			}
			std::string LoadPass = NowCharName + "/" + NowAnimName;
			lpImageMng.LoadContinueImageId(LoadPass.c_str(), NowUseDataName);
			lpColMng.ColLoad(NowCharName, NowAnimName);

			colList.clear();
			auto colVector = lpColMng.GetCollider(NowCharName, NowAnimName);
			for (auto itr : colVector)
			{
				colList.push_back(itr);
			}
			if (colList.size())
			{
				data.offset = colList.begin()->offset;
			}
			nowIdListInit(id);
		}

	}
	else
	{
		//読み込むデータを変更する
		if (controller.GetInputDown(KEY_INPUT_F1))
		{
			int tmp = MessageBox(
				NULL,								// オーナーウィンドウのハンドル
				"設定する画像を変更しますか？",     // メッセージボックス内のテキスト
				"確認ダイアログ",					// メッセージボックスのタイトル
				MB_OKCANCEL						    // メッセージボックスのスタイル
			);

			if (tmp == IDOK)
			{
				ChangePicture();
				return std::move(own);
			}
		}

		// Aｷｰで情報の保存
		if (controller.GetInputDown(KEY_INPUT_A) && newEditFlag)
		{
			AddCol();
		}


		// Sで次のﾌﾚｰﾑへ
		if (controller.GetInputDown(KEY_INPUT_S))
		{
			while (nowIdList.size())
			{
				colList.push_back(*nowIdList.begin());
				nowIdList.pop_front();
			}
			(id < lpImageMng.GetSize(NowCharName + "/" + NowAnimName) - 1 ? id++ : id = 0);	// 次の画像へ
			nowIdListInit(id);
			data.id = id;

			

			// 次のidのcolListが存在するなら画像offset値を書き換える
			if (colList.size() == 0)
			{
				data.offset = { 0,0 };
			}
			else
			{
				auto itr = colList.begin();		// itrを先頭に設定
				auto nextItr = colList.begin();
				nextItr++;

				while (true)
				{
					if (nextItr == colList.end())
					{
						data.offset = itr->offset;
						break;
					}

					if (itr->id == id)
					{
						if (nextItr->id != id)	// ﾌﾚｰﾑが切り替わった時
						{
							data.offset = nextItr->offset;
							break;
						}
					}
					itr++;
					nextItr++;
				}
			}
		}

		// 既存ｺﾗｲﾀﾞｰ編集中にDｷｰを押すと動ﾌﾚｰﾑ内の次のｺﾘｼﾞｮﾝになる
		if (controller.GetInputDown(KEY_INPUT_D) && !newEditFlag)
		{
			if (nowIdList.size())
			{
				auto last = nowIdList.end();
				last--;
				if (editItr == last)
				{
					editItr = nowIdList.begin();
				}
				else
				{
					editItr++;
				}
			}
		}

		// Cｷｰで表示中のｺﾗｲﾀﾞｰ削除
		if (controller.GetInputDown(KEY_INPUT_C))
		{
			nowIdList.clear();
			newEditFlag = true;
		}

		// Eｷｰでｴﾃﾞｨｯﾄﾓｰﾄﾞを変更
		if (controller.GetInputDown(KEY_INPUT_E))
		{
			newEditFlag = !newEditFlag;
			while (nowIdList.size())
			{
				colList.push_back(*nowIdList.begin());
				nowIdList.pop_front();
			}

			nowIdListInit(id);
		}

		// 左ｸﾘｯｸでstartPosの決定
		if (controller.GetMouseInputDown(MOUSE_INPUT_LEFT))
		{
			data.startPos = controller.GetMousePos();
		}

		// Bﾎﾞﾀﾝで矩形と円形の切り替え
		if (controller.GetInputDown(KEY_INPUT_B))
		{
			data.boxFlag = !(data.boxFlag);
		}

		// Wﾎﾞﾀﾝで武器とﾌﾟﾚｲﾔの切り替え
		if (controller.GetInputDown(KEY_INPUT_W))
		{
			data.weponFlag = (data.weponFlag == COL_TYPE_PLAYER ? COL_TYPE_WEAPON : COL_TYPE_PLAYER);
			if (data.weponFlag == COL_TYPE_PLAYER)
			{
				data.attackPoint = 0;
				data.angryPoint = 0;
				data.specialPoint = 0;
			}
		}

		if (newEditFlag)
		{
			if (data.weponFlag == COL_TYPE_WEAPON)
			{
				// 8  9 ｷｰで攻撃力の設定
				if (controller.GetInputDown(KEY_INPUT_NUMPAD8)) { data.attackPoint++; }
				if (controller.GetInputDown(KEY_INPUT_NUMPAD9)) { data.attackPoint--; }
				// 5  6 ｷｰで怒り力の設定
				if (controller.GetInputDown(KEY_INPUT_NUMPAD5)) { data.angryPoint++; }
				if (controller.GetInputDown(KEY_INPUT_NUMPAD6)) { data.angryPoint--; }
				// 2  3 ｷｰで必殺力の設定
				if (controller.GetInputDown(KEY_INPUT_NUMPAD2)) { data.specialPoint++; }
				if (controller.GetInputDown(KEY_INPUT_NUMPAD3)) { data.specialPoint--; }
			}
		}
		else
		{
			if ((*editItr).weponFlag == COL_TYPE_WEAPON)
			{
				// 8  9 ｷｰで攻撃力の設定
				if (controller.GetInputDown(KEY_INPUT_NUMPAD8)) { (*editItr).attackPoint++; }
				if (controller.GetInputDown(KEY_INPUT_NUMPAD9)) { (*editItr).attackPoint--; }
				// 5  6 ｷｰで怒り力の設定
				if (controller.GetInputDown(KEY_INPUT_NUMPAD5)) { (*editItr).angryPoint++; }
				if (controller.GetInputDown(KEY_INPUT_NUMPAD6)) { (*editItr).angryPoint--; }
				// 2  3 ｷｰで必殺力の設定
				if (controller.GetInputDown(KEY_INPUT_NUMPAD2)) { (*editItr).specialPoint++; }
				if (controller.GetInputDown(KEY_INPUT_NUMPAD3)) { (*editItr).specialPoint--; }
			}
		}


		// 方向ｷｰで画像ｵﾌｾｯﾄの変更
		if (controller.GetInputDown(KEY_INPUT_UP))
		{
			data.offset.y -= 1;
		}
		if (controller.GetInputDown(KEY_INPUT_DOWN))
		{
			data.offset.y += 1;
		}
		if (controller.GetInputDown(KEY_INPUT_RIGHT))
		{
			data.offset.x += 1;
		}
		if (controller.GetInputDown(KEY_INPUT_LEFT))
		{
			data.offset.x -= 1;
		}

		if (controller.GetInputDown(KEY_INPUT_F6))
		{
			//下にあるのが元々のコードです(一時的に増田システムに切り替えています)
			//ColSave();

			int tmp = MessageBox(
				NULL,								// オーナーウィンドウのハンドル
				"現在の登録データを保存しますか？",     // メッセージボックス内のテキスト
				"確認ダイアログ",					// メッセージボックスのタイトル
				MB_OKCANCEL						    // メッセージボックスのスタイル
			);

			if (tmp == IDOK)
			{
				ColSave(NowCharName, NowAnimName);
			}
		}

		if (controller.GetInputDown(KEY_INPUT_F5))
		{
			//下にあるのが元々のコードです(一時的に増田システムに切り替えています)
			//lpColMng.ColLoad();

			//for (int i = 0; i < lpColMng.colMap["待機"].size(); i++)
			//{
			//	colList.push_back(lpColMng.colMap["待機"][i]);
			//}

			int tmp = MessageBox(
				NULL,								// オーナーウィンドウのハンドル
				"保存されているデータをロードしますか？",     // メッセージボックス内のテキスト
				"確認ダイアログ",					// メッセージボックスのタイトル
				MB_OKCANCEL						    // メッセージボックスのスタイル
			);

			if (tmp == IDOK)
			{
				lpColMng.ColLoad(NowCharName, NowAnimName);

				for (int i = 0; i < lpColMng.GetCollider(NowCharName, NowAnimName).size(); i++)
				{
					colList.push_back(lpColMng.GetCollider(NowCharName, NowAnimName)[i]);
				}
			}
			data.offset = colList.begin()->offset;	// 画像ｵﾌｾｯﾄ値を書き換える
		}

		data.r = static_cast<int>(sqrt((nowPos.x - data.startPos.x) * (nowPos.x - data.startPos.x) + (nowPos.y - data.startPos.y) * (nowPos.y - data.startPos.y)));
		nowPos = controller.GetMousePos();
	}
	Draw();
	return std::move(own);
}

void CollisionEditScene::Draw(void)
{
	ClsDrawScreen();

	VECTOR2 offset = { COL_OFFSET_X , COL_OFFSET_Y };

	// 罫線部分にDrawBoxで色を塗る
	DrawBox(VECTOR2(COL_OFFSET_X - 200, COL_OFFSET_Y - 200), VECTOR2(COL_OFFSET_X + 200, COL_OFFSET_Y + 200), 0x00ff00, true);

	// 罫線の描画
	DrawGrid();

	int colColor = (data.weponFlag == COL_TYPE_PLAYER ? 0x0000ff : 0xff0000);

	// 画像の描画
	if (lpImageMng.GetSize(NowCharName + "/" + NowAnimName))
	{
		DrawRotaGraph(
			data.offset.x + COL_OFFSET_X,
			data.offset.y + COL_OFFSET_Y,
			1.0,
			0,
			lpImageMng.GetID(NowCharName + "/" + NowAnimName)[id],
			true);
	}
	else
	{
		DrawCircle(COL_OFFSET_X, COL_OFFSET_Y,100,0xff0000);
	}

	// 当たり判定範囲の描画
	if (newEditFlag)
	{
		if (data.boxFlag)
		{
			DrawBox(data.startPos, nowPos, colColor, false);
		}
		else
		{
			DrawCircle(data.startPos, data.r, colColor, false);
		}
	}

	// 同ﾌﾚｰﾑで保存した当たり判定範囲の描画
	for (auto col = nowIdList.begin(); col != nowIdList.end();col++)
	{
		if (nowIdList.size() == 0)
		{
			break;
		}
		int tmpColor = ((*col).weponFlag == COL_TYPE_PLAYER ? 0x0000ff : 0xff0000);

		// 編集中のｺﾗｲﾀﾞｰなら点滅させる
		if (!newEditFlag && col == editItr)
		{
			if ((editCnt / 30) % 2)
			{
				if ((*col).boxFlag)
				{
					DrawBox((*col).startPos + offset, (*col).endPos + offset, tmpColor, false);
				}
				else
				{
					DrawCircle((*col).startPos + offset, (*col).r, tmpColor, false);
				}
			}
		}
		else
		{
			if ((*col).boxFlag)
			{
				DrawBox((*col).startPos + offset, (*col).endPos + offset, tmpColor, false);
			}
			else
			{
				DrawCircle((*col).startPos + offset, (*col).r, tmpColor, false);
			}
		}
	}


	// 文字関係
	DrawInfo(colColor);

	// ﾊﾞｯﾌｧ関係----------------------------------
	SetDrawScreen(DX_SCREEN_BACK);
	ClsDrawScreen();
	DrawGraph(0, 0, lpSceneMng.GetTmpBuffer(), true);	// ここで画面効果を追加できる
	SetDrawScreen(lpSceneMng.GetTmpBuffer());
	// -------------------------------------------


	ScreenFlip();
}

bool CollisionEditScene::ColSave(void)
{
	DataHeader expData = {
		static_cast<int>(colList.size()),
	};

	colVector.resize(colList.size());

	auto itr = colList.begin();
	for (int i = 0; i < colList.size(); i++)
	{
		colVector[i] = (*itr);
		itr++;
	}

	FILE* file;

	fopen_s(&file, "colData/wait.map", "wb");		// ﾌｧｲﾙｵｰﾌﾟﾝ　開いて書き込んで閉じる

	fwrite(&expData, sizeof(expData), 1, file);		// ﾃﾞｰﾀﾍｯﾀﾞｰの書き込み
	fwrite(&colVector[0], sizeof(ColInf), colVector.size(), file);	// colVectorの中身の書き込み

	fclose(file);
	return true;
}

bool CollisionEditScene::ColSave(std::string CharName, std::string AnimName)
{
	// nowIdListの中身をcolListにぶち込む
	for (auto col = nowIdList.begin(); col != nowIdList.end();)
	{
		colList.push_back(*col);
		col = nowIdList.erase(col);
	}

	// 一つも情報が入っていなければ保存しない
	if (!colList.size())
	{
		return false;
	}

	DataHeader expData = {
		static_cast<int>(colList.size()),
	};

	colVector.resize(colList.size());

	auto itr = colList.begin();
	for (int i = 0; i < colList.size(); i++)
	{
		colVector[i] = (*itr);
		itr++;
	}

	FILE* file;


	std::string SavePass = "colData/" + CharName + "/" + AnimName + ".map";		//保存先のファイルパスを作成


	fopen_s(&file, SavePass.c_str(), "wb");		// ﾌｧｲﾙｵｰﾌﾟﾝ　開いて書き込んで閉じる

	fwrite(&expData, sizeof(expData), 1, file);		// ﾃﾞｰﾀﾍｯﾀﾞｰの書き込み
	fwrite(&colVector[0], sizeof(ColInf), colVector.size(), file);	// colVectorの中身の書き込み

	fclose(file);

	nowIdListInit(id);

	return true;
}

void CollisionEditScene::ChangePicture(void)
{
	id = 0;
	colList.clear();

	char FilePass[40];
	char DataName[40];
	char CharName[20];
	char AnimName[20];

	FilePass[0] = '\0';
	DataName[0] = '\0';
	CharName[0] = '\0';
	AnimName[0] = '\0';

	//auto DrawInfo = [&]()
	//{
	//	ClsDrawScreen();

	//	DrawFormatString(960, 490, 0x00ff00, "キャラクター名");
	//	DrawFormatString(960, 510, 0xff0000, CharName);
	//	DrawFormatString(960, 530, 0x00ff00, "アニメーション名");
	//	DrawFormatString(960, 550, 0xff0000, AnimName);
	//	DrawFormatString(960, 570, 0x00ff00, "データ名");
	//	DrawFormatString(960, 590, 0xff0000, DataName);

	//	// ﾊﾞｯﾌｧ関係----------------------------------
	//	SetDrawScreen(DX_SCREEN_BACK);
	//	ClsDrawScreen();
	//	DrawGraph(0, 0, lpSceneMng.GetTmpBuffer(), true);	// ここで画面効果を追加できる
	//	SetDrawScreen(lpSceneMng.GetTmpBuffer());
	//	// -------------------------------------------

	//	ScreenFlip();
	//};

	//DrawInfo();
	//KeyInputString(960, 510, 50, CharName, FALSE);

	//DrawInfo();
	//KeyInputString(960, 550, 50, AnimName, FALSE);

	//DrawInfo();
	//KeyInputString(960, 590, 50, DataName, FALSE);

	//NowCharName = std::string(CharName);
	//NowAnimName = std::string(AnimName);
	//NowUseDataName = std::string(DataName);

	//std::string LoadPass = NowCharName + "/" + NowAnimName;

	//lpImageMng.LoadContinueImageId(LoadPass.c_str(), NowUseDataName, animNum[NowAnimName]);

	nowPos = { 0,0 };

	ColInfoInit();	// ｺﾗｲﾀﾞｰ情報の初期化
	data.weponFlag		= COL_TYPE_PLAYER;	// 武器のｺﾗｲﾀﾞｰ　ﾌﾟﾚｲﾔのｺﾗｲﾀﾞｰ
	data.attackPoint	= 0;				// 攻撃力を0にする
	data.angryPoint		= 0;
	data.specialPoint	= 0;

	//-----------------------------------------------
	changePictureFlag	= true;
	changeCnt			= 0;
	int cnt = 0;
	for (auto itr : charNameTbl)
	{
		if (itr == NowCharName)
		{
			tblCnt = cnt;
			break;
		}
		cnt++;
	}
	//-----------------------------------------------

	return;
}

int CollisionEditScene::Init(void)
{

	NowCharName = "雨野降太";
	NowAnimName = "待機";
	NowUseDataName = "wait";

	std::string LoadPass = NowCharName + "/" + NowAnimName;

	lpImageMng.LoadContinueImageId(LoadPass.c_str(), NowUseDataName);

	colList.clear();
	auto colVector = lpColMng.GetCollider(NowCharName, NowAnimName);
	for (auto itr : colVector)
	{
		colList.push_back(itr);
	}
	if (colList.size())
	{
		data.offset = colList.begin()->offset;
	}

	editCnt = 0;
	id = 0;
	// 現在のidのｺﾗｲﾀﾞｰをcolListからnowIdListに移動させる
	nowIdListInit(id);

	nowPos = { 0,0 };

	ColInfoInit();	// ｺﾗｲﾀﾞｰ情報の初期化
	data.weponFlag		= COL_TYPE_PLAYER;	// 武器のｺﾗｲﾀﾞｰ　ﾌﾟﾚｲﾔのｺﾗｲﾀﾞｰ
	data.attackPoint	= 0;				// 攻撃力を0にする
	data.angryPoint		= 0;
	data.specialPoint	= 0;

	changePictureFlag	= false; 
	changeCnt	= 0;
	tblCnt				= 0;
	newEditFlag			= false;

	charNameTbl = {
		"雨野降太",
		"双葉青砥" ,
		"竹部箒"
	};

	animNameTbl = {
		"待機",
		"歩く",
		"走る",
		"ジャンプ始まり",
		"ジャンプ中",
		"通常攻撃1",
		"通常攻撃2",
		"通常攻撃3",
		"通常攻撃_上",
		"通常攻撃_下始まり",
		"通常攻撃_下中",
		"必殺技",
		"ガード",
		"ダメージ" ,
		"溜め攻撃",
		"溜め始まり",
		"溜め中",
		"傘浮遊",
		"怒りゲージ溜め",
		"回避",
		"死亡"
	};

	dataNameTbl = {
		"wait",
		"walk",
		"dash",
		"jumpStart",
		"jump",
		"attack",
		"attackSecond",
		"attackThird",
		"attackUp",
		"attackDownStart",
		"attackDown",
		"specialAttack",
		"guard",
		"damage",
		"chargeAttack",
		"chargeStart",
		"charge",
		"fly",
		"angryCharge",
		"avoid",
		"death"
	};

	maxTblPtn = {
		P_CHAR_MAX,
		ANIM_MAX,
		ANIM_MAX
	};

	return 0;
}

void CollisionEditScene::nowIdListInit(int id)
{
	nowIdList.clear();
	auto itr = colList.begin();
	while ( itr != colList.end())
	{
		if ((*itr).id == id)
		{
			nowIdList.push_back(*itr);
			itr = colList.erase(itr);
		}
		else
		{
			itr++;
		}
	}

	if (nowIdList.size())
	{
		editItr = nowIdList.begin();
	}
	else
	{
		// 既存ｺﾗｲﾀﾞｰが存在しないので新規追加状態にする
		newEditFlag = true;
	}
}

void CollisionEditScene::ColInfoInit(void)
{
	data.boxFlag = true;				// true:矩形　false:円形

	data.startPos = { 0,0 };			// 矩形開始位置
	data.endPos = { 0,0 };				// 矩形終了位置

	data.r;								// 円形半径
	data.id = id;						// id
}

void CollisionEditScene::AddCol(void)
{
	data.startPos -= VECTOR2(COL_OFFSET_X, COL_OFFSET_Y);
	data.endPos	   = nowPos - VECTOR2(COL_OFFSET_X, COL_OFFSET_Y);

	nowIdList.push_back(data);

	//if (colList.size() == 0)
	//{
	//}
	//else
	//{
	//	auto itr	 = colList.begin();		// itrを先頭に設定
	//	auto nextItr = colList.begin();
	//	nextItr++;

	//	while (true)
	//	{
	//		if (nextItr == colList.end())
	//		{
	//			colList.insert(nextItr, data);	// 同idの一番最後にdataを追加
	//			ColInfoInit();					// ｺﾗｲﾀﾞ-情報の初期化
	//			break;
	//		}

	//		if (itr->id == id)
	//		{
	//			if (nextItr->id != id)
	//			{
	//				colList.insert(itr, data);	// 同idの一番最後にdataを追加
	//				ColInfoInit();				// ｺﾗｲﾀﾞ-情報の初期化
	//				break;
	//			}
	//		}
	//		itr++;
	//		nextItr++;
	//	}
	//}
}

void CollisionEditScene::DrawGrid(void)
{
	VECTOR2 offset = { COL_OFFSET_X , COL_OFFSET_Y };
	int gridSize   = 10;	// ｸﾞﾘｯﾄﾞの大きさ
	int gridRange  = 200;	// 描画の範囲　例:gridSize = 300　なら{-300,-300}から{300,300}までｸﾞﾘｯﾄﾞを引く

	// 縦線（ｸﾞﾘｯﾄﾞ）
	VECTOR2 tmp1( - gridRange,	- gridRange );
	VECTOR2 tmp2( - gridRange,	  gridRange );
	for (; tmp1.x <= gridRange; tmp1.x += gridSize)
	{
		tmp2.x = tmp1.x;
		DrawLine(tmp1 + offset, tmp2 + offset, 0xffffff);	// ｵﾌｾｯﾄの計算はできるだけ最後にすること。
	}
	// 横線（ｸﾞﾘｯﾄﾞ）
	tmp1 = VECTOR2	( - gridRange, - gridRange);
	VECTOR2 tmp3	(   gridRange, - gridRange);
	for (; tmp3.y <= gridRange; tmp3.y += gridSize)
	{
		tmp1.y = tmp3.y;
		DrawLine(tmp1 + offset, tmp3 + offset, 0xffffff);
	}

	// 横線
	DrawLine(VECTOR2(0, COL_OFFSET_Y - PLAYER_SIZE_Y / 2 + 2), VECTOR2(1280, COL_OFFSET_Y - PLAYER_SIZE_Y / 2 + 2), 0xff0000);
	DrawLine(VECTOR2(0, COL_OFFSET_Y + PLAYER_SIZE_Y / 2 - 2), VECTOR2(1280, COL_OFFSET_Y + PLAYER_SIZE_Y / 2 - 2), 0xff0000);
	// 縦線
	DrawLine(VECTOR2(COL_OFFSET_X - PLAYER_SIZE_Y / 2 + 2, 0), VECTOR2(COL_OFFSET_X - PLAYER_SIZE_Y / 2 + 2, 720), 0xff0000);
	DrawLine(VECTOR2(COL_OFFSET_X + PLAYER_SIZE_Y / 2 - 2, 0), VECTOR2(COL_OFFSET_X + PLAYER_SIZE_Y / 2 - 2, 720), 0xff0000);
}

void CollisionEditScene::DrawInfo(int colColor)
{
	ColInf inf;
	inf = ( newEditFlag ? data:(*editItr));
	// プレイヤ or 武器
	if (inf.weponFlag == COL_TYPE_PLAYER)
	{
		DrawFormatString(900, 90, colColor, "[W]当たり判定 : プレイヤ");
	}
	else
	{
		DrawFormatString(900, 90, colColor, "[W]当たり判定 : 武器");
	}

	VECTOR2 endPos = (newEditFlag ? (nowPos) : inf.endPos);

	// 開始位置　終了位置
	DrawFormatString(900, 120, 0xffffff, "開始位置:%d,%d", inf.startPos.x	- COL_OFFSET_X, inf.startPos.y	- COL_OFFSET_Y);
	DrawFormatString(900, 140, 0xffffff, "終了位置:%d,%d", endPos.x			- COL_OFFSET_X, endPos.y		- COL_OFFSET_Y);
	// 画像オフセット
	DrawFormatString(900, 170, 0xffffff, "画像オフセットX:%d", inf.offset.x);
	DrawFormatString(900, 190, 0xffffff, "画像オフセットY:%d", inf.offset.y);


	//使用データの描画
	DrawFormatString(960, 490, 0x00ff00, "キャラクター名");
	DrawFormatString(960, 510, 0xff0000, NowCharName.c_str());
	DrawFormatString(960, 530, 0x00ff00, "アニメーション名");
	DrawFormatString(960, 550, 0xff0000, NowAnimName.c_str());
	if (changePictureFlag)
	{
		DrawFormatString(960,450,0xffffff,"方向キーで変更");
		DrawFormatString(940, 510 + changeCnt * 40, 0xff0000, "→");
	}


	// 矩形 or 円形
	SetFontSize(32);
	if (inf.boxFlag)
	{
		DrawFormatString(50, 50, 0xffffff, "[B]BOX");
	}
	else
	{
		DrawFormatString(50, 50, 0xffffff, "[B]CIRCLE");
		// 半径r
		DrawFormatString(50, 100, 0xffffff, "半径 = %d", inf.r);
	}

	DrawFormatString(50, 10, 0xffffff, newEditFlag?"[E]新規追加":"[E]既存編集");


	// 何ﾌﾚｰﾑ目か
	SetFontSize(64);
	DrawFormatString(50, 500, 0xffffff, "%d番目", id);

	// 攻撃力の表示
	SetFontSize(32);
	if (inf.weponFlag)
	{
		DrawFormatString(900, 230, 0xffffff, "[8↑9↓]攻撃力:%d", inf.attackPoint);
		DrawFormatString(900, 270, 0xffffff, "[5↑6↓]怒り力:%d", inf.angryPoint);
		DrawFormatString(900, 310, 0xffffff, "[2↑3↓]必殺力:%d", inf.specialPoint);
	}

	DrawString(500,	 10, "Aで決定", 0xffffff);
	DrawString(500,	 40, "Sで次のフレーム",0xffffff);
	DrawString(500,	 70, "Cで表示コライダーの削除", 0xffffff);
if (!newEditFlag)
{
	DrawString(500, 100, "Dで編集コライダー変更", 0xffffff);
}

	SetFontSize(16);
}
