#pragma once
#include "Obj.h"


class Character :
	public Obj
{
public:
	Character();
	~Character();

	bool InitAnim(void);		// ��Ұ��݂̏�����

	bool CheckObjType(OBJ_TYPE type);

	void Draw(void);		// �`�揈��

protected:
	std::string characterName;
};

