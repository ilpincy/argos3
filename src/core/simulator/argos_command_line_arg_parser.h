/**
 * @file <argos3/core/simulator/argos_command_line_arg_parser.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef ARGOS_COMMAND_LINE_ARG_PARSER_H
#define ARGOS_COMMAND_LINE_ARG_PARSER_H

namespace argos {
   class CARGoSCommandLineArgParser;
}

#include <argos3/core/utility/configuration/command_line_arg_parser.h>
#include <fstream>

namespace argos {

   /**
    * The command line argument parser used by the main ARGoS executable.
    * If you plan to write your own <tt>main.cpp</tt> file, and want to
    * keep the same command line arguments, use this class.
    * @see CCommandLineArgParser
    */
   class CARGoSCommandLineArgParser : public CCommandLineArgParser {

   public:

      /**
       * The possible actions that can be done.
       */
      enum EAction {
         ACTION_UNKNOWN = 0,
         ACTION_SHOW_HELP,
         ACTION_SHOW_VERSION,
         ACTION_RUN_EXPERIMENT,
         ACTION_QUERY
      };

   public:

      /**
       * Class constructor.
       */
      CARGoSCommandLineArgParser();

      /**
       * Class destructor.
       */
      virtual ~CARGoSCommandLineArgParser();

      /**
       * Prints usage information to the wanted log.
       * @param c_log The output log (LOG or LOGERR)
       * @see CARGoSLogger
       * @see LOG
       * @see LOGERR
       */
      virtual void PrintUsage(CARGoSLog& c_log);

      /**
       * Prints the current ARGoS version and release.
       */
      virtual void PrintVersion();

      /**
       * Parses the command line.
       * In parsing is successful, it sets m_eAction to the action to perform,
       * along with the relevant operands.
       * @param n_argc The number of command line arguments.
       * @param ppch_argv An array of C-style strings containing the arguments.
       * @throws CARGoSException if a parse error occurs.
       * @see GetAction()
       * @see GetExperimentConfigFile()
       * @see GetQuery()
       * @see IsLogColored()
       * @see IsHelpWanted()
       */
      virtual void Parse(SInt32 n_argc,
                         char** ppch_argv);

      /**
       * Returns the action parsed by Parse().
       * @return The action parsed by Parse().
       * @see Parse()
       */
      inline EAction GetAction() {
         return m_eAction;
      }

      /**
       * Returns the experiment configuration file as parsed by Parse().
       * The returned value is meaningful only if GetAction() returns ACTION_RUN_EXPERIMENT.
       * @return The experiment configuration file as parsed by Parse().
       * @see Parse()
       */
      inline const std::string& GetExperimentConfigFile() {
         return m_strExperimentConfigFile;
      }

      /**
       * Returns the query on the plugins as parsed by Parse().
       * The returned value is meaningful only if GetAction() returns ACTION_QUERY.
       * @return The query on the plugins as parsed by Parse().
       * @see Parse()
       */
      inline const std::string& GetQuery() {
         return m_strQuery;
      }

      /**
       * Returns <tt>true</tt> if color is enabled for LOG and LOGERR.
       * @see Parse()
       * @see CARGoSLogger
       * @see LOG
       * @see LOGERR
       */
      inline bool IsLogColored() {
         return ! m_bNonColoredLog;
      }

      /**
       * Returns <tt>true</tt> if color if a help message must be displayed.
       * Help maybe be necessary either in case of parsing error, or because
       * the <tt>--help</t> argument was passed on the command line.
       * @see Parse()
       */
      inline bool IsHelpWanted() {
         return m_bHelpWanted;
      }

   private:

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
      bool m_bVersionWanted;

   };

}

#endif
