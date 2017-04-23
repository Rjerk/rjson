#include "rjson.h"

int main(int argc, char** argv)
{
	if (argc != 2) {
		cerr << "Usage: ./rjson filename." << endl;
		return -1;
	}
	string js = getJsonFromFile(string(argv[1]));

	cout << "Input: \n" << js << endl;

    // json parser
	RJson json(js);
    cout << "\nParsing json: ";
	parse_code code = json.parseJson();
	json.parseCodeHandle(code);

	json_value_t* number = json.getValueFromObject("number");
	assert(number != nullptr && number->type == RJSON_NUMBER);
	cout << number->num << endl;

    json_value_t* str = json.getValueFromObject("string");
    assert(str != nullptr && str->type == RJSON_STRING);
    cout << str->s << endl;

	// json generator
	string outputJson = json.generator();
	cout << "\nJson generator: \n" << outputJson << endl;

	// parse the output as test.
	RJson testOutput(outputJson);
	cout << "\nGeneration test: ";
	code =testOutput.parseJson();
	testOutput.parseCodeHandle(code);
	cout << endl;
}
