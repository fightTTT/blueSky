#include "SceneMng.h"
#include "StickHuman.h"


StickHuman::StickHuman(VECTOR2 pos, VECTOR2 offset, PAD_ID id, DIR charaDir) : Character(offset)
{
	switch (lpSceneMng.GetCharID(id))
	{
	case 0:
		characterName = "�_�l��";
		AddAnim("�g��", 0, 0, 11, 4, false, 0, 0);
		spAttackAnimName[0] = "�g��";
		spAttackAnimFileName[0] = "hadou";
		spAttackType[0] = SKILL_TYPE_PUNCH;
		animAttributeTbl["�g��"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["�g��"][1] = ANIM_ATTRIBUTE_NON;

		spAttackCommand[0][DIR_RIGHT].resize(3);
		spAttackCommand[0][DIR_RIGHT] = { COM_DIR_DOWN, COM_DIR_RIGHT_DOWN, COM_DIR_RIGHT };
		spAttackCommand[0][DIR_LEFT].resize(3);
		spAttackCommand[0][DIR_LEFT] = { COM_DIR_DOWN, COM_DIR_LEFT_DOWN, COM_DIR_LEFT };

		AddAnim("�S��r", 0, 0, 18, 4, false, 0, 0);
		spAttackAnimName[1] = "�S��r";
		spAttackAnimFileName[1] = "hyakuretsu";
		spAttackType[1] = SKILL_TYPE_KICK;
		animAttributeTbl["�S��r"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["�S��r"][1] = ANIM_ATTRIBUTE_NON;

		spAttackCommand[1][DIR_RIGHT].resize(2);
		spAttackCommand[1][DIR_RIGHT] = { COM_DIR_CENTER, COM_DIR_ACCUMULATE };
		spAttackCommand[1][DIR_LEFT].resize(2);
		spAttackCommand[1][DIR_LEFT] = { COM_DIR_CENTER, COM_DIR_ACCUMULATE };

		AddAnim("����", 0, 0, 16, 4, false, 0, 0);
		spAttackAnimName[2] = "����";
		spAttackAnimFileName[2] = "shoryu";
		spAttackType[2] = SKILL_TYPE_PUNCH;
		animAttributeTbl["����"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["����"][1] = ANIM_ATTRIBUTE_INVINCIBLE;

		spAttackCommand[2][DIR_RIGHT].resize(3);
		spAttackCommand[2][DIR_RIGHT] = { COM_DIR_RIGHT, COM_DIR_DOWN, COM_DIR_RIGHT_DOWN };
		spAttackCommand[2][DIR_LEFT].resize(3);
		spAttackCommand[2][DIR_LEFT] = { COM_DIR_LEFT, COM_DIR_DOWN, COM_DIR_LEFT_DOWN };
		break;

	case 1:
		characterName = "�_�l��_��";
		AddAnim("�h�˖C", 0, 0, 11, 4, false, 0, 0);
		spAttackAnimName[0] = "�h�˖C";
		spAttackAnimFileName[0] = "sitotsu";
		spAttackType[0] = SKILL_TYPE_PUNCH;
		animAttributeTbl["�h�˖C"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["�h�˖C"][1] = ANIM_ATTRIBUTE_NON;

		spAttackCommand[0][DIR_RIGHT].resize(3);
		spAttackCommand[0][DIR_RIGHT] = { COM_DIR_DOWN, COM_DIR_RIGHT_DOWN, COM_DIR_RIGHT };
		spAttackCommand[0][DIR_LEFT].resize(3);
		spAttackCommand[0][DIR_LEFT] = { COM_DIR_DOWN, COM_DIR_LEFT_DOWN, COM_DIR_LEFT };

		AddAnim("�E�}�L�b�N", 0, 0, 7, 4, false, 0, 0);
		spAttackAnimName[1] = "�E�}�L�b�N";
		spAttackAnimFileName[1] = "horseKick";
		spAttackType[1] = SKILL_TYPE_KICK;
		animAttributeTbl["�E�}�L�b�N"][0] = ANIM_ATTRIBUTE_SQUAT;
		animAttributeTbl["�E�}�L�b�N"][1] = ANIM_ATTRIBUTE_NON;

		spAttackCommand[1][DIR_RIGHT].resize(3);
		spAttackCommand[1][DIR_RIGHT] = { COM_DIR_LEFT, COM_DIR_LEFT_DOWN, COM_DIR_DOWN };
		spAttackCommand[1][DIR_LEFT].resize(3);
		spAttackCommand[1][DIR_LEFT] = { COM_DIR_RIGHT, COM_DIR_RIGHT_DOWN, COM_DIR_DOWN };

		AddAnim("���[�����O�A�^�b�N", 0, 0, 6, 4, false, 0, 0);
		spAttackAnimName[2] = "���[�����O�A�^�b�N";
		spAttackAnimFileName[2] = "rolling";
		spAttackType[2] = SKILL_TYPE_PUNCH;
		animAttributeTbl["���[�����O�A�^�b�N"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["���[�����O�A�^�b�N"][1] = ANIM_ATTRIBUTE_NON;

		spAttackCommand[2][DIR_RIGHT].resize(3);
		spAttackCommand[2][DIR_RIGHT] = { COM_DIR_LEFT, COM_DIR_ACCUMULATE, COM_DIR_RIGHT };
		spAttackCommand[2][DIR_LEFT].resize(3);
		spAttackCommand[2][DIR_LEFT] = { COM_DIR_RIGHT, COM_DIR_ACCUMULATE, COM_DIR_LEFT };
		break;

	case 2:
		characterName = "�_�l��_��";
		AddAnim("�C����", 0, 0, 9, 4, false, 0, 0);
		spAttackAnimName[0] = "�C����";
		spAttackAnimFileName[0] = "kikoh";
		spAttackType[0] = SKILL_TYPE_PUNCH;
		animAttributeTbl["�C����"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["�C����"][1] = ANIM_ATTRIBUTE_NON;

		spAttackCommand[0][DIR_RIGHT].resize(3);
		spAttackCommand[0][DIR_RIGHT] = { COM_DIR_DOWN, COM_DIR_RIGHT_DOWN, COM_DIR_RIGHT };
		spAttackCommand[0][DIR_LEFT].resize(3);
		spAttackCommand[0][DIR_LEFT] = { COM_DIR_DOWN, COM_DIR_LEFT_DOWN, COM_DIR_LEFT };

		AddAnim("�����r", 0, 0, 4, 4, true, 0, 0);
		spAttackAnimName[1] = "�����r";
		spAttackAnimFileName[1] = "senpu";
		spAttackType[1] = SKILL_TYPE_KICK;

		spAttackCommand[1][DIR_RIGHT].resize(3);
		spAttackCommand[1][DIR_RIGHT] = { COM_DIR_DOWN, COM_DIR_LEFT_DOWN, COM_DIR_LEFT };
		spAttackCommand[1][DIR_LEFT].resize(3);
		spAttackCommand[1][DIR_LEFT] = { COM_DIR_DOWN, COM_DIR_RIGHT_DOWN, COM_DIR_RIGHT };

		AddAnim("�J���t�[�L�b�N", 0, 0, 17, 4, false, 0, 0);
		spAttackAnimName[2] = "�J���t�[�L�b�N";
		spAttackAnimFileName[2] = "kung";
		spAttackType[2] = SKILL_TYPE_PUNCH;
		animAttributeTbl["�J���t�[�L�b�N"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["�J���t�[�L�b�N"][1] = ANIM_ATTRIBUTE_INVINCIBLE;

		spAttackCommand[2][DIR_RIGHT].resize(3);
		spAttackCommand[2][DIR_RIGHT] = { COM_DIR_RIGHT, COM_DIR_DOWN, COM_DIR_RIGHT_DOWN };
		spAttackCommand[2][DIR_LEFT].resize(3);
		spAttackCommand[2][DIR_LEFT] = { COM_DIR_LEFT, COM_DIR_DOWN, COM_DIR_LEFT_DOWN };
		break;

	case 3:
		characterName = "�_�l��_��";
		AddAnim("���߂͂ߔg", 0, 0, 10, 4, false, 0, 0);
		spAttackAnimName[0] = "���߂͂ߔg";
		spAttackAnimFileName[0] = "kamehame";
		spAttackType[0] = SKILL_TYPE_PUNCH;
		animAttributeTbl["���߂͂ߔg"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["���߂͂ߔg"][1] = ANIM_ATTRIBUTE_NON;

		spAttackCommand[0][DIR_RIGHT].resize(3);
		spAttackCommand[0][DIR_RIGHT] = { COM_DIR_DOWN, COM_DIR_RIGHT_DOWN, COM_DIR_RIGHT };
		spAttackCommand[0][DIR_LEFT].resize(3);
		spAttackCommand[0][DIR_LEFT] = { COM_DIR_DOWN, COM_DIR_LEFT_DOWN, COM_DIR_LEFT };

		AddAnim("���b�V��", 0, 0, 15, 4, false, 0, 0);
		spAttackAnimName[1] = "���b�V��";
		spAttackAnimFileName[1] = "rush";
		spAttackType[1] = SKILL_TYPE_KICK;

		spAttackCommand[1][DIR_RIGHT].resize(3);
		spAttackCommand[1][DIR_RIGHT] = { COM_DIR_DOWN, COM_DIR_LEFT_DOWN, COM_DIR_LEFT };
		spAttackCommand[1][DIR_LEFT].resize(3);
		spAttackCommand[1][DIR_LEFT] = { COM_DIR_DOWN, COM_DIR_RIGHT_DOWN, COM_DIR_RIGHT };

		AddAnim("�񂵏R��", 0, 0, 26, 4, false, 0, 0);
		spAttackAnimName[2] = "�񂵏R��";
		spAttackAnimFileName[2] = "roundKick";
		spAttackType[2] = SKILL_TYPE_PUNCH;
		animAttributeTbl["�񂵏R��"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["�񂵏R��"][1] = ANIM_ATTRIBUTE_INVINCIBLE;

		spAttackCommand[2][DIR_RIGHT].resize(3);
		spAttackCommand[2][DIR_RIGHT] = { COM_DIR_RIGHT, COM_DIR_DOWN, COM_DIR_RIGHT_DOWN };
		spAttackCommand[2][DIR_LEFT].resize(3);
		spAttackCommand[2][DIR_LEFT] = { COM_DIR_LEFT, COM_DIR_DOWN, COM_DIR_LEFT_DOWN };
		break;

	case 4:
		characterName = "�_�l��_��";
		AddAnim("�\�j�b�N�u�[��", 0, 0, 7, 4, false, 0, 0);
		spAttackAnimName[0] = "�\�j�b�N�u�[��";
		spAttackAnimFileName[0] = "sonic";
		spAttackType[0] = SKILL_TYPE_PUNCH;
		animAttributeTbl["�\�j�b�N�u�[��"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["�\�j�b�N�u�[��"][1] = ANIM_ATTRIBUTE_NON;

		spAttackCommand[0][DIR_RIGHT].resize(3);
		spAttackCommand[0][DIR_RIGHT] = { COM_DIR_DOWN, COM_DIR_RIGHT_DOWN, COM_DIR_RIGHT };
		spAttackCommand[0][DIR_LEFT].resize(3);
		spAttackCommand[0][DIR_LEFT] = { COM_DIR_DOWN, COM_DIR_LEFT_DOWN, COM_DIR_LEFT };

		AddAnim("�����L���N", 0, 0, 10, 4, false, 0, 0);
		spAttackAnimName[1] = "�����L���N";
		spAttackAnimFileName[1] = "cutterKick";
		spAttackType[1] = SKILL_TYPE_KICK;

		spAttackCommand[1][DIR_RIGHT].resize(3);
		spAttackCommand[1][DIR_RIGHT] = { COM_DIR_DOWN, COM_DIR_LEFT_DOWN, COM_DIR_LEFT };
		spAttackCommand[1][DIR_LEFT].resize(3);
		spAttackCommand[1][DIR_LEFT] = { COM_DIR_DOWN, COM_DIR_RIGHT_DOWN, COM_DIR_RIGHT };

		AddAnim("�~�T�C���A���[", 0, 0, 6, 4, false, 0, 0);
		spAttackAnimName[2] = "�~�T�C���A���[";
		spAttackAnimFileName[2] = "missile";
		spAttackType[2] = SKILL_TYPE_PUNCH;
		animAttributeTbl["�~�T�C���A���["][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["�~�T�C���A���["][1] = ANIM_ATTRIBUTE_INVINCIBLE;

		spAttackCommand[2][DIR_RIGHT].resize(3);
		spAttackCommand[2][DIR_RIGHT] = { COM_DIR_RIGHT, COM_DIR_DOWN, COM_DIR_RIGHT_DOWN };
		spAttackCommand[2][DIR_LEFT].resize(3);
		spAttackCommand[2][DIR_LEFT] = { COM_DIR_LEFT, COM_DIR_DOWN, COM_DIR_LEFT_DOWN };
		break;

	case 5:
		characterName = "�_�l��_��";
		AddAnim("�Ⓤ��", 0, 0, 16, 4, false, 0, 0);
		spAttackAnimName[0] = "�Ⓤ��";
		spAttackAnimFileName[0] = "throw";
		spAttackType[0] = SKILL_TYPE_PUNCH;
		animAttributeTbl["�Ⓤ��"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["�Ⓤ��"][1] = ANIM_ATTRIBUTE_NON;

		spAttackCommand[0][DIR_RIGHT].resize(3);
		spAttackCommand[0][DIR_RIGHT] = { COM_DIR_DOWN, COM_DIR_RIGHT_DOWN, COM_DIR_RIGHT };
		spAttackCommand[0][DIR_LEFT].resize(3);
		spAttackCommand[0][DIR_LEFT] = { COM_DIR_DOWN, COM_DIR_LEFT_DOWN, COM_DIR_LEFT };

		AddAnim("�n�ʊ�", 0, 0, 15, 4, false, 0, 0);
		spAttackAnimName[1] = "�n�ʊ�";
		spAttackAnimFileName[1] = "split";
		spAttackType[1] = SKILL_TYPE_KICK;

		spAttackCommand[1][DIR_RIGHT].resize(3);
		spAttackCommand[1][DIR_RIGHT] = { COM_DIR_DOWN, COM_DIR_LEFT_DOWN, COM_DIR_LEFT };
		spAttackCommand[1][DIR_LEFT].resize(3);
		spAttackCommand[1][DIR_LEFT] = { COM_DIR_DOWN, COM_DIR_RIGHT_DOWN, COM_DIR_RIGHT };

		AddAnim("�^�b�N��", 0, 0, 7, 4, false, 0, 0);
		spAttackAnimName[2] = "�^�b�N��";
		spAttackAnimFileName[2] = "tackle";
		spAttackType[2] = SKILL_TYPE_PUNCH;
		animAttributeTbl["�^�b�N��"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["�^�b�N��"][1] = ANIM_ATTRIBUTE_INVINCIBLE;

		spAttackCommand[2][DIR_RIGHT].resize(3);
		spAttackCommand[2][DIR_RIGHT] = { COM_DIR_RIGHT, COM_DIR_DOWN, COM_DIR_RIGHT_DOWN };
		spAttackCommand[2][DIR_LEFT].resize(3);
		spAttackCommand[2][DIR_LEFT] = { COM_DIR_LEFT, COM_DIR_DOWN, COM_DIR_LEFT_DOWN };
		break;

	case 6:
		characterName = "�_�l��_�s���N";
		AddAnim("�T�C�R�J�b�^�[", 0, 0, 9, 4, false, 0, 0);
		spAttackAnimName[0] = "�T�C�R�J�b�^�[";
		spAttackAnimFileName[0] = "psycho";
		spAttackType[0] = SKILL_TYPE_PUNCH;
		animAttributeTbl["�T�C�R�J�b�^�["][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["�T�C�R�J�b�^�["][1] = ANIM_ATTRIBUTE_NON;

		spAttackCommand[0][DIR_RIGHT].resize(3);
		spAttackCommand[0][DIR_RIGHT] = { COM_DIR_DOWN, COM_DIR_RIGHT_DOWN, COM_DIR_RIGHT };
		spAttackCommand[0][DIR_LEFT].resize(3);
		spAttackCommand[0][DIR_LEFT] = { COM_DIR_DOWN, COM_DIR_LEFT_DOWN, COM_DIR_LEFT };

		AddAnim("���[�v", 0, 0, 11, 4, false, 0, 0);
		spAttackAnimName[1] = "���[�v";
		spAttackAnimFileName[1] = "warp";
		spAttackType[1] = SKILL_TYPE_KICK;

		spAttackCommand[1][DIR_RIGHT].resize(3);
		spAttackCommand[1][DIR_RIGHT] = { COM_DIR_DOWN, COM_DIR_LEFT_DOWN, COM_DIR_LEFT };
		spAttackCommand[1][DIR_LEFT].resize(3);
		spAttackCommand[1][DIR_LEFT] = { COM_DIR_DOWN, COM_DIR_RIGHT_DOWN, COM_DIR_RIGHT };

		AddAnim("�����Ɨ��Ƃ�", 0, 0, 14, 4, false, 0, 0);
		spAttackAnimName[2] = "�����Ɨ��Ƃ�";
		spAttackAnimFileName[2] = "heeldrop";
		spAttackType[2] = SKILL_TYPE_PUNCH;
		animAttributeTbl["�����Ɨ��Ƃ�"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["�����Ɨ��Ƃ�"][1] = ANIM_ATTRIBUTE_INVINCIBLE;

		spAttackCommand[2][DIR_RIGHT].resize(3);
		spAttackCommand[2][DIR_RIGHT] = { COM_DIR_RIGHT, COM_DIR_DOWN, COM_DIR_RIGHT_DOWN };
		spAttackCommand[2][DIR_LEFT].resize(3);
		spAttackCommand[2][DIR_LEFT] = { COM_DIR_LEFT, COM_DIR_DOWN, COM_DIR_LEFT_DOWN };
		break;

	case 7:
		characterName = "�_�l��_���F";
		AddAnim("���̑���", 0, 0, 12, 4, false, 0, 0);
		spAttackAnimName[0] = "���̑���";
		spAttackAnimFileName[0] = "ibuki";
		spAttackType[0] = SKILL_TYPE_PUNCH;
		animAttributeTbl["���̑���"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["���̑���"][1] = ANIM_ATTRIBUTE_NON;

		spAttackCommand[0][DIR_RIGHT].resize(3);
		spAttackCommand[0][DIR_RIGHT] = { COM_DIR_DOWN, COM_DIR_RIGHT_DOWN, COM_DIR_RIGHT };
		spAttackCommand[0][DIR_LEFT].resize(3);
		spAttackCommand[0][DIR_LEFT] = { COM_DIR_DOWN, COM_DIR_LEFT_DOWN, COM_DIR_LEFT };

		AddAnim("�\�[����", 0, 0, 13, 4, false, 0, 0);
		spAttackAnimName[1] = "�\�[����";
		spAttackAnimFileName[1] = "soran";
		spAttackType[1] = SKILL_TYPE_KICK;

		spAttackCommand[1][DIR_RIGHT].resize(3);
		spAttackCommand[1][DIR_RIGHT] = { COM_DIR_DOWN, COM_DIR_LEFT_DOWN, COM_DIR_LEFT };
		spAttackCommand[1][DIR_LEFT].resize(3);
		spAttackCommand[1][DIR_LEFT] = { COM_DIR_DOWN, COM_DIR_RIGHT_DOWN, COM_DIR_RIGHT };

		AddAnim("�A�N�Z��", 0, 0, 21, 4, false, 0, 0);
		spAttackAnimName[2] = "�A�N�Z��";
		spAttackAnimFileName[2] = "accele";
		spAttackType[2] = SKILL_TYPE_PUNCH;
		animAttributeTbl["�A�N�Z��"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["�A�N�Z��"][1] = ANIM_ATTRIBUTE_INVINCIBLE;

		spAttackCommand[2][DIR_RIGHT].resize(3);
		spAttackCommand[2][DIR_RIGHT] = { COM_DIR_RIGHT, COM_DIR_DOWN, COM_DIR_RIGHT_DOWN };
		spAttackCommand[2][DIR_LEFT].resize(3);
		spAttackCommand[2][DIR_LEFT] = { COM_DIR_LEFT, COM_DIR_DOWN, COM_DIR_LEFT_DOWN };
		break;

	default:
		break;
	}

	Init("image/" + characterName + "/�ҋ@/stand_0.png", VECTOR2(256, 256), VECTOR2(1, 1), VECTOR2(pos.x, pos.y), false, id);

	dir = charaDir;
}

StickHuman::~StickHuman()
{
}
