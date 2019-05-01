#pragma once
#include "Obj.h"

#include <map>

class Character :
	public Obj
{
public:
	Character();
	~Character();

	bool InitAnim(void);		// ��Ұ��݂̏�����

	bool CheckObjType(OBJ_TYPE type);

	void Draw(void);		// �`�揈��

private:
	std::map<std::string, std::string> animFileName;		// ��Ұ��݂̉摜̧�ٖ� (��: ��Ұ��ݖ�)

protected:
	std::string characterName;
};

