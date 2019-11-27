/**
 * @file <argos3/core/simulator/argos_command_line_arg_parser.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "argos_command_line_arg_parser.h"
#include <argos3/core/config.h>

namespace argos {

   /****************************************/
   /****************************************/

   CARGoSCommandLineArgParser::CARGoSCommandLineArgParser() :
      m_eAction(ACTION_UNKNOWN),
      m_pcInitLogStream(NULL),
      m_pcInitLogErrStream(NULL) {
      AddFlag(
         'h',
         "help",
         "display this usage information",
         m_bHelpWanted
         );
      AddFlag(
         'v',
         "version",
         "display the current version and release",
         m_bVersionWanted
         );
      AddFlag(
         'n',
         "no-color",
         "do not use colored output [OPTIONAL]",
         m_bNonColoredLog
         );
      AddArgument<std::string>(
         'c',
         "config-file",
         "the experiment XML configuration file",
         m_strExperimentConfigFile
         );
      AddArgument<std::string>(
         'q',
         "query",
         "query the available plugins",
         m_strQuery
         );
      AddArgument<std::string>(
         'l',
         "log-file",
         "output log to file [OPTIONAL]",
         m_strLogFileName
         );
      AddArgument<std::string>(
         'e',
         "logerr-file",
         "output logerr to file [OPTIONAL]",
         m_strLogErrFileName
         );
   }

   /****************************************/
   /****************************************/

   CARGoSCommandLineArgParser::~CARGoSCommandLineArgParser() {
      if(m_cLogFile.is_open()) {
         LOG.GetStream().rdbuf(m_pcInitLogStream);
         m_cLogFile.close();
      }
      if(m_cLogErrFile.is_open()) {
         LOGERR.GetStream().rdbuf(m_pcInitLogErrStream);
         m_cLogErrFile.close();
      }
   }

   /****************************************/
   /****************************************/

   void CARGoSCommandLineArgParser::Parse(SInt32 n_argc,
                                          char** ppch_argv) {
      CCommandLineArgParser::Parse(n_argc, ppch_argv);
      /* Configure LOG/LOGERR coloring */
      if(m_bNonColoredLog) {
         LOG.DisableColoredOutput();
         LOGERR.DisableColoredOutput();
      }

      /* Check whether LOG and LOGERR should go to files */
      if(m_strLogFileName != "") {
         LOG.DisableColoredOutput();
         m_cLogFile.open(m_strLogFileName.c_str(), std::ios::trunc | std::ios::out);
         if(m_cLogFile.fail()) {
            THROW_ARGOSEXCEPTION("Error opening file \"" << m_strLogFileName << "\"");
         }
         m_pcInitLogStream = LOG.GetStream().rdbuf();
         LOG.GetStream().rdbuf(m_cLogFile.rdbuf());
      }
      if(m_strLogErrFileName != "") {
         LOGERR.DisableColoredOutput();
         m_cLogErrFile.open(m_strLogErrFileName.c_str(), std::ios::trunc | std::ios::out);
         if(m_cLogErrFile.fail()) {
            THROW_ARGOSEXCEPTION("Error opening file \"" << m_strLogErrFileName << "\"");
         }
         m_pcInitLogErrStream = LOGERR.GetStream().rdbuf();
         LOGERR.GetStream().rdbuf(m_cLogErrFile.rdbuf());
      }

      /* Check that either -h, -v, -c or -q was passed (strictly one of them) */
      UInt32 nOptionsOn = 0;
      if(m_strExperimentConfigFile != "") ++nOptionsOn;
      if(m_strQuery != "") ++nOptionsOn;
      if(m_bHelpWanted) ++nOptionsOn;
      if(m_bVersionWanted) ++nOptionsOn;
      if(nOptionsOn == 0) {
         THROW_ARGOSEXCEPTION("No --help, --version, --config-file or --query options specified.");
      }
      if(nOptionsOn > 1) {
         THROW_ARGOSEXCEPTION("Options --help, --version, --config-file and --query are mutually exclusive.");
      }

      if(m_strExperimentConfigFile != "") {
         m_eAction = ACTION_RUN_EXPERIMENT;
      }

      if(m_strQuery != "") {
         m_eAction = ACTION_QUERY;
      }

      if(m_bHelpWanted) {
         m_eAction = ACTION_SHOW_HELP;
      }

      if(m_bVersionWanted) {
         m_eAction = ACTION_SHOW_VERSION;
      }

   }

   /****************************************/
   /****************************************/

   void CARGoSCommandLineArgParser::PrintUsage(CARGoSLog& c_log) {
      c_log << "Autonomous Robots Go Swarming (ARGoS) v" << ARGOS_VERSION << "-" << ARGOS_RELEASE << std::endl;
      c_log << "Released under the terms of the MIT license." << std::endl;
      c_log << std::endl;
      c_log << "Usage: argos3 [OPTIONS]" << std::endl;
      c_log << std::endl;
      c_log << "   -h       | --help                  display this usage information" << std::endl;
      c_log << "   -v       | --version               display ARGoS version and release" << std::endl;
      c_log << "   -c FILE  | --config-file FILE      the experiment XML configuration file" << std::endl;
      c_log << "   -q QUERY | --query QUERY           query the available plugins." << std::endl;
      c_log << "   -n       | --no-color              do not use colored output [OPTIONAL]" << std::endl;
      c_log << "   -l       | --log-file FILE         redirect LOG to FILE [OPTIONAL]" << std::endl;
      c_log << "   -e       | --logerr-file FILE      redirect LOGERR to FILE [OPTIONAL]" << std::endl << std::endl;
      c_log << "The options --config-file and --query are mutually exclusive. Either you use" << std::endl;
      c_log << "the first, and thus you run an experiment, or you use the second to query the" << std::endl;
      c_log << "plugins." << std::endl << std::endl;
      c_log << "EXAMPLES" << std::endl << std::endl;
      c_log << "To run an experiment, type:" << std::endl << std::endl;
      c_log << "   argos3 -c /path/to/myconfig.argos" << std::endl << std::endl;
      c_log << "To query the plugins, type:" << std::endl << std::endl;
      c_log << "   argos3 -q QUERY" << std::endl << std::endl;
      c_log << "where QUERY can have the following values:" << std::endl << std::endl;
      c_log << "   all                  print a list of all the available plugins" << std::endl;
      c_log << "   actuators            print a list of all the available actuators" << std::endl;
      c_log << "   sensors              print a list of all the available sensors" << std::endl;
      c_log << "   physics_engines      print a list of all the available physics engines" << std::endl;
      c_log << "   media                print a list of all the available media" << std::endl;
      c_log << "   visualizations       print a list of all the available visualizations" << std::endl;
      c_log << "   entities             print a list of all the available entities" << std::endl << std::endl;
      c_log << "Alternatively, QUERY can be the name of a specific plugin as returned by the" << std::endl;
      c_log << "above commands. In this case, you get a complete description of the matching" << std::endl;
      c_log << "plugins." << std::endl << std::endl;
   }

   /****************************************/
   /****************************************/

   void CARGoSCommandLineArgParser::PrintVersion() {
     LOG << "ARGOS_VERSION=" ARGOS_VERSION "-" ARGOS_RELEASE << std::endl;
     LOG << "ARGOS_INSTALL_PREFIX=" ARGOS_INSTALL_PREFIX << std::endl;
#ifdef ARGOS_USE_DOUBLE
     LOG << "ARGOS_USE_DOUBLE=ON" << std::endl;
#else
     LOG << "ARGOS_USE_DOUBLE=OFF" << std::endl;
#endif
#ifdef ARGOS_WITH_LUA
     LOG << "ARGOS_WITH_LUA=ON" << std::endl;
#else
     LOG << "ARGOS_WITH_LUA=OFF" << std::endl;
#endif
     LOG << "ARGOS_BUILD_FLAGS=" ARGOS_BUILD_FLAGS << std::endl;
   }

   /****************************************/
   /****************************************/

}
