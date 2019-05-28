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

	void ChangeState(std::string key);

	void CheckHitFlag();

	std::string GetSPAttackName(int idx);

	// �������U���t���O�Z�b�g
	void SetLongAttackFlag(bool flag) { longAttackFlag = flag; }

	// �W�����v�̃^�C�v
	void SetJumpType(JUMP_TYPE type) { jumpType = type; }
	JUMP_TYPE GetJumpType() { return jumpType; }

	// �����蔻������擾
	HitData GetHitData() const;

	// �e���˂܂ł̃J�E���g���擾
	const int GetShotCreateCount() const { return shotCreateCount; }

	bool isSPLongAttack(std::string spAnimName);

	void CheckDamage(ANIM_ATTRIBUTE att);

private:

	bool InitAnim(void);

	std::map<std::string, std::string> animFileName;		// ��Ұ��݂̉摜̧�ٖ� (��: ��Ұ��ݖ�)

	void AddStateObj(std::string key, std::shared_ptr<AIState> state);

	// state�N���X��ۑ�
	std::map<std::string, std::shared_ptr<AIState>> stateObj;

	// ���݂̃X�e�[�g��key
	std::string currentStateName;

protected:

	bool Init(std::string fileName, VECTOR2 divSize, VECTOR2 divCut, VECTOR2 pos, bool turn);

	bool longAttackFlag;			// �������U���t���O
	JUMP_TYPE jumpType;				// �W�����v�����̃^�C�v
	int shotCreateCount;			// shot�𐶐�����܂ł̃J�E���g

	std::map<std::string, int> spAttackDamage;

	std::string characterName;
	std::string spAttackAnimName[3];
	std::string spAttackAnimFileName[3];
};

