#include "stdafx.h"
#include "Cannon.h"

Cannon::Cannon()
{
	_position.x = Render::device.Width() / 2;
	_position.y = 10;
	_angle = 0;
	_overheat = 0;
	_texCannon = Core::resourceManager.Get<Render::Texture>("cannon");
}

void Cannon::Draw()
{
	// �������� ������� � ��������� ����
	if (!_bullets.empty())
	{
		for (int i=0;i<_bullets.size();i++)
		{
			_bullets[i]->Draw();
		}
	}

	Render::device.PushMatrix();

	Render::device.MatrixTranslate(math::Vector3(_position.x, _position.y, 0));
	Render::device.MatrixRotate(math::Vector3(0, 0, 1), _angle);

	IRect texRect = _texCannon->getBitmapRect();
	FRect rect(texRect), uv(0, 1, 0, 1);
	_texCannon->TranslateUV(rect, uv);

	// ������� �� �������� ������ ��������, ����� �������� ���� �� ��������
	Render::device.MatrixTranslate(math::Vector3(-texRect.width / 2, 0, 0.f));

	_texCannon->Bind();

	Render::DrawQuad(rect, uv);

	Render::device.PopMatrix();
}


void Cannon::Update(float dt)
{
	if (!_bullets.empty())
	{
		for (int i = 0; i<_bullets.size(); i++)
		{
			_bullets[i]->Update(dt);

			// �������� ����, ���� �������� �� ������� ������
			if (_bullets[i]->GetPosition().x > Render::device.Width() ||
				_bullets[i]->GetPosition().x < 0 ||
				_bullets[i]->GetPosition().y > Render::device.Height() ||
				_bullets[i]->GetPosition().y < 0 
				)
			{
				DelBullet(i);
			}
		}
	}

	
	_overheat -= OVERHEAT * dt; // C������ ��������

	if (_overheat < 0)
	{
		_overheat = 0;
	}

	IPoint mouse_pos = Core::mainInput.GetMousePos();

	// ����������� ���� ��������, ������������ ��������� ����
	_angle = math::atan((mouse_pos.y - _position.y), (mouse_pos.x - _position.x)) * 180 / math::PI - 90;
}

// ��������� ���� � �������� � ����������� ��������
void Cannon::AddBullet(int speed, EffectsContainer &effCont)
{
	_bullets.push_back(new Bullet(GetPositionForBullet(),speed,_angle, effCont));
	_overheat += OVERHEAT_FOR_BULLET;
}

// �������� ����
void Cannon::DelBullet(int i)
{
	if (_bullets[i]->GetEffect())
	{
		_bullets[i]->GetEffect()->Kill();
	}

	delete _bullets[i];
	_bullets.erase(_bullets.begin() + i);
}

// ������������� ������� �����
void Cannon::SetPosition(FPoint pos)
{
	_position.x = pos.x;
	_position.y = pos.y;
}

// �������� ������� �����
FPoint Cannon::GetPosition()
{
	return _position;
}

// ������������� ���� ��������
void Cannon::SetAngle(float angleB)
{
	_angle = angleB;
}

// ��������� ���� ��������
float Cannon::GetAngle()
{
	return _angle;
}

// �������� �������� ��������
int Cannon::GetOverheat()
{
	return _overheat;
}

// �������� ������� ��� �������� (��������� ���������� ��� ����)
FPoint Cannon::GetPositionForBullet()
{
	FPoint p2;

	float length = 0.f, y = 0.f, x = 0.f;

	length = _texCannon->Height();
	y = cosf(_angle*math::PI/180)*length;

	p2.y = y + _position.y;

	x = sqrtf(length*length - y*y);

	if (_angle >= 0)
	{
		p2.x = _position.x - x;
	}
	else
	{
		p2.x = _position.x + x;
	}
	
	return p2;
}

// �������� ��������� ����
std::vector<Bullet*> Cannon::GetBullets()
{
	return _bullets;
}

Cannon::~Cannon()
{

}