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
	// 共通の初期化
	Init(imageSize, offset, frontImage, backImage, TurnFlag);
	waitCnt = wait;

	// めくるﾍﾟｰｼﾞの向き固有の初期化
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
	// 共通の初期化
	Init(imageSize, offset, frontImage, backImage, TurnFlag);
	waitCnt = waitTime * 60.0f;

	// めくるﾍﾟｰｼﾞの向き固有の初期化
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
				// 再生
				lpSoundMng.SetSound("ﾍﾟｰｼﾞめくり");
				seFlag = false;
			}
		}
	}
	// 半分めくったら、めくっているﾍﾟｰｼﾞのﾊﾝﾄﾞﾙを入れ替える
	slideImage = slideNum <= 6 ? frontBuffer : backBuffer;

	if (turnDir == TURN_DIR_LEFT)
	{
		// 後ろ(2枚目)の右半分の描画
		DrawRectGraph(offset.x + imageSize.x / 2, offset.y, imageSize.x / 2, 0, imageSize.x / 2, imageSize.y, backImage, FALSE, FALSE);
		// 前　(1枚目)の左半分の描画
		DrawRectGraph(offset.x, offset.y, 0, 0, imageSize.x / 2, imageSize.y, frontImage, FALSE, FALSE);
	}
	else
	{
		// 後ろ(2枚目)の左半分の描画
		DrawRectGraph(offset.x, offset.y, 0, 0, imageSize.x / 2, imageSize.y, backImage, FALSE, FALSE);
		// 前　(1枚目)の右半分の描画
		DrawRectGraph(offset.x + imageSize.x / 2, offset.y, imageSize.x / 2, 0, imageSize.x / 2, imageSize.y, frontImage, FALSE, FALSE);
	}


	SetDrawMode(DX_DRAWMODE_BILINEAR);

	// めくるﾍﾟｰｼﾞの描画
	DrawModiGraph(
		pos[slideNum][0] + offset,			// 左上
		pos[slideNum][1] + offset,			// 右上
		pos[slideNum][2] + offset,			// 右下
		pos[slideNum][3] + offset,			// 左下
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

	lpSoundMng.SoundInit("sound/SE類/PageSe.mp3", "ﾍﾟｰｼﾞめくり");
}

void TurnThePage::InitTurnLeft()
{
	// ﾌﾛﾝﾄﾊﾞｯﾌｧに前ﾍﾟｰｼﾞの右半分を描画
	frontBuffer = MakeScreen(imageSize.x / 2, imageSize.y, TRUE);	// 新しいﾊﾞｯﾌｧを作成
	SetDrawScreen(frontBuffer);
	DrawRectGraph(0, 0, imageSize.x / 2, 0, imageSize.x / 2, imageSize.y, frontImage, FALSE, FALSE);
	// ﾊﾞｯｸﾊﾞｯﾌｧに後ﾍﾟｰｼﾞの左半分を描画　
	backBuffer = MakeScreen(imageSize.x / 2, imageSize.y, TRUE);	// 新しいﾊﾞｯﾌｧを作成
	SetDrawScreen(backBuffer);
	DrawRectGraph(0, 0, 0, 0, imageSize.x / 2, imageSize.y, backImage, FALSE, FALSE);
	SetDrawScreen(lpSceneMng.GetTmpBuffer());						// tmpBufferに描画するように変更

	pos =
	{
		// 0枚目
		VECTOR2(imageSize.x / 2,0),										VECTOR2(imageSize.x,	  0),
		VECTOR2(imageSize.x ,imageSize.y),								VECTOR2(imageSize.x / 2,imageSize.y),
		// 1枚目
		VECTOR2(imageSize.x / 2,0),										VECTOR2(imageSize.x + 2 ,-2),
		VECTOR2(imageSize.x + 2,imageSize.y + 2),						VECTOR2(imageSize.x / 2,imageSize.y),
		// 2枚目
		VECTOR2(imageSize.x / 2,0),										VECTOR2(imageSize.x + 4 ,-4),
		VECTOR2(imageSize.x + 4,imageSize.y + 4),						VECTOR2(imageSize.x / 2,imageSize.y),
		// 3枚目
		VECTOR2(imageSize.x / 2,0),										VECTOR2(imageSize.x + 2 ,-6),
		VECTOR2(imageSize.x + 2,imageSize.y + 6),						VECTOR2(imageSize.x / 2,imageSize.y),
		// 4枚目
		VECTOR2(imageSize.x / 2,0),										VECTOR2(imageSize.x / 20 * 19,  -imageSize.y / 20),
		VECTOR2(imageSize.x / 20 * 19, imageSize.y + imageSize.y / 20),	VECTOR2(imageSize.x / 2,imageSize.y),
		// 5枚目
		VECTOR2(imageSize.x / 2,0),										VECTOR2(imageSize.x / 20 * 16,  -imageSize.y / 15),
		VECTOR2(imageSize.x / 20 * 16, imageSize.y + imageSize.y / 15),	VECTOR2(imageSize.x / 2,imageSize.y),
		// 6枚目
		VECTOR2(imageSize.x / 2,0),										VECTOR2(imageSize.x / 20 * 12,  -imageSize.y / 15),
		VECTOR2(imageSize.x / 20 * 12, imageSize.y + imageSize.y / 15),	VECTOR2(imageSize.x / 2,imageSize.y),

		// 7枚目
		VECTOR2(imageSize.x / 20 * 7,		-imageSize.y / 15),			VECTOR2(imageSize.x / 2, 0),
		VECTOR2(imageSize.x / 2,	imageSize.y),						VECTOR2(imageSize.x / 20 * 7,imageSize.y + imageSize.y / 15),
		// 8枚目
		VECTOR2(imageSize.x / 20 * 3,		-imageSize.y / 17),			VECTOR2(imageSize.x / 2, 0),
		VECTOR2(imageSize.x / 2,	imageSize.y),						VECTOR2(imageSize.x / 20 * 3,imageSize.y + imageSize.y / 17),
		// 9枚目
		VECTOR2(imageSize.x / 20 * 1,		-imageSize.y / 19),			VECTOR2(imageSize.x / 2, 0),
		VECTOR2(imageSize.x / 2,	imageSize.y),						VECTOR2(imageSize.x / 20 * 1,imageSize.y + imageSize.y / 19),
		// 10枚目
		VECTOR2(-2,	-2),												VECTOR2(imageSize.x / 2, 0),
		VECTOR2(imageSize.x / 2,	imageSize.y),						VECTOR2(-2,imageSize.y + 2),
		// 11枚目
		VECTOR2(0 ,	0),													VECTOR2(imageSize.x / 2, 0),
		VECTOR2(imageSize.x / 2,	imageSize.y),						VECTOR2(0, imageSize.y),
	};
}

void TurnThePage::InitTurnRight()
{
	// ﾌﾛﾝﾄﾊﾞｯﾌｧに前ﾍﾟｰｼﾞの左半分を描画
	frontBuffer = MakeScreen(imageSize.x / 2, imageSize.y, TRUE);	// 新しいﾊﾞｯﾌｧを作成
	SetDrawScreen(frontBuffer);
	DrawRectGraph(0, 0, 0, 0, imageSize.x / 2, imageSize.y, frontImage, FALSE, FALSE);
	// ﾊﾞｯｸﾊﾞｯﾌｧに後ﾍﾟｰｼﾞの右半分を描画　
	backBuffer = MakeScreen(imageSize.x / 2, imageSize.y, TRUE);	// 新しいﾊﾞｯﾌｧを作成
	SetDrawScreen(backBuffer);
	DrawRectGraph(0, 0, imageSize.x / 2, 0, imageSize.x / 2, imageSize.y, backImage, FALSE, FALSE);
	SetDrawScreen(lpSceneMng.GetTmpBuffer());						// tmpBufferに描画するように変更

	pos =
	{
		// 0枚目
		VECTOR2(0,0),										VECTOR2(imageSize.x / 2,0),
		VECTOR2(imageSize.x / 2,imageSize.y),				VECTOR2(0, imageSize.y),
		// 1枚目
		VECTOR2(-2,-2),										VECTOR2(imageSize.x / 2,0),
		VECTOR2(imageSize.x / 2,imageSize.y),				VECTOR2(-2,imageSize.y + 2),
		// 2枚目
		VECTOR2(-4,-4),										VECTOR2(imageSize.x / 2,0),
		VECTOR2(imageSize.x / 2,imageSize.y),				VECTOR2(-4, imageSize.y + 4),
		// 3枚目
		VECTOR2(-2,-6),										VECTOR2(imageSize.x / 2,0),
		VECTOR2(imageSize.x / 2,imageSize.y),				VECTOR2(-2, imageSize.y + 6),
		// 4枚目
		VECTOR2(imageSize.x / 20 * 2, -imageSize.y / 20),	VECTOR2(imageSize.x / 2,0),
		VECTOR2(imageSize.x / 2,imageSize.y),				VECTOR2(imageSize.x / 20 * 2, imageSize.y + imageSize.y / 20),
		// 5枚目
		VECTOR2(imageSize.x / 20 * 6, -imageSize.y / 15),	VECTOR2(imageSize.x / 2,0),
		VECTOR2(imageSize.x / 2,imageSize.y),				VECTOR2(imageSize.x / 20 * 6, imageSize.y + imageSize.y / 15),
		// 6枚目
		VECTOR2(imageSize.x / 20 * 9, -imageSize.y / 15),	VECTOR2(imageSize.x / 2,0),
		VECTOR2(imageSize.x / 2,imageSize.y),				VECTOR2(imageSize.x / 20 * 9, imageSize.y + imageSize.y / 15),

		// 7枚目
		VECTOR2(imageSize.x / 2,0),							VECTOR2(imageSize.x / 20 * 13, -imageSize.y / 15),
		VECTOR2(imageSize.x / 20 * 13, imageSize.y + imageSize.y / 15),	VECTOR2(imageSize.x / 2,imageSize.y),
		// 8枚目
		VECTOR2(imageSize.x / 2,0),							VECTOR2(imageSize.x / 20 * 17, -imageSize.y / 17),
		VECTOR2(imageSize.x / 20 * 17, imageSize.y + imageSize.y / 17),	VECTOR2(imageSize.x / 2,imageSize.y),
		// 9枚目
		VECTOR2(imageSize.x / 2,0),							VECTOR2(imageSize.x / 20 * 19, -imageSize.y / 19),
		VECTOR2(imageSize.x / 20 * 19, imageSize.y + imageSize.y / 19),	VECTOR2(imageSize.x / 2,imageSize.y),
		// 10枚目
		VECTOR2(imageSize.x / 2,0),							VECTOR2(imageSize.x + 2, -2),
		VECTOR2(imageSize.x,imageSize.y + 2),				VECTOR2(imageSize.x / 2,imageSize.y),
		// 11枚目
		VECTOR2(imageSize.x / 2,0),							VECTOR2(imageSize.x, 0),
		VECTOR2(imageSize.x,imageSize.y),					VECTOR2(imageSize.x / 2,imageSize.y),
	};
}

int DrawModiGraph(VECTOR2 leftUp, VECTOR2 rightUP, VECTOR2 rightDown, VECTOR2 leftDown, int GrHandle)
{
	DrawModiGraph(
		leftUp.x, leftUp.y,			//左上
		rightUP.x, rightUP.y,       //右上
		rightDown.x, rightDown.y,	//右下
		leftDown.x, leftDown.y,		//左下
		GrHandle,
		TRUE);
	return true;
}
