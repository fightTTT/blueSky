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

	bool deleteFlag;			// �폜�t���O

	PAD_ID padID;				// ���˂����L�����N�^�[��PADID

	VECTOR2 hitBoxSize;			// �����蔻��̋�`�̻���
};

