#ifndef SHARED_CMD_LINE_H_
#define SHARED_CMD_LINE_H_

#include <string>
#include <map>

namespace shared
{
class CommandLine
{
public:
	CommandLine(const std::wstring &cmd);
	~CommandLine();

	bool Get(const std::wstring &key, std::wstring &value);
	bool IsExist(const std::wstring &key);
	std::map<std::wstring, std::wstring> GetMap(void);

protected:
	// �򵥽�cmd��'/'�ָ�������������'/'�������
	static bool Parse(const std::wstring &cmd, std::map<std::wstring, std::wstring> &param_map);

protected:
	std::wstring cmd_;
	std::map<std::wstring, std::wstring> map_;
};
}//namespace shared

#endif //SHARED_CMD_LINE_H_
