#pragma once
#include "Obj.h"

#include <map>

class Character :
	public Obj
{
public:
	Character();
	~Character();

	bool InitAnim(void);		// ｱﾆﾒｰｼｮﾝの初期化

	bool CheckObjType(OBJ_TYPE type);

	void Draw(void);		// 描画処理

private:
	std::map<std::string, std::string> animFileName;		// ｱﾆﾒｰｼｮﾝの画像ﾌｧｲﾙ名 (ｷｰ: ｱﾆﾒｰｼｮﾝ名)

protected:
	std::string characterName;
};

