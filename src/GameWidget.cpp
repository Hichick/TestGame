#include "stdafx.h"
#include "GameWidget.h"
#include "Cannon.h"
#include "Bullet.h"
#include "Target.h"

GameWidget::GameWidget(const std::string& name, rapidxml::xml_node<>* elem)
	: Widget(name)
	, Time(0)
	, CountTarget(0)
	, Speed(0)
	, _timer(0)
	, _start(false)
	, _repeat(false)
{
	Init();
}

// Функция для считывания значений переменных из файла
int ReadValueFromFile(char name_file[], char name_var[])
{
	FILE *file;

	if ( (file = fopen(name_file, "r")) == NULL )
	{
		return 1;
	}

	char name[20] = "";
	int value = 0;
	int var_value = 1;

	while (fscanf(file, "%s = %d;", name, &(value)) != EOF) {

		if (strcmp(name, name_var)==0)
		{
			var_value = value;
			break;
		}
	}

	return var_value;
}

void GameWidget::Init()
{
	_texBackground = Core::resourceManager.Get<Render::Texture>("background");
	_texStart = Core::resourceManager.Get<Render::Texture>("start_normal");
	_texRepeat = Core::resourceManager.Get<Render::Texture>("repeat_normal");

	_status = START; // Устанавливаем начальный экран

	_score = 0;

	_cannon = new Cannon();

	// Считываем параметры из файла
	CountTarget = ReadValueFromFile("input.txt", "CountTarget");
	Speed = ReadValueFromFile("input.txt", "Speed");
	Time = ReadValueFromFile("input.txt", "Time");
	//-----------------------------------

	// Генерируем цели
	for (int i=0;i<CountTarget;i++)
	{
		_targets.push_back(new Target());
	}
	
}

void GameWidget::Draw()
{
	_texBackground->Draw(); // Рисуем текстуру фона
	
	switch (_status)
	{
		case START:

			Blackout(0,0,0,128); // Рисуем затемнение экрана
			Render::BindFont("lobster_big");
			Render::PrintString(Render::device.Width() / 2, Render::device.Height() / 2, "START  GAME", 1.f, CenterAlign);
			DrawTexture(_texStart,FPoint(Render::device.Width() / 2, Render::device.Height() / 2 - 300)); // Рисуем кнопку старт
			break;

		case GAME:

			// Рисуем мишени
			for (int i = 0; i<_targets.size(); i++)
			{
				_targets[i]->Draw();
			}

			
			_cannon->Draw(); // Рисуем пушку

			_effCont.Draw(); // Рисуем эффекты


			// Рисуем полосу перегрева
			if (_cannon->GetOverheat()>50)
			{
				if (_cannon->GetOverheat()>75)
				{
					DrawOverheat(Color(255, 50, 50, 255), _cannon->GetOverheat(), OVERHEAT_HEIGHT);
				}
				else
				{
					DrawOverheat(Color(250, 200, 80, 255), _cannon->GetOverheat(), OVERHEAT_HEIGHT);
				}	
			}
			else
			{
				DrawOverheat(Color(166, 220, 220, 255), _cannon->GetOverheat(), OVERHEAT_HEIGHT);
			}
			
			Render::BindFont("lobster");
			Render::PrintString(Render::device.Width() / 2, Render::device.Height() - 20, "Timer: " + utils::lexical_cast(Time - (int)_timer), 1.f, CenterAlign);
			Render::PrintString(20, Render::device.Height() - 20, "Score: " + utils::lexical_cast(_score), 1.f, LeftAlign);
			Render::PrintString(20, 40, "Overheat ", 1.f, LeftAlign);

			break;
		case END:

			Blackout(0, 0, 0, 128);
			Render::BindFont("lobster_big");
			
			// Выводим результаты игры
			if (_score == CountTarget)
			{
				Render::PrintString(Render::device.Width() / 2, Render::device.Height() / 2, "YOU WIN!!! \nScore: " + utils::lexical_cast(_score) + " of " + utils::lexical_cast(CountTarget), 1.f, CenterAlign);
			}
			else
			{
				if (_timer > Time)
				{
					Render::PrintString(Render::device.Width() / 2, Render::device.Height() / 2 + 100, "YOU LOSE!!!\nTime is out\nScore: " + utils::lexical_cast(_score) + " of " + utils::lexical_cast(CountTarget), 1.f, CenterAlign);
				}
				else
				{
					Render::PrintString(Render::device.Width() / 2, Render::device.Height() / 2 + 100, "YOU LOSE!!!\nCannon was overheat\nScore: " + utils::lexical_cast(_score) + " of " + utils::lexical_cast(CountTarget), 1.f, CenterAlign);

				}
				
			}
			
			DrawTexture(_texRepeat, FPoint(Render::device.Width() / 2, Render::device.Height() / 2 - 300)); // Рисуем кнопку рестарт
			
			break;
		
	}


}

void GameWidget::Update(float dt)
{

	switch (_status)
	{
		case START:
			_timer = 0;
			break;
		case GAME:

			_cannon->Update(dt);

			_effCont.Update(dt);

			for (int i = 0; i<_targets.size(); i++)
			{
				_targets[i]->Update(dt);
			}


			// Проверяем попадание пули по целям
			for (int i = 0; i<_targets.size(); i++)
			{
				bool temp = false; // Временный Флаг попадания

				for (int j = 0; j < _cannon->GetBullets().size(); j++)
				{
					temp = _targets[i]->CheckHit(_cannon->GetBullets()[j]->GetPosition(), _cannon->GetBullets()[j]->GetIRect(),_effCont);

					if (temp)
					{
						_cannon->DelBullet(j); 

						_targets[i]->GetEffect()->Finish();
						_targets[i]->GetEffect() = NULL;

						delete _targets[i];
						_targets.erase(_targets.begin() + i);

						_score++;
						break;
					}

				}

			}


			// Проверка условий окончания игры
			if (Time - (int)_timer <= 0 || _score == CountTarget || _cannon->GetOverheat()>100)
			{
				_status = END;
			}

			_timer += dt;

			break;
		case END:
			
			break;

	}

	
}

// Обрабатываем событие нажатия мыши
bool GameWidget::MouseDown(const IPoint &mouse_pos)
{
	if (Core::mainInput.GetMouseRightButton())
	{

	}
	else
	{

		switch (_status)
		{
		case START:

			// Нажатие ЛКМ на кнопку старт
			if (_start)
			{
				_status = GAME;
			}

			break;
		case GAME:

			_cannon->AddBullet(Speed,_effCont); // Добавляем пулю при нажатия ЛКМ во время игры

			break;
		case END:

			// Нажатие ЛКМ на кнопку рестарт
			if (_repeat)
			{
				delete _cannon;
				_effCont.KillAllEffects();

				for (int i = 0; i<_targets.size(); i++)
				{
					delete _targets[i];
				}

				_targets.clear();
				Init();
			}

			break;
		
		
		}

		

	}

	return false;
}

// Обрабатываем событие движения мыши
void GameWidget::MouseMove(const IPoint &mouse_pos)
{

	switch (_status)
	{
		case START:
			{
				IRect rect; // Прямоугольник кнопки старт
				rect.x = Render::device.Width() / 2 - _texStart->Width() / 2;
				rect.y = Render::device.Height() / 2 - 300;
				rect.width = _texStart->Width();
				rect.height = _texStart->Height();

				// Проверка нахождения курсора мыши на кнопке старт
				if (PointHitRect(mouse_pos, rect))
				{
					_texStart = Core::resourceManager.Get<Render::Texture>("start_hover");
					_start = true;
				}
				else
				{
					_texStart = Core::resourceManager.Get<Render::Texture>("start_normal");
					_start = false;
				}

				break;
			}
		case GAME:
			break;
		case END:
				{
					IRect rect; // Прямоугольник кнопки рестарт
					rect.x = Render::device.Width() / 2 - _texRepeat->Width() / 2;
					rect.y = Render::device.Height() / 2 - 300;
					rect.width = _texRepeat->Width();
					rect.height = _texRepeat->Height();

					// Проверка нахождения курсора мыши на кнопке рестарт
					if (PointHitRect(mouse_pos, rect))
					{
						_texRepeat = Core::resourceManager.Get<Render::Texture>("repeat_hover");
						_repeat = true;
					}
					else
					{
						_texRepeat = Core::resourceManager.Get<Render::Texture>("repeat_normal");
						_repeat = false;
					}

					break;
				}
			
	}
	
}

void GameWidget::MouseUp(const IPoint &mouse_pos)
{
	
	
}

// Рисуем затемнение экрана
void GameWidget::Blackout(int red,int green, int blue,int alpha)
{
	Render::device.SetTexturing(false);
	Render::BeginColor(Color(red, green, blue, alpha));
	Render::DrawRect(0, 0, Render::device.Width(), Render::device.Height());
	Render::EndColor();
	Render::device.SetTexturing(true);
}

// Рисуем текстуры кнопок
void GameWidget::DrawTexture(Render::Texture* tex, FPoint position)
{
	Render::device.PushMatrix();

	Render::device.MatrixTranslate(math::Vector3(position.x, position.y, 0));

	IRect texRect = tex->getBitmapRect();
	FRect rect(texRect), uv(0, 1, 0, 1);
	tex->TranslateUV(rect, uv);

	Render::device.MatrixTranslate(math::Vector3(-texRect.width / 2, 0, 0.f));

	tex->Bind();

	Render::DrawQuad(rect, uv);

	Render::device.PopMatrix();

}

// Рисуем полоску перегрузки
void GameWidget::DrawOverheat(Color rgba,int overheat,int height)
{
	Render::device.SetTexturing(false);
	Render::BeginColor(rgba);
	Render::DrawRect(0, 0, Render::device.Width() / 100 * overheat, height);
	Render::EndColor();
	Render::device.SetTexturing(true);
}

// Проверка попадания точки в прямоугольник
bool GameWidget::PointHitRect(IPoint point,IRect rect)
{
	if (
		point.x > rect.x &&
		point.x < rect.x + rect.Width() &&
		point.y < rect.y + rect.Height() &&
		point.y > rect.y
		)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void GameWidget::AcceptMessage(const Message& message)
{
	
}

void GameWidget::KeyPressed(int keyCode)
{
	
}

void GameWidget::CharPressed(int unicodeChar)
{
	
}
