/**
 * @file core/simulator/argos_command_line_arg_parser.h
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef ARGOS_COMMAND_LINE_ARG_PARSER_H
#define ARGOS_COMMAND_LINE_ARG_PARSER_H

namespace argos {
   class CARGoSCommandLineArgParser;
   class CSimulator;
}

#include <argos3/core/utility/configuration/command_line_arg_parser.h>
#include <fstream>

namespace argos {

   class CARGoSCommandLineArgParser : public CCommandLineArgParser {

   public:

      enum EAction {
         ACTION_UNKNOWN = 0,
         ACTION_SHOW_HELP,
         ACTION_RUN_EXPERIMENT,
         ACTION_QUERY
      };

   public:

      CARGoSCommandLineArgParser(CSimulator& c_simulator);
      virtual ~CARGoSCommandLineArgParser();

      virtual void PrintUsage(CARGoSLog& c_log);

      virtual void Parse(SInt32 n_argc,
                         char** ppch_argv);

      inline EAction GetAction() {
         return m_eAction;
      }

      inline const std::string& GetExperimentConfigFile() {
         return m_strExperimentConfigFile;
      }

      inline const std::string& GetQuery() {
         return m_strQuery;
      }

      inline bool IsLogColored() {
         return ! m_bNonColoredLog;
      }

      inline bool IsHelpWanted() {
         return m_bHelpWanted;
      }

   private:

      CSimulator& m_cSimulator;
      EAction m_eAction;
      std::string m_strExperimentConfigFile;
      std::string m_strQuery;
      std::string m_strLogFileName;
      std::ofstream m_cLogFile;
      std::streambuf* m_pcInitLogStream;
      std::string m_strLogErrFileName;
      std::ofstream m_cLogErrFile;
      std::streambuf* m_pcInitLogErrStream;
      bool m_bNonColoredLog;
      bool m_bHelpWanted;

   };

}

#endif
