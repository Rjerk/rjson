#include "src/rjson.h"
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

    //string js = getJsonFromFile(argv[1]);
    //    cout << "Input: \n" << js << endl;

    // json parser
    RJSON parser("1e-10000");

    cout << "\nParsing json: ";
    parser.parseJson();

    auto value = parser.getValue();
    std::cout << value->getNumber() << std::endl;
/*
	auto obj_num = value->getObjSize(); (void) obj_num;
	assert(obj_num == 7);

    auto false_val = value->getValueFromObject("false"); (void) false_val;
    assert(false_val != nullptr);
    assert(false_val->getType() == RJSON_FALSE);

    auto num_val = value->getValueFromObject("number"); (void) num_val;
    assert(num_val != nullptr);
    assert(num_val->getType() == RJSON_NUMBER);
    assert(num_val->getNumber() == 123);

    auto str_val = value->getValueFromObject("string"); (void) str_val;
    assert(str_val != nullptr);
    assert(str_val->getType() == RJSON_STRING);
    assert(*(str_val->getString()) == "ssss");

    auto arr_val = value->getValueFromObject("array"); (void) arr_val;
    assert(arr_val != nullptr);
    assert(arr_val->getType() == RJSON_ARRAY);
    assert(*(arr_val->getArray()[0].getString()) == "123");
    assert(arr_val->getArray()[1].getNumber() == 234);

    auto obj_val = value->getValueFromObject("object"); (void) obj_val;
    assert(obj_val != nullptr);
    assert(obj_val->getType() == RJSON_OBJECT);
    assert(*(obj_val->getValueFromObject("key1")->getString()) == "k1");
    assert(*(obj_val->getValueFromObject("key2")->getString()) == "k2");

    // json generator
    auto output_json = parser.generator();
    cout << "\nJson generator: \n" << output_json << endl;

    // parse the output as test.
    RJSON test_output(output_json);
    cout << "\nGeneration test: ";
    test_output.parseJson();
    cout << endl;
*/
}
