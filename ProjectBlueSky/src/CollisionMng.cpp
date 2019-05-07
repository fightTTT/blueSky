#include "CollisionMng.h"



bool CollisionMng::ColLoad(std::string charaName, std::string animName)
{
	if (colMap[charaName].isLoad(animName))
	{
		// ���łɂ���ꍇ�ǂݍ��݂��Ȃ�
		return false;
	}

	// �t�@�C���p�X�쐬

	DataHeader header;
	// animNum read
	// hitBoxNum��animNum��resize

	std::vector<ColInfo> data;
	data.resize(header.animNum);
	for (int i = 0; i < data.size(); ++i)
	{
		data[i].hitBox.resize(header.hitBoxNum[i]);
		// read
	}

	colMap[charaName].SetColData(animName, data);

	return true;
}

CollisionMng::CollisionMng()
{
}


CollisionMng::~CollisionMng()
{
}
