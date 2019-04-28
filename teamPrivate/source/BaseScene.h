#pragma once
#include <memory>
#include <array>
#include "FontMng.h"
#include "ImageMng.h"

#define GAME_SCREEN_X (40)	// ｹﾞｰﾑｴﾘｱｵﾌｾｯﾄ
#define GAME_SCREEN_Y (75)
#define SCREEN_SIZE_X			(1280)	// 画面解像度X
#define SCREEN_SIZE_Y			(720)	// 画面解像度Y
#define GAME_SCREEN_SIZE_X		(640)	// ｹﾞｰﾑ画面
#define GAME_SCREEN_SIZE_Y		(480)

#define CHIP_SIZE (20)					// 1ﾏｽのｻｲｽﾞ

#define FLASING_LOW_CNT			(50)	// ゆっくり点滅用
#define FLASING_HIGH_CNT		(6)		// はげしく点滅用

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

