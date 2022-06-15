#ifndef REGISTRY_HELPER_H
#define REGISTRY_HELPER_H

#pragma once

#define WIN32_LEAN_AND_MEAN

#include <stdexcept>
#include <string>
#include <Windows.h>

// https://docs.microsoft.com/en-us/archive/msdn-magazine/2017/may/c-use-modern-c-to-access-the-windows-registry
namespace registry_helper
{
	class registry_error final : public std::runtime_error
	{
	private:
		LONG error_code_{};

	public:
		registry_error(const char* message, const LONG error_code) :
			std::runtime_error{ message },
			error_code_{ error_code }
		{}


		[[nodiscard]] LONG error_code() const noexcept
		{
			return error_code_;
		}
	};


	inline auto reg_get_dword(
		HKEY h_key,
		const std::wstring& sub_key,
		const std::wstring& value
	) -> DWORD
	{
		DWORD data{};
		DWORD data_size = sizeof(data);

		const LONG ret_code = RegGetValue(
			h_key,
			sub_key.c_str(),
			value.c_str(),
			RRF_RT_REG_DWORD,
			nullptr,
			&data,
			&data_size
		);

		if (ret_code != ERROR_SUCCESS)
		{
			throw registry_error{ "Cannot read DWORD from registry", ret_code };
		}

		return data;
	}

	inline auto reg_get_string(
		HKEY h_key,
		const std::wstring& sub_key,
		const std::wstring& value
	) -> std::wstring
	{
		DWORD data_size{};
		const LONG size_ret_code = RegGetValue(
			h_key,
			sub_key.c_str(),
			value.c_str(),
			RRF_RT_REG_SZ,
			nullptr,
			nullptr,
			&data_size
		);

		const auto err_msg = "Cannot read string from registry";
		if (size_ret_code != ERROR_SUCCESS)
		{
			throw registry_error{ err_msg, size_ret_code };
		}

		std::wstring data;
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

	inline auto is_windows_elven() -> bool
	{
		constexpr auto windows11_build_no = 22000;
		const std::wstring s = reg_get_string(
			HKEY_LOCAL_MACHINE, 
			L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion",
			L"CurrentBuildNumber");

		const auto build_no = std::stoi(s);

		return build_no >= windows11_build_no;
	}
}
#endif


