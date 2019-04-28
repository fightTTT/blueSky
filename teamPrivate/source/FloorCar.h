#pragma once
#include <array>
#include "BaseFloor.h"

enum FLOOR_CAR
{
	MOV_LEFT,
	MOV_RIGHT,
	MOV_MAX
};

enum CAR_TYPE
{
	CAR_NOMAL,
	CAR_KYUKYUSYA,
	CAR_MAX
};

typedef struct {
	VECTOR2 floorSize;
	int		frontSizeX;
	std::string imagePass;
}CarInf;

class FloorCar :
	public BaseFloor
{
public:
	//床のサイズ, 車の画像で前面からの床までの距離, 車の画像名
	FloorCar(void);
	~FloorCar();
	void Update(void);
	void Draw(void);

private:
	bool Init(void);

	std::array<CarInf, CAR_MAX> carInfTbl;
	FLOOR_CAR movType;
	CAR_TYPE carType;
	std::array<int, MOV_MAX> startPosTbl;
	VECTOR2 graphSize;
	std::string imagePass;
	int frontSize;	// 車の画像で前端から床までの距離
	int backSize;	// 車の画像で床から画像後端までの距離
};