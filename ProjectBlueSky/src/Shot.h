#pragma once

#include "Obj.h"

class Shot :
	public Obj
{
public:
	Shot(VECTOR2 pos, VECTOR2 offset, DIR dir, PAD_ID id);
	~Shot();

	void SetMove(const GameCtrl &ctl, weekListObj objList);

	bool CheckDeth();
	bool CheckObjType(OBJ_TYPE type);
	void Draw(void);

private:
	VECTOR2 ssize;
	int speed;
	void CheckHitFlag(void);

	bool deleteFlag;			// 削除フラグ

	PAD_ID padID;				// 発射したキャラクターのPADID

	VECTOR2 hitBoxSize;			// 当たり判定の矩形のｻｲｽﾞ
};

