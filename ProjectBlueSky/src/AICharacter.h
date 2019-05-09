#pragma once

#include "Obj.h"

enum AI_STATE_TYPE
{
	AI_STATE_NONE,
	AI_STATE_MOVE,
	AI_STATE_ATTACK,
	AI_STATE_JUMP,
	AI_STATE_MAX
};

class AICharacter :
	public Obj
{
public:
	AICharacter();
	~AICharacter();

	bool CheckObjType(OBJ_TYPE type);

	void SetMove(const GameCtrl &ctl, weekListObj objList);

	void Draw();

private:

	bool InitAnim(void);

	std::map<std::string, std::string> animFileName;		// ��Ұ��݂̉摜̧�ٖ� (��: ��Ұ��ݖ�)


protected:

	bool Init(std::string fileName, VECTOR2 divSize, VECTOR2 divCut, VECTOR2 pos, bool turn);		// ������	����: ̧�ٖ�, ��������, ������, ���W, ���]�׸�, �p�b�h�ԍ�

	AI_STATE_TYPE type;		// ���ݎ��s���̏��

	bool jumpFlag;
	VECTOR2 jumpSpeed;

	std::string characterName;
	std::string spAttackAnimName[3] = { "�Z1", "�Z2" , "�Z3" };
	std::string spAttackAnimFileName[3] = { "waza_1", "waza_2" , "waza_3" };
};

