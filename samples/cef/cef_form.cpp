#include "stdafx.h"
#include "cef_form.h"

const std::wstring CefForm::kClassName = L"CEF_Control_Example";

CefForm::CefForm()
{
}

CefForm::~CefForm()
{
}

std::wstring CefForm::GetSkinFolder()
{
	return L"cef";
}

std::wstring CefForm::GetSkinFile()
{
	return L"cef.xml";
}

std::wstring CefForm::GetWindowClassName() const
{
	return kClassName;
}

ui::Control* CefForm::CreateControl(const std::wstring& pstrClass)
{
	// 扫描 XML 发现有名称为 CefControl 的节点，则创建一个 ui::CefControl 控件
	if (pstrClass == L"CefControl")
	{
		ui::CefControlBase* ctrl;
		if (nim_cef::CefManager::GetInstance()->IsEnableOffsetRender())
			ctrl = new ui::CefControl;
		else
			ctrl = new ui::CefNativeControl;
		ctrl->AttachGetBrowserSettings(ToWeakCallback([this]()
			{
				CefBrowserSettings browser_settings;
				browser_settings.javascript_access_clipboard = STATE_ENABLED;
				browser_settings.javascript_dom_paste = STATE_ENABLED;
				browser_settings.file_access_from_file_urls = STATE_ENABLED;
				browser_settings.universal_access_from_file_urls = STATE_ENABLED;
				browser_settings.web_security = STATE_DISABLED;
				//browser_settings.local_storage = STATE_ENABLED;
				return browser_settings;
			}));
		return ctrl;
	}

	return NULL;
}

void CefForm::InitWindow()
{
	// 监听鼠标单击事件
	m_pRoot->AttachBubbledEvent(ui::kEventClick, nbase::Bind(&CefForm::OnClicked, this, std::placeholders::_1));

	// 从 XML 中查找指定控件
	cef_control_		= dynamic_cast<ui::CefControlBase*>(FindControl(L"cef_control"));
	cef_control_dev_	= dynamic_cast<ui::CefControlBase*>(FindControl(L"cef_control_dev"));
	btn_dev_tool_		= dynamic_cast<ui::Button*>(FindControl(L"btn_dev_tool"));
	edit_url_			= dynamic_cast<ui::RichEdit*>(FindControl(L"edit_url"));

	// 设置输入框样式
	edit_url_->SetSelAllOnFocus(true);
	edit_url_->AttachReturn(nbase::Bind(&CefForm::OnNavigate, this, std::placeholders::_1));

	// 监听页面加载完毕通知
	cef_control_->AttachLoadEnd(nbase::Bind(&CefForm::OnLoadEnd, this, std::placeholders::_1));
	cef_control_->AttachLoadingStateChange(nbase::Bind(&CefForm::OnLoadingStateChange, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

	// 打开开发者工具
	cef_control_->AttachDevTools(cef_control_dev_);

	// 加载皮肤目录下的 html 文件
	cef_control_->LoadURL(QPath::GetAppPath() + L"resources\\themes\\default\\cef\\cef.html");

	if (!nim_cef::CefManager::GetInstance()->IsEnableOffsetRender())
		cef_control_dev_->SetVisible(false);
}

LRESULT CefForm::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	nim_cef::CefManager::GetInstance()->PostQuitMessage(0L);
	return __super::OnClose(uMsg, wParam, lParam, bHandled);
}

bool CefForm::OnClicked(ui::EventArgs* msg)
{
	std::wstring name = msg->pSender->GetName();

	if (name == L"btn_dev_tool")
	{
		if (cef_control_->IsAttachedDevTools())
		{
			cef_control_->DettachDevTools();
		}
		else
		{
			cef_control_->AttachDevTools(cef_control_dev_);
		}

		if (nim_cef::CefManager::GetInstance()->IsEnableOffsetRender())
		{
			cef_control_dev_->SetVisible(cef_control_->IsAttachedDevTools());
		}
	}
	else if (name == L"btn_back")
	{
		cef_control_->GoBack();
	}
	else if (name == L"btn_forward")
	{
		cef_control_->GoForward();
	}
	else if (name == L"btn_navigate")
	{
		OnNavigate(nullptr);
	}
	else if (name == L"btn_refresh")
	{
		cef_control_->Refresh();
	}

	return true;
}

bool CefForm::OnNavigate(ui::EventArgs* msg)
{
	if (!edit_url_->GetText().empty())
	{
		cef_control_->LoadURL(edit_url_->GetText());
		cef_control_->SetFocus();
	}

	return true;
}

void CefForm::OnLoadEnd(int httpStatusCode)
{
	// 注册一个方法提供前端调用
	cef_control_->RegisterCppFunc(L"ShowMessageBox", ToWeakCallback([this](const std::string& params, nim_cef::ReportResultFunction callback) {
		shared::Toast::ShowToast(nbase::UTF8ToUTF16(params), 3000, GetHWND());
		callback(false, R"({ "message": "Success." })");
	}));
}

void CefForm::OnLoadingStateChange(bool isLoading, bool canGoBack, bool canGoForward)
{	
	FindControl(L"btn_refresh")->SetEnabled(!isLoading);
	FindControl(L"btn_back")->SetEnabled(canGoBack);
	FindControl(L"btn_forward")->SetEnabled(canGoForward);	
}
