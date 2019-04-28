#include "ExpOrb.h"
#include "math.h"
#include <stdlib.h>
#include "Obj.h"
#include "ImageMng.h"
#include "SoundMng.h"

#define PI				3.141592

#define WAIT_TIME			60		// 生成されてから待つﾌﾚｰﾑ数
#define WAIT_VANISH_TIME	(90 + WAIT_TIME) 		// このﾌﾚｰﾑを超えると目的地が消滅したと判断して玉を破棄
#define INS_RAND_RANGE		64		// 円形に生成される際の中心座標からの距離

#define ARRIVE_RANGE		20		// 終点座標から半径ARRIVE_RANGEの円を引いて、その中に玉が入ればdelete

#define ORB_SIZE			32		// ｹﾞｰｼﾞ玉のｻｲｽﾞ
#define ORB_FLASH_INV		6		// ｹﾞｰｼﾞ玉のｱﾆﾒｰｼｮﾝ間隔
#define ANIM_MAX			4		// ｱﾆﾒｰｼｮﾝ枚数

ExpOrb::ExpOrb(VECTOR2 startPos, int padNum, int addPoint)
{
	// 円形に生成する
	// -180°から180°(-3.14 ～ 3.14)の間でﾗﾝﾀﾞﾑな数字を取得する 
	int length314 = (rand() % (314 * 2 + 1)) - 314;				// -314～314を取得
	double circleRad = static_cast<double>(length314) / 100.0;	// -3.14～3.14を取得
	// 生成座標の決定
	startPos.x += static_cast<int>(INS_RAND_RANGE * cos(circleRad));
	startPos.y += static_cast<int>(INS_RAND_RANGE * sin(circleRad));

	// 初期化関係
	cnt = 0;
	pos_d.x = startPos.x;
	pos_d.y = startPos.y;

	this->addPoint	= addPoint;
	this->padNum	= padNum;

	animId = 0;

	this->endPos_d.x = 999.0;
	this->endPos_d.y = 999.0;

	lpSoundMng.SoundInit("sound/SE類/玉取得.mp3", "玉取得");

}

ExpOrb::~ExpOrb()
{
}

void ExpOrb::UpDate(weakListObj objList)
{
	if (cnt > WAIT_TIME)
	{
		POS_d endPos_d;

		bool moveF = false;		// true:移動OK	false:移動不可

		for (auto &obj : *objList.lock())
		{
			if (obj->GetPadNum() == padNum)
			{
				moveF = true;	// 目的地が見つかったので移動OK

				VECTOR2 tmp	= obj->GetPos();
				endPos_d.x	= static_cast<double>(tmp.x);
				endPos_d.y	= static_cast<double>(tmp.y);
			}
		}

		// x軸y軸の移動量を計算する
		if (moveF)
		{
			this->endPos_d = endPos_d;

			// 玉を飛ばす角度をラジアンで求める
			rad = atan2((static_cast<double>(endPos_d.y - pos_d.y)), (static_cast<double>(endPos_d.x - pos_d.x)));

			// x軸、y軸の移動量を算出
			moveSpeed.x = 10.0f * cos(rad);
			moveSpeed.y = 10.0f * sin(rad);


			// 移動
			pos_d.x = pos_d.x + moveSpeed.x;
			pos_d.y = pos_d.y + moveSpeed.y;
		}
	}

	cnt++;
	animId += !(cnt % ORB_FLASH_INV);
	animId %= ANIM_MAX;
}

void ExpOrb::DeathPrc(weakListObj objList)
{
	for (auto &obj : *objList.lock())
	{
		if (obj->GetPadNum() == padNum)
		{
			obj->AddSpecialPoint(addPoint);
		}
	}
}

bool ExpOrb::CheckArrive(void)
{
	if (cnt > WAIT_VANISH_TIME)
	{
		return true;		// 一定時間経過したので消える
	}

	if (cnt > WAIT_TIME)
	{
		POS_d r;			// 現在座標から終点座標までの、x軸y軸の長さ
		r.x = (pos_d.x - endPos_d.x);
		r.y = (pos_d.y - endPos_d.y);

		int hypotenuse;		// 現在座標から終点座標までの、斜辺の長さの「2乗」
		hypotenuse = static_cast<int>((r.x * r.x) + (r.y * r.y));

		// 終点座標から半径ARRIVE_RANGEの円を引いて、その中に玉が入ればdelete
		if (hypotenuse < (ARRIVE_RANGE * ARRIVE_RANGE))
		{
			lpSoundMng.SetSound("玉取得");
			return true;
		}
	}

	return false;
}

VECTOR2 ExpOrb::GetPos(void)
{
	return VECTOR2(static_cast<int>(pos_d.x), static_cast<int>(pos_d.y));
}

int ExpOrb::GetOrbColor(void)
{
	return orbColor;
}

void ExpOrb::Draw(void)
{
	DrawRectGraph(
		static_cast<int>(pos_d.x - ORB_SIZE / 2), 
		static_cast<int>(pos_d.y - ORB_SIZE / 2),
		ORB_SIZE * animId, 0,
		ORB_SIZE, ORB_SIZE,
		lpImageMng.GetID("image/UI/StarBlue.png")[0], true, false);
}
