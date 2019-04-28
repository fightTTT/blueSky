#pragma once

enum class PARAM {
	JUMPING,		// 上昇中　加速度が0より小さい
	FALLING,		// 落下中　加速度が0より大きい
	MAX
};