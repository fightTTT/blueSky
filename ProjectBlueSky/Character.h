#pragma once
#include "Obj.h"


class Character :
	public Obj
{
public:
	Character();
	~Character();

	bool InitAnim(void);		// ｱﾆﾒｰｼｮﾝの初期化

	bool CheckObjType(OBJ_TYPE type);

	void Draw(void);		// 描画処理

protected:
	std::string characterName;
};

