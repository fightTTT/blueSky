#include "DxLib.h"
#include "SceneMng.h"
#include "ImageMng.h"
#include "Obj.h"
#include "BaseFloor.h"
#include "Gamecontroller.h"
#include "Collision.h"
#include "Color.h"
#include "SoundMng.h"

#define ADD_SPECIAL_POINT	(1)			// ﾀﾞﾒｰｼﾞを受けた際に加算する必殺ｹﾞｰｼﾞ量

#define ADD_ANGRY_POINT		(1.0f)		// 怒りｹﾞｰｼﾞ溜め時のポイント加算量
#define REMOVE_ANGRY_POINT	(-2)		// 攻撃した側から減らす怒りｹﾞｰｼﾞ量

#define CHARGE_CNT_LW		(45)		// 弱ﾁｬｰｼﾞ完了までのﾌﾚｰﾑ数
#define CHARGE_CNT_HV		(90)		// 強ﾁｬｰｼﾞ完了までのﾌﾚｰﾑ数

#define HIT_CNT_MAX			(5)

#define SPECIAL_GAUGE_SIZE_X	(160)
#define SPECIAL_GAUGE_SIZE_Y	(27)

#define INVISIBLE_TIME		(120);		// 無敵時間

#define HEAVY_DAMEGE	(10)			// 数値以上だと大ﾀﾞﾒｰｼﾞ判定
#define MEDIUM_DAMEGE	(5)				// 数値以上だと中ﾀﾞﾒｰｼﾞ判定

#define FLY_TEXT_VANISH_CNT	(100)		// FLY_TEXTが消えるまでのﾌﾚｰﾑ数

#define HP_RED_COLOR_NUM	(30)		// HPがこの値以下になると赤文字になる（通常は緑）
#define HP_YELLOW_COLOR_NUM	(100)		// HPがこの値以下になると黄文字になる（通常は緑）

#define AVOID_COST			(50)		// 回避によって消費するｶﾞｰﾄﾞの耐久値

// 溜め完了ｱﾆﾒｰｼｮﾝ関系
#define CHARGE_FINISH_IMAGE_X		(150)
#define CHARGE_FINISH_IMAGE_Y		(150)
#define CHARGE_FINISH_ANIM_CNT_X	(5)
#define CHARGE_FINISH_ANIM_CNT_Y	(2)
#define CHARGE_FINISH_ANIM_SPEED	(3)

// 影の描画
#define SHADOW_DEF_SIZX (20)
#define SHADOW_DEF_SIZY (5)

// 残像の表示数
#define SHADOW_DRAW_FRAM		(10)

// ｼﾞｬﾝﾌﾟ
#define JUMP_CNT_MAX   (2)		// 最大ｼﾞｬﾝﾌﾟ可能回数
#define JUMP_EFFECT_INV	(5)		// ｼﾞｬﾝﾌﾟｴﾌｪｸﾄ間隔

// ｶﾞｰﾄﾞ
#define GUARD_TIME_MAX	(180)	// 最大ｶﾞｰﾄﾞ可能時間

// 吹っ飛ばし力
#define FLY_HIGH_POWER VECTOR2(3,7)

RESULT_INFO Obj::resultInfo;

Obj::Obj()
{
}

bool Obj::init(std::string charName)
{
	Obj::charName = charName;

	initAnim();

	guardTime = GetGuardTimeMax();

	dir = DIR::RIGHT;
	vX = 0.0;
	vY = 0.0;
	speed = PLAYER_DEF_SPEED;
	jumpCnt = GetJumpCntMax();

	hp = HP;
	oldHp = hp;
	angryPoint = 0.0f;
	specialPoint = 0;

	invInf = {};
	floorCheckFlag = false;
	parentFloor.itrCnt = 0;
	parentFloor.oldPos = { 0,0 };

	dashFlag = false;
	downFlag = false;
	framCnt = 0;
	nowTimeLR = 0;
	nowTimeDown = 0;
	animId = 0;
	hitCnt = 0;
	jumpEffectCnt = 0; 
	jumpEffectFlag = false;
	jumpPos = {};

	deri = DERI_NON;

	fallFlag = false;

	hitPlayerInf = {};
	multiHitFlag = 0U;
	damageInv = 0U;
	nextMultHitInv = 0U;
	attackDir = DIR::LEFT;

	multiAttackNum = 0;

	angryFlag = false;
	chargeStage = CHARGE_NON;
	chargeCnt = 0;

	spAttackFlag = false;
	spAttackCnt = 0;
	spAttackHitFlag = false;
	spAttackInv = 0U;

	gravity = 0.4f;

	fixCtrlCnt = 0;

	deathFlag = false;

	shadowPosY = GROUND_POS_Y;

	// charNameをﾅﾝﾊﾞﾘﾝｸﾞする
	charNumbering["雨野降太"] = 0;
	charNumbering["双葉青砥"] = 1;
	charNumbering["箒"] = 2;

	std::string color[4] = { "red","blue","green","yellow" };
	std::string imagePass = "image/しゃぼん玉/shabon_" + color[padNum] + ".png";

	guardImage = lpImageMng.GetID(imagePass)[0];

	hitEffectFileName = {
		"大ダメージ","ドガッ.png",
		"中ダメージ","バキッ.png",
		"小ダメージ(ぽかっ)","ぽかっ.png",
	};

	plStateFileName = { "sweat.png", "angry.png", "special.png" };

	plStateCnt = 30;

	playerNumSprite = true;
	shadowList.clear();

	// 必殺技ｹﾞｰｼﾞの色(青→緑→黄→橙→赤)
	spGaugeColorTbl = { 0x0000ff, 0x00ff7f ,0xffff00 ,0xff8c00 ,0xff0000 };

	// ﾌｫﾝﾄの追加
	lpFontMng.FontInit("font/TanukiMagic.ttf", "たぬき油性マジック", "たぬき20縁", 20, -1, true, false);
	lpFontMng.FontInit("font/TanukiMagic.ttf", "たぬき油性マジック", "たぬき40縁", 40, -1, true, false);
	lpFontMng.FontInit("font/TanukiMagic.ttf", "たぬき油性マジック", "たぬき80縁斜体", 80, 9, true, true);
	lpFontMng.FontInit("font/Molot.otf", "Molot", "Molot80縁斜体", 80, 9, true, true);
	lpFontMng.FontInit("font/Square.ttf", "SquareFont", "スクエア28縁斜体", 28, -1, true, true);
	lpFontMng.FontInit("font/Square.ttf", "SquareFont", "スクエア18縁斜体", 18, -1, true, true);
	lpFontMng.FontInit("font/Square.ttf", "SquareFont", "スクエア20縁斜体", 20, -1, true, true);
	// Seの登録
	initSe();

	// ﾘｻﾞﾙﾄ情報の初期化
	resultInfo.totalGiveDamage[padNum]		= 0;
	resultInfo.deathEnemyCnt[padNum]		= 0;
	resultInfo.chickenKillCnt[padNum]		= 0;
	resultInfo.chickenCnt[padNum]			= 0;
	resultInfo.deathOrder[padNum]			= PLAYER_MAX;	// PLAYER_MAXなら生きている
	resultInfo.charName[padNum]				= charName;

	// 画像の空読み込み
	lpImageMng.GetID("image/UI/fire.png")[0];
	lpImageMng.GetID("image/UI/詠唱8.png")[0];
	lpImageMng.GetID("image/UI/詠唱1.png")[0];
	lpImageMng.GetID("image/UI/dash.png")[0];
	lpImageMng.GetID("image/UI/jump.png",VECTOR2(61,39),VECTOR2(7,1));
	lpImageMng.GetID("image/UI/angry2.png")[0];
	lpImageMng.GetID("image/UI/specialGaugeFrame.png")[0];
	lpImageMng.GetID("image/UI/specialGauge.png")[0];

	lpImageMng.GetID("image/UI/sweat.png")[0];
	lpImageMng.GetID("image/UI/angry.png")[0];
	lpImageMng.GetID("image/UI/special.png")[0];

	lpImageMng.GetID("image/UI/大ダメージ/ドガッ.png")[0];
	lpImageMng.GetID("image/UI/中ダメージ/バキッ.png")[0];
	lpImageMng.GetID("image/UI/小ダメージ(ぽかっ)/ぽかっ.png")[0];

	return true;
}

bool Obj::initAnim(void)
{
	return false;
}

bool Obj::initSe(void)
{
	// Seの登録
	lpSoundMng.SoundInit("sound/SE類/Attack.mp3", "通常攻撃");
	lpSoundMng.SoundInit("sound/SE類/Charge.mp3", "溜め始まり");
	lpSoundMng.SoundInit("sound/SE類/ChargeMax.mp3", "溜め完了");
	lpSoundMng.SoundInit("sound/SE類/Smash.mp3", "溜め攻撃");

	lpSoundMng.SoundInit("sound/SE類/DamageS.mp3", "ダメージ小");
	lpSoundMng.SoundInit("sound/SE類/DamageM.mp3", "ダメージ中");
	lpSoundMng.SoundInit("sound/SE類/DamageL.mp3", "ダメージ大");

	lpSoundMng.SoundInit("sound/SE類/Guard.mp3", "ガード");
	lpSoundMng.SoundInit("sound/SE類/コン.mp3", "ガード成功");

	lpSoundMng.SoundInit("sound/SE類/FootSteps.mp3", "歩く");
	lpSoundMng.SoundInit("sound/SE類/Jump.mp3", "ジャンプ始まり");

	lpSoundMng.SoundInit("sound/SE類/AngryCharge.mp3", "怒りゲージ溜め", 85);
	lpSoundMng.SoundInit("sound/SE類/AngerMAX.mp3", "怒りMAX");

	lpSoundMng.SoundInit("sound/SE類/Special.mp3", "必殺技");
	lpSoundMng.SoundInit("sound/SE類/SpMAX.mp3", "SPMAX");

	lpSoundMng.SoundInit("sound/SE類/KnockOut.mp3", "ダウン",80);

	lpSoundMng.SoundInit("sound/SE類/UmbFloat.mp3", "傘浮遊");

	walkSeCnt = 0;
	angChargeSeCnt = 0;
	umbFloatCnt = 0;
	spmaxSeFlag = false;
	chargeSeFlag = false;
	angerMaxSeFlag = false;

	return true;
}

void Obj::PlaySe(void)
{
	// ﾗﾑﾀﾞ式
	auto PlaySE = [=](std::string animName, std::string soundName)
	{
		if (CheckAnimSe(animName))
		{
			lpSoundMng.SetSound(soundName);
		}
	};

	// 音の再生
	PlaySE("通常攻撃1", "通常攻撃");
	PlaySE("通常攻撃2", "通常攻撃");
	PlaySE("通常攻撃3", "通常攻撃");

	PlaySE("通常攻撃_上", "通常攻撃");
	PlaySE("通常攻撃_下始まり", "通常攻撃");

	PlaySE("溜め始まり", "溜め始まり");
	PlaySE("溜め攻撃", "溜め攻撃");

	// 溜め攻撃のSE
	if (chargeCnt == CHARGE_CNT_LW)
	{
		lpSoundMng.SetSound("溜め完了");
	}
	if (chargeCnt == CHARGE_CNT_HV && !chargeSeFlag)
	{
		lpSoundMng.SetSound("溜め完了");
		chargeSeFlag = true;
	}
	if (chargeCnt == 0)
	{
		chargeSeFlag = false;
	}

	PlaySE("ジャンプ始まり", "ジャンプ始まり");

	PlaySE("ガード", "ガード");

	PlaySE("必殺技", "必殺技");

	if (CheckDamageSe() == DMG_LW)
	{
		lpSoundMng.SetSound("ダメージ小");
	}

	if (CheckDamageSe() == DMG_MD)
	{
		lpSoundMng.SetSound("ダメージ中");
	}

	if (CheckDamageSe() == DMG_HV)
	{
		lpSoundMng.SetSound("ダメージ大");
	}

	if (animName == "歩く" || animName == "走る")
	{
		if (animName == "歩く")
		{
			if (walkSeCnt % 20 == 0)
			{
				lpSoundMng.SetSound("歩く");
			}
		}
		if (animName == "走る")
		{
			if (walkSeCnt % 13 == 0)
			{
				lpSoundMng.SetSound("歩く");
			}

		}
		walkSeCnt++;
	}
	else
	{
		walkSeCnt = 0;
	}

	if (animName == "怒りゲージ溜め")
	{
		if (angChargeSeCnt % 10 == 0)
		{
			lpSoundMng.SetSound("怒りゲージ溜め");
		}
		angChargeSeCnt++;
	}
	else
	{
		angChargeSeCnt = 0;
	}

	if (specialPoint == 100)
	{
		if (!spmaxSeFlag)
		{
			lpSoundMng.SetSound("SPMAX");
			spmaxSeFlag = true;
		}
	}
	else
	{
		spmaxSeFlag = false;
	}

	if (angryFlag)
	{
		if (!angerMaxSeFlag)
		{
			lpSoundMng.SetSound("怒りMAX");
			angerMaxSeFlag = true;
		}
	}
	else
	{
		angerMaxSeFlag = false;
	}

	if (animName == "傘浮遊")
	{
		if (umbFloatCnt % 60 == 30)
		{
			lpSoundMng.SetSound("傘浮遊");
		}
		umbFloatCnt++;
	}
	else
	{
		umbFloatCnt = 0;
	}
}

void Obj::PlaySPSe(void)
{
}

Obj::~Obj()
{
}

void Obj::UpDate(const GameController &controller, weakListObj objList, weakListChicken chickenList, bool entryFlag)
{
	GetJoypadXInputState(padNum + 1, &xstate[padNum]);

	if (AlwaysUpDate(controller, chickenList))
	{
		return;
	}

	VECTOR2 tmpPos = pos;

	// 自分のｺﾘｼﾞｮﾝの取得
	auto collisionVec = lpColMng.GetCollider(GetCharName(), GetAnim(), animId);

	int maxEndPosY = GetColMaxEndPosY(collisionVec);				// 現ﾌﾚｰﾑのｺﾗｲﾀﾞｰの最大Y座標

	PosUpDate(tmpPos,maxEndPosY);

	if (param == PARAM::FALLING)
	{
		floorCheckFlag = true;
	}

	if (!fixCtrlCnt)	// 硬直時は行動できない
	{
		// 	攻撃
		if (animPtnTbl[GetAnim()] == ANIM_TYPE_MULTI_HIT)
		{
			DeriAttack(controller);
		}
		else
		{
			Attack(controller, tmpPos, entryFlag);
			if (spAttackFlag)
			{
				spAttackFlag = false;
				spAttackCnt += GetSpAttackTimeMax();
			}
			// 派生ｸﾗｽ特有の攻撃方法を行う
			UniqueAttack(tmpPos);
		}
	}

	if (!spAttackHitFlag && GetAnim() != "必殺技")
	{
		// (   攻撃中 && 着地している   )　　←の場合以外は座標移動できる
		if (!((animPtnTbl[GetAnim()] != ANIM_TYPE_NO_ATTACK) && (locate != LOCATE::NON)))
		{
			if (!fixCtrlCnt || locate == LOCATE::NON)
			{
				// ﾀﾞｯｼｭ
				Dash(controller, tmpPos);
			}
		}
	}

	if (!fixCtrlCnt)	// 硬直時は行動できない
	{
		// ｼﾞｬﾝﾌﾟ
		JumpFnc(controller, tmpPos, maxEndPosY);

		// ↓↓キーの処理
		if (controller.GetInputDown(PAD_INPUT_DOWN, padNum))
		{
			if (GetAnim() != "ガード")
			{
				if ((framCnt - nowTimeDown) < PRASS_INV && locate != LOCATE::NON && animPtnTbl[GetAnim()] == ANIM_TYPE_NO_ATTACK)
				{
					downFlag = true;
				}
				nowTimeDown = framCnt;
			}
		}
		if (downFlag)
		{
			// ｻｶﾞにいる時に↓↓キーで下界へ
			floorCheckFlag = false;
		}
		if (controller.GetInputHold(PAD_INPUT_DOWN, padNum))
		{
			// 空中↓キーで急下降
			if ((locate == LOCATE::NON) && (param == PARAM::FALLING) && GetAnim() != "必殺技")
			{
				fallFlag = true;
			}
		}
	}

	if (ColGround(tmpPos, collisionVec, maxEndPosY))	// 地面と床の当たり判定
	{
		UniqueColGround();	// 継承先の処理
	}

	MoveEndUpDate(tmpPos, controller, maxEndPosY);		// 移動完了後の更新
}

void Obj::CheckHit(weakListObj objList, std::list<ExpOrb*>& expOrbList, bool entryFlag)
{
	// 1ﾌﾚｰﾑ前のHPの残量
	oldHp = hp;

	// 全員がｴﾝﾄﾘｰしていない(始まっていない)時は当たり判定をしない
	if (!entryFlag)
	{
		return;
	}

	// damageInvが60なら
	if (damageInv == 60U)
	{
		hitPlayerInf = {};		// 受けた攻撃情報をﾘｾｯﾄ
	}
	if (damageInv == 1U)
	{
		hitCnt = 0;
	}
	// damageInvが0より大きいなら1減算する
	damageInv > 0U ? damageInv-- : damageInv = 0U;
	// nextMultHitInvが1なら無敵状態に
	if (nextMultHitInv == 1U && !spAttackHitFlag)
	{
		pos.x += 30 * (static_cast<int>(attackDir) * -2 + 1);		// ﾉｯｸﾊﾞｯｸ
		hitCnt = 0;
		visible = INVISIBLE_TIME;					// 1s無敵状態
		spAttackInv = 0;
	}
	// nextMultHitInvが0より大きいなら1減算する
	nextMultHitInv > 0U ? nextMultHitInv-- : nextMultHitInv = 0U;

	if (spAttackInv == 1U)
	{
		spAttackHitFlag = false;
		pos.x += 30 * (static_cast<int>(attackDir) * -2 + 1);		// ﾉｯｸﾊﾞｯｸ
		hitCnt = 0;
		visible = INVISIBLE_TIME;					// 1s無敵状態
	}
	spAttackInv > 0U ? spAttackInv-- : spAttackInv = 0U;

	// 攻撃を受ける側の当たり判定

	if (spAttackCnt)
	{
		return;
	}

	if (visible)
	{
		return;					// 無敵時間中なら当たり判定を行わない
	}

	for (auto &obj : *objList.lock())
	{
		if (obj->padNum == this->padNum)
		{
			continue;			// 自分自身とは当たり判定を行わない
		}

		if (obj->GetAnim() == "死亡")
		{
			continue;
		}

		// 判定しようとしている攻撃がそのﾌﾟﾚｲﾔｰから受けた最後の攻撃と同じなら
		if (hitPlayerInf[(*obj).GetPadNum()].attackName == (*obj).GetAnim())
		{
			// ｺﾏごとに攻撃を受けか
			if (!multiHitFlag)
			{
				continue;
			}
			if (hitPlayerInf[(*obj).GetPadNum()].animId == (*obj).GetAnimId())
			{
				continue;			// 判定しようとしている攻撃がそのﾌﾟﾚｲﾔｰから受けた最後の攻撃と同じｺﾏなら判定を行わない
			}
		}

		auto myCollision = GetPlayerCollision();
		for (auto myCol : myCollision)
		{
			if (myCol.weponFlag)
			{
				continue;		// 自分自身の武器とは当たり判定を行わない
			}

			auto otherCollision = (*obj).GetPlayerCollision();
			for (auto otherCol : otherCollision)
			{
				if (!otherCol.weponFlag)
				{
					continue;	// 相手の武器ｺﾗｲﾀﾞｰ以外とは当たり判定を行わない
				}

				//////////////////////////////////////////////////////////////////////////
				// ｺｺまで抜けた時点で　													//
				// myCol	= 自分の武器ではないｺﾗｲﾀﾞｰがVectorで格納されている			//
				// otherCol = 敵の武器ｺﾗｲﾀﾞｰがVectorで格納されている					//
				//////////////////////////////////////////////////////////////////////////

				// dir == LEFT なら当たり判定の向きを逆にする
				myCol.startPos.x *= static_cast<int>(dir) * -2 + 1;
				myCol.endPos.x *= static_cast<int>(dir) * -2 + 1;
				VECTOR2 myStartPos = pos + myCol.startPos;
				VECTOR2 myEndPos = pos + myCol.endPos;

				auto otherPos = (*obj).GetPos();
				auto otherPosOld = (*obj).GetPosOld();
				VECTOR2 defPos = otherPos - otherPosOld;	// posの差
				otherCol.startPos.x *= static_cast<int>((*obj).GetDir()) * -2 + 1;
				otherCol.endPos.x *= static_cast<int>((*obj).GetDir()) * -2 + 1;
				VECTOR2 otherStartPos = (*obj).GetPos() + otherCol.startPos;
				VECTOR2 otherEndPos = (*obj).GetPos() + otherCol.endPos;

				// 左上始まりになるようにする
				if (otherStartPos.x > otherEndPos.x
					|| otherStartPos.y > otherEndPos.y)
				{
					VECTOR2 tmp = otherStartPos;
					otherStartPos = otherEndPos;
					otherEndPos = tmp;
				}

				defPos *= static_cast<int>((*obj).GetDir()) * -2 + 1;
				otherStartPos -= defPos;

				// 当たり判定を行う
				if (Square(myStartPos, myEndPos, otherStartPos, otherEndPos))
				{
					// 攻撃を受けた側 --------------------------
					hitPlayerInf[(*obj).GetPadNum()] = {	// 受けた攻撃の情報を格納
						(*obj).GetAnim(),
						(*obj).GetAnimId()
					};
					attackDir = (*obj).GetDir();

					// 攻撃を受けた側 --------------------------
					if (GetAnim() == "ガード")
					{
						guardTime -= otherCol.attackPoint / 2;	// 相手の攻撃分、ｶﾞｰﾄﾞ時間が減る
						if (guardTime < 0)
						{
							guardTime = 0;
						}
						lpSoundMng.SetSound("ガード成功");
						pos.x += 10 * (static_cast<int>(attackDir) * -2 + 1);		// ﾉｯｸﾊﾞｯｸ
						return;
					}

					damageInv = 90U;						// 受けた攻撃の情報を1.5s保持

					// 体力削り
					int damage = otherCol.attackPoint;		// 被ﾀﾞﾒｰｼﾞ量を変数に格納
					if (obj->CheckAngryMode())
					{
						damage = damage * 2;	// ぶちぎれﾓｰﾄﾞなら被ﾀﾞﾒｰｼﾞ量2倍
					}
					// ﾁｬｰｼﾞLWなら被ﾀﾞﾒｰｼﾞ量2倍、HVなら3倍
					damage = damage + (obj->CheckCharge() * damage);
					if (obj->CheckSpAttackCnt())
					{
						damage = damage * 2;
					}

					if (damage > hp)
					{
						damage = hp;
					}

					hp -= damage;

					if (hp <= 0)
					{
						// SEの再生
						lpSoundMng.SetSound("ダウン");

						SetAnim("死亡");
						int deathPlCnt = 0;		// 死んでいるﾌﾟﾚｲﾔの人数
						for (auto data : resultInfo.deathOrder)
						{
							deathPlCnt += (data != PLAYER_MAX) ? 1 : 0;
						}
						resultInfo.deathOrder[padNum] = deathPlCnt + 1;
					}
					else
					{
						SetAnim("ダメージ");					// 被ﾀﾞﾒｰｼﾞｱﾆﾒｰｼｮﾝ
					}

					animCnt = 0;
					hitCnt++;								// 攻撃を受けた回数の加算
					vY = 0.0f;

					// FLY_TEXTの情報をListに追加
					if (damage > 0)		// 0ﾀﾞﾒは表示しない
					{
						FLY_TEXT data;
						data.cnt = FLY_TEXT_VANISH_CNT;
						data.damage = damage;
						// 円形に生成する
						// -180°から180°(-3.14 ～ 3.14)の間でﾗﾝﾀﾞﾑな数字を取得する 
						int length314 = (rand() % (314 * 2 + 1)) - 314;				// -314～314を取得
						double circleRad = static_cast<double>(length314) / 100.0;	// -3.14～3.14を取得
																					// 生成座標の決定
						data.insPos.x = static_cast<int>(pos.x + 35 * cos(circleRad));
						data.insPos.y = pos.y - 50;
						flyTextList.push_back(data);
					}

					bool flyHigh = false;					// 吹っ飛ぶのか		true:吹っ飛ぶ
					auto oppAnim = (*obj).GetAnim();		// 相手のｱﾆﾒｰｼｮﾝ
					if ((hitCnt >= HIT_CNT_MAX && oppAnim != "必殺技") || oppAnim == "溜め攻撃")
					{
						pos.x += 30 * (static_cast<int>(attackDir) * -2 + 1);		// ﾉｯｸﾊﾞｯｸ
						hitCnt = 0; 
						flyHigh = true;
						visible = INVISIBLE_TIME;
					}
					if (oppAnim == "必殺技")
					{
						spAttackInv = 90U;			// 1.5s必殺技を食らわなければ移動可能にする
						spAttackHitFlag = true;		// 必殺技を受けている
						parentFloor.itrCnt = 99999;	// 座標固定するために床に乗っていないことにする
													// もし必殺技の最後の攻撃なら	今は－2で無理やり調整しているが必殺技その後硬直でｱﾆﾒｰｼｮﾝを分けるといいと思われる
						if ((*obj).GetAnimFrame() - 2 <= (*obj).GetAnimId())
						{
							flyHigh = true;
							spAttackInv = 1U;
						}
					}

					// もしその攻撃が大技系(連撃の最後や必殺技)なら
					if (oppAnim == "通常攻撃3" || oppAnim == "必殺技")
					{
						multiHitFlag = true;	// 毎ﾌﾚｰﾑ攻撃が当たるようになる
						if (oppAnim == "通常攻撃3")
						{
							flyHigh = true;
							nextMultHitInv = 1U;	// 無敵状態に
						}
					}
					else
					{
						multiHitFlag = false;
					}

					if (flyHigh)
					{
						locate = LOCATE::NON;
						vX =  static_cast<float>((*obj).GetFlyHighPower().x);
						vY = -static_cast<float>((*obj).GetFlyHighPower().y);
					}

					// 必殺玉の生成
					ExpOrb* SpecialOrb = new ExpOrb(pos, padNum, ADD_SPECIAL_POINT);
					expOrbList.push_back(SpecialOrb);

					// 攻撃した側 -------------------------
					obj->AddAngryPoint(REMOVE_ANGRY_POINT);								// 怒りｹﾞｰｼﾞ減算
					resultInfo.totalGiveDamage[obj->GetPadNum()] += damage;				// 与えた総ﾀﾞﾒｰｼﾞ加算
					resultInfo.deathEnemyCnt[obj->GetPadNum()] += (hp <= 0) ? 1 : 0;	// ﾗｽｱﾀ取ったら倒した人数に+1
																						// 必殺ｹﾞｰｼﾞ加算
					if (!(oppAnim == "必殺技"))
					{
						ExpOrb* SpecialOrb = new ExpOrb(obj->GetPos(), obj->GetPadNum(), otherCol.specialPoint);
						expOrbList.push_back(SpecialOrb);
					}

					auto screenSize = lpSceneMng.GetScreenSize();
					if (pos.x <= 0)
					{
						pos.x = 0;
					}
					if (pos.x > screenSize.x)
					{
						pos.x = screenSize.x;
					}

					if (invInf.invCnt)
					{
						invInf.invCnt--;
						return;							// 無敵ｶｳﾝﾄがあればﾀﾞﾒｰｼﾞを受けない
					}

					return;
				}
			}
		}
	}
}

bool Obj::CheckAngryMode(void)
{
	return angryFlag;
}

CHARGE_STAGE Obj::CheckCharge(void)
{
	return chargeStage;
}

CUT_IN_INFO Obj::CheckSpecialCutIn(void)
{
	CUT_IN_INFO rtnInfo = {};
	// 必殺技を出したﾌﾚｰﾑならtrueを返す
	if (animNameOld != "必殺技" && animName == "必殺技")
	{
		rtnInfo.padNum = padNum;
		rtnInfo.pos = pos;
		rtnInfo.cnt = 0;
		rtnInfo.flag = true;
		return rtnInfo;
	}
	return rtnInfo;
}

bool Obj::CheckObjType(OBJ_TYPE type)
{
	return (type == OBJ_TYPE_MAX);
}

void Obj::Draw(bool moveStopFlag, CUT_IN_INFO cutInInfo)
{

	// UI欄の枠描画
	SetDrawBlendMode(DX_BLENDMODE_ADD, 128);
	DrawGraph(padNum * 320, 560, lpImageMng.GetID("image/ゲームシーン用/枠2.png")[0], true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 128);

	// 怒りｹﾞｰｼﾞ溜めｴﾌｪｸﾄの描画
	if (GetAnim() == "怒りゲージ溜め")
	{
		// ｷｬﾗの後ろ
		DrawRectGraph(
			pos.x - 100 / 2, pos.y - 130 / 2,
			((framCnt / 5) % 5) * 100, 0,
			100, 100,
			lpImageMng.GetID("image/UI/fire.png")[0], true, false);

		// ｷｬﾗ立ち絵の後ろ
		DrawRectGraph(
			320 * padNum - 40, 570 - 50,
			((framCnt / 5) % 5) * 200, 0,
			200, 200,
			lpImageMng.GetID("image/UI/fire_big.png")[0], true, false);
	}

	// 強攻撃ﾁｬｰｼﾞ溜めｴﾌｪｸﾄの描画
	if (GetAnim() == "溜め始まり" || GetAnim() == "溜め中")
	{
		if (chargeStage == CHARGE_NON)
		{
			int animCnt = chargeCnt / 12;	// ++されるよー 
			if (animCnt < 5 * 2)
			{
				DrawRectGraph(
					pos.x - 150 / 2, pos.y - 150 / 2,
					150 * (animCnt % 5), 150 * (animCnt / 5 % 2),
					150, 150,
					lpImageMng.GetID("image/UI/溜め2.png")[0], true, false);
			}
		}
		else
		{
			// 溜め2段階目は色を変える
			int r, g, b = 0;
			if (chargeStage == CHARGE_HV)
			{
				GetDrawBright(&r, &g, &b);
				SetDrawBright(255, 125, 125);
			}

			// 強攻撃、ﾁｬｰｼﾞ完了ｴﾌｪｸﾄの描画
			int animCnt = chargeCnt / CHARGE_FINISH_ANIM_SPEED;	// ++されるよー 
			DrawRectGraph(
				pos.x - CHARGE_FINISH_IMAGE_X / 2, pos.y - CHARGE_FINISH_IMAGE_Y / 2,
				CHARGE_FINISH_IMAGE_X * (animCnt % CHARGE_FINISH_ANIM_CNT_X), CHARGE_FINISH_IMAGE_Y * (animCnt / CHARGE_FINISH_ANIM_CNT_X % CHARGE_FINISH_ANIM_CNT_Y),
				CHARGE_FINISH_IMAGE_X, CHARGE_FINISH_IMAGE_Y,
				lpImageMng.GetID("image/UI/詠唱1.png")[0], true, false);

			if (chargeStage == CHARGE_HV)
			{
				SetDrawBright(r, g, b);
			}
		}
	}

	bool checkFlag = false;

	for (auto itr : (*floorList))
	{
		if ((*itr).GetPos().y >= pos.y)
		{
			if (((*itr).GetPos().x <= pos.x + 32) && (pos.x - 32 <= ((*itr).GetPos().x + (*itr).GetSize().x)))
			{
				shadowPosY = (*itr).GetPos().y;
				checkFlag = true;
			}
		}
	}

	if (!checkFlag)
	{
		shadowPosY = GROUND_POS_Y;
	}

	int decreseSiz = (shadowPosY - pos.y) / 50;

	VECTOR2 shadowSiz = VECTOR2(SHADOW_DEF_SIZX, SHADOW_DEF_SIZY) - decreseSiz;

	if (shadowSiz.y <= 1)
	{
		shadowSiz.y = 1;
	}

	// ｷｬﾗ影の描画
	DrawOvalAA(static_cast<float>(pos.x), static_cast<float>(shadowPosY), static_cast<float>(shadowSiz.x), static_cast<float>(shadowSiz.y), 16, 0x000000, true);

	// ﾀﾞｯｼｭｴﾌｪｸﾄの描画
	if (GetAnim() == "走る")
	{
		DrawRectGraph(
			pos.x - 45 + (static_cast<int>(dir) * 45), pos.y,
			((framCnt / 4) % 3) * 48, 0,
			42, 35, lpImageMng.GetID("image/UI/dash.png")[0], true, static_cast<bool>(dir));
	}

	// ｼﾞｬﾝﾌﾟｴﾌｪｸﾄの描画
	if (jumpEffectFlag)
	{
		DrawRotaGraph(jumpPos.x, jumpPos.y, 1.0, 0.0,
			lpImageMng.GetID("image/UI/jump.png")[jumpEffectCnt / JUMP_EFFECT_INV], true);
	}

	std::vector<ColInf> collisonVec = lpColMng.GetCollider(charName, animName, animId);

	CharacterDraw(collisonVec, moveStopFlag, cutInInfo);	// ｷｬﾗの描画

	FlyTextDraw();			// ﾌﾗｲﾃｷｽﾄの描画

	AngryGaugeDraw();		// 怒りｹﾞｰｼﾞUIの描画

	SpecialGaugeDraw(cutInInfo.flag);		// 必殺ｹﾞｰｼﾞUIの描画

	GuardDraw();			// ｶﾞｰﾄﾞUIの描画

	StateIconDraw();		// ｽﾃｰﾀｽｱｲｺﾝ(ﾌｷﾀﾞｼ)の描画

	DrawGraph(padNum * 320, 560, lpImageMng.GetID("image/ゲームシーン用/枠1.png")[0], true);
							// ぶちぎれﾓｰﾄﾞかどうかのデバック表示
	if (angryFlag)
	{
		DrawRotaGraph(
			320 * padNum + ((framCnt / 30) % 2) * 40 + 40,
			600 + ((framCnt / 30) % 2) * 60,
			0.8,
			0.0,
			lpImageMng.GetID("image/UI/angry2.png")[0],
			true);
	}
	// HPの描画
	VECTOR2 offset = {};
	if (GetAnim() == "ダメージ" && !moveStopFlag)
	{
		offset = { GetRand(20) - 10, GetRand(20) - 10 };
	}

	int hpColor = 0x00ff7f;			// 緑
	if (hp <= HP_YELLOW_COLOR_NUM)
	{
		hpColor = 0xffff00;			// 黄色
	}
	if (hp < HP_RED_COLOR_NUM)
	{
		hpColor = 0xff4500;			// 赤
	}
	int drawLengh;
	drawLengh = GetDrawFormatStringWidthToHandle(lpFontMng.SetFont("Molot80縁斜体"), "%d", hp);
	DrawFormatStringToHandle(/*150*/300 + 320 * padNum + offset.x - drawLengh, 575 + offset.y, hpColor, lpFontMng.SetFont("Molot80縁斜体"), "%d", hp);
	DrawStringToHandle(120 + 320 * padNum /*+ offset.x*/, 625 /*+ offset.y*/, "HP", 0xffffff, lpFontMng.SetFont("スクエア20縁斜体"));

	// ﾋｯﾄｴﾌｪｸﾄ
	if (hitEffect.cnt > 0)
	{
		DrawRotaGraph(
			hitEffect.insPos, 1.0, hitEffect.rota + 1.57,
			lpImageMng.GetID("image/ヒット用エフェクト/" + hitEffectFileName[hitEffect.damageRate][0] + "/" + hitEffectFileName[hitEffect.damageRate][1])[0],
			true, false);
	}

	// 1P2Pの描画
	if (playerNumSprite && hp > 0)
	{
		DrawRotaGraph(
			pos.x, pos.y - 80,
			1.0, 0.0,
			lpImageMng.GetID("image/UI/1234.png")[padNum], true, false);
	}

	//// debugｷｬﾗ頭上に表示する文字
	//DrawFormatString(pos.x - 20, pos.y - 102, 0xffffff,GetAnim().c_str(), true);
	//DrawFormatString(pos.x - 20, pos.y - 122, 0xff0000, "ckillCnt:%d", resultInfo.chickenKillCnt[padNum], true);
	//DrawFormatString(pos.x - 20, pos.y - 142, 0xff0000, "stage:%d", chargeStage, true);
	//DrawFormatString(pos.x - 20, pos.y - 162, 0xff0000, "animCnt:%d", chargeCnt, true);

	// 自分のｺﾘｼﾞｮﾝの取得
	for (auto &col : collisonVec)
	{
		// dir == LEFT なら当たり判定の向きを逆にする
		col.startPos.x *= static_cast<int>(dir) * -2 + 1;
		col.endPos.x *= static_cast<int>(dir) * -2 + 1;
		VECTOR2 startPos = pos + col.startPos;
		VECTOR2 endPos = pos + col.endPos;

		//DrawBox(startPos, endPos, col.weponFlag ? 0x0000ff : 0xff0000, false);
	}

	// 怒りｹﾞｰｼﾞと必殺ｹﾞｰｼﾞの数値表示
	for (int j = 0; j < PLAYER_MAX; j++)
	{
		//DrawFormatString(130 + 320 * padNum, 660, 0x000000, "怒りポイント %d", angryPoint);

		int numDigit = 0;	// 数字の桁数　例(3 = 1, 60 = 2, 100 = 3)
		int tmp = specialPoint;
		while (tmp >= 10)
		{
			tmp /= 10;		// 桁を1つ減らす
			numDigit++;
		}

		DrawFormatStringToHandle(255 - (numDigit * 10) + (320 * padNum), 675, 0xff0000, lpFontMng.SetFont("スクエア28縁斜体"), "%d", specialPoint);
		DrawStringToHandle(290 + (320 * padNum), 685, "%", 0xffffff, lpFontMng.SetFont("スクエア18縁斜体"));
		//DrawFormatStringToHandle(205 - (numDigit * 10) + (320 * padNum), 655, 0xff0000, lpFontMng.SetFont("たぬき40縁"), "%d", specialPoint);
		//DrawStringToHandle(250 + (320 * padNum), 675, "％", 0xffffff, lpFontMng.SetFont("たぬき20縁"));
	}
}

const bool Obj::CheckAnimSe(std::string anim)
{
	if (animName == anim)
	{
		if (animName != animNameOld)
		{
			return true;
		}
		return false;
	}
	return false;
}

const DMG_LATE Obj::CheckDamageSe(void)
{
	// 現在のHPと1ﾌﾚｰﾑ前のHPとの差
	int difHp = oldHp - hp;

	if (hp != oldHp)
	{
		if (difHp >= HEAVY_DAMEGE)
		{
			// 大ﾀﾞﾒｰｼﾞ
			return DMG_HV;
		}
		else if (difHp >= MEDIUM_DAMEGE)
		{
			// 中ﾀﾞﾒｰｼﾞ
			return DMG_MD;
		}
		else
		{
			// 小ﾀﾞﾒｰｼﾞ
			return DMG_LW;
		}
	}
	return DMG_MAX;
}

void Obj::ResultInfoInit()
{
	Obj::resultInfo = {};
}

void Obj::AddChickenCnt(int padNum)
{
	resultInfo.chickenCnt[padNum]++;
}

int Obj::GetChickenCnt(int padNum)
{
	return resultInfo.chickenCnt[padNum];
}


int Obj::GetGuardTimeMax(void)
{
	return GUARD_TIME_MAX;
}

int Obj::GetJumpCntMax(void)
{
	return JUMP_CNT_MAX;
}

bool Obj::AlwaysUpDate(const GameController &controller, weakListChicken chickenList)
{
	if (invInf.animName != GetAnim())
	{
		invInf = {};
	}
	animNameOld = animName;
	posOld = pos;
	dirOld = dir;
	animIdOld = animId;
	framCnt++;

	auto updateCnt = [](auto& cnt) {
		if (cnt)
		{
			cnt--;
		}
	};
	updateCnt(visible);
	updateCnt(fixCtrlCnt);
	updateCnt(spAttackCnt);

	// ｱﾆﾒｰｼｮﾝidの計算
	UpDateAnimId();

	if (controller.GetInputDown(PAD_INPUT_7, padNum))
	{
		playerNumSprite = !playerNumSprite;
	}

	// ぶちぎれﾓｰﾄﾞのチェック
	if (angryPoint >= 100.0f)
	{
		angryFlag = true;
		angryCnt = ANGRY_CNT * 60;
		angryPoint = 0.0f;
	}
	if (angryFlag)
	{
		angryCnt >= 0 ? angryCnt-- : angryFlag = false;
	}

	// ｶﾞｰﾄﾞ時間の更新
	if (GetAnim() == "ガード")
	{
		guardTime <= 0 ? guardTime = 0 : guardTime--;
		if (guardTime <= 0)
		{
			fixCtrlCnt = 60;
			SetAnim("待機");
		}
	}
	else
	{
		if (guardTime < GetGuardTimeMax())
		{
			guardTime++;
		}
	}

	if (GetAnim() == "死亡")
	{
		if (animEndFlag)
		{
			AddChickenList()(chickenList, std::make_unique<Chicken>(floorList, pos, padNum, static_cast<CHICKEN_COLOR>(rand() % CHICKEN_COLOR_MAX)));
			deathFlag = true;
		}
		return true;
	}

	return false;
}

int Obj::GetColMaxEndPosY(std::vector<ColInf> collisionVec)
{
	int maxEndPosY = 0;
	for (auto &col : collisionVec)
	{
		if (col.weponFlag)
		{
			continue;				// 武器ｺﾗｲﾀﾞｰはmaxEndPosYに含まない
		}

		if (col.startPos.y > maxEndPosY)
		{
			maxEndPosY = col.startPos.y;
		}
		if (col.endPos.y > maxEndPosY)
		{
			maxEndPosY = col.endPos.y;
		}
	}
	return maxEndPosY;
}

void Obj::PosUpDate(VECTOR2 & tmpPos, int maxEndPosY)
{
	if (GetAnim() != "必殺技" && !spAttackHitFlag)
	{
		// 座標補正
		PosAdjust(tmpPos, maxEndPosY);

		// 自由落下
		if (vY > 0.0 && locate == LOCATE::NON)
		{
			param = PARAM::FALLING;
		}

		vY += gravity;

		// 速度分posを加算
		tmpPos.y += static_cast<int>(vY) * (static_cast<int>(fallFlag && param == PARAM::FALLING) + 1);
		tmpPos.x += static_cast<int>(vX);
	}
}

void Obj::DeriAttack(const GameController &controller)
{
	if (controller.GetInputDown(GPAD_INPUT_X, padNum))
	{
		if (deri == DERI_NON)
		{
			deri = DERI_NOMAL;// 攻撃中にさらに攻撃ﾎﾞﾀﾝの入力があった場合
		}
		if (GetAnim() == "通常攻撃2" && deri == DERI_NOMAL)
		{
			if (controller.GetInputHold(PAD_INPUT_DOWN, padNum))
			{
				deri = DERI_DOWN;
			}
			if (controller.GetInputHold(PAD_INPUT_UP, padNum))
			{
				deri = DERI_UP;
			}
		}
	}
	return;
}

void Obj::Attack(const GameController &controller, VECTOR2 &tmpPos, bool entryFlag)
{
	// 強攻撃ﾁｬｰｼﾞのｱﾆﾒｰｼｮﾝｶｳﾝﾀｰ更新
	chargeCnt = ((GetAnim() == "溜め始まり" || GetAnim() == "溜め中") ? chargeCnt + 1 : 0);

	if (GetAnim() == "必殺技")
	{
		return;
	}

	if (GetAnim() == "ダメージ")
	{
		// ﾁｬｰｼﾞ関連の情報をﾘｾｯﾄ
		chargeStage = CHARGE_NON;
		chargeCnt = 0;

		return;
	}

	if (GetAnim() == "回避")
	{
		tmpPos.x += (5 * -(static_cast<int>(dir) * 2 - 1));
		return;
	}

	// ｶﾞｰﾄﾞ
	if (controller.GetInputHold(GPAD_INPUT_R1, padNum) && guardTime)
	{
		if (animPtnTbl[GetAnim()] == ANIM_TYPE_NO_ATTACK || GetAnim() == "ガード")
		{
			bool flag = false;
			if (controller.GetInputDown(PAD_INPUT_LEFT, padNum))
			{
				dir = DIR::LEFT;
				flag = true;
			}
			if (controller.GetInputDown(PAD_INPUT_RIGHT, padNum))
			{
				dir = DIR::RIGHT;
				flag = true;
			}

			if (GetAnim() != "回避")
			{
				if (flag && guardTime >= AVOID_COST)
				{
					SetAnim("回避");
					visible = lpImageMng.GetSize(charName + "/" + "回避") * animTable[animName][ANIM_TBL_INV];
					guardTime -= AVOID_COST;
				}
			}
		}
		if (locate != LOCATE::NON && GetAnim() != "回避")
		{
			SetAnim("ガード");
			return;
		}
	}
	// ｶﾞｰﾄﾞﾎﾞﾀﾝを離したとき
	else
	{
		if (GetAnim() == "ガード")
		{
			SetAnim("待機");
			return;
		}
	}

	// 怒りﾓｰﾄﾞでないなら怒りゲージ溜め可能
	if (!angryFlag)
	{
		// 怒りゲージ溜め
		if (controller.GetInputHold(GPAD_INPUT_L1, padNum))
		{
			if (animPtnTbl[GetAnim()] == ANIM_TYPE_NO_ATTACK && locate != LOCATE::NON)
			{
				SetAnim("怒りゲージ溜め");
				return;
			}
			if (GetAnim() == "怒りゲージ溜め" && entryFlag)
			{
				if (angryPoint < 100 && !angryFlag)
				{
					angryPoint += ADD_ANGRY_POINT;
				}
			}
		}
		// 怒りゲージボタンを離したとき
		else
		{
			if (GetAnim() == "怒りゲージ溜め")
			{
				SetAnim("待機");
				fixCtrlCnt = FIX_TIME;
				return;
			}
		}

		// 怒りｹﾞｰｼﾞMAXで待機ｱﾆﾒｰｼｮﾝへ移行
		if (angryPoint >= 100)
		{
			SetAnim("待機");
		}
	}


	// 溜め
	if (controller.GetInputHold(GPAD_INPUT_Y, padNum))
	{
		if (animPtnTbl[GetAnim()] == ANIM_TYPE_NO_ATTACK)
		{
			if (GetAnim() != "溜め中")
			{
				SetAnim("溜め始まり");
				return;
			}
		}
		if (GetAnim() == "溜め中" && chargeStage != CHARGE_HV)
		{
			if (chargeCnt >= CHARGE_CNT_LW)
			{
				chargeStage = CHARGE_LW;
			}
			if (chargeCnt >= CHARGE_CNT_HV)
			{
				chargeStage = CHARGE_HV;
			}
		}
	}
	// 溜め攻撃
	else
	{
		if (GetAnim() == "溜め中")
		{
			if (charName == "双葉青砥")
			{
				invInf.invCnt = 1;
				invInf.animName = "溜め攻撃";
			}
			SetAnim("溜め攻撃");		// Yﾎﾞﾀﾝを離した時に溜めが完了していれば攻撃
			return;
		}
	}

	if (animPtnTbl[GetAnim()] != ANIM_TYPE_NO_ATTACK)
	{
		return;
	}

	// 通常攻撃
	if (controller.GetInputDown(GPAD_INPUT_X, padNum))
	{
		if (controller.GetInputHold(PAD_INPUT_UP, padNum))
		{
			if (charName == "竹部箒")
			{
				if (jumpCnt)
				{
					downFlag = false;
					param = PARAM::JUMPING;
					floorCheckFlag = false;
					jumpCnt--;
					locate = LOCATE::NON;
					SetAnim("通常攻撃_上");
				}
			}
			else
			{
				SetAnim("通常攻撃_上");
			}
			return;
		}
		else if (controller.GetInputHold(PAD_INPUT_DOWN, padNum))
		{
			SetAnim("通常攻撃_下始まり");
			return;
		}
		else
		{
			SetAnim("通常攻撃1");
			return;
		}
	}

	// 必殺技
	if ((xstate[padNum].LeftTrigger >= 255) && (xstate[padNum].RightTrigger >= 255))
		//if (controller.GetInputHold(GPAD_INPUT_B, padNum))
	{
		if (specialPoint >= 100)
		{
			spAttackFlag = true;
			SetAnim("必殺技");
			spAttackCnt += lpImageMng.GetSize(charName + "/" + animName) * animTable[animName][ANIM_TBL_INV];
			specialPoint = 0;
			return;
		}
	}

	return;
}

void Obj::UniqueAttack(VECTOR2 & tmpPos)
{
}

void Obj::UpDateAnimId(void)
{
	if (imageName.length() == 0)
	{
		return;
	}

	if (jumpEffectFlag)
	{
		// ｼﾞｬﾝﾌﾟｴﾌｪｸﾄ発生中の処理
		jumpEffectCnt++;
		if ((jumpEffectCnt / JUMP_EFFECT_INV) >= lpImageMng.GetID("image/UI/jump.png").size())
		{
			// ｱﾆﾒｰｼｮﾝが終わったのでﾌﾗｸﾞを折る
			jumpEffectFlag = false;
		}
	}

	if (animTable.find(animName) != animTable.end())
	{
		int count = (animCnt / animTable[animName][ANIM_TBL_INV]);
		if (animTable[animName][ANIM_TBL_LOOP] || count < animTable[animName][ANIM_TBL_FRAME])
		{
			count = (count % animTable[animName][ANIM_TBL_FRAME]);
		}
		else
		{
			count = animTable[animName][ANIM_TBL_FRAME] - 1;
			animEndFlag = true;
		}

		animId = (animTable[animName][ANIM_TBL_START_ID])
			+ count;
		animCnt += (angryFlag || (spAttackCnt && charName == "双葉青砥")) + 1;
	}
}

void Obj::Dash(const GameController &controller, VECTOR2 &tmpPos)
{
	if (GetAnim() == "ダメージ" || GetAnim() == "ガード")
	{
		return;
	}

	bool moveFlag = false;	// 左右移動していればtrue
	if (controller.GetInputHold(PAD_INPUT_RIGHT, padNum))
	{
		tmpPos.x += speed * (1 + static_cast<int>(dashFlag));
		if (animPtnTbl[GetAnim()] == ANIM_TYPE_NO_ATTACK)
		{
			dir = DIR::RIGHT;
		}
		moveFlag = true;
	}
	else if (controller.GetInputHold(PAD_INPUT_LEFT, padNum))
	{
		tmpPos.x -= speed * (1 + static_cast<int>(dashFlag));
		if (animPtnTbl[GetAnim()] == ANIM_TYPE_NO_ATTACK)
		{
			dir = DIR::LEFT;
		}
		moveFlag = true;
	}
	else
	{
		if ((locate != LOCATE::NON && (GetAnim() != "溜め中")))
		{
			SetAnim("待機");
		}
		dashFlag = false;
	}

	if (GetAnim() == "溜め中" || GetAnim() == "傘浮遊")
	{
		return;
	}

	if (dir != dirOld)
	{
		if (locate != LOCATE::NON)
		{
			SetAnim("歩く");
			dashFlag = false;
		}
	}

	if (locate != LOCATE::NON)
	{
		if (moveFlag)
		{
			dashFlag ? SetAnim("走る") : SetAnim("歩く");
		}
	}

	if ((controller.GetInputDown(PAD_INPUT_RIGHT, padNum) || controller.GetInputDown(PAD_INPUT_LEFT, padNum)))
	{
		if (locate != LOCATE::NON)
		{
			if ((framCnt - nowTimeLR < PRASS_INV) && (dir == dirOld))
			{
				SetAnim("走る");
				dashFlag = true;
			}
			else
			{
				SetAnim("歩く");
				dashFlag = false;
			}
		}
		nowTimeLR = framCnt;
	}
}

void Obj::JumpFnc(const GameController &controller, VECTOR2 &tmpPos, int maxEndPosY)
{
	// 必殺技を受けているとき	又は	与えているとき	は動けない
	if (!spAttackHitFlag && GetAnim() != "必殺技")
	{

		if (animPtnTbl[GetAnim()] == ANIM_TYPE_NO_ATTACK)
		{
			// ｼﾞｬﾝﾌﾟ
			if (controller.GetInputDown(GPAD_INPUT_A, padNum) && (jumpCnt > 0))
			{
				jumpCnt--;
				jumpEffectFlag = true;
				jumpEffectCnt = 0;
				jumpPos = VECTOR2(tmpPos.x, tmpPos.y + maxEndPosY);
				downFlag = false;
				param = PARAM::JUMPING;
				floorCheckFlag = false;

				// 下キーを押している && 地に足がついていれば ハイジャンプ
				vY = (controller.GetInputHold(PAD_INPUT_DOWN, padNum) && locate != LOCATE::NON ? -15.0f : -10.0f);
				locate = LOCATE::NON;

				SetAnim("ジャンプ始まり");
			}
		}
	}
}

bool Obj::ColGround(VECTOR2& tmpPos, std::vector<ColInf> colVec,int maxEndPosY)
{
	if (param != PARAM::JUMPING)
	{
		bool hitFlag = false;		// 床か地面につけばtrue

		auto GetColPos = [&](VECTOR2 pos)
		{
			pos.x *= static_cast<int>(dir) * -2 + 1;
			return VECTOR2(tmpPos + pos);
		};

		// 浮いている床との当たり判定
		if (floorCheckFlag && !spAttackHitFlag && GetAnim() != "必殺技")
		{
			bool flag = false;
			int itrCnt = 0;
			for (auto itr : *floorList)
			{
				VECTOR2 floorStartPos = itr->GetPos();
				VECTOR2 floorEndPos = itr->GetPos() + itr->GetSize();
				for (auto &col : colVec)
				{
					if (col.weponFlag)
					{
						continue;
					}

					VECTOR2 startPos = GetColPos(col.startPos);
					VECTOR2 endPos = GetColPos(col.endPos);

					// 1ﾄﾞｯﾄ加算することで丁度床の上にいるときも当たるようにしている
					endPos.y++;

					if (Square(VECTOR2(startPos.x, endPos.y - 20), endPos, floorStartPos, floorEndPos))
					{
						// 浮いている壁と衝突した場合
						flag = true;
						parentFloor.oldPos = itr->GetPos();
						parentFloor.itrCnt = itrCnt;
					}
				}
				itrCnt++;
			}
			if (flag)
			{
				hitFlag = true;
				tmpPos.y = parentFloor.oldPos.y - maxEndPosY;
				locate = LOCATE::FLOOR;
			}
			else
			{
				parentFloor.itrCnt = 0;
			}
		}


		// 地面との当たり判定
		bool flag = false;
		VECTOR2 groundStartPos(0, GROUND_POS_Y);
		VECTOR2 groundEndPos = groundStartPos + VECTOR2(1280, 120);

		for (auto &col : colVec)
		{
			if (col.weponFlag)
			{
				continue;
			}

			VECTOR2 startPos = GetColPos(col.startPos);
			VECTOR2 endPos = GetColPos(col.endPos);
			// 2ﾄﾞｯﾄ加算することで丁度床の上にいるときも当たるようにしている
			endPos.y += 2;

			if (Square(startPos, endPos, groundStartPos, groundEndPos))
			{
				if (col.weponFlag)
				{
					continue;
				}
				flag = true;
			}
		}
		if (flag)
		{
			hitFlag = true;
			locate = LOCATE::GROUND;
			tmpPos.y = GROUND_POS_Y - maxEndPosY;
		}

		if (hitFlag)
		{
			fallFlag = false;
			jumpCnt = GetJumpCntMax();
			if (GetAnim() == "ジャンプ中")
			{
				dashFlag ? SetAnim("走る") : SetAnim("待機");
			}
			if (GetAnim() == "通常攻撃_下中_空中")
			{
				fixCtrlCnt = FIX_TIME;
				SetAnim("待機");
			}
			vY = 0.0f;
			vX = 0.0f;

			return true;
		}
	}

	return false;
}

void Obj::UniqueColGround(void)
{
}

void Obj::PosAdjust(VECTOR2 &tmpPos, int maxEndPosY)
{
	// 地面に乗っている場合の座標補正
	if (animIdOld != animId)
	{
		if (locate == LOCATE::GROUND)
		{
			tmpPos.y = GROUND_POS_Y - maxEndPosY;
			vY = 0.0f;
			vX = 0.0f;
		}
	}

	// 浮いている床に乗っている場合の座標補正
	if ((locate == LOCATE::FLOOR) && (parentFloor.itrCnt != 99999))
	{
		auto floor = floorList->begin();
		for (int i = 0; i < parentFloor.itrCnt; i++)
		{
			floor++;
		}
		// 浮いている床が動いた座標だけﾌﾟﾚｲﾔの座標を動かす
		//tmpPos += (parentFloor.oldPos - (*itr)->GetPos());
		tmpPos.y = (*floor)->GetPos().y - maxEndPosY;
		tmpPos.x += (*floor)->GetPos().x - parentFloor.oldPos.x;
		vY = 0.0f;
		vX = 0.0f;
		//tmpPos.y = (*itr)->GetPos().y - maxEndPosY;
		parentFloor.itrCnt = 99999;	// ｴﾗｰｶｳﾝﾄを入れておく
	}
}

void Obj::SetNextAnim(bool animEndFlag)
{
	if (!animEndFlag)
	{
		return;
	}
	if (GetAnim() == "ジャンプ始まり")
	{
		SetAnim("ジャンプ中");
	}

	// 攻撃系ｱﾆﾒｰｼｮﾝが終了した時
	if (animPtnTbl[GetAnim()] != ANIM_TYPE_NO_ATTACK)
	{
		// 溜め始まりｱﾆﾒｰｼｮﾝが終了したら溜め中ｱﾆﾒｰｼｮﾝへ移行
		if (GetAnim() == "溜め始まり")
		{
			SetAnim("溜め中");
		}
		else if (GetAnim() == "通常攻撃_下始まり")
		{
			(locate == LOCATE::NON) ? SetAnim("通常攻撃_下中_空中") : SetAnim("通常攻撃_下中_地上");
		}
		// 通常攻撃系ｱﾆﾒｰｼｮﾝが終了したら待機ｱﾆﾒｰｼｮﾝへ移行
		else
		{
			fixCtrlCnt = FIX_TIME;
			// 攻撃ｱﾆﾒｰｼｮﾝ終了時に連撃ﾌﾗｸﾞが立っているか
			std::string str;
			switch (deri)
			{
			case DERI_NOMAL:
				deri = DERI_NON;
				fixCtrlCnt = 0;
				multiAttackNum++;
				str = "通常攻撃" + std::to_string(multiAttackNum + 1);
				SetAnim(str);
				break;

			case DERI_UP:
				deri = DERI_NON;
				fixCtrlCnt = 0;
				SetAnim("通常攻撃_上");
				multiAttackNum = 0;
				break;

			case DERI_DOWN:
				deri = DERI_NON;
				fixCtrlCnt = 0;
				SetAnim("通常攻撃_下始まり");
				multiAttackNum = 0;
				break;

			case DERI_NON:
			default:
				deri = DERI_NON;
				if (GetAnim() == "ダメージ")
				{
					fixCtrlCnt = 0;
					if (!spAttackHitFlag)
					{
						SetAnim("待機");
					}
				}
				else
				{
					SetAnim("待機");
				}
				multiAttackNum = 0;
				break;
			}

			if (multiAttackNum > 2)
			{
				fixCtrlCnt = 0;
				multiAttackNum = 0;	// 3連撃で終了
				SetAnim("待機");
			}

			chargeStage = CHARGE_NON;
			gravity = 0.4f;
		}
	}
}

void Obj::MoveEndUpDate(VECTOR2 tmpPos, const GameController &controller, int maxEndPosY)
{
	// 画面外に行かないように
	PosOutOfRangeCtrl(tmpPos, maxEndPosY);

	if (vY > 0.0f)
	{
		locate = LOCATE::NON;
	}

	// 座標移動
	pos = tmpPos;

	if (locate == LOCATE::NON)
	{
		auto anim = GetAnim();
		if (anim == "走る" || anim == "待機" || anim == "歩く")
		{
			SetAnim("ジャンプ中");
		}
	}

	SetNextAnim(animEndFlag);

	FlyUpDate(controller);	// 傘浮遊中の状態更新
	FlyTextUpDate();		// ﾌﾗｲﾃｷｽﾄの状態更新
	HitEffectUpDate();		// ﾋｯﾄｴﾌｪｸﾄの状態更新
	PlStateUpDate();		// plStateListの状態更新
	
	PlaySe();				//Seの再生

	if (GetAnim() == "必殺技")
	{
		spSeCnt++;
	}
	else
	{
		spSeCnt = 0;
	}
	PlaySPSe();				// 必殺技Seの再生
}

int Obj::GetSpAttackTimeMax(void)
{
	return 0;
}

void Obj::PosOutOfRangeCtrl(VECTOR2& pos, int maxEndPosY)
{
	// 画面外に行かないように
	auto screenSize = lpSceneMng.GetScreenSize();
	if (pos.x < 0)
	{
		pos.x = 0;
	}
	if (pos.x > screenSize.x)
	{
		pos.x = screenSize.x;
	}
	if (pos.y < 0)
	{
		pos.y = 0;
	}
	if (pos.y > GROUND_POS_Y - maxEndPosY)
	{
		pos.y = GROUND_POS_Y - maxEndPosY;
	}
}

void Obj::FlyUpDate(const GameController &controller)
{
	if (gravity == 0)
	{
		// 傘浮遊で着地した場合
		if (locate != LOCATE::NON)
		{
			gravity = 0.4f;		// 重力を0.4に戻す
			SetAnim("待機");
		}
		// 傘浮遊中に攻撃ｷｰ入力があった場合は重力を0.4に戻す
		if (
			controller.GetInputDown(GPAD_INPUT_B, padNum)	// 必殺技
			|| controller.GetInputDown(GPAD_INPUT_X, padNum)	// 通常攻撃
			|| controller.GetInputDown(GPAD_INPUT_Y, padNum)	// 強攻撃
			|| controller.GetInputDown(GPAD_INPUT_A, padNum)	// ｼﾞｬﾝﾌﾟ
			)
		{
			gravity = 0.4f;		//重力を0.4に戻す
		}
		// 傘浮遊中に↓ｷｰ入力があった場合は重力を0.4に戻し、ｼﾞｬﾝﾌﾟ中ｱﾆﾒｰｼｮﾝへ移行
		if (controller.GetInputDown(PAD_INPUT_DOWN, padNum))
		{
			gravity = 0.4f;
			SetAnim("ジャンプ中");
		}
	}
}

void Obj::FlyTextUpDate(void)
{
	if (flyTextList.size())
	{
		for (auto itr = flyTextList.begin(); itr != flyTextList.end();)
		{
			itr->cnt--;
			itr->insPos.y -= 1;
			if (itr->cnt < 0)
			{
				itr = flyTextList.erase(itr);
			}
			else
			{
				itr++;
			}
		}
	}
}

void Obj::HitEffectUpDate(void)
{
	// ｶｳﾝﾀｰ更新
	if (hitEffect.cnt > 0)
	{
		hitEffect.cnt--;
	}

	// ここから下は新しいﾋｯﾄｴﾌｪｸﾄをlistに追加する処理
	if (CheckDamageSe() == DMG_MAX)
	{
		return;		// ﾀﾞﾒｰｼﾞを受けていないならreturn
	}

	HIT_EFFECT data;
	// 扇形に生成する
	// -120°から-60°(-2.10 ～ -1.05)の間でﾗﾝﾀﾞﾑな数字を取得する 
	int length = -((rand() % (105 + 1)) + 105);			// -210～-105を取得
	data.rota = static_cast<double>(length) / 100.0;	// -2.10～-1.05を取得
														// 生成座標の決定
	data.insPos.x += static_cast<int>(100 * cos(data.rota) + pos.x);
	data.insPos.y += static_cast<int>(100 * sin(data.rota) + pos.y);

	data.cnt = 60;
	data.damageRate = CheckDamageSe();

	// ﾃﾞｰﾀ更新
	hitEffect = data;
}

void Obj::PlStateUpDate(void)
{
	// ﾘｽﾄに追加するﾗﾑﾀﾞ
	auto listPush = [&](PL_STATE state)
	{
		bool flag = true;
		for (auto data : plStateList)
		{
			if (data == state)
			{
				flag = false;
			}
		}
		if (flag)
		{
			// listの中に追加PL_STATE_SEWATがないなら追加する
			plStateList.push_back(state);
		}
	};

	// ﾘｽﾄから削除するﾗﾑﾀﾞ
	auto listErase = [&](PL_STATE state)
	{
		for (auto itr = plStateList.begin(); itr != plStateList.end(); itr++)
		{
			if (*itr == state)
			{
				if (itr == plStateItr)
				{
					plStateItr = plStateList.erase(itr);
					if (plStateItr == plStateList.end())
					{
						plStateItr = plStateList.begin();
					}
				}
				else
				{
					plStateList.erase(itr);
				}
				plStateCnt = 30;
				break;
			}
		}
	};

	// ﾘｽﾄに追加したり削除したり
	if (hp <= HP_RED_COLOR_NUM)
	{
		listPush(PL_STATE_SEWAT);
	}
	angryFlag ? listPush(PL_STATE_ANGRY) : listErase(PL_STATE_ANGRY);
	specialPoint >= 100 ? listPush(PL_STATE_SPECIAL) : listErase(PL_STATE_SPECIAL);


	if (plStateList.size() == 1)
	{
		plStateItr = plStateList.begin();
	}

	// ｱｲｺﾝが2個以上の時
	if (plStateList.size() >= 2)
	{
		plStateCnt--;
		if (plStateCnt <= 0)
		{
			plStateCnt = 30;
			auto testItr = plStateItr;
			testItr++;
			if (testItr == plStateList.end())
			{
				plStateItr = plStateList.begin();
			}
			else
			{
				plStateItr++;
			}
		}
	}
}

void Obj::AngryGaugeDraw(void)
{
	// 画面下部ｷｬﾗ情報の描画(デバッグコードですー)
	std::string iconName = "image/" + charName + "/アイコン/" + "icon_play.png";
	std::string iconNameRed = "image/" + charName + "/アイコン/" + "iconRed.png";

	// ｷｬﾗｱｲｺﾝの描画
	VECTOR2 iconSize(163, 150);
	float iconStdSize = 150.0f / 100.0f;
	int localAngryPoint = static_cast<int>(angryFlag ? 100 : angryPoint);

	DrawGraph(320 * padNum , 565, lpImageMng.GetID(iconName)[0], true);

	// ｷｬﾗｱｲｺﾝ怒りUI
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	int startPosFromlength = static_cast<int>(iconSize.y - iconStdSize * localAngryPoint);

	DrawRectGraph(
		320 * padNum, 565 + startPosFromlength,
		0, startPosFromlength,
		iconSize.x, iconSize.y - startPosFromlength,
		lpImageMng.GetID(iconNameRed)[0], true, false);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 128);
}

void Obj::SpecialGaugeDraw(bool cutInFlag)
{
	float gaugeStdSize = static_cast<float>(SPECIAL_GAUGE_SIZE_X) / 100.0f;
	VECTOR2 startPos = VECTOR2(120 + 320 * padNum + 8, 660 + 4);
	int gaugeColor = spGaugeColorTbl[(specialPoint / 20) % spGaugeColorTbl.size()];

	// 白部分の描画
	DrawBox(
		startPos,
		VECTOR2(startPos.x + SPECIAL_GAUGE_SIZE_X, startPos.y + SPECIAL_GAUGE_SIZE_Y),
		0xffffff, true);

	// 色付き部分の描画
	if (specialPoint >= 100)
	{
		DrawRectGraph(
			startPos.x, startPos.y,
			320 - (framCnt*5%320),0,
			SPECIAL_GAUGE_SIZE_X, SPECIAL_GAUGE_SIZE_Y,
			lpImageMng.GetID("image/ゲームシーン用/虹.png")[0],true,false);
	}
	else
	{
		DrawBox(
			startPos,
			VECTOR2(startPos.x + static_cast<int>(gaugeStdSize * specialPoint), startPos.y + SPECIAL_GAUGE_SIZE_Y),
			gaugeColor, true);
	}

	// ｶｯﾄｲﾝ中なら画面を暗く、そうでないなら明るく
	if (cutInFlag)
	{
		SetDrawBright(140, 140, 140);
	}
	else
	{
		SetDrawBright(255, 255, 255);
	}

	//// 必殺ゲージ枠
	//DrawGraph(130 + 320 * padNum, 660, lpImageMng.GetID("image/UI/specialGaugeFrame.png")[0], true);
	// 必殺ゲージ枠
	DrawGraph(120 + 320 * padNum, 660, lpImageMng.GetID("image/UI/specialGaugeFrame.png")[0], true);
}

void Obj::GuardDraw(void)
{
	if (GetAnim() != "ガード")
	{
		return;
	}

	// ｶﾞｰﾄﾞｴﾌｪｸﾄの表示
	int cnt = animCnt / 5;	// ++されるよー 
	DrawRectGraph(
		pos.x - 150 / 2, pos.y - 150 / 2,
		150 * (cnt % 5), 150 * (cnt / 5 % 2),
		150, 150,
		lpImageMng.GetID("image/UI/補助4.png")[0], true, false);

	// 丸い画像の表示
	float extRate = static_cast<float>(guardTime) / GetGuardTimeMax();
	DrawRotaGraph(pos, extRate, 0, guardImage, true, false);
}

void Obj::StateIconDraw(void)
{
	if (hp <= 0)
	{
		return;
	}

	// ｱｲｺﾝが1個の時はそのｱｲｺﾝを交互に描画
	if (plStateList.size() == 1)
	{
		std::string fName = "image/UI/" + plStateFileName[*plStateList.begin()];
		DrawRectGraph(
			pos.x + ((static_cast<int>(dir) * 2 - 1) * 28) - 32 / 2, pos.y - 50,
			((framCnt / 30) % 2) * 32, 0,
			32, 32, lpImageMng.GetID(fName)[0], true, static_cast<int>(dir) - 1);
	}
	// ｱｲｺﾝが2個以上の時
	if (plStateList.size() >= 2)
	{
		std::string fName = "image/UI/" + plStateFileName[*plStateItr];
		DrawRectGraph(
			pos.x + ((static_cast<int>(dir) * 2 - 1) * 28) - 32 / 2, pos.y - 50,
			0, 0,
			32, 32, lpImageMng.GetID(fName)[0], true, static_cast<int>(dir) - 1);
	}
}

void Obj::FlyTextDraw(void)
{
	if (hp <= 0)
	{
		return;
	}

	// ﾌﾗｲﾃｷｽﾄの描画
	if (flyTextList.size())
	{
		for (auto text : flyTextList)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>((255.0f / (float)FLY_TEXT_VANISH_CNT) * text.cnt));
			DrawFormatStringToHandle(text.insPos.x, text.insPos.y, 0xff0000, lpFontMng.SetFont("たぬき20縁"), "%d", text.damage);
		}
	}

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}

void Obj::CharacterDraw(std::vector<ColInf> collisonVec, bool moveStopFlag, CUT_IN_INFO cutInInfo)
{
	if (!((visible / 5) % 2) || cutInInfo.flag)
	{
		if (animId < IMAGE_ID(imageName).size())
		{
			auto offset = (*collisonVec.begin()).offset;
			// dir == LEFT なら当たり判定の向きを逆にする
			offset.x *= static_cast<int>(dir) * -2 + 1;
			VECTOR2 drawPos = pos + offset;

			if (angryFlag)
			{
				SetDrawBright(255, 0, 0);
			}
			if (spAttackCnt)
			{
				int rgb = HsvChangeRgb((framCnt * 10) % 360, 255, 255);
				int r = (rgb & 0xff0000) >> 16;
				int g = (rgb & 0x00ff00) >> 8;
				int b = (rgb & 0x0000ff);
				SetDrawBright(r, g, b);
			}

			if (GetAnim() == "死亡")
			{
				SetDrawBlendMode(DX_BLENDMODE_INVSRC, 255 - animCnt * 2);
				DrawRotaGraph(
					drawPos,
					1,
					0,
					IMAGE_ID(imageName)[animId],
					true,
					static_cast<int>(dir));
				SetDrawBlendMode(DX_BLENDMODE_ADD, 255 - animCnt * 2);
			}

			if (cutInInfo.flag)
			{
				if (cutInInfo.padNum == padNum)
				{
					SetDrawBright(255, 255, 255);
				}
				else
				{
					// ｶｯﾄｲﾝ中、必殺技発動中のｷｬﾗ以外は暗くする
					SetDrawBright(140, 140, 140);
				}
			}

			// 残像の描画
			for (auto &shadow : shadowList)
			{
				shadow.cnt--;
			}
			shadowList.remove_if([](SHADOW& shadow) {return shadow.cnt < 0; });
			if (angryFlag || spAttackCnt)
			{
				for (auto shadow : shadowList)
				{
					SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((shadow.cnt / 2) * 255) / SHADOW_DRAW_FRAM);

					DrawRotaGraph(
						shadow.pos + offset,
						1,
						0,
						IMAGE_ID(charName + "/" + shadow.animName)[shadow.animid],
						true,
						static_cast<int>(shadow.dir));
				}
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
			}

			DrawRotaGraph(
				drawPos,
				1,
				0,
				IMAGE_ID(imageName)[animId],
				true,
				static_cast<int>(dir));

			if (angryFlag || spAttackCnt)
			{
				SHADOW shadow = {
					pos,
					offset,
					animName,
					animId,
					dir,
					SHADOW_DRAW_FRAM
				};
				shadowList.push_back(shadow);
			}

			// ｶｯﾄｲﾝ中なら画面を暗く、そうでないなら明るく
			if (cutInInfo.flag)
			{
				SetDrawBright(140, 140, 140);
			}
			else
			{
				SetDrawBright(255, 255, 255);
			}
		}
	}
}

const VECTOR2 & Obj::GetPos(void)
{
	return pos;
}

const VECTOR2 Obj::GetPosOld(void)
{
	return posOld;
}

bool Obj::AddAnim(std::string animName, int inv, bool loop, std::string animFile)
{
	int frame = lpImageMng.GetSize(charName + "/" + animName);
	animTable[animName][ANIM_TBL_START_ID] = 0;			// 今回のObjｸﾗｽでは必ず0
	animTable[animName][ANIM_TBL_FRAME] = frame;
	animTable[animName][ANIM_TBL_INV] = inv;
	animTable[animName][ANIM_TBL_LOOP] = loop;
	animFileName[animName] = animFile;
	return true;
}


bool Obj::SetAnim(std::string animName)
{
	// ﾃﾞｰﾀが存在していればﾒﾝﾊﾞ変数のanimNameに引数をｾｯﾄし、していなければfalseを返す
	if (Obj::animName == animName)
	{
		return true;
	}
	if (animTable.find(animName) == animTable.end())
	{
		return false;
	}

	animEndFlag = false;
	Obj::animName = animName;
	Obj::imageName = charName + "/" + animName;
	Obj::animCnt = 0;
	animId = 0;

	return true;
}

void Obj::AddSpecialPoint(int specialPoint)
{
	this->specialPoint += specialPoint;
	if (this->specialPoint > 100)
	{
		this->specialPoint = 100;
	}
}

void Obj::AddAngryPoint(int angryPoint)
{
	if (!angryFlag)
	{
		this->angryPoint += angryPoint;
		if (this->angryPoint > 100.0f)
		{
			this->angryPoint = 100.0f;
		}
	}

	if (this->angryPoint < 0.0f)
	{
		this->angryPoint = 0.0f;
	}
}

void Obj::AddChickenKillCnt(void)
{
	resultInfo.chickenKillCnt[padNum]++;
	return;
}

const std::string Obj::GetAnim(void)
{
	return animName;
}

const std::string Obj::GetAnim(ANIM_STATE type)
{
	// 1ﾌﾚｰﾑ前
	if (type == ANIM_OLD)
	{
		return animNameOld;
	}

	// 現在のﾌﾚｰﾑ
	return animName;
}

const std::vector<ColInf> Obj::GetPlayerCollision(void)
{
	return lpColMng.GetCollider(charName, animName, animId);
}

const DIR Obj::GetDir(void)
{
	return dir;
}

const int Obj::GetHp(void)
{
	return hp;
}

const int Obj::GetAnimId(void)
{
	return animId;
}

const int Obj::GetAnimFrame(void)
{
	return lpImageMng.GetSize(imageName);
}

const VECTOR2 Obj::GetFlyHighPower(void)
{  
	return  VECTOR2(FLY_HIGH_POWER.x * (static_cast<int>(dir) * -2 + 1) , FLY_HIGH_POWER.y);
}

const std::string Obj::GetCharName(void)
{
	return charName;
}

const RESULT_INFO Obj::GetResultInfo(void)
{
	return resultInfo;
}

const bool Obj::CheckSpAttackCnt(void)
{
	return static_cast<bool>(spAttackCnt);
}