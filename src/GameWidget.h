#pragma once

#include "Cannon.h"
#include "Bullet.h"
#include "Target.h"
#include <vector>


// ��������� ����
enum status {START,GAME,END};

#define OVERHEAT_HEIGHT 7 // ������ ������� ���������

///
/// ������ - �������� ���������� ������� �� ������.
/// �� ������������ ����, � ����� ����� ��������� ������ �������.
///
class GameWidget : public GUI::Widget
{
public:
	GameWidget(const std::string& name, rapidxml::xml_node<>* elem);

	void Draw() override;
	void Update(float dt) override;
	
	void AcceptMessage(const Message& message) override;
	
	bool MouseDown(const IPoint& mouse_pos) override;
	void MouseMove(const IPoint& mouse_pos) override;
	void MouseUp(const IPoint& mouse_pos) override;

	void KeyPressed(int keyCode) override;
	void CharPressed(int unicodeChar) override;

	void Blackout(int red, int green, int blue, int alpha);
	void DrawTexture(Render::Texture* tex,FPoint position);
	bool PointHitRect(IPoint point, IRect rect);
	void DrawOverheat(Color rgba,int overheat,int height);


private:
	void Init();

private:

	int Time = 0; // ����� ���������� �� ���� � ��������
	int CountTarget = 0; // ���������� �����
	int Speed = 0; // �������� ����� ����

	float _timer; // ������
	status _status; // ������� ��������� ����
	int _score; // ������� ���������� �����

	Cannon* _cannon; // �����
	std::vector<Target*> _targets; // ����

	Render::Texture* _texBackground; // �������� ����
	Render::Texture* _texStart; // �������� ������ �����
	Render::Texture* _texRepeat; // �������� ������ ���������
					 
	EffectsContainer	_effCont; // ��������� � ���������

	bool _start; // ���� ���������� ������, ���� ������ ������� �� ������ �����
	bool _repeat; // ���� ���������� ������, ���� ������ ������� �� ������ �������
};
