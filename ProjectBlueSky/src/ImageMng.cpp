#include <string>
#include "DxLib.h"
#include "VECTOR2.h"
#include "ImageMng.h"

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

ImageMng::ImageMng()
{
}

ImageMng::~ImageMng()
{
}


const VEC_INT& ImageMng::GetID(std::string f_name)
{
	if (imgMap.find(f_name) == imgMap.end())			// imgMap[f_name]�����݂��邩�̔��f	true:���݂��Ȃ��@false:���݂���
	{
		imgMap[f_name].resize(1);						// imgMap[f_name]�z���ػ��ނ���
		imgMap[f_name][0] = LoadGraph(f_name.c_str());	// imgMap[f_name][0]��̧�ٖ�f_name�̉摜������ق���
		if (imgMap[f_name][0] == -1)
		{
			AST();
		}
	}
	return imgMap[f_name];								// imgMap[f_name]��Ԃ�
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

void ImageMng::LoadImageCharacterAll(std::string characterName, std::vector<std::string> animName, std::map<std::string, std::string> animFileName)
{
	char f_name[70];		// ̧�ٖ�
	int frameNum;			// ��Ұ��݂̺ϔԍ� (0����)
	int tmpHandle;

	for (unsigned int i = 0; i < animName.size(); i++)		// ��Ұ��݂̎�ނ̐�����ٰ��
	{
		frameNum = 0;

		while (true)
		{
			// (��ז��A��Ұ��ݖ��A��Ұ��ݖ��ɑΉ�����̧�ٖ��A��Ұ��݂̺ϔԍ�)�����Ƃ��߽�����܂�̧�ٖ����쐬
			sprintf_s(f_name, "image/%s/%s/%s_%d.png", characterName.c_str(), animName[i].c_str(), animFileName[animName[i]].c_str(), frameNum);

			if ((imgMap.find(f_name) == imgMap.end()) || (frameNum < 100))
			{
				tmpHandle = LoadGraph(f_name);		// tmpHandle��̧�ٖ�f_name�̉摜������ق���

				if (tmpHandle == -1)
				{
					break;		// ������Ȃ�����
				}
				else
				{
					imgMap[f_name].resize(1);			// imgMap[f_name]�z���ػ��ނ���
					imgMap[f_name][0] = tmpHandle;		// tmpHandle�͌��������Ƃ��̂��̂Ȃ̂�imgMap[f_name][0]�ɑ��
					frameNum++;							// num��i�߂Ď��̉摜��ǂݍ��ޏ���
				}
			}
		}
	}
}
