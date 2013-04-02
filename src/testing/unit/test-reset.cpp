
/**
 * @file <argos3/testing/unit/test-reset.cpp>
 *
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 */
#include <argos3/core/simulator/simulator.h>
#include <argos3/core/utility/plugins/dynamic_loading.h>
#include <argos3/core/utility/string_utilities.h>

using namespace argos;

int main(int n_argc, char** ppch_argv) {
   if(n_argc != 3) {
      LOGERR << "Usage:" << std::endl;
      LOGERR << ppch_argv[0] << " <repeats> <xml>" << std::endl << std::endl;
      LOGERR.Flush();
      return 1;
   }
   /* Create a new instance of the simulator */
   CSimulator& cSimulator = CSimulator::GetInstance();
   try {
      UInt32 unRepeats = FromString<UInt32>(ppch_argv[1]);
      CDynamicLoading::LoadAllLibraries();
      cSimulator.SetExperimentFileName(ppch_argv[2]);
      cSimulator.LoadExperiment();
      for(UInt32 i = 0; i < unRepeats; ++i) {
         LOG << "[INFO] === Repetition #" << i+1 << " START" << std::endl;
         cSimulator.Execute();
         LOG << "[INFO] === Repetition #" << i+1 << " END" << std::endl;
         cSimulator.Reset();
         LOG << "[INFO] === Reset" << std::endl;
      }
      cSimulator.Destroy();
   }
   catch(std::exception& ex) {
      /* A fatal error occurred: dispose of data, print error and exit */
      LOGERR << ex.what() << std::endl;
      LOG.Flush();
      LOGERR.Flush();
      cSimulator.Destroy();
      LOG.Flush();
      LOGERR.Flush();
      return 1;
   }
   /* Everything's ok, exit */
   return 0;
}
