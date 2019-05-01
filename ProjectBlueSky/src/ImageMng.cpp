#include <string>
#include "DxLib.h"
#include "VECTOR2.h"
#include "ImageMng.h"

// ﾃﾞﾊﾞｯｸﾒｯｾｰｼﾞ用定義
#ifdef _DEBUG		// 失敗時の処理
#define AST() {\
	CHAR ast_mes[256];\
	wsprintf(ast_mes, "%s %d行目\0", __FILE__, __LINE__);\
	MessageBox(0, ast_mes, "ｱｻｰﾄ表示", MB_OK);\
	}
#else				// 成功時の処理
#define AST()
#endif		// _DEBUG

ImageMng::ImageMng()
{
}

ImageMng::~ImageMng()
{
}


const VEC_INT& ImageMng::GetID(std::string f_name)
{
	if (imgMap.find(f_name) == imgMap.end())			// imgMap[f_name]が存在するかの判断	true:存在しない　false:存在する
	{
		imgMap[f_name].resize(1);						// imgMap[f_name]配列をﾘｻｲｽﾞする
		imgMap[f_name][0] = LoadGraph(f_name.c_str());	// imgMap[f_name][0]にﾌｧｲﾙ名f_nameの画像のﾊﾝﾄﾞﾙを代入
		if (imgMap[f_name][0] == -1)
		{
			AST();
		}
	}
	return imgMap[f_name];								// imgMap[f_name]を返す
}

const VEC_INT & ImageMng::GetID(std::string f_name, VECTOR2 divSize, VECTOR2 divCut)
{
	if (imgMap.find(f_name) == imgMap.end())
	{
		imgMap[f_name].resize(divCut.x*divCut.y);
		if (LoadDivGraph(f_name.c_str(),
			divCut.x*divCut.y,
			divCut.x, divCut.y,
			divSize.x, divSize.y,
			&imgMap[f_name][0]) == -1)
		{
			AST();
		}
	}
	return imgMap[f_name];
}

void ImageMng::LoadImageCharacterAll(std::string characterName, std::string animName[], std::map<std::string, std::string> animFileName)
{
	bool findFlag;			// ｱﾆﾒｰｼｮﾝの画像が見つかったかどうかのﾌﾗｸﾞ
	std::string f_name;		// ﾌｧｲﾙ名
	int frameNum;			// ｱﾆﾒｰｼｮﾝのｺﾏ番号 (0ｽﾀｰﾄ)
	int tmpHandle;

	for (unsigned int i = 0; i < 6; i++)		// ｱﾆﾒｰｼｮﾝの種類の数だけﾙｰﾌﾟ
	{
		findFlag = true;
		frameNum = 0;

		do
		{
			// (ｷｬﾗ名、ｱﾆﾒｰｼｮﾝ名、ｱﾆﾒｰｼｮﾝ名に対応したﾌｧｲﾙ名、ｱﾆﾒｰｼｮﾝのｺﾏ番号)をもとにﾊﾟｽ情報を含んだﾌｧｲﾙ名を作成
			f_name = ("image/" + characterName + "/" + animName[i] + "/" + animFileName[animName[i]] + "_" + std::to_string(frameNum) + ".png");

			tmpHandle = LoadGraph(f_name.c_str());		// tmpHandleにﾌｧｲﾙ名f_nameの画像のﾊﾝﾄﾞﾙを代入

			if (tmpHandle == -1)
			{
				findFlag = false;		// 見つからなかった
			}
			else
			{
				imgMap[f_name].resize(1);			// imgMap[f_name]配列をﾘｻｲｽﾞする
				imgMap[f_name][0] = tmpHandle;		// tmpHandleは見つかったときのものなのでimgMap[f_name][0]に代入
				frameNum++;							// numを進めて次の画像を読み込む準備
			}

		} while (findFlag);		// findFlagがtrueの時はまだ先があるかもしれないから繰り返す、falseならそれより先はないはずだから抜ける
	}
}
