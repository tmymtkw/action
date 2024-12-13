#pragma once

struct MovementStatus {
	// XY�����̈ړ��p�����[�^
	// ���s���x
	const float WALK = 300.0f;
	// �X�v�����g���x
	const float SPRINT = 150.0f;

	// Z�����̈ړ��p�����[�^
	// �W�����v�����x
	const float JUMP = 15.0f;
	// ���������x
	const float FALL = 40.0f;
	// �������x�̍ő�l
	const float FALLMAX = -15.0f;

	// �u�����N�p�����[�^
	// �u�����N���x
	const float BLINK = 1000.0f;
	// �u�����N����
	const float BLINKTIME = 1.0f;
	//const float BLINKDEC = 5.0f;

	// �R���|�[�l���g�̉�]���x
	const float ROTATE = 500.0f;
};