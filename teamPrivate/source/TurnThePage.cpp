#include "DxLib.h"
#include "TurnThePage.h"
#include "SceneMng.h"
#include "GameController.h"
#include "ImageMng.h"
#include "SoundMng.h"

#define slideImageMax 11
#define wait 180.0

TurnThePage::TurnThePage()
{
}

TurnThePage::TurnThePage(VECTOR2 imageSize, VECTOR2 offset, int frontImage, int backImage, bool TurnFlag)
{
	// ���ʂ̏�����
	Init(imageSize, offset, frontImage, backImage, TurnFlag);
	waitCnt = wait;

	// �߂����߰�ނ̌����ŗL�̏�����
	if (turnDir == TURN_DIR_LEFT)
	{
		InitTurnLeft();
	}
	else
	{
		InitTurnRight();
	}
}

TurnThePage::TurnThePage(VECTOR2 imageSize, VECTOR2 offset, int frontImage, int backImage, float waitTime, bool TurnFlag)
{
	// ���ʂ̏�����
	Init(imageSize, offset, frontImage, backImage, TurnFlag);
	waitCnt = waitTime * 60.0f;

	// �߂����߰�ނ̌����ŗL�̏�����
	if (turnDir == TURN_DIR_LEFT)
	{
		InitTurnLeft();
	}
	else
	{
		InitTurnRight();
	}
}


TurnThePage::~TurnThePage()
{
}

bool TurnThePage::Draw(void)
{
	cnt++;
	if (cnt >= waitCnt)
	{
		if (cnt % 5 == 0)
		{
			slideNum += (slideNum < slideImageMax);

			if (seFlag)
			{
				// �Đ�
				lpSoundMng.SetSound("�߰�ނ߂���");
				seFlag = false;
			}
		}
	}
	// �����߂�������A�߂����Ă����߰�ނ�����ق����ւ���
	slideImage = slideNum <= 6 ? frontBuffer : backBuffer;

	if (turnDir == TURN_DIR_LEFT)
	{
		// ���(2����)�̉E�����̕`��
		DrawRectGraph(offset.x + imageSize.x / 2, offset.y, imageSize.x / 2, 0, imageSize.x / 2, imageSize.y, backImage, FALSE, FALSE);
		// �O�@(1����)�̍������̕`��
		DrawRectGraph(offset.x, offset.y, 0, 0, imageSize.x / 2, imageSize.y, frontImage, FALSE, FALSE);
	}
	else
	{
		// ���(2����)�̍������̕`��
		DrawRectGraph(offset.x, offset.y, 0, 0, imageSize.x / 2, imageSize.y, backImage, FALSE, FALSE);
		// �O�@(1����)�̉E�����̕`��
		DrawRectGraph(offset.x + imageSize.x / 2, offset.y, imageSize.x / 2, 0, imageSize.x / 2, imageSize.y, frontImage, FALSE, FALSE);
	}


	SetDrawMode(DX_DRAWMODE_BILINEAR);

	// �߂����߰�ނ̕`��
	DrawModiGraph(
		pos[slideNum][0] + offset,			// ����
		pos[slideNum][1] + offset,			// �E��
		pos[slideNum][2] + offset,			// �E��
		pos[slideNum][3] + offset,			// ����
		slideImage);

	SetDrawMode(DX_DRAWMODE_NEAREST);

	return (slideNum == slideImageMax ? true : false);
}

void TurnThePage::Init(VECTOR2 imageSize, VECTOR2 offset, int frontImage, int backImage, bool TurnFlag)
{
	this->frontImage = frontImage;
	this->backImage = backImage;
	this->imageSize = imageSize;
	this->offset = offset;
	turnDir = (TURN_DIR)TurnFlag;

	slideNum = 0;
	cnt = 0;
	seFlag = true;

	lpSoundMng.SoundInit("sound/SE��/PageSe.mp3", "�߰�ނ߂���");
}

void TurnThePage::InitTurnLeft()
{
	// �����ޯ̧�ɑO�߰�ނ̉E������`��
	frontBuffer = MakeScreen(imageSize.x / 2, imageSize.y, TRUE);	// �V�����ޯ̧���쐬
	SetDrawScreen(frontBuffer);
	DrawRectGraph(0, 0, imageSize.x / 2, 0, imageSize.x / 2, imageSize.y, frontImage, FALSE, FALSE);
	// �ޯ��ޯ̧�Ɍ��߰�ނ̍�������`��@
	backBuffer = MakeScreen(imageSize.x / 2, imageSize.y, TRUE);	// �V�����ޯ̧���쐬
	SetDrawScreen(backBuffer);
	DrawRectGraph(0, 0, 0, 0, imageSize.x / 2, imageSize.y, backImage, FALSE, FALSE);
	SetDrawScreen(lpSceneMng.GetTmpBuffer());						// tmpBuffer�ɕ`�悷��悤�ɕύX

	pos =
	{
		// 0����
		VECTOR2(imageSize.x / 2,0),										VECTOR2(imageSize.x,	  0),
		VECTOR2(imageSize.x ,imageSize.y),								VECTOR2(imageSize.x / 2,imageSize.y),
		// 1����
		VECTOR2(imageSize.x / 2,0),										VECTOR2(imageSize.x + 2 ,-2),
		VECTOR2(imageSize.x + 2,imageSize.y + 2),						VECTOR2(imageSize.x / 2,imageSize.y),
		// 2����
		VECTOR2(imageSize.x / 2,0),										VECTOR2(imageSize.x + 4 ,-4),
		VECTOR2(imageSize.x + 4,imageSize.y + 4),						VECTOR2(imageSize.x / 2,imageSize.y),
		// 3����
		VECTOR2(imageSize.x / 2,0),										VECTOR2(imageSize.x + 2 ,-6),
		VECTOR2(imageSize.x + 2,imageSize.y + 6),						VECTOR2(imageSize.x / 2,imageSize.y),
		// 4����
		VECTOR2(imageSize.x / 2,0),										VECTOR2(imageSize.x / 20 * 19,  -imageSize.y / 20),
		VECTOR2(imageSize.x / 20 * 19, imageSize.y + imageSize.y / 20),	VECTOR2(imageSize.x / 2,imageSize.y),
		// 5����
		VECTOR2(imageSize.x / 2,0),										VECTOR2(imageSize.x / 20 * 16,  -imageSize.y / 15),
		VECTOR2(imageSize.x / 20 * 16, imageSize.y + imageSize.y / 15),	VECTOR2(imageSize.x / 2,imageSize.y),
		// 6����
		VECTOR2(imageSize.x / 2,0),										VECTOR2(imageSize.x / 20 * 12,  -imageSize.y / 15),
		VECTOR2(imageSize.x / 20 * 12, imageSize.y + imageSize.y / 15),	VECTOR2(imageSize.x / 2,imageSize.y),

		// 7����
		VECTOR2(imageSize.x / 20 * 7,		-imageSize.y / 15),			VECTOR2(imageSize.x / 2, 0),
		VECTOR2(imageSize.x / 2,	imageSize.y),						VECTOR2(imageSize.x / 20 * 7,imageSize.y + imageSize.y / 15),
		// 8����
		VECTOR2(imageSize.x / 20 * 3,		-imageSize.y / 17),			VECTOR2(imageSize.x / 2, 0),
		VECTOR2(imageSize.x / 2,	imageSize.y),						VECTOR2(imageSize.x / 20 * 3,imageSize.y + imageSize.y / 17),
		// 9����
		VECTOR2(imageSize.x / 20 * 1,		-imageSize.y / 19),			VECTOR2(imageSize.x / 2, 0),
		VECTOR2(imageSize.x / 2,	imageSize.y),						VECTOR2(imageSize.x / 20 * 1,imageSize.y + imageSize.y / 19),
		// 10����
		VECTOR2(-2,	-2),												VECTOR2(imageSize.x / 2, 0),
		VECTOR2(imageSize.x / 2,	imageSize.y),						VECTOR2(-2,imageSize.y + 2),
		// 11����
		VECTOR2(0 ,	0),													VECTOR2(imageSize.x / 2, 0),
		VECTOR2(imageSize.x / 2,	imageSize.y),						VECTOR2(0, imageSize.y),
	};
}

void TurnThePage::InitTurnRight()
{
	// �����ޯ̧�ɑO�߰�ނ̍�������`��
	frontBuffer = MakeScreen(imageSize.x / 2, imageSize.y, TRUE);	// �V�����ޯ̧���쐬
	SetDrawScreen(frontBuffer);
	DrawRectGraph(0, 0, 0, 0, imageSize.x / 2, imageSize.y, frontImage, FALSE, FALSE);
	// �ޯ��ޯ̧�Ɍ��߰�ނ̉E������`��@
	backBuffer = MakeScreen(imageSize.x / 2, imageSize.y, TRUE);	// �V�����ޯ̧���쐬
	SetDrawScreen(backBuffer);
	DrawRectGraph(0, 0, imageSize.x / 2, 0, imageSize.x / 2, imageSize.y, backImage, FALSE, FALSE);
	SetDrawScreen(lpSceneMng.GetTmpBuffer());						// tmpBuffer�ɕ`�悷��悤�ɕύX

	pos =
	{
		// 0����
		VECTOR2(0,0),										VECTOR2(imageSize.x / 2,0),
		VECTOR2(imageSize.x / 2,imageSize.y),				VECTOR2(0, imageSize.y),
		// 1����
		VECTOR2(-2,-2),										VECTOR2(imageSize.x / 2,0),
		VECTOR2(imageSize.x / 2,imageSize.y),				VECTOR2(-2,imageSize.y + 2),
		// 2����
		VECTOR2(-4,-4),										VECTOR2(imageSize.x / 2,0),
		VECTOR2(imageSize.x / 2,imageSize.y),				VECTOR2(-4, imageSize.y + 4),
		// 3����
		VECTOR2(-2,-6),										VECTOR2(imageSize.x / 2,0),
		VECTOR2(imageSize.x / 2,imageSize.y),				VECTOR2(-2, imageSize.y + 6),
		// 4����
		VECTOR2(imageSize.x / 20 * 2, -imageSize.y / 20),	VECTOR2(imageSize.x / 2,0),
		VECTOR2(imageSize.x / 2,imageSize.y),				VECTOR2(imageSize.x / 20 * 2, imageSize.y + imageSize.y / 20),
		// 5����
		VECTOR2(imageSize.x / 20 * 6, -imageSize.y / 15),	VECTOR2(imageSize.x / 2,0),
		VECTOR2(imageSize.x / 2,imageSize.y),				VECTOR2(imageSize.x / 20 * 6, imageSize.y + imageSize.y / 15),
		// 6����
		VECTOR2(imageSize.x / 20 * 9, -imageSize.y / 15),	VECTOR2(imageSize.x / 2,0),
		VECTOR2(imageSize.x / 2,imageSize.y),				VECTOR2(imageSize.x / 20 * 9, imageSize.y + imageSize.y / 15),

		// 7����
		VECTOR2(imageSize.x / 2,0),							VECTOR2(imageSize.x / 20 * 13, -imageSize.y / 15),
		VECTOR2(imageSize.x / 20 * 13, imageSize.y + imageSize.y / 15),	VECTOR2(imageSize.x / 2,imageSize.y),
		// 8����
		VECTOR2(imageSize.x / 2,0),							VECTOR2(imageSize.x / 20 * 17, -imageSize.y / 17),
		VECTOR2(imageSize.x / 20 * 17, imageSize.y + imageSize.y / 17),	VECTOR2(imageSize.x / 2,imageSize.y),
		// 9����
		VECTOR2(imageSize.x / 2,0),							VECTOR2(imageSize.x / 20 * 19, -imageSize.y / 19),
		VECTOR2(imageSize.x / 20 * 19, imageSize.y + imageSize.y / 19),	VECTOR2(imageSize.x / 2,imageSize.y),
		// 10����
		VECTOR2(imageSize.x / 2,0),							VECTOR2(imageSize.x + 2, -2),
		VECTOR2(imageSize.x,imageSize.y + 2),				VECTOR2(imageSize.x / 2,imageSize.y),
		// 11����
		VECTOR2(imageSize.x / 2,0),							VECTOR2(imageSize.x, 0),
		VECTOR2(imageSize.x,imageSize.y),					VECTOR2(imageSize.x / 2,imageSize.y),
	};
}

int DrawModiGraph(VECTOR2 leftUp, VECTOR2 rightUP, VECTOR2 rightDown, VECTOR2 leftDown, int GrHandle)
{
	DrawModiGraph(
		leftUp.x, leftUp.y,			//����
		rightUP.x, rightUP.y,       //�E��
		rightDown.x, rightDown.y,	//�E��
		leftDown.x, leftDown.y,		//����
		GrHandle,
		TRUE);
	return true;
}
