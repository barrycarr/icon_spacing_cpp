#include "cmd_line_parser.h"

namespace command_line_helper
{
	using namespace std;

	auto cmd_line_parser::add(const string& name, const regex& regex) -> void
	{
		this->regexs_.insert({ name, regex });
	}

	result_map cmd_line_parser::parse(const int argc, const char* argv[])
	{
		for(int i = 1; i < argc; i++)
		{
			const string arg = argv[i];
			for (auto it = this->regexs_.begin(); it != this->regexs_.end(); ++it)
			{
				string key = it->first;
				regex rx = it->second;
				
				auto rx_begin = regex_iterator<string::const_iterator>(arg.begin(), arg.end(), rx);
				auto rx_end = regex_iterator<string::const_iterator>();

				for (regex_iterator<string::const_iterator> ri = rx_begin; ri != rx_end; ++ri)
				{
					smatch match = *ri;
					string match_str = match.str();
					results_.insert({ key, match_str });
				}
			}
		}
		return results_;
	}
}

