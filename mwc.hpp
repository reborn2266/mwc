#ifndef _MWC_HPP_
#define _MWC_HPP_

#include <string>
#include <tr1/memory>
#include <stdint.h>
#include <gmock/gmock.h>

namespace MC {

class file_loader {
   public:
      file_loader(const char *f);
      virtual ~file_loader() = 0;
      
      virtual const std::string& content(const char *f) = 0;
};

/* production file_loader */
class real_file_loader: public file_loader {
   public:
      const std::string& content(const char *f);
   
   private:
      std::string file_content;
};

/* mock file_loader for UT */
class mock_file_loader: public file_loader {
   public:
      MOCK_CONST_METHOD1(content, const std::string&(const char *));
};

/* mwc, our CUT, have some dependency needed to be broken by abstraction tricks */
template <typename FL = real_file_loader>
class mwc {
   protected:
      std::tr1::shared_ptr<FL> 
      creat_fl(const std::string &fn)
      {
         if (fn == std::string()) {
            return (new FL);
         }

         return 0;
      }

   public:
      mwc() {}
      mwc(const char *fn)
      {
         fl = create_fl(fn);
      }

      void load(const char *fn)
      {
         //fl = create_fl(fn);
      }

      uint32_t query(const char *w);

   private:
      std::tr1::shared_ptr<file_loader> fl;
};

} //end of ns MC

#endif //end of _MWC_HPP_
