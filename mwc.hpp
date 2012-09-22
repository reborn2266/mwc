#ifndef _MWC_HPP_
#define _MWC_HPP_

#include <string>
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
      void content(const std::string &fn, std::string &content);
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
};

/* production ini manager, should be a "ini_mgr" in type not nessary in inheritance relationship  */
class product_ini_mgr {
	public:
		void open(const char *ini_file);
		uint32_t min_word_length() const;
		void exclude_words(std::set<std::string> &ew) const;
		bool enable_case_sensitive() const;
};

/* mock ini manager */
class mock_ini_mgr: public ini_mgr {
	public:
		MOCK_METHOD1(open, void(const char *));
		MOCK_CONST_METHOD0(min_word_length, uint32_t());
		MOCK_CONST_METHOD1(exclude_words, void (std::set<std::string>));
		MOCK_CONST_METHOD0(enable_case_sensitive, bool ());
};

/* abstract dependency policy for mwc */
template<typename FL, typename INI>
struct mwc_dep {
	typedef FL 		file_loader_type;
	typedef INI 	ini_mgr_type;
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
      void count_word(const std::string &source, std::map<std::string, uint32_t> &wc)
	  {
		 ini.open("./mwc.conf");

		 std::set<std::string> exclude_words;
	     uint32_t min_word_length = ini.min_word_length();
		 
		 enable_case_sensitive = ini.enable_case_sensitive();
		 ini.exclude_words(exclude_words);

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
