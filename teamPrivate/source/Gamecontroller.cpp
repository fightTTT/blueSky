#include "Gamecontroller.h"

GameController::GameController()
{
	OldMouseInputFlag = 0;
	NowMouseInputFlag = 0;
	GetMousePoint(&MousePos.x, &MousePos.y);

	PadArray = {
		&HitPad0_NowArray , &HitPad0_OldArray,
		&HitPad1_NowArray , &HitPad1_OldArray,
		&HitPad2_NowArray , &HitPad2_OldArray,
		&HitPad3_NowArray , &HitPad3_OldArray,
	};
}


GameController::~GameController()
{
}

const HitKeyArray& GameController::GetNowInput() const
{
	return HitKeyNowArray;
}

const HitKeyArray& GameController::GetOldInput() const
{
	return HitKeyOldArray;
}

const int & GameController::GetNowMouseInputFlag() const
{
	return NowMouseInputFlag;
}

const int & GameController::GetOldMouseInputFlag() const
{
	return OldMouseInputFlag;
}

const VECTOR2 & GameController::GetMousePos() const
{
	return MousePos;
}

void GameController::Update()
{
	// Key�̓��͂��X�V����
	RefreshInput();
}

const bool GameController::GetNowInputKey(int InputKeyCode) const
{
	return HitKeyNowArray[InputKeyCode] == 1;
}

const bool GameController::GetOldInputKey(int InputKeyCode) const
{
	return HitKeyOldArray[InputKeyCode] == 1;
}

bool GameController::GetInputDown(int InputKeyCode) const
{
	// �O�t���[����0�ŁA���t���[����1�̏ꍇ�{�^�����������܂�Ă���Ɣ��f�ł���
	// �ǂ̃L�[�𔻕ʂ��邩�͈�����KeyCode��p����
	if (HitKeyNowArray[InputKeyCode] & ~HitKeyOldArray[InputKeyCode])
	{
		return true;
	}

	return false;
}

bool GameController::GetInputHold(int InputKeyCode) const
{
	// �O�t���[����1�ŁA���t���[����1�̏ꍇ�{�^�����������܂ꑱ���Ă���Ɣ��f�ł���
	// �ǂ̃L�[�𔻕ʂ��邩�͈�����KeyCode��p����
	if (HitKeyNowArray[InputKeyCode] & HitKeyOldArray[InputKeyCode])
	{
		return true;
	}
	return false;
}

bool GameController::GetInputUp(int InputKeyCode) const
{
	// �O�t���[����1�ŁA���t���[����0�̏ꍇ�{�^���������ꂽ�u�ԂƔ��f�ł���
	// �ǂ̃L�[�𔻕ʂ��邩�͈�����KeyCode��p����
	if ( ~HitKeyNowArray[InputKeyCode] & HitKeyOldArray[InputKeyCode])
	{
		return true;
	}
	return false;
}

bool GameController::GetMouseInputDown(int InputKeyCode) const
{
	// �O�t���[����0�ŁA���t���[����1�̏ꍇ�{�^�����������܂�Ă���Ɣ��f�ł���
	// �ǂ̃L�[�𔻕ʂ��邩�͈�����KeyCode��p����
	if ( (NowMouseInputFlag & InputKeyCode) & ~(OldMouseInputFlag & InputKeyCode) )
	{
		return true;
	}
	return false;
}

bool GameController::GetMouseInputHold(int InputKeyCode) const
{
	// �O�t���[����1�ŁA���t���[����1�̏ꍇ�{�^�����������܂ꑱ���Ă���Ɣ��f�ł���
	// �ǂ̃L�[�𔻕ʂ��邩�͈�����KeyCode��p����
	if ( (NowMouseInputFlag & InputKeyCode) & (OldMouseInputFlag & InputKeyCode) )
	{
		return true;
	}
	return false;
}

bool GameController::GetMouseInputUp(int InputKeyCode) const
{
	// �O�t���[����1�ŁA���t���[����0�̏ꍇ�{�^���������ꂽ�u�ԂƔ��f�ł���
	// �ǂ̃L�[�𔻕ʂ��邩�͈�����KeyCode��p����
	if ( ~(NowMouseInputFlag & InputKeyCode) & (OldMouseInputFlag & InputKeyCode) )
	{
		return true;
	}
	return false;
}

bool GameController::GetInputDown(int InputPadCode, int PadNum) const
{
	// �O�t���[����0�ŁA���t���[����1�̏ꍇ�{�^�����������܂�Ă���Ɣ��f�ł���
	// �ǂ̃L�[�𔻕ʂ��邩�͈�����PadCode��p����
	if (~(*PadArray[PadNum][1] & InputPadCode) & (*PadArray[PadNum][0] & InputPadCode) )
	{
		return true;
	}
	return false;
}

bool GameController::GetInputHold(int InputPadCode, int PadNum) const
{
	// �O�t���[����1�ŁA���t���[����1�̏ꍇ�{�^�����������܂ꑱ���Ă���Ɣ��f�ł���
	// �ǂ̃L�[�𔻕ʂ��邩�͈�����PadCode��p����
	if ( (*PadArray[PadNum][1] & InputPadCode) & (*PadArray[PadNum][0] & InputPadCode) )
	{
		return true;
	}
	return false;
}

bool GameController::GetInputUp(int InputPadCode, int PadNum) const
{
	// �O�t���[����1�ŁA���t���[����0�̏ꍇ�{�^���������ꂽ�u�ԂƔ��f�ł���
	// �ǂ̃L�[�𔻕ʂ��邩�͈�����PadCode��p����
	if ( (*PadArray[PadNum][1] & InputPadCode) & ~(*PadArray[PadNum][0] & InputPadCode))
	{
		return true;
	}
	return false;
}

void GameController::RefreshInput(void)
{
	// KeyArray�̍X�V
	HitKeyOldArray = HitKeyNowArray;
	GetHitKeyStateAll(&HitKeyNowArray[0]);
	GetMousePoint(&MousePos.x, &MousePos.y);
	// MouseInputFlag�̍X�V
	OldMouseInputFlag = NowMouseInputFlag;
	NowMouseInputFlag = GetMouseInput();
	// PadArray�̍X�V
	HitPad0_OldArray = HitPad0_NowArray;
	HitPad1_OldArray = HitPad1_NowArray;
	HitPad2_OldArray = HitPad2_NowArray;
	HitPad3_OldArray = HitPad3_NowArray;
	HitPad0_NowArray = GetJoypadInputState(DX_INPUT_PAD1);
	//HitPad1_NowArray = GetJoypadInputState(DX_INPUT_KEY);	// �J�����ł�PAD2�𷰓��͑Ή����Ă܂��B�R���g���[���[��1�����Ȃ������
	HitPad1_NowArray = GetJoypadInputState(DX_INPUT_PAD2);
	HitPad2_NowArray = GetJoypadInputState(DX_INPUT_PAD3);
	HitPad3_NowArray = GetJoypadInputState(DX_INPUT_PAD4);
}