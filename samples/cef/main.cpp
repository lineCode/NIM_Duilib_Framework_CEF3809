// basic.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "main.h"
#include "cef_form.h"

#pragma comment(lib, "dbghelp.lib")

void RegisterCustomSchemes(CefRawPtr<CefSchemeRegistrar> registrar)
{
	//registrar->AddCustomScheme(kScheme, CEF_SCHEME_OPTION_STANDARD | CEF_SCHEME_OPTION_CORS_ENABLED | CEF_SCHEME_OPTION_FETCH_ENABLED);
}

void BeforeCommandLineProcessing(const CefString& process_type, CefRefPtr<CefCommandLine> command_line)
{
	// Pass additional command-line flags to the browser process.
	if (process_type.empty())
	{
		command_line->AppendSwitchWithValue("ppapi-flash-version", "20.0.0.228");
		command_line->AppendSwitchWithValue("ppapi-flash-path", "PepperFlash\\pepflashplayer.dll");
		//同一个域下的使用同一个渲染进程
		command_line->AppendSwitch("process-per-site");
		command_line->AppendSwitch("disable-gpu");
		command_line->AppendSwitch("disable-gpu-compositing");
		command_line->AppendSwitch("disable-web-security");
		//command_line->AppendSwitchWithValue("proxy-server", "SOCKS5://127.0.0.1:1080");	

		// 开启离屏渲染
		if (nim_cef::CefManager::GetInstance()->IsEnableOffsetRender())
		{
			command_line->AppendSwitch("disable-surfaces");
			command_line->AppendSwitch("enable-begin-frame-scheduling");
		}
	}
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// 将 bin\\cef 目录添加到环境变量，这样可以将所有 CEF 相关文件放到该目录下，方便管理
	// 在项目属性->连接器->输入，延迟加载 libcef.dll
	nim_cef::CefManager::GetInstance()->AddCefDllToPath();

	HRESULT hr = ::OleInitialize(NULL);
	if (FAILED(hr))
		return 0;

	// 初始化 CEF
	CefSettings settings;
	if (!nim_cef::CefManager::GetInstance()->Initialize(QPath::GetAppPath() + L"cef_temp\\", settings, kEnableOffsetRender, RegisterCustomSchemes, BeforeCommandLineProcessing))
	{
		return 0;
	}

	// 创建主线程
	MainThread thread;

	// 执行主线程循环
	thread.RunOnCurrentThreadWithLoop(nbase::MessageLoop::kUIMessageLoop);

    // 清理 CEF
    nim_cef::CefManager::GetInstance()->UnInitialize();

	::OleUninitialize();

	return 0;
}

void MainThread::Init()
{
	nbase::ThreadManager::RegisterThread(kThreadUI);

	// 获取资源路径，初始化全局参数
	// 默认皮肤使用 resources\\themes\\default
	// 默认语言使用 resources\\lang\\zh_CN
	// 如需修改请指定 Startup 最后两个参数
	std::wstring theme_dir = QPath::GetAppPath();
	ui::GlobalManager::Startup(theme_dir + L"resources\\", ui::CreateControlCallback(), false);

	// 创建一个默认带有阴影的居中窗口
	CefForm* window = new CefForm();
	window->Create(NULL, CefForm::kClassName.c_str(), WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX, 0, nim_cef::CefManager::GetInstance()->IsEnableOffsetRender());
	window->CenterWindow();
	window->ShowWindow();
}

void MainThread::Cleanup()
{
	ui::GlobalManager::Shutdown();
	SetThreadWasQuitProperly(true);
	nbase::ThreadManager::UnregisterThread();
}
