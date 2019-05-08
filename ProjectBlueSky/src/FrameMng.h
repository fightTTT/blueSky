#pragma once


class FrameMng
{
public:
	FrameMng() : startTime(0), frameCount(0) {}
	~FrameMng();

	void FrameStart();
	void FrameEnd();

	void Draw();

private:

	double startTime;	// FPS計測開始時間
	int frameCount;		// フレーム数のカウント
	double average;		// 平均FPS
};

