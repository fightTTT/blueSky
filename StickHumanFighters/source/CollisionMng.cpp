#include "CollisionMng.h"



bool CollisionMng::ColLoad(std::string charaName, std::string animName, int animFrame)
{
	if (colMap[charaName].isLoad(animName))
	{
		// すでにある場合読み込みしない
		return false;
	}

	// ファイルパス作成

	DataHeader header;


	header.animNum = animFrame;


	// ファイルパス作成
	std::string loadPass = "colData/" + charaName + "/" + animName + ".map";		//読み込み先のファイルパスを作成

	// hitBoxNumをanimNumでresize
	header.hitBoxNum.resize(header.animNum);

	//data.resize(header.animNum);

	int colNum = header.animNum;

	HitBox oneTimeData;

	FILE* file;

	fopen_s(&file, loadPass.c_str(), "rb");

	if (!file)
	{
		colFlag[animName] = false;
		return false;
	}



	fread(&header.animNum, sizeof(int), 1, file);

	fread(&header.hitBoxNum[0], sizeof(int)*header.hitBoxNum.size(), 1, file);

	if (colNum != header.animNum)
	{
		colFlag[animName] = false;
		return false;
	}

	data.resize(header.animNum);


	for (int i = 0; i < data.size(); ++i)
	{
		data[i].hitBox.resize(header.hitBoxNum[i]);

		for (int a = 0; a < data[i].hitBox.size(); a++)
		{

			fread(&oneTimeData, sizeof(oneTimeData), 1, file);

			data[i].hitBox[a] = oneTimeData;
		}
	}
	fclose(file);

	colFlag[animName] = true;
	colMap[charaName].SetColData(animName, data);

	return true;
}

const bool CollisionMng::GetColFlag(std::string animName)
{
	return colFlag[animName];
}

bool CollisionMng::collisionCheck(VECTOR2 startPos, VECTOR2 endPos,VECTOR2 startPos2,VECTOR2 endPos2)
{
	return false;
}

CollisionMng::CollisionMng()
{
}

CollisionMng::~CollisionMng()
{
}