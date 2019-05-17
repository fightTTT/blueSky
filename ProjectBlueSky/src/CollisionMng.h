#pragma once

#include "Collision.h"

struct DataHeader
{
	int animNum;				// アニメーション画像の枚数
	std::vector<int> hitBoxNum;	// 画像1枚のHitBoxの数
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

	// キー : キャラクター名
	std::map<std::string, bool> colFlag;
	std::map<std::string, Collision> colMap;
};
