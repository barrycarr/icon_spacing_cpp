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

	auto reg_get_dword(
		HKEY h_key,
		const std::wstring& sub_key,
		const std::wstring& value
	) -> DWORD;

	auto reg_get_string(
		HKEY h_key,
		const std::wstring& sub_key,
		const std::wstring& value
	)->std::wstring;

	auto is_windows_eleven() -> bool;
}
#endif


