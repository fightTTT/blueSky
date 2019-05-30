#pragma once

#include "Collision.h"

struct DataHeader
{
	int animNum;				// �A�j���[�V�����摜�̖���
	std::vector<int> hitBoxNum;	// �摜1����HitBox�̐�
};

#define lpColMng CollisionMng::GetInstance()

class CollisionMng
{
public:
	static CollisionMng &GetInstance()
	{
		static CollisionMng s_Instance;
		return s_Instance;
	}

	bool ColLoad(std::string charaName, std::string animName, int animFrame);

	ColInfo GetCollisionData(std::string charName, std::string animName, int animIdx) { return colMap[charName].GetColData(animName, animIdx); }

	const bool GetColFlag(std::string animName);

private:

	CollisionMng();
	~CollisionMng();

	CollisionMng(const CollisionMng&);
	void operator=(const CollisionMng&) {}

	std::vector<ColInfo> data;

	// �L�[ : �L�����N�^�[��
	std::map<std::string, bool> colFlag;
	std::map<std::string, Collision> colMap;
};
