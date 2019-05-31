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

	double startTime;	// FPS�v���J�n����
	int frameCount;		// �t���[�����̃J�E���g
	double average;		// ����FPS
};

