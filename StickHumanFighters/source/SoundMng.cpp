#include "SoundMng.h"

#include "DxLib.h"

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

const int SoundMng::GetID(std::string filePath)
{
	if (!soundMap.count(filePath))
	{
		soundMap[filePath] = LoadSoundMem(filePath.c_str());

		if (soundMap[filePath] == -1)
		{
			AST();
		}
	}
	return soundMap[filePath];
}

SoundMng::SoundMng()
{
}

SoundMng::~SoundMng()
{
	for (auto map : soundMap)
	{
		DeleteSoundMem(map.second);
	}
}
