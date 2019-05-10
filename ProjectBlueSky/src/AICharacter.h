#pragma once

#include "Obj.h"

class AIState;

class AICharacter :
	public Obj
{
public:
	AICharacter();
	~AICharacter();

	bool CheckObjType(OBJ_TYPE type);

	void SetMove(const GameCtrl &ctl, weekListObj objList);

	void Draw();

	void ChangeState(AIState* s);

	void SetLongAttackFlag(bool flag) { longAttackFlag = flag; }

private:

	bool InitAnim(void);

	std::map<std::string, std::string> animFileName;		// ±ÆÒ°¼®İ‚Ì‰æ‘œÌ§²Ù–¼ (·°: ±ÆÒ°¼®İ–¼)


protected:

	bool Init(std::string fileName, VECTOR2 divSize, VECTOR2 divCut, VECTOR2 pos, bool turn);		// ‰Šú‰»	ˆø”: Ì§²Ù–¼, •ªŠ„»²½Ş, •ªŠ„”, À•W, ”½“]Ì×¸Ş, ƒpƒbƒh”Ô†

	AIState* state;

	bool longAttackFlag;			// ‰“‹——£UŒ‚ƒtƒ‰ƒO

	std::string characterName;
	std::string spAttackAnimName[3] = { "‹Z1", "‹Z2" , "‹Z3" };
	std::string spAttackAnimFileName[3] = { "waza_1", "waza_2" , "waza_3" };
};

