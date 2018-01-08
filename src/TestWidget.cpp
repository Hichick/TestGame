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
	
	// Максимальное значение для угла пушки 88 градусов
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

	
	// Рисуем ось Y
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
	// Получаем текущее положение курсора мыши.
	//
	IPoint mouse_pos = Core::mainInput.GetMousePos();


	// Повороты текстуры пушки

	Render::device.PushMatrix();

	if (mouse_pos.x > Render::device.Width()/2)
	{
		can->SetPosition(FPoint(Render::device.Width() / 2 + can->GetTexture()->Width() / 2,10));
		can->SetAngle((-angle(0, mouse_pos.y, Render::device.Width() / 2 - mouse_pos.x, mouse_pos.y)));

		Render::device.MatrixTranslate(can->GetPosition().x, can->GetPosition().y, 0);
		Render::device.MatrixRotate(math::Vector3(0, 0, 1), can->GetAngle());
		Render::device.MatrixScale(-1.f, 1.f, 0.f); // Отзеркаливаем (потому что поворот осуществлятся относительно левого нижнего угла и при повороте на -30(330) градусов смещятся текстура)
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
	// Проталкиваем в стек текущее преобразование координат, чтобы в дальнейшем
	// можно было восстановить это преобразование вызовом PopMatrix.
	//
	Render::device.PushMatrix();
	
	//
	// Изменяем текущее преобразование координат, перемещая центр координат в позицию мыши
	// и поворачивая координаты относительно этого центра вокруг оси z на угол _angle.
	//
	Render::device.MatrixTranslate((float)mouse_pos.x, (float)mouse_pos.y, 0);
	Render::device.MatrixRotate(math::Vector3(0, 0, 1), _angle);

	if (!_curTex)
	{
		//
		// Метод Texture::Draw() выводит квадратный спрайт с размерами текстуры
		// в центре координат (0, 0). Центр координат и преобразование координат вершин
		// спрайта устанавливаются с применением текущего преобразования.
		//
		// При вызове метода Texture::Draw() вызывать Texture::Bind() необязательно.
		//
		_tex1->Draw();
	}
	else
	{
		IRect texRect = _tex2->getBitmapRect();

		//
		// При отрисовке текстуры можно вручную задавать UV координаты той части текстуры,
		// которая будет натянута на вершины спрайта. UV координаты должны быть нормализованы,
		// т.е., вне зависимости от размера текстуры в текселях, размер любой текстуры
		// равен 1 (UV координаты задаются в диапазоне 0..1, хотя ничто не мешает задать их
		// больше единицы, при этом в случае установленной адресации текстуры REPEAT, текстура
		// будет размножена по этой стороне соответствующее количество раз).
		//

		FRect rect(texRect);
		FRect uv(0, 1, 0, 1);

		_tex2->TranslateUV(rect, uv);

		Render::device.MatrixScale(_scale);
		Render::device.MatrixTranslate(-texRect.width * 0.5f, -texRect.height * 0.5f, 0.0f);

		//
		// Привязываем текстуру.
		//
		_tex2->Bind();
		
		//
		// Метод DrawQuad() выводит в графическое устройство квадратный спрайт, состоящий их двух
		// примитивов - треугольников, используя при этом текущий цвет для вершин и привязанную (binded) текстуру,
		// если разрешено текстурирование.
		//
		// Перед вызовом DrawQuad() должен быть вызов Texture::Bind() либо SetTexturing(false),
		// иначе визуальный результат будет непредсказуемым.
		//
		Render::DrawQuad(rect, uv);
	}

	//
	// Воостанавливаем прежнее преобразование координат, снимая со стека изменённый фрейм.
	//
	Render::device.PopMatrix();
	
	//
	// Получаем текущие координаты объекта, двигающегося по сплайну
	//
	FPoint currentPosition = spline.getGlobalFrame(math::clamp(0.0f, 1.0f, _timer/12.56f ));

	//
	// И рисуем объект в этих координатах
	//
	Render::device.PushMatrix();
	Render::device.MatrixTranslate(currentPosition.x, currentPosition.y, 0);
	_tex3->Draw();
	Render::device.PopMatrix();


	

	//
	// Этот вызов отключает текстурирование при отрисовке.
	//
	Render::device.SetTexturing(false);
	
	//
	// Метод BeginColor() проталкивает в стек текущий цвет вершин и устанавливает новый.
	//
	Render::BeginColor(Color(255, 128, 0, 255));
	
	//
	// Метод DrawRect() выводит в графическое устройство квадратный спрайт, состоящий их двух
	// примитивов - треугольников, используя при этом текущий цвет для вершин и привязанную (binded) текстуру,
	// если разрешено текстурирование.
	//
	// Перед вызовом DrawRect() должен быть вызов Texture::Bind(), либо SetTexturing(false),
	// иначе визуальный результат будет непредсказуемым.
	//
	Render::DrawRect(924, 0, 100, 100);
	
	//
	// Метод EndColor() снимает со стека текущий цвет вершин, восстанавливая прежний.
	//
	Render::EndColor();
	
	//
	// Опять включаем текстурирование.
	//
	Render::device.SetTexturing(true);

	//
	// Рисуем все эффекты, которые добавили в контейнер (Update() для контейнера вызывать не нужно).
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
	// Обновим контейнер с эффектами
	//
	_effCont.Update(dt);

	//
	// dt - значение времени в секундах, прошедшее от предыдущего кадра.
	// Оно может принимать разные значения, в зависимости от производительности системы
	// и сложности сцены.
	//
	// Для того, чтобы наша анимация зависела только от времени, и не зависела от
	// производительности системы, мы должны рассчитывать её от этого значения.
	//
	// Увеличиваем наш таймер с удвоенной скоростью.
	//
	_timer += dt * 2;
	
	//
	// Зацикливаем таймер в диапазоне (0, 2п).
	// Это нужно делать для предотвращения получения некорректных значений,
	// если вдруг переполнится разрядная сетка (float переполнился) или задержка
	// от предыдущего кадра была слишкой большой (система тормози-и-ит).
	//
	// Диапазон значений выбран равным (0, 2п), потому что мы используем это значение
	// для расчёта синуса, и большие значения будут просто периодически повторять результат.
	//
	while (_timer > 4 * math::PI)
	{
		_timer -= 4 * math::PI;
	}
	
	//
	// Анимирование параметра масштабирования в зависимости от таймера.
	//
	_scale = 0.8f + 0.25f * sinf(_timer);

	
	
}

bool TestWidget::MouseDown(const IPoint &mouse_pos)
{
	if (Core::mainInput.GetMouseRightButton())
	{
		//
		// При нажатии на правую кнопку мыши, создаём эффект шлейфа за мышкой.
		//
		_eff = _effCont.AddEffect("Iskra");
		_eff->posX = mouse_pos.x + 0.f;
		_eff->posY = mouse_pos.y + 0.f;
		_eff->Reset();
		
		//
		// И изменяем угол наклона текстуры.
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
		// При нажатии на левую кнопку мыши, создаём временный эффект, который завершится сам.
		//
		//ParticleEffectPtr eff = _effCont.AddEffect("FindItem2");
		//eff->posX = mouse_pos.x + 0.f;
		//eff->posY = mouse_pos.y + 0.f;
		//eff->Reset();

		//
		// Изменяем значение с 0 на 1 и наоборот.
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
		// Если эффект создан, то перемещаем его в позицию мыши.
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
		// Если эффект создан, то при отпускании мыши завершаем его.
		//
		_eff->Finish();
		_eff = NULL;
	}
}

void TestWidget::AcceptMessage(const Message& message)
{
	//
	// Виджету могут посылаться сообщения с параметрами.
	//

	const std::string& publisher = message.getPublisher();
	const std::string& data = message.getData();
}

void TestWidget::KeyPressed(int keyCode)
{
	//
	// keyCode - виртуальный код клавиши.
	// В качестве значений для проверки нужно использовать константы VK_.
	//

	if (keyCode == VK_A) {
		// Реакция на нажатие кнопки A
	}
}

void TestWidget::CharPressed(int unicodeChar)
{
	//
	// unicodeChar - Unicode код введённого символа
	//

	if (unicodeChar == L'а') {
		// Реакция на ввод символа 'а'
	}
}
