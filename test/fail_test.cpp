#define BOOST_TEST_MODULE RJSON FAIL TEST
#include <boost/test/included/unit_test.hpp>
#include "../src/rjson.h"

using namespace rjson;

//---------------INVLIAD_VALUE-------------------

BOOST_AUTO_TEST_SUITE(INVALID_VALUE)

BOOST_AUTO_TEST_CASE(fail01)
{
    RJSON parser("Fail");
    BOOST_TEST(parser.parseJson() == PARSE_INVALID_VALUE);
}

BOOST_AUTO_TEST_CASE(fail02)
{
    RJSON parser("TrUe");
    BOOST_TEST(parser.parseJson() == PARSE_INVALID_VALUE);
}

BOOST_AUTO_TEST_CASE(fail03)
{
    RJSON parser("NUll");
    BOOST_TEST(parser.parseJson() == PARSE_INVALID_VALUE);
}

BOOST_AUTO_TEST_CASE(fail04)
{
    RJSON parser("?");
    BOOST_TEST(parser.parseJson() == PARSE_INVALID_VALUE);
}

BOOST_AUTO_TEST_SUITE_END()


// -----------------NUMBER_TOO_BIG--------------------

BOOST_AUTO_TEST_SUITE(NUMBER_TEST_TOO_BIG)

#define TEST_NUMBER_TOO_BIG(expect, json) \
    do { \
        parser.setJsonText(json); \
        BOOST_TEST(parser.parseJson() == PARSE_NUMBER_TOO_BIG); \
    } while (0);

BOOST_AUTO_TEST_CASE(NUMBER_TOO_BIG)
{
    RJSON parser("");
    TEST_NUMBER_TOO_BIG(0.0, "1e-10000");
    TEST_NUMBER_TOO_BIG( 4.9406564584124654e-324, "4.9406564584124654e-324");
    TEST_NUMBER_TOO_BIG(-4.9406564584124654e-324, "-4.9406564584124654e-324");
    TEST_NUMBER_TOO_BIG( 2.2250738585072009e-308, "2.2250738585072009e-308")
    TEST_NUMBER_TOO_BIG(-2.2250738585072009e-308, "-2.2250738585072009e-308");
}

BOOST_AUTO_TEST_SUITE_END()







