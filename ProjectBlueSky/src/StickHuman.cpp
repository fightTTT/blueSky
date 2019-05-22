#include "SceneMng.h"
#include "StickHuman.h"


StickHuman::StickHuman(VECTOR2 pos, VECTOR2 offset, PAD_ID id, DIR charaDir) : Character(offset)
{
	switch (lpSceneMng.GetCharID(id))
	{
	case 0:
		characterName = "棒人間";
		AddAnim("波動", 0, 0, 11, 4, false, 0, 0);
		spAttackAnimName[0] = "波動";
		spAttackAnimFileName[0] = "hadou";
		animAttributeTbl["波動"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["波動"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["波動"][2] = ANIM_ATTRIBUTE_NON;

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
		//spAttackType[0] = SKILL_TYPE_PUNCH;
		animAttributeTbl["刺突砲"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["刺突砲"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["刺突砲"][2] = ANIM_ATTRIBUTE_NON;

		spAttackCommand[0][DIR_RIGHT].resize(3);
		spAttackCommand[0][DIR_RIGHT] = { SP_COM_DOWN, SP_COM_RIGHT_DOWN, SP_COM_RIGHT };
		spAttackCommand[0][DIR_LEFT].resize(3);
		spAttackCommand[0][DIR_LEFT] = { SP_COM_DOWN, SP_COM_LEFT_DOWN, SP_COM_LEFT };

		AddAnim("ウマキック", 0, 0, 7, 4, false, 0, 0);
		spAttackAnimName[1] = "ウマキック";
		spAttackAnimFileName[1] = "horseKick";
		//spAttackType[1] = SKILL_TYPE_KICK;
		animAttributeTbl["ウマキック"][0] = ANIM_ATTRIBUTE_SQUAT;
		animAttributeTbl["ウマキック"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["ウマキック"][2] = ANIM_ATTRIBUTE_NON;

		spAttackCommand[1][DIR_RIGHT].resize(3);
		spAttackCommand[1][DIR_RIGHT] = { SP_COM_LEFT, SP_COM_LEFT_DOWN, SP_COM_DOWN };
		spAttackCommand[1][DIR_LEFT].resize(3);
		spAttackCommand[1][DIR_LEFT] = { SP_COM_RIGHT, SP_COM_RIGHT_DOWN, SP_COM_DOWN };

		AddAnim("ローリングアタック", 0, 0, 6, 4, false, 0, 0);
		spAttackAnimName[2] = "ローリングアタック";
		spAttackAnimFileName[2] = "rolling";
		//spAttackType[2] = SKILL_TYPE_PUNCH;
		animAttributeTbl["ローリングアタック"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["ローリングアタック"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["ローリングアタック"][2] = ANIM_ATTRIBUTE_NON;

		spAttackCommand[2][DIR_RIGHT].resize(4);
		spAttackCommand[2][DIR_RIGHT] = { SP_COM_LEFT, SP_COM_ACCUMULATE, SP_COM_CENTER, SP_COM_RIGHT };
		spAttackCommand[2][DIR_LEFT].resize(4);
		spAttackCommand[2][DIR_LEFT] = { SP_COM_RIGHT, SP_COM_ACCUMULATE, SP_COM_CENTER, SP_COM_LEFT };
		break;

	case 2:
		characterName = "棒人間_赤";
		AddAnim("気功拳", 0, 0, 9, 4, false, 0, 0);
		spAttackAnimName[0] = "気功拳";
		spAttackAnimFileName[0] = "kikoh";
		//spAttackType[0] = SKILL_TYPE_PUNCH;
		animAttributeTbl["気功拳"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["気功拳"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["気功拳"][2] = ANIM_ATTRIBUTE_NON;

		spAttackCommand[0][DIR_RIGHT].resize(3);
		spAttackCommand[0][DIR_RIGHT] = { SP_COM_DOWN, SP_COM_RIGHT_DOWN, SP_COM_RIGHT };
		spAttackCommand[0][DIR_LEFT].resize(3);
		spAttackCommand[0][DIR_LEFT] = { SP_COM_DOWN, SP_COM_LEFT_DOWN, SP_COM_LEFT };

		AddAnim("旋風脚", 0, 0, 4, 4, true, 0, 0);
		spAttackAnimName[1] = "旋風脚";
		spAttackAnimFileName[1] = "senpu";
		//spAttackType[1] = SKILL_TYPE_KICK;
		animAttributeTbl["旋風脚"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["旋風脚"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["旋風脚"][2] = ANIM_ATTRIBUTE_NON;

		spAttackCommand[1][DIR_RIGHT].resize(3);
		spAttackCommand[1][DIR_RIGHT] = { SP_COM_DOWN, SP_COM_LEFT_DOWN, SP_COM_LEFT };
		spAttackCommand[1][DIR_LEFT].resize(3);
		spAttackCommand[1][DIR_LEFT] = { SP_COM_DOWN, SP_COM_RIGHT_DOWN, SP_COM_RIGHT };

		AddAnim("カンフーキック", 0, 0, 17, 4, false, 0, 0);
		spAttackAnimName[2] = "カンフーキック";
		spAttackAnimFileName[2] = "kung";
		//spAttackType[2] = SKILL_TYPE_PUNCH;
		animAttributeTbl["カンフーキック"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["カンフーキック"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["カンフーキック"][2] = ANIM_ATTRIBUTE_NON;

		spAttackCommand[2][DIR_RIGHT].resize(3);
		spAttackCommand[2][DIR_RIGHT] = { SP_COM_RIGHT, SP_COM_DOWN, SP_COM_RIGHT_DOWN };
		spAttackCommand[2][DIR_LEFT].resize(3);
		spAttackCommand[2][DIR_LEFT] = { SP_COM_LEFT, SP_COM_DOWN, SP_COM_LEFT_DOWN };
		break;

	case 3:
		characterName = "棒人間_青";
		AddAnim("かめはめ波", 0, 0, 10, 4, false, 0, 0);
		spAttackAnimName[0] = "かめはめ波";
		spAttackAnimFileName[0] = "kamehame";
		//spAttackType[0] = SKILL_TYPE_PUNCH;
		animAttributeTbl["かめはめ波"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["かめはめ波"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["かめはめ波"][2] = ANIM_ATTRIBUTE_NON;

		spAttackCommand[0][DIR_RIGHT].resize(3);
		spAttackCommand[0][DIR_RIGHT] = { SP_COM_DOWN, SP_COM_RIGHT_DOWN, SP_COM_RIGHT };
		spAttackCommand[0][DIR_LEFT].resize(3);
		spAttackCommand[0][DIR_LEFT] = { SP_COM_DOWN, SP_COM_LEFT_DOWN, SP_COM_LEFT };

		AddAnim("ラッシュ", 0, 0, 15, 4, false, 0, 0);
		spAttackAnimName[1] = "ラッシュ";
		spAttackAnimFileName[1] = "rush";
		//spAttackType[1] = SKILL_TYPE_KICK;
		animAttributeTbl["ラッシュ"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["ラッシュ"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["ラッシュ"][2] = ANIM_ATTRIBUTE_NON;

		spAttackCommand[1][DIR_RIGHT].resize(3);
		spAttackCommand[1][DIR_RIGHT] = { SP_COM_DOWN, SP_COM_LEFT_DOWN, SP_COM_LEFT };
		spAttackCommand[1][DIR_LEFT].resize(3);
		spAttackCommand[1][DIR_LEFT] = { SP_COM_DOWN, SP_COM_RIGHT_DOWN, SP_COM_RIGHT };

		AddAnim("回し蹴り", 0, 0, 26, 4, false, 0, 0);
		spAttackAnimName[2] = "回し蹴り";
		spAttackAnimFileName[2] = "roundKick";
		//spAttackType[2] = SKILL_TYPE_PUNCH;
		animAttributeTbl["回し蹴り"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["回し蹴り"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["回し蹴り"][2] = ANIM_ATTRIBUTE_NON;

		spAttackCommand[2][DIR_RIGHT].resize(3);
		spAttackCommand[2][DIR_RIGHT] = { SP_COM_RIGHT, SP_COM_DOWN, SP_COM_RIGHT_DOWN };
		spAttackCommand[2][DIR_LEFT].resize(3);
		spAttackCommand[2][DIR_LEFT] = { SP_COM_LEFT, SP_COM_DOWN, SP_COM_LEFT_DOWN };
		break;

	case 4:
		characterName = "棒人間_緑";
		AddAnim("ソニックブーム", 0, 0, 7, 4, false, 0, 0);
		spAttackAnimName[0] = "ソニックブーム";
		spAttackAnimFileName[0] = "sonic";
		//spAttackType[0] = SKILL_TYPE_PUNCH;
		animAttributeTbl["ソニックブーム"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["ソニックブーム"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["ソニックブーム"][2] = ANIM_ATTRIBUTE_NON;

		spAttackCommand[0][DIR_RIGHT].resize(3);
		spAttackCommand[0][DIR_RIGHT] = { SP_COM_DOWN, SP_COM_RIGHT_DOWN, SP_COM_RIGHT };
		spAttackCommand[0][DIR_LEFT].resize(3);
		spAttackCommand[0][DIR_LEFT] = { SP_COM_DOWN, SP_COM_LEFT_DOWN, SP_COM_LEFT };

		AddAnim("ランキャク", 0, 0, 10, 4, false, 0, 0);
		spAttackAnimName[1] = "ランキャク";
		spAttackAnimFileName[1] = "cutterKick";
		//spAttackType[1] = SKILL_TYPE_KICK;
		animAttributeTbl["ランキャク"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["ランキャク"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["ランキャク"][2] = ANIM_ATTRIBUTE_NON;

		spAttackCommand[1][DIR_RIGHT].resize(3);
		spAttackCommand[1][DIR_RIGHT] = { SP_COM_DOWN, SP_COM_LEFT_DOWN, SP_COM_LEFT };
		spAttackCommand[1][DIR_LEFT].resize(3);
		spAttackCommand[1][DIR_LEFT] = { SP_COM_DOWN, SP_COM_RIGHT_DOWN, SP_COM_RIGHT };

		AddAnim("ミサイルアロー", 0, 0, 6, 4, false, 0, 0);
		spAttackAnimName[2] = "ミサイルアロー";
		spAttackAnimFileName[2] = "missile";
		//spAttackType[2] = SKILL_TYPE_PUNCH;
		animAttributeTbl["ミサイルアロー"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["ミサイルアロー"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["ミサイルアロー"][2] = ANIM_ATTRIBUTE_NON;

		spAttackCommand[2][DIR_RIGHT].resize(3);
		spAttackCommand[2][DIR_RIGHT] = { SP_COM_RIGHT, SP_COM_DOWN, SP_COM_RIGHT_DOWN };
		spAttackCommand[2][DIR_LEFT].resize(3);
		spAttackCommand[2][DIR_LEFT] = { SP_COM_LEFT, SP_COM_DOWN, SP_COM_LEFT_DOWN };
		break;

	case 5:
		characterName = "棒人間_黄";
		AddAnim("岩投げ", 0, 0, 16, 4, false, 0, 0);
		spAttackAnimName[0] = "岩投げ";
		spAttackAnimFileName[0] = "throw";
		//spAttackType[0] = SKILL_TYPE_PUNCH;
		animAttributeTbl["岩投げ"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["岩投げ"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["岩投げ"][2] = ANIM_ATTRIBUTE_NON;

		spAttackCommand[0][DIR_RIGHT].resize(3);
		spAttackCommand[0][DIR_RIGHT] = { SP_COM_DOWN, SP_COM_RIGHT_DOWN, SP_COM_RIGHT };
		spAttackCommand[0][DIR_LEFT].resize(3);
		spAttackCommand[0][DIR_LEFT] = { SP_COM_DOWN, SP_COM_LEFT_DOWN, SP_COM_LEFT };

		AddAnim("地面割", 0, 0, 15, 4, false, 0, 0);
		spAttackAnimName[1] = "地面割";
		spAttackAnimFileName[1] = "split";
		//spAttackType[1] = SKILL_TYPE_KICK;
		animAttributeTbl["地面割"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["地面割"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["地面割"][2] = ANIM_ATTRIBUTE_NON;

		spAttackCommand[1][DIR_RIGHT].resize(3);
		spAttackCommand[1][DIR_RIGHT] = { SP_COM_DOWN, SP_COM_LEFT_DOWN, SP_COM_LEFT };
		spAttackCommand[1][DIR_LEFT].resize(3);
		spAttackCommand[1][DIR_LEFT] = { SP_COM_DOWN, SP_COM_RIGHT_DOWN, SP_COM_RIGHT };

		AddAnim("タックル", 0, 0, 7, 4, false, 0, 0);
		spAttackAnimName[2] = "タックル";
		spAttackAnimFileName[2] = "tackle";
		//spAttackType[2] = SKILL_TYPE_PUNCH;
		animAttributeTbl["タックル"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["タックル"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["タックル"][2] = ANIM_ATTRIBUTE_NON;

		spAttackCommand[2][DIR_RIGHT].resize(3);
		spAttackCommand[2][DIR_RIGHT] = { SP_COM_RIGHT, SP_COM_DOWN, SP_COM_RIGHT_DOWN };
		spAttackCommand[2][DIR_LEFT].resize(3);
		spAttackCommand[2][DIR_LEFT] = { SP_COM_LEFT, SP_COM_DOWN, SP_COM_LEFT_DOWN };
		break;

	case 6:
		characterName = "棒人間_ピンク";
		AddAnim("サイコカッター", 0, 0, 9, 4, false, 0, 0);
		spAttackAnimName[0] = "サイコカッター";
		spAttackAnimFileName[0] = "psycho";
		//spAttackType[0] = SKILL_TYPE_PUNCH;
		animAttributeTbl["サイコカッター"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["サイコカッター"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["サイコカッター"][2] = ANIM_ATTRIBUTE_NON;

		spAttackCommand[0][DIR_RIGHT].resize(3);
		spAttackCommand[0][DIR_RIGHT] = { SP_COM_DOWN, SP_COM_RIGHT_DOWN, SP_COM_RIGHT };
		spAttackCommand[0][DIR_LEFT].resize(3);
		spAttackCommand[0][DIR_LEFT] = { SP_COM_DOWN, SP_COM_LEFT_DOWN, SP_COM_LEFT };

		AddAnim("ワープ", 0, 0, 11, 4, false, 0, 0);
		spAttackAnimName[1] = "ワープ";
		spAttackAnimFileName[1] = "warp";
		//spAttackType[1] = SKILL_TYPE_KICK;
		animAttributeTbl["ワープ"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["ワープ"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["ワープ"][2] = ANIM_ATTRIBUTE_NON;

		spAttackCommand[1][DIR_RIGHT].resize(3);
		spAttackCommand[1][DIR_RIGHT] = { SP_COM_DOWN, SP_COM_LEFT_DOWN, SP_COM_LEFT };
		spAttackCommand[1][DIR_LEFT].resize(3);
		spAttackCommand[1][DIR_LEFT] = { SP_COM_DOWN, SP_COM_RIGHT_DOWN, SP_COM_RIGHT };

		AddAnim("かかと落とし", 0, 0, 14, 4, false, 0, 0);
		spAttackAnimName[2] = "かかと落とし";
		spAttackAnimFileName[2] = "heeldrop";
		//spAttackType[2] = SKILL_TYPE_PUNCH;
		animAttributeTbl["かかと落とし"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["かかと落とし"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["かかと落とし"][2] = ANIM_ATTRIBUTE_NON;

		spAttackCommand[2][DIR_RIGHT].resize(3);
		spAttackCommand[2][DIR_RIGHT] = { SP_COM_RIGHT, SP_COM_DOWN, SP_COM_RIGHT_DOWN };
		spAttackCommand[2][DIR_LEFT].resize(3);
		spAttackCommand[2][DIR_LEFT] = { SP_COM_LEFT, SP_COM_DOWN, SP_COM_LEFT_DOWN };
		break;

	case 7:
		characterName = "棒人間_水色";
		AddAnim("竜の息吹", 0, 0, 12, 4, false, 0, 0);
		spAttackAnimName[0] = "竜の息吹";
		spAttackAnimFileName[0] = "ibuki";
		//spAttackType[0] = SKILL_TYPE_PUNCH;
		animAttributeTbl["竜の息吹"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["竜の息吹"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["竜の息吹"][2] = ANIM_ATTRIBUTE_NON;

		spAttackCommand[0][DIR_RIGHT].resize(3);
		spAttackCommand[0][DIR_RIGHT] = { SP_COM_DOWN, SP_COM_RIGHT_DOWN, SP_COM_RIGHT };
		spAttackCommand[0][DIR_LEFT].resize(3);
		spAttackCommand[0][DIR_LEFT] = { SP_COM_DOWN, SP_COM_LEFT_DOWN, SP_COM_LEFT };

		AddAnim("ソーラン", 0, 0, 13, 4, false, 0, 0);
		spAttackAnimName[1] = "ソーラン";
		spAttackAnimFileName[1] = "soran";
		//spAttackType[1] = SKILL_TYPE_KICK;
		animAttributeTbl["ソーラン"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["ソーラン"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["ソーラン"][2] = ANIM_ATTRIBUTE_NON;

		spAttackCommand[1][DIR_RIGHT].resize(3);
		spAttackCommand[1][DIR_RIGHT] = { SP_COM_DOWN, SP_COM_LEFT_DOWN, SP_COM_LEFT };
		spAttackCommand[1][DIR_LEFT].resize(3);
		spAttackCommand[1][DIR_LEFT] = { SP_COM_DOWN, SP_COM_RIGHT_DOWN, SP_COM_RIGHT };

		AddAnim("アクセル", 0, 0, 21, 4, false, 0, 0);
		spAttackAnimName[2] = "アクセル";
		spAttackAnimFileName[2] = "accele";
		//spAttackType[2] = SKILL_TYPE_PUNCH;
		animAttributeTbl["アクセル"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["アクセル"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["アクセル"][2] = ANIM_ATTRIBUTE_NON;

		spAttackCommand[2][DIR_RIGHT].resize(3);
		spAttackCommand[2][DIR_RIGHT] = { SP_COM_RIGHT, SP_COM_DOWN, SP_COM_RIGHT_DOWN };
		spAttackCommand[2][DIR_LEFT].resize(3);
		spAttackCommand[2][DIR_LEFT] = { SP_COM_LEFT, SP_COM_DOWN, SP_COM_LEFT_DOWN };
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
