#pragma once
#include <memory>
#include <array>
#include "FontMng.h"
#include "ImageMng.h"

#define GAME_SCREEN_X (40)	// �ްѴر�̾��
#define GAME_SCREEN_Y (75)
#define SCREEN_SIZE_X			(1280)	// ��ʉ𑜓xX
#define SCREEN_SIZE_Y			(720)	// ��ʉ𑜓xY
#define GAME_SCREEN_SIZE_X		(640)	// �ްщ��
#define GAME_SCREEN_SIZE_Y		(480)

#define CHIP_SIZE (20)					// 1Ͻ�̻���

#define FLASING_LOW_CNT			(50)	// �������_�ŗp
#define FLASING_HIGH_CNT		(6)		// �͂������_�ŗp

class GameController;
class BaseScene;
using unique_Base = std::unique_ptr<BaseScene>;

class BaseScene
{
public:
	virtual ~BaseScene();
	virtual unique_Base Update(unique_Base own, const GameController &controller) = 0;
private:
	virtual int Init(void) = 0;
};

