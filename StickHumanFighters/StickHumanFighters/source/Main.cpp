#include "Dxlib.h"	// DxLibײ���؂��g�p����
#include "SceneMng.h"

// ---------- WinMain�֐�
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	lpSceneMng.Run();
	DxLib_End();	// DXײ���؂̏I������
	return 0;
}