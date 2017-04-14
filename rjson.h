#include <string>
#include <iostream>
#include <memory>
#include <cassert>
#include <cctype>

using std::cin;
using std::cout;
using std::endl;
using std::cerr;
using std::string;
using std::shared_ptr;

string getJsonFromFile(const string& filename);

using json_value_t = struct json_value;
using json_pair_t = struct json_pair;

enum json_type {
	RJSON_STRING,
	RJSON_NUMBER,
	RJSON_OBJECT,
	RJSON_ARRAY,
	RJSON_FALSE,
	RJSON_TRUE,
	RJSON_NULL
};

struct json_value {
	json_type type;
	union {
		double num; // number
		struct {
			char* s;
			size_t len;
		};
		struct { // object
			json_pair_t* pair;
			size_t objSize;
		};
		struct { // array
			json_value_t* elem;
			size_t arrSize;
		};
	};
};

struct json_pair {
	char* str;
	size_t len;
	json_value_t value;
};

enum parse_code {
	PARSE_OK,
	PARSE_INVALID_VALUE,
	PARSE_NOT_SINGULAR_VALUE,
	PARSE_NUMBER_TOO_BIG,
	PARSE_INVALID_ESCAPE_CHARCTER,
	PARSE_INVALID_UNICODE_HEX,
	PARSE_MISS_QUOTATION_MARK,
	PARSE_INVALID_STRING_CHAR,
	PARSE_INVALID_UNICODE_SURROGATE,
	PARSE_MISS_KEY,
	PARSE_MISS_COLON,
	PARSE_MISS_COMMA_OR_SQUARE_BRACKET
};


class RJson {
public:
	RJson(const string& js);
	~RJson();
	void parseJson();
	string generator();
private:
	parse_code parseValue(json_value_t* v);
	parse_code parseLiteral(json_value_t* v, const string& literal, json_type type);
	parse_code parseString(json_value_t* v);
	parse_code parseStringRaw(json_value_t* v, char** str, size_t* len);
	parse_code parseNumber(json_value_t* v);
	parse_code parseObject(json_value_t* v);
	parse_code parseArray(json_value_t* v);
	void* pushJson(size_t sz);
	void* popJson(size_t sz);
	void freeValue(json_value_t* v);
	void setString(json_value_t* v, const char* str, size_t len);
	void parseCodeHandle(parse_code code);
	void encodeUTF8(unsigned u);
	const char* parse4HexDigits(const char* p, unsigned* u);
	void cleanWhitespace() { while (isspace(*json)) ++json; }
	void eatChar(char ch) { assert(*json == ch); ++json; }
	bool isdigit_1to9(char ch) { return ch != '0' && isdigit(ch); }
private:
	const char* json;
	json_value_t jsonValue;
private:
	char* stack;
	size_t top;
	size_t size;
};
