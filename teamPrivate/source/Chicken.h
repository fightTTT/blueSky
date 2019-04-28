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
	CHICKEN_COLOR_NOMAL,	// ���炷�Ƃ����
	CHICKEN_COLOR_WHITE,	// ��
	CHICKEN_COLOR_BROWN,	// ��
	CHICKEN_COLOR_MAX
};

typedef struct {
	std::string attackName;		// �󂯂��U����
	int animId;					// �󂯂��U���̺ϐ�
}CHICKEN_HIT_PLAYER_INF;

typedef struct {
	VECTOR2 oldPos;	// �����Ă��鏰��1�ڰт܂����W���
	int itrCnt;		// �����Ă��鏰�Ɠ����蔻�肪�������ꍇ�̐i��itr��
}CHICKEN_PARENT_FLOOR;

typedef struct {
	VECTOR2 pos;	// ���W
	int image;		// �摜
	int alpha;		// �����ސ�
	bool endFlag;	// ���S��]������	true:�]���ς�
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

	const int GetPadNum(void);		// padNum����肷��
	bool CheckExpCnt(void);			// expCnt�ɐ��l�������Ă����true

private:
	void Dash(const GameController &controller, VECTOR2 &tmpPos);				// �ޯ���̏���

	std::vector<ColInf> GetMyCol(void);

	std::array<std::string, CHICKEN_COLOR_MAX> chickenImagePass;

	VECTOR2 pos;
	DIR dir;
	DIR dirOld;
	std::string imageName;
	CHICKEN_COLOR color;
	LOCATE locate;
	void PosAdjust(VECTOR2 &tmpPos, int maxEndPosY);			// ���W�␳
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
	int UICnt;			// UI�𐶂₷���߂̶���

	// SE�p
	bool deathSeFlag;
};

