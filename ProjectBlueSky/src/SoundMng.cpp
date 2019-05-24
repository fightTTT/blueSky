#include "SoundMng.h"

#include "DxLib.h"

// ���ޯ�ү���ޗp��`
#ifdef _DEBUG		// ���s���̏���
#define AST() {\
	CHAR ast_mes[256];\
	wsprintf(ast_mes, "%s %d�s��\0", __FILE__, __LINE__);\
	MessageBox(0, ast_mes, "���ĕ\��", MB_OK);\
	}
#else				// �������̏���
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
