#include <iostream>
#include <tr1/memory>
#include <cppunit/extensions/HelperMacros.h>

#include "mwc.hpp"

class ProductMWCTester: public CppUnit::TestFixture
{
   CPPUNIT_TEST_SUITE(ProductMWCTester);
   CPPUNIT_TEST(MWC_QueryValidWord_ReturnCount);
   CPPUNIT_TEST_SUITE_END();
public:
   ProductMWCTester() {}
   void setUp() {}
   void tearDown() {}
   void MWC_QueryValidWord_ReturnCount();
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ProductMWCTester, "alltest");

void ProductMWCTester::MWC_QueryValidWord_ReturnCount()
{
   MC::mwc<> wc;

   wc.load("./data");

   CPPUNIT_ASSERT(2 == wc.query("ooooo"));
}
