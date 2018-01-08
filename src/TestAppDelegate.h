#ifndef __TESTAPPDELEGATE_H__
#define __TESTAPPDELEGATE_H__

#pragma once

class TestAppDelegate : public Core::EngineAppDelegate {
public:
	TestAppDelegate();

	virtual void GameContentSize(int deviceWidth, int deviceHeight, int &width, int &height) override;
	virtual void ScreenMode(DeviceMode &mode) override;

	virtual void RegisterTypes() override; // Регистрация виджетов

	virtual void LoadResources() override; // Загрузка основных ресурсов
	virtual void OnResourceLoaded() override;

	virtual void OnPostDraw() override;
};

#endif // __TESTAPPDELEGATE_H__
