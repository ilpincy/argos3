#include "loop_functions.h"
#include <argos3/plugins/robots/builderbot/simulator/builderbot_entity.h>
#include <argos3/plugins/robots/block/simulator/block_entity.h>
#include <argos3/core/simulator/entity/embodied_entity.h>

namespace argos {

   /****************************************/
   /****************************************/

   bool CTestLoopFunctions::IsExperimentFinished() {
      /* wait five ticks before evaluating the test */
      if(GetSpace().GetSimulationClock() < 100) {
         return false;
      }
      else {
         CBlockEntity* pcBlock = nullptr;
         try {
            pcBlock = dynamic_cast<CBlockEntity*>(&GetSpace().GetEntity("block"));
         }
         catch(CARGoSException& ex) {
            THROW_ARGOSEXCEPTION_NESTED("Entity was not added to the simulator", ex);
         }
         if(pcBlock == nullptr) {
            THROW_ARGOSEXCEPTION("Entity with identifier \"block\" was not a block");
         }
         const CVector3& cBlockPosition = pcBlock->GetEmbodiedEntity().GetOriginAnchor().Position;
         if(Distance(cBlockPosition, TARGET_POSITION) > TARGET_THRESHOLD) {
            THROW_ARGOSEXCEPTION("BuilderBot did not pick up block");
         }
         return true;
      }
   }

   /****************************************/
   /****************************************/

   const CVector3 CTestLoopFunctions::TARGET_POSITION(0.15, 0.0, 0.1375);
   const Real CTestLoopFunctions::TARGET_THRESHOLD(0.005);

   /****************************************/
   /****************************************/

   REGISTER_LOOP_FUNCTIONS(CTestLoopFunctions, "test_loop_functions");

}
