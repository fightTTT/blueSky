#pragma once
#include "Obj.h"
class Lid :
	public Obj
{
public:
	Lid();
	Lid(sharedListFloor floorList, VECTOR2 pos, int padNum);
	~Lid();

	const int GetPadNum(void);

	bool initAnim(void);

	void Draw(bool pauseFlag, CUT_IN_INFO cutInInfo); 
	
	const VECTOR2 GetFlyHighPower(void);

private:
	bool CheckObjType(OBJ_TYPE type);
	void SetMove(const GameController &controller, weakListObj objList, weakListChicken chickenList, bool entryFlag);
	void UniqueAttack(VECTOR2& tmpPos);
	int GetSpAttackTimeMax(void); 
	int GetGuardTimeMax(void);						// ·¬×‚²‚Æ‚ÌÅ‘å¶Ş°ÄŞŠÔ‚ğ“¾‚é
	void PlaySPSe(void);
};

