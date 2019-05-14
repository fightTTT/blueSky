#include "Dxlib.h"	// DxLibﾗｲﾌﾞﾗﾘを使用する
#include "GameCtrl.h"
#include "GameScene.h"
#include "SceneMng.h"
#include "FrameMng.h"
#include "SelectScene.h"

#define SCREEN_SIZE_X (1280)
#define SCREEN_SIZE_Y (720)

SceneMng::SceneMng()
{
	SysInit();
}

SceneMng::~SceneMng()
{
}


void SceneMng::Run(void)
{
	activeScene = std::make_unique<SelectScene>();

	// ---------- ｹﾞｰﾑﾙｰﾌﾟ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		frame->FrameStart();
		gameCtl->UpDate();

		ClsDrawScreen();

		activeScene = activeScene->UpDate(std::move(activeScene), *gameCtl);
		frame->FrameEnd();

		frame->Draw();

		ScreenFlip();
	}
}

VECTOR2 SceneMng::GetScreenSize(void)
{
	return VECTOR2(SCREEN_SIZE_X, SCREEN_SIZE_Y);
}

const int SceneMng::GetCharID(PAD_ID padID)
{
	return charID[static_cast<int>(padID)];
}

void SceneMng::SetCharID(PAD_ID padID,int id)
{
	charID[static_cast<int>(padID)] = id;
}

bool SceneMng::GetDecidFlag(PAD_ID padID)
{
	return decidFlag[static_cast<int>(padID)];
}

void SceneMng::SetDecidFlag(PAD_ID padID, bool decidFlag)
{
	SceneMng::decidFlag[static_cast<int>(padID)] = decidFlag;
}

int SceneMng::GetModeFlag(void)
{
	return modeFlag;
}

bool SceneMng::SysInit(void)
{
	// ｼｽﾃﾑ処理
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 16);		// 65536色ﾓｰﾄﾞに設定
	ChangeWindowMode(true);				// true:window　false:ﾌﾙｽｸﾘｰﾝ
	SetWindowText("BLUE SKY");
	if (DxLib_Init() == -1) return false;	// DXﾗｲﾌﾞﾗﾘ初期化処理
	SetDrawScreen(DX_SCREEN_BACK);		// ひとまずﾊﾞｯｸﾊﾞｯﾌｧに描画
	gameCtl = std::make_unique<GameCtrl>();
	frame = std::make_unique<FrameMng>();

	for (int cnt = 0; cnt < PLAYER_CNT_MAX; cnt++)
	{
		charID[cnt] = -1;
	}

	modeFlag = 0;

	return true;
}
