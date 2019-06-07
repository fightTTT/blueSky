#include <DxLib.h>
#include <string>
#include "InputManager.h"
#include "CollisionMng.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define IMAGE_OFFSET_X (WINDOW_WIDTH / 2 - 50)
#define IMAGE_OFFSET_Y (WINDOW_HEIGHT / 2 - 50)

int main(int argc, char* argv[])
{
	SetGraphMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32);
	ChangeWindowMode(true);
	if (DxLib_Init() == -1)return false;
	SetDrawScreen(DX_SCREEN_BACK);
	SetDragFileValidFlag(TRUE);

	InputManager inputData;
	VECTOR2 boxStartPos(-1,-1);

	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		ClsDrawScreen();

		lpColMng.Update();
		lpColMng.Draw();

		ScreenFlip();
	}

	DxLib_End();

	return 0;
}