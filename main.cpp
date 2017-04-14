#include "rjson.h"

int main()
{
	string js = getJsonFromFile("test.txt");
	RJson json(js);
	json.parseJson();
	return 0;
}
