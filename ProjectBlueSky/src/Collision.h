#pragma once

#include <vector>
#include <map>
#include "VECTOR2.h"

enum ColType
{
	COLTYPE_ATTACK,		// 攻撃用の当たり判定
	COLTYPE_HIT,		// ダメージ受ける用の当たり判定
	COLTYPE_GUARD,		// ガード用の当たり判定
	COLTYPE_MAX
};

struct HitBox
{
	ColType type;		// 当たり判定の種類
	bool rectFlag;
	Rect rect;			// 矩形の当たり判定の開始地点と終了地点
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