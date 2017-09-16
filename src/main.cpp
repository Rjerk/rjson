#include "rjson.h"
#include <string>
#include <iostream>

using namespace rjson;
using std::string;
using std::cout;
using std::endl;

int main(int argc, char** argv)
{
    if (argc != 2) {
        std::cerr << "Usage: ./rjson filename." << endl;
    	return -1;
    }

    string js = getJsonFromFile(string(argv[1]));
    cout << "Input: \n" << js << endl;

    // json parser
    JsonParser parser(js);

    cout << "\nParsing json: ";
    parse_code code = parser.parseJson();
    parser.parseCodeHandle(code);

    auto value = parser.getValue();

	auto obj_num = value->getObjSize(); (void) obj_num;
	assert(obj_num == 7);

    auto num_val = value->getValueFromObject("number"); (void) num_val;
    assert(num_val != nullptr);
    assert(num_val->getType() == RJSON_NUMBER);
    assert(num_val->getNumber() == 123);

    auto str_val = value->getValueFromObject("string"); (void) str_val;
    assert(str_val != nullptr);
    assert(str_val->getType() == RJSON_STRING);
    assert(*(str_val->getString()) == "ssss");

    // json generator
    auto outputJson = parser.generator();
    cout << "\nJson generator: \n" << outputJson << endl;

    // parse the output as test.
    JsonParser testOutput(outputJson);
    cout << "\nGeneration test: ";
    code = testOutput.parseJson();
    testOutput.parseCodeHandle(code);
    cout << endl;
}
