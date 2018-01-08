#include "stdafx.h"
#include "Bullet.h"
#include <time.h>

Bullet::Bullet(FPoint position,int speed,float angle, EffectsContainer &effCont)
{

	_position = position;
	_speed = speed;
	_angle = angle;
	_hit = false;

	_texBullet = Core::resourceManager.Get<Render::Texture>("bullet");
	_eff = effCont.AddEffect("path");

	_eff->posX = _position.x;
	_eff->posY = _position.y;
	_eff->Reset();
}


void Bullet::Draw()
{
	// ������ ����, ���� ��� ��� �� ������ � ����
	if (!_hit) {
		Render::device.PushMatrix();
		
		Render::device.MatrixTranslate(math::Vector3(_position.x, _position.y, 0));
		Render::device.MatrixRotate(math::Vector3(0, 0, 1), _angle);

		IRect texRect = _texBullet->getBitmapRect();
		FRect rect(texRect), uv(0, 1, 0, 1);
		_texBullet->TranslateUV(rect, uv);

		Render::device.MatrixTranslate(math::Vector3(-texRect.width / 2, -texRect.height, 0.f));

		_texBullet->Bind();

		Render::DrawQuad(rect, uv);

		Render::device.PopMatrix();
	}
}


// ��������� ������� ���� � ������� ������
void Bullet::Update(float dt)
{
	_position.x += _speed * math::cos((_angle + 90) * math::PI / 180) * dt * K_SPEED;
	_position.y += _speed * math::sin((_angle + 90) * math::PI / 180) * dt * K_SPEED;

	_eff->posX = _position.x;
	_eff->posY = _position.y;
}

// �������� ������� ����
FPoint Bullet::GetPosition()
{
	return _position;
}

// ������������� ���� ���������
void Bullet::SetHit(bool hit)
{
	_hit = hit;
	_eff->Finish();
	_eff = NULL;
}

// �������� ���� ���������
bool Bullet::GetHit()
{
	return _hit;
}

// �������� ������������� ����
IRect Bullet::GetIRect()
{
	return _texBullet->getBitmapRect();
}

// �������� ������ ������
ParticleEffectPtr Bullet::GetEffect()
{
	return _eff;
}

Bullet::~Bullet()
{
}