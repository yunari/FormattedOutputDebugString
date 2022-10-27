#include "FormattedOutputDebugString.h"

#include <string>
#include <Windows.h>			// OutputDebugString
#include <atlconv.h>			// USES_CONVERSION, A2W
#include <stdarg.h>				// va_start

#include <boost/format.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread.hpp>

namespace FOutputDebugString
{
#define MAX_LOG_SIZE 4096

	std::string GetTypeStr(int log_type)
	{
		switch (log_type)
		{
		case FLOGTYPE_INFO:
			return "INFO";
		case FLOGTYPE_ERROR:
			return "ERROR";
		case FLOGTYPE_DEBUG:
			return "DEBUG";
		}
		return "";
	}

	std::wstring GetTypeWstr(int log_type)
	{
		switch (log_type)
		{
		case FLOGTYPE_INFO:
			return L"INFO";
		case FLOGTYPE_ERROR:
			return L"ERROR";
		case FLOGTYPE_DEBUG:
			return L"DEBUG";
		}
		return L"";
	}

	std::string GetDateTimeStr()
	{
		return boost::posix_time::to_simple_string(boost::posix_time::second_clock::local_time());
	}

	std::wstring GetDateTimeWstr()
	{
		std::wstringstream wsstr;
		wsstr << boost::posix_time::second_clock::local_time();
		return wsstr.str();
	}

	void String2Wstring(std::string& in_str, std::wstring& out_wstr)
	{
		USES_CONVERSION;
		out_wstr = A2W(in_str.c_str());
	}

	void WriteLog(int log_type, const char* module_name, const char* method_name, const char* log, ...)
	{
		// 입력받은 로그 포맷팅
		char log_buffer[MAX_LOG_SIZE] = { 0, };
		va_list arg;
		va_start(arg, log);
		_vsnprintf_s(log_buffer, sizeof(log_buffer), _TRUNCATE, log, arg);
		va_end(arg);

		// 로그 사이즈 체크
		std::string str_log = log;

		if (str_log.size() > MAX_LOG_SIZE)
			str_log.resize(MAX_LOG_SIZE);

		// 전체 로그 포맷팅
		char buffer[MAX_LOG_SIZE] = { 0, };
		boost::iostreams::stream<boost::iostreams::array_sink> stream(buffer, sizeof(buffer) - 1);

		stream << (boost::format("%s [%u][%s][%s][%s] %s\n") % GetDateTimeStr() % boost::this_thread::get_id() % GetTypeStr(log_type) % module_name % method_name % log_buffer) << '\0';

		OutputDebugStringA(buffer);
	}

	void WriteLog(int log_type, const wchar_t* module_name, const char* method_name, const wchar_t* log, ...)
	{
		// 입력받은 로그 포맷팅
		WCHAR log_buffer[MAX_LOG_SIZE] = { 0, };
		va_list arg;
		va_start(arg, log);
		_vsnwprintf_s(log_buffer, sizeof(log_buffer), _TRUNCATE, log, arg);
		va_end(arg);

		// 로그 사이즈 체크
		std::wstring wstr_log = log;

		if (wstr_log.size() > MAX_LOG_SIZE)
			wstr_log.resize(MAX_LOG_SIZE);

		// method_name 변환
		std::string str_method_name = method_name;
		std::wstring wstr_method_name = L"";
		String2Wstring(str_method_name, wstr_method_name);

		// 로그 포맷팅
		wchar_t buffer[MAX_LOG_SIZE] = { 0, };
		boost::iostreams::stream<boost::iostreams::basic_array_sink<wchar_t>> stream(buffer, sizeof(buffer) - 1);

		stream << (boost::wformat(L"%s [%u][%s][%s][%s] %s\n") % GetDateTimeWstr() % boost::this_thread::get_id() % GetTypeWstr(log_type) % module_name % wstr_method_name % log_buffer) << '\0';

		OutputDebugStringW(buffer);
	}
}