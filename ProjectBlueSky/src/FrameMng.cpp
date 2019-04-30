#include "FrameMng.h"
#include "DxLib.h"

#define FPS 60.0

FrameMng::~FrameMng()
{
}

void FrameMng::FrameStart()
{
	if (frameCount == 1)
	{
		startTime = static_cast<double>(GetNowCount());
	}

	if (frameCount == FPS)
	{
		double time = static_cast<float>(GetNowCount());
		average = 1000.0 / ((time - startTime) / FPS);
		frameCount = 0;
		startTime = time;
	}

	frameCount++;
}

void FrameMng::FrameEnd()
{
	double tookTime = static_cast<double>(GetNowCount()) - startTime;
	double waitTime = static_cast<double>(frameCount) * 1000.0 / FPS - tookTime;
	if (waitTime > 0.0)
	{
		Sleep(static_cast<DWORD>(waitTime));
	}
}

void FrameMng::Draw()
{
	DrawFormatString(0, 0, 0xffffff, "FPS : %.1lf", average);
}
