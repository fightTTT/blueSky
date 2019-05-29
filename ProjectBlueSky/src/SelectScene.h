#pragma once
#include <array>
#include "VECTOR2.h"
#include "BaseScene.h"
#include "ObjList.h"

#define CHAR_CNT_MAX (8)

class SelectScene :
	public BaseScene
{
public:
	SelectScene();
	~SelectScene();

	unique_Base UpDate(unique_Base own, const GameCtrl &controller);

private:
	VECTOR2 scSize;
	int itvCnt;				// ������ٗp����
	int flamCnt;			// �_�ŗp
	bool sceneChangeFlag;	// ��݂�ύX�����׸�
	bool commandTableFlag1;	// 1P�̺���ޕ\��\�������׸�
	bool commandTableFlag2;	// 1P�̺���ޕ\��\�������׸�

	int Init(void);
	void SelectDraw(void);
	sharedListObj objList;

	std::array<std::array<char, 12>, CHAR_CNT_MAX> charNameTbl;				// ���ID�ɑΉ��������O��ð���
};