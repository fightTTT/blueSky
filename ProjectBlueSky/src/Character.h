#pragma once
#include "Obj.h"

#include <map>

enum PAD_ID;

class Character :
	public Obj
{
public:
	Character(VECTOR2 offset);
	~Character();

	bool CheckObjType(OBJ_TYPE type);

	void SetMove(const GameCtrl &ctl, weekListObj objList);

	void Draw(void);		// �`�揈��

private:
	bool InitAnim(void);

	std::map<std::string, std::string> animFileName;		// ��Ұ��݂̉摜̧�ٖ� (��: ��Ұ��ݖ�)

	PAD_ID padNum;		// �g�p���Ă���R���g���[���[�ԍ�
	bool jumpFlag;
	VECTOR2 jumpSpeed;

protected:
	bool Init(std::string fileName, VECTOR2 divSize, VECTOR2 divCut, VECTOR2 pos, bool turn, PAD_ID id);		// ������	����: ̧�ٖ�, ��������, ������, ���W, ���]�׸�, �p�b�h�ԍ�

	std::string characterName;
	std::string spAttackAnimName[3] = {"�Z1", "�Z2" , "�Z3" };
	std::string spAttackAnimFileName[3] = { "waza_1", "waza_2" , "waza_3" };
};

