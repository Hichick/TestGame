#pragma once

#include <Render.h>

#define K_SPEED 10 // �����. ��������

// ����� ����
class Bullet
{
public:
	Bullet(FPoint position,int speed,float angle, EffectsContainer &effCont);
	~Bullet();

	void Draw();
	void Update(float dt);

	FPoint GetPosition();
	void SetHit(bool hit);
	bool GetHit();
	IRect GetIRect();
	ParticleEffectPtr GetEffect();

private:

	FPoint _position; // �������
	float _speed; // �������� ���� (����/���)
	float _angle; // ���� ������
	bool _hit; // ���� ��������� ����

	Render::Texture* _texBullet; // �������� ����

	ParticleEffectPtr _eff; // ������ ������

};


