#pragma once

struct MovementStatus {
	// XY方向の移動パラメータ
	// 歩行速度
	const float WALK = 300.0f;
	// スプリント速度
	const float SPRINT = 150.0f;

	// Z方向の移動パラメータ
	// ジャンプ初速度
	const float JUMP = 15.0f;
	// 落下加速度
	const float FALL = 40.0f;
	// 落下速度の最大値
	const float FALLMAX = -15.0f;

	// ブリンクパラメータ
	// ブリンク速度
	const float BLINK = 1000.0f;
	// ブリンク時間
	const float BLINKTIME = 1.0f;
	//const float BLINKDEC = 5.0f;

	// コンポーネントの回転速度
	const float ROTATE = 500.0f;
};