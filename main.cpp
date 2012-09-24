#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TextOutputter.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestFailure.h>

#define TERMINAL_PINK_START      "\033[35;1m"
#define TERMINAL_GREEN_START     "\033[32m"
#define TERMINAL_COLOR_END       "\033[0m"

class ProgressListener: public CppUnit::TestListener {
   private:
      bool _failed;
   public:

   ProgressListener(): _failed(false) {}

   void endTest(CppUnit::Test *test)
   {
      std::cout << "  " << test->getName() << "  ";
      if (_failed) {
         std::cout << TERMINAL_PINK_START << "FALED" << TERMINAL_COLOR_END << std::endl;
      } else {
         std::cout << TERMINAL_GREEN_START << "PASS" << TERMINAL_COLOR_END << std::endl;
      }

      _failed = false;
   }

   void addFailure(const CppUnit::TestFailure &failure)
   {
      _failed = true;
   }
};

int main(void)
{
   CppUnit::TextUi::TestRunner runner;
   CppUnit::TestFactoryRegistry &registry = CppUnit::TestFactoryRegistry::getRegistry("alltest");
   ProgressListener listener;

   runner.addTest(registry.makeTest());
   runner.eventManager().addListener(&listener);
   runner.run("", false, true, true);
   return runner.result().testFailuresTotal() ? 1 : 0;
}
