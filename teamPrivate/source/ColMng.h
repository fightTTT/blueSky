#pragma once
#include <vector>
#include <map>
#include <list>
#include "VECTOR2.h"

struct DataHeader
{
	int colCnt;						// �ۑ�����ײ�ް�̐�
};

enum COL_TYPE {
	COL_TYPE_PLAYER,
	COL_TYPE_WEAPON,
	COL_TYPE_NON
};

typedef struct
{
	bool	 boxFlag;		// true:��`�@false:�~�`
	COL_TYPE weponFlag;		// ����̺ײ�ް�@��ڲԂ̺ײ�ް

	VECTOR2	 startPos;		// ��`�J�n�ʒu
	VECTOR2  endPos;		// ��`�I���ʒu

	VECTOR2 offset;			// �摜�̾�Ēl	���������Œ���������

	int		 r;				// �~�`���a

	int		 id;			// ���ڰіڂ�

	int		 attackPoint;	// �U����
	int		 angryPoint;	// �{��ް���߲��
	int		 specialPoint;	// �K�E�ް���߲��
}ColInf;

#define lpColMng  ColMng::GetInstance()

class ColMng
{
public:
	static ColMng &GetInstance(void)
	{
		static ColMng s_Instance;
		return (s_Instance);
	}

	//�Q�b�^�[�@�L�������ƃA�j���[�V���������w�肵�āA���̃A�j���[�V������Vector��Ԃ�(���c�V�X�e���Q�e�X�g����)
    std::vector<ColInf> GetCollider(std::string CharName, std::string AnimName);
	std::vector<ColInf> GetCollider(std::string CharName, std::string AnimName,int animFrame);

	//�V���[�_�[�@�L�������ƃA�j���[�V���������w�肵��Map�Ɋi�[����(���c�V�X�e���Q�e�X�g����)
	void ColLoad(std::string CharName, std::string AnimName);
private:
	ColMng();
	~ColMng();

	// ��ז�����Ұ��ݖ�����Ұ��ݔԍ�
	std::map<std::string, std::map<std::string, std::vector<ColInf>> >	colMap;			//�ײ�ް�����i�[
};

