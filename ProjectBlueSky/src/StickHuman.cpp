#include "SceneMng.h"
#include "StickHuman.h"


StickHuman::StickHuman(VECTOR2 pos, VECTOR2 offset, PAD_ID id, DIR charaDir) : Character(offset)
{

	spAttackAnimName[0] = "技1";
	spAttackAnimName[1] = "技2";
	spAttackAnimName[2] = "技3";
	spAttackAnimFileName[0] = "waza_1";
	spAttackAnimFileName[1] = "waza_2";
	spAttackAnimFileName[2] = "waza_3";
	shotCreateCnt = 0;

	switch (lpSceneMng.GetCharID(id))
	{
	case 0:
		characterName = "棒人間";
		AddAnim("波動", 0, 0, 11, 4, false, 0, 0);
		spAttackAnimName[0] = "波動";
		spAttackAnimFileName[0] = "hadou";
		animAttributeTbl["波動"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["波動"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["波動"][2] = ANIM_ATTRIBUTE_SHOT;
		shotCreateCnt = 30;

		spAttackCommand[0][DIR_RIGHT].resize(4);
		spAttackCommand[0][DIR_RIGHT] = { SP_COM_DOWN, SP_COM_RIGHT_DOWN, SP_COM_RIGHT, SP_COM_PUNCH };
		spAttackCommand[0][DIR_LEFT].resize(4);
		spAttackCommand[0][DIR_LEFT] = { SP_COM_DOWN, SP_COM_LEFT_DOWN, SP_COM_LEFT, SP_COM_PUNCH };

		AddAnim("百裂脚", 0, 0, 18, 4, false, 0, 0);
		spAttackAnimName[1] = "百裂脚";
		spAttackAnimFileName[1] = "hyakuretsu";
		animAttributeTbl["百裂脚"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["百裂脚"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["百裂脚"][2] = ANIM_ATTRIBUTE_NON;

		spAttackCommand[1][DIR_RIGHT].resize(8);
		spAttackCommand[1][DIR_RIGHT] = { SP_COM_CENTER, SP_COM_KICK, SP_COM_CENTER, SP_COM_KICK, SP_COM_CENTER, SP_COM_KICK, SP_COM_CENTER, SP_COM_KICK };
		spAttackCommand[1][DIR_LEFT].resize(8);
		spAttackCommand[1][DIR_LEFT] = { SP_COM_CENTER, SP_COM_KICK, SP_COM_CENTER, SP_COM_KICK, SP_COM_CENTER, SP_COM_KICK, SP_COM_CENTER, SP_COM_KICK };

		AddAnim("昇竜", 0, 0, 16, 4, false, 0, 0);
		spAttackAnimName[2] = "昇竜";
		spAttackAnimFileName[2] = "shoryu";
		animAttributeTbl["昇竜"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["昇竜"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["昇竜"][2] = ANIM_ATTRIBUTE_INVINCIBLE;

		spAttackCommand[2][DIR_RIGHT].resize(4);
		spAttackCommand[2][DIR_RIGHT] = { SP_COM_RIGHT, SP_COM_DOWN, SP_COM_RIGHT_DOWN, SP_COM_PUNCH };
		spAttackCommand[2][DIR_LEFT].resize(4);
		spAttackCommand[2][DIR_LEFT] = { SP_COM_LEFT, SP_COM_DOWN, SP_COM_LEFT_DOWN, SP_COM_PUNCH };
		break;

	case 1:
		characterName = "棒人間_黒";
		AddAnim("刺突砲", 0, 0, 11, 4, false, 0, 0);
		spAttackAnimName[0] = "刺突砲";
		spAttackAnimFileName[0] = "sitotsu";
		animAttributeTbl["刺突砲"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["刺突砲"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["刺突砲"][2] = ANIM_ATTRIBUTE_SHOT;
		shotCreateCnt = 30;

		spAttackCommand[0][DIR_RIGHT].resize(3);
		spAttackCommand[0][DIR_RIGHT] = { SP_COM_PUNCH, SP_COM_ACCUMULATE, SP_COM_CENTER };
		spAttackCommand[0][DIR_LEFT].resize(3);
		spAttackCommand[0][DIR_LEFT] = { SP_COM_PUNCH, SP_COM_ACCUMULATE, SP_COM_CENTER };

		AddAnim("ウマキック", 0, 0, 7, 4, false, 0, 0);
		spAttackAnimName[1] = "ウマキック";
		spAttackAnimFileName[1] = "horseKick";
		animAttributeTbl["ウマキック"][0] = ANIM_ATTRIBUTE_SQUAT;
		animAttributeTbl["ウマキック"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["ウマキック"][2] = ANIM_ATTRIBUTE_NON;

		spAttackCommand[1][DIR_RIGHT].resize(4);
		spAttackCommand[1][DIR_RIGHT] = { SP_COM_LEFT, SP_COM_LEFT_DOWN, SP_COM_DOWN, SP_COM_KICK };
		spAttackCommand[1][DIR_LEFT].resize(4);
		spAttackCommand[1][DIR_LEFT] = { SP_COM_RIGHT, SP_COM_RIGHT_DOWN, SP_COM_DOWN, SP_COM_KICK };

		AddAnim("ローリングアタック", 0, 0, 6, 4, false, 0, 0);
		spAttackAnimName[2] = "ローリングアタック";
		spAttackAnimFileName[2] = "rolling";
		animAttributeTbl["ローリングアタック"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["ローリングアタック"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["ローリングアタック"][2] = ANIM_ATTRIBUTE_NON;

		spAttackCommand[2][DIR_RIGHT].resize(5);
		spAttackCommand[2][DIR_RIGHT] = { SP_COM_LEFT, SP_COM_ACCUMULATE, SP_COM_CENTER, SP_COM_RIGHT, SP_COM_PUNCH };
		spAttackCommand[2][DIR_LEFT].resize(5);
		spAttackCommand[2][DIR_LEFT] = { SP_COM_RIGHT, SP_COM_ACCUMULATE, SP_COM_CENTER, SP_COM_LEFT, SP_COM_PUNCH };
		break;

	case 2:
		characterName = "棒人間_赤";
		AddAnim("気功拳", 0, 0, 9, 4, false, 0, 0);
		spAttackAnimName[0] = "気功拳";
		spAttackAnimFileName[0] = "kikoh";
		animAttributeTbl["気功拳"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["気功拳"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["気功拳"][2] = ANIM_ATTRIBUTE_SHOT;
		shotCreateCnt = 30;

		spAttackCommand[0][DIR_RIGHT].resize(4);
		spAttackCommand[0][DIR_RIGHT] = { SP_COM_DOWN, SP_COM_RIGHT_DOWN, SP_COM_RIGHT, SP_COM_PUNCH };
		spAttackCommand[0][DIR_LEFT].resize(4);
		spAttackCommand[0][DIR_LEFT] = { SP_COM_DOWN, SP_COM_LEFT_DOWN, SP_COM_LEFT, SP_COM_PUNCH };

		AddAnim("旋風脚", 0, 0, 4, 4, true, 0, 0);
		spAttackAnimName[1] = "旋風脚";
		spAttackAnimFileName[1] = "senpu";
		animAttributeTbl["旋風脚"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["旋風脚"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["旋風脚"][2] = ANIM_ATTRIBUTE_NON;

		spAttackCommand[1][DIR_RIGHT].resize(4);
		spAttackCommand[1][DIR_RIGHT] = { SP_COM_DOWN, SP_COM_LEFT_DOWN, SP_COM_LEFT, SP_COM_KICK };
		spAttackCommand[1][DIR_LEFT].resize(4);
		spAttackCommand[1][DIR_LEFT] = { SP_COM_DOWN, SP_COM_RIGHT_DOWN, SP_COM_RIGHT, SP_COM_KICK };

		AddAnim("カンフーキック", 0, 0, 17, 4, false, 0, 0);
		spAttackAnimName[2] = "カンフーキック";
		spAttackAnimFileName[2] = "kung";
		animAttributeTbl["カンフーキック"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["カンフーキック"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["カンフーキック"][2] = ANIM_ATTRIBUTE_INVINCIBLE;

		spAttackCommand[2][DIR_RIGHT].resize(4);
		spAttackCommand[2][DIR_RIGHT] = { SP_COM_RIGHT, SP_COM_DOWN, SP_COM_RIGHT_DOWN, SP_COM_KICK };
		spAttackCommand[2][DIR_LEFT].resize(4);
		spAttackCommand[2][DIR_LEFT] = { SP_COM_LEFT, SP_COM_DOWN, SP_COM_LEFT_DOWN, SP_COM_KICK };
		break;

	case 3:
		characterName = "棒人間_青";
		AddAnim("かめはめ波", 0, 0, 10, 4, false, 0, 0);
		spAttackAnimName[0] = "かめはめ波";
		spAttackAnimFileName[0] = "kamehame";
		animAttributeTbl["かめはめ波"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["かめはめ波"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["かめはめ波"][2] = ANIM_ATTRIBUTE_SHOT;
		shotCreateCnt = 30;

		spAttackCommand[0][DIR_RIGHT].resize(3);
		spAttackCommand[0][DIR_RIGHT] = { SP_COM_PUNCH, SP_COM_ACCUMULATE, SP_COM_CENTER };
		spAttackCommand[0][DIR_LEFT].resize(3);
		spAttackCommand[0][DIR_LEFT] = { SP_COM_PUNCH, SP_COM_ACCUMULATE, SP_COM_CENTER };

		AddAnim("ラッシュ", 0, 0, 15, 4, false, 0, 0);
		spAttackAnimName[1] = "ラッシュ";
		spAttackAnimFileName[1] = "rush";
		animAttributeTbl["ラッシュ"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["ラッシュ"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["ラッシュ"][2] = ANIM_ATTRIBUTE_NON;

		spAttackCommand[1][DIR_RIGHT].resize(8);
		spAttackCommand[1][DIR_RIGHT] = { SP_COM_CENTER, SP_COM_PUNCH, SP_COM_CENTER, SP_COM_PUNCH, SP_COM_CENTER, SP_COM_PUNCH, SP_COM_CENTER, SP_COM_PUNCH };
		spAttackCommand[1][DIR_LEFT].resize(8);
		spAttackCommand[1][DIR_LEFT] = { SP_COM_CENTER, SP_COM_PUNCH, SP_COM_CENTER, SP_COM_PUNCH, SP_COM_CENTER, SP_COM_PUNCH, SP_COM_CENTER, SP_COM_PUNCH };

		AddAnim("回し蹴り", 0, 0, 26, 4, false, 0, 0);
		spAttackAnimName[2] = "回し蹴り";
		spAttackAnimFileName[2] = "roundKick";
		animAttributeTbl["回し蹴り"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["回し蹴り"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["回し蹴り"][2] = ANIM_ATTRIBUTE_INVINCIBLE;

		spAttackCommand[2][DIR_RIGHT].resize(6);
		spAttackCommand[2][DIR_RIGHT] = { SP_COM_LEFT, SP_COM_LEFT_DOWN, SP_COM_DOWN, SP_COM_RIGHT_DOWN, SP_COM_RIGHT, SP_COM_KICK };
		spAttackCommand[2][DIR_LEFT].resize(6);
		spAttackCommand[2][DIR_LEFT] = { SP_COM_RIGHT, SP_COM_RIGHT_DOWN, SP_COM_DOWN, SP_COM_LEFT_DOWN, SP_COM_LEFT, SP_COM_KICK };
		break;

	case 4:
		characterName = "棒人間_緑";
		AddAnim("ソニックブーム", 0, 0, 7, 4, false, 0, 0);
		spAttackAnimName[0] = "ソニックブーム";
		spAttackAnimFileName[0] = "sonic";
		animAttributeTbl["ソニックブーム"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["ソニックブーム"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["ソニックブーム"][2] = ANIM_ATTRIBUTE_SHOT;
		shotCreateCnt = 22;

		spAttackCommand[0][DIR_RIGHT].resize(4);
		spAttackCommand[0][DIR_RIGHT] = { SP_COM_DOWN, SP_COM_RIGHT_DOWN, SP_COM_RIGHT, SP_COM_PUNCH };
		spAttackCommand[0][DIR_LEFT].resize(4);
		spAttackCommand[0][DIR_LEFT] = { SP_COM_DOWN, SP_COM_LEFT_DOWN, SP_COM_LEFT, SP_COM_PUNCH };

		AddAnim("ミサイルアロー", 0, 0, 6, 4, false, 0, 0);
		spAttackAnimName[1] = "ミサイルアロー";
		spAttackAnimFileName[1] = "missile";
		animAttributeTbl["ミサイルアロー"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["ミサイルアロー"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["ミサイルアロー"][2] = ANIM_ATTRIBUTE_NON;

		spAttackCommand[1][DIR_RIGHT].resize(5);
		spAttackCommand[1][DIR_RIGHT] = { SP_COM_LEFT, SP_COM_ACCUMULATE, SP_COM_CENTER, SP_COM_RIGHT, SP_COM_PUNCH };
		spAttackCommand[1][DIR_LEFT].resize(5);
		spAttackCommand[1][DIR_LEFT] = { SP_COM_RIGHT, SP_COM_ACCUMULATE, SP_COM_CENTER, SP_COM_LEFT, SP_COM_PUNCH };

		AddAnim("ランキャク", 0, 0, 10, 4, false, 0, 0);
		spAttackAnimName[2] = "ランキャク";
		spAttackAnimFileName[2] = "cutterKick";
		animAttributeTbl["ランキャク"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["ランキャク"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["ランキャク"][2] = ANIM_ATTRIBUTE_INVINCIBLE;

		spAttackCommand[2][DIR_RIGHT].resize(4);
		spAttackCommand[2][DIR_RIGHT] = { SP_COM_RIGHT, SP_COM_DOWN, SP_COM_RIGHT_DOWN, SP_COM_KICK };
		spAttackCommand[2][DIR_LEFT].resize(4);
		spAttackCommand[2][DIR_LEFT] = { SP_COM_LEFT, SP_COM_DOWN, SP_COM_LEFT_DOWN, SP_COM_KICK };
		break;

	case 5:
		characterName = "棒人間_黄";
		AddAnim("岩投げ", 0, 0, 16, 4, false, 0, 0);
		spAttackAnimName[0] = "岩投げ";
		spAttackAnimFileName[0] = "throw";
		animAttributeTbl["岩投げ"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["岩投げ"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["岩投げ"][2] = ANIM_ATTRIBUTE_SHOT;
		shotCreateCnt = 45;

		spAttackCommand[0][DIR_RIGHT].resize(3);
		spAttackCommand[0][DIR_RIGHT] = { SP_COM_PUNCH, SP_COM_ACCUMULATE, SP_COM_CENTER };
		spAttackCommand[0][DIR_LEFT].resize(3);
		spAttackCommand[0][DIR_LEFT] = { SP_COM_PUNCH, SP_COM_ACCUMULATE, SP_COM_CENTER };

		AddAnim("地面割", 0, 0, 15, 4, false, 0, 0);
		spAttackAnimName[1] = "地面割";
		spAttackAnimFileName[1] = "split";
		animAttributeTbl["地面割"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["地面割"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["地面割"][2] = ANIM_ATTRIBUTE_NON;

		spAttackCommand[1][DIR_RIGHT].resize(3);
		spAttackCommand[1][DIR_RIGHT] = { SP_COM_DOWN, SP_COM_ACCUMULATE, SP_COM_PUNCH };
		spAttackCommand[1][DIR_LEFT].resize(3);
		spAttackCommand[1][DIR_LEFT] = { SP_COM_DOWN, SP_COM_ACCUMULATE, SP_COM_PUNCH };

		AddAnim("タックル", 0, 0, 7, 4, false, 0, 0);
		spAttackAnimName[2] = "タックル";
		spAttackAnimFileName[2] = "tackle";
		animAttributeTbl["タックル"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["タックル"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["タックル"][2] = ANIM_ATTRIBUTE_NON;

		spAttackCommand[2][DIR_RIGHT].resize(5);
		spAttackCommand[2][DIR_RIGHT] = { SP_COM_LEFT, SP_COM_ACCUMULATE, SP_COM_CENTER, SP_COM_RIGHT, SP_COM_PUNCH };
		spAttackCommand[2][DIR_LEFT].resize(5);
		spAttackCommand[2][DIR_LEFT] = { SP_COM_RIGHT, SP_COM_ACCUMULATE, SP_COM_CENTER, SP_COM_LEFT, SP_COM_PUNCH };
		break;

	case 6:
		characterName = "棒人間_ピンク";
		AddAnim("サイコカッター", 0, 0, 9, 4, false, 0, 0);
		spAttackAnimName[0] = "サイコカッター";
		spAttackAnimFileName[0] = "psycho";
		animAttributeTbl["サイコカッター"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["サイコカッター"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["サイコカッター"][2] = ANIM_ATTRIBUTE_SHOT;
		shotCreateCnt = 30;

		spAttackCommand[0][DIR_RIGHT].resize(3);
		spAttackCommand[0][DIR_RIGHT] = { SP_COM_PUNCH, SP_COM_ACCUMULATE, SP_COM_CENTER };
		spAttackCommand[0][DIR_LEFT].resize(3);
		spAttackCommand[0][DIR_LEFT] = { SP_COM_PUNCH, SP_COM_ACCUMULATE, SP_COM_CENTER };

		AddAnim("かかと落とし", 0, 0, 14, 4, false, 0, 0);
		spAttackAnimName[1] = "かかと落とし";
		spAttackAnimFileName[1] = "heeldrop";
		animAttributeTbl["かかと落とし"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["かかと落とし"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["かかと落とし"][2] = ANIM_ATTRIBUTE_NON;

		spAttackCommand[1][DIR_RIGHT].resize(4);
		spAttackCommand[1][DIR_RIGHT] = { SP_COM_RIGHT, SP_COM_RIGHT_DOWN, SP_COM_DOWN, SP_COM_KICK };
		spAttackCommand[1][DIR_LEFT].resize(4);
		spAttackCommand[1][DIR_LEFT] = { SP_COM_LEFT, SP_COM_LEFT_DOWN, SP_COM_DOWN, SP_COM_KICK };
		break;

		AddAnim("ワープ", 0, 0, 11, 4, false, 0, 0);
		spAttackAnimName[2] = "ワープ";
		spAttackAnimFileName[2] = "warp";
		animAttributeTbl["ワープ"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["ワープ"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["ワープ"][2] = ANIM_ATTRIBUTE_INVINCIBLE;

		spAttackCommand[2][DIR_RIGHT].resize(7);
		spAttackCommand[2][DIR_RIGHT] = { SP_COM_DOWN, SP_COM_RIGHT_DOWN, SP_COM_RIGHT, SP_COM_DOWN, SP_COM_RIGHT_DOWN, SP_COM_RIGHT, SP_COM_KICK };
		spAttackCommand[2][DIR_LEFT].resize(7);
		spAttackCommand[2][DIR_LEFT] = { SP_COM_DOWN, SP_COM_LEFT_DOWN, SP_COM_LEFT, SP_COM_DOWN, SP_COM_LEFT_DOWN, SP_COM_LEFT, SP_COM_KICK };

	case 7:
		characterName = "棒人間_水色";
		AddAnim("竜の息吹", 0, 0, 12, 4, false, 0, 0);
		spAttackAnimName[0] = "竜の息吹";
		spAttackAnimFileName[0] = "ibuki";
		animAttributeTbl["竜の息吹"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["竜の息吹"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["竜の息吹"][2] = ANIM_ATTRIBUTE_SHOT;
		shotCreateCnt = 30;

		spAttackCommand[0][DIR_RIGHT].resize(3);
		spAttackCommand[0][DIR_RIGHT] = { SP_COM_PUNCH, SP_COM_ACCUMULATE, SP_COM_CENTER };
		spAttackCommand[0][DIR_LEFT].resize(3);
		spAttackCommand[0][DIR_LEFT] = { SP_COM_PUNCH, SP_COM_ACCUMULATE, SP_COM_CENTER };

		AddAnim("ソーラン", 0, 0, 13, 4, false, 0, 0);
		spAttackAnimName[1] = "ソーラン";
		spAttackAnimFileName[1] = "soran";
		animAttributeTbl["ソーラン"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["ソーラン"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["ソーラン"][2] = ANIM_ATTRIBUTE_NON;

		spAttackCommand[1][DIR_RIGHT].resize(3);
		spAttackCommand[1][DIR_RIGHT] = { SP_COM_DOWN, SP_COM_RIGHT_DOWN, SP_COM_RIGHT, SP_COM_PUNCH };
		spAttackCommand[1][DIR_LEFT].resize(3);
		spAttackCommand[1][DIR_LEFT] = { SP_COM_DOWN, SP_COM_LEFT_DOWN, SP_COM_LEFT, SP_COM_PUNCH };

		AddAnim("アクセル", 0, 0, 21, 4, false, 0, 0);
		spAttackAnimName[2] = "アクセル";
		spAttackAnimFileName[2] = "accele";
		animAttributeTbl["アクセル"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["アクセル"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["アクセル"][2] = ANIM_ATTRIBUTE_NON;

		spAttackCommand[2][DIR_RIGHT].resize(4);
		spAttackCommand[2][DIR_RIGHT] = { SP_COM_DOWN, SP_COM_LEFT_DOWN, SP_COM_LEFT, SP_COM_KICK };
		spAttackCommand[2][DIR_LEFT].resize(4);
		spAttackCommand[2][DIR_LEFT] = { SP_COM_DOWN, SP_COM_RIGHT_DOWN, SP_COM_RIGHT, SP_COM_KICK };
		break;

	default:
		break;
	}

	Init("image/" + characterName + "/待機/stand_0.png", VECTOR2(256, 256), VECTOR2(1, 1), VECTOR2(pos.x, pos.y), false, id);

	dir = charaDir;
}

StickHuman::~StickHuman()
{
}
