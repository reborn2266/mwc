#include <iostream>
#include <tr1/memory>
#include <cppunit/extensions/HelperMacros.h>

#include "mwc.hpp"

class MockMWCTester: public CppUnit::TestFixture
{
   CPPUNIT_TEST_SUITE(MockMWCTester);
   CPPUNIT_TEST(MWC_QueryValidWord_ReturnCount);
   CPPUNIT_TEST(MWC_QueryTooShortValidWord_ReturnCount);
   CPPUNIT_TEST(MWC_QueryExcludeWord_ReturnCount);
   CPPUNIT_TEST_SUITE_END();
public:
   MockMWCTester() {}
   void setUp() {}
   void tearDown() {}
   void MWC_QueryValidWord_ReturnCount();
   void MWC_QueryTooShortValidWord_ReturnCount();
   void MWC_QueryExcludeWord_ReturnCount();
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(MockMWCTester, "alltest");

class mock_file_loader0: public MC::file_loader {
   public:
      void content(const std::string &fn, std::string &source)
      {
         source = "";
         source += "ooooo\nI\nam who ooooo\nwho ope\nThere\n";
      }
};

class mock_ini_mgr0: public MC::ini_mgr {
   public:
      void open(const char *ini_file)
      {
         exclude_words_.insert("The"); 
         exclude_words_.insert("I"); 
         exclude_words_.insert("There"); 
      }

      uint32_t min_word_length() const { return 2; }

      const std::set<std::string> &exclude_words() const { return exclude_words_; }

      bool enable_case_sensitive() const { return true; }
   private:
      std::set<std::string> exclude_words_;
};

void MockMWCTester::MWC_QueryValidWord_ReturnCount()
{
   typedef MC::mwc_dep<mock_file_loader0, mock_ini_mgr0> mock_mwc_dep;
   MC::mwc<mock_mwc_dep> wc;
   wc.load("./data");

   CPPUNIT_ASSERT(2 == wc.query("ooooo"));
}

void MockMWCTester::MWC_QueryTooShortValidWord_ReturnCount()
{
   typedef MC::mwc_dep<mock_file_loader0, mock_ini_mgr0> mock_mwc_dep;
   MC::mwc<mock_mwc_dep> wc;

   wc.load("./data");

   CPPUNIT_ASSERT_EQUAL(0, wc.query("I"));
}

void MockMWCTester::MWC_QueryExcludeWord_ReturnCount()
{
   typedef MC::mwc_dep<mock_file_loader0, mock_ini_mgr0> mock_mwc_dep;
   MC::mwc<mock_mwc_dep> wc;

   wc.load("./data");

   CPPUNIT_ASSERT_EQUAL(0, wc.query("There"));
}
