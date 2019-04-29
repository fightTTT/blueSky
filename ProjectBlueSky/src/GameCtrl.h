#pragma once
#include <array>

using KEY_ARRAY = std::array<char, 256>;

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
	bool UpDate(void);		// ���̓��͏��̍X�V������

private:
	KEY_ARRAY data;			// ���݂��ڰт̷����͏����i�[����
	KEY_ARRAY dataOld;		// 1�ڰёO���ڰт̷����͏����i�[����
};
