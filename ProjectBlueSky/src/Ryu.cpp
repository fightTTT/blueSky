#include "Ryu.h"

Ryu::Ryu()
{
	characterName = "リュウ";

	Init("image/リュウ/待機/stand_0.png", VECTOR2(290, 178), VECTOR2(1, 1), VECTOR2(300, 300));
	InitAnim();
}

Ryu::~Ryu()
{
}
