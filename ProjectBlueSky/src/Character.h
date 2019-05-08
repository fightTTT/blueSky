#pragma once
#include "Obj.h"

#include <map>

class Character :
	public Obj
{
public:
	Character(VECTOR2 offset);
	~Character();

	bool CheckObjType(OBJ_TYPE type);

	void Draw(void);		// �`�揈��

private:
	bool InitAnim(void);

	std::map<std::string, std::string> animFileName;		// ��Ұ��݂̉摜̧�ٖ� (��: ��Ұ��ݖ�)

protected:
	bool Init(std::string fileName, VECTOR2 divSize, VECTOR2 divCut, VECTOR2 pos, bool turn);		// ������	����: ̧�ٖ�, ��������, ������, ���W, ���]�׸�

	std::string characterName;
	std::string spAttackAnimName[3] = {"�Z1", "�Z2" , "�Z3" };
	std::string spAttackAnimFileName[3] = { "waza_1", "waza_2" , "waza_3" };
};

