#include <iostream>
#include <tr1/memory>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestAssert.h>

#include "mwc.hpp"

class ProductMWCTester: public CppUnit::TestFixture
{
   CPPUNIT_TEST_SUITE(ProductMWCTester);
   CPPUNIT_TEST(MWC_QueryValidWord_ReturnCount);
   CPPUNIT_TEST(MWC_QueryTooShortValidWord_ReturnCount);
   CPPUNIT_TEST(MWC_QueryExcludeWord_ReturnCount);
   CPPUNIT_TEST_SUITE_END();
public:
   ProductMWCTester() {}
   void setUp() {}
   void tearDown() {}
   void MWC_QueryValidWord_ReturnCount();
   void MWC_QueryTooShortValidWord_ReturnCount();
   void MWC_QueryExcludeWord_ReturnCount();
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ProductMWCTester, "alltest");

void ProductMWCTester::MWC_QueryValidWord_ReturnCount()
{
   MC::mwc<> wc;

   wc.load("./data");

   CPPUNIT_ASSERT_EQUAL(2, wc.query("ooooo"));
}

void ProductMWCTester::MWC_QueryTooShortValidWord_ReturnCount()
{
   MC::mwc<> wc;

   wc.load("./data");

   CPPUNIT_ASSERT_EQUAL(0, wc.query("I"));
}

void ProductMWCTester::MWC_QueryExcludeWord_ReturnCount()
{
   MC::mwc<> wc;

   wc.load("./data");

   CPPUNIT_ASSERT_EQUAL(0, wc.query("There"));
}
