#define BOOST_TEST_MODULE RJSON PASS TEST
#include <boost/test/included/unit_test.hpp>
#include "../src/rjson.h"
#include <string>

using namespace rjson;

// -------------LITERAL_TEST_OK-----------------

BOOST_AUTO_TEST_SUITE(OK)

BOOST_AUTO_TEST_CASE(success01)
{
    RJSON parser("null");
    BOOST_TEST(parser.parseJson() == PARSE_OK);
}

BOOST_AUTO_TEST_CASE(success02)
{
    RJSON parser("true");
    BOOST_TEST(parser.parseJson() == PARSE_OK);
}

BOOST_AUTO_TEST_CASE(success03)
{
    RJSON parser("false");
    BOOST_TEST(parser.parseJson() == PARSE_OK);
}

BOOST_AUTO_TEST_CASE(success04)
{
    RJSON parser("null");
    BOOST_TEST(parser.parseJson() == PARSE_OK);
}

BOOST_AUTO_TEST_SUITE_END()

// ----------------NUMBER_TEST_OK-----------------

BOOST_AUTO_TEST_SUITE(NUMBER_TEST)

#define TEST_NUMBER_OK(expect, json) \
    do { \
        parser.setJsonText(json); \
        BOOST_TEST(parser.parseJson() == PARSE_OK); \
        auto num_val = parser.getValue(); \
        BOOST_TEST(num_val->getType() == RJSON_NUMBER); \
        BOOST_TEST(num_val->getNumber() == expect); \
    } while (0);

BOOST_AUTO_TEST_CASE(NUMBER_TEST_OK)
{
    RJSON parser("");
    TEST_NUMBER_OK(0.0, "0");
    TEST_NUMBER_OK(0.0, "-0");
    TEST_NUMBER_OK(0.0, "-0.0");
    TEST_NUMBER_OK(1.0, "1");
    TEST_NUMBER_OK(-1.0, "-1");
    TEST_NUMBER_OK(1.5, "1.5");
    TEST_NUMBER_OK(-1.5, "-1.5");
    TEST_NUMBER_OK(3.1416, "3.1416");
    TEST_NUMBER_OK(1E10, "1E10");
    TEST_NUMBER_OK(1e10, "1e10");
    TEST_NUMBER_OK(1E+10, "1E+10");
    TEST_NUMBER_OK(1E-10, "1E-10");
    TEST_NUMBER_OK(-1E10, "-1E10");
    TEST_NUMBER_OK(-1e10, "-1e10");
    TEST_NUMBER_OK(-1E+10, "-1E+10");
    TEST_NUMBER_OK(-1E-10, "-1E-10");
    TEST_NUMBER_OK(1.234E+10, "1.234E+10");
    TEST_NUMBER_OK(1.234E-10, "1.234E-10");
    TEST_NUMBER_OK(1.0000000000000002, "1.0000000000000002");
    //TEST_NUMBER_OK( 4.9406564584124654e-324, "4.9406564584124654e-324"); /* minimum denormal */
    //TEST_NUMBER_OK(-4.9406564584124654e-324, "-4.9406564584124654e-324");
    //TEST_NUMBER_OK(2.2250738585072009e-308, "2.2250738585072009e-308");  /* Max subnormal double */
    //TEST_NUMBER_OK( -2.2250738585072009e-308, "-2.2250738585072009e-308");
    TEST_NUMBER_OK(2.2250738585072014e-308, "2.2250738585072014e-308");  /* Min normal positive double */
    TEST_NUMBER_OK(-2.2250738585072014e-308, "-2.2250738585072014e-308");
    TEST_NUMBER_OK( 1.7976931348623157e+308, "1.7976931348623157e+308");  /* Max double */
    TEST_NUMBER_OK(-1.7976931348623157e+308, "-1.7976931348623157e+308");
}

BOOST_AUTO_TEST_SUITE_END()


// ------------------- STRING_TEST_OK---------------------

BOOST_AUTO_TEST_SUITE(STRING_TEST)

#define TEST_STRING_OK(expect, json) \
    do { \
        std::cout << "Expect: " << std::string(expect) << " " << "Json: " << json << std::endl; \
        parser.setJsonText(json); \
        BOOST_TEST(parser.parseJson() == PARSE_OK); \
        auto str_val = parser.getValue(); \
        BOOST_TEST(str_val->getType() == RJSON_STRING); \
        BOOST_TEST(*(str_val->getString()) == std::string(expect)); \
    } while (0);

BOOST_AUTO_TEST_CASE(STRING_TEST_OK)
{
    RJSON parser("");
    TEST_STRING_OK("", "\"\"");
    TEST_STRING_OK("Hello", "\"Hello\"");
    TEST_STRING_OK("Hello\nWorld", "\"Hello\\nWorld\"");
    TEST_STRING_OK("\" \\ / \b \f \n \r \t", "\"\\\" \\\\ \\/ \\b \\f \\n \\r \\t\"");
    TEST_STRING_OK("Hello\0World", "\"Hello\\u0000World\"");
    TEST_STRING_OK("\x24", "\"\\u0024\"");         /* Dollar sign U+0024 */
    TEST_STRING_OK("\xC2\xA2", "\"\\u00A2\"");     /* Cents sign U+00A2 */
    TEST_STRING_OK("\xE2\x82\xAC", "\"\\u20AC\""); /* Euro sign U+20AC */
    TEST_STRING_OK("\xF0\x9D\x84\x9E", "\"\\uD834\\uDD1E\"");  /* G clef sign U+1D11E */
    TEST_STRING_OK("\xF0\x9D\x84\x9E", "\"\\ud834\\udd1e\"");  /* G clef sign U+1D11E */
}

BOOST_AUTO_TEST_SUITE_END()

