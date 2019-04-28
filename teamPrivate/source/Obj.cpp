#include "DxLib.h"
#include "SceneMng.h"
#include "ImageMng.h"
#include "Obj.h"
#include "BaseFloor.h"
#include "Gamecontroller.h"
#include "Collision.h"
#include "Color.h"
#include "SoundMng.h"

#define ADD_SPECIAL_POINT	(1)			// ��Ұ�ނ��󂯂��ۂɉ��Z����K�E�ް�ޗ�

#define ADD_ANGRY_POINT		(1.0f)		// �{��ް�ޗ��ߎ��̃|�C���g���Z��
#define REMOVE_ANGRY_POINT	(-2)		// �U�����������猸�炷�{��ް�ޗ�

#define CHARGE_CNT_LW		(45)		// ������ފ����܂ł��ڰѐ�
#define CHARGE_CNT_HV		(90)		// ������ފ����܂ł��ڰѐ�

#define HIT_CNT_MAX			(5)

#define SPECIAL_GAUGE_SIZE_X	(160)
#define SPECIAL_GAUGE_SIZE_Y	(27)

#define INVISIBLE_TIME		(120);		// ���G����

#define HEAVY_DAMEGE	(10)			// ���l�ȏゾ�Ƒ���Ұ�ޔ���
#define MEDIUM_DAMEGE	(5)				// ���l�ȏゾ�ƒ���Ұ�ޔ���

#define FLY_TEXT_VANISH_CNT	(100)		// FLY_TEXT��������܂ł��ڰѐ�

#define HP_RED_COLOR_NUM	(30)		// HP�����̒l�ȉ��ɂȂ�Ɛԕ����ɂȂ�i�ʏ�͗΁j
#define HP_YELLOW_COLOR_NUM	(100)		// HP�����̒l�ȉ��ɂȂ�Ɖ������ɂȂ�i�ʏ�͗΁j

#define AVOID_COST			(50)		// ����ɂ���ď����ް�ނ̑ϋv�l

// ���ߊ�����Ұ��݊֌n
#define CHARGE_FINISH_IMAGE_X		(150)
#define CHARGE_FINISH_IMAGE_Y		(150)
#define CHARGE_FINISH_ANIM_CNT_X	(5)
#define CHARGE_FINISH_ANIM_CNT_Y	(2)
#define CHARGE_FINISH_ANIM_SPEED	(3)

// �e�̕`��
#define SHADOW_DEF_SIZX (20)
#define SHADOW_DEF_SIZY (5)

// �c���̕\����
#define SHADOW_DRAW_FRAM		(10)

// �ެ���
#define JUMP_CNT_MAX   (2)		// �ő�ެ��߉\��
#define JUMP_EFFECT_INV	(5)		// �ެ��ߴ̪�ĊԊu

// �ް��
#define GUARD_TIME_MAX	(180)	// �ő�ް�މ\����

// ������΂���
#define FLY_HIGH_POWER VECTOR2(3,7)

RESULT_INFO Obj::resultInfo;

Obj::Obj()
{
}

bool Obj::init(std::string charName)
{
	Obj::charName = charName;

	initAnim();

	guardTime = GetGuardTimeMax();

	dir = DIR::RIGHT;
	vX = 0.0;
	vY = 0.0;
	speed = PLAYER_DEF_SPEED;
	jumpCnt = GetJumpCntMax();

	hp = HP;
	oldHp = hp;
	angryPoint = 0.0f;
	specialPoint = 0;

	invInf = {};
	floorCheckFlag = false;
	parentFloor.itrCnt = 0;
	parentFloor.oldPos = { 0,0 };

	dashFlag = false;
	downFlag = false;
	framCnt = 0;
	nowTimeLR = 0;
	nowTimeDown = 0;
	animId = 0;
	hitCnt = 0;
	jumpEffectCnt = 0; 
	jumpEffectFlag = false;
	jumpPos = {};

	deri = DERI_NON;

	fallFlag = false;

	hitPlayerInf = {};
	multiHitFlag = 0U;
	damageInv = 0U;
	nextMultHitInv = 0U;
	attackDir = DIR::LEFT;

	multiAttackNum = 0;

	angryFlag = false;
	chargeStage = CHARGE_NON;
	chargeCnt = 0;

	spAttackFlag = false;
	spAttackCnt = 0;
	spAttackHitFlag = false;
	spAttackInv = 0U;

	gravity = 0.4f;

	fixCtrlCnt = 0;

	deathFlag = false;

	shadowPosY = GROUND_POS_Y;

	// charName�������ݸނ���
	charNumbering["�J��~��"] = 0;
	charNumbering["�o�t�u"] = 1;
	charNumbering["�"] = 2;

	std::string color[4] = { "red","blue","green","yellow" };
	std::string imagePass = "image/����ڂ��/shabon_" + color[padNum] + ".png";

	guardImage = lpImageMng.GetID(imagePass)[0];

	hitEffectFileName = {
		"��_���[�W","�h�K�b.png",
		"���_���[�W","�o�L�b.png",
		"���_���[�W(�ۂ���)","�ۂ���.png",
	};

	plStateFileName = { "sweat.png", "angry.png", "special.png" };

	plStateCnt = 30;

	playerNumSprite = true;
	shadowList.clear();

	// �K�E�Z�ް�ނ̐F(���΁������򁨐�)
	spGaugeColorTbl = { 0x0000ff, 0x00ff7f ,0xffff00 ,0xff8c00 ,0xff0000 };

	// ̫�Ă̒ǉ�
	lpFontMng.FontInit("font/TanukiMagic.ttf", "���ʂ������}�W�b�N", "���ʂ�20��", 20, -1, true, false);
	lpFontMng.FontInit("font/TanukiMagic.ttf", "���ʂ������}�W�b�N", "���ʂ�40��", 40, -1, true, false);
	lpFontMng.FontInit("font/TanukiMagic.ttf", "���ʂ������}�W�b�N", "���ʂ�80���Α�", 80, 9, true, true);
	lpFontMng.FontInit("font/Molot.otf", "Molot", "Molot80���Α�", 80, 9, true, true);
	lpFontMng.FontInit("font/Square.ttf", "SquareFont", "�X�N�G�A28���Α�", 28, -1, true, true);
	lpFontMng.FontInit("font/Square.ttf", "SquareFont", "�X�N�G�A18���Α�", 18, -1, true, true);
	lpFontMng.FontInit("font/Square.ttf", "SquareFont", "�X�N�G�A20���Α�", 20, -1, true, true);
	// Se�̓o�^
	initSe();

	// ػ��ď��̏�����
	resultInfo.totalGiveDamage[padNum]		= 0;
	resultInfo.deathEnemyCnt[padNum]		= 0;
	resultInfo.chickenKillCnt[padNum]		= 0;
	resultInfo.chickenCnt[padNum]			= 0;
	resultInfo.deathOrder[padNum]			= PLAYER_MAX;	// PLAYER_MAX�Ȃ琶���Ă���
	resultInfo.charName[padNum]				= charName;

	// �摜�̋�ǂݍ���
	lpImageMng.GetID("image/UI/fire.png")[0];
	lpImageMng.GetID("image/UI/�r��8.png")[0];
	lpImageMng.GetID("image/UI/�r��1.png")[0];
	lpImageMng.GetID("image/UI/dash.png")[0];
	lpImageMng.GetID("image/UI/jump.png",VECTOR2(61,39),VECTOR2(7,1));
	lpImageMng.GetID("image/UI/angry2.png")[0];
	lpImageMng.GetID("image/UI/specialGaugeFrame.png")[0];
	lpImageMng.GetID("image/UI/specialGauge.png")[0];

	lpImageMng.GetID("image/UI/sweat.png")[0];
	lpImageMng.GetID("image/UI/angry.png")[0];
	lpImageMng.GetID("image/UI/special.png")[0];

	lpImageMng.GetID("image/UI/��_���[�W/�h�K�b.png")[0];
	lpImageMng.GetID("image/UI/���_���[�W/�o�L�b.png")[0];
	lpImageMng.GetID("image/UI/���_���[�W(�ۂ���)/�ۂ���.png")[0];

	return true;
}

bool Obj::initAnim(void)
{
	return false;
}

bool Obj::initSe(void)
{
	// Se�̓o�^
	lpSoundMng.SoundInit("sound/SE��/Attack.mp3", "�ʏ�U��");
	lpSoundMng.SoundInit("sound/SE��/Charge.mp3", "���ߎn�܂�");
	lpSoundMng.SoundInit("sound/SE��/ChargeMax.mp3", "���ߊ���");
	lpSoundMng.SoundInit("sound/SE��/Smash.mp3", "���ߍU��");

	lpSoundMng.SoundInit("sound/SE��/DamageS.mp3", "�_���[�W��");
	lpSoundMng.SoundInit("sound/SE��/DamageM.mp3", "�_���[�W��");
	lpSoundMng.SoundInit("sound/SE��/DamageL.mp3", "�_���[�W��");

	lpSoundMng.SoundInit("sound/SE��/Guard.mp3", "�K�[�h");
	lpSoundMng.SoundInit("sound/SE��/�R��.mp3", "�K�[�h����");

	lpSoundMng.SoundInit("sound/SE��/FootSteps.mp3", "����");
	lpSoundMng.SoundInit("sound/SE��/Jump.mp3", "�W�����v�n�܂�");

	lpSoundMng.SoundInit("sound/SE��/AngryCharge.mp3", "�{��Q�[�W����", 85);
	lpSoundMng.SoundInit("sound/SE��/AngerMAX.mp3", "�{��MAX");

	lpSoundMng.SoundInit("sound/SE��/Special.mp3", "�K�E�Z");
	lpSoundMng.SoundInit("sound/SE��/SpMAX.mp3", "SPMAX");

	lpSoundMng.SoundInit("sound/SE��/KnockOut.mp3", "�_�E��",80);

	lpSoundMng.SoundInit("sound/SE��/UmbFloat.mp3", "�P���V");

	walkSeCnt = 0;
	angChargeSeCnt = 0;
	umbFloatCnt = 0;
	spmaxSeFlag = false;
	chargeSeFlag = false;
	angerMaxSeFlag = false;

	return true;
}

void Obj::PlaySe(void)
{
	// ���ގ�
	auto PlaySE = [=](std::string animName, std::string soundName)
	{
		if (CheckAnimSe(animName))
		{
			lpSoundMng.SetSound(soundName);
		}
	};

	// ���̍Đ�
	PlaySE("�ʏ�U��1", "�ʏ�U��");
	PlaySE("�ʏ�U��2", "�ʏ�U��");
	PlaySE("�ʏ�U��3", "�ʏ�U��");

	PlaySE("�ʏ�U��_��", "�ʏ�U��");
	PlaySE("�ʏ�U��_���n�܂�", "�ʏ�U��");

	PlaySE("���ߎn�܂�", "���ߎn�܂�");
	PlaySE("���ߍU��", "���ߍU��");

	// ���ߍU����SE
	if (chargeCnt == CHARGE_CNT_LW)
	{
		lpSoundMng.SetSound("���ߊ���");
	}
	if (chargeCnt == CHARGE_CNT_HV && !chargeSeFlag)
	{
		lpSoundMng.SetSound("���ߊ���");
		chargeSeFlag = true;
	}
	if (chargeCnt == 0)
	{
		chargeSeFlag = false;
	}

	PlaySE("�W�����v�n�܂�", "�W�����v�n�܂�");

	PlaySE("�K�[�h", "�K�[�h");

	PlaySE("�K�E�Z", "�K�E�Z");

	if (CheckDamageSe() == DMG_LW)
	{
		lpSoundMng.SetSound("�_���[�W��");
	}

	if (CheckDamageSe() == DMG_MD)
	{
		lpSoundMng.SetSound("�_���[�W��");
	}

	if (CheckDamageSe() == DMG_HV)
	{
		lpSoundMng.SetSound("�_���[�W��");
	}

	if (animName == "����" || animName == "����")
	{
		if (animName == "����")
		{
			if (walkSeCnt % 20 == 0)
			{
				lpSoundMng.SetSound("����");
			}
		}
		if (animName == "����")
		{
			if (walkSeCnt % 13 == 0)
			{
				lpSoundMng.SetSound("����");
			}

		}
		walkSeCnt++;
	}
	else
	{
		walkSeCnt = 0;
	}

	if (animName == "�{��Q�[�W����")
	{
		if (angChargeSeCnt % 10 == 0)
		{
			lpSoundMng.SetSound("�{��Q�[�W����");
		}
		angChargeSeCnt++;
	}
	else
	{
		angChargeSeCnt = 0;
	}

	if (specialPoint == 100)
	{
		if (!spmaxSeFlag)
		{
			lpSoundMng.SetSound("SPMAX");
			spmaxSeFlag = true;
		}
	}
	else
	{
		spmaxSeFlag = false;
	}

	if (angryFlag)
	{
		if (!angerMaxSeFlag)
		{
			lpSoundMng.SetSound("�{��MAX");
			angerMaxSeFlag = true;
		}
	}
	else
	{
		angerMaxSeFlag = false;
	}

	if (animName == "�P���V")
	{
		if (umbFloatCnt % 60 == 30)
		{
			lpSoundMng.SetSound("�P���V");
		}
		umbFloatCnt++;
	}
	else
	{
		umbFloatCnt = 0;
	}
}

void Obj::PlaySPSe(void)
{
}

Obj::~Obj()
{
}

void Obj::UpDate(const GameController &controller, weakListObj objList, weakListChicken chickenList, bool entryFlag)
{
	GetJoypadXInputState(padNum + 1, &xstate[padNum]);

	if (AlwaysUpDate(controller, chickenList))
	{
		return;
	}

	VECTOR2 tmpPos = pos;

	// �����̺ؼޮ݂̎擾
	auto collisionVec = lpColMng.GetCollider(GetCharName(), GetAnim(), animId);

	int maxEndPosY = GetColMaxEndPosY(collisionVec);				// ���ڰт̺ײ�ް�̍ő�Y���W

	PosUpDate(tmpPos,maxEndPosY);

	if (param == PARAM::FALLING)
	{
		floorCheckFlag = true;
	}

	if (!fixCtrlCnt)	// �d�����͍s���ł��Ȃ�
	{
		// 	�U��
		if (animPtnTbl[GetAnim()] == ANIM_TYPE_MULTI_HIT)
		{
			DeriAttack(controller);
		}
		else
		{
			Attack(controller, tmpPos, entryFlag);
			if (spAttackFlag)
			{
				spAttackFlag = false;
				spAttackCnt += GetSpAttackTimeMax();
			}
			// �h���׽���L�̍U�����@���s��
			UniqueAttack(tmpPos);
		}
	}

	if (!spAttackHitFlag && GetAnim() != "�K�E�Z")
	{
		// (   �U���� && ���n���Ă���   )�@�@���̏ꍇ�ȊO�͍��W�ړ��ł���
		if (!((animPtnTbl[GetAnim()] != ANIM_TYPE_NO_ATTACK) && (locate != LOCATE::NON)))
		{
			if (!fixCtrlCnt || locate == LOCATE::NON)
			{
				// �ޯ��
				Dash(controller, tmpPos);
			}
		}
	}

	if (!fixCtrlCnt)	// �d�����͍s���ł��Ȃ�
	{
		// �ެ���
		JumpFnc(controller, tmpPos, maxEndPosY);

		// �����L�[�̏���
		if (controller.GetInputDown(PAD_INPUT_DOWN, padNum))
		{
			if (GetAnim() != "�K�[�h")
			{
				if ((framCnt - nowTimeDown) < PRASS_INV && locate != LOCATE::NON && animPtnTbl[GetAnim()] == ANIM_TYPE_NO_ATTACK)
				{
					downFlag = true;
				}
				nowTimeDown = framCnt;
			}
		}
		if (downFlag)
		{
			// ��ނɂ��鎞�Ɂ����L�[�ŉ��E��
			floorCheckFlag = false;
		}
		if (controller.GetInputHold(PAD_INPUT_DOWN, padNum))
		{
			// �󒆁��L�[�ŋ}���~
			if ((locate == LOCATE::NON) && (param == PARAM::FALLING) && GetAnim() != "�K�E�Z")
			{
				fallFlag = true;
			}
		}
	}

	if (ColGround(tmpPos, collisionVec, maxEndPosY))	// �n�ʂƏ��̓����蔻��
	{
		UniqueColGround();	// �p����̏���
	}

	MoveEndUpDate(tmpPos, controller, maxEndPosY);		// �ړ�������̍X�V
}

void Obj::CheckHit(weakListObj objList, std::list<ExpOrb*>& expOrbList, bool entryFlag)
{
	// 1�ڰёO��HP�̎c��
	oldHp = hp;

	// �S�������ذ���Ă��Ȃ�(�n�܂��Ă��Ȃ�)���͓����蔻������Ȃ�
	if (!entryFlag)
	{
		return;
	}

	// damageInv��60�Ȃ�
	if (damageInv == 60U)
	{
		hitPlayerInf = {};		// �󂯂��U������ؾ��
	}
	if (damageInv == 1U)
	{
		hitCnt = 0;
	}
	// damageInv��0���傫���Ȃ�1���Z����
	damageInv > 0U ? damageInv-- : damageInv = 0U;
	// nextMultHitInv��1�Ȃ疳�G��Ԃ�
	if (nextMultHitInv == 1U && !spAttackHitFlag)
	{
		pos.x += 30 * (static_cast<int>(attackDir) * -2 + 1);		// ɯ��ޯ�
		hitCnt = 0;
		visible = INVISIBLE_TIME;					// 1s���G���
		spAttackInv = 0;
	}
	// nextMultHitInv��0���傫���Ȃ�1���Z����
	nextMultHitInv > 0U ? nextMultHitInv-- : nextMultHitInv = 0U;

	if (spAttackInv == 1U)
	{
		spAttackHitFlag = false;
		pos.x += 30 * (static_cast<int>(attackDir) * -2 + 1);		// ɯ��ޯ�
		hitCnt = 0;
		visible = INVISIBLE_TIME;					// 1s���G���
	}
	spAttackInv > 0U ? spAttackInv-- : spAttackInv = 0U;

	// �U�����󂯂鑤�̓����蔻��

	if (spAttackCnt)
	{
		return;
	}

	if (visible)
	{
		return;					// ���G���Ԓ��Ȃ瓖���蔻����s��Ȃ�
	}

	for (auto &obj : *objList.lock())
	{
		if (obj->padNum == this->padNum)
		{
			continue;			// �������g�Ƃ͓����蔻����s��Ȃ�
		}

		if (obj->GetAnim() == "���S")
		{
			continue;
		}

		// ���肵�悤�Ƃ��Ă���U����������ڲ԰����󂯂��Ō�̍U���Ɠ����Ȃ�
		if (hitPlayerInf[(*obj).GetPadNum()].attackName == (*obj).GetAnim())
		{
			// �ς��ƂɍU�����󂯂�
			if (!multiHitFlag)
			{
				continue;
			}
			if (hitPlayerInf[(*obj).GetPadNum()].animId == (*obj).GetAnimId())
			{
				continue;			// ���肵�悤�Ƃ��Ă���U����������ڲ԰����󂯂��Ō�̍U���Ɠ����ςȂ画����s��Ȃ�
			}
		}

		auto myCollision = GetPlayerCollision();
		for (auto myCol : myCollision)
		{
			if (myCol.weponFlag)
			{
				continue;		// �������g�̕���Ƃ͓����蔻����s��Ȃ�
			}

			auto otherCollision = (*obj).GetPlayerCollision();
			for (auto otherCol : otherCollision)
			{
				if (!otherCol.weponFlag)
				{
					continue;	// ����̕���ײ�ް�ȊO�Ƃ͓����蔻����s��Ȃ�
				}

				//////////////////////////////////////////////////////////////////////////
				// ���܂Ŕ��������_�Ł@													//
				// myCol	= �����̕���ł͂Ȃ��ײ�ް��Vector�Ŋi�[����Ă���			//
				// otherCol = �G�̕���ײ�ް��Vector�Ŋi�[����Ă���					//
				//////////////////////////////////////////////////////////////////////////

				// dir == LEFT �Ȃ瓖���蔻��̌������t�ɂ���
				myCol.startPos.x *= static_cast<int>(dir) * -2 + 1;
				myCol.endPos.x *= static_cast<int>(dir) * -2 + 1;
				VECTOR2 myStartPos = pos + myCol.startPos;
				VECTOR2 myEndPos = pos + myCol.endPos;

				auto otherPos = (*obj).GetPos();
				auto otherPosOld = (*obj).GetPosOld();
				VECTOR2 defPos = otherPos - otherPosOld;	// pos�̍�
				otherCol.startPos.x *= static_cast<int>((*obj).GetDir()) * -2 + 1;
				otherCol.endPos.x *= static_cast<int>((*obj).GetDir()) * -2 + 1;
				VECTOR2 otherStartPos = (*obj).GetPos() + otherCol.startPos;
				VECTOR2 otherEndPos = (*obj).GetPos() + otherCol.endPos;

				// ����n�܂�ɂȂ�悤�ɂ���
				if (otherStartPos.x > otherEndPos.x
					|| otherStartPos.y > otherEndPos.y)
				{
					VECTOR2 tmp = otherStartPos;
					otherStartPos = otherEndPos;
					otherEndPos = tmp;
				}

				defPos *= static_cast<int>((*obj).GetDir()) * -2 + 1;
				otherStartPos -= defPos;

				// �����蔻����s��
				if (Square(myStartPos, myEndPos, otherStartPos, otherEndPos))
				{
					// �U�����󂯂��� --------------------------
					hitPlayerInf[(*obj).GetPadNum()] = {	// �󂯂��U���̏����i�[
						(*obj).GetAnim(),
						(*obj).GetAnimId()
					};
					attackDir = (*obj).GetDir();

					// �U�����󂯂��� --------------------------
					if (GetAnim() == "�K�[�h")
					{
						guardTime -= otherCol.attackPoint / 2;	// ����̍U�����A�ް�ގ��Ԃ�����
						if (guardTime < 0)
						{
							guardTime = 0;
						}
						lpSoundMng.SetSound("�K�[�h����");
						pos.x += 10 * (static_cast<int>(attackDir) * -2 + 1);		// ɯ��ޯ�
						return;
					}

					damageInv = 90U;						// �󂯂��U���̏���1.5s�ێ�

					// �̗͍��
					int damage = otherCol.attackPoint;		// ����Ұ�ޗʂ�ϐ��Ɋi�[
					if (obj->CheckAngryMode())
					{
						damage = damage * 2;	// �Ԃ�����Ӱ�ނȂ����Ұ�ޗ�2�{
					}
					// �����LW�Ȃ����Ұ�ޗ�2�{�AHV�Ȃ�3�{
					damage = damage + (obj->CheckCharge() * damage);
					if (obj->CheckSpAttackCnt())
					{
						damage = damage * 2;
					}

					if (damage > hp)
					{
						damage = hp;
					}

					hp -= damage;

					if (hp <= 0)
					{
						// SE�̍Đ�
						lpSoundMng.SetSound("�_�E��");

						SetAnim("���S");
						int deathPlCnt = 0;		// ����ł�����ڲԂ̐l��
						for (auto data : resultInfo.deathOrder)
						{
							deathPlCnt += (data != PLAYER_MAX) ? 1 : 0;
						}
						resultInfo.deathOrder[padNum] = deathPlCnt + 1;
					}
					else
					{
						SetAnim("�_���[�W");					// ����Ұ�ޱ�Ұ���
					}

					animCnt = 0;
					hitCnt++;								// �U�����󂯂��񐔂̉��Z
					vY = 0.0f;

					// FLY_TEXT�̏���List�ɒǉ�
					if (damage > 0)		// 0��҂͕\�����Ȃ�
					{
						FLY_TEXT data;
						data.cnt = FLY_TEXT_VANISH_CNT;
						data.damage = damage;
						// �~�`�ɐ�������
						// -180������180��(-3.14 �` 3.14)�̊Ԃ�����тȐ������擾���� 
						int length314 = (rand() % (314 * 2 + 1)) - 314;				// -314�`314���擾
						double circleRad = static_cast<double>(length314) / 100.0;	// -3.14�`3.14���擾
																					// �������W�̌���
						data.insPos.x = static_cast<int>(pos.x + 35 * cos(circleRad));
						data.insPos.y = pos.y - 50;
						flyTextList.push_back(data);
					}

					bool flyHigh = false;					// ������Ԃ̂�		true:�������
					auto oppAnim = (*obj).GetAnim();		// ����̱�Ұ���
					if ((hitCnt >= HIT_CNT_MAX && oppAnim != "�K�E�Z") || oppAnim == "���ߍU��")
					{
						pos.x += 30 * (static_cast<int>(attackDir) * -2 + 1);		// ɯ��ޯ�
						hitCnt = 0; 
						flyHigh = true;
						visible = INVISIBLE_TIME;
					}
					if (oppAnim == "�K�E�Z")
					{
						spAttackInv = 90U;			// 1.5s�K�E�Z��H���Ȃ���Έړ��\�ɂ���
						spAttackHitFlag = true;		// �K�E�Z���󂯂Ă���
						parentFloor.itrCnt = 99999;	// ���W�Œ肷�邽�߂ɏ��ɏ���Ă��Ȃ����Ƃɂ���
													// �����K�E�Z�̍Ō�̍U���Ȃ�	���́|2�Ŗ�����蒲�����Ă��邪�K�E�Z���̌�d���ű�Ұ��݂𕪂���Ƃ����Ǝv����
						if ((*obj).GetAnimFrame() - 2 <= (*obj).GetAnimId())
						{
							flyHigh = true;
							spAttackInv = 1U;
						}
					}

					// �������̍U������Z�n(�A���̍Ō��K�E�Z)�Ȃ�
					if (oppAnim == "�ʏ�U��3" || oppAnim == "�K�E�Z")
					{
						multiHitFlag = true;	// ���ڰэU����������悤�ɂȂ�
						if (oppAnim == "�ʏ�U��3")
						{
							flyHigh = true;
							nextMultHitInv = 1U;	// ���G��Ԃ�
						}
					}
					else
					{
						multiHitFlag = false;
					}

					if (flyHigh)
					{
						locate = LOCATE::NON;
						vX =  static_cast<float>((*obj).GetFlyHighPower().x);
						vY = -static_cast<float>((*obj).GetFlyHighPower().y);
					}

					// �K�E�ʂ̐���
					ExpOrb* SpecialOrb = new ExpOrb(pos, padNum, ADD_SPECIAL_POINT);
					expOrbList.push_back(SpecialOrb);

					// �U�������� -------------------------
					obj->AddAngryPoint(REMOVE_ANGRY_POINT);								// �{��ް�ތ��Z
					resultInfo.totalGiveDamage[obj->GetPadNum()] += damage;				// �^��������Ұ�މ��Z
					resultInfo.deathEnemyCnt[obj->GetPadNum()] += (hp <= 0) ? 1 : 0;	// ׽���������|�����l����+1
																						// �K�E�ް�މ��Z
					if (!(oppAnim == "�K�E�Z"))
					{
						ExpOrb* SpecialOrb = new ExpOrb(obj->GetPos(), obj->GetPadNum(), otherCol.specialPoint);
						expOrbList.push_back(SpecialOrb);
					}

					auto screenSize = lpSceneMng.GetScreenSize();
					if (pos.x <= 0)
					{
						pos.x = 0;
					}
					if (pos.x > screenSize.x)
					{
						pos.x = screenSize.x;
					}

					if (invInf.invCnt)
					{
						invInf.invCnt--;
						return;							// ���G���Ă��������Ұ�ނ��󂯂Ȃ�
					}

					return;
				}
			}
		}
	}
}

bool Obj::CheckAngryMode(void)
{
	return angryFlag;
}

CHARGE_STAGE Obj::CheckCharge(void)
{
	return chargeStage;
}

CUT_IN_INFO Obj::CheckSpecialCutIn(void)
{
	CUT_IN_INFO rtnInfo = {};
	// �K�E�Z���o�����ڰтȂ�true��Ԃ�
	if (animNameOld != "�K�E�Z" && animName == "�K�E�Z")
	{
		rtnInfo.padNum = padNum;
		rtnInfo.pos = pos;
		rtnInfo.cnt = 0;
		rtnInfo.flag = true;
		return rtnInfo;
	}
	return rtnInfo;
}

bool Obj::CheckObjType(OBJ_TYPE type)
{
	return (type == OBJ_TYPE_MAX);
}

void Obj::Draw(bool moveStopFlag, CUT_IN_INFO cutInInfo)
{

	// UI���̘g�`��
	SetDrawBlendMode(DX_BLENDMODE_ADD, 128);
	DrawGraph(padNum * 320, 560, lpImageMng.GetID("image/�Q�[���V�[���p/�g2.png")[0], true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 128);

	// �{��ް�ޗ��ߴ̪�Ă̕`��
	if (GetAnim() == "�{��Q�[�W����")
	{
		// ��ׂ̌��
		DrawRectGraph(
			pos.x - 100 / 2, pos.y - 130 / 2,
			((framCnt / 5) % 5) * 100, 0,
			100, 100,
			lpImageMng.GetID("image/UI/fire.png")[0], true, false);

		// ��ח����G�̌��
		DrawRectGraph(
			320 * padNum - 40, 570 - 50,
			((framCnt / 5) % 5) * 200, 0,
			200, 200,
			lpImageMng.GetID("image/UI/fire_big.png")[0], true, false);
	}

	// ���U������ޗ��ߴ̪�Ă̕`��
	if (GetAnim() == "���ߎn�܂�" || GetAnim() == "���ߒ�")
	{
		if (chargeStage == CHARGE_NON)
		{
			int animCnt = chargeCnt / 12;	// ++������[ 
			if (animCnt < 5 * 2)
			{
				DrawRectGraph(
					pos.x - 150 / 2, pos.y - 150 / 2,
					150 * (animCnt % 5), 150 * (animCnt / 5 % 2),
					150, 150,
					lpImageMng.GetID("image/UI/����2.png")[0], true, false);
			}
		}
		else
		{
			// ����2�i�K�ڂ͐F��ς���
			int r, g, b = 0;
			if (chargeStage == CHARGE_HV)
			{
				GetDrawBright(&r, &g, &b);
				SetDrawBright(255, 125, 125);
			}

			// ���U���A����ފ����̪�Ă̕`��
			int animCnt = chargeCnt / CHARGE_FINISH_ANIM_SPEED;	// ++������[ 
			DrawRectGraph(
				pos.x - CHARGE_FINISH_IMAGE_X / 2, pos.y - CHARGE_FINISH_IMAGE_Y / 2,
				CHARGE_FINISH_IMAGE_X * (animCnt % CHARGE_FINISH_ANIM_CNT_X), CHARGE_FINISH_IMAGE_Y * (animCnt / CHARGE_FINISH_ANIM_CNT_X % CHARGE_FINISH_ANIM_CNT_Y),
				CHARGE_FINISH_IMAGE_X, CHARGE_FINISH_IMAGE_Y,
				lpImageMng.GetID("image/UI/�r��1.png")[0], true, false);

			if (chargeStage == CHARGE_HV)
			{
				SetDrawBright(r, g, b);
			}
		}
	}

	bool checkFlag = false;

	for (auto itr : (*floorList))
	{
		if ((*itr).GetPos().y >= pos.y)
		{
			if (((*itr).GetPos().x <= pos.x + 32) && (pos.x - 32 <= ((*itr).GetPos().x + (*itr).GetSize().x)))
			{
				shadowPosY = (*itr).GetPos().y;
				checkFlag = true;
			}
		}
	}

	if (!checkFlag)
	{
		shadowPosY = GROUND_POS_Y;
	}

	int decreseSiz = (shadowPosY - pos.y) / 50;

	VECTOR2 shadowSiz = VECTOR2(SHADOW_DEF_SIZX, SHADOW_DEF_SIZY) - decreseSiz;

	if (shadowSiz.y <= 1)
	{
		shadowSiz.y = 1;
	}

	// ��׉e�̕`��
	DrawOvalAA(static_cast<float>(pos.x), static_cast<float>(shadowPosY), static_cast<float>(shadowSiz.x), static_cast<float>(shadowSiz.y), 16, 0x000000, true);

	// �ޯ���̪�Ă̕`��
	if (GetAnim() == "����")
	{
		DrawRectGraph(
			pos.x - 45 + (static_cast<int>(dir) * 45), pos.y,
			((framCnt / 4) % 3) * 48, 0,
			42, 35, lpImageMng.GetID("image/UI/dash.png")[0], true, static_cast<bool>(dir));
	}

	// �ެ��ߴ̪�Ă̕`��
	if (jumpEffectFlag)
	{
		DrawRotaGraph(jumpPos.x, jumpPos.y, 1.0, 0.0,
			lpImageMng.GetID("image/UI/jump.png")[jumpEffectCnt / JUMP_EFFECT_INV], true);
	}

	std::vector<ColInf> collisonVec = lpColMng.GetCollider(charName, animName, animId);

	CharacterDraw(collisonVec, moveStopFlag, cutInInfo);	// ��ׂ̕`��

	FlyTextDraw();			// �ײ÷�Ă̕`��

	AngryGaugeDraw();		// �{��ް��UI�̕`��

	SpecialGaugeDraw(cutInInfo.flag);		// �K�E�ް��UI�̕`��

	GuardDraw();			// �ް��UI�̕`��

	StateIconDraw();		// �ð������(̷�޼)�̕`��

	DrawGraph(padNum * 320, 560, lpImageMng.GetID("image/�Q�[���V�[���p/�g1.png")[0], true);
							// �Ԃ�����Ӱ�ނ��ǂ����̃f�o�b�N�\��
	if (angryFlag)
	{
		DrawRotaGraph(
			320 * padNum + ((framCnt / 30) % 2) * 40 + 40,
			600 + ((framCnt / 30) % 2) * 60,
			0.8,
			0.0,
			lpImageMng.GetID("image/UI/angry2.png")[0],
			true);
	}
	// HP�̕`��
	VECTOR2 offset = {};
	if (GetAnim() == "�_���[�W" && !moveStopFlag)
	{
		offset = { GetRand(20) - 10, GetRand(20) - 10 };
	}

	int hpColor = 0x00ff7f;			// ��
	if (hp <= HP_YELLOW_COLOR_NUM)
	{
		hpColor = 0xffff00;			// ���F
	}
	if (hp < HP_RED_COLOR_NUM)
	{
		hpColor = 0xff4500;			// ��
	}
	int drawLengh;
	drawLengh = GetDrawFormatStringWidthToHandle(lpFontMng.SetFont("Molot80���Α�"), "%d", hp);
	DrawFormatStringToHandle(/*150*/300 + 320 * padNum + offset.x - drawLengh, 575 + offset.y, hpColor, lpFontMng.SetFont("Molot80���Α�"), "%d", hp);
	DrawStringToHandle(120 + 320 * padNum /*+ offset.x*/, 625 /*+ offset.y*/, "HP", 0xffffff, lpFontMng.SetFont("�X�N�G�A20���Α�"));

	// ˯Ĵ̪��
	if (hitEffect.cnt > 0)
	{
		DrawRotaGraph(
			hitEffect.insPos, 1.0, hitEffect.rota + 1.57,
			lpImageMng.GetID("image/�q�b�g�p�G�t�F�N�g/" + hitEffectFileName[hitEffect.damageRate][0] + "/" + hitEffectFileName[hitEffect.damageRate][1])[0],
			true, false);
	}

	// 1P2P�̕`��
	if (playerNumSprite && hp > 0)
	{
		DrawRotaGraph(
			pos.x, pos.y - 80,
			1.0, 0.0,
			lpImageMng.GetID("image/UI/1234.png")[padNum], true, false);
	}

	//// debug��ד���ɕ\�����镶��
	//DrawFormatString(pos.x - 20, pos.y - 102, 0xffffff,GetAnim().c_str(), true);
	//DrawFormatString(pos.x - 20, pos.y - 122, 0xff0000, "ckillCnt:%d", resultInfo.chickenKillCnt[padNum], true);
	//DrawFormatString(pos.x - 20, pos.y - 142, 0xff0000, "stage:%d", chargeStage, true);
	//DrawFormatString(pos.x - 20, pos.y - 162, 0xff0000, "animCnt:%d", chargeCnt, true);

	// �����̺ؼޮ݂̎擾
	for (auto &col : collisonVec)
	{
		// dir == LEFT �Ȃ瓖���蔻��̌������t�ɂ���
		col.startPos.x *= static_cast<int>(dir) * -2 + 1;
		col.endPos.x *= static_cast<int>(dir) * -2 + 1;
		VECTOR2 startPos = pos + col.startPos;
		VECTOR2 endPos = pos + col.endPos;

		//DrawBox(startPos, endPos, col.weponFlag ? 0x0000ff : 0xff0000, false);
	}

	// �{��ް�ނƕK�E�ް�ނ̐��l�\��
	for (int j = 0; j < PLAYER_MAX; j++)
	{
		//DrawFormatString(130 + 320 * padNum, 660, 0x000000, "�{��|�C���g %d", angryPoint);

		int numDigit = 0;	// �����̌����@��(3 = 1, 60 = 2, 100 = 3)
		int tmp = specialPoint;
		while (tmp >= 10)
		{
			tmp /= 10;		// ����1���炷
			numDigit++;
		}

		DrawFormatStringToHandle(255 - (numDigit * 10) + (320 * padNum), 675, 0xff0000, lpFontMng.SetFont("�X�N�G�A28���Α�"), "%d", specialPoint);
		DrawStringToHandle(290 + (320 * padNum), 685, "%", 0xffffff, lpFontMng.SetFont("�X�N�G�A18���Α�"));
		//DrawFormatStringToHandle(205 - (numDigit * 10) + (320 * padNum), 655, 0xff0000, lpFontMng.SetFont("���ʂ�40��"), "%d", specialPoint);
		//DrawStringToHandle(250 + (320 * padNum), 675, "��", 0xffffff, lpFontMng.SetFont("���ʂ�20��"));
	}
}

const bool Obj::CheckAnimSe(std::string anim)
{
	if (animName == anim)
	{
		if (animName != animNameOld)
		{
			return true;
		}
		return false;
	}
	return false;
}

const DMG_LATE Obj::CheckDamageSe(void)
{
	// ���݂�HP��1�ڰёO��HP�Ƃ̍�
	int difHp = oldHp - hp;

	if (hp != oldHp)
	{
		if (difHp >= HEAVY_DAMEGE)
		{
			// ����Ұ��
			return DMG_HV;
		}
		else if (difHp >= MEDIUM_DAMEGE)
		{
			// ����Ұ��
			return DMG_MD;
		}
		else
		{
			// ����Ұ��
			return DMG_LW;
		}
	}
	return DMG_MAX;
}

void Obj::ResultInfoInit()
{
	Obj::resultInfo = {};
}

void Obj::AddChickenCnt(int padNum)
{
	resultInfo.chickenCnt[padNum]++;
}

int Obj::GetChickenCnt(int padNum)
{
	return resultInfo.chickenCnt[padNum];
}


int Obj::GetGuardTimeMax(void)
{
	return GUARD_TIME_MAX;
}

int Obj::GetJumpCntMax(void)
{
	return JUMP_CNT_MAX;
}

bool Obj::AlwaysUpDate(const GameController &controller, weakListChicken chickenList)
{
	if (invInf.animName != GetAnim())
	{
		invInf = {};
	}
	animNameOld = animName;
	posOld = pos;
	dirOld = dir;
	animIdOld = animId;
	framCnt++;

	auto updateCnt = [](auto& cnt) {
		if (cnt)
		{
			cnt--;
		}
	};
	updateCnt(visible);
	updateCnt(fixCtrlCnt);
	updateCnt(spAttackCnt);

	// ��Ұ���id�̌v�Z
	UpDateAnimId();

	if (controller.GetInputDown(PAD_INPUT_7, padNum))
	{
		playerNumSprite = !playerNumSprite;
	}

	// �Ԃ�����Ӱ�ނ̃`�F�b�N
	if (angryPoint >= 100.0f)
	{
		angryFlag = true;
		angryCnt = ANGRY_CNT * 60;
		angryPoint = 0.0f;
	}
	if (angryFlag)
	{
		angryCnt >= 0 ? angryCnt-- : angryFlag = false;
	}

	// �ް�ގ��Ԃ̍X�V
	if (GetAnim() == "�K�[�h")
	{
		guardTime <= 0 ? guardTime = 0 : guardTime--;
		if (guardTime <= 0)
		{
			fixCtrlCnt = 60;
			SetAnim("�ҋ@");
		}
	}
	else
	{
		if (guardTime < GetGuardTimeMax())
		{
			guardTime++;
		}
	}

	if (GetAnim() == "���S")
	{
		if (animEndFlag)
		{
			AddChickenList()(chickenList, std::make_unique<Chicken>(floorList, pos, padNum, static_cast<CHICKEN_COLOR>(rand() % CHICKEN_COLOR_MAX)));
			deathFlag = true;
		}
		return true;
	}

	return false;
}

int Obj::GetColMaxEndPosY(std::vector<ColInf> collisionVec)
{
	int maxEndPosY = 0;
	for (auto &col : collisionVec)
	{
		if (col.weponFlag)
		{
			continue;				// ����ײ�ް��maxEndPosY�Ɋ܂܂Ȃ�
		}

		if (col.startPos.y > maxEndPosY)
		{
			maxEndPosY = col.startPos.y;
		}
		if (col.endPos.y > maxEndPosY)
		{
			maxEndPosY = col.endPos.y;
		}
	}
	return maxEndPosY;
}

void Obj::PosUpDate(VECTOR2 & tmpPos, int maxEndPosY)
{
	if (GetAnim() != "�K�E�Z" && !spAttackHitFlag)
	{
		// ���W�␳
		PosAdjust(tmpPos, maxEndPosY);

		// ���R����
		if (vY > 0.0 && locate == LOCATE::NON)
		{
			param = PARAM::FALLING;
		}

		vY += gravity;

		// ���x��pos�����Z
		tmpPos.y += static_cast<int>(vY) * (static_cast<int>(fallFlag && param == PARAM::FALLING) + 1);
		tmpPos.x += static_cast<int>(vX);
	}
}

void Obj::DeriAttack(const GameController &controller)
{
	if (controller.GetInputDown(GPAD_INPUT_X, padNum))
	{
		if (deri == DERI_NON)
		{
			deri = DERI_NOMAL;// �U�����ɂ���ɍU�����݂̓��͂��������ꍇ
		}
		if (GetAnim() == "�ʏ�U��2" && deri == DERI_NOMAL)
		{
			if (controller.GetInputHold(PAD_INPUT_DOWN, padNum))
			{
				deri = DERI_DOWN;
			}
			if (controller.GetInputHold(PAD_INPUT_UP, padNum))
			{
				deri = DERI_UP;
			}
		}
	}
	return;
}

void Obj::Attack(const GameController &controller, VECTOR2 &tmpPos, bool entryFlag)
{
	// ���U������ނ̱�Ұ��ݶ�����X�V
	chargeCnt = ((GetAnim() == "���ߎn�܂�" || GetAnim() == "���ߒ�") ? chargeCnt + 1 : 0);

	if (GetAnim() == "�K�E�Z")
	{
		return;
	}

	if (GetAnim() == "�_���[�W")
	{
		// ����ފ֘A�̏���ؾ��
		chargeStage = CHARGE_NON;
		chargeCnt = 0;

		return;
	}

	if (GetAnim() == "���")
	{
		tmpPos.x += (5 * -(static_cast<int>(dir) * 2 - 1));
		return;
	}

	// �ް��
	if (controller.GetInputHold(GPAD_INPUT_R1, padNum) && guardTime)
	{
		if (animPtnTbl[GetAnim()] == ANIM_TYPE_NO_ATTACK || GetAnim() == "�K�[�h")
		{
			bool flag = false;
			if (controller.GetInputDown(PAD_INPUT_LEFT, padNum))
			{
				dir = DIR::LEFT;
				flag = true;
			}
			if (controller.GetInputDown(PAD_INPUT_RIGHT, padNum))
			{
				dir = DIR::RIGHT;
				flag = true;
			}

			if (GetAnim() != "���")
			{
				if (flag && guardTime >= AVOID_COST)
				{
					SetAnim("���");
					visible = lpImageMng.GetSize(charName + "/" + "���") * animTable[animName][ANIM_TBL_INV];
					guardTime -= AVOID_COST;
				}
			}
		}
		if (locate != LOCATE::NON && GetAnim() != "���")
		{
			SetAnim("�K�[�h");
			return;
		}
	}
	// �ް�����݂𗣂����Ƃ�
	else
	{
		if (GetAnim() == "�K�[�h")
		{
			SetAnim("�ҋ@");
			return;
		}
	}

	// �{��Ӱ�ނłȂ��Ȃ�{��Q�[�W���߉\
	if (!angryFlag)
	{
		// �{��Q�[�W����
		if (controller.GetInputHold(GPAD_INPUT_L1, padNum))
		{
			if (animPtnTbl[GetAnim()] == ANIM_TYPE_NO_ATTACK && locate != LOCATE::NON)
			{
				SetAnim("�{��Q�[�W����");
				return;
			}
			if (GetAnim() == "�{��Q�[�W����" && entryFlag)
			{
				if (angryPoint < 100 && !angryFlag)
				{
					angryPoint += ADD_ANGRY_POINT;
				}
			}
		}
		// �{��Q�[�W�{�^���𗣂����Ƃ�
		else
		{
			if (GetAnim() == "�{��Q�[�W����")
			{
				SetAnim("�ҋ@");
				fixCtrlCnt = FIX_TIME;
				return;
			}
		}

		// �{��ް��MAX�őҋ@��Ұ��݂ֈڍs
		if (angryPoint >= 100)
		{
			SetAnim("�ҋ@");
		}
	}


	// ����
	if (controller.GetInputHold(GPAD_INPUT_Y, padNum))
	{
		if (animPtnTbl[GetAnim()] == ANIM_TYPE_NO_ATTACK)
		{
			if (GetAnim() != "���ߒ�")
			{
				SetAnim("���ߎn�܂�");
				return;
			}
		}
		if (GetAnim() == "���ߒ�" && chargeStage != CHARGE_HV)
		{
			if (chargeCnt >= CHARGE_CNT_LW)
			{
				chargeStage = CHARGE_LW;
			}
			if (chargeCnt >= CHARGE_CNT_HV)
			{
				chargeStage = CHARGE_HV;
			}
		}
	}
	// ���ߍU��
	else
	{
		if (GetAnim() == "���ߒ�")
		{
			if (charName == "�o�t�u")
			{
				invInf.invCnt = 1;
				invInf.animName = "���ߍU��";
			}
			SetAnim("���ߍU��");		// Y���݂𗣂������ɗ��߂��������Ă���΍U��
			return;
		}
	}

	if (animPtnTbl[GetAnim()] != ANIM_TYPE_NO_ATTACK)
	{
		return;
	}

	// �ʏ�U��
	if (controller.GetInputDown(GPAD_INPUT_X, padNum))
	{
		if (controller.GetInputHold(PAD_INPUT_UP, padNum))
		{
			if (charName == "�|���")
			{
				if (jumpCnt)
				{
					downFlag = false;
					param = PARAM::JUMPING;
					floorCheckFlag = false;
					jumpCnt--;
					locate = LOCATE::NON;
					SetAnim("�ʏ�U��_��");
				}
			}
			else
			{
				SetAnim("�ʏ�U��_��");
			}
			return;
		}
		else if (controller.GetInputHold(PAD_INPUT_DOWN, padNum))
		{
			SetAnim("�ʏ�U��_���n�܂�");
			return;
		}
		else
		{
			SetAnim("�ʏ�U��1");
			return;
		}
	}

	// �K�E�Z
	if ((xstate[padNum].LeftTrigger >= 255) && (xstate[padNum].RightTrigger >= 255))
		//if (controller.GetInputHold(GPAD_INPUT_B, padNum))
	{
		if (specialPoint >= 100)
		{
			spAttackFlag = true;
			SetAnim("�K�E�Z");
			spAttackCnt += lpImageMng.GetSize(charName + "/" + animName) * animTable[animName][ANIM_TBL_INV];
			specialPoint = 0;
			return;
		}
	}

	return;
}

void Obj::UniqueAttack(VECTOR2 & tmpPos)
{
}

void Obj::UpDateAnimId(void)
{
	if (imageName.length() == 0)
	{
		return;
	}

	if (jumpEffectFlag)
	{
		// �ެ��ߴ̪�Ĕ������̏���
		jumpEffectCnt++;
		if ((jumpEffectCnt / JUMP_EFFECT_INV) >= lpImageMng.GetID("image/UI/jump.png").size())
		{
			// ��Ұ��݂��I������̂��׸ނ�܂�
			jumpEffectFlag = false;
		}
	}

	if (animTable.find(animName) != animTable.end())
	{
		int count = (animCnt / animTable[animName][ANIM_TBL_INV]);
		if (animTable[animName][ANIM_TBL_LOOP] || count < animTable[animName][ANIM_TBL_FRAME])
		{
			count = (count % animTable[animName][ANIM_TBL_FRAME]);
		}
		else
		{
			count = animTable[animName][ANIM_TBL_FRAME] - 1;
			animEndFlag = true;
		}

		animId = (animTable[animName][ANIM_TBL_START_ID])
			+ count;
		animCnt += (angryFlag || (spAttackCnt && charName == "�o�t�u")) + 1;
	}
}

void Obj::Dash(const GameController &controller, VECTOR2 &tmpPos)
{
	if (GetAnim() == "�_���[�W" || GetAnim() == "�K�[�h")
	{
		return;
	}

	bool moveFlag = false;	// ���E�ړ����Ă����true
	if (controller.GetInputHold(PAD_INPUT_RIGHT, padNum))
	{
		tmpPos.x += speed * (1 + static_cast<int>(dashFlag));
		if (animPtnTbl[GetAnim()] == ANIM_TYPE_NO_ATTACK)
		{
			dir = DIR::RIGHT;
		}
		moveFlag = true;
	}
	else if (controller.GetInputHold(PAD_INPUT_LEFT, padNum))
	{
		tmpPos.x -= speed * (1 + static_cast<int>(dashFlag));
		if (animPtnTbl[GetAnim()] == ANIM_TYPE_NO_ATTACK)
		{
			dir = DIR::LEFT;
		}
		moveFlag = true;
	}
	else
	{
		if ((locate != LOCATE::NON && (GetAnim() != "���ߒ�")))
		{
			SetAnim("�ҋ@");
		}
		dashFlag = false;
	}

	if (GetAnim() == "���ߒ�" || GetAnim() == "�P���V")
	{
		return;
	}

	if (dir != dirOld)
	{
		if (locate != LOCATE::NON)
		{
			SetAnim("����");
			dashFlag = false;
		}
	}

	if (locate != LOCATE::NON)
	{
		if (moveFlag)
		{
			dashFlag ? SetAnim("����") : SetAnim("����");
		}
	}

	if ((controller.GetInputDown(PAD_INPUT_RIGHT, padNum) || controller.GetInputDown(PAD_INPUT_LEFT, padNum)))
	{
		if (locate != LOCATE::NON)
		{
			if ((framCnt - nowTimeLR < PRASS_INV) && (dir == dirOld))
			{
				SetAnim("����");
				dashFlag = true;
			}
			else
			{
				SetAnim("����");
				dashFlag = false;
			}
		}
		nowTimeLR = framCnt;
	}
}

void Obj::JumpFnc(const GameController &controller, VECTOR2 &tmpPos, int maxEndPosY)
{
	// �K�E�Z���󂯂Ă���Ƃ�	����	�^���Ă���Ƃ�	�͓����Ȃ�
	if (!spAttackHitFlag && GetAnim() != "�K�E�Z")
	{

		if (animPtnTbl[GetAnim()] == ANIM_TYPE_NO_ATTACK)
		{
			// �ެ���
			if (controller.GetInputDown(GPAD_INPUT_A, padNum) && (jumpCnt > 0))
			{
				jumpCnt--;
				jumpEffectFlag = true;
				jumpEffectCnt = 0;
				jumpPos = VECTOR2(tmpPos.x, tmpPos.y + maxEndPosY);
				downFlag = false;
				param = PARAM::JUMPING;
				floorCheckFlag = false;

				// ���L�[�������Ă��� && �n�ɑ������Ă���� �n�C�W�����v
				vY = (controller.GetInputHold(PAD_INPUT_DOWN, padNum) && locate != LOCATE::NON ? -15.0f : -10.0f);
				locate = LOCATE::NON;

				SetAnim("�W�����v�n�܂�");
			}
		}
	}
}

bool Obj::ColGround(VECTOR2& tmpPos, std::vector<ColInf> colVec,int maxEndPosY)
{
	if (param != PARAM::JUMPING)
	{
		bool hitFlag = false;		// �����n�ʂɂ���true

		auto GetColPos = [&](VECTOR2 pos)
		{
			pos.x *= static_cast<int>(dir) * -2 + 1;
			return VECTOR2(tmpPos + pos);
		};

		// �����Ă��鏰�Ƃ̓����蔻��
		if (floorCheckFlag && !spAttackHitFlag && GetAnim() != "�K�E�Z")
		{
			bool flag = false;
			int itrCnt = 0;
			for (auto itr : *floorList)
			{
				VECTOR2 floorStartPos = itr->GetPos();
				VECTOR2 floorEndPos = itr->GetPos() + itr->GetSize();
				for (auto &col : colVec)
				{
					if (col.weponFlag)
					{
						continue;
					}

					VECTOR2 startPos = GetColPos(col.startPos);
					VECTOR2 endPos = GetColPos(col.endPos);

					// 1�ޯĉ��Z���邱�ƂŒ��x���̏�ɂ���Ƃ���������悤�ɂ��Ă���
					endPos.y++;

					if (Square(VECTOR2(startPos.x, endPos.y - 20), endPos, floorStartPos, floorEndPos))
					{
						// �����Ă���ǂƏՓ˂����ꍇ
						flag = true;
						parentFloor.oldPos = itr->GetPos();
						parentFloor.itrCnt = itrCnt;
					}
				}
				itrCnt++;
			}
			if (flag)
			{
				hitFlag = true;
				tmpPos.y = parentFloor.oldPos.y - maxEndPosY;
				locate = LOCATE::FLOOR;
			}
			else
			{
				parentFloor.itrCnt = 0;
			}
		}


		// �n�ʂƂ̓����蔻��
		bool flag = false;
		VECTOR2 groundStartPos(0, GROUND_POS_Y);
		VECTOR2 groundEndPos = groundStartPos + VECTOR2(1280, 120);

		for (auto &col : colVec)
		{
			if (col.weponFlag)
			{
				continue;
			}

			VECTOR2 startPos = GetColPos(col.startPos);
			VECTOR2 endPos = GetColPos(col.endPos);
			// 2�ޯĉ��Z���邱�ƂŒ��x���̏�ɂ���Ƃ���������悤�ɂ��Ă���
			endPos.y += 2;

			if (Square(startPos, endPos, groundStartPos, groundEndPos))
			{
				if (col.weponFlag)
				{
					continue;
				}
				flag = true;
			}
		}
		if (flag)
		{
			hitFlag = true;
			locate = LOCATE::GROUND;
			tmpPos.y = GROUND_POS_Y - maxEndPosY;
		}

		if (hitFlag)
		{
			fallFlag = false;
			jumpCnt = GetJumpCntMax();
			if (GetAnim() == "�W�����v��")
			{
				dashFlag ? SetAnim("����") : SetAnim("�ҋ@");
			}
			if (GetAnim() == "�ʏ�U��_����_��")
			{
				fixCtrlCnt = FIX_TIME;
				SetAnim("�ҋ@");
			}
			vY = 0.0f;
			vX = 0.0f;

			return true;
		}
	}

	return false;
}

void Obj::UniqueColGround(void)
{
}

void Obj::PosAdjust(VECTOR2 &tmpPos, int maxEndPosY)
{
	// �n�ʂɏ���Ă���ꍇ�̍��W�␳
	if (animIdOld != animId)
	{
		if (locate == LOCATE::GROUND)
		{
			tmpPos.y = GROUND_POS_Y - maxEndPosY;
			vY = 0.0f;
			vX = 0.0f;
		}
	}

	// �����Ă��鏰�ɏ���Ă���ꍇ�̍��W�␳
	if ((locate == LOCATE::FLOOR) && (parentFloor.itrCnt != 99999))
	{
		auto floor = floorList->begin();
		for (int i = 0; i < parentFloor.itrCnt; i++)
		{
			floor++;
		}
		// �����Ă��鏰�����������W������ڲԂ̍��W�𓮂���
		//tmpPos += (parentFloor.oldPos - (*itr)->GetPos());
		tmpPos.y = (*floor)->GetPos().y - maxEndPosY;
		tmpPos.x += (*floor)->GetPos().x - parentFloor.oldPos.x;
		vY = 0.0f;
		vX = 0.0f;
		//tmpPos.y = (*itr)->GetPos().y - maxEndPosY;
		parentFloor.itrCnt = 99999;	// �װ���Ă����Ă���
	}
}

void Obj::SetNextAnim(bool animEndFlag)
{
	if (!animEndFlag)
	{
		return;
	}
	if (GetAnim() == "�W�����v�n�܂�")
	{
		SetAnim("�W�����v��");
	}

	// �U���n��Ұ��݂��I��������
	if (animPtnTbl[GetAnim()] != ANIM_TYPE_NO_ATTACK)
	{
		// ���ߎn�܂��Ұ��݂��I�������痭�ߒ���Ұ��݂ֈڍs
		if (GetAnim() == "���ߎn�܂�")
		{
			SetAnim("���ߒ�");
		}
		else if (GetAnim() == "�ʏ�U��_���n�܂�")
		{
			(locate == LOCATE::NON) ? SetAnim("�ʏ�U��_����_��") : SetAnim("�ʏ�U��_����_�n��");
		}
		// �ʏ�U���n��Ұ��݂��I��������ҋ@��Ұ��݂ֈڍs
		else
		{
			fixCtrlCnt = FIX_TIME;
			// �U����Ұ��ݏI�����ɘA���׸ނ������Ă��邩
			std::string str;
			switch (deri)
			{
			case DERI_NOMAL:
				deri = DERI_NON;
				fixCtrlCnt = 0;
				multiAttackNum++;
				str = "�ʏ�U��" + std::to_string(multiAttackNum + 1);
				SetAnim(str);
				break;

			case DERI_UP:
				deri = DERI_NON;
				fixCtrlCnt = 0;
				SetAnim("�ʏ�U��_��");
				multiAttackNum = 0;
				break;

			case DERI_DOWN:
				deri = DERI_NON;
				fixCtrlCnt = 0;
				SetAnim("�ʏ�U��_���n�܂�");
				multiAttackNum = 0;
				break;

			case DERI_NON:
			default:
				deri = DERI_NON;
				if (GetAnim() == "�_���[�W")
				{
					fixCtrlCnt = 0;
					if (!spAttackHitFlag)
					{
						SetAnim("�ҋ@");
					}
				}
				else
				{
					SetAnim("�ҋ@");
				}
				multiAttackNum = 0;
				break;
			}

			if (multiAttackNum > 2)
			{
				fixCtrlCnt = 0;
				multiAttackNum = 0;	// 3�A���ŏI��
				SetAnim("�ҋ@");
			}

			chargeStage = CHARGE_NON;
			gravity = 0.4f;
		}
	}
}

void Obj::MoveEndUpDate(VECTOR2 tmpPos, const GameController &controller, int maxEndPosY)
{
	// ��ʊO�ɍs���Ȃ��悤��
	PosOutOfRangeCtrl(tmpPos, maxEndPosY);

	if (vY > 0.0f)
	{
		locate = LOCATE::NON;
	}

	// ���W�ړ�
	pos = tmpPos;

	if (locate == LOCATE::NON)
	{
		auto anim = GetAnim();
		if (anim == "����" || anim == "�ҋ@" || anim == "����")
		{
			SetAnim("�W�����v��");
		}
	}

	SetNextAnim(animEndFlag);

	FlyUpDate(controller);	// �P���V���̏�ԍX�V
	FlyTextUpDate();		// �ײ÷�Ă̏�ԍX�V
	HitEffectUpDate();		// ˯Ĵ̪�Ă̏�ԍX�V
	PlStateUpDate();		// plStateList�̏�ԍX�V
	
	PlaySe();				//Se�̍Đ�

	if (GetAnim() == "�K�E�Z")
	{
		spSeCnt++;
	}
	else
	{
		spSeCnt = 0;
	}
	PlaySPSe();				// �K�E�ZSe�̍Đ�
}

int Obj::GetSpAttackTimeMax(void)
{
	return 0;
}

void Obj::PosOutOfRangeCtrl(VECTOR2& pos, int maxEndPosY)
{
	// ��ʊO�ɍs���Ȃ��悤��
	auto screenSize = lpSceneMng.GetScreenSize();
	if (pos.x < 0)
	{
		pos.x = 0;
	}
	if (pos.x > screenSize.x)
	{
		pos.x = screenSize.x;
	}
	if (pos.y < 0)
	{
		pos.y = 0;
	}
	if (pos.y > GROUND_POS_Y - maxEndPosY)
	{
		pos.y = GROUND_POS_Y - maxEndPosY;
	}
}

void Obj::FlyUpDate(const GameController &controller)
{
	if (gravity == 0)
	{
		// �P���V�Œ��n�����ꍇ
		if (locate != LOCATE::NON)
		{
			gravity = 0.4f;		// �d�͂�0.4�ɖ߂�
			SetAnim("�ҋ@");
		}
		// �P���V���ɍU�������͂��������ꍇ�͏d�͂�0.4�ɖ߂�
		if (
			controller.GetInputDown(GPAD_INPUT_B, padNum)	// �K�E�Z
			|| controller.GetInputDown(GPAD_INPUT_X, padNum)	// �ʏ�U��
			|| controller.GetInputDown(GPAD_INPUT_Y, padNum)	// ���U��
			|| controller.GetInputDown(GPAD_INPUT_A, padNum)	// �ެ���
			)
		{
			gravity = 0.4f;		//�d�͂�0.4�ɖ߂�
		}
		// �P���V���Ɂ������͂��������ꍇ�͏d�͂�0.4�ɖ߂��A�ެ��ߒ���Ұ��݂ֈڍs
		if (controller.GetInputDown(PAD_INPUT_DOWN, padNum))
		{
			gravity = 0.4f;
			SetAnim("�W�����v��");
		}
	}
}

void Obj::FlyTextUpDate(void)
{
	if (flyTextList.size())
	{
		for (auto itr = flyTextList.begin(); itr != flyTextList.end();)
		{
			itr->cnt--;
			itr->insPos.y -= 1;
			if (itr->cnt < 0)
			{
				itr = flyTextList.erase(itr);
			}
			else
			{
				itr++;
			}
		}
	}
}

void Obj::HitEffectUpDate(void)
{
	// ������X�V
	if (hitEffect.cnt > 0)
	{
		hitEffect.cnt--;
	}

	// �������牺�͐V����˯Ĵ̪�Ă�list�ɒǉ����鏈��
	if (CheckDamageSe() == DMG_MAX)
	{
		return;		// ��Ұ�ނ��󂯂Ă��Ȃ��Ȃ�return
	}

	HIT_EFFECT data;
	// ��`�ɐ�������
	// -120������-60��(-2.10 �` -1.05)�̊Ԃ�����тȐ������擾���� 
	int length = -((rand() % (105 + 1)) + 105);			// -210�`-105���擾
	data.rota = static_cast<double>(length) / 100.0;	// -2.10�`-1.05���擾
														// �������W�̌���
	data.insPos.x += static_cast<int>(100 * cos(data.rota) + pos.x);
	data.insPos.y += static_cast<int>(100 * sin(data.rota) + pos.y);

	data.cnt = 60;
	data.damageRate = CheckDamageSe();

	// �ް��X�V
	hitEffect = data;
}

void Obj::PlStateUpDate(void)
{
	// ؽĂɒǉ���������
	auto listPush = [&](PL_STATE state)
	{
		bool flag = true;
		for (auto data : plStateList)
		{
			if (data == state)
			{
				flag = false;
			}
		}
		if (flag)
		{
			// list�̒��ɒǉ�PL_STATE_SEWAT���Ȃ��Ȃ�ǉ�����
			plStateList.push_back(state);
		}
	};

	// ؽĂ���폜��������
	auto listErase = [&](PL_STATE state)
	{
		for (auto itr = plStateList.begin(); itr != plStateList.end(); itr++)
		{
			if (*itr == state)
			{
				if (itr == plStateItr)
				{
					plStateItr = plStateList.erase(itr);
					if (plStateItr == plStateList.end())
					{
						plStateItr = plStateList.begin();
					}
				}
				else
				{
					plStateList.erase(itr);
				}
				plStateCnt = 30;
				break;
			}
		}
	};

	// ؽĂɒǉ�������폜������
	if (hp <= HP_RED_COLOR_NUM)
	{
		listPush(PL_STATE_SEWAT);
	}
	angryFlag ? listPush(PL_STATE_ANGRY) : listErase(PL_STATE_ANGRY);
	specialPoint >= 100 ? listPush(PL_STATE_SPECIAL) : listErase(PL_STATE_SPECIAL);


	if (plStateList.size() == 1)
	{
		plStateItr = plStateList.begin();
	}

	// ���݂�2�ȏ�̎�
	if (plStateList.size() >= 2)
	{
		plStateCnt--;
		if (plStateCnt <= 0)
		{
			plStateCnt = 30;
			auto testItr = plStateItr;
			testItr++;
			if (testItr == plStateList.end())
			{
				plStateItr = plStateList.begin();
			}
			else
			{
				plStateItr++;
			}
		}
	}
}

void Obj::AngryGaugeDraw(void)
{
	// ��ʉ�����׏��̕`��(�f�o�b�O�R�[�h�ł��[)
	std::string iconName = "image/" + charName + "/�A�C�R��/" + "icon_play.png";
	std::string iconNameRed = "image/" + charName + "/�A�C�R��/" + "iconRed.png";

	// ��ױ��݂̕`��
	VECTOR2 iconSize(163, 150);
	float iconStdSize = 150.0f / 100.0f;
	int localAngryPoint = static_cast<int>(angryFlag ? 100 : angryPoint);

	DrawGraph(320 * padNum , 565, lpImageMng.GetID(iconName)[0], true);

	// ��ױ��ݓ{��UI
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	int startPosFromlength = static_cast<int>(iconSize.y - iconStdSize * localAngryPoint);

	DrawRectGraph(
		320 * padNum, 565 + startPosFromlength,
		0, startPosFromlength,
		iconSize.x, iconSize.y - startPosFromlength,
		lpImageMng.GetID(iconNameRed)[0], true, false);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 128);
}

void Obj::SpecialGaugeDraw(bool cutInFlag)
{
	float gaugeStdSize = static_cast<float>(SPECIAL_GAUGE_SIZE_X) / 100.0f;
	VECTOR2 startPos = VECTOR2(120 + 320 * padNum + 8, 660 + 4);
	int gaugeColor = spGaugeColorTbl[(specialPoint / 20) % spGaugeColorTbl.size()];

	// �������̕`��
	DrawBox(
		startPos,
		VECTOR2(startPos.x + SPECIAL_GAUGE_SIZE_X, startPos.y + SPECIAL_GAUGE_SIZE_Y),
		0xffffff, true);

	// �F�t�������̕`��
	if (specialPoint >= 100)
	{
		DrawRectGraph(
			startPos.x, startPos.y,
			320 - (framCnt*5%320),0,
			SPECIAL_GAUGE_SIZE_X, SPECIAL_GAUGE_SIZE_Y,
			lpImageMng.GetID("image/�Q�[���V�[���p/��.png")[0],true,false);
	}
	else
	{
		DrawBox(
			startPos,
			VECTOR2(startPos.x + static_cast<int>(gaugeStdSize * specialPoint), startPos.y + SPECIAL_GAUGE_SIZE_Y),
			gaugeColor, true);
	}

	// ��Ĳݒ��Ȃ��ʂ��Â��A�����łȂ��Ȃ疾�邭
	if (cutInFlag)
	{
		SetDrawBright(140, 140, 140);
	}
	else
	{
		SetDrawBright(255, 255, 255);
	}

	//// �K�E�Q�[�W�g
	//DrawGraph(130 + 320 * padNum, 660, lpImageMng.GetID("image/UI/specialGaugeFrame.png")[0], true);
	// �K�E�Q�[�W�g
	DrawGraph(120 + 320 * padNum, 660, lpImageMng.GetID("image/UI/specialGaugeFrame.png")[0], true);
}

void Obj::GuardDraw(void)
{
	if (GetAnim() != "�K�[�h")
	{
		return;
	}

	// �ް�޴̪�Ă̕\��
	int cnt = animCnt / 5;	// ++������[ 
	DrawRectGraph(
		pos.x - 150 / 2, pos.y - 150 / 2,
		150 * (cnt % 5), 150 * (cnt / 5 % 2),
		150, 150,
		lpImageMng.GetID("image/UI/�⏕4.png")[0], true, false);

	// �ۂ��摜�̕\��
	float extRate = static_cast<float>(guardTime) / GetGuardTimeMax();
	DrawRotaGraph(pos, extRate, 0, guardImage, true, false);
}

void Obj::StateIconDraw(void)
{
	if (hp <= 0)
	{
		return;
	}

	// ���݂�1�̎��͂��̱��݂����݂ɕ`��
	if (plStateList.size() == 1)
	{
		std::string fName = "image/UI/" + plStateFileName[*plStateList.begin()];
		DrawRectGraph(
			pos.x + ((static_cast<int>(dir) * 2 - 1) * 28) - 32 / 2, pos.y - 50,
			((framCnt / 30) % 2) * 32, 0,
			32, 32, lpImageMng.GetID(fName)[0], true, static_cast<int>(dir) - 1);
	}
	// ���݂�2�ȏ�̎�
	if (plStateList.size() >= 2)
	{
		std::string fName = "image/UI/" + plStateFileName[*plStateItr];
		DrawRectGraph(
			pos.x + ((static_cast<int>(dir) * 2 - 1) * 28) - 32 / 2, pos.y - 50,
			0, 0,
			32, 32, lpImageMng.GetID(fName)[0], true, static_cast<int>(dir) - 1);
	}
}

void Obj::FlyTextDraw(void)
{
	if (hp <= 0)
	{
		return;
	}

	// �ײ÷�Ă̕`��
	if (flyTextList.size())
	{
		for (auto text : flyTextList)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>((255.0f / (float)FLY_TEXT_VANISH_CNT) * text.cnt));
			DrawFormatStringToHandle(text.insPos.x, text.insPos.y, 0xff0000, lpFontMng.SetFont("���ʂ�20��"), "%d", text.damage);
		}
	}

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}

void Obj::CharacterDraw(std::vector<ColInf> collisonVec, bool moveStopFlag, CUT_IN_INFO cutInInfo)
{
	if (!((visible / 5) % 2) || cutInInfo.flag)
	{
		if (animId < IMAGE_ID(imageName).size())
		{
			auto offset = (*collisonVec.begin()).offset;
			// dir == LEFT �Ȃ瓖���蔻��̌������t�ɂ���
			offset.x *= static_cast<int>(dir) * -2 + 1;
			VECTOR2 drawPos = pos + offset;

			if (angryFlag)
			{
				SetDrawBright(255, 0, 0);
			}
			if (spAttackCnt)
			{
				int rgb = HsvChangeRgb((framCnt * 10) % 360, 255, 255);
				int r = (rgb & 0xff0000) >> 16;
				int g = (rgb & 0x00ff00) >> 8;
				int b = (rgb & 0x0000ff);
				SetDrawBright(r, g, b);
			}

			if (GetAnim() == "���S")
			{
				SetDrawBlendMode(DX_BLENDMODE_INVSRC, 255 - animCnt * 2);
				DrawRotaGraph(
					drawPos,
					1,
					0,
					IMAGE_ID(imageName)[animId],
					true,
					static_cast<int>(dir));
				SetDrawBlendMode(DX_BLENDMODE_ADD, 255 - animCnt * 2);
			}

			if (cutInInfo.flag)
			{
				if (cutInInfo.padNum == padNum)
				{
					SetDrawBright(255, 255, 255);
				}
				else
				{
					// ��Ĳݒ��A�K�E�Z�������̷�׈ȊO�͈Â�����
					SetDrawBright(140, 140, 140);
				}
			}

			// �c���̕`��
			for (auto &shadow : shadowList)
			{
				shadow.cnt--;
			}
			shadowList.remove_if([](SHADOW& shadow) {return shadow.cnt < 0; });
			if (angryFlag || spAttackCnt)
			{
				for (auto shadow : shadowList)
				{
					SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((shadow.cnt / 2) * 255) / SHADOW_DRAW_FRAM);

					DrawRotaGraph(
						shadow.pos + offset,
						1,
						0,
						IMAGE_ID(charName + "/" + shadow.animName)[shadow.animid],
						true,
						static_cast<int>(shadow.dir));
				}
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
			}

			DrawRotaGraph(
				drawPos,
				1,
				0,
				IMAGE_ID(imageName)[animId],
				true,
				static_cast<int>(dir));

			if (angryFlag || spAttackCnt)
			{
				SHADOW shadow = {
					pos,
					offset,
					animName,
					animId,
					dir,
					SHADOW_DRAW_FRAM
				};
				shadowList.push_back(shadow);
			}

			// ��Ĳݒ��Ȃ��ʂ��Â��A�����łȂ��Ȃ疾�邭
			if (cutInInfo.flag)
			{
				SetDrawBright(140, 140, 140);
			}
			else
			{
				SetDrawBright(255, 255, 255);
			}
		}
	}
}

const VECTOR2 & Obj::GetPos(void)
{
	return pos;
}

const VECTOR2 Obj::GetPosOld(void)
{
	return posOld;
}

bool Obj::AddAnim(std::string animName, int inv, bool loop, std::string animFile)
{
	int frame = lpImageMng.GetSize(charName + "/" + animName);
	animTable[animName][ANIM_TBL_START_ID] = 0;			// �����Obj�׽�ł͕K��0
	animTable[animName][ANIM_TBL_FRAME] = frame;
	animTable[animName][ANIM_TBL_INV] = inv;
	animTable[animName][ANIM_TBL_LOOP] = loop;
	animFileName[animName] = animFile;
	return true;
}


bool Obj::SetAnim(std::string animName)
{
	// �ް������݂��Ă�������ޕϐ���animName�Ɉ�����Ă��A���Ă��Ȃ����false��Ԃ�
	if (Obj::animName == animName)
	{
		return true;
	}
	if (animTable.find(animName) == animTable.end())
	{
		return false;
	}

	animEndFlag = false;
	Obj::animName = animName;
	Obj::imageName = charName + "/" + animName;
	Obj::animCnt = 0;
	animId = 0;

	return true;
}

void Obj::AddSpecialPoint(int specialPoint)
{
	this->specialPoint += specialPoint;
	if (this->specialPoint > 100)
	{
		this->specialPoint = 100;
	}
}

void Obj::AddAngryPoint(int angryPoint)
{
	if (!angryFlag)
	{
		this->angryPoint += angryPoint;
		if (this->angryPoint > 100.0f)
		{
			this->angryPoint = 100.0f;
		}
	}

	if (this->angryPoint < 0.0f)
	{
		this->angryPoint = 0.0f;
	}
}

void Obj::AddChickenKillCnt(void)
{
	resultInfo.chickenKillCnt[padNum]++;
	return;
}

const std::string Obj::GetAnim(void)
{
	return animName;
}

const std::string Obj::GetAnim(ANIM_STATE type)
{
	// 1�ڰёO
	if (type == ANIM_OLD)
	{
		return animNameOld;
	}

	// ���݂��ڰ�
	return animName;
}

const std::vector<ColInf> Obj::GetPlayerCollision(void)
{
	return lpColMng.GetCollider(charName, animName, animId);
}

const DIR Obj::GetDir(void)
{
	return dir;
}

const int Obj::GetHp(void)
{
	return hp;
}

const int Obj::GetAnimId(void)
{
	return animId;
}

const int Obj::GetAnimFrame(void)
{
	return lpImageMng.GetSize(imageName);
}

const VECTOR2 Obj::GetFlyHighPower(void)
{  
	return  VECTOR2(FLY_HIGH_POWER.x * (static_cast<int>(dir) * -2 + 1) , FLY_HIGH_POWER.y);
}

const std::string Obj::GetCharName(void)
{
	return charName;
}

const RESULT_INFO Obj::GetResultInfo(void)
{
	return resultInfo;
}

const bool Obj::CheckSpAttackCnt(void)
{
	return static_cast<bool>(spAttackCnt);
}