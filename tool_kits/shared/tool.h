#ifndef SHARED_TOOL_H_
#define SHARED_TOOL_H_

#include <string>
#include <vector>
#include "base/time/time.h"
#include "base/base_types.h"

namespace shared
{
namespace tools
{
// ϵͳ���
int GetOSVersion();
std::string GetOSVersionString();
bool IsWow64();

// �ַ�������
std::string	FormatTime(nbase::Time::TimeStruct	t);
std::wstring FormatBytes(double bytes);
std::wstring FormatBps(double bytes);
void SplitString(const std::string &str, const std::string &seperator, std::vector<std::string> &array);
void SplitString(const std::wstring &str, const std::wstring &seperator, std::vector<std::wstring> &array);
std::string StringReverse(const std::string &input);

// �����޷���ʾ�ı����ַ�
std::string FilterUCS4(const std::string& str, bool replace_spaces = true);

// ��ʽ��SQL�������ַ�������ת�嵥���ţ�������ִ��SQL����׳��쳣
UTF8String FormatSQLText(const UTF8String &sql_text);

// �����ܷ񷢶���
bool IsChinaPhoneNumber(const std::string &str);
bool IsAuthCode(const std::string &str);

// �ļ�����
std::wstring GetSpecialFolder(int id);
std::wstring GetResourcePath(void);
void MakeExtendPath(std::wstring &path);
void ClearFile(const wchar_t *path, long file_length_limit);
bool FilePathIsExist(const std::string &utf8_filepath_in, bool is_directory);

// ���õݹ�ķ�ʽ����ָ��Ŀ�µ��ļ�
bool FindFile(const std::wstring &dir,const std::wstring &file_desc,std::wstring &file_path);
std::wstring EnsureValidFileName(const std::wstring &file_name, const std::wstring &find = L"\\/:*?\"<>|", const std::wstring &replace = L"_");

// ��ѧ���
int CalcAvg(const std::vector<int> &vec, bool is_revise = true);
int CalcStDev(const std::vector<int> &vec);

// ����ַ
void SafeOpenUrl(const std::wstring &url, int show_cmd);
void SafeOpenUrlEx(const std::string &url, int show_cmd);

bool RestartComputer();
bool RunAppWithRedirection(const std::wstring &app_path, const std::wstring &app_cmd, std::string &output);

// ����ʱ��������Ⱥ���
std::string GenerateTimeStamp();

// Register Server
BOOL DllRegisterServer(LPCTSTR lpszFileName);
BOOL DllUnregisterServer(LPCTSTR lpszFileName);

// ����ͨ��message
void RegisterShowWndForgroudMessage();
UINT GetShowWndForgroudMessageID();

// ��ȡͼƬ���Ͷ�Ӧ��CLSID
bool GetImageCLSID(const wchar_t* format, CLSID* pCLSID);

// �����ļ��µ��ļ������޵�һ�㣩
struct FileItem
{
	std::wstring file_;
	long size_;
};

typedef std::vector<std::wstring> StringVector;
typedef std::vector<FileItem> FileItemVector;

class File
{
public:
	//dir�����Ҫ"\\"
	static void GetFolder(const std::wstring &dir, StringVector &out, bool full_path = false); 
	//dir�����Ҫ"\\"
	static void GetFile(const std::wstring &dir, std::vector<FileItem> &out, bool full_path = false); 
};

}//namespace tools
}//namespace shared

#endif //SHARED_TOOL_H_
