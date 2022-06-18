#pragma once
#include <map>
#include <regex>

namespace command_line_helper
{
	using result_map = std::map<const std::string, const std::string>;
	using regex_map = std::map<const std::string, const std::regex>;
	class cmd_line_parser
	{
	private:
		regex_map regexs_;
		result_map results_;

	public:
		cmd_line_parser() = default;

		void add(const std::string& name, const std::regex& regex);
		result_map parse(const int argc, const char* argv[]);
	};

}


