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
	bool Init(std::string fileName, VECTOR2 divSize, VECTOR2 divCut, VECTOR2 pos);		// ������	����: ̧�ٖ�, ��������, ������, ���W

	std::string characterName;

	bool longAttackFlag;		// �������U���t���O
	VECTOR2 longAttackPos;		// �������U�����W
};

