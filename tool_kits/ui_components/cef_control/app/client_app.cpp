#include "stdafx.h"
#include "client_app.h" 
#include <string>

#include "include/cef_cookie.h"
#include "include/cef_process_message.h"
#include "include/cef_task.h"
#include "include/cef_v8.h"
#include "cef_control/manager/cef_manager.h"
#include "cef_control/util/util.h"

namespace nim_cef
{

ClientApp::ClientApp() 
{
	last_node_is_editable_ = false;
}

void ClientApp::AttachRegisterCustomSchemes(const RegisterCustomSchemesCallback& callback)
{
	register_custom_schemes_callback = callback;
}

void ClientApp::AttachBeforeCommandLineProcessing(const BeforeCommandLineProcessingCallback& callback)
{
	before_command_line_processing_callback = callback;
}

//////////////////////////////////////////////////////////////////////////////////////////
// CefApp methods.
void ClientApp::OnBeforeCommandLineProcessing(const CefString& process_type, CefRefPtr<CefCommandLine> command_line)
{
	if (before_command_line_processing_callback)
		before_command_line_processing_callback(process_type, command_line);	
}

//////////////////////////////////////////////////////////////////////////////////////////
void ClientApp::OnRegisterCustomSchemes(CefRawPtr<CefSchemeRegistrar> registrar)
{
	// Default schemes that support cookies.
	cookieable_schemes_.push_back("http");
	cookieable_schemes_.push_back("https");
	if (register_custom_schemes_callback)
		register_custom_schemes_callback(registrar);	
}

}