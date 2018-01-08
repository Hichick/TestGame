#pragma once
#include <Render.h>
#include <vector>

#define PADDING 10 // ������� �� ���� � ��������

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

	FPoint _position_start; // ������� ������ ��� ������ ��������
	FPoint _position_end; // ������� ������ ��� ����� ��������
	FPoint _position; // ������� �������
	float _speed; // �������� ������
	float _angle; // ���� ������
	float _scale; // ������� ������

	Render::Texture* _texTarget; // �������� ������
	ParticleEffectPtr _eff; // ������ ����������� ������

	bool _destroy; // ���� ����������� ������
	
};

