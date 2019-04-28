#pragma once
#include <array>
#include <vector>
#include "ColMng.h"
#include "VECTOR2.h"
#include "classFloor.h"
#include "classObj.h"
#include "classChicken.h"
#include "DIR.h"
#include "PARAM.h"
#include "LOCATE.h"

enum CHICKEN_COLOR {
	CHICKEN_COLOR_NOMAL,	// いらすとやﾁｷﾝ
	CHICKEN_COLOR_WHITE,	// 白
	CHICKEN_COLOR_BROWN,	// 茶
	CHICKEN_COLOR_MAX
};

typedef struct {
	std::string attackName;		// 受けた攻撃名
	int animId;					// 受けた攻撃のｺﾏ数
}CHICKEN_HIT_PLAYER_INF;

typedef struct {
	VECTOR2 oldPos;	// 浮いている床の1ﾌﾚｰﾑまえ座標情報
	int itrCnt;		// 浮いている床と当たり判定があった場合の進んだitr数
}CHICKEN_PARENT_FLOOR;

typedef struct {
	VECTOR2 pos;	// 座標
	int image;		// 画像
	int alpha;		// ﾌﾞﾚﾝﾄﾞ数
	bool endFlag;	// 死亡後転生処理	true:転生済み
}CHICKEN_SOUL;

class GameController;

class Chicken
{
public:
	Chicken(sharedListFloor floorList, VECTOR2 pos, int padNum, CHICKEN_COLOR chickenColor);
	~Chicken();

	void UpDate(const GameController & controller, weakListChicken chickenList);
	void CheckHit(weakListObj objList);
	void Draw(void);

	const int GetPadNum(void);		// padNumを入手する
	bool CheckExpCnt(void);			// expCntに数値が入っていればtrue

private:
	void Dash(const GameController &controller, VECTOR2 &tmpPos);				// ﾀﾞｯｼｭの処理

	std::vector<ColInf> GetMyCol(void);

	std::array<std::string, CHICKEN_COLOR_MAX> chickenImagePass;

	VECTOR2 pos;
	DIR dir;
	DIR dirOld;
	std::string imageName;
	CHICKEN_COLOR color;
	LOCATE locate;
	void PosAdjust(VECTOR2 &tmpPos, int maxEndPosY);			// 座標補正
	float vY;
	float vX;
	PARAM param;
	float gravity;
	bool floorCheckFlag;
	CHICKEN_PARENT_FLOOR parentFloor;
	bool fallFlag;
	int padNum;
	int jumpCnt;
	bool downFlag;
	int nowTimeDown; 
	int nowTimeLR;
	int framCnt;
	int speed;
	bool dashFlag;
	sharedListFloor floorList;
	std::array<CHICKEN_HIT_PLAYER_INF, 4> hitPlayerInf;
	int visible;
	int fixCtrl;
	int damageInv;
	int hp;
	bool deathFlag;
	int expCnt;
	CHICKEN_SOUL chickenSoul;
	int chickenWalkSeCnt;

	// UI
	int UICnt;			// UIを生やすためのｶｳﾝﾄ

	// SE用
	bool deathSeFlag;
};

