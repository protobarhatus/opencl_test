#include "code_load.h"
#include <fstream>

void cutBetweeen(std::string& text, const std::string& up, const std::string& down)
{
	text.erase(text.find(up), text.find(down) + down.length() - text.find(up));
}

std::string loadCode(const std::string& file_name)
{
	std::fstream file(file_name);

	std::string code;
	std::string lin;

	while (std::getline(file, lin))
	{
		
		code += lin + "\n";
	}

	cutBetweeen(code, "#define ___META___", "#undef ___META___");
	cutBetweeen(code, "#define ___EXTERNAL___", "#undef ___EXTERNAL___");

	file.close();
	return code;
}