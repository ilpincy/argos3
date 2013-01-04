/**
 * @file <argos3/core/utility/configuration/command_line_arg_parser.h>
 *
 * This file provides the definition of the class CCommandLineArgParser.
 * It is an easy-to-use command line parser.
 *
 * It supports a subset of the GNU-style syntax:
 * <pre>
 *   $ program --gnu-style-flag         # OK
 *   $ program --gnu-style-option value # OK
 *   $ program --gnu-style-option=value # ERROR
 *   $ program -x                       # OK
 *   $ program -xyz value               # OK
 * </pre>
 *
 * However, positional arguments are not supported:
 * <pre>
 *   $ program value       # ERROR
 *   $ program --arg value # OK
 *   $ program -a value    # OK
 * </pre>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef COMMAND_LINE_ARG_PARSER_H
#define COMMAND_LINE_ARG_PARSER_H

namespace argos {
   class CCommandLineArgParser;
}

#include <argos3/core/utility/datatypes/datatypes.h>
#include <argos3/core/utility/string_utilities.h>
#include <argos3/core/utility/logging/argos_log.h>
#include <vector>
#include <string>
#include <iostream>

namespace argos {

   /**
    * Easy-to-use command line argument parser.
    *
    * Example: you want to parse
    * <pre>
    *   $ program --flag --arg1 intvalue --arg2 stringvalue # GNU-style
    *   $ program -f -1 intvalue -2 stringvalue             # short style
    * </pre>
    *
    * The code to do it:
    *
    * <pre>
    *   int main(int argc, char** argv) {
    *     bool bIsFlag;         // will be true if the flag was specified, false otherwise
    *     UInt32 unIntValue;    // stores th parsed intvalue
    *     std::string strValue; // stores the parsed stringvalue
    *
    *     // Create the parser
    *     CCommandLineArgParser cCLAP;
    *     // Add the flag
    *     cCLAP.AddFlag(
    *       'f',
    *       "--flag",
    *       "This is a flag",
    *       bIsFlag);
    *     // Add the int argument
    *     cCLAP.AddArgument<UInt32>(
    *       '1',
    *       "--arg1",
    *       "This is an int value",
    *       unIntValue);
    *     // Add the string argument
    *     cCLAP.AddArgument<std::string>(
    *       '2',
    *       "--arg2",
    *       "This is a string value",
    *       strValue);
    *
    *     // Parse the command line!
    *     // In case of errors, a CARGoSException is thrown
    *     try {
    *       cCLAP.Parse(argc, argv);
    *     }
    *     catch(CARGoSException& ex) {
    *       LOGERR << "Error: " << ex.what() << std::endl;
    *     }
    * </pre>
    */
   class CCommandLineArgParser {

   public:

      /**
       * Class constructor.
       */
      CCommandLineArgParser();

      /**
       * Class destructor.
       */
      virtual ~CCommandLineArgParser();

      /**
       * Adds a flag to the parser.
       * @param ch_short_option A single character for the short option
       * @param str_long_option The long option
       * @param str_description The description shown by PrintUsage()
       * @param b_flag The boolean variable to associate to this entry
       */
      void AddFlag(char ch_short_option,
                   const std::string& str_long_option,
                   const std::string& str_description,
                   bool& b_flag) {
         m_vecArguments.push_back(
            new CArgument<bool>(
               ch_short_option,
               str_long_option,
               str_description,
               true,
               b_flag));
         b_flag = false;
      }

      /**
       * Adds an argument to the parser.
       * @param ch_short_option A single character for the short option
       * @param str_long_option The long option
       * @param str_description The description shown by PrintUsage()
       * @param t_buffer The buffer variable to associate to this entry
       */
      template <typename T> void AddArgument(char ch_short_option,
                                             const std::string& str_long_option,
                                             const std::string& str_description,
                                             T& t_buffer) {
         m_vecArguments.push_back(
            new CArgument<T>(
               ch_short_option,
               str_long_option,
               str_description,
               false,
               t_buffer));
      }

      /**
       * Prints the arguments on the log.
       * If you want a better layout, you need to extend this method.
       * @param c_log The destination log: LOG or LOGERR
       * @see CARGoSLog
       */
      virtual void PrintUsage(CARGoSLog& c_log);

      /**
       * Parses the arguments on the command line.
       * @param n_argc The number of arguments to parse
       * @param ppch_argv The string array containing the command line
       */
      virtual void Parse(SInt32 n_argc,
                         char** ppch_argv);

   private:

      void ParseLongOption (SInt32 n_argc,  char** ppch_argv);
      void ParseShortOption (SInt32 n_argc, char** ppch_argv);
      void ParseShortOptions(SInt32 n_argc, char** ppch_argv);

   private:

      class CAbstractArgument {

      public:

         virtual ~CAbstractArgument() {}
         virtual void Parse(const std::string& str_value) = 0;

      public:

         char        ShortOption;
         std::string LongOption;
         std::string Description;
         bool IsFlag;

      };

      template <typename T> class CArgument : public CAbstractArgument {

      public:
      
         CArgument(char ch_short_option,
                   const std::string& str_long_option,
                   const std::string& str_description,
                   bool b_is_flag,
                   T& t_buffer) :
            m_tBuffer(t_buffer) {
            ShortOption = ch_short_option;
            LongOption  = str_long_option;
            Description = str_description;
            IsFlag      = b_is_flag;
         }
      
         virtual void Parse(const std::string& str_value) {
            m_tBuffer = FromString<T>(str_value);
         }

         T& m_tBuffer;
      };

   private:

      std::vector<CAbstractArgument*> m_vecArguments;
      SInt32 m_nCurrentArgument;

   };

}

#endif
