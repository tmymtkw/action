#pragma once

struct PlayerStatus {
public:
	// ゲージの最大値
	float MaxPower = 100.0f;

	// HP
	// HP + AP + SP = MaxPower
	// 0 < AP < SP -> APを消費してもSPには影響なし
	float HP = 50.0f;

	float HPMax = 50.0f;

	// 回復速度
	float HealHPValue = 70.0f;
	float HealSPValue = 15.0f;
	float HealHPRatio = 0.5f;

	// ブリンク時間
	const float BLINKTIME = 0.5f;

	// ブリンク時消費量
	float BlinkPower = 30.0f;

private:
};