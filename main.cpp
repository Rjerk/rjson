#include "rjson.h"

int main(int argc, char** argv)
{
	if (argc != 2) {
		cerr << "Usage: ./rjson filename." << endl;
		return -1;
	}
	string js = getJsonFromFile(string(argv[1]));
	RJson json(js);
	cout << "Input: \n" << js << endl;
    cout << "\nParsing json: ";
	json.parseJson();
	string outputJson = json.generator();
	cout << "\nJson generator: \n" << outputJson << endl;
	RJson testOutput(outputJson);
	cout << "\nGeneration test: ";
	testOutput.parseJson();
	cout << endl;
	return 0;
}
