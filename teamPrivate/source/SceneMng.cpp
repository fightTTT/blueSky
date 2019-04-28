#include "Dxlib.h"
#include "SceneMng.h"
#include "CollisionEditScene.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "VECTOR2.h"
#include "ImageMng.h"
#include "GameController.h"
#include "Fps.h"

#define SCREEN_SIZE_X (1280)	// 画面解像度
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
	//activeScene = std::make_unique<CollisionEditScene>();
	activeScene = std::make_unique</*TitleScene*/CollisionEditScene>();
	Fps fps;
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		fps.Update();		// FPSの更新
		gameCtl->Update();
		activeScene = activeScene->Update(std::move(activeScene),*gameCtl);

		fps.Wait();			// 待機
	}
}

VECTOR2 SceneMng::GetScreenSize(void)
{
	return VECTOR2(SCREEN_SIZE_X, SCREEN_SIZE_Y);
}

VECTOR2 SceneMng::GetGameScreenSize(void)
{
	return VECTOR2(GAME_SCREEN_SIZE_X, GAME_SCREEN_SIZE_Y);
}

bool SceneMng::SetDrawOffset(VECTOR2 vec)
{
	drawOffset = vec;
	return true;
}

const VECTOR2 & SceneMng::GetDrawOffset(void)
{
	return drawOffset;
}

int SceneMng::GetChipSize(void)
{
	return CHIP_SIZE;
}

int SceneMng::GetTmpBuffer(void)
{
	return tmpBuffer;
}

bool SceneMng::SysInit(void)
{
	// ｼｽﾃﾑ処理
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 16);		// 65536色ﾓｰﾄﾞに設定
	ChangeWindowMode(true);					// true:window　false:ﾌﾙｽｸﾘｰﾝ
	SetWindowText("private");
	if (DxLib_Init() == -1) return false;	// DXﾗｲﾌﾞﾗﾘ初期化処理
	tmpBuffer = MakeScreen(SCREEN_SIZE_X, SCREEN_SIZE_Y, TRUE);	// 新しいﾊﾞｯﾌｧを作成
	SetDrawScreen(tmpBuffer);
	gameCtl = std::make_unique<GameController>();
	srand(static_cast<unsigned>(time(NULL)));
	return true;
}

int DrawLine(VECTOR2 vec1, VECTOR2 vec2, unsigned int Color, int Thickness)
{
	DxLib::DrawLine(vec1.x, vec1.y, vec2.x, vec2.y, Color, Thickness);
	return 0;
}

int	DrawBox(VECTOR2 vec1, VECTOR2 vec2,unsigned int Color, int FillFlag)
{
	DxLib::DrawBox(vec1.x, vec1.y, vec2.x, vec2.y, Color, FillFlag);
	return 0;
}

int DrawGraph(VECTOR2 vec, int GrHandle, int TransFlag)
{
	DxLib::DrawGraph(vec.x,vec.y, GrHandle, TransFlag);
	return 0;
}

int DrawCircle(VECTOR2 vec, int r, unsigned int Color, int FillFlag, int LineThickness)
{
	DxLib::DrawCircle(vec.x,vec.y,r,Color,FillFlag,LineThickness);
	return 0;
}

int DrawRotaGraph(VECTOR2 vec, double ExRate, double Angle, int GrHandle, int TransFlag, int TurnFlag)
{
	DxLib::DrawRotaGraph(vec.x,vec.y,ExRate,Angle,GrHandle,TransFlag,TurnFlag);
	return 0;
}


int DrawRotaGraph3(VECTOR2 vec, VECTOR2 rate , double ExtRateX, double ExtRateY, double Angle, int GrHandle, int TransFlag, int TurnFlag)
{
	DxLib::DrawRotaGraph3(vec.x,vec.y, rate.x, rate.y, ExtRateX,  ExtRateY,  Angle,  GrHandle,  TransFlag,  TurnFlag);
	return 0;
}

