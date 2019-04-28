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

	bool CheckArrive(void);		// true:�ړI���W�܂œ��B����	false:���B���ĂȂ�

	VECTOR2 GetPos(void);
	int		GetOrbColor(void);

	void Draw(void);

private:
	unsigned int animId;			// ��Ұ��݂�id���i�[

	int cnt;

	POS_d  pos_d;		// ���ݍ��W(double�^)
	POS_d  endPos_d;	// �I�_���W(double�^)
	double rad;			// �p�x
	POS_d  moveSpeed;	// �ړ���(double�^)

	int orbColor;		// ��:�K�E�ް��		��:�{��ް�ނ�

	int addPoint;

	int padNum;
};
