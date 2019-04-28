#pragma once
#include "Obj.h"


class Umbrella :
	public Obj
{
public:
	Umbrella();
	Umbrella(sharedListFloor floorList, VECTOR2 pos, int padNum);
	~Umbrella();

	const int GetPadNum(void);

	bool initAnim(void);

	void Draw(void);

private:
	bool CheckObjType(OBJ_TYPE type);
	void SetMove(const GameController &controller, weakListObj objList, weakListChicken chickenList, bool entryFlag);
	void UniqueAttack(VECTOR2& tmpPos);					// �ŗL�U���̓����̐ݒ�
	void UniqueColGround(void);							// ��ׂ��Ƃ̒n�ʂ⏰�Ɖ�����������̏������s��
	void SetNextAnim(bool animEndFlag); 
	
	void PlaySPSe(void);	// �K�E�Z�̍Đ�
};

