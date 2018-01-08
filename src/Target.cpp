#include "stdafx.h"
#include "Target.h"
#include <string>

Target::Target()
{
	
	// ���������� ��������. � �������� �������
	_position_start = FPoint(PADDING, math::random(200, Render::device.Height() - PADDING));
	_position_end = FPoint(Render::device.Width() - PADDING, math::random(200, Render::device.Height() - PADDING));
	_position = _position_start;

	// ������� ��� ���� ����� ���� �������� � ������ �������
	if (math::random_bool())
	{
		_position_start = _position_end;
		_position_end = _position;
		_position = _position_start;
	}
	//-----------------------------------------------------

	_angle = math::atan((_position_end.y - _position_start.y), (_position_end.x - _position_start.x)) * 180 / math::PI; // ��������� ���� ������

	// ���������� ���� �� 3-� �����
	switch (math::random(1, 3))
	{
		case 1:
			_speed = math::random(100.f, 150.f);		
			_texTarget = Core::resourceManager.Get<Render::Texture>("target_blue");
			break;
		case 2:
			_speed = math::random(150.f, 250.f);
			_texTarget = Core::resourceManager.Get<Render::Texture>("target_red");
			break;
		case 3:
			_speed = math::random(50.f, 100.f);
			_texTarget = Core::resourceManager.Get<Render::Texture>("target_brown");
			break;
	}
	//-------------------------------------

	_scale = 1.0f;

	_destroy = false;
}

void Target::Draw()
{
	if (!_destroy) {
		Render::device.PushMatrix();

		Render::device.MatrixTranslate(math::Vector3(_position.x, _position.y, 0));

		IRect texRect = _texTarget->getBitmapRect();
		FRect rect(texRect), uv(0, 1, 0, 1);
		_texTarget->TranslateUV(rect, uv);

		Render::device.MatrixScale(_scale);
		Render::device.MatrixTranslate(math::Vector3(-texRect.width / 2, -texRect.height, 0.f));

		_texTarget->Bind();

		Render::DrawQuad(rect, uv);

		Render::device.PopMatrix();
	}

}

void Target::Update(float dt)
{
	// ������ �������, ���� ������ �������� �� ������� ����
	if ( (_position.x) > (Render::device.Width() - _texTarget->Width() / 2))
	{
		_position_start = _position_end;
		_position_end = FPoint(_texTarget->Width()/2, math::random(200, Render::device.Height()-PADDING));
		_position = _position_start;
		_angle = math::atan((_position_end.y - _position_start.y), (_position_end.x - _position_start.x)) * 180 / math::PI;
	}

	// ������ �������, ���� ������ �������� �� ������ ����
	if (_position.x < _texTarget->Width() / 2)
	{
		_position_start = _position_end;
		_position_end = FPoint(Render::device.Width() - _texTarget->Width() / 2, math::random(200, Render::device.Height()-PADDING));
		_position = _position_start;
		_angle = math::atan((_position_end.y - _position_start.y), (_position_end.x - _position_start.x)) * 180 / math::PI;
	}

	// ����������� ������� ������� ������
	_position.x += _speed * math::cos((_angle) * math::PI / 180) * dt;
	_position.y += _speed * math::sin((_angle) * math::PI / 180) * dt;
	
	//char str[100];
	//sprintf(str, "\nX: %f, Y: %f , sX: %f, sY: %f, eX: %f, eY: %f", _position.x, _position.y, _position_start.x, _position_start.y, _position_end.x, _position_end.y);
	//OutputDebugString(str);
}

// �������� ������� ������� �������
FPoint Target::GetPosition()
{
	return  _position;
}

// �������� ������ ����������� ������
ParticleEffectPtr Target::GetEffect()
{
	return _eff;
}


// ����� ��� �������� ��������� ���� �� ������
bool Target::CheckHit(FPoint position_bullet,IRect rect_bullet, EffectsContainer &effCont)
{
	IRect _texRect = _texTarget->getBitmapRect();

	_texRect.Inflate(-3); // ��������� ������������� ������ (������ ��� ������ ������� � ����� ����)

	// ������� ����������� ���� ����������
	if (
		!((position_bullet.x + rect_bullet.Width() / 2) < (_position.x - _texRect.Width()/2) || (position_bullet.x - rect_bullet.Width() / 2) > _position.x + _texRect.Width()/2 ) &&
		!((position_bullet.y ) < (_position.y - _texRect.Height()) || (position_bullet.y - rect_bullet.Height()) > _position.y )
		)
	{
		_eff = effCont.AddEffect("boom");
		_eff->posX = _position.x;
		_eff->posY = _position.y - _texRect.Height()/2;
		_eff->Reset();

		_destroy = true;
	}
	else
	{
		_destroy = false;
	}

	return _destroy;
}

Target::~Target()
{
}
