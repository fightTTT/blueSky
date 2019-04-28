#pragma once
#include <mutex>
#include <list>
#include <memory>
#include "Obj.h"
#include "VECTOR2.h"
#include "GameController.h"
#include "BaseScene.h"

#define lpSceneMng SceneMng::GetInstance()

// PS4�p����̨��		
#define GPAD_INPUT_X		PAD_INPUT_1		// PS4�� = ��	�� = Z
#define GPAD_INPUT_A		PAD_INPUT_2		// PS4�� = �~	�� = X
#define GPAD_INPUT_B		PAD_INPUT_3		// PS4�� = ��	�� = C
#define GPAD_INPUT_Y		PAD_INPUT_4		// PS4�� = ��	�� = A
#define GPAD_INPUT_L1		PAD_INPUT_5		// PS4�� = L1	�� = S
#define GPAD_INPUT_R1		PAD_INPUT_6		// PS4�� = R1	�� = D

// xbox�p����̨��		
//#define GPAD_INPUT_X		PAD_INPUT_3		// xbox�� = X
//#define GPAD_INPUT_A		PAD_INPUT_1		// xbox�� = A
//#define GPAD_INPUT_B		PAD_INPUT_2		// xbox�� = B
//#define GPAD_INPUT_Y		PAD_INPUT_4		// xbox�� = Y
//#define GPAD_INPUT_L1		PAD_INPUT_5		// xbox�� = LB
//#define GPAD_INPUT_R1		PAD_INPUT_6		// xbox�� = RB

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
		static SceneMng s_Instance;		// �ݽ�ݽ static�Ȃ̂�1�x��������Ȃ�(�������m�ۂ��Ȃ�)
		return (s_Instance);
	}
	void Run(void);
	VECTOR2 GetScreenSize(void);
	VECTOR2 GetGameScreenSize(void);
	bool SetDrawOffset(VECTOR2 vec);
	const VECTOR2 &GetDrawOffset(void);
	int GetChipSize(void);

	int GetTmpBuffer(void);	// �ޯ̧������ق��擾

private:
	SceneMng();
	~SceneMng();

	VECTOR2 drawOffset;
	std::unique_ptr<GameController> gameCtl;
	unique_Base activeScene;

	bool SysInit(void);

	int tmpBuffer;		// ��ʌ��ʗp�o�b�t�@�@���ʂ����Ȃ��Ȃ�ʏ�`�悷��
};

