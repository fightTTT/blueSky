#pragma once
#include <mutex>
#include <list>
#include <memory>
#include "Obj.h"
#include "VECTOR2.h"
#include "GameController.h"
#include "BaseScene.h"

#define lpSceneMng SceneMng::GetInstance()

// PS4用ｷｰｺﾝﾌｨｸﾞ		
#define GPAD_INPUT_X		PAD_INPUT_1		// PS4ｺﾝ = □	ｷｰ = Z
#define GPAD_INPUT_A		PAD_INPUT_2		// PS4ｺﾝ = ×	ｷｰ = X
#define GPAD_INPUT_B		PAD_INPUT_3		// PS4ｺﾝ = ○	ｷｰ = C
#define GPAD_INPUT_Y		PAD_INPUT_4		// PS4ｺﾝ = △	ｷｰ = A
#define GPAD_INPUT_L1		PAD_INPUT_5		// PS4ｺﾝ = L1	ｷｰ = S
#define GPAD_INPUT_R1		PAD_INPUT_6		// PS4ｺﾝ = R1	ｷｰ = D

// xbox用ｷｰｺﾝﾌｨｸﾞ		
//#define GPAD_INPUT_X		PAD_INPUT_3		// xboxｺﾝ = X
//#define GPAD_INPUT_A		PAD_INPUT_1		// xboxｺﾝ = A
//#define GPAD_INPUT_B		PAD_INPUT_2		// xboxｺﾝ = B
//#define GPAD_INPUT_Y		PAD_INPUT_4		// xboxｺﾝ = Y
//#define GPAD_INPUT_L1		PAD_INPUT_5		// xboxｺﾝ = LB
//#define GPAD_INPUT_R1		PAD_INPUT_6		// xboxｺﾝ = RB

class Obj;
class GameController;

int	DrawLine(VECTOR2 vec1,	VECTOR2 vec2, unsigned int Color, int Thickness = 1);
int	DrawBox	(VECTOR2 vec1,	VECTOR2 vec2, unsigned int Color, int	FillFlag = 1);
int	DrawGraph(VECTOR2 vec,	int GrHandle, int TransFlag = 1);
int	DrawCircle(VECTOR2 vec, int   r, unsigned int Color, int FillFlag = TRUE, int   LineThickness = 1);
int DrawRotaGraph(VECTOR2 vec, double ExRate, double Angle, int GrHandle, int TransFlag, int TurnFlag = FALSE);
int	DrawRotaGraph3(VECTOR2 vec, VECTOR2 rate, double ExtRateX, double ExtRateY, double Angle, int GrHandle, int TransFlag, int TurnFlag = FALSE);

class SceneMng
{
public:
	static SceneMng &GetInstance(void)
	{
		static SceneMng s_Instance;		// ｲﾝｽﾀﾝｽ staticなので1度しか作られない(メモリ確保しない)
		return (s_Instance);
	}
	void Run(void);
	VECTOR2 GetScreenSize(void);
	VECTOR2 GetGameScreenSize(void);
	bool SetDrawOffset(VECTOR2 vec);
	const VECTOR2 &GetDrawOffset(void);
	int GetChipSize(void);

	int GetTmpBuffer(void);	// ﾊﾞｯﾌｧのﾊﾝﾄﾞﾙを取得

private:
	SceneMng();
	~SceneMng();

	VECTOR2 drawOffset;
	std::unique_ptr<GameController> gameCtl;
	unique_Base activeScene;

	bool SysInit(void);

	int tmpBuffer;		// 画面効果用バッファ　効果をつけないなら通常描画する
};

