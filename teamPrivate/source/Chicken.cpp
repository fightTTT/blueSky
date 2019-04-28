#include "SceneMng.h"
#include "ImageMng.h"
#include "ColMng.h"
#include "Collision.h"
#include "Chicken.h"
#include "SoundMng.h"

#define JUMP_CNT_MAX	(2)
#define INVISIBLE_TIME	(60)
#define CHICKEN_FIX_TIME		(15)
#define CHICKEN_SIZE	(48)

#define EXP_SIZE		(96)
#define EXP_FRAM_X		(6)
#define EXP_FRAM_Y		(4)
#define EXP_INV			(3)
#define EXP_CNT_MAX		(EXP_FRAM_X * EXP_FRAM_Y)
#define UI_UP_CNT		(150)

Chicken::Chicken(sharedListFloor floorList, VECTOR2 pos, int padNum, CHICKEN_COLOR chickenColor)
{
	Obj::AddChickenCnt(padNum);

	Chicken::color		= chickenColor;
	Chicken::floorList	= floorList;
	Chicken::padNum		= padNum;
	Chicken::pos		= pos;

	chickenImagePass	= {
		"oniwatorisama01",
		"oniwatorisama02",
		"nwtr"
	};

	imageName = "image/�{/" + chickenImagePass[chickenColor] + ".png";
	lpImageMng.GetID("image/�{/blast.png", VECTOR2(EXP_SIZE, EXP_SIZE), VECTOR2(EXP_FRAM_X, EXP_FRAM_Y));

	hp			= 2;
	speed		= 4;
	gravity		= 0.4f;

	dirOld		= DIR::RIGHT;
	vY			= 0;
	vX			= 0;
	locate		= LOCATE::NON;
	param		= PARAM::FALLING;
	parentFloor = {};
	fallFlag	= false;
	jumpCnt		= JUMP_CNT_MAX;
	downFlag	= false;
	nowTimeDown = 0;
	nowTimeLR	= 0;
	framCnt		= 0;
	dashFlag	= false;
	visible		= 0;
	fixCtrl		= 0;
	damageInv	= 0;
	expCnt		= 0;
	deathFlag	= false;
	chickenSoul = {};
	chickenSoul.image	= -1;	// �����ǂݍ���ł��Ȃ����Ƃ�\�����߁A�װ�l�����Ă���
	floorCheckFlag		= true;

	// UI
	UICnt		= 0;

	// �����ǉ�
	lpSoundMng.SoundInit("sound/SE��/Chicken1.mp3", "������");
	lpSoundMng.SoundInit("sound/SE��/Chicken2.mp3", "���");
	lpSoundMng.SoundInit("sound/SE��/Chicken3.mp3", "���");
	lpSoundMng.SoundInit("sound/SE��/Chicken4.mp3", "�������");
	lpSoundMng.SoundInit("sound/SE��/Bomb.mp3", "����",85); 
	lpSoundMng.SoundInit("sound/SE��/Dead.mp3", "���S");
	lpSoundMng.SoundInit("sound/SE��/Continue.mp3", "����",85);

	// �Đ�
	lpSoundMng.SetSound("����");
	lpSoundMng.SetSound("������");
	
	chickenWalkSeCnt = 0;
	deathSeFlag = false;
};


Chicken::~Chicken()
{
}

void Chicken::UpDate(const GameController & controller, weakListChicken chickenList)
{

	if (visible)
	{
		visible--;
	}

	if (deathFlag && expCnt)
	{
		return;
	}

	// UI�̕\���ʒu���w��ʒu�܂ŏ㏸
	if (UICnt < UI_UP_CNT)
	{
		UICnt+=2;
	}

	// ���S && ���V�ς� && �]�����Ă��Ȃ�
	if (deathFlag && chickenSoul.alpha <= 0 && !chickenSoul.endFlag)
	{
		chickenSoul.endFlag = true;
		AddChickenList()(chickenList, std::make_unique<Chicken>(floorList, VECTOR2(pos.x,-CHICKEN_SIZE), padNum, color));
	}

	if (fixCtrl)
	{
		fixCtrl--;
	}

	if (damageInv)
	{
		damageInv--;
	}

	if (damageInv == 1)
	{
		hitPlayerInf = {};
	}

	framCnt++;
	dirOld = dir;

	VECTOR2 tmpPos = pos;

	// �����̺ؼޮ݂̎擾
	auto myCollision = GetMyCol();

	int maxEndPosY = 0;				// ���ڰт̺ײ�ް�̍ő�Y���W
	for (auto &col : myCollision)
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

	if (param == PARAM::FALLING && !downFlag)
	{
		floorCheckFlag = true;
	}

	if (!fixCtrl && !deathFlag)
	{
		// �ޯ��
		Dash(controller, tmpPos);

		// �ެ���
		if (controller.GetInputDown(GPAD_INPUT_A, padNum) && (jumpCnt > 0))
		{
			// �Đ�
			lpSoundMng.SetSound("���");

			jumpCnt--;
			downFlag = false;
			param = PARAM::JUMPING;
			floorCheckFlag = false;

			// ���L�[�������Ă��� && �n�ɑ������Ă���� �n�C�W�����v
			vY = (controller.GetInputHold(PAD_INPUT_DOWN, padNum) && locate != LOCATE::NON ? -15.0f : -10.0f);
			locate = LOCATE::NON;
		}


		// �����L�[�̏���
		if (controller.GetInputDown(PAD_INPUT_DOWN, padNum))
		{
			if ((framCnt - nowTimeDown) < PRASS_INV && locate != LOCATE::NON)
			{
				floorCheckFlag = false;
				downFlag = true;
			}
			nowTimeDown = framCnt;
		}
		if (controller.GetInputHold(PAD_INPUT_DOWN, padNum))
		{
			// �󒆁��L�[�ŋ}���~
			if ((locate == LOCATE::NON) && (param == PARAM::FALLING))
			{
				fallFlag = true;
			}
		}
	}

	if (param != PARAM::JUMPING)
	{
		bool hitFlag = false;		// �����n�ʂɂ���true

		auto GetColPos = [&](VECTOR2 pos)
		{
			pos.x *= static_cast<int>(dir) * -2 + 1;
			return VECTOR2(tmpPos + pos);
		};

		// �����Ă��鏰�Ƃ̓����蔻��
		if (floorCheckFlag)
		{
			bool flag = false;
			int itrCnt = 0;
			for (auto itr : *floorList)
			{
				VECTOR2 floorStartPos = itr->GetPos();
				VECTOR2 floorEndPos = itr->GetPos() + itr->GetSize();
				for (auto &col : myCollision)
				{
					if (col.weponFlag)
					{
						continue;
					}

					VECTOR2 startPos	= GetColPos(col.startPos);
					VECTOR2 endPos		= GetColPos(col.endPos);

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

		for (auto &col : myCollision)
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
			jumpCnt = JUMP_CNT_MAX;
			vY = 0.0f;
			vX = 0.0f;
		}
	}

	// ��ʊO�ɍs���Ȃ��悤��
	auto screenSize = lpSceneMng.GetScreenSize();
	if (tmpPos.x <= 0)
	{
		tmpPos.x = 0;
	}
	if (tmpPos.x > screenSize.x)
	{
		tmpPos.x = screenSize.x;
	}
	if (tmpPos.y <= 0)
	{
		tmpPos.y = 0;
	}
	if (tmpPos.y > screenSize.y)
	{
		tmpPos.y = GROUND_POS_Y - maxEndPosY;
	}

	if (vY > 0.0f)
	{
		locate = LOCATE::NON;
	}

	// ���W�ړ�
	pos = tmpPos;

}

void Chicken::CheckHit(weakListObj objList)
{
	if (expCnt)
	{
		expCnt--;
	}
	else if (deathFlag && chickenSoul.alpha > 0)
	{
		chickenSoul.pos.y--;
		chickenSoul.alpha -= 2;
	}
	else
	{
	}

	if (deathFlag)
	{
		return;
	}

	if (visible)
	{
		return;					// ���G���Ԓ��Ȃ瓖���蔻����s��Ȃ�
	}

	for (auto &obj : *objList.lock())
	{
		// ���肵�悤�Ƃ��Ă���U����������ڲ԰����󂯂��Ō�̍U���Ɠ����Ȃ�
		if (hitPlayerInf[(*obj).GetPadNum()].attackName == (*obj).GetAnim())
		{
			if (hitPlayerInf[(*obj).GetPadNum()].animId == (*obj).GetAnimId())
			{
				continue;			// ���肵�悤�Ƃ��Ă���U����������ڲ԰����󂯂��Ō�̍U���Ɠ����ςȂ画����s��Ȃ�
			}
		}


		// �����̺ؼޮ݂̎擾
		auto myCollision = GetMyCol();

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

				otherCol.startPos.x *= static_cast<int>((*obj).GetDir()) * -2 + 1;
				otherCol.endPos.x *= static_cast<int>((*obj).GetDir()) * -2 + 1;
				VECTOR2 otherStartPos = (*obj).GetPos() + otherCol.startPos;
				VECTOR2 otherEndPos = (*obj).GetPos() + otherCol.endPos;

				// �����蔻����s��
				if (Square(myStartPos, myEndPos, otherStartPos, otherEndPos))
				{
					// �U�����󂯂��� --------------------------
					hitPlayerInf[(*obj).GetPadNum()] = {	// �󂯂��U���̏����i�[
						(*obj).GetAnim(),
						(*obj).GetAnimId()
					};

					vY = -10.0f;
					vX =  10.0f * (static_cast<int>((*obj).GetDir()) * -2 + 1);
					damageInv = 60;
					locate = LOCATE::NON;
					visible = INVISIBLE_TIME;
					fixCtrl = CHICKEN_FIX_TIME;

					hp--;
					if (!hp)
					{
						// �Đ�
						lpSoundMng.SetSound("����");
						deathFlag = true;
						(*obj).AddChickenKillCnt();
						expCnt = EXP_FRAM_X * EXP_FRAM_Y * EXP_INV - 1;
						vY = 0.0f;
						vX = 0.0f;
					}

					// �Đ�
					lpSoundMng.SetSound("���");


					return;
				}
			}
		}
	}
}

void Chicken::Draw(void)
{
	if (!((visible / 5) % 2))
	{
		double rate = 1.0;
		int r, g, b;
		GetDrawBright(&r,&g,&b);
		if ( hp <= 1 && (framCnt/5 %2) && !deathFlag)
		{
			SetDrawBright(255, 128, 128);
			rate = 1.25;
		}
		// ��ݖ{��
		DrawRotaGraph(
			pos,
			rate,
			static_cast<double>(chickenSoul.image != -1) * 3.141592,
			lpImageMng.GetID(imageName)[0],
			true,
			static_cast<int>(dir));
		SetDrawBright(r,g,b);
	}

	if (expCnt)
	{
		// ����
		int animCnt = ((EXP_CNT_MAX * EXP_INV - 1) - expCnt) / EXP_INV;
		DrawRotaGraph(
			pos,
			animCnt * 3.0 / EXP_CNT_MAX,
			0.0,
			lpImageMng.GetID("image/�{/blast.png")[animCnt],
			true);
	}
	else
	{
		if (deathFlag)
		{
			if (chickenSoul.image == -1)
			{
				// chickenSoul�̏�����
				chickenSoul.pos	  = pos;
				chickenSoul.alpha = 255;

				// �^������chicken�����
				chickenSoul.image = MakeScreen(CHICKEN_SIZE, CHICKEN_SIZE,true);
				SetDrawScreen(chickenSoul.image);
				ClsDrawScreen();
				SetDrawBlendMode(DX_BLENDMODE_INVSRC, 255);
				DrawRotaGraph(
					CHICKEN_SIZE / 2,
					CHICKEN_SIZE / 2,
					1.0,
					0.0,
					lpImageMng.GetID(imageName)[0],
					true,
					static_cast<int>(dir));
				SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
				DrawRotaGraph(
					CHICKEN_SIZE / 2,
					CHICKEN_SIZE / 2,
					1.0,
					0.0,
					lpImageMng.GetID(imageName)[0],
					true,
					static_cast<int>(dir));
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
				SetDrawScreen(lpSceneMng.GetTmpBuffer());
			}

			if (chickenSoul.alpha > 0)
			{
				// �Đ�
				if (!deathSeFlag)
				{
					lpSoundMng.SetSound("���S");
					deathSeFlag = true;
				}

				// ���S��̔������I������� && �󒆂ł͂Ȃ�
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, chickenSoul.alpha);
				DrawRotaGraph(
					chickenSoul.pos,
					1.0,
					0.0,
					chickenSoul.image,
					true,
					static_cast<int>(dir));
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
			}
		}
	}

	// UI�`��
	if (!deathFlag)
	{
		int chickenCnt = Obj::GetChickenCnt(padNum);
		int drawLengh;
		drawLengh = GetDrawFormatStringWidthToHandle(
			lpFontMng.SetFont("EP50"), 
			chickenCnt == 1 ? "����" : "%d���", chickenCnt);
		DrawFormatStringToHandle(320 * (padNum+1) - drawLengh, SCREEN_SIZE_Y - UICnt, 0xff0000, 
			lpFontMng.SetFont("EP50"), 
			chickenCnt == 1 ? "����" : "%d���", chickenCnt, true);
		DrawGraph(VECTOR2(320 * padNum, SCREEN_SIZE_Y - UICnt), lpImageMng.GetID("image/�{/�A�C�R��.png")[0], true);
	}
}

const int Chicken::GetPadNum(void)
{
	return padNum;
}

bool Chicken::CheckExpCnt(void)
{
	return (expCnt > 0);
}

void Chicken::Dash(const GameController & controller, VECTOR2 & tmpPos)
{
	if (controller.GetInputHold(PAD_INPUT_RIGHT, padNum))
	{
		dir = DIR::RIGHT;
		tmpPos.x += speed * (1 + static_cast<int>(dashFlag));
		
		chickenWalkSeCnt++;
	}
	else if (controller.GetInputHold(PAD_INPUT_LEFT, padNum))
	{
		dir = DIR::LEFT;
		tmpPos.x -= speed * (1 + static_cast<int>(dashFlag));

		chickenWalkSeCnt++;
	}
	else
	{
		dashFlag = false;

		chickenWalkSeCnt = 0;
	}

	if (dir != dirOld)
	{
		if (locate != LOCATE::NON)
		{
			dashFlag = false;
		}
	}

	if ((controller.GetInputDown(PAD_INPUT_RIGHT, padNum) || controller.GetInputDown(PAD_INPUT_LEFT, padNum)))
	{
		if (locate != LOCATE::NON)
		{
			if ((framCnt - nowTimeLR < PRASS_INV) && (dir == dirOld))
			{
				dashFlag = true;

				chickenWalkSeCnt++;
			}
			else
			{
				dashFlag = false;

				chickenWalkSeCnt = 0;
			}
		}
		nowTimeLR = framCnt;
	}
	if (locate == LOCATE::GROUND || locate == LOCATE::FLOOR)
	{
		if (chickenWalkSeCnt % 13 == 1)
		{
			lpSoundMng.SetSound("�������");
		}
	}
}

std::vector<ColInf> Chicken::GetMyCol(void)
{
	std::vector<ColInf> myCollision;
	myCollision.resize(1);
	myCollision[0] = {};
	myCollision[0].boxFlag = true;
	myCollision[0].id = 0;
	myCollision[0].startPos = { -CHICKEN_SIZE / 2,-CHICKEN_SIZE / 2 };
	myCollision[0].endPos = { CHICKEN_SIZE / 2,CHICKEN_SIZE / 2 };

	return myCollision;
}

void Chicken::PosAdjust(VECTOR2 & tmpPos, int maxEndPosY)
{
	// �n�ʂɏ���Ă���ꍇ�̍��W�␳
	if (locate == LOCATE::GROUND)
	{
		tmpPos.y = GROUND_POS_Y - maxEndPosY;
		vY = 0.0f;
		vX = 0.0f;
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
		tmpPos.y =  (*floor)->GetPos().y - maxEndPosY;
		tmpPos.x += (*floor)->GetPos().x - parentFloor.oldPos.x;
		vY = 0.0f;
		vX = 0.0f;
		//tmpPos.y = (*itr)->GetPos().y - maxEndPosY;
		parentFloor.itrCnt = 99999;	// �װ���Ă����Ă���
	}
}
