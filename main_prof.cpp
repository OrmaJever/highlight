#include "data.h"
#include <sstream>
#include <fstream>

int main(int argc, char **argv)
{
	std::fstream file("C:/index.php");
	if(!file) {
		return -1;
	};

	std::ostringstream oss;
	oss << file.rdbuf();
	HL::Data::string code = oss.str();
	HL::Data::highlight(code, 0);
	return 0;
}
