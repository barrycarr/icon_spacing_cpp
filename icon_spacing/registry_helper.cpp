#include "registry_helper.h"

// https://docs.microsoft.com/en-us/archive/msdn-magazine/2017/may/c-use-modern-c-to-access-the-windows-registry

namespace registry_helper
{
	using namespace std;

	auto reg_get_dword(HKEY h_key, const wstring& sub_key, const wstring& value) -> DWORD
	{
		DWORD data{};
		DWORD data_size = sizeof(data);

		const auto ret_code = RegGetValue(
			h_key,
			sub_key.c_str(),
			value.c_str(),
			RRF_RT_REG_DWORD,
			nullptr,
			&data,
			&data_size
		);

		if ( ret_code != ERROR_SUCCESS)
		{
			throw registry_error{ "Cannot read DWORD from registry", ret_code };
		}

		return data;
	}

	auto reg_get_string(HKEY h_key, const wstring& sub_key, const wstring& value) -> wstring
	{
		const auto err_msg = "Cannot read string from registry";
		DWORD data_size{};

		const auto size_ret_code = RegGetValue(
			h_key,
			sub_key.c_str(),
			value.c_str(),
			RRF_RT_REG_SZ,
			nullptr,
			nullptr,
			&data_size
		);

		if (size_ret_code != ERROR_SUCCESS)
		{
			throw registry_error{ err_msg, size_ret_code };
		}

		wstring data;
		data.resize(data_size / sizeof(wchar_t));

		const LONG ret_code = RegGetValue(
			h_key,
			sub_key.c_str(),
			value.c_str(),
			RRF_RT_REG_SZ,
			nullptr,
			&data[0],
			&data_size
		);

		if (ret_code != ERROR_SUCCESS)
		{
			throw registry_error{ err_msg, ret_code };
		}

		const DWORD string_len_in_wchars = (data_size / sizeof(wchar_t)) - 1;
		data.resize(string_len_in_wchars);

		return data;
	}

	auto is_windows_eleven() -> bool
	{
		constexpr auto windows11_build_no = 22000;

		const wstring s = reg_get_string(
			HKEY_LOCAL_MACHINE, 
			L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion",
			L"CurrentBuildNumber");

		const auto build_no = stoi(s);

		return build_no >= windows11_build_no;
	}
}
