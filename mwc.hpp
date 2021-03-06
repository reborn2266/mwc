#ifndef _MWC_HPP_
#define _MWC_HPP_

#include <string>
#include <fstream>
#include <sstream>
#include <tr1/memory>
#include <stdint.h>
#include <gmock/gmock.h>

namespace MC {

   /* abstract interface to be a file loader */
   class file_loader {
      public:
         virtual ~file_loader() {}
         virtual void content(const std::string &fn, std::string &content) {}
   };

   /* production file_loader, should be a "file_loader" in type not necessary in inheritance relationship */
   class product_file_loader {
      public:
         void content(const std::string &fn, std::string &content);
   };

   /* abstract interface to be a ini manager */
   class ini_mgr {
      public:
         virtual ~ini_mgr() {}
         virtual void open(const char *ini_file) {}
         virtual uint32_t min_word_length() const {}
         virtual const std::set<std::string> &exclude_words() const {}
         virtual bool enable_case_sensitive() const {}
   };

   /* production ini manager, should be a "ini_mgr" in type not nessary in inheritance relationship  */
   class product_ini_mgr {
      public:
         product_ini_mgr(): min_word_length_(0), enable_case_sensitive_(false) {}
         void open(const char *ini_file);
         uint32_t min_word_length() const { return min_word_length_; }
         const std::set<std::string> &exclude_words() const { return exclude_words_; }
         bool enable_case_sensitive() const { return enable_case_sensitive_; }
      private:
         uint32_t min_word_length_;
         std::set<std::string> exclude_words_;
         bool enable_case_sensitive_;
   };

   /* abstract dependency policy for mwc */
   template<typename FL_, typename INI_>
      struct mwc_dep {
         typedef FL_ 		FL;
         typedef INI_ 		INI;
      };

   /* production dependency to be used in mwc */
   typedef mwc_dep<product_file_loader, product_ini_mgr> product_mwc_dep;

   /* mwc, our CUT, have some dependency needed to be broken by abstraction tricks */
   template <typename DEP = product_mwc_dep>
      class mwc {
         typedef typename DEP::FL 		FL;
         typedef typename DEP::INI		INI;
         protected:
         void count_word(std::string &source, std::map<std::string, uint32_t> &wc)
         {
            ini.open("./mwc.conf");

            uint32_t min_word_length = ini.min_word_length();
            const std::set<std::string> &exclude_words = ini.exclude_words();

            enable_case_sensitive = ini.enable_case_sensitive();

            if (!enable_case_sensitive) {
               for (uint32_t i = 0; i < source.length() ; ++i) {
                  source[i] = toupper(source[i]);
               }
            }

            std::stringstream ss;
            ss << source;

            do {
               std::string str;

               /* from source, get a string (delimit by space) */
               ss >> str;

               if (str.length() <= min_word_length) {
                  continue;
               }

               /* check if current str is in exclude words */
               std::set<std::string>::iterator iter = exclude_words.find(str);
               if (iter != exclude_words.end()) {
                  continue;
               }

               /* a valid str, add its count */
               wc[str] += 1;
            } while(!ss.eof());
         }

         public:
         mwc(): enable_case_sensitive(false) {}

         void load(const char *fn) 
         {
            std::string content;

            fl.content(fn, content);
            count_word(content, wc);
         }

         int query(const char *w)
         {
            return wc[w];
         }

         private:
         FL fl;
         INI ini;
         std::map<std::string, uint32_t> wc;
         bool enable_case_sensitive;
      };

} //end of ns MC

#endif //end of _MWC_HPP_
