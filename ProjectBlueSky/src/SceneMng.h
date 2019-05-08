#pragma once
#include <memory>
#include "VECTOR2.h"
#include "BaseScene.h"

class GameCtrl;
class FrameMng;

#define lpSceneMng SceneMng::GetInstance()

class SceneMng
{
public:
	static SceneMng &GetInstance(void)			// ���̸׽���߲�����擾����A�����żݸ���݂̏�Ԃɂ��Ă���
	{
		static SceneMng s_Instance;
		return s_Instance;
	}
	void Run(void);
	VECTOR2 GetScreenSize(void);						// ��ذݻ��ނ��擾����

private:
	SceneMng();
	~SceneMng();

	std::shared_ptr<GameCtrl> gameCtl;		// GameController�׽���߲���i�[�p�ϐ�

	unique_Base activeScene;	// �e��݂ɱ������邽�߂��ư��߲���

	std::unique_ptr<FrameMng> frame;

	bool SysInit(void);			// ���ѓI�ȕϐ��̏��������s��
};

