#pragma once
#include <array>

enum BUTTON_TYPE
{
	// ���X�e�B�b�N
	THUMB_L_UP,
	THUMB_L_DOWN,
	THUMB_L_RIGHT,
	THUMB_L_LEFT,

	// �E�X�e�B�b�N
	THUMB_R_UP,
	THUMB_R_DOWN,
	THUMB_R_RIGHT,
	THUMB_R_LEFT,

	// �\���L�[
	BUTTON_UP,
	BUTTON_DOWN,
	BUTTON_RIGHT,
	BUTTON_LEFT,

	BUTTON_X,
	BUTTON_Y,
	BUTTON_A,
	BUTTON_B,

	// �X�^�[�g�{�^��
	BUTTON_START,

	BUTTON_TYPE_MAX
};

enum PAD_ID
{
	PAD_1,
	PAD_2,
	PAD_MAX
};

using KEY_ARRAY = std::array<char, 256>;
using PAD_ARRAY = std::array<bool, BUTTON_TYPE_MAX>;

enum KEY_TYPE
{
	KEY_TYPE_NOW,
	KEY_TYPE_OLD,
	KEY_TYPE_MAX
};

class GameCtrl
{
public:
	GameCtrl();
	~GameCtrl();

	const KEY_ARRAY &GetCtrl(KEY_TYPE type) const;		// �����ɑΉ��������ݸނ̷����͏����擾����
	bool GetPadData(PAD_ID padNum, BUTTON_TYPE button) const;
	bool GetPadDataTrg(PAD_ID padNum, BUTTON_TYPE button) const;

	bool UpDate(void);		// ���̓��͏��̍X�V������

private:
	KEY_ARRAY data;			// ���݂��ڰт̷����͏����i�[����
	KEY_ARRAY dataOld;		// 1�ڰёO���ڰт̷����͏����i�[����

	std::array<PAD_ARRAY, PAD_MAX> padData;
	std::array<PAD_ARRAY, PAD_MAX> padDataOld;
};
