/**
 * @file <argos3/core/utility/string_utilities.cpp>
 *
 * @brief This file provides the implementation of
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 * @author Vito Trianni - <vito.trianni@istc.cnr.it>
 */

#include "string_utilities.h"
#include <cstdlib>
#include <regex.h>

namespace argos {

   void Tokenize(const std::string& str_string,
                 std::vector<std::string>& vec_tokens,
                 const std::string& str_delimiters) {
      // this taken from
      // http://oopweb.com/CPP/Documents/CPPHOWTO/Volume/C++Programming-HOWTO-7.html

      /* Skip delimiters at beginning */
      std::string::size_type lastPos =
               str_string.find_first_not_of(str_delimiters, 0);

      /* Find first "non-delimiter" */
      std::string::size_type pos = str_string.find_first_of(str_delimiters,
                                                            lastPos);

      while(std::string::npos != pos || std::string::npos != lastPos) {
         /* Found a token, add it to the vector */
         vec_tokens.push_back(str_string.substr(lastPos, pos - lastPos));

         /* Skip delimiters.  Note the "not_of" */
         lastPos = str_string.find_first_not_of(str_delimiters, pos);

         /* Find next "non-delimiter" */
         pos = str_string.find_first_of(str_delimiters, lastPos);
      }
   }

   /****************************************/
   /****************************************/

   std::string StringToUpperCase(const std::string& str_string) {
      char* buf = new char[str_string.length()];
      str_string.copy(buf, str_string.length());

      for(unsigned int i = 0; i < str_string.length(); ++i)
         buf[i] = toupper(buf[i]);

      std::string r(buf, str_string.length());

      delete[] buf;

      return r;
   }

   /****************************************/
   /****************************************/

   std::string StringToLowerCase(const std::string& str_string) {
      char* buf = new char[str_string.length()];
      str_string.copy(buf, str_string.length());

      for(unsigned int i = 0; i < str_string.length(); ++i)
         buf[i] = tolower(buf[i]);

      std::string r(buf, str_string.length());

      delete[] buf;

      return r;
   }

   /****************************************/
   /****************************************/

   void Replace(std::string& str_buffer,
                const std::string& str_original,
                const std::string& str_new) {
      /* Start from the beginning of the string */
      size_t unPos = 0;
      do {
         /* Look for the substring to replace */
         unPos = str_buffer.find(str_original, unPos);
         /* Has it been found? */
         if(unPos != std::string::npos) {
            /* Yes, it has been found */
            /* Replace the substring with the new one */
            str_buffer.replace(unPos, str_original.length(), str_new);
            /* Update unPos so that it point past the end of the replaced portion */
            unPos += str_new.length();
            /* Make sure unPos does not exceed the string length */
            if(unPos >= str_buffer.length()) {
               unPos = std::string::npos;
            }
         }
         /* Continue until the original string has been found */
      } while(unPos != std::string::npos);
   }
   
   /****************************************/
   /****************************************/

   bool MatchPattern(const std::string& str_input,
                     const std::string& str_pattern) {
      /* Code taken from
         http://www.opengroup.org/onlinepubs/000095399/functions/regexec.html
       */
      UInt32 nStatus;
      regex_t tRegExp;
      if(::regcomp(&tRegExp, str_pattern.c_str(), REG_EXTENDED | REG_NOSUB) != 0) {
         return false;
      }
      nStatus = ::regexec(&tRegExp, str_input.c_str(), 0, NULL, 0);
      ::regfree(&tRegExp);
      if (nStatus != 0) {
         return false;
      }
      return true;
   }
   
   /****************************************/
   /****************************************/

   std::string& ExpandEnvVariables(std::string& str_buffer) {
      size_t unStart = 0, unEnd;
      std::string strVarName;
      char* pchVarValue;
      bool bDone = false;
      do {
         /* Look for the $ character */
         unStart = str_buffer.find_first_of('$');
         /* Has it been found, and is it not the last character? */
         if(unStart != std::string::npos &&
            unStart+1 < str_buffer.length()) {
            /* Yes, it has been found */
            /* Look for the first non-alphanumeric character */
            unEnd = str_buffer.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_", unStart+1);
            /* Has it been found? */
            if(unEnd != std::string::npos) {
               /* Yes, it has been found */
               strVarName = str_buffer.substr(unStart+1, unEnd-unStart-1);
            }
            else {
               /* No, it has not been found */
               strVarName = str_buffer.substr(unStart+1, str_buffer.length()-unStart-1);
            }
            /* Get the variable value */
            pchVarValue = ::getenv(strVarName.c_str());
            /* Was the value found? */
            if(pchVarValue != NULL) {
               /* Yes, it was */
               /* Replace the variable name with its value */
               str_buffer.replace(unStart, strVarName.length()+1, pchVarValue);
            }
            else {
               /* Erase the variable name from the string */
               str_buffer.erase(unStart, strVarName.length()+1);
            }
            /* Reset the start pointer to prepare the next search */
            unStart = 0;
         }
         else {
            /* No further $ chars found or end of string reached */
            bDone = true;
         }
      } while(!bDone);
      return str_buffer;
   }

   /****************************************/
   /****************************************/

}
