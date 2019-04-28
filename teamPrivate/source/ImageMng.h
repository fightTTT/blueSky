#pragma once
#include <map>
#include <vector>
#include <mutex>
#include "VECTOR2.h"

class VECTOR2;
using VEC_INT = std::vector<int>;
#define lpImageMng ImageMng::GetInstance()
#define IMAGE_ID(X) (ImageMng::GetInstance().GetID(X))

class ImageMng
{
public:
	static ImageMng &GetInstance(void)
	{
		static ImageMng s_Instance;
		return (s_Instance);
	}
	VECTOR2 divSize;
	VECTOR2 divCnt;
	const VEC_INT& GetID(std::string f_name);		// �摜����ق̓���
	const VEC_INT& GetID(std::string f_name, VECTOR2 divSize, VECTOR2 divCnt);		// �摜����ق̓���
	const VEC_INT& LoadContinueImageId(std::string f_Pass, std::string f_name);    //�A�ԉ摜�f�[�^��ǂݍ���
	const int GetSize(std::string f_name);

private:
	ImageMng();
	~ImageMng();
	// std::map<���̌^,�A�z�z��̌^> �A�z�z��
	std::map<std::string, VEC_INT> imgMap;
};

