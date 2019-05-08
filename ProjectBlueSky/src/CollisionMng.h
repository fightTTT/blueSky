#pragma once

#include "Collision.h"

struct DataHeader
{
	int animNum;				// �A�j���[�V�����摜�̖���
	std::vector<int> hitBoxNum;	// �摜1����HitBox�̐�
};

class CollisionMng
{
public:
	static CollisionMng &GetInstance()
	{
		static CollisionMng s_Instance;
		return s_Instance;
	}

	bool ColLoad(std::string charaName, std::string animName);

private:

	CollisionMng();
	~CollisionMng();

	// �L�[ : �L�����N�^�[��
	std::map<std::string, Collision> colMap;
};

