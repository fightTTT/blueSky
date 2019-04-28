#pragma once
#include <array>
#include "BaseScene.h"
#include "GameScene.h"
#include "Fader.h"

#define PAGE_CNT_MAX (4)

enum AWARD {
	AWARD_GIVE_DAMAGE,		// �^������Ұ��
	AWARD_DEATH_ENEMY_CNT,	// �|�����G�̐� 
	AWARD_DEATH_ODER,		// ���񂾏���
	AWARD_CHICKEN,			// �ɉh��
	AWARD_MAX,
};

enum GRAY_OUT {
	GRAY_OUT_NON,
	GRAY_OUT_UP,			// �グ��
	GRAY_OUT_DOWN,			// ������
	GRAY_OUT_MAX
};

enum STATE {
	STATE_FADE_IN,			// ̪��޲ݒ�
	STATE_AWARD,			// ���ʔ��\��
	STATE_FADE_OUT,			// ̪��ޱ�Ē�
	STATE_NEXT,				// ���̼�݂ɑJ��
	STATE_MAX,
};

class ResultScene :
	public BaseScene
{
public:
	ResultScene(RESULT_INFO resultInfo, int joinPlayerNum);
	~ResultScene();

	unique_Base Update(unique_Base own, const GameController &ctrl);

private:
	void StateFadeInFunc(void);
	void StateAwardFunc(const GameController &ctrl);
	void StateFadeOutFunc(void);

	bool AwardFunc(int pNum, AWARD awardState);

	void NoJoinPadNumPointInit(RESULT_INFO& c_resultInfo);	// �ްтɎQ�����Ă��Ȃ��߯�ޔԍ��̓_����-1�ŏ�����
	void SortByRank(void);				// resultInfo���~���ɕ��ёւ�rankInfo�Ɋi�[����

	void DrawGrayOut(void);
	void GrayOutUpdate(void);

	int	 GetRankColor(int rank);		// ���ʂɑΉ������F��16�i���ŕԂ�
	int	 GetPointColor(int point);		// ���_�ɑΉ������F��16�i���ŕԂ��@rankPointTbl�ɑΉ������F

	void DrawFade(void);		// ̪��޲݁E̪��ޱ�ĕ`��p

	int Init(void);
	void ResultDraw(void);

	int resultCnt;

	int grayOutCnt;
	GRAY_OUT grayOut;

	RESULT_INFO resultInfo;		// �e����ްь��ʏ��
	RESULT_INFO rankInfo;		// �e���叇�ʏ��
	RESULT_INFO pointInfo;		// �e����l���߲�ď��

	float pointUnit;			// 1�߲�ĕӂ�̸��̂̒���

	std::array<int, PLAYER_MAX>		PosXTbl;			// �e�_���̂̒��SX���W
	std::array<int, PLAYER_MAX>		graphAddPoint;		// ���݂̖_���̂��߲��
	std::array<int, AWARD_MAX>		GraphColorTbl;		// �_���̂̐F
	std::array<int, PLAYER_MAX>		rankPointTbl;		// ���ʂ̊�b�_

	std::array<std::array<float, AWARD_MAX>, PLAYER_MAX>		graphPoint;
	std::array<std::array<int, AWARD_MAX>, PLAYER_MAX>			graphLength;

	AWARD awardState;			// �e����̏��
	STATE resultState;			// ػ��ļ�݂̏��

	bool blackFlag;

	bool awardEndFlag;			// ture:�e����̖_���̉��Z�܂ŏI��
	bool resEndFlag;			// true:�S����̖_���̉��Z�܂ŏI��
	int  awardCnt;				// �e���傲�Ƃ̶�����@0����++�����

	int scoreMax;				// �ō����_

	// SE�p
	bool winSeFlag;
	int seCnt;
	bool seFlag;
	bool seFlag2;
	bool seFlag3;

	int ajustCnt;				// ��ʕς���đ��{�^�������Ă��܂�����h���ׂ̃J�E���^�[

	Fader<double> alpha;		// ̪��޲݁E̪��ޱ�Ẳ�ʓ����x
};