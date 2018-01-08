#pragma once
#include <Render.h>
#include "Bullet.h"

#define OVERHEAT 1 // ����. ���������
#define OVERHEAT_FOR_BULLET 50 // ���-�� ��������� �� ������� [1,100]

// ����� �����

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

	FPoint GetPositionForBullet(); // �������� ������� ��� �������� (��������� ���������� ��� ����)

	std::vector<Bullet*> GetBullets();

private:

	Render::Texture* _texCannon; // �������� �����

	FPoint _position; // ����� ���������� �� ��������� �����
	float _angle; // ���� ��������
	int _overheat; // ������� ��������� �����

	std::vector<Bullet*> _bullets; // �������� ����

};

