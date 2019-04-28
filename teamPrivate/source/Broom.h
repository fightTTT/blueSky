#pragma once
#include "Obj.h"
class Broom :
	public Obj
{
public:
	Broom();
	Broom(sharedListFloor floorList, VECTOR2 pos, int padNum);
	~Broom();

	const int GetPadNum(void);

	bool initAnim(void);

	void Draw(void);

private:
	bool CheckObjType(OBJ_TYPE type);
	void SetMove(const GameController &controller, weakListObj objList, weakListChicken chickenList, bool entryFlag);
	void PosUpDate(VECTOR2& tmpPos, int maxEndPosY);		// ���W�̕␳��d�͂ɂ��e���̌v�Z���s��
	void UniqueAttack(VECTOR2& tmpPos);					// �ŗL�U���̓����̐ݒ�
	void SetNextAnim(bool animEndFlag);
	void PlaySPSe(void);
	int GetJumpCntMax(void);						// �ő�ެ��ߐ��𓾂�
};

