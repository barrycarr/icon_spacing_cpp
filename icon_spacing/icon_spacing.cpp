// icon_spacing.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <map>

#include "registry_helper.h"

using namespace std;

enum class distance_option {wide, medium, narrow};

const map<const char*, distance_option> distance_map{
	{"w", distance_option::wide },
	{"m", distance_option::medium},
	{"n", distance_option::narrow},
};

struct options
{
	distance_option distance;
	bool update;
};

auto get_options(const int argc, char* argv[]) -> options 
{
	auto distance = distance_option::wide;
	auto update = false;

	for (int i = 1; i < argc; i++)
	{
				
	}

	return {
		distance,
		update
	};
}

auto main(const int argc, char *argv[]) -> int
{
	cout << "Windows 11 desktop icon spacing utility\n";
	if (!registry_helper::is_windows_eleven())
	{
		cout << "This utility is for Windows 11 or greater.";
		return -11;
	}

	auto options = get_options(argc, argv);
}
