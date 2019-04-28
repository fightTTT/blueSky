#pragma once
#include "DxLib.h"
#include "VECTOR2.h"
#include <array>

#define GET_KEY_INTERVAL	  30		// InputIntervalHold���Ŏg����ATrue��Ԃ��Ԋu�̏����l
#define GET_KEY_INTERVAL_MIN  5			// InputIntervalHold���Ŏg����ATrue��Ԃ��Ԋu�̍ŏ��l



typedef std::array<char, 256> HitKeyArray;

class GameController
{
public:
	GameController();
	~GameController();
	const HitKeyArray& GetNowInput()const;		// ���t���[����HitKeyArray�̃Q�b�^�[
	const HitKeyArray& GetOldInput()const;		// 1�t���[���O��HitKeyArray�̃Q�b�^�[

	const int& GetNowMouseInputFlag()const;		// ���t���[����MouseInputFlag�̃Q�b�^�[
	const int& GetOldMouseInputFlag()const;		// 1�t���[���O��MouseInputFlag�̃Q�b�^�[
	const VECTOR2& GetMousePos()const;			// �J�[�\�����W�̃Q�b�^�[

	void Update();

	// KeyCode���w�肷��΂���KeyCode�̃t���O��Ԃ��Ă����
	const bool GetNowInputKey(int InputKeyCode)const;		// ���t���[����InputKeyCode�ɓn���ꂽKeyCode�̃t���O��Ԃ�
	const bool GetOldInputKey(int InputKeyCode)const;		// �O�t���[����InputKeyCode�ɓn���ꂽKeyCode�̃t���O��Ԃ�

	//----------------------------------------------------------------------------------//
	// ������DxLib�Őݒ肳�ꂽKeyCode�𑗂邾���Ō��True��False���A���Ă���			//
	// True���A���Ă��Ă���Ƃ��������ɍ����Ă���Ƃ��ŁAFalse�̎��͏����ɍ����Ă��Ȃ���//
	//----------------------------------------------------------------------------------//

	bool GetInputDown			(int InputKeyCode)const;	// 1�t���[������True��Ԃ��A���������Ă�True��Ԃ��Ȃ�
	bool GetInputHold			(int InputKeyCode)const;	// �O�t���[������p�����ĉ�����Ă��鎞��True��Ԃ�
	bool GetInputUp				(int InputKeyCode)const;	// �{�^���������ꂽ��Ԃ��痣���ꂽ�Ƃ��ɂP�t���[������True��Ԃ�

	bool GetMouseInputDown		(int InputKeyCode)const;	// 1�t���[������True��Ԃ��A���������Ă�True��Ԃ��Ȃ�
	bool GetMouseInputHold		(int InputKeyCode)const;	// �O�t���[������p�����ĉ�����Ă��鎞��True��Ԃ�
	bool GetMouseInputUp		(int InputKeyCode)const;	// �{�^���������ꂽ��Ԃ��痣���ꂽ�Ƃ��ɂP�t���[������True��Ԃ�

	bool GetInputDown		(int InputPadCode,int PadNum)const;	// 1�t���[������True��Ԃ��A���������Ă�True��Ԃ��Ȃ�
	bool GetInputHold		(int InputPadCode,int PadNum)const;	// �O�t���[������p�����ĉ�����Ă��鎞��True��Ԃ�
	bool GetInputUp			(int InputPadCode,int PadNum)const;	// �{�^���������ꂽ��Ԃ��痣���ꂽ�Ƃ��ɂP�t���[������True��Ԃ�

protected:
	HitKeyArray HitKeyNowArray;								// ���t���[���̓��͂���Ă���L�[��񂪊i�[����Ă���
	HitKeyArray HitKeyOldArray;								// 1�t���[���O�ɓ��͂���Ă���L�[��񂪊i�[����Ă���

	int NowMouseInputFlag;									// ���t���[���̃}�E�X�̃L�[���͂��i�[�����
	int OldMouseInputFlag;									// 1�t���[���̃}�E�X�̃L�[���͂��i�[�����

	VECTOR2 MousePos;										//�}�E�X�̃|�W�V����

	int HitPad0_NowArray;									// ���t���[���̓��͂���Ă���p�b�h�O��񂪊i�[����Ă���
	int HitPad0_OldArray;									// 1�t���[���O�ɓ��͂���Ă���p�b�h�O��񂪊i�[����Ă���
	int HitPad1_NowArray;									// ���t���[���̓��͂���Ă���p�b�h�P��񂪊i�[����Ă���
	int HitPad1_OldArray;									// 1�t���[���O�ɓ��͂���Ă���p�b�h�P��񂪊i�[����Ă���
	int HitPad2_NowArray;									// ���t���[���̓��͂���Ă���p�b�h�Q��񂪊i�[����Ă���
	int HitPad2_OldArray;									// 1�t���[���O�ɓ��͂���Ă���p�b�h�Q��񂪊i�[����Ă���
	int HitPad3_NowArray;									// ���t���[���̓��͂���Ă���p�b�h�R��񂪊i�[����Ă���
	int HitPad3_OldArray;									// 1�t���[���O�ɓ��͂���Ă���p�b�h�R��񂪊i�[����Ă���

	std::array<int*[2], 4> PadArray;							// GetInput�֐��̑�2��������APad��񂪊i�[����Ă���z��փA�N�Z�X����
private:
	void RefreshInput(void);								// HitKeyArray�̃��t���b�V��
};

