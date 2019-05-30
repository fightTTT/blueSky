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

	// 一時的な変数(あとでクラスにする予定)
	/*char filePath[MAX_PATH];
	std::string imagePath;
	bool inputFlag = false;
	int texHandle = -1;*/
	InputManager inputData;
	VECTOR2 boxStartPos(-1,-1);


	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		ClsDrawScreen();

		lpColMng.Update();
		lpColMng.Draw();
		
		//inputData.Update();

		//if (inputFlag)
		//{
		//	if (inputData.MouseStateDown(MOUSE_INPUT_LEFT))
		//	{
		//		boxStartPos = inputData.MousePos();
		//	}

		//	if (inputData.MouseStateUp(MOUSE_INPUT_LEFT))
		//	{
		//		boxStartPos = VECTOR2(-1,-1);
		//	}

		//	if (texHandle != -1)
		//	{
		//		DrawGraph(IMAGE_OFFSET_X, IMAGE_OFFSET_Y, texHandle, true);
		//	}

		//	if (!(boxStartPos.x == -1 && boxStartPos.y == -1))
		//	{
		//		auto mousePos = inputData.MousePos();
		//		DrawBox(boxStartPos.x, boxStartPos.y, mousePos.x, mousePos.y, 0xffffff, false);
		//	}
		//}
		//else
		//{
		//	if (GetDragFileNum() > 0)
		//	{
		//		GetDragFilePath(filePath);

		//		auto a = std::string(filePath)+"\\" +"0" + ".png";



		//		texHandle = LoadGraph(a.c_str());

		//		imagePath = filePath;

		//		auto findNum = imagePath.rfind("\\");

		//		imagePath = imagePath.substr(findNum - 1);

		//		//auto i = imagePath.find("\\");
		//		//std::string replace = "/";
		//		//imagePath.replace(i, 1, replace);.

		//		inputFlag = true;

		//		DrawString(100, 100, imagePath.c_str(), 0xffffff);
		//	}
		//	

		//	DrawString(200, IMAGE_OFFSET_Y, "画像ファイルをドラッグ&ドロップしてください", 0xffffff);
		//}

		ScreenFlip();
	}

	DxLib_End();

	return 0;
}