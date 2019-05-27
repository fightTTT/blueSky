#include "AIStickHuman.h"
#include "SceneMng.h"

AIStickHuman::AIStickHuman(VECTOR2 pos, VECTOR2 offset, DIR charaDir) : AICharacter(offset)
{
	shotCreateCount = 0;
	spAttackAnimName[0] = { "技1"};
	spAttackAnimName[1] = { "技2"};
	spAttackAnimName[2] = { "技3"};
	spAttackAnimFileName[0] = { "waza_1"};
	spAttackAnimFileName[1] = { "waza_2"};
	spAttackAnimFileName[2] = { "waza_3"};

	spAttackDamage["百裂脚"] = -10;
	spAttackDamage["昇竜"] = -30;
	spAttackDamage["ウマキック"] = -15;
	spAttackDamage["ローリングアタック"] = -15;
	spAttackDamage["旋風脚"] = -15;
	spAttackDamage["カンフーキック"] = -30;
	spAttackDamage["ラッシュ"] = -10;
	spAttackDamage["回し蹴り"] = -20;
	spAttackDamage["ミサイルアロー"] = -15;
	spAttackDamage["ランキャク"] = -20;
	spAttackDamage["地面割"] = -25;
	spAttackDamage["タックル"] = -15;
	spAttackDamage["かかと落とし"] = -25;
	spAttackDamage["アクセル"] = -40;
	spAttackDamage["蹴り返し"] = -15;


	switch (lpSceneMng.GetCharID(PAD_AI))
	{
	case 0:
		characterName = "棒人間";
		AddAnim("波動", 0, 0, 11, 4, false, 0, 0);
		spAttackAnimName[0] = "波動";
		spAttackAnimFileName[0] = "hadou";
		animAttributeTbl["波動"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["波動"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["波動"][2] = ANIM_ATTRIBUTE_SHOT;

		AddAnim("百裂脚", 0, 0, 18, 4, false, 0, 0);
		spAttackAnimName[1] = "百裂脚";
		spAttackAnimFileName[1] = "hyakuretsu";
		animAttributeTbl["百裂脚"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["百裂脚"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["百裂脚"][2] = ANIM_ATTRIBUTE_NON;

		AddAnim("昇竜", 0, 0, 16, 4, false, 0, 0);
		spAttackAnimName[2] = "昇竜";
		spAttackAnimFileName[2] = "shoryu";
		animAttributeTbl["昇竜"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["昇竜"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["昇竜"][2] = ANIM_ATTRIBUTE_INVINCIBLE;

		shotCreateCount = 30;

		break;

	case 1:
		characterName = "棒人間_黒";
		AddAnim("刺突砲", 0, 0, 11, 4, false, 0, 0);
		spAttackAnimName[0] = "刺突砲";
		spAttackAnimFileName[0] = "sitotsu";
		animAttributeTbl["刺突砲"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["刺突砲"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["刺突砲"][2] = ANIM_ATTRIBUTE_SHOT;

		AddAnim("ウマキック", 0, 0, 7, 5, false, 0, 0);
		spAttackAnimName[1] = "ウマキック";
		spAttackAnimFileName[1] = "horseKick";
		animAttributeTbl["ウマキック"][0] = ANIM_ATTRIBUTE_SQUAT;
		animAttributeTbl["ウマキック"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["ウマキック"][2] = ANIM_ATTRIBUTE_NON;

		AddAnim("ローリングアタック", 0, 0, 6, 1, true, 0, -16);
		spAttackAnimName[2] = "ローリングアタック";
		spAttackAnimFileName[2] = "rolling";
		animAttributeTbl["ローリングアタック"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["ローリングアタック"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["ローリングアタック"][2] = ANIM_ATTRIBUTE_NON;

		shotCreateCount = 30;

		break;

	case 2:
		characterName = "棒人間_赤";
		AddAnim("気功拳", 0, 0, 9, 4, false, 0, 0);
		spAttackAnimName[0] = "気功拳";
		spAttackAnimFileName[0] = "kikoh";
		animAttributeTbl["気功拳"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["気功拳"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["気功拳"][2] = ANIM_ATTRIBUTE_SHOT;

		AddAnim("旋風脚", 0, 0, 4, 3, true, 0, -16);
		spAttackAnimName[1] = "旋風脚";
		spAttackAnimFileName[1] = "senpu";
		animAttributeTbl["旋風脚"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["旋風脚"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["旋風脚"][2] = ANIM_ATTRIBUTE_NON;

		AddAnim("カンフーキック", 0, 0, 17, 3, false, 0, 0);
		spAttackAnimName[2] = "カンフーキック";
		spAttackAnimFileName[2] = "kung";
		animAttributeTbl["カンフーキック"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["カンフーキック"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["カンフーキック"][2] = ANIM_ATTRIBUTE_INVINCIBLE;

		shotCreateCount = 30;

		break;

	case 3:
		characterName = "棒人間_青";
		AddAnim("かめはめ波", 0, 0, 10, 4, false, 0, 0);
		spAttackAnimName[0] = "かめはめ波";
		spAttackAnimFileName[0] = "kamehame";
		animAttributeTbl["かめはめ波"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["かめはめ波"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["かめはめ波"][2] = ANIM_ATTRIBUTE_SHOT;

		AddAnim("ラッシュ", 0, 0, 15, 1, true, 0, 0);
		spAttackAnimName[1] = "ラッシュ";
		spAttackAnimFileName[1] = "rush";
		animAttributeTbl["ラッシュ"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["ラッシュ"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["ラッシュ"][2] = ANIM_ATTRIBUTE_NON;

		AddAnim("回し蹴り", 0, 0, 26, 3, false, 0, 0);
		spAttackAnimName[2] = "回し蹴り";
		spAttackAnimFileName[2] = "roundKick";
		animAttributeTbl["回し蹴り"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["回し蹴り"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["回し蹴り"][2] = ANIM_ATTRIBUTE_INVINCIBLE;

		shotCreateCount = 30;

		break;

	case 4:
		characterName = "棒人間_緑";
		AddAnim("ソニックブーム", 0, 0, 7, 4, false, 0, 0);
		spAttackAnimName[0] = "ソニックブーム";
		spAttackAnimFileName[0] = "sonic";
		animAttributeTbl["ソニックブーム"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["ソニックブーム"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["ソニックブーム"][2] = ANIM_ATTRIBUTE_SHOT;

		AddAnim("ミサイルアロー", 0, 0, 6, 4, false, 0, -16);
		spAttackAnimName[1] = "ミサイルアロー";
		spAttackAnimFileName[1] = "missile";
		animAttributeTbl["ミサイルアロー"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["ミサイルアロー"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["ミサイルアロー"][2] = ANIM_ATTRIBUTE_NON;

		AddAnim("ランキャク", 0, 0, 10, 3, false, 0, -16);
		spAttackAnimName[2] = "ランキャク";
		spAttackAnimFileName[2] = "cutterKick";
		animAttributeTbl["ランキャク"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["ランキャク"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["ランキャク"][2] = ANIM_ATTRIBUTE_INVINCIBLE;

		shotCreateCount = 22;

		break;

	case 5:
		characterName = "棒人間_黄";
		AddAnim("岩投げ", 0, 0, 16, 4, false, 0, 0);
		spAttackAnimName[0] = "岩投げ";
		spAttackAnimFileName[0] = "throw";
		animAttributeTbl["岩投げ"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["岩投げ"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["岩投げ"][2] = ANIM_ATTRIBUTE_SHOT;

		AddAnim("地面割", 0, 0, 22, 4, false, 50, 30);
		spAttackAnimName[1] = "地面割";
		spAttackAnimFileName[1] = "split";
		animAttributeTbl["地面割"][0] = ANIM_ATTRIBUTE_AIR;
		animAttributeTbl["地面割"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["地面割"][2] = ANIM_ATTRIBUTE_NON;

		AddAnim("タックル", 0, 0, 7, 4, false, 0, 0);
		spAttackAnimName[2] = "タックル";
		spAttackAnimFileName[2] = "tackle";
		animAttributeTbl["タックル"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["タックル"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["タックル"][2] = ANIM_ATTRIBUTE_NON;

		shotCreateCount = 45;

		break;

	case 6:
		characterName = "棒人間_ピンク";
		AddAnim("サイコカッター", 0, 0, 9, 4, false, 0, 0);
		spAttackAnimName[0] = "サイコカッター";
		spAttackAnimFileName[0] = "psycho";
		animAttributeTbl["サイコカッター"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["サイコカッター"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["サイコカッター"][2] = ANIM_ATTRIBUTE_SHOT;

		AddAnim("かかと落とし", 0, 0, 14, 4, false, 0, 0);
		spAttackAnimName[1] = "かかと落とし";
		spAttackAnimFileName[1] = "heeldrop";
		animAttributeTbl["かかと落とし"][0] = ANIM_ATTRIBUTE_AIR;
		animAttributeTbl["かかと落とし"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["かかと落とし"][2] = ANIM_ATTRIBUTE_NON;

		AddAnim("ワープ", 0, 0, 12, 5, false, 0, 0);
		spAttackAnimName[2] = "ワープ";
		spAttackAnimFileName[2] = "warp";
		animAttributeTbl["ワープ"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["ワープ"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["ワープ"][2] = ANIM_ATTRIBUTE_INVINCIBLE;

		shotCreateCount = 22;

		break;

	case 7:
		characterName = "棒人間_水色";
		AddAnim("竜の息吹", 0, 0, 12, 4, false, 0, 0);
		spAttackAnimName[0] = "竜の息吹";
		spAttackAnimFileName[0] = "ibuki";
		animAttributeTbl["竜の息吹"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["竜の息吹"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["竜の息吹"][2] = ANIM_ATTRIBUTE_SHOT;

		AddAnim("アクセル", 0, 0, 21, 4, false, 0, 0);
		spAttackAnimName[1] = "アクセル";
		spAttackAnimFileName[1] = "accele";
		animAttributeTbl["アクセル"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["アクセル"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["アクセル"][2] = ANIM_ATTRIBUTE_NON;

		AddAnim("蹴り返し", 0, 0, 14, 3, false, 0, 0);
		spAttackAnimName[2] = "蹴り返し";
		spAttackAnimFileName[2] = "turn";
		animAttributeTbl["蹴り返し"][0] = ANIM_ATTRIBUTE_AIR;
		animAttributeTbl["蹴り返し"][1] = ANIM_ATTRIBUTE_ATTACK_SP;
		animAttributeTbl["蹴り返し"][2] = ANIM_ATTRIBUTE_NON;

		shotCreateCount = 26;

		break;

	default:
		break;
	}

	Init("image/" + characterName + "/待機/stand_0.png", VECTOR2(256, 256), VECTOR2(1, 1), VECTOR2(pos.x, pos.y), false);

	dir = charaDir;
}

AIStickHuman::~AIStickHuman()
{
}
