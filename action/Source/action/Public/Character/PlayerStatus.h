#pragma once

struct PlayerStatus {
public:
	// �Q�[�W�̍ő�l
	float MaxPower = 100.0f;

	// HP
	// HP + AP + SP = MaxPower
	// 0 < AP < SP -> AP������Ă�SP�ɂ͉e���Ȃ�
	float HP = 50.0f;

	float HPMax = 50.0f;

	// �񕜑��x
	float HealHPValue = 70.0f;
	float HealSPValue = 15.0f;
	float HealHPRatio = 0.5f;

	// �u�����N����
	const float BLINKTIME = 0.5f;

	// �u�����N�������
	float BlinkPower = 30.0f;

private:
};