#pragma once
#include <array>
#include "VECTOR2.h"
#include "Obj.h"

#define CHAR_CNT_MAX (8)
#define PALYER_CNT_MAX (2)

class CharSelCursor :
	public Obj
{
public:
	CharSelCursor(PAD_ID padId);
	~CharSelCursor();

private:
	VECTOR2 sSize;
	void SetMove(const GameCtrl &ctl, weekListObj objList);		// ���ق̓���
	bool CheckObjType(OBJ_TYPE type);
	int charID;													// �I���ł��鷬ׂ�ID
	int backCurID;												// �ޯ���ݗp���ق�ID
	int Init(void);
	void Draw(void);

	std::array<VECTOR2, CHAR_CNT_MAX> charSelPosTbl;					// ���ID�ɑΉ������ʒu��ð���
	std::array<VECTOR2, 2> sceneCurPosTbl;								// ����ޯ��p���݂�ð���
	std::array<unsigned int, PALYER_CNT_MAX> colorTbl;					// ��ڲ���ɑΉ������F��ð���

	bool decidFlag;			//�I�����肵�Ă��邩�ǂ������׸�(true:���肵�Ă���, false:���肵�Ă��Ȃ�)

	int mCount;		// ���ق̉�]�p�ϐ�
	int charCurMask;		// ��׾ڶ��ق�Ͻ�
	int sceneCurMask;		// ����ޯ����ق�Ͻ�
	bool padIdFlag;			// �ǂ��߯�ނ��g�p���ꂽ�����׸�(2P�ΐ펞����)�@true = 1P, false = 2P
};