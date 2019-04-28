#pragma once
#include "Obj.h"


class Character :
	public Obj
{
public:
	Character();
	~Character();

	bool InitAnim(void);		// ±ÆÒ°¼®İ‚Ì‰Šú‰»

	bool CheckObjType(OBJ_TYPE type);

	void Draw(void);		// •`‰æˆ—

protected:
	std::string characterName;
};

