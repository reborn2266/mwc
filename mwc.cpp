#include <string>
#include <fstream>
#include <sstream>

#include "mwc.hpp"

namespace MC {

   void product_file_loader::content(const std::string &fn, std::string &content)
   {
      std::ifstream ifs(fn.c_str());

      while (ifs.good()) {
         int c = ifs.get();
         if (ifs.good()) {
            content += c;
         }
      }
   }
   
   void product_ini_mgr::open(const char *ini_file)
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
} //end of ns MC
