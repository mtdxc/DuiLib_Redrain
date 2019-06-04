// basic.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "layouts_form.h"
#include "resource.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// 获取资源路径，初始化全局参数
	// 默认皮肤使用 resources\\themes\\default
	// 默认语言使用 resources\\lang\\zh_CN
	// 如需修改请指定 Startup 最后两个参数
	std::wstring theme_dir = ui::PathUtil::GetCurrentModuleDir();
	ui::GlobalManager::Startup(theme_dir + L"resources\\", ui::CreateControlCallback(), false);

	// 一个仿微信的布局示例
	LayoutsForm::ShowCustomWindow(L"basic_layout", L"layouts", L"wechat.xml");

	// 一个仿登录窗口的布局示例
	// LayoutsForm::ShowCustomWindow(L"login", L"layouts", L"login.xml");
	ui::GlobalManager::MessageLoop();

	ui::GlobalManager::Shutdown();

}
