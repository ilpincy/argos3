#include "loop_functions.h"

namespace argos {

   /****************************************/
   /****************************************/

   bool CTestLoopFunctions::IsExperimentFinished() {
      GetSpace().GetEntity("fb");
      return true;
   }

   /****************************************/
   /****************************************/

   REGISTER_LOOP_FUNCTIONS(CTestLoopFunctions, "test_loop_functions");

}
