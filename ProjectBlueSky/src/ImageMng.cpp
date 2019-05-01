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

void ImageMng::LoadImageCharacterAll(std::string characterName, std::string animName[], std::map<std::string, std::string> animFileName)
{
	bool findFlag;			// ��Ұ��݂̉摜�������������ǂ������׸�
	std::string f_name;		// ̧�ٖ�
	int frameNum;			// ��Ұ��݂̺ϔԍ� (0����)
	int tmpHandle;

	for (unsigned int i = 0; i < 6; i++)		// ��Ұ��݂̎�ނ̐�����ٰ��
	{
		findFlag = true;
		frameNum = 0;

		do
		{
			// (��ז��A��Ұ��ݖ��A��Ұ��ݖ��ɑΉ�����̧�ٖ��A��Ұ��݂̺ϔԍ�)�����Ƃ��߽�����܂�̧�ٖ����쐬
			f_name = ("image/" + characterName + "/" + animName[i] + "/" + animFileName[animName[i]] + "_" + std::to_string(frameNum) + ".png");

			tmpHandle = LoadGraph(f_name.c_str());		// tmpHandle��̧�ٖ�f_name�̉摜������ق���

			if (tmpHandle == -1)
			{
				findFlag = false;		// ������Ȃ�����
			}
			else
			{
				imgMap[f_name].resize(1);			// imgMap[f_name]�z���ػ��ނ���
				imgMap[f_name][0] = tmpHandle;		// tmpHandle�͌��������Ƃ��̂��̂Ȃ̂�imgMap[f_name][0]�ɑ��
				frameNum++;							// num��i�߂Ď��̉摜��ǂݍ��ޏ���
			}

		} while (findFlag);		// findFlag��true�̎��͂܂��悪���邩������Ȃ�����J��Ԃ��Afalse�Ȃ炻�����͂Ȃ��͂������甲����
	}
}
