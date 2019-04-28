#pragma once
#pragma once
#include <string>
#include <map>
#include "VECTOR2.h"
#include "classObj.h"
#include "BaseFloor.h"
#include "GameScene.h"
#include "ColMng.h"
#include "DIR.h"
#include "PARAM.h"
#include "LOCATE.h"

#define ANGRY_CNT			(10)	// �Ԃ�����Ӱ�ނ̕b��

#define PRASS_INV (30)	// ����������p


#define FIX_TIME			(15);		// �d������
#define GROUND_POS_Y	540
#define PLAYER_DEF_SPEED (3)

enum ANIM_TBL {
	ANIM_TBL_START_ID,	// �J�n�ʒu
	ANIM_TBL_FRAME,		// �ϐ�
	ANIM_TBL_INV,		// �Ԋu
	ANIM_TBL_LOOP,		// ���[�v���邩�ǂ���
	ANIM_TBL_MAX
};

enum OBJ_TYPE {
	OBJ_TYPE_PLAYER,
	OBJ_TYPE_WHEPONN,
	OBJ_TYPE_STAGE_OBJ,
	OBJ_TYPE_MAX
};

enum ANIM_TYPE {
	ANIM_TYPE_NO_ATTACK,		// �U����Ұ��݂ł͂Ȃ�
	ANIM_TYPE_ATTACK,			// �A���ł͂Ȃ���Ұ���
	ANIM_TYPE_MULTI_HIT,		// �A�������Ұ���
};

enum ANIM_STATE {
	ANIM_NOW,			// ���݂̱�Ұ���
	ANIM_OLD,			// 1�ڰёO�̱�Ұ���
	ANIM_MAX,
};

enum DMG_LATE {
	DMG_HV,			// ����Ұ��
	DMG_MD,			// ����Ұ��
	DMG_LW,			// ����Ұ��
	DMG_MAX,
};

enum PL_STATE {
	PL_STATE_SEWAT,		// ��(HP���Ȃ�)
	PL_STATE_ANGRY,		// �{���������
	PL_STATE_SPECIAL,	// �K�E�Z�������
	PL_STATE_MAX
};

enum DERI {
	DERI_NON,	// �h�����Ȃ�
	DERI_NOMAL,	// �ʏ�U��3
	DERI_UP,	// �ʏ�U��_��
	DERI_DOWN,	// �ʏ�U��_��
	DERI_MAX
};

enum CHARGE_STAGE {
	CHARGE_NON,		// ���߂Ȃ�
	CHARGE_LW,		// ����1�i�K
	CHARGE_HV,		// ����2�i�K
	CHARGE_MAX,
};

typedef struct {
	std::array<bool, PL_STATE_MAX>	plState;
	int cnt;			// ���l����--���Ă����B0�ɂȂ��plStateItr��i�߂Ď���̷�޼�ɂ���
}PL_STATE_MULTI;

typedef struct {
	std::string attackName;		// �󂯂��U����
	int animId;					// �󂯂��U���̺ϐ�
}HIT_PLAYER_INF;

typedef struct {
	VECTOR2 oldPos;	// �����Ă��鏰��1�ڰт܂����W���
	int itrCnt;		// �����Ă��鏰�Ɠ����蔻�肪�������ꍇ�̐i��itr��
}PARENT_FLOOR;

typedef struct {
	VECTOR2 insPos;	// ��Ұ�ނ��\�������n�_
	int		cnt;	// cnt--���Ă����A0�ɋ߂��Ȃ�ɂꓧ����
	int		damage;	// �󂯂���Ұ��
}FLY_TEXT;

typedef struct {
	VECTOR2		insPos;			// ˯Ĵ̪�Ă��\�������n�_
	int			cnt;			// cnt--���Ă����A0�ɋ߂��Ȃ�Ə�����1
	DMG_LATE	damageRate;		// �󂯂���Ұ�ނ̑傫��(���̑傫���ɂ���ĕ\������˯Ĵ̪�ĕς���)
	double		rota;			// ��]�`��̊p�x
}HIT_EFFECT;

typedef struct {
	int invCnt;					// ���G�̉�
	std::string animName;		// ���G����Ұ���
}INV_INF;

typedef struct {
	VECTOR2 pos;				// �`����W
	VECTOR2 offset;				// �`��̾��
	std::string animName;		// ��Ұ��ݖ�
	unsigned int animid;		// ��Ұ��ݺϐ�
	DIR dir;					// ����
	int cnt;					// ���l�܂ōs���Ə���
}SHADOW;

class GameController;

class Obj
{
public:
	Obj();
	virtual ~Obj();

	// init(�ǂݍ��޷��Ȱ�)
	virtual bool init(std::string charName);
	virtual bool initAnim(void) = 0;	// ��Ұ��݂̏����ǉ��ݒ�
	bool initSe(void);					// Se�̏����ݒ�

	void UpDate(const GameController &controller, weakListObj objList, weakListChicken chickenList, bool entryFlag);	// ��ԍX�V

	const VECTOR2 &GetPos(void);							// ۰�ٕϐ��͊�{�A�Q�ƂŕԂ��Ȃ�
	const VECTOR2 GetPosOld(void);
	const std::string GetAnim(void);						// ����Set����Ă����Ұ��ݖ���Ԃ�
	const std::string GetAnim(ANIM_STATE type);				// 1�ڰёO�����݂̱�Ұ��ݖ���Ԃ�
	const std::vector<ColInf> GetPlayerCollision(void);		// ���̃v���C���[���玩���̺ؼޮ݂����邽�߂̊֐�
	const DIR GetDir(void);									// ���݂̌�����Ԃ�
	virtual const int GetPadNum(void) = 0;					// ���۰װ�ԍ���Ԃ�
	const int GetHp(void);									// hp��Ԃ�
	const int GetAnimId(void);								// ��Ұ���id��Ԃ�
	const int GetAnimFrame(void);
	virtual const VECTOR2 GetFlyHighPower(void);
	const std::string GetCharName(void);
	const RESULT_INFO GetResultInfo(void);					// ػ��ĉ�ʂŕK�v�ȏ���Ԃ�
	const bool CheckSpAttackCnt(void);						// �K�E�Z���Ȃ�true

															// AddAnim(��Ұ��ݖ�,�J�n�ϖ�x,�J�n�ϖ�y,���ϐ�,�؂�ւ��ڰѐ�,���[�v���邩)
	bool AddAnim(std::string animName, int inv, bool loop, std::string animFile);

	bool SetAnim(std::string animName);				// ��Ұ��݂̐ݒ�	�ݒ肵����Ұ��݂��s��
	void AddSpecialPoint(int specialPoint);			// �K�E�ް�ނ̉��Z���s��
	void AddAngryPoint(int angryPoint);				// �{��ް�ނ̉��Z(��Ɍ��Z)���s��
	void AddChickenKillCnt(void);					// resultInfo��chickenKillCnt��+1����

	bool CheckDeath(void) { return deathFlag; }	// �������m�F	 ��:true	��:false
	virtual bool CheckObjType(OBJ_TYPE type) = 0;
	void		 CheckHit(weakListObj objList, std::list<ExpOrb*>& expOrbList, bool entryFlag);	// �����蔻��(�����̑̂Ƒ���̕���)
	bool		 CheckAngryMode(void);				// true:�Ԃ�����Ӱ��			false:�ʏ�
	CHARGE_STAGE CheckCharge(void);					// ���ߒi�K��Ԃ�(���߂Ȃ� = 0, ����1 = 1, ����2 = 2)
	CUT_IN_INFO	 CheckSpecialCutIn(void);			// �K�E�Z�𔭓������߯�ޔԍ��ƍ��W��Ԃ�

	virtual void Draw(bool moveStopFlag, CUT_IN_INFO cutInInfo);		// �`��

	// SE�p�֐�
	const bool		CheckAnimSe(std::string anim);		// 1�ڰёO�ƌ��݂̱�Ұ��݂��Ⴄ�Ȃ�true��Ԃ�(Player��SE�p)
	const DMG_LATE	CheckDamageSe(void);				// ��x����Ұ�ޗʂ��v�Z������Ұ�ނȂǂ𔻒肷��

	static void ResultInfoInit();
	static void AddChickenCnt(int padNum);					// chickenCnt++����
	static int GetChickenCnt(int padNum);

private:
	int shadowPosY;

	// ��Ұ��݂��Ƃ̊J�n�ʒu,�ϐ�,�Ԋu���i�[
	std::map<std::string, int[ANIM_TBL_MAX]>animTable;
	std::map<std::string, std::string>		animFileName;
	std::map<std::string, int>				charNumbering;		// charName�������ݸނ���@�J��~�� = 0, �o�t�u = 1

	std::array<std::string[2], DMG_MAX>		hitEffectFileName;	// ����Ғ���҂̉摜�̖��O���Ǘ�����p

	std::string animName;				// �\����Ұ��ݖ�
	std::string charName;

	std::list<SHADOW>	shadowList;		// �c���̏����i�[
	std::list<FLY_TEXT> flyTextList;	// �ײ÷�Ă̏����i�[�����

	std::list<PL_STATE>	plStateList;	// ��ڲԂ̏�Ԃ��ۑ����ꂽlist�@�����o���̕\���Ɏg�p����
	std::list<PL_STATE>::iterator plStateItr;

	std::array<std::string, PL_STATE_MAX> plStateFileName;		// ̷�޼�̉摜�̖��O���Ǘ�����p

	std::map<std::string, std::vector<ColInf>> colMap;	// player�̺ؼޮ݂��i�[(��Ұ��ݖ��ŊǗ�)

	std::array<HIT_PLAYER_INF, 4> hitPlayerInf;		// �e��ڲ԰����󂯂��Ō�̍U�����i�[(ɯ��ޯ�����ؾ��)

	std::array<int, 5>	spGaugeColorTbl;			// �K�E�Z�ް�ނ̐Fð��فi���΁������򁨐ԁj

	static RESULT_INFO resultInfo;	// ػ��ĉ�ʂŕK�v�ȏ��

	virtual void SetMove(const GameController &controller, weakListObj objList, weakListChicken chickenList, bool entryFlag) = 0;

protected:

	std::map<std::string, ANIM_TYPE>		animPtnTbl;		// ��Ұ��݂��퓬�n(�U����h��)�Ȃ�true

	virtual int GetGuardTimeMax(void);						// ��ׂ��Ƃ̍ő�ް�ގ��Ԃ𓾂�
	virtual int GetJumpCntMax(void);						// ��ׂ��Ƃ̍ő�ެ��ߐ��𓾂�
	virtual int GetSpAttackTimeMax(void);

	bool AlwaysUpDate(const GameController &controller, weakListChicken chickenList);			// ���ڰэX�V
	int  GetColMaxEndPosY(std::vector <ColInf> collisionVec);		// ���݂̺ײ�ް�̒��ōł��傫��Y���W��Ԃ�
	virtual void PosUpDate(VECTOR2& tmpPos, int maxEndPosY);		// ���W�̕␳��d�͂ɂ��e���̌v�Z���s��
	void DeriAttack(const GameController &controller);			// �e��ڲ԰���ʂ̔h���U�����s��
	void Attack(const GameController &controller, VECTOR2 &tmpPpos, bool entryFlag);				// �U����Ұ��݂̾��
	virtual void UniqueAttack(VECTOR2& tmpPos);					// �ŗL�U���̓����̐ݒ�
	void UpDateAnimId(void);		// ��Ұ���id�̌v�Z
	void Dash(const GameController &controller, VECTOR2 &tmpPos);				// �ޯ���̏���
	void JumpFnc(const GameController &controller, VECTOR2 &tmpPos, int maxEndPosY);		// �ެ��߂����ۂɌĂ΂��֐�
	bool ColGround(VECTOR2& tmpPos, std::vector<ColInf> colVec,int maxEndPosY);			// �n�ʂƏ��Ƃ̓����蔻��
	virtual void UniqueColGround(void);							// ��ׂ��Ƃ̒n�ʂ⏰�Ɖ�����������̏������s��
	void PosAdjust(VECTOR2 &tmpPos, int maxEndPosY);			// ���W�␳
	virtual void SetNextAnim(bool animEndFlag);					// animEndFlag��true�Ȃ玟�̱�Ұ��݂�Ă���
	void MoveEndUpDate(VECTOR2 tmpPos, const GameController &controller, int maxEndPosY);			// player�s����ɍs���X�V
	void PosOutOfRangeCtrl(VECTOR2& pos, int maxEndPosY);		// ���W�͈͊O����(�␳������W,�n�ʂ���pos�܂ł̋���)
	void FlyUpDate(const GameController &controller);			// �P���V���̏�ԍX�V
	void FlyTextUpDate(void);		// �ײ÷�Ă̏�ԍX�V
	void HitEffectUpDate(void);		// ˯Ĵ̪�Ă̏�ԍX�V
	void PlStateUpDate(void);		// plStateList�̏�ԍX�V

	void AngryGaugeDraw(void);		// �{��ް��UI�̕`��
	void SpecialGaugeDraw(bool cutInFlag);	// �K�E�ް��UI�̕`��
	void GuardDraw(void);			// �ް�ނ̋ʂ̕`��
	void StateIconDraw(void);		// �ð������(̷�޼)�̕`��
	void FlyTextDraw(void);			// �ײ÷�Ă̕`��
	void CharacterDraw(std::vector<ColInf> collisonVec, bool moveStopFlag, CUT_IN_INFO cutInInfo);		// ���췬ׂ̕`��

	// Se�p
	void PlaySe(void);				// Se�̍Đ�
	virtual void PlaySPSe(void);	// �K�E�Z�̍Đ�

	int plStateCnt;

	DIR dir;						// �����Ă������
	DIR dirOld;						// 1�ڰёO�����Ă�������
	VECTOR2 pos;					// ���W
	VECTOR2 posOld;					// 1�ڰёO�̍��W

	// SE�p
	std::string animNameOld;		// 1�ڰёO�̱�Ұ��ݖ�
	int oldHp;						// 1�ڰёO��HP

	// UI�\���n
	int hp;							// �̗�
	float angryPoint;				// �{��ް��
	int specialPoint;				// �K�E�ް��
	int padNum;						// �ޮ��߯�ނ̔ԍ� 0����3�܂�

	INV_INF invInf;					// ���G��Ԃ̶��ĉ񐔕��U����H����Ă������Ȃ�
	unsigned int visible;			// �\��:��\���؂�ւ�
	int speed;						// �ړ����x
	int jumpCnt;					// �ެ��߂ł����
	PARAM param;					// ��ڲԂ̏�ԁ@�ެ��߁A̫�فA��׳���
	LOCATE locate;					// ������ꏊ���i�[
	float vX;						// x������
	float vY;						// y�����x

	float jumpTime;
	bool fallFlag;					// �󒆂ŉ��L�[��������true�ɂȂ藎�����x����{�ɂȂ�

	bool floorCheckFlag;			// ��ڲ԰�̑���������荂���ʒu�ɂ���Ȃ�true

	sharedListFloor floorList;
	PARENT_FLOOR parentFloor;		// ���������n���Ă��黶ނ̏��

	int   framCnt;					// �o���ڰъi�[�p�ϐ�

	bool dashFlag;					// ���E��������x�������ޯ���B�ޯ�����Ȃ�true
	bool downFlag;					// ������f����2�x����������true
	int nowTimeLR;
	int nowTimeDown;

	unsigned int animId;			// ��Ұ��݂�id���i�[
	unsigned int animIdOld;			// 1�ڰёO�̱�Ұ��݂�id���i�[
	std::string imageName;			// �\���摜̧�ٖ�
	unsigned int animCnt;			// �ϐ������
	bool animEndFlag;				// ��Ұ��ݍŏI�ϓ��B�׸�
	unsigned int jumpEffectCnt;		// �ެ��߱�Ұ��ݗp����
	bool jumpEffectFlag;			// �ެ��ߴ̪�Ĕ�������true
	VECTOR2 jumpPos;				// �ެ��߂������W

	unsigned int damageInv;			// ���ڰь��Z����1�ɂȂ�����󂯂��U���̏�������
	unsigned int multiHitFlag;		// ����Ұ��ݺϓ����蔻�肷�邩�ǂ���	true:����
	unsigned int nextMultHitInv;	// ���i�q�b�g�U����1�x�U������Ă����莞�ԍU�����Ȃ���visible��L���ɂ���@�����Ɨǂ����O����
	DIR attackDir;					// �U�����ꂽ�������i�[

	DERI deri;						//�ʏ�U������̔h��
	int  multiAttackNum;			// �A���������ݸށ@	�ʏ�U��0�`�ʏ�U��2
	int  hitCnt;					// �U�����󂯂�����Z	��萔�ɂȂ�����0�ɂ��Ė��G��Ԃ� ���� ��莞�ԍU�����Ȃ���0��

	bool angryFlag;					// true:�Ԃ�����Ӱ�ށ@false:�ʏ�
	int  angryCnt;					// �Ԃ�����Ӱ�ނ̎���(�ڰѐ�)

	CHARGE_STAGE chargeStage;		// ���ߒi�K�@
	int  chargeCnt;					// ���ߍU���ŗ��߂Ă��鎞�ԁ@��莞�ԗ��߂���U����UP ���߱�Ұ��̶݂����

	bool spAttackFlag;				// �K�E�Z��������true
	int  spAttackCnt;				// �K�E�Z���Ȃ琔�l�������Ă���		0�Ȃ�K�E�Z���ł͂Ȃ�
	bool spAttackHitFlag;			// �K�E�Z��H����Ă��邩	true:�H����Ă�		false:�H����Ă��Ȃ�
	unsigned int spAttackInv;		// �K�E�Z��H����Ă����莞�ԍU�����󂯂Ȃ������疳�G��Ԃɂ��Ĉړ����\�ɂ���

	int guardTime;					// �ް�ގ��ԁ@�͈͂�0�`180�@�U�����󂯂�ƍU���͂ɔ�Ⴕ�Ēl������@�����Ȃ���Ώ��X�ɉ�

	float gravity;					// ���ڰщ��Z����@velocityY�̗ʁ@�P���V�̎��ȊO��0.4

	int guardImage;					// �ް�މ摜

	int fixCtrlCnt;					// �d�����Đ��l������Ԃ͓����Ȃ�

									// ˯Ĵ̪��
	HIT_EFFECT hitEffect;			// ˯Ĵ̪�Ă�ؽ�

	bool playerNumSprite;			// 1P2P�̕\���׸�

	bool deathFlag;					// ���S�׸�		true:���S	false:����

	XINPUT_STATE xstate[PLAYER_MAX];

	// SE�p
	int walkSeCnt;
	int angChargeSeCnt;
	int umbFloatCnt;
	int spSeCnt;
	bool spmaxSeFlag;
	bool chargeSeFlag;
	bool angerMaxSeFlag;

};