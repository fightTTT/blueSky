#pragma once

#include <vector>
#include <map>
#include "VECTOR2.h"

enum ColType
{
	COLTYPE_ATTACK,
	COLTYPE_HIT,
	COLTYPE_MAX
};

struct HitBox
{
	ColType type;

	bool rectFlag;	// �����蔻�肪��`���~��

	Rect rect;
	Circle circle;
};

struct ColInfo
{
	std::vector<HitBox> hitBox;
};

class Collision
{
public:
	Collision();
	~Collision();

	// ���݂̃A�j���[�V������hitbox�̔z����擾
	ColInfo GetColData(std::string animName, int animIdx) { return colData[animName][animIdx]; }

	// �w�肵���A�j���[�V��������ColInfo��ۑ�
	void SetColData(std::string animName, std::vector<ColInfo> info);

	// �w�肵���A�j���[�V�������̃f�[�^���ǂݍ��ݍς݂��ǂ���
	bool isLoad(std::string animName);

private:

	// �L�[ : �A�j���[�V������
	std::map<std::string, std::vector<ColInfo>> colData;
};

