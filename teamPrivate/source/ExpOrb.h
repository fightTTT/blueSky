#pragma once
#include "VECTOR2.h"
#include "classObj.h"

class Obj;

typedef struct
{
	double x;
	double y;
}POS_d;

class ExpOrb
{
public:
	ExpOrb(VECTOR2 startPos, int padNum, int addPoint);
	~ExpOrb();

	void UpDate(weakListObj objList);

	void DeathPrc(weakListObj objList);

	bool CheckArrive(void);		// true:目的座標まで到達した	false:到達してない

	VECTOR2 GetPos(void);
	int		GetOrbColor(void);

	void Draw(void);

private:
	unsigned int animId;			// ｱﾆﾒｰｼｮﾝのidを格納

	int cnt;

	POS_d  pos_d;		// 現在座標(double型)
	POS_d  endPos_d;	// 終点座標(double型)
	double rad;			// 角度
	POS_d  moveSpeed;	// 移動量(double型)

	int orbColor;		// 青:必殺ｹﾞｰｼﾞ		赤:怒りｹﾞｰｼﾞか

	int addPoint;

	int padNum;
};
