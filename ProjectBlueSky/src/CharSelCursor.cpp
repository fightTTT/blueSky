#include "CharSelCursor.h"
#include "SceneMng.h"
#include "DxLib.h"
#include "GameCtrl.h"

#define BOX_SIZE_X (100)		// ��ױ��݂�X������
#define BOX_SIZE_Y (100)		// ��ױ��݂�Y������


CharSelCursor::CharSelCursor()
{
	Init();
}


CharSelCursor::~CharSelCursor()
{
}

void CharSelCursor::SetMove(const GameCtrl & ctl, weekListObj objList)
{
	/* ���ق̈ړ� */
	if (ctl.GetPadDataTrg(PAD_1, THUMB_L_UP))
	{
		pos.y -= BOX_SIZE_Y;
	}
	if (ctl.GetPadDataTrg(PAD_1, THUMB_L_RIGHT))
	{
		pos.x += BOX_SIZE_X;
	}
	if (ctl.GetPadDataTrg(PAD_1, THUMB_L_DOWN))
	{
		pos.y += BOX_SIZE_Y;
	}
	if (ctl.GetPadDataTrg(PAD_1, THUMB_L_LEFT))
	{
		pos.x -= BOX_SIZE_X;
	}

	/* ���ق̈ړ����� */
	if (pos.x < (sSize.x / 2) - (BOX_SIZE_X * 2))
	{
		pos.x = (sSize.x / 2) - (BOX_SIZE_X * 2);
	}
	if (pos.x > (sSize.x / 2) + BOX_SIZE_X)
	{
		pos.x = (sSize.x / 2) + BOX_SIZE_X;
	}
	if (pos.y < (sSize.y * 3 / 5))
	{
		pos.y = (sSize.y * 3 / 5);
	}
	if (pos.y > (sSize.y * 3 / 5)+BOX_SIZE_Y)
	{
		pos.y = (sSize.y * 3 / 5) + BOX_SIZE_Y;
	}

}

bool CharSelCursor::CheckObjType(OBJ_TYPE type)
{
	return false;
}

int CharSelCursor::Init(void)
{
	sSize = lpSceneMng.GetScreenSize();
	pos.x = (sSize.x / 2) - (BOX_SIZE_X * 2);		// ���ق�X�������l
	pos.y = (sSize.y * 3 / 5);						// ���ق�Y�������l
	return 0;
}

void CharSelCursor::Draw(void)
{
	
	DrawBox(pos.x, pos.y, pos.x + BOX_SIZE_X, pos.y+ BOX_SIZE_Y, 0xffffff, false);
}

