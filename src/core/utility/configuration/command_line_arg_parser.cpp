/**
 * @file <argos3/core/utility/configuration/command_line_arg_parser.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "command_line_arg_parser.h"
#include <cstring>

namespace argos {

   /****************************************/
   /****************************************/

   CCommandLineArgParser::CCommandLineArgParser() :
      m_nCurrentArgument(0) {
   }

   /****************************************/
   /****************************************/

   CCommandLineArgParser::~CCommandLineArgParser() {
      while(! m_vecArguments.empty()) {
         delete m_vecArguments.back();
         m_vecArguments.pop_back();
      }
   }

   /****************************************/
   /****************************************/

   void CCommandLineArgParser::PrintUsage(CARGoSLog& c_log) {
      for(size_t i = 0; i < m_vecArguments.size(); ++i) {
         c_log << "-" << m_vecArguments[i]->ShortOption << "|"
               << "--" << m_vecArguments[i]->LongOption;
         if(!m_vecArguments[i]->IsFlag) {
            c_log << " <value>";
         }
         c_log << "\t"
               << m_vecArguments[i]->Description
               << std::endl;
      }
   }

   /****************************************/
   /****************************************/

   void CCommandLineArgParser::Parse(SInt32 n_argc,
                                     char** ppch_argv) {
      /* Used to store information about each argument to parse */
      size_t unArgLength;
      char* pchCurrentArg;
      /* Start parsing from the beginning */
      m_nCurrentArgument = 1;
      /* Go through all the passed arguments */
      while(m_nCurrentArgument < n_argc) {
         /*
          * Parsing does as follows:
          * If the first characters is a - followed by other charactares, then it's an option
          *   If the second character is a - too, it's a long one
          *   Otherwise it's a (set of) short option(s)
          * If the first character is not a -, or the string is just "-", error
          */
         pchCurrentArg = ppch_argv[m_nCurrentArgument];
         unArgLength = ::strlen(pchCurrentArg);
         if(ppch_argv[m_nCurrentArgument][0] != '-' ||
            unArgLength == 1) {
            THROW_ARGOSEXCEPTION("Unrecognized option \"" << pchCurrentArg << "\".");
         }
         else {
            /* The first character is a - and more characters are to come */
            if(pchCurrentArg[1] == '-') {
               /* Also the second char is a - */
               if(unArgLength > 2) {
                  /* We have a long option */
                  ParseLongOption(n_argc, ppch_argv);
               }
               else {
                  /* It's just a --, which we don't support */
                  THROW_ARGOSEXCEPTION("Unrecognized option \"" << pchCurrentArg << "\".");
               }
            }
            else {
               /* We have a short option like -x, or a set of them like -xyz */
               if(unArgLength == 2) {
                  /* Single short option: -x */
                  ParseShortOption(n_argc, ppch_argv);
               }
               else {
                  /* Set of short options: -xyz */
                  ParseShortOptions(n_argc, ppch_argv);
               }
            }
         }
         /* Parse next option */
         ++m_nCurrentArgument;
      }
   }

   /****************************************/
   /****************************************/

   void CCommandLineArgParser::ParseLongOption(SInt32 n_argc,
                                               char** ppch_argv) {
      /* Strip the initial -- from the option */
      std::string strArg(ppch_argv[m_nCurrentArgument]+2);
      try {
         /* Search for the option in the vector */
         for(size_t i = 0; i < m_vecArguments.size(); ++i) {
            if(m_vecArguments[i]->LongOption == strArg) {
               /* The option has been found */
               if(m_vecArguments[i]->IsFlag) {
                  /* It's a flag, pass "true" to set the boolean to true */
                  m_vecArguments[i]->Parse("true");
               }
               else {
                  /* It's an argument, use what follows as parameter */
                  ++m_nCurrentArgument;
                  if(m_nCurrentArgument == n_argc) {
                     THROW_ARGOSEXCEPTION("Missing argument for option \"--" << strArg << "\".");
                  }
                  m_vecArguments[i]->Parse(ppch_argv[m_nCurrentArgument]);
               }
               /* No need to continue searching */
               return;
            }
         }
      }
      catch(CARGoSException& ex) {
         /* Something went wrong with the parsing */
         THROW_ARGOSEXCEPTION_NESTED("Error parsing option \"--" << strArg << "\".", ex);
      }
      /* If you get here, it's because the option hasn't been found */
      THROW_ARGOSEXCEPTION("Unrecognized option \"--" << strArg << "\".");
   }

   /****************************************/
   /****************************************/

   void CCommandLineArgParser::ParseShortOption(SInt32 n_argc,
                                                char** ppch_argv) {
      /* Strip the initial - from the option */
      char strArg(ppch_argv[m_nCurrentArgument][1]);
      try {
         /* Search for the option in the vector */
         for(size_t i = 0; i < m_vecArguments.size(); ++i) {
            if(m_vecArguments[i]->ShortOption == strArg) {
               /* The option has been found */
               if(m_vecArguments[i]->IsFlag) {
                  /* It's a flag, pass "true" to set the boolean to true */
                  m_vecArguments[i]->Parse("true");
               }
               else {
                  /* It's an argument, use what follows as parameter */
                  ++m_nCurrentArgument;
                  if(m_nCurrentArgument == n_argc) {
                     THROW_ARGOSEXCEPTION("Missing argument for option \"-" << strArg << "\".");
                  }
                  m_vecArguments[i]->Parse(ppch_argv[m_nCurrentArgument]);
               }
               /* No need to continue searching */
               return;
            }
         }
      }
      catch(CARGoSException& ex) {
         /* Something went wrong with the parsing */
         THROW_ARGOSEXCEPTION_NESTED("Error parsing option \"-" << strArg << "\".", ex);
      }
      /* If you get here, it's because the option hasn't been found */
      THROW_ARGOSEXCEPTION("Unrecognized option \"-" << strArg << "\".");
   }

   /****************************************/
   /****************************************/

   void CCommandLineArgParser::ParseShortOptions(SInt32 n_argc,
                                                 char** ppch_argv) {
      /* Strip the initial - from the option */
      std::string strArg(ppch_argv[m_nCurrentArgument]+1);
      /* Go through all options */
      for(size_t o = 0; o < strArg.length(); ++o) {
         /* Search for the option in the vector */
         size_t i = 0;
         bool bFound = false;
         try {
            while(i < m_vecArguments.size() && !bFound) {
               if(m_vecArguments[i]->ShortOption == strArg[o]) {
                  /* The option has been found */
                  if(m_vecArguments[i]->IsFlag) {
                     /* It's a flag, pass "true" to set the boolean to true */
                     m_vecArguments[i]->Parse("true");
                  }
                  else {
                     /* It's an argument, only the last short option is allowed to have one */
                     if(o < strArg.length()-1) {
                        THROW_ARGOSEXCEPTION("Option \"-" << strArg[o] << "\" requires an argument.");
                     }
                     else {
                        ++m_nCurrentArgument;
                        m_vecArguments[i]->Parse(ppch_argv[m_nCurrentArgument]);
                     }
                  }
                  /* No need to continue searching */
                  bFound = true;
               }
               else {
                  /* Try next option */
                  ++i;
               }
            }
         }
         catch(CARGoSException& ex) {
            /* Something went wrong with the parsing */
            THROW_ARGOSEXCEPTION_NESTED("Error parsing option \"-" << strArg << "\".", ex);
         }
         if(!bFound) {
            /* The option hasn't been found */
            THROW_ARGOSEXCEPTION("Unrecognized option \"-" << strArg[o] << "\".");
         }
      }
   }

   /****************************************/
   /****************************************/
   
}
