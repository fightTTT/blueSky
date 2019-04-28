#include "ExpOrb.h"
#include "math.h"
#include <stdlib.h>
#include "Obj.h"
#include "ImageMng.h"
#include "SoundMng.h"

#define PI				3.141592

#define WAIT_TIME			60		// ��������Ă���҂��ڰѐ�
#define WAIT_VANISH_TIME	(90 + WAIT_TIME) 		// �����ڰт𒴂���ƖړI�n�����ł����Ɣ��f���ċʂ�j��
#define INS_RAND_RANGE		64		// �~�`�ɐ��������ۂ̒��S���W����̋���

#define ARRIVE_RANGE		20		// �I�_���W���甼�aARRIVE_RANGE�̉~�������āA���̒��ɋʂ������delete

#define ORB_SIZE			32		// �ް�ދʂ̻���
#define ORB_FLASH_INV		6		// �ް�ދʂ̱�Ұ��݊Ԋu
#define ANIM_MAX			4		// ��Ұ��ݖ���

ExpOrb::ExpOrb(VECTOR2 startPos, int padNum, int addPoint)
{
	// �~�`�ɐ�������
	// -180������180��(-3.14 �` 3.14)�̊Ԃ�����тȐ������擾���� 
	int length314 = (rand() % (314 * 2 + 1)) - 314;				// -314�`314���擾
	double circleRad = static_cast<double>(length314) / 100.0;	// -3.14�`3.14���擾
	// �������W�̌���
	startPos.x += static_cast<int>(INS_RAND_RANGE * cos(circleRad));
	startPos.y += static_cast<int>(INS_RAND_RANGE * sin(circleRad));

	// �������֌W
	cnt = 0;
	pos_d.x = startPos.x;
	pos_d.y = startPos.y;

	this->addPoint	= addPoint;
	this->padNum	= padNum;

	animId = 0;

	this->endPos_d.x = 999.0;
	this->endPos_d.y = 999.0;

	lpSoundMng.SoundInit("sound/SE��/�ʎ擾.mp3", "�ʎ擾");

}

ExpOrb::~ExpOrb()
{
}

void ExpOrb::UpDate(weakListObj objList)
{
	if (cnt > WAIT_TIME)
	{
		POS_d endPos_d;

		bool moveF = false;		// true:�ړ�OK	false:�ړ��s��

		for (auto &obj : *objList.lock())
		{
			if (obj->GetPadNum() == padNum)
			{
				moveF = true;	// �ړI�n�����������̂ňړ�OK

				VECTOR2 tmp	= obj->GetPos();
				endPos_d.x	= static_cast<double>(tmp.x);
				endPos_d.y	= static_cast<double>(tmp.y);
			}
		}

		// x��y���̈ړ��ʂ��v�Z����
		if (moveF)
		{
			this->endPos_d = endPos_d;

			// �ʂ��΂��p�x�����W�A���ŋ��߂�
			rad = atan2((static_cast<double>(endPos_d.y - pos_d.y)), (static_cast<double>(endPos_d.x - pos_d.x)));

			// x���Ay���̈ړ��ʂ��Z�o
			moveSpeed.x = 10.0f * cos(rad);
			moveSpeed.y = 10.0f * sin(rad);


			// �ړ�
			pos_d.x = pos_d.x + moveSpeed.x;
			pos_d.y = pos_d.y + moveSpeed.y;
		}
	}

	cnt++;
	animId += !(cnt % ORB_FLASH_INV);
	animId %= ANIM_MAX;
}

void ExpOrb::DeathPrc(weakListObj objList)
{
	for (auto &obj : *objList.lock())
	{
		if (obj->GetPadNum() == padNum)
		{
			obj->AddSpecialPoint(addPoint);
		}
	}
}

bool ExpOrb::CheckArrive(void)
{
	if (cnt > WAIT_VANISH_TIME)
	{
		return true;		// ��莞�Ԍo�߂����̂ŏ�����
	}

	if (cnt > WAIT_TIME)
	{
		POS_d r;			// ���ݍ��W����I�_���W�܂ł́Ax��y���̒���
		r.x = (pos_d.x - endPos_d.x);
		r.y = (pos_d.y - endPos_d.y);

		int hypotenuse;		// ���ݍ��W����I�_���W�܂ł́A�Εӂ̒����́u2��v
		hypotenuse = static_cast<int>((r.x * r.x) + (r.y * r.y));

		// �I�_���W���甼�aARRIVE_RANGE�̉~�������āA���̒��ɋʂ������delete
		if (hypotenuse < (ARRIVE_RANGE * ARRIVE_RANGE))
		{
			lpSoundMng.SetSound("�ʎ擾");
			return true;
		}
	}

	return false;
}

VECTOR2 ExpOrb::GetPos(void)
{
	return VECTOR2(static_cast<int>(pos_d.x), static_cast<int>(pos_d.y));
}

int ExpOrb::GetOrbColor(void)
{
	return orbColor;
}

void ExpOrb::Draw(void)
{
	DrawRectGraph(
		static_cast<int>(pos_d.x - ORB_SIZE / 2), 
		static_cast<int>(pos_d.y - ORB_SIZE / 2),
		ORB_SIZE * animId, 0,
		ORB_SIZE, ORB_SIZE,
		lpImageMng.GetID("image/UI/StarBlue.png")[0], true, false);
}
