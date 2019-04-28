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
	//���̃T�C�Y, �Ԃ̉摜�őO�ʂ���̏��܂ł̋���, �Ԃ̉摜��
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
	int frontSize;	// �Ԃ̉摜�őO�[���珰�܂ł̋���
	int backSize;	// �Ԃ̉摜�ŏ�����摜��[�܂ł̋���
};