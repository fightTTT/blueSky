#pragma once
#include "Obj.h"

#include <map>

class Character :
	public Obj
{
public:
	Character();
	~Character();

	bool InitAnim(void);		// ±ÆÒ°¼®İ‚Ì‰Šú‰»

	bool CheckObjType(OBJ_TYPE type);

	void Draw(void);		// •`‰æˆ—

private:
	std::map<std::string, std::string> animFileName;		// ±ÆÒ°¼®İ‚Ì‰æ‘œÌ§²Ù–¼ (·°: ±ÆÒ°¼®İ–¼)

protected:
	std::string characterName;
};

