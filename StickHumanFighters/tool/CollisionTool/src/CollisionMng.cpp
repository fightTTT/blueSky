#include <DxLib.h>
#include <string>
#include "CollisionMng.h"



bool CollisionMng::ColLoad(std::string pasName, std::string animName)
{
	

	// ファイルパス作成
	std::string loadPass = pasName + "\\" + animName + ".map";		//読み込み先のファイルパスを作成
	
	// hitBoxNumをanimNumでresize
	header.hitBoxNum.resize(header.animNum);
	
	//data.resize(header.animNum);

	HitBox oneTimeData;

	FILE* file;

	fopen_s(&file, loadPass.c_str(), "rb");

	if (!file)
	{
		return false;
	}

	

	fread(&header.animNum, sizeof(int), 1, file);

	fread(&header.hitBoxNum[0], sizeof(int)*header.hitBoxNum.size(), 1, file);




	for (int i = 0; i < data.size(); ++i)
	{
		data[i].hitBox.resize(header.hitBoxNum[i]);

		for (int a = 0; a < data[i].hitBox.size(); a++)
		{
		

			fread(&oneTimeData, sizeof(oneTimeData), 1, file);

			data[i].hitBox[a] = oneTimeData;
		}
	}


	

	fclose(file);

	loadFlag = false;


	return true;
}

void CollisionMng::Update(void)
{
	inputData.Update();

	auto mousePos = inputData.MousePos();

	if (inputFlag)
	{
		if (inputData.MouseStateDown(MOUSE_INPUT_LEFT))
		{
			colData.rect.startPos = inputData.MousePos();
		}


		if (inputData.MouseStateUp(MOUSE_INPUT_LEFT))
		{
			colData.rect.endPos = inputData.MousePos();
		}	

		
		

	/*	if (!(colData.rect.startPos.x == -1 && colData.rect.startPos.y == -1))
		{
			
			DrawBox(colData.rect.startPos.x, colData.rect.startPos.y, mousePos.x, mousePos.y, 0xffffff, false);
		}*/
	}
	
	if ((!loadFlag)&&(!saveFlag))
	{
		if (GetDragFileNum() > 0)
		{
			char filePath[MAX_PATH];

			GetDragFilePath(filePath);

			std::string f_name;

			frameNum = 0;



			bool findFlag = true;

			do
			{

				f_name = std::string(filePath) + "\\" + std::to_string(frameNum) + ".png";

				animFram[frameNum] = LoadGraph(f_name.c_str());		// tmpHandleにﾌｧｲﾙ名f_nameの画像のﾊﾝﾄﾞﾙを代入

				if (animFram[frameNum] == -1)
				{
					findFlag = false;		// 見つからなかった
				}
				else
				{

					frameNum++;							// numを進めて次の画像を読み込む準備
				}

			} while (findFlag);		// findFlagがtrueの時はまだ先があるかもしれないから繰り返す、falseならそれより先はないはずだから抜ける

			// 一つのアニメーションの画像総数
			header.animNum = frameNum;
			frameNum = 0;
			data.resize(header.animNum);
			//texHandle = LoadGraph(f_name.c_str());

			// 以下の処理でパスからキャラ名とアニメーション名を抽出

			animName = filePath;

			auto findNum = animName.rfind("\\");

			animName = animName.substr(findNum + 1);

			//charName = charName.substr(findNum - 6, 6);

			// 当たり判定読み込み
			//ColLoad(charName, animName);



			//auto i = imagePath.find("\\");
			//std::string replace = "/";
			//imagePath.replace(i, 1, replace);

			inputFlag = true;

			//DrawString(100, 100, charName.c_str(), 0xffffff);
		}
	}

	

	if (inputData.KeyStateDown(KEY_INPUT_RIGHT))
	{
		if (header.animNum - 1 > frameNum)
		{
			frameNum++;
			
		}
	}
	else if(inputData.KeyStateDown(KEY_INPUT_LEFT))
	{
		if (0 < frameNum)
		{
			frameNum--;
			
		}
	}
	else
	{
	}
	
	// Zキーで当たり判定を一つ消す	
	if (inputData.KeyStateDown(KEY_INPUT_Z))
	{
		if (!(data[frameNum].hitBox.size() == 0))
		{
			data[frameNum].hitBox.pop_back();
		}
	}

	// 右クリックで攻撃とプレイヤーの切り替え
	if (inputData.MouseStateDown(MOUSE_INPUT_RIGHT))
	{
		colTypeCnt++;
	}

	colTypeCnt = colTypeCnt % COLTYPE_MAX;

	if (colTypeCnt == COLTYPE_ATTACK)
	{
		colData.type = COLTYPE_ATTACK;
	}
	else if (colTypeCnt == COLTYPE_HIT)
	{
		colData.type = COLTYPE_HIT;
	}
	else if(colTypeCnt == COLTYPE_GUARD)
	{
		colData.type = COLTYPE_GUARD;
	}
	else
	{
	}


	// Aキーで一つの当たり判定保存
	if (inputData.KeyStateDown(KEY_INPUT_A))
	{
		colData.rect.endPos = mousePos;

		colData.rect.startPos -= VECTOR2(IMAGE_OFFSET_X+ (256 / 2), IMAGE_OFFSET_Y + 256);
		colData.rect.endPos -= VECTOR2(IMAGE_OFFSET_X + (256 / 2), IMAGE_OFFSET_Y + 256);

		data[frameNum].hitBox.push_back(colData);
	}

	if (inputData.KeyStateDown(KEY_INPUT_F6))
	{
		//ColSave();

		int tmp = MessageBox(
			NULL,								// オーナーウィンドウのハンドル
			"現在の登録データを保存しますか？",     // メッセージボックス内のテキスト
			"確認ダイアログ",					// メッセージボックスのタイトル
			MB_OKCANCEL						    // メッセージボックスのスタイル
		);

		if (tmp == IDOK)
		{
			saveFlag = true;
		}
	}

	if (saveFlag)
	{
		char filePath[MAX_PATH];

		std::string f_name;

		if (GetDragFileNum() > 0)
		{
			GetDragFilePath(filePath);

			f_name = std::string(filePath);

			ColSave(f_name, animName);
		}
		else
		{
			DrawString(200, IMAGE_OFFSET_Y, "colファイルをドラッグ&ドロップしてください", 0xffffff);

		}
	}

	if (inputData.KeyStateDown(KEY_INPUT_F5))
	{
		int tmp = MessageBox(
			NULL,								// オーナーウィンドウのハンドル
			"保存されているデータをロードしますか？",     // メッセージボックス内のテキスト
			"確認ダイアログ",					// メッセージボックスのタイトル
			MB_OKCANCEL						    // メッセージボックスのスタイル
		);

		if (tmp == IDOK)
		{
			loadFlag = true;
		}
	}

	if (loadFlag)
	{
		char filePath[MAX_PATH];

		std::string f_name;


		if (GetDragFileNum() > 0)
		{
			GetDragFilePath(filePath);

			f_name = std::string(filePath);

			ColLoad(f_name, animName);
		}
		else
		{
			DrawString(200, IMAGE_OFFSET_Y, "colファイルをドラッグ&ドロップしてください", 0xffffff);

		}
	}

}

void CollisionMng::Draw(void)
{
	VECTOR2 offset = VECTOR2(IMAGE_OFFSET_X + (256 / 2), IMAGE_OFFSET_Y + 256);

	int colColor = (colData.type == COLTYPE_ATTACK ? 0xff0000 : (colData.type == COLTYPE_HIT ? 0x87CEEB:0x00ff00));

	auto mousePos = inputData.MousePos();

	if (inputFlag)
	{
		if (animFram[frameNum] != -1)
		{
			DrawGraph(IMAGE_OFFSET_X, IMAGE_OFFSET_Y, animFram[frameNum], true);
		}
	}
	else
	{
		DrawString(200, IMAGE_OFFSET_Y, "画像ファイルをドラッグ&ドロップしてください", 0xffffff);
	}
	
	// 現在の
	if (!(colData.rect.startPos.x == -1 && colData.rect.startPos.y == -1))
	{
		DrawBox(colData.rect.startPos.x, colData.rect.startPos.y, mousePos.x, mousePos.y, colColor, false);
	}

	if (data.size())
	{
		for (auto col = data[frameNum].hitBox.begin(); col != data[frameNum].hitBox.end(); col++)
		{
			if (data[frameNum].hitBox.size() == 0)
			{
				break;
			}

			int tmpColor = ((*col).type == COLTYPE_ATTACK ? 0xff0000 : ((*col).type == COLTYPE_HIT ? 0x87CEEB : 0x00ff00));

			DrawBox((*col).rect.startPos.x + offset.x, (*col).rect.startPos.y + offset.y, (*col).rect.endPos.x + offset.x, (*col).rect.endPos.y + offset.y, tmpColor, false);
		}
	}
	

	
}

CollisionMng::CollisionMng()
{
	Init();
}


CollisionMng::~CollisionMng()
{
}

void CollisionMng::ColSave(std::string pathName, std::string animName)
{


	if (!data.size())
	{
		return;
	}

	DataHeader expData = {
		static_cast<int>(data.size()),
	};

	

	std::string SavePass = pathName + "\\" + animName + ".map";		//保存先のファイルパスを作成

	HitBox oneTimeData;

	FILE* file;

	fopen_s(&file, SavePass.c_str(), "wb");		// ﾌｧｲﾙｵｰﾌﾟﾝ　開いて書き込んで閉じる

	expData.hitBoxNum.resize(data.size());
	for (int i = 0; i < data.size(); i++)
	{
		expData.hitBoxNum[i] = data[i].hitBox.size();
	}
	

	fwrite(&expData.animNum, sizeof(int), 1, file);

	fwrite(&expData.hitBoxNum[0], sizeof(int)*expData.hitBoxNum.size(), 1, file);

	for (int i = 0; i < data.size(); ++i)
	{
		for (int a = 0; a < data[i].hitBox.size(); a++)
		{
			oneTimeData = data[i].hitBox[a];

			fwrite(&oneTimeData, sizeof(oneTimeData),1, file);
		}
	}

	fclose(file);

	saveFlag = false;

}

void CollisionMng::Init(void)
{
	data.clear();
	colData.rect.startPos = { -1,-1 };
	colData.rect.endPos   = { -1,-1 };
	inputFlag = false;
	loadFlag = false;
	saveFlag = false;
	colTypeCnt = 0;

	for (int i = 0; i < 20; i++)
	{
		animFram[i] = 0;
	}
}