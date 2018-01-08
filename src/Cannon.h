#pragma once
#include <Render.h>
#include "Bullet.h"

#define OVERHEAT 1 // Коэф. перегрева
#define OVERHEAT_FOR_BULLET 50 // Кол-во перегрева за выстрел [1,100]

// Класс Пушки

class Cannon
{
public:
	Cannon();
	~Cannon();

	void Draw();
	void Update(float dt);

	void AddBullet(int speed, EffectsContainer &effCont); 
	void DelBullet(int i); 

	void SetPosition(FPoint pos);
	FPoint GetPosition();
	void SetAngle(float angleB);
	float GetAngle();
	int GetOverheat();

	FPoint GetPositionForBullet(); // Получаем позицию для выстрела (начальную координату для пули)

	std::vector<Bullet*> GetBullets();

private:

	Render::Texture* _texCannon; // Текстура пушки

	FPoint _position; // Точка отвечающая за положение пушки
	float _angle; // Угол поворота
	int _overheat; // Процент перегрева пушки

	std::vector<Bullet*> _bullets; // Колекция пуль

};

