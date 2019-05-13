#pragma once

#include "Obj.h"

class AIState;

class AICharacter :
	public Obj
{
public:
	AICharacter(VECTOR2 offset);
	~AICharacter();

	bool CheckObjType(OBJ_TYPE type);

	void SetMove(const GameCtrl &ctl, weekListObj objList);

	void Draw();

	void ChangeState(AIState* s);

	// �������U���t���O�Z�b�g
	void SetLongAttackFlag(bool flag) { longAttackFlag = flag; }

	// �W�����v����t���O�擾
	bool GetShotJumpFlag() const { return shotJumpFlag; }
	void SetShotJumpFlag(bool flag) { shotJumpFlag = flag; }

	// �����ύX�t���O�Z�b�g
	void SetDirChange(bool flag) { dirChangeFlag = flag; }

private:

	bool InitAnim(void);

	std::map<std::string, std::string> animFileName;		// ��Ұ��݂̉摜̧�ٖ� (��: ��Ұ��ݖ�)


protected:

	bool Init(std::string fileName, VECTOR2 divSize, VECTOR2 divCut, VECTOR2 pos, bool turn);		// ������	����: ̧�ٖ�, ��������, ������, ���W, ���]�׸�, �p�b�h�ԍ�

	AIState* state;

	bool longAttackFlag;			// �������U���t���O
	bool shotJumpFlag;				// �e�̃W�����v����t���O
	bool dirChangeFlag;				// �����ύX�t���O

	std::string characterName;
	std::string spAttackAnimName[3] = { "�Z1", "�Z2" , "�Z3" };
	std::string spAttackAnimFileName[3] = { "waza_1", "waza_2" , "waza_3" };
};

