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
	int animNum;				// アニメーション画像の枚数
	std::vector<int> hitBoxNum;	// 画像1枚のHitBoxの数
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

	std::vector<ColInfo> data;		// その画像の当たり判定をまとめたvector
	//std::list<HitBox> collist;		// その画像の当たり判定をまとめたlist
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

