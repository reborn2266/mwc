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
         virtual ~file_loader() = 0;
         virtual void content(const std::string &fn, std::string &content) = 0;
   };

   /* production file_loader, should be a "file_loader" in type not necessary in inheritance relationship */
   class product_file_loader {
      public:
         void content(const std::string &fn, std::string &content)
         {
            std::ifstream ifs(fn.c_str());

            while (ifs.good()) {
               int c = ifs.get();
               if (ifs.good()) {
                  content += c;
               }
            }
         }
   };

   /* mock file_loader for UT */
   class mock_file_loader: public file_loader {
      public:
         MOCK_CONST_METHOD2(content, void (const std::string &, std::string &));
   };

   /* abstract interface to be a ini manager */
   class ini_mgr {
      public:
         virtual ~ini_mgr() =0;
         virtual void open(const char *ini_file);
         virtual uint32_t min_word_length() const;
         virtual const std::set<std::string> &exclude_words() const;
         virtual bool enable_case_sensitive() const;
   };

   /* production ini manager, should be a "ini_mgr" in type not nessary in inheritance relationship  */
   class product_ini_mgr {
      public:
         product_ini_mgr(): min_word_length_(0), enable_case_sensitive_(false) {}
         void open(const char *ini_file)
         {
            std::ifstream ifs(ini_file);
            char buf[256];
            
            while (ifs.good()) {
               ifs.getline(buf, 256);
               if (ifs.good()) {
                  std::stringstream ss(buf);
                  std::string attr;
                  std::string tmp;
                  ss >> attr;
                  ss >> tmp;
                  if (attr == "min_word_length") {
                     ss >> min_word_length_;
                  } else if (attr == "exclude_words") {
                     do {
                        std::string w;
                        ss >> w;
                        if (w[w.length() - 1] == ',') {
                           w[w.length() - 1] = '\0';
                           exclude_words_.insert(w);
                        } else {
                           exclude_words_.insert(w);
                           break;
                        }
                     } while(true);
                  } else if (attr == "enable_case_sensitive") {
                     std::string w;
                     ss >> w;
                     if (w == "True") {
                        enable_case_sensitive_ = true;
                     } else {
                        enable_case_sensitive_ = false;
                     }
                  }
               }
            }
         }

         uint32_t min_word_length() const { return min_word_length_; }
         const std::set<std::string> &exclude_words() const { return exclude_words_; }
         bool enable_case_sensitive() const { return enable_case_sensitive_; }
      private:
         uint32_t min_word_length_;
         std::set<std::string> exclude_words_;
         bool enable_case_sensitive_;
   };

   /* mock ini manager */
   class mock_ini_mgr: public ini_mgr {
      public:
         MOCK_METHOD1(open, void(const char *));
         MOCK_CONST_METHOD0(min_word_length, uint32_t());
         MOCK_CONST_METHOD0(exclude_words, const std::set<std::string>& ());
         MOCK_CONST_METHOD0(enable_case_sensitive, bool ());
   };

   /* abstract dependency policy for mwc */
   template<typename FL_, typename INI_>
      struct mwc_dep {
         typedef FL_ 		FL;
         typedef INI_ 		INI;
      };

   /* production dependency to be used in mwc */
   typedef mwc_dep<product_file_loader, product_ini_mgr> product_mwc_dep;

   /* unit test dependency break used in mwc, simailar mock_mwc_dep type must be defined when testing */
   //typedef mwc_mock<mock_file_loader, mock_ini_mgr> mock_mwc_dep;

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
#if 1
            for(std::map<std::string, uint32_t>::iterator iter = wc.begin();
                  iter != wc.end(); ++iter) {
               std::cout << iter->first << " " << iter->second << std::endl;
            }
#endif
         }

         public:
         mwc(): enable_case_sensitive(false) {}

         void load(const char *fn) 
         {
            std::string content;

            fl.content(fn, content);
            count_word(content, wc);
         }

         uint32_t query(const char *w)
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
