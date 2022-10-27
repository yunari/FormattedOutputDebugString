#pragma once

namespace FOutputDebugString
{
#define FLOGTYPE_INFO	0
#define FLOGTYPE_ERROR	1
#define FLOGTYPE_DEBUG	2

	void WriteLog(int log_type, const char* module_name, const char* method_name, const char* log, ...);
	void WriteLog(int log_type, const wchar_t* module_name, const char* method_name, const wchar_t* log, ...);
};