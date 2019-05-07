#pragma once
#include "Obj.h"

#include <map>

class Character :
	public Obj
{
public:
	Character();
	~Character();

	bool CheckObjType(OBJ_TYPE type);

	void Draw(void);		// •`‰æˆ—

private:
	bool InitAnim(void);

	std::map<std::string, std::string> animFileName;		// ±ÆÒ°¼®İ‚Ì‰æ‘œÌ§²Ù–¼ (·°: ±ÆÒ°¼®İ–¼)

protected:
	bool Init(std::string fileName, VECTOR2 divSize, VECTOR2 divCut, VECTOR2 pos);		// ‰Šú‰»	ˆø”: Ì§²Ù–¼, •ªŠ„»²½Ş, •ªŠ„”, À•W

	std::string characterName;
};

