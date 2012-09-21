#ifndef _MWC_HPP_
#define _MWC_HPP_

#include <string>
#include <tr1/memory>
#include <stdint.h>
#include <gmock/gmock.h>

namespace MC {

class file_loader {
   public:
      virtual ~file_loader() = 0;
      virtual void content(const std::string &fn, std::string &content) = 0;
};

/* production file_loader */
class real_file_loader: public file_loader {
   public:
      void content(const std::string &fn, std::string &content);
};

/* mock file_loader for UT */
class mock_file_loader: public file_loader {
   public:
      MOCK_CONST_METHOD2(content, void (const std::string &, std::string &));
};

/* mwc, our CUT, have some dependency needed to be broken by abstraction tricks */
template <typename FL = real_file_loader>
class mwc {
   protected:
      std::tr1::shared_ptr<FL> 
      creat_fl() { return (new FL); }

   public:
      mwc() { fl = mwc::create_fl(); }

      void load(const char *fn) 
      {
         fl->content(fn, content);
      }

      uint32_t query(const char *w);

   private:
      std::tr1::shared_ptr<FL> fl;
      std::string content;
};

} //end of ns MC

#endif //end of _MWC_HPP_
