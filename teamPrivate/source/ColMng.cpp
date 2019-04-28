#include "DxLib.h"
#include "ColMng.h"


ColMng::ColMng()
{
}

ColMng::~ColMng()
{
}

std::vector<ColInf> ColMng::GetCollider(std::string CharName, std::string AnimName)
{
	if (colMap.find(CharName) == colMap.end()
	||  colMap[CharName].find(AnimName) == colMap[CharName].end())
	{
		ColLoad(CharName, AnimName);
	}

	return colMap[CharName][AnimName];
}

std::vector<ColInf> ColMng::GetCollider(std::string CharName, std::string AnimName, int animFrame)
{
	auto vec = GetCollider(CharName, AnimName);

	std::vector<ColInf> colVec;
	for (auto &data : vec)
	{
		if (data.id == animFrame)
		{
			colVec.push_back(data);
		}
	}

	return colVec;
}

void ColMng::ColLoad(std::string CharName, std::string AnimName)
{
	FILE* file;
	DataHeader expData;
	std::string loadPass = "colData/"+CharName+"/"+ AnimName + ".map";		//読み込み先のファイルパスを作成

	fopen_s(&file,loadPass.c_str(), "rb");
	if (!file)
	{
		return;
	}

	fread(&expData, sizeof(expData), 1, file);

	colMap[CharName][AnimName].resize(expData.colCnt);

	fread(&colMap[CharName][AnimName][0], sizeof(ColInf), colMap[CharName][AnimName].size(), file);

	fclose(file);
}