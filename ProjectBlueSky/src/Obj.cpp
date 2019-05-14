#include"DxLib.h"
#include"SceneMng.h"
#include "ImageMng.h"
#include "Obj.h"

Obj::Obj()
{
	visible = true;
}

Obj::Obj(VECTOR2 drawOffset) :drawOffset(drawOffset)
{
	visible = true;
}

Obj::~Obj()
{
}

bool Obj::Init(std::string fileName, VECTOR2 divSize, VECTOR2 divCut, bool turn)
{
	lpImageMng.GetID(fileName, divSize, divCut);
	imageName = fileName;
	this->divSize = divSize;
	this->divCut = divCut;
	turnFlag = turn;
	return true;
}

bool Obj::Init(std::string fileName, VECTOR2 divSize, VECTOR2 divCut, VECTOR2 pos, bool turn)
{
	Init(fileName, divSize, divCut, turn);
	SetPos(pos);
	return true;
}

void Obj::UpDate(const GameCtrl &ctl)
{
	SetMove(ctl);
}

void Obj::UpDate(const GameCtrl & ctl, weekListObj objList)
{
	SetMove(ctl, objList);
}

void Obj::Draw(void)
{
	if (imageName.length() == 0)
	{
		return;
	}
	unsigned int id = 0;
	VECTOR2 animOffset(0, 0);
	if (animTable.find(animName) != animTable.end())
	{
		int count = animCnt / animTable[animName][ANIM_TBL_INV];
		if (animTable[animName][ANIM_TBL_LOOP] || count < animTable[animName][ANIM_TBL_FRAME])
		{
			count %= animTable[animName][ANIM_TBL_FRAME];
		}
		else
		{
			count = animTable[animName][ANIM_TBL_FRAME] - 1;
			animEndFlag = true;
		}

		id = animTable[animName][ANIM_TBL_START_ID] + count;

		if (dir == DIR_RIGHT)
		{
			animOffset = { animTable[animName][ANIM_TBL_OFFSET_X] , animTable[animName][ANIM_TBL_OFFSET_Y] };
		}
		else
		{
			animOffset = { -animTable[animName][ANIM_TBL_OFFSET_X] , animTable[animName][ANIM_TBL_OFFSET_Y] };
		}
	}

	if (id < IMAGE_ID(imageName).size())
	{
		if (visible)
		{
			DrawRotaGraph(drawOffset.x + animOffset.x + pos.x + (divSize.x / 2), drawOffset.y + animOffset.y + pos.y + (divSize.y / 2), 1.0, 0.0, IMAGE_ID(imageName)[id], true, turnFlag);
		}
	}
	animCnt++;
}

void Obj::Draw(unsigned int id)
{
	if (imageName.length() == 0)
	{
		return;
	}
	if (id < IMAGE_ID(imageName).size())
	{
		if (visible)
		{
			DrawRotaGraph(drawOffset.x + pos.x + (divSize.x / 2), drawOffset.y + pos.y + (divSize.y / 2), 1.0, 0.0, IMAGE_ID(imageName)[id], true, turnFlag);
		}
	}
}

const VECTOR2 & Obj::GetPos(void)
{
	return pos;
}

void Obj::AddPos(VECTOR2 addPos)
{
	pos += addPos;
}

std::string Obj::GetImageName(void)
{
	return imageName;
}

bool Obj::AddAnim(std::string animName, int id_x, int id_y, int frame, int inv, bool loop, int offset_x, int offset_y)
{
	animTable[animName][ANIM_TBL_START_ID] = (id_y * divCut.x) + (id_x);
	animTable[animName][ANIM_TBL_FRAME] = frame;
	animTable[animName][ANIM_TBL_INV] = inv;
	animTable[animName][ANIM_TBL_LOOP] = loop;
	animTable[animName][ANIM_TBL_OFFSET_X] = offset_x;
	animTable[animName][ANIM_TBL_OFFSET_Y] = offset_y;
	return true;
}

bool Obj::SetAnim(std::string animName)
{
	if (Obj::animName == animName)
	{
		return true;
	}
	if (animTable.find(animName) == animTable.end())
	{
		return false;
	}

	Obj::animName = animName;
	Obj::animCnt = 0;
	Obj::animEndFlag = false;

	return true;
}

const int Obj::GetAnimFrame(std::string animName)
{
	return animTable[animName][ANIM_TBL_FRAME];
}

std::string Obj::GetAnim(void)
{
	return animName;
}

void Obj::SetDir(DIR d)
{
	tmpDir = d;
}

void Obj::SetEnemyState(EnemyState state)
{
	enemyState = state;
}

void Obj::SetPos(VECTOR2 pos)
{
	this->pos = pos;
}

void Obj::SetHitData(bool flag, ColType type)
{
	hitData.colType = type;
	hitData.hitFlag = flag;
}

void Obj::SetMove(const GameCtrl & ctl)
{
}

void Obj::SetMove(const GameCtrl & ctl, weekListObj objList)
{
}
