#pragma once
#include <Render.h>
#include <vector>

#define PADDING 10 // Отступы от края в пикселях

class Target
{
public:
	Target();
	~Target();

	void Draw();
	void Update(float dt);

	FPoint GetPosition();
	ParticleEffectPtr GetEffect();

	bool CheckHit(FPoint position_bullet,IRect rect_bullet,EffectsContainer &effCont);

private:

	FPoint _position_start; // Позиция мишени для начала движения
	FPoint _position_end; // Позиция мишени для конца движения
	FPoint _position; // Текущая позиция
	float _speed; // Скорость мишени
	float _angle; // Угол полета
	float _scale; // Масштаб мишени

	Render::Texture* _texTarget; // Текстура мишени
	ParticleEffectPtr _eff; // Эффект уничтожения мишени

	bool _destroy; // Флаг уничтожения мишени
	
};

