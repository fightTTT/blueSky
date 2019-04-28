#include"DxLib.h"
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
	if (imgMap.find(f_name) == imgMap.end())			//imgMap[f_name]が存在するかの判断	true:存在しない　false:存在する
	{
		imgMap[f_name].resize(1);						//imgMap[f_name]配列をﾘｻｲｽﾞする
		imgMap[f_name][0] = LoadGraph(f_name.c_str());	//imgMap[f_name][0]にﾌｧｲﾙ名f_nameの画像のﾊﾝﾄﾞﾙを代入
		if (imgMap[f_name][0] == -1)
		{
			AST();
		}
	}
	return imgMap[f_name];								//imgMap[f_name]を返す
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
