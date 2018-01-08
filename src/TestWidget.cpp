#include "stdafx.h"
#include "TestWidget.h"
#include "Cannon.h"
#include "Bullet.h"

TestWidget::TestWidget(const std::string& name, rapidxml::xml_node<>* elem)
	: Widget(name)
	, _curTex(0)
	, _timer(0)
	, _angle(0)
	, _eff(NULL)
	, _scale(0.f)
{
	Init();
}

void TestWidget::Init()
{
	
	_tex1 = Core::resourceManager.Get<Render::Texture>("btnStart_Text");
	_tex2 = Core::resourceManager.Get<Render::Texture>("Circle");
	_tex3 = Core::resourceManager.Get<Render::Texture>("Star");
	_texBG = Core::resourceManager.Get<Render::Texture>("background");
	//_texCanon = Core::resourceManager.Get<Render::Texture>("canon");
	//_texBall = Core::resourceManager.Get<Render::Texture>("ball");

	_curTex = 0;
	_angle = 0;

	spline.addKey(0.0f, FPoint(0.0f, 400.0f));
	spline.addKey(0.1f, FPoint(51.0f, 300.0f));
	spline.addKey(0.2f, FPoint(153.0f, 400.0f));
	spline.addKey(0.3f, FPoint(254.0f, 300.0f));
	spline.addKey(0.4f, FPoint(355.0f, 400.0f));
	spline.addKey(0.5f, FPoint(456.0f, 300.0f));
	spline.addKey(0.6f, FPoint(568.0f, 400.0f));
	spline.addKey(0.7f, FPoint(669.0f, 300.0f));
	spline.addKey(0.8f, FPoint(770.0f, 400.0f));
	spline.addKey(0.9f, FPoint(880.0f, 300.0f));
	spline.addKey(1.0f, FPoint(1024.0f, 400.0f));
	spline.CalculateGradient();


	can = new Cannon(
		FPoint(Render::device.Width() / 2 ,10), 
		Core::resourceManager.Get<Render::Texture>("canon")
	);
}

float angle(int x1, int y1, int x2, int y2)
{
	float t = (x1*x2 + y1*y2) / (sqrt((float)x1*x1 + y1*y1)*sqrt((float)x2*x2 + y2*y2));
	if (t<-1) t = -1;
	else if (t> 1) t = 1;

	float angle = acosf(t) * 180 / math::PI;
	
	// ������������ �������� ��� ���� ����� 88 ��������
	if (angle < 88.0f)
	{
		return angle;
	}
	else
	{
		return 88.0f;
	}

}

void TestWidget::Draw()
{

	_texBG->Draw();

	
	// ������ ��� Y
	Render::BeginColor(Color(255, 128, 0, 255));
	Render::DrawLine(FPoint(Render::device.Width() / 2, 0), FPoint(Render::device.Width() / 2, Render::device.Height()));
	Render::DrawLine(can->GetPositionForShoot(), FPoint(Render::device.Width() / 2, Render::device.Height()));
	Render::EndColor();

	int i = 0;

	for (i = 0; i < v_bullet.size(); i++)
	{


		if ((v_bullet[i]->GetCurPosition().y + 2)<v_bullet[i]->GetEndPosition().y)
		{

			Render::device.PushMatrix();

			v_bullet[i]->SetCurPosition(_timer);

			Render::device.MatrixTranslate(v_bullet[i]->GetCurPosition().x, v_bullet[i]->GetCurPosition().y, 0);
			v_bullet[i]->GetTexture()->Draw();

			Render::device.PopMatrix();

			

		}
		else
		{
			v_bullet.erase(v_bullet.begin() + i);
		}


	}
	
	//
	// �������� ������� ��������� ������� ����.
	//
	IPoint mouse_pos = Core::mainInput.GetMousePos();


	// �������� �������� �����

	Render::device.PushMatrix();

	if (mouse_pos.x > Render::device.Width()/2)
	{
		can->SetPosition(FPoint(Render::device.Width() / 2 + can->GetTexture()->Width() / 2,10));
		can->SetAngle((-angle(0, mouse_pos.y, Render::device.Width() / 2 - mouse_pos.x, mouse_pos.y)));

		Render::device.MatrixTranslate(can->GetPosition().x, can->GetPosition().y, 0);
		Render::device.MatrixRotate(math::Vector3(0, 0, 1), can->GetAngle());
		Render::device.MatrixScale(-1.f, 1.f, 0.f); // ������������� (������ ��� ������� ������������� ������������ ������ ������� ���� � ��� �������� �� -30(330) �������� �������� ��������)
	}
	else
	{
		can->SetPosition(FPoint(Render::device.Width() / 2 - can->GetTexture()->Width() / 2, 10));
		can->SetAngle((angle(0, mouse_pos.y, Render::device.Width() / 2 - mouse_pos.x, mouse_pos.y)));
		
		Render::device.MatrixTranslate(can->GetPosition().x, can->GetPosition().y, 0);
		Render::device.MatrixRotate(math::Vector3(0, 0, 1), can->GetAngle());
	}
	
	can->GetTexture()->Draw();

	Render::device.PopMatrix();
	//------------------------------------

	
	

	


	/*TimedSpline<FPoint> shoot_spline;

	shoot_spline.addKey(0.0f, FPoint(Render::device.Width() / 2, 10.0f));
	shoot_spline.addKey(1.0f, FPoint(Render::device.Height() / math::cos(angle(0, mouse_pos.y, Render::device.Width() / 2 - mouse_pos.x, mouse_pos.y)), Render::device.Height()));
	shoot_spline.CalculateGradient();

	FPoint current_pos = shoot_spline.getGlobalFrame(math::clamp(0.0f, 1.0f, _timer / 12.56f));

	Render::device.PushMatrix();

	Render::device.MatrixTranslate(current_pos.x, current_pos.y, 0);
	_texBall->Draw();

	Render::device.PopMatrix();*/

	


	//
	// ������������ � ���� ������� �������������� ���������, ����� � ����������
	// ����� ���� ������������ ��� �������������� ������� PopMatrix.
	//
	Render::device.PushMatrix();
	
	//
	// �������� ������� �������������� ���������, ��������� ����� ��������� � ������� ����
	// � ����������� ���������� ������������ ����� ������ ������ ��� z �� ���� _angle.
	//
	Render::device.MatrixTranslate((float)mouse_pos.x, (float)mouse_pos.y, 0);
	Render::device.MatrixRotate(math::Vector3(0, 0, 1), _angle);

	if (!_curTex)
	{
		//
		// ����� Texture::Draw() ������� ���������� ������ � ��������� ��������
		// � ������ ��������� (0, 0). ����� ��������� � �������������� ��������� ������
		// ������� ��������������� � ����������� �������� ��������������.
		//
		// ��� ������ ������ Texture::Draw() �������� Texture::Bind() �������������.
		//
		_tex1->Draw();
	}
	else
	{
		IRect texRect = _tex2->getBitmapRect();

		//
		// ��� ��������� �������� ����� ������� �������� UV ���������� ��� ����� ��������,
		// ������� ����� �������� �� ������� �������. UV ���������� ������ ���� �������������,
		// �.�., ��� ����������� �� ������� �������� � ��������, ������ ����� ��������
		// ����� 1 (UV ���������� �������� � ��������� 0..1, ���� ����� �� ������ ������ ��
		// ������ �������, ��� ���� � ������ ������������� ��������� �������� REPEAT, ��������
		// ����� ���������� �� ���� ������� ��������������� ���������� ���).
		//

		FRect rect(texRect);
		FRect uv(0, 1, 0, 1);

		_tex2->TranslateUV(rect, uv);

		Render::device.MatrixScale(_scale);
		Render::device.MatrixTranslate(-texRect.width * 0.5f, -texRect.height * 0.5f, 0.0f);

		//
		// ����������� ��������.
		//
		_tex2->Bind();
		
		//
		// ����� DrawQuad() ������� � ����������� ���������� ���������� ������, ��������� �� ����
		// ���������� - �������������, ��������� ��� ���� ������� ���� ��� ������ � ����������� (binded) ��������,
		// ���� ��������� ���������������.
		//
		// ����� ������� DrawQuad() ������ ���� ����� Texture::Bind() ���� SetTexturing(false),
		// ����� ���������� ��������� ����� ���������������.
		//
		Render::DrawQuad(rect, uv);
	}

	//
	// ��������������� ������� �������������� ���������, ������ �� ����� ��������� �����.
	//
	Render::device.PopMatrix();
	
	//
	// �������� ������� ���������� �������, ������������ �� �������
	//
	FPoint currentPosition = spline.getGlobalFrame(math::clamp(0.0f, 1.0f, _timer/12.56f ));

	//
	// � ������ ������ � ���� �����������
	//
	Render::device.PushMatrix();
	Render::device.MatrixTranslate(currentPosition.x, currentPosition.y, 0);
	_tex3->Draw();
	Render::device.PopMatrix();


	

	//
	// ���� ����� ��������� ��������������� ��� ���������.
	//
	Render::device.SetTexturing(false);
	
	//
	// ����� BeginColor() ������������ � ���� ������� ���� ������ � ������������� �����.
	//
	Render::BeginColor(Color(255, 128, 0, 255));
	
	//
	// ����� DrawRect() ������� � ����������� ���������� ���������� ������, ��������� �� ����
	// ���������� - �������������, ��������� ��� ���� ������� ���� ��� ������ � ����������� (binded) ��������,
	// ���� ��������� ���������������.
	//
	// ����� ������� DrawRect() ������ ���� ����� Texture::Bind(), ���� SetTexturing(false),
	// ����� ���������� ��������� ����� ���������������.
	//
	Render::DrawRect(924, 0, 100, 100);
	
	//
	// ����� EndColor() ������� �� ����� ������� ���� ������, �������������� �������.
	//
	Render::EndColor();
	
	//
	// ����� �������� ���������������.
	//
	Render::device.SetTexturing(true);

	//
	// ������ ��� �������, ������� �������� � ��������� (Update() ��� ���������� �������� �� �����).
	//
	_effCont.Draw();

	Render::BindFont("arial");
	Render::PrintString(924 + 100 / 2, 35, utils::lexical_cast(mouse_pos.x) + ", " + utils::lexical_cast(mouse_pos.y), 1.f, CenterAlign);
	Render::PrintString(924 + 100 / 2, 55, "Angle: " + utils::lexical_cast(angle(0,mouse_pos.y, Render::device.Width() / 2 - mouse_pos.x, mouse_pos.y)) , 1.f, CenterAlign);
	Render::PrintString(924 + 100 / 2, 85, "x: " + utils::lexical_cast(can->GetPositionForShoot().x), 1.f, CenterAlign);
	Render::PrintString(924 + 100 / 2, 65, "y: " + utils::lexical_cast(can->GetPositionForShoot().y), 1.f, CenterAlign);

	
}

void TestWidget::Update(float dt)
{
	//
	// ������� ��������� � ���������
	//
	_effCont.Update(dt);

	//
	// dt - �������� ������� � ��������, ��������� �� ����������� �����.
	// ��� ����� ��������� ������ ��������, � ����������� �� ������������������ �������
	// � ��������� �����.
	//
	// ��� ����, ����� ���� �������� �������� ������ �� �������, � �� �������� ��
	// ������������������ �������, �� ������ ������������ � �� ����� ��������.
	//
	// ����������� ��� ������ � ��������� ���������.
	//
	_timer += dt * 2;
	
	//
	// ����������� ������ � ��������� (0, 2�).
	// ��� ����� ������ ��� �������������� ��������� ������������ ��������,
	// ���� ����� ������������ ��������� ����� (float ������������) ��� ��������
	// �� ����������� ����� ���� ������� ������� (������� �������-�-��).
	//
	// �������� �������� ������ ������ (0, 2�), ������ ��� �� ���������� ��� ��������
	// ��� ������� ������, � ������� �������� ����� ������ ������������ ��������� ���������.
	//
	while (_timer > 4 * math::PI)
	{
		_timer -= 4 * math::PI;
	}
	
	//
	// ������������ ��������� ��������������� � ����������� �� �������.
	//
	_scale = 0.8f + 0.25f * sinf(_timer);

	
	
}

bool TestWidget::MouseDown(const IPoint &mouse_pos)
{
	if (Core::mainInput.GetMouseRightButton())
	{
		//
		// ��� ������� �� ������ ������ ����, ������ ������ ������ �� ������.
		//
		_eff = _effCont.AddEffect("Iskra");
		_eff->posX = mouse_pos.x + 0.f;
		_eff->posY = mouse_pos.y + 0.f;
		_eff->Reset();
		
		//
		// � �������� ���� ������� ��������.
		//
		_angle += 25;
		while (_angle > 360)
		{
			_angle -= 360;
		}
	}
	else
	{

		
		 v_bullet.push_back(new Bullet(can->GetPositionForShoot(), FPoint(512.f, 600.f),_timer));
		
		


		//
		// ��� ������� �� ����� ������ ����, ������ ��������� ������, ������� ���������� ���.
		//
		//ParticleEffectPtr eff = _effCont.AddEffect("FindItem2");
		//eff->posX = mouse_pos.x + 0.f;
		//eff->posY = mouse_pos.y + 0.f;
		//eff->Reset();

		//
		// �������� �������� � 0 �� 1 � ��������.
		//
		//_curTex = 1 - _curTex;
	}
	return false;
}

void TestWidget::MouseMove(const IPoint &mouse_pos)
{
	if (_eff)
	{
		//
		// ���� ������ ������, �� ���������� ��� � ������� ����.
		//
		_eff->posX = mouse_pos.x + 0.f;
		_eff->posY = mouse_pos.y + 0.f;
	}
}

void TestWidget::MouseUp(const IPoint &mouse_pos)
{
	if (_eff)
	{
		//
		// ���� ������ ������, �� ��� ���������� ���� ��������� ���.
		//
		_eff->Finish();
		_eff = NULL;
	}
}

void TestWidget::AcceptMessage(const Message& message)
{
	//
	// ������� ����� ���������� ��������� � �����������.
	//

	const std::string& publisher = message.getPublisher();
	const std::string& data = message.getData();
}

void TestWidget::KeyPressed(int keyCode)
{
	//
	// keyCode - ����������� ��� �������.
	// � �������� �������� ��� �������� ����� ������������ ��������� VK_.
	//

	if (keyCode == VK_A) {
		// ������� �� ������� ������ A
	}
}

void TestWidget::CharPressed(int unicodeChar)
{
	//
	// unicodeChar - Unicode ��� ��������� �������
	//

	if (unicodeChar == L'�') {
		// ������� �� ���� ������� '�'
	}
}
