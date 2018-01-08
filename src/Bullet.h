#pragma once

#include <Render.h>

#define K_SPEED 10 // Коэфф. скорости

// Класс Пули
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

	FPoint _position; // Позиция
	float _speed; // Скорость пули (пикс/сек)
	float _angle; // Угол полета
	bool _hit; // Флаг попадания пули

	Render::Texture* _texBullet; // Текстура пули

	ParticleEffectPtr _eff; // Эффект шлейфа

};


