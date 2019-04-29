#pragma once
#include <array>

using KEY_ARRAY = std::array<char, 256>;

enum KEY_TYPE
{
	KEY_TYPE_NOW,
	KEY_TYPE_OLD,
	KEY_TYPE_MAX
};

class GameCtrl
{
public:
	GameCtrl();
	~GameCtrl();

	const KEY_ARRAY &GetCtrl(KEY_TYPE type) const;		// ˆø”‚É‘Î‰‚µ‚½À²Ğİ¸Ş‚Ì·°“ü—Íî•ñ‚ğæ“¾‚·‚é
	bool UpDate(void);		// ·°‚Ì“ü—Íî•ñ‚ÌXV‚ğ‚·‚é

private:
	KEY_ARRAY data;			// Œ»İ‚ÌÌÚ°Ñ‚Ì·°“ü—Íî•ñ‚ğŠi”[‚·‚é
	KEY_ARRAY dataOld;		// 1ÌÚ°Ñ‘O‚ÌÌÚ°Ñ‚Ì·°“ü—Íî•ñ‚ğŠi”[‚·‚é
};
