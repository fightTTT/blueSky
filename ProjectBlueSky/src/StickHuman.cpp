#include "StickHuman.h"


StickHuman::StickHuman(VECTOR2 pos, VECTOR2 offset, PAD_ID id, DIR charaDir) : Character(offset)
{
	characterName = "棒人間";

	Init("image/棒人間/待機/stand_0.png", VECTOR2(256, 256), VECTOR2(1, 1), VECTOR2(pos.x, pos.y), false, id);

	spAttackCommand[0][DIR_RIGHT].resize(3);
	spAttackCommand[0][DIR_RIGHT] = { COM_DIR_DOWN, COM_DIR_RIGHT_DOWN, COM_DIR_RIGHT };
	spAttackCommand[0][DIR_LEFT].resize(3);
	spAttackCommand[0][DIR_LEFT] = { COM_DIR_DOWN, COM_DIR_LEFT_DOWN, COM_DIR_LEFT };

	spAttackCommand[1][DIR_RIGHT].resize(1);
	spAttackCommand[1][DIR_RIGHT] = { COM_DIR_CENTER };
	spAttackCommand[1][DIR_LEFT].resize(1);
	spAttackCommand[1][DIR_LEFT] = { COM_DIR_CENTER };

	spAttackCommand[2][DIR_RIGHT].resize(1);
	spAttackCommand[2][DIR_RIGHT] = { COM_DIR_CENTER };
	spAttackCommand[2][DIR_LEFT].resize(1);
	spAttackCommand[2][DIR_LEFT] = { COM_DIR_CENTER };

	dir = charaDir;
}

StickHuman::~StickHuman()
{
}
