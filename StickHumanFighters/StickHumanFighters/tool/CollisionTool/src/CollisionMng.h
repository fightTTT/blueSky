#pragma once
#include<list>
#include "Collision.h"
#include "InputManager.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define IMAGE_OFFSET_X (WINDOW_WIDTH / 2 - 50)
#define IMAGE_OFFSET_Y (WINDOW_HEIGHT / 2 - 50)

struct DataHeader
{
	int animNum;				// �A�j���[�V�����摜�̖���
	std::vector<int> hitBoxNum;	// �摜1����HitBox�̐�
};

#define lpColMng  CollisionMng::GetInstance()

class CollisionMng
{
public:
	static CollisionMng &GetInstance()
	{
		static CollisionMng s_Instance;
		return s_Instance;
	}

	bool ColLoad(std::string charaName, std::string animName);
	void Update(void);
	void Draw(void);

private:

	CollisionMng();
	~CollisionMng();

	std::vector<ColInfo> data;		// ���̉摜�̓����蔻����܂Ƃ߂�vector
	//std::list<HitBox> collist;		// ���̉摜�̓����蔻����܂Ƃ߂�list
	HitBox colData;					

	
	std::string charName;
	std::string animName;
	int frameNum;
	bool inputFlag;
	//int texHandle;
	InputManager inputData;
	DataHeader header;
	int animFram[20];
	int colTypeCnt;

	bool loadFlag;
	bool saveFlag;

	void ColSave(std::string pasName, std::string animName);

	//VECTOR2 mousePos;

	void Init(void);

	
};

