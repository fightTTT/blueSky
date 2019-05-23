#pragma once

#include "Obj.h"

enum JUMP_TYPE
{
	JUMP_TYPE_FRONT,
	JUMP_TYPE_BACK,
	JUMP_TYPE_UP,
	JUMP_TYPE_RAND,
	JUMP_TYPE_MAX
};

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

	std::string GetSPAttackName(int idx);

	// �������U���t���O�Z�b�g
	void SetLongAttackFlag(bool flag) { longAttackFlag = flag; }

	// �����ύX�t���O�Z�b�g
	void SetDirChange(bool flag) { dirChangeFlag = flag; }

	void SetJumpType(JUMP_TYPE type) { jumpType = type; }
	JUMP_TYPE GetJumpType() { return jumpType; }

	HitData GetHitData() const;

	void CheckHitFlag();

private:

	bool InitAnim(void);

	std::map<std::string, std::string> animFileName;		// ��Ұ��݂̉摜̧�ٖ� (��: ��Ұ��ݖ�)


protected:

	bool Init(std::string fileName, VECTOR2 divSize, VECTOR2 divCut, VECTOR2 pos, bool turn);		// ������	����: ̧�ٖ�, ��������, ������, ���W, ���]�׸�, �p�b�h�ԍ�

	AIState* state;

	bool longAttackFlag;			// �������U���t���O
	bool dirChangeFlag;				// �����ύX�t���O
	JUMP_TYPE jumpType;				// �W�����v�����̃^�C�v
	int shotCreateCount;			// shot�𐶐�����܂ł̃J�E���g

	std::string characterName;
	std::string spAttackAnimName[3];
	std::string spAttackAnimFileName[3];
};

