#pragma once

#include <vector>
#include <map>
#include "VECTOR2.h"

enum ColType
{
	COLTYPE_ATTACK,
	COLTYPE_HIT,
	COLTYPE_MAX
};

struct HitBox
{
	ColType type;

	bool rectFlag;	// 当たり判定が矩形か円か

	Rect rect;
	Circle circle;
};

struct ColInfo
{
	std::vector<HitBox> hitBox;
};

class Collision
{
public:
	Collision();
	~Collision();

	// 現在のアニメーションのhitboxの配列を取得
	ColInfo GetColData(std::string animName, int animIdx) { return colData[animName][animIdx]; }

	// 指定したアニメーション名でColInfoを保存
	void SetColData(std::string animName, std::vector<ColInfo> info);

	// 指定したアニメーション名のデータが読み込み済みかどうか
	bool isLoad(std::string animName);

private:

	// キー : アニメーション名
	std::map<std::string, std::vector<ColInfo>> colData;
};

