#include "AIStickHuman.h"
#include "SceneMng.h"

AIStickHuman::AIStickHuman(VECTOR2 pos, VECTOR2 offset, DIR charaDir) : AICharacter(offset)
{
	shotCreateCount = 0;
	spAttackAnimName[0] = { "�Z1"};
	spAttackAnimName[1] = { "�Z2"};
	spAttackAnimName[2] = { "�Z3"};
	spAttackAnimFileName[0] = { "waza_1"};
	spAttackAnimFileName[1] = { "waza_2"};
	spAttackAnimFileName[2] = { "waza_3"};

	spAttackDamage["�S��r"] = -10;
	spAttackDamage["����"] = -30;
	spAttackDamage["�E�}�L�b�N"] = -15;
	spAttackDamage["���[�����O�A�^�b�N"] = -15;
	spAttackDamage["�����r"] = -15;
	spAttackDamage["�J���t�[�L�b�N"] = -30;
	spAttackDamage["���b�V��"] = -10;
	spAttackDamage["�񂵏R��"] = -20;
	spAttackDamage["�~�T�C���A���["] = -15;
	spAttackDamage["�����L���N"] = -20;
	spAttackDamage["�n�ʊ�"] = -25;
	spAttackDamage["�^�b�N��"] = -15;
	spAttackDamage["�����Ɨ��Ƃ�"] = -25;
	spAttackDamage["�A�N�Z��"] = -40;
	spAttackDamage["�R��Ԃ�"] = -15;


	switch (lpSceneMng.GetCharID(PAD_AI))
	{
	case 0:
		characterName = "�_�l��";
		AddAnim("�g��", 0, 0, 11, 4, false, 0, 0);
		spAttackAnimName[0] = "�g��";
		spAttackAnimFileName[0] = "hadou";
		animAttributeTbl["�g��"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["�g��"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["�g��"][2] = ANIM_ATTRIBUTE_SHOT;

		AddAnim("�S��r", 0, 0, 18, 4, false, 0, 0);
		spAttackAnimName[1] = "�S��r";
		spAttackAnimFileName[1] = "hyakuretsu";
		animAttributeTbl["�S��r"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["�S��r"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["�S��r"][2] = ANIM_ATTRIBUTE_NON;

		AddAnim("����", 0, 0, 16, 4, false, 0, 0);
		spAttackAnimName[2] = "����";
		spAttackAnimFileName[2] = "shoryu";
		animAttributeTbl["����"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["����"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["����"][2] = ANIM_ATTRIBUTE_INVINCIBLE;

		shotCreateCount = 30;

		break;

	case 1:
		characterName = "�_�l��_��";
		AddAnim("�h�˖C", 0, 0, 11, 4, false, 0, 0);
		spAttackAnimName[0] = "�h�˖C";
		spAttackAnimFileName[0] = "sitotsu";
		animAttributeTbl["�h�˖C"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["�h�˖C"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["�h�˖C"][2] = ANIM_ATTRIBUTE_SHOT;

		AddAnim("�E�}�L�b�N", 0, 0, 7, 5, false, 0, 0);
		spAttackAnimName[1] = "�E�}�L�b�N";
		spAttackAnimFileName[1] = "horseKick";
		animAttributeTbl["�E�}�L�b�N"][0] = ANIM_ATTRIBUTE_SQUAT;
		animAttributeTbl["�E�}�L�b�N"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["�E�}�L�b�N"][2] = ANIM_ATTRIBUTE_NON;

		AddAnim("���[�����O�A�^�b�N", 0, 0, 6, 1, true, 0, -16);
		spAttackAnimName[2] = "���[�����O�A�^�b�N";
		spAttackAnimFileName[2] = "rolling";
		animAttributeTbl["���[�����O�A�^�b�N"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["���[�����O�A�^�b�N"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["���[�����O�A�^�b�N"][2] = ANIM_ATTRIBUTE_NON;

		shotCreateCount = 30;

		break;

	case 2:
		characterName = "�_�l��_��";
		AddAnim("�C����", 0, 0, 9, 4, false, 0, 0);
		spAttackAnimName[0] = "�C����";
		spAttackAnimFileName[0] = "kikoh";
		animAttributeTbl["�C����"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["�C����"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["�C����"][2] = ANIM_ATTRIBUTE_SHOT;

		AddAnim("�����r", 0, 0, 4, 3, true, 0, -16);
		spAttackAnimName[1] = "�����r";
		spAttackAnimFileName[1] = "senpu";
		animAttributeTbl["�����r"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["�����r"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["�����r"][2] = ANIM_ATTRIBUTE_NON;

		AddAnim("�J���t�[�L�b�N", 0, 0, 17, 3, false, 0, 0);
		spAttackAnimName[2] = "�J���t�[�L�b�N";
		spAttackAnimFileName[2] = "kung";
		animAttributeTbl["�J���t�[�L�b�N"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["�J���t�[�L�b�N"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["�J���t�[�L�b�N"][2] = ANIM_ATTRIBUTE_INVINCIBLE;

		shotCreateCount = 30;

		break;

	case 3:
		characterName = "�_�l��_��";
		AddAnim("���߂͂ߔg", 0, 0, 10, 4, false, 0, 0);
		spAttackAnimName[0] = "���߂͂ߔg";
		spAttackAnimFileName[0] = "kamehame";
		animAttributeTbl["���߂͂ߔg"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["���߂͂ߔg"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["���߂͂ߔg"][2] = ANIM_ATTRIBUTE_SHOT;

		AddAnim("���b�V��", 0, 0, 15, 1, true, 0, 0);
		spAttackAnimName[1] = "���b�V��";
		spAttackAnimFileName[1] = "rush";
		animAttributeTbl["���b�V��"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["���b�V��"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["���b�V��"][2] = ANIM_ATTRIBUTE_NON;

		AddAnim("�񂵏R��", 0, 0, 26, 3, false, 0, 0);
		spAttackAnimName[2] = "�񂵏R��";
		spAttackAnimFileName[2] = "roundKick";
		animAttributeTbl["�񂵏R��"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["�񂵏R��"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["�񂵏R��"][2] = ANIM_ATTRIBUTE_INVINCIBLE;

		shotCreateCount = 30;

		break;

	case 4:
		characterName = "�_�l��_��";
		AddAnim("�\�j�b�N�u�[��", 0, 0, 7, 4, false, 0, 0);
		spAttackAnimName[0] = "�\�j�b�N�u�[��";
		spAttackAnimFileName[0] = "sonic";
		animAttributeTbl["�\�j�b�N�u�[��"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["�\�j�b�N�u�[��"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["�\�j�b�N�u�[��"][2] = ANIM_ATTRIBUTE_SHOT;

		AddAnim("�~�T�C���A���[", 0, 0, 6, 4, false, 0, -16);
		spAttackAnimName[1] = "�~�T�C���A���[";
		spAttackAnimFileName[1] = "missile";
		animAttributeTbl["�~�T�C���A���["][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["�~�T�C���A���["][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["�~�T�C���A���["][2] = ANIM_ATTRIBUTE_NON;

		AddAnim("�����L���N", 0, 0, 10, 3, false, 0, -16);
		spAttackAnimName[2] = "�����L���N";
		spAttackAnimFileName[2] = "cutterKick";
		animAttributeTbl["�����L���N"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["�����L���N"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["�����L���N"][2] = ANIM_ATTRIBUTE_INVINCIBLE;

		shotCreateCount = 22;

		break;

	case 5:
		characterName = "�_�l��_��";
		AddAnim("�Ⓤ��", 0, 0, 16, 4, false, 0, 0);
		spAttackAnimName[0] = "�Ⓤ��";
		spAttackAnimFileName[0] = "throw";
		animAttributeTbl["�Ⓤ��"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["�Ⓤ��"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["�Ⓤ��"][2] = ANIM_ATTRIBUTE_SHOT;

		AddAnim("�n�ʊ�", 0, 0, 22, 4, false, 50, 30);
		spAttackAnimName[1] = "�n�ʊ�";
		spAttackAnimFileName[1] = "split";
		animAttributeTbl["�n�ʊ�"][0] = ANIM_ATTRIBUTE_AIR;
		animAttributeTbl["�n�ʊ�"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["�n�ʊ�"][2] = ANIM_ATTRIBUTE_NON;

		AddAnim("�^�b�N��", 0, 0, 7, 4, false, 0, 0);
		spAttackAnimName[2] = "�^�b�N��";
		spAttackAnimFileName[2] = "tackle";
		animAttributeTbl["�^�b�N��"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["�^�b�N��"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["�^�b�N��"][2] = ANIM_ATTRIBUTE_NON;

		shotCreateCount = 45;

		break;

	case 6:
		characterName = "�_�l��_�s���N";
		AddAnim("�T�C�R�J�b�^�[", 0, 0, 9, 4, false, 0, 0);
		spAttackAnimName[0] = "�T�C�R�J�b�^�[";
		spAttackAnimFileName[0] = "psycho";
		animAttributeTbl["�T�C�R�J�b�^�["][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["�T�C�R�J�b�^�["][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["�T�C�R�J�b�^�["][2] = ANIM_ATTRIBUTE_SHOT;

		AddAnim("�����Ɨ��Ƃ�", 0, 0, 14, 4, false, 0, 0);
		spAttackAnimName[1] = "�����Ɨ��Ƃ�";
		spAttackAnimFileName[1] = "heeldrop";
		animAttributeTbl["�����Ɨ��Ƃ�"][0] = ANIM_ATTRIBUTE_AIR;
		animAttributeTbl["�����Ɨ��Ƃ�"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["�����Ɨ��Ƃ�"][2] = ANIM_ATTRIBUTE_NON;

		AddAnim("���[�v", 0, 0, 12, 5, false, 0, 0);
		spAttackAnimName[2] = "���[�v";
		spAttackAnimFileName[2] = "warp";
		animAttributeTbl["���[�v"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["���[�v"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["���[�v"][2] = ANIM_ATTRIBUTE_INVINCIBLE;

		shotCreateCount = 22;

		break;

	case 7:
		characterName = "�_�l��_���F";
		AddAnim("���̑���", 0, 0, 12, 4, false, 0, 0);
		spAttackAnimName[0] = "���̑���";
		spAttackAnimFileName[0] = "ibuki";
		animAttributeTbl["���̑���"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["���̑���"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["���̑���"][2] = ANIM_ATTRIBUTE_SHOT;

		AddAnim("�A�N�Z��", 0, 0, 21, 4, false, 0, 0);
		spAttackAnimName[1] = "�A�N�Z��";
		spAttackAnimFileName[1] = "accele";
		animAttributeTbl["�A�N�Z��"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["�A�N�Z��"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["�A�N�Z��"][2] = ANIM_ATTRIBUTE_NON;

		AddAnim("�R��Ԃ�", 0, 0, 14, 3, false, 0, 0);
		spAttackAnimName[2] = "�R��Ԃ�";
		spAttackAnimFileName[2] = "turn";
		animAttributeTbl["�R��Ԃ�"][0] = ANIM_ATTRIBUTE_AIR;
		animAttributeTbl["�R��Ԃ�"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["�R��Ԃ�"][2] = ANIM_ATTRIBUTE_NON;

		shotCreateCount = 26;

		break;

	default:
		break;
	}

	Init("image/" + characterName + "/�ҋ@/stand_0.png", VECTOR2(256, 256), VECTOR2(1, 1), VECTOR2(pos.x, pos.y), false);

	dir = charaDir;
}

AIStickHuman::~AIStickHuman()
{
}
