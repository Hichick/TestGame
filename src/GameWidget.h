#pragma once

#include "Cannon.h"
#include "Bullet.h"
#include "Target.h"
#include <vector>


// Состояния игры
enum status {START,GAME,END};

#define OVERHEAT_HEIGHT 7 // Высота полоски перегрева

///
/// Виджет - основной визуальный элемент на экране.
/// Он отрисовывает себя, а также может содержать другие виджеты.
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

	int Time = 0; // Время выделяемое на игру в секундах
	int CountTarget = 0; // Количество целей
	int Speed = 0; // Скорость полет пули

	float _timer; // Таймер
	status _status; // Текущее состояние игры
	int _score; // Счетчик пораженных целей

	Cannon* _cannon; // Пушка
	std::vector<Target*> _targets; // Цели

	Render::Texture* _texBackground; // Текстура фона
	Render::Texture* _texStart; // Текстура кнопки старт
	Render::Texture* _texRepeat; // Текстура кнопки повторить
					 
	EffectsContainer	_effCont; // Контейнер с эффектами

	bool _start; // Флаг возвращает истину, если курсор наведен на кнопку старт
	bool _repeat; // Флаг возвращает истину, если курсор наведен на кнопку рестарт
};
