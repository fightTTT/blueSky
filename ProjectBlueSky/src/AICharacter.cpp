#include "AICharacter.h"
#include "ImageMng.h"
#include "Shot.h"
#include "CollisionMng.h"
#include "AIState.h"
#include "MoveState.h"
#include "WaitState.h"
#include "SceneMng.h"

#include "DxLib.h"

AICharacter::AICharacter(VECTOR2 offset) : Obj(offset)
{
	longAttackFlag = false;
	dirChangeFlag = true;
	jumpType = JUMP_TYPE_RAND;

	padID = PAD_AI;

	DrawHPCount = 0.0f;

	ChangeState(MoveState::GetInstance());
}

AICharacter::~AICharacter()
{
}

bool AICharacter::CheckObjType(OBJ_TYPE type)
{
	return (type == OBJ_TYPE_AICHARACTER);
}

void AICharacter::SetMove(const GameCtrl & ctl, weekListObj objList)
{
	if (dirChangeFlag)
	{
		// �L�����̌����ύX
		dir = tmpDir;
	}

	if (state)
	{
		state->Update(this);
	}

	if (longAttackFlag)
	{
		// �������U��
		if (dir == DIR_RIGHT)
		{
			AddObjList()(objList, std::make_unique<Shot>(pos + VECTOR2(256, 64), drawOffset, dir, padID));
		}
		else
		{
			AddObjList()(objList, std::make_unique<Shot>(pos + VECTOR2(-128, 64), drawOffset, dir, padID));
		}
	
		longAttackFlag = false;
	}
}

void AICharacter::Draw()
{
	if (imageName.length() == 0)
	{
		return;
	}
	int id = 0;
	VECTOR2 animOffset(0, 0);
	if (animTable.find(animName) != animTable.end())
	{
		id = animCnt / animTable[animName][ANIM_TBL_INV];
		if (animTable[animName][ANIM_TBL_LOOP] || id < animTable[animName][ANIM_TBL_FRAME])
		{
			id %= animTable[animName][ANIM_TBL_FRAME];
		}
		else
		{
			id = animTable[animName][ANIM_TBL_FRAME] - 1;
			animEndFlag = true;
		}

		if (dir == DIR_RIGHT)
		{
			animOffset = { animTable[animName][ANIM_TBL_OFFSET_X] , animTable[animName][ANIM_TBL_OFFSET_Y] };
		}
		else
		{
			animOffset = { -animTable[animName][ANIM_TBL_OFFSET_X] , animTable[animName][ANIM_TBL_OFFSET_Y] };
		}
	}

	std::string nowAnim = GetAnim();
	imageName = ("image/" + characterName + "/" + nowAnim + "/" + animFileName[nowAnim] + "_" + std::to_string(id) + ".png");

	// DIR��RIGHT�̏ꍇ�摜�𔽓]
	if (dir == DIR_LEFT)
	{
		turnFlag = true;
	}
	else
	{
		turnFlag = false;
	}

	if (visible)
	{
		SetDrawBright(100, 255, 100);
		DrawRotaGraph(drawOffset.x + animOffset.x + pos.x + (divSize.x / 2), drawOffset.y + animOffset.y + pos.y + (divSize.y / 2), 1.0, 0.0, IMAGE_ID(imageName)[0], true, turnFlag);
		SetDrawBright(255, 255, 255);
	}

	if (lpColMng.GetColFlag(animName))
	{
		ColInfo colData = lpColMng.GetCollisionData("�_�l��", animName, id);

		int colColor;

		for (int i = 0; i < colData.hitBox.size(); i++)
		{
			colData.hitBox[i].rect.startPos.x *= static_cast<int>(dir) * -2 + 1;
			colData.hitBox[i].rect.endPos.x *= static_cast<int>(dir) * -2 + 1;

			colColor = (colData.hitBox[i].type == COLTYPE_ATTACK ? 0xff0000 : (colData.hitBox[i].type == COLTYPE_HIT ? 0x0000ff : 0x00ff00));

			DrawBox(drawOffset.x + animOffset.x + pos.x + (divSize.x / 2) + colData.hitBox[i].rect.startPos.x, drawOffset.y + animOffset.y + pos.y + divSize.y + colData.hitBox[i].rect.startPos.y, drawOffset.x + animOffset.x +  pos.x + (divSize.x / 2) + colData.hitBox[i].rect.endPos.x, drawOffset.y + animOffset.y + pos.y + divSize.y + colData.hitBox[i].rect.endPos.y, colColor, false);
		}
	}

	auto ssize = lpSceneMng.GetScreenSize();
	int hpColor = 0;

	if (playerHP != playerHPOld)
	{
		DrawHPCount += 0.2f;

		playerHPOld -= static_cast<int>(DrawHPCount);

		if (static_cast<int>(DrawHPCount) == 1)
		{
			DrawHPCount = 0.0f;
		}
	}

	// HP�\��
	if (playerHP <= 50 && playerHP > 25)
	{
		hpColor = 0xffd900;
	}
	else if (playerHP <= 25)
	{
		hpColor = 0xff0000;
	}
	else
	{
		hpColor = 0x00ff00;
	}
	DrawFormatString(ssize.x - 410, 25, 0xffffff, "AI�̎c��HP %d \n", playerHP);
	DrawBox(ssize.x - 410, 65, ssize.x - 410 + 300, 85, 0x000000, true);
	DrawBox(ssize.x - 410, 65, ssize.x - 410 + (playerHPOld * 3), 85, 0xff0000, true);
	DrawBox(ssize.x - 410, 65, ssize.x - 410 + (playerHP * 3), 85, hpColor, true);
	DrawTurnGraph((ssize.x - 430), 55, IMAGE_ID("image/�Q�[���V�[���p/hpGage.png")[0], true);

	DrawTurnGraph(ssize.x - 400 + 300, 5, IMAGE_DIV_ID("image/�L�����Z���p/charIcon_small.png", VECTOR2(100, 100), VECTOR2(8, 1))[lpSceneMng.GetCharID(padID)], true);

	if (!animStopFlag)
	{
		animCnt++;
	}
}

void AICharacter::ChangeState(AIState * s)
{
	state = s;
	state->Init(this);
}

HitData AICharacter::GetHitData() const
{
	return hitData;
}

void AICharacter::CheckHitFlag()
{
	state->CheckHitFlag(this);
}

bool AICharacter::InitAnim(void)
{
	AddAnim("�ҋ@", 0, 0, 6, 4, true, 0, 0);
	AddAnim("�O�ړ�", 0, 0, 8, 4, true, 0, 0);
	AddAnim("���ړ�", 0, 0, 8, 4, true, 0, 0);
	AddAnim("�W�����v_��", 0, 0, 10, 4, true, 0, 0);
	AddAnim("�W�����v_�O", 0, 0, 6, 4, true, 0, 0);
	AddAnim("�W�����v_���", 0, 0, 6, 4, true, 0, 0);
	AddAnim("���Ⴊ��", 0, 0, 1, 4, true, 0, 0);
	AddAnim("���Ⴊ��_���", 0, 0, 1, 4, true, 0, 0);
	AddAnim("���Ⴊ�ݎn��", 0, 0, 2, 4, false, 0, 0);
	AddAnim("�����n��", 0, 0, 3, 4, false, 0, 0);
	AddAnim("�p���`_��", 0, 0, 6, 4, false, 0, 0);
	AddAnim("�p���`_��", 0, 0, 8, 4, false, 0, 0);
	AddAnim("�p���`_��_��", 0, 0, 4, 4, false, 0, 0);
	AddAnim("�p���`_��_��", 0, 0, 5, 4, false, 0, 0);
	AddAnim("�p���`_��_���Ⴊ��", 0, 0, 6, 4, false, 0, 0);
	AddAnim("�p���`_��_���Ⴊ��", 0, 0, 9, 4, false, 0, 0);
	AddAnim("�L�b�N_��", 0, 0, 6, 4, false, 64, 0);
	AddAnim("�L�b�N_��", 0, 0, 8, 4, false, 0, 0);
	AddAnim("�L�b�N_��_��", 0, 0, 4, 4, false, 0, 0);
	AddAnim("�L�b�N_��_��", 0, 0, 5, 4, false, 0, 0);
	AddAnim("�L�b�N_��_���Ⴊ��", 0, 0, 6, 4, false, 0, 0);
	AddAnim("�L�b�N_��_���Ⴊ��", 0, 0, 9, 4, false, 0, 0);
	AddAnim("�K�[�h_����", 0, 0, 1, 4, true, 0, 0);
	AddAnim("�K�[�h_���Ⴊ��", 0, 0, 1, 4, true, 0, 0);
	AddAnim("�_���[�W_����", 0, 0, 5, 4, false, 0, 0);
	AddAnim("�_���[�W_�_�E��", 0, 0, 13, 4, false, 0, 0);
	AddAnim("�N���オ��", 0, 0, 10, 4, false, 0, 0);
	SetAnim("�ҋ@");

	return true;
}

bool AICharacter::Init(std::string fileName, VECTOR2 divSize, VECTOR2 divCut, VECTOR2 pos, bool turn)
{
	Obj::Init(fileName, divSize, divCut, pos, turn);
	InitAnim();

	// �ʏ�̃A�N�V����
	animFileName["�ҋ@"] = "stand";
	animFileName["�O�ړ�"] = "move_front";
	animFileName["���ړ�"] = "move_back";
	animFileName["�W�����v_��"] = "jump";
	animFileName["�W�����v_�O"] = "jump_front";
	animFileName["�W�����v_���"] = "jump_back";
	animFileName["���Ⴊ��"] = "squat";
	animFileName["���Ⴊ��_���"] = "squat_back";
	animFileName["���Ⴊ�ݎn��"] = "squat_start";
	animFileName["�����n��"] = "stand_start";
	animFileName["�p���`_��"] = "punch_small";
	animFileName["�p���`_��"] = "punch_big";
	animFileName["�p���`_��_��"] = "punch_small_air";
	animFileName["�p���`_��_��"] = "punch_big_air";
	animFileName["�p���`_��_���Ⴊ��"] = "punch_small_squat";
	animFileName["�p���`_��_���Ⴊ��"] = "punch_big_squat";
	animFileName["�L�b�N_��"] = "kick_small";
	animFileName["�L�b�N_��"] = "kick_big";
	animFileName["�L�b�N_��_��"] = "kick_small_air";
	animFileName["�L�b�N_��_��"] = "kick_big_air";
	animFileName["�L�b�N_��_���Ⴊ��"] = "kick_small_squat";
	animFileName["�L�b�N_��_���Ⴊ��"] = "kick_big_squat";
	animFileName["�K�[�h_����"] = "guard";
	animFileName["�K�[�h_���Ⴊ��"] = "guard_squat";
	animFileName["�_���[�W_����"] = "damage";
	animFileName["�_���[�W_�_�E��"] = "damage_down";
	animFileName["�N���オ��"] = "get_up";

	animAttributeTbl["�ҋ@"][0] = ANIM_ATTRIBUTE_STAND;
	animAttributeTbl["�ҋ@"][1] = ANIM_ATTRIBUTE_NON;
	animAttributeTbl["�O�ړ�"][0] = ANIM_ATTRIBUTE_STAND;
	animAttributeTbl["�O�ړ�"][1] = ANIM_ATTRIBUTE_NON;
	animAttributeTbl["���ړ�"][0] = ANIM_ATTRIBUTE_STAND;
	animAttributeTbl["���ړ�"][1] = ANIM_ATTRIBUTE_NON;
	animAttributeTbl["�W�����v_��"][0] = ANIM_ATTRIBUTE_AIR;
	animAttributeTbl["�W�����v_��"][1] = ANIM_ATTRIBUTE_NON;
	animAttributeTbl["�W�����v_�O"][0] = ANIM_ATTRIBUTE_AIR;
	animAttributeTbl["�W�����v_�O"][1] = ANIM_ATTRIBUTE_NON;
	animAttributeTbl["�W�����v_���"][0] = ANIM_ATTRIBUTE_AIR;
	animAttributeTbl["�W�����v_���"][1] = ANIM_ATTRIBUTE_NON;
	animAttributeTbl["���Ⴊ��"][0] = ANIM_ATTRIBUTE_SQUAT;
	animAttributeTbl["���Ⴊ��"][1] = ANIM_ATTRIBUTE_NON;
	animAttributeTbl["���Ⴊ��_���"][0] = ANIM_ATTRIBUTE_SQUAT;
	animAttributeTbl["���Ⴊ��_���"][1] = ANIM_ATTRIBUTE_NON;
	animAttributeTbl["���Ⴊ�ݎn��"][0] = ANIM_ATTRIBUTE_SQUAT;
	animAttributeTbl["���Ⴊ�ݎn��"][1] = ANIM_ATTRIBUTE_NON;
	animAttributeTbl["�����n��"][0] = ANIM_ATTRIBUTE_STAND;
	animAttributeTbl["�����n��"][1] = ANIM_ATTRIBUTE_NON;
	animAttributeTbl["�p���`_��"][0] = ANIM_ATTRIBUTE_STAND;
	animAttributeTbl["�p���`_��"][1] = ANIM_ATTRIBUTE_ATTACK;
	animAttributeTbl["�p���`_��"][0] = ANIM_ATTRIBUTE_STAND;
	animAttributeTbl["�p���`_��"][1] = ANIM_ATTRIBUTE_ATTACK;
	animAttributeTbl["�p���`_��_��"][0] = ANIM_ATTRIBUTE_AIR;
	animAttributeTbl["�p���`_��_��"][1] = ANIM_ATTRIBUTE_ATTACK;
	animAttributeTbl["�p���`_��_��"][0] = ANIM_ATTRIBUTE_AIR;
	animAttributeTbl["�p���`_��_��"][1] = ANIM_ATTRIBUTE_ATTACK;
	animAttributeTbl["�p���`_��_���Ⴊ��"][0] = ANIM_ATTRIBUTE_SQUAT;
	animAttributeTbl["�p���`_��_���Ⴊ��"][1] = ANIM_ATTRIBUTE_ATTACK;
	animAttributeTbl["�p���`_��_���Ⴊ��"][0] = ANIM_ATTRIBUTE_SQUAT;
	animAttributeTbl["�p���`_��_���Ⴊ��"][1] = ANIM_ATTRIBUTE_ATTACK;
	animAttributeTbl["�L�b�N_��"][0] = ANIM_ATTRIBUTE_STAND;
	animAttributeTbl["�L�b�N_��"][1] = ANIM_ATTRIBUTE_ATTACK;
	animAttributeTbl["�L�b�N_��"][0] = ANIM_ATTRIBUTE_STAND;
	animAttributeTbl["�L�b�N_��"][1] = ANIM_ATTRIBUTE_ATTACK;
	animAttributeTbl["�L�b�N_��_��"][0] = ANIM_ATTRIBUTE_AIR;
	animAttributeTbl["�L�b�N_��_��"][1] = ANIM_ATTRIBUTE_ATTACK;
	animAttributeTbl["�L�b�N_��_��"][0] = ANIM_ATTRIBUTE_AIR;
	animAttributeTbl["�L�b�N_��_��"][1] = ANIM_ATTRIBUTE_ATTACK;
	animAttributeTbl["�L�b�N_��_���Ⴊ��"][0] = ANIM_ATTRIBUTE_SQUAT;
	animAttributeTbl["�L�b�N_��_���Ⴊ��"][1] = ANIM_ATTRIBUTE_ATTACK;
	animAttributeTbl["�L�b�N_��_���Ⴊ��"][0] = ANIM_ATTRIBUTE_SQUAT;
	animAttributeTbl["�L�b�N_��_���Ⴊ��"][1] = ANIM_ATTRIBUTE_ATTACK;
	animAttributeTbl["�K�[�h_����"][0] = ANIM_ATTRIBUTE_STAND;
	animAttributeTbl["�K�[�h_����"][1] = ANIM_ATTRIBUTE_GUARD;
	animAttributeTbl["�K�[�h_���Ⴊ��"][0] = ANIM_ATTRIBUTE_SQUAT;
	animAttributeTbl["�K�[�h_���Ⴊ��"][1] = ANIM_ATTRIBUTE_GUARD;
	animAttributeTbl["�_���[�W_����"][0] = ANIM_ATTRIBUTE_STAND;
	animAttributeTbl["�_���[�W_����"][1] = ANIM_ATTRIBUTE_NON;
	animAttributeTbl["�_���[�W_�_�E��"][0] = ANIM_ATTRIBUTE_AIR;
	animAttributeTbl["�_���[�W_�_�E��"][1] = ANIM_ATTRIBUTE_INVINCIBLE;
	animAttributeTbl["�N���オ��"][0] = ANIM_ATTRIBUTE_STAND;
	animAttributeTbl["�N���オ��"][1] = ANIM_ATTRIBUTE_INVINCIBLE;

	std::vector<std::string> animName = { "�ҋ@",
										  "�O�ړ�",
										  "���ړ�",
										  "�W�����v_��",
										  "�W�����v_�O",
										  "�W�����v_���",
										  "���Ⴊ��",
										  "���Ⴊ��_���",
										  "���Ⴊ�ݎn��",
										  "�����n��",
										  "�p���`_��",
										  "�p���`_��",
										  "�p���`_��_��",
										  "�p���`_��_��",
										  "�p���`_��_���Ⴊ��",
										  "�p���`_��_���Ⴊ��",
										  "�L�b�N_��",
										  "�L�b�N_��",
										  "�L�b�N_��_��",
										  "�L�b�N_��_��",
										  "�L�b�N_��_���Ⴊ��",
										  "�L�b�N_��_���Ⴊ��",
										  "�K�[�h_����",
										  "�K�[�h_���Ⴊ��",
										  "�_���[�W_����",
										  "�_���[�W_�_�E��",
										  "�N���オ��" };		// ��Ұ��ݖ���v�f�Ƃ��Ď���vector

	// �K�E�Z�n
	if ((spAttackAnimName[0] != "�Z1") && (spAttackAnimFileName[0] != "waza_1"))
	{
		animFileName[spAttackAnimName[0]] = spAttackAnimFileName[0];
		animName.push_back(spAttackAnimName[0]);
	}
	if ((spAttackAnimName[1] != "�Z2") && (spAttackAnimFileName[1] != "waza_2"))
	{
		animFileName[spAttackAnimName[1]] = spAttackAnimFileName[1];
		animName.push_back(spAttackAnimName[1]);
	}
	if ((spAttackAnimName[2] != "�Z3") && (spAttackAnimFileName[2] != "waza_3"))
	{
		animFileName[spAttackAnimName[2]] = spAttackAnimFileName[2];
		animName.push_back(spAttackAnimName[2]);
	}

	// �ꊇ�ǂݍ���
	lpImageMng.LoadImageCharacterAll(characterName, animName, animFileName);

	return true;
}
