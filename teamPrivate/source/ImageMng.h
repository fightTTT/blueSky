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
	const VEC_INT& GetID(std::string f_name);		// 画像ﾊﾝﾄﾞﾙの入手
	const VEC_INT& GetID(std::string f_name, VECTOR2 divSize, VECTOR2 divCnt);		// 画像ﾊﾝﾄﾞﾙの入手
	const VEC_INT& LoadContinueImageId(std::string f_Pass, std::string f_name);    //連番画像データを読み込む
	const int GetSize(std::string f_name);

private:
	ImageMng();
	~ImageMng();
	// std::map<ｷｰの型,連想配列の型> 連想配列名
	std::map<std::string, VEC_INT> imgMap;
};

