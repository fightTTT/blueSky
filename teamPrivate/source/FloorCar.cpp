#include "DxLib.h"
#include "ImageMng.h"
#include "SceneMng.h"
#include "FloorCar.h"

#define RUN_Y			(530)

FloorCar::FloorCar(void)
{
	carInfTbl[CAR_NOMAL].floorSize	= VECTOR2(100, 5);
	carInfTbl[CAR_NOMAL].frontSizeX = 80;
	carInfTbl[CAR_NOMAL].imagePass	= "image/ゲームシーン用/car1.png";

	carInfTbl[CAR_KYUKYUSYA].floorSize	= VECTOR2(180,5);
	carInfTbl[CAR_KYUKYUSYA].frontSizeX = 60;
	carInfTbl[CAR_KYUKYUSYA].imagePass	= "image/ゲームシーン用/car_side_kyukyusya.png";

	Init();
}

FloorCar::~FloorCar()
{
}

void FloorCar::Update(void)
{
	pos.x += speed.x * (static_cast<int>(movType) * 2 - 1);

	if (movType == MOV_LEFT)
	{
		if (pos.x <= startPosTbl[static_cast<int>(MOV_RIGHT)])
		{
			Init();
		}
	}
	else
	{
		if (pos.x >= startPosTbl[static_cast<int>(MOV_LEFT)])
		{
			Init();
		}
	}
}

void FloorCar::Draw(void)
{
	VECTOR2 drawPos = pos + graphSize / 2;
	drawPos.x -= (movType == MOV_LEFT ? frontSize : backSize);

	DrawRotaGraph(
		drawPos,
		1.0, 0.0,
		lpImageMng.GetID(imagePass)[0],
		true, static_cast<bool>(movType)
	);

	BaseFloor::Draw();
}

bool FloorCar::Init(void)
{
	carType = static_cast<CAR_TYPE>(rand() % CAR_MAX);
	FloorCar::imagePass = carInfTbl[carType].imagePass;
	BaseFloor::size = carInfTbl[carType].floorSize;
	FloorCar::frontSize = carInfTbl[carType].frontSizeX;
	GetGraphSize(lpImageMng.GetID(imagePass)[0], &graphSize.x, &graphSize.y);
	backSize = graphSize.x - frontSize - size.x;

	startPosTbl = {
		lpSceneMng.GetScreenSize().x + graphSize.x + 100,
		-graphSize.x - 100
	};

	movType = static_cast<FLOOR_CAR>(rand() % 2);
	pos.x = startPosTbl[static_cast<int>(movType)];
	pos.y = RUN_Y - graphSize.y - (static_cast<int>(movType) * 30);
	speed.x = (rand() % 4) + 4;

	return true;
}
