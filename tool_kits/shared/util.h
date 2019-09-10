#pragma once
#include <string>
#include <map>

class QString
{
public:
};

class QPath
{
public:
	/**
	 * @brief ��ȡ exe ����Ŀ¼
	 * @return ���� exe ����Ŀ¼��β����б��
	 */
	static std::wstring GetAppPath();

	/**
	 * @brief ���һ��Ŀ¼��ִ�г���Ļ���������
	 * @param[in] directory Ҫ��ӵ�Ŀ¼
	 * @return ��
	 */
	static void AddNewEnvironment(const std::wstring& directory);
};

class QCommand
{
public:
	static void ParseCommand(const std::wstring &cmd);
	static bool AppStartWidthCommand(const std::wstring &app, const std::wstring &cmd);
	static bool RestartApp(const std::wstring &cmd);
	static std::wstring Get(const std::wstring &key);
	static void Set(const std::wstring &key, const std::wstring &value);
	static void Erase(const std::wstring &key);
private:
	static std::map<std::wstring,std::wstring> key_value_;
};