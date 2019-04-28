#include <string>
#include "Dxlib.h"
#include "ColMng.h"
#include "ImageMng.h"

const VEC_INT& ImageMng::GetID(std::string f_name)
{
	// map�ɱ��������u���ް�����Ȃ������烁�������m�ۂ��Ă��̒l��Ԃ�
	if (imgMap.find(f_name) == imgMap.end())
	{
		imgMap[f_name].resize(1);
		// imgMap[map][VEC_INT]
		imgMap[f_name][0] = LoadGraph((/*"image/" + */f_name/* + ".png"*/).c_str());
	}
	return imgMap[f_name];
}

const VEC_INT & ImageMng::GetID(std::string f_name, VECTOR2 divSize, VECTOR2 divCnt)
{
	if (imgMap.find(f_name) == imgMap.end())
	{
		imgMap[f_name].resize(divCnt.x*divCnt.y);
		LoadDivGraph(
			(f_name).c_str(), 
			divCnt.x*divCnt.y, 
			divCnt.x, 
			divCnt.y,
			divSize.x,
			divSize.y, 
			&imgMap[f_name][0]);
	}
	return imgMap[f_name];
}

const VEC_INT & ImageMng::LoadContinueImageId(std::string f_Pass, std::string f_name)
{
	if (imgMap.find(f_Pass) == imgMap.end() || imgMap[f_Pass].size() == 0)
	{
		int AnimFrame = 0;
		while (true)//�A�ԃf�[�^���ׂēǂݍ��ނ܂Ń��[�v
		{
			imgMap[f_Pass].resize(AnimFrame + 1);						//�K�v�A�j���[�V��������VECTOR�̃��T�C�Y

																	
			std::string FilePassString = "image/" + f_Pass + "/" + f_name + "_" + std::to_string(AnimFrame) + ".png";			//�A�ԃf�[�^�̃t�@�C���p�X�𐶐�

			const char *FilePassChar = FilePassString.c_str();											//Char�^�ɕϊ�

			imgMap[f_Pass][AnimFrame] = LoadGraph(FilePassChar);												//1�����ǂݍ���
			if (imgMap[f_Pass][AnimFrame] == -1)
			{
				imgMap[f_Pass].resize(AnimFrame);
				break;
			}
			AnimFrame++;
		}
	}
	return imgMap[f_Pass];
}

const int ImageMng::GetSize(std::string f_name)
{
	return static_cast<const int>(imgMap[f_name].size());
}

ImageMng::ImageMng()
{
}


ImageMng::~ImageMng()
{
}
