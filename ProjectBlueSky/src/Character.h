#pragma once
#include "Obj.h"

#include <map>
#include <list>
#include <array>
#include <vector>

enum PAD_ID;

enum SKILL_TYPE
{
	SKILL_TYPE_PUNCH,
	SKILL_TYPE_KICK,
	SKILL_TYPE_MAX
};

enum COM_DIR
{
	COM_DIR_CENTER,
	COM_DIR_UP,
	COM_DIR_RIGHT_UP,
	COM_DIR_RIGHT,
	COM_DIR_RIGHT_DOWN,
	COM_DIR_DOWN,
	COM_DIR_LEFT_DOWN,
	COM_DIR_LEFT,
	COM_DIR_LEFT_UP,
	COM_DIR_ACCUMULATE,
	COM_DIR_MAX
};

class Character :
	public Obj
{
public:
	Character(VECTOR2 offset);
	~Character();

	bool CheckObjType(OBJ_TYPE type);

	void SetMove(const GameCtrl &ctl, weekListObj objList);			// ���X�V
	void CheckHitFlag(void);										// �����蔻��̏��������ƁA����ɔ����ǉ��ōs������

	void Draw(void);		// �`�揈��

private:
	bool InitAnim(void);
	void CommandUpDate(const GameCtrl & ctl);
	bool CheckCommand(int skillNum);

	VECTOR2 ssize;

	std::map<std::string, std::string> animFileName;		// ��Ұ��݂̉摜̧�ٖ� (��: ��Ұ��ݖ�)

	std::list<COM_DIR> comList;
	COM_DIR comDir;
	COM_DIR comDirOld;
	int comClearCnt;

	int comboCnt;

	VECTOR2 jumpSpeed;

	VECTOR2 fallSpeed;
	
	int knockBackSpeed;
	bool knockBackFlag;

protected:
	bool Init(std::string fileName, VECTOR2 divSize, VECTOR2 divCut, VECTOR2 pos, bool turn, PAD_ID id);		// ������	����: ̧�ٖ�, ��������, ������, ���W, ���]�׸�, �p�b�h�ԍ�

	std::string characterName;
	std::string spAttackAnimName[3] = {"�Z1", "�Z2" , "�Z3" };
	std::string spAttackAnimFileName[3] = { "waza_1", "waza_2" , "waza_3" };
	SKILL_TYPE spAttackType[3] = { SKILL_TYPE_MAX,SKILL_TYPE_MAX,SKILL_TYPE_MAX };
	std::array<std::array<std::vector<COM_DIR>, DIR_MAX>, 3> spAttackCommand;		// spAttackCommand[�Z�ԍ�][�����̕���][�R�}���h]
};

