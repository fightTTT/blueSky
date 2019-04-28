#pragma once
#include "VECTOR2.h"
#include <array>

class GameController;


// �摜�̎��R�ό`�`��
int	DrawModiGraph(
	VECTOR2 leftUp,
	VECTOR2 rightUP,
	VECTOR2 rightDown,
	VECTOR2 leftDown,
	int GrHandle);


// �߰�ނ��߂������
enum TURN_DIR {
	TURN_DIR_LEFT,		// ���ɂ߂���
	TURN_DIR_RIGHT,		// �E�ɂ߂���
	TURN_DIR_MAX,
};

class TurnThePage
{
public:
	TurnThePage();
	// 3�b��Ɍ����	(TrunFlag = ture)�E�E�E�E�߂���
	TurnThePage(VECTOR2 imageSize, VECTOR2 offset, int frontImage, int backImage, bool TurnFlag = false);
	// ������߰�ނ̎��Ԃ̎w��(�b)
	TurnThePage(VECTOR2 imageSize, VECTOR2 offset, int frontImage, int backImage, float waitTime, bool TurnFlag = false);

	~TurnThePage();

	bool Draw(void);

private:
	// �E�߂��荶�߂��苤�ʂ̏�����
	void Init(VECTOR2 imageSize, VECTOR2 offset, int frontImage, int backImage, bool TurnFlag);
	// �E�߂���ŗL�̏�����
	void InitTurnLeft();
	// ���߂���ŗL�̏�����
	void InitTurnRight();

	std::array<std::array<VECTOR2, 4>, 12>	pos;		// �ό`�`��̍ۂ̍��W���i�[

	VECTOR2 imageSize;	// �摜�̑傫��		�����Ƃ��Ď󂯎��
	VECTOR2 offset;		// �`��̾�Ēl		�����Ƃ��Ď󂯎��
	int frontImage;		// �O�߰�ނ������	�����Ƃ��Ď󂯎��
	int backImage;		// ���߰�ނ������	�����Ƃ��Ď󂯎��

	int frontBuffer;	// �O�߰�ނ̉E�����̕`��
	int backBuffer;		// ���߰�ނ̍������̕`��
	int slideImage;		// �߂����Ă����߰�ނ������(�O����flontBuffer�㔼��backBuffer(���߂���̏ꍇ))

	int cnt;			// �����
	int slideNum;		// ���݂߂����Ă����߰�ނ̔ԍ�	�ő吔��#define slideImageMax

	float waitCnt;		// �߰�ނ������܂ł̶���

	TURN_DIR turnDir;	// �߰�ނ��߂������

	bool seFlag;
};