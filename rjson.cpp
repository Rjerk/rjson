#include "rjson.h"
#include <fstream>
#include <sstream>
#include <cstring>
#include <cmath>
const size_t STACK_INIT_SIZE = 100;

RJson::RJson(const string& js)
{
	json = const_cast<char*>(js.data());
	stack = new char[STACK_INIT_SIZE];
	top = 0;
	size = STACK_INIT_SIZE;
	v.type = RJSON_NULL;
}

RJson::~RJson()
{
	delete [] stack;
	json = nullptr;
}

void RJson::parseJson()
{
	cleanWhitespace();
	parse_code code;
	if ((code = parseValue(&v)) == PARSE_OK) {
		if (*json != '\0') {
			code = PARSE_NOT_SINGULAR_VALUE;
		}
	}
	parseCodeHandle(code);
}

string RJson::generator()
{
//	if (jsonValue.type == RJSON_STRING) {
//		return string(jsonValue.s);
//	} else {
//		return string();
//	}
	return string();
}

parse_code RJson::parseValue(json_value_t* v)
{
	switch (*json) {
		case 't':  return parseLiteral(v, "true", RJSON_TRUE);
		case 'f':  return parseLiteral(v, "false", RJSON_FALSE);
		case 'n':  return parseLiteral(v, "null", RJSON_NULL);
		case '\"': return parseString(v);
		case '{':  return parseObject(v);
		case '[':  return parseArray(v);
		case '\0': return PARSE_OK;
		default:   return parseNumber(v);
	}
	return PARSE_INVALID_VALUE;
}

parse_code RJson::parseLiteral(json_value_t* v, const string& literal, const json_type type)
{
	eatChar(literal[0]);
	size_t i = 0;
	for (; literal[i+1]; ++i) {
		if (json[i] != literal[i+1])
			return PARSE_INVALID_VALUE;
	}
	json += i;
	v->type = type;
	return PARSE_OK;
}

parse_code RJson::parseNumber(json_value_t* v)
{
	const char* p = json;
	if (*p == '-')
		++p;

	if (*p == '0') {
		++p;
	} else {
		if (!isdigit_1to9(*p)) return PARSE_INVALID_VALUE;
		while (isdigit(*p)) ++p;
	}

	if (*p == '.') {
		++p;
		if (!isdigit(*p)) return PARSE_INVALID_VALUE;
		while (isdigit(*p)) ++p;
	}

	if (*p == 'e' || *p == 'E') {
		++p;
		if (*p == '+' || *p == '-')
			++p;
		if (!isdigit(*p)) return PARSE_INVALID_VALUE;
		while (isdigit(*p)) ++p;
	}
	errno = 0;
	double d = std::strtod(json, NULL);
	if ((errno == ERANGE) || (d == HUGE_VAL) || (d == -HUGE_VAL))
		return PARSE_NUMBER_TOO_BIG;
	v->num = d;
	v->type = RJSON_NUMBER;
	json = p;
	return PARSE_OK;
}

void* RJson::pushJson(size_t sz)
{
	assert(sz > 0);
	while (sz + top >= size)
		size += (size >> 1);
	char* tmp = new char[size];
	memcpy(tmp, stack, top);
	stack = tmp;
	tmp = nullptr;

	int t = top;
	top += sz;
	return stack + t;
}

void* RJson::popJson(size_t sz)
{
	assert(sz <= top);
	return stack + (top -= sz);
}

const char* RJson::parse4HexDigits(const char* p, unsigned* u)
{
	std::stringstream ss;
	string hex4;
	*u = 0;
	for (int i = 0; i < 4; ++i) {
		if (isxdigit(*p))
			hex4 += *p++;
		else
			return NULL;
	}
	ss << std::hex << hex4;
	ss >> *u;
	return p;
}

void RJson::encodeUTF8(unsigned u)
{
	if (u <= 0x7F) { // xxxxxxx -> 0xxxxxxx
		*((char*)pushJson(sizeof(char))) = (u & 0xFF);
	} else if (u <= 0x7FF) { // xxxxx xxxxxx -> 110xxxxx 10xxxxxx
		*((char*)pushJson(sizeof(char))) = (0xC0 | ((u >> 6) & 0xFF));
		*((char*)pushJson(sizeof(char))) = (0x80 | ( u       & 0x3F));
	} else if (u <= 0xFFFF) { // xxxx xxxxxx xxxxxx -> 1110xxxx 10xxxxxx 10xxxxxx
		*((char*)pushJson(sizeof(char))) = (0xE0 | ((u >> 12) & 0xFF));
		*((char*)pushJson(sizeof(char))) = (0x80 | ((u >>  6) & 0x3F));
		*((char*)pushJson(sizeof(char))) = (0x80 | ( u        & 0x3F));
	} else {
		assert(u <= 0x10FFFF);// xxx xxxxxx xxxxxx xxxxxx -> 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
		*((char*)pushJson(sizeof(char))) = (0xF0 | ((u >> 18) & 0xFF));
		*((char*)pushJson(sizeof(char))) = (0x80 | ((u >> 12) & 0x3F));
		*((char*)pushJson(sizeof(char))) = (0x80 | ((u >>  6) & 0x3F));
		*((char*)pushJson(sizeof(char))) = (0x80 | ( u        & 0x3F));
	}
}

parse_code RJson::parseStringRaw(json_value_t* v, char** str, size_t* len)
{
	eatChar('\"');
	const char* p = json;
	size_t pos = top;
	unsigned u, u2;
	for (;;) {
		char ch = *p++;
		switch (ch) {
			case '\"':
				*len = top - pos;
				*str = (char*)popJson(*len);
				//*str[*len] = '\0';
				json = p;
				return PARSE_OK;
			case '\\': {
				switch (*p++) {
					case '\"': *((char*)pushJson(sizeof(ch))) = '\"'; break;
					case '\\': *((char*)pushJson(sizeof(ch))) = '\\'; break;
					case '/':  *((char*)pushJson(sizeof(ch))) = '/';  break;
					case 'b':  *((char*)pushJson(sizeof(ch))) = '\b'; break;
					case 'f':  *((char*)pushJson(sizeof(ch))) = '\f'; break;
					case 'n':  *((char*)pushJson(sizeof(ch))) = '\n'; break;
					case 'r':  *((char*)pushJson(sizeof(ch))) = '\r'; break;
					case 't':  *((char*)pushJson(sizeof(ch))) = '\t'; break;
					case 'u': {
						if (!(p = parse4HexDigits(p, &u))) {
							top = pos;
							return PARSE_INVALID_UNICODE_HEX;
						}
						// first codepoint is high surrogate, then handle low surrogate
						if (u >= 0xD800 && u <= 0xDBFF) {
							if (*p++ != '\\') {
								top = pos;
								return PARSE_INVALID_UNICODE_SURROGATE;
							}
							if (*p++ != 'u') {
								top = pos;
								return PARSE_INVALID_UNICODE_SURROGATE;
							}
							if (!(p = parse4HexDigits(p, &u2))) {
								top = pos;
								return PARSE_INVALID_UNICODE_HEX;
							}
							if (u2 < 0xDC00 || u2 > 0xDFFF) {
								top = pos;
								return PARSE_INVALID_UNICODE_SURROGATE;
							}
							// codepoint = 0x10000 + (H - 0xD800) × 0x400 + (L - 0xDC00)
							u = (((u - 0xD800) << 10) | (u2 - 0xDC00)) + 0x1000;
						}
						encodeUTF8(u);
						break;
					}
					default:
						top = pos;
						return PARSE_INVALID_ESCAPE_CHARCTER;
				}
				break;
			}
			case '\0':
				top = pos;
				return PARSE_MISS_QUOTATION_MARK;
			default:
				if ((unsigned char) ch < 0x20) {
					top = pos;
					return PARSE_INVALID_STRING_CHAR;
				}
				*((char*)pushJson(sizeof(ch))) = ch;
		}
	}
}

void RJson::setString(json_value_t*v, const char* str, size_t len)
{
	assert(v != NULL && (str != NULL || len == 0));
	freeValue(v);
	v->s = new char[len+1];
	memcpy(v->s, str, len);
	v->s[len] = '\0';
	v->len = len;
	v->type = RJSON_STRING;
}

parse_code RJson::parseString(json_value_t* v)
{
	parse_code ret;
	char* s;
	size_t len;
	if ((ret = parseStringRaw(v, &s, &len)) == PARSE_OK) {
		setString(v, s, len);
	}
	return ret;
}

parse_code RJson::parseObject(json_value_t* v)
{
	eatChar('{');
	cleanWhitespace();
	if (*json == '}') {
		++json;
		v->type = RJSON_OBJECT;
		v->pair = nullptr;
		v->objSize = 0;
		return PARSE_OK;
	}

	json_pair_t pair;
	size_t sz = 0;
	parse_code ret = PARSE_INVALID_VALUE;

	while (true) {
		char* str = nullptr;
		v->type = RJSON_NULL;
		pair.str = nullptr;
		pair.len = 0;
		pair.value.type = RJSON_NULL;
		// parse string
		if (*json != '\"') {
			ret = PARSE_MISS_KEY;
			break;
		}
		if ((ret = parseStringRaw(v, &str, &pair.len)) != PARSE_OK)
			break;
		memcpy(pair.str = new char[pair.len+1], str, pair.len);
		pair.str[pair.len] = '\0';
		str = nullptr;

		cleanWhitespace();
		if (*json != ':') {
			ret = PARSE_MISS_COLON;
			break;
		}
		++json;
		cleanWhitespace();
		// parse value
		if ((ret = parseValue(&pair.value)) != PARSE_OK)
			break;
		memcpy(pushJson(sizeof(json_pair_t)), &pair, sizeof(json_pair_t));
		pair.str = nullptr;
		++sz;

		cleanWhitespace();
		if (*json == ',') {
			++json;
			cleanWhitespace();
		} else if (*json == '}') {
			size_t sumsize = sizeof(json_pair_t) * sz;
			++json;
			v->type = RJSON_OBJECT;
			v->objSize = sz;
			memcpy(v->pair = new json_pair_t[sumsize], popJson(sumsize), sumsize);
			return PARSE_OK;
		} else {
			ret = PARSE_MISS_COMMA_OR_SQUARE_BRACKET;
			break;
		}
	}

	delete [] pair.str;
	for (size_t i = 0; i < sz; ++i) {
		json_pair_t* p = (json_pair_t*)popJson(sizeof(json_pair_t));
		delete [] p->str;
		freeValue(v);
	}

	v->type = RJSON_NULL;
	return ret;
}

parse_code RJson::parseArray(json_value_t* v)
{
	eatChar('[');
	cleanWhitespace();
    parse_code ret = PARSE_INVALID_VALUE;
    return ret;
}

void RJson::freeValue(json_value_t* v)
{
	assert(v != NULL);
	switch (v->type) {
		case RJSON_STRING:
			delete [] v->s;
			break;
		case RJSON_OBJECT:
			for (size_t i = 0; i < v->objSize; ++i) {
				delete [] v->pair[i].str;
				freeValue(&v->pair[i].value);
			}
			delete [] v->pair;
			break;
//		case RJSON_ARRAY:
//			for (int i = 0; i < v->arrSize; ++i) {
//				freeValue(&v->elem[i]);
//			}
//			delete [] v->elem;
//			break;
		default:
			break;
	}
	v->type = RJSON_NULL;
}

void RJson::parseCodeHandle(parse_code code)
{
	switch (code) {
		case PARSE_OK:
			cout << "Parse ok." << endl;
			freeValue(&v);
			break;
		case PARSE_INVALID_VALUE:
			cout << "Invalid value."; break;
		case PARSE_MISS_COLON:
			cout << "Miss colon." << endl; break;
		case PARSE_NOT_SINGULAR_VALUE:
			cout << "Not singular value." << endl; break;
		case PARSE_NUMBER_TOO_BIG:
			cout << "Number is too big." << endl; break;
		case PARSE_INVALID_ESCAPE_CHARCTER:
			cout << "Invalid escaped character." << endl; break;
		case PARSE_INVALID_UNICODE_HEX:
			cout << "Invalid unicode hexadecimal digits." << endl; break;
		case PARSE_MISS_QUOTATION_MARK:
			cout << "Miss quotation mark in string." << endl; break;
		case PARSE_INVALID_STRING_CHAR:
			cout << "Invalid char in string." << endl; break;
		case PARSE_INVALID_UNICODE_SURROGATE:
			cout << "Invalid unicode surrogate in string." << endl; break;
		default:
			cout << "Unexpected error." << endl;
	}
//	if (code == PARSE_OK) {
//		cout << "Json generator:\n" << generator() << endl;
//	}
}

string getJsonFromFile(const string& filename)
{
	std::ifstream t(filename);
	std::stringstream buffer;
	buffer << t.rdbuf();
	return buffer.str();
}
