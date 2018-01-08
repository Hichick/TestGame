#pragma once

#include "Cannon.h"
#include "Bullet.h"
#include <vector>

///
/// Виджет - основной визуальный элемент на экране.
/// Он отрисовывает себя, а также может содержать другие виджеты.
///
class TestWidget : public GUI::Widget
{
public:
	TestWidget(const std::string& name, rapidxml::xml_node<>* elem);

	void Draw() override;
	void Update(float dt) override;
	
	void AcceptMessage(const Message& message) override;
	
	bool MouseDown(const IPoint& mouse_pos) override;
	void MouseMove(const IPoint& mouse_pos) override;
	void MouseUp(const IPoint& mouse_pos) override;

	void KeyPressed(int keyCode) override;
	void CharPressed(int unicodeChar) override;

private:
	void Init();

private:
	float _timer;
	
	float _scale;
	float _angle;
	
	Render::Texture* _tex1;
	Render::Texture* _tex2;
	Render::Texture* _tex3;
	Render::Texture* _texBG;
	Render::Texture* _texCanon;
	Render::Texture* _texBall;

	int _curTex;

	bool shoot = false;

	EffectsContainer _effCont;
	ParticleEffectPtr _eff;

	TimedSpline<FPoint> spline;

	Cannon* can;

	std::vector<Bullet*> v_bullet;

};
