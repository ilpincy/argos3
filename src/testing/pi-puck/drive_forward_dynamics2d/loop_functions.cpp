#include "loop_functions.h"
#include <argos3/plugins/robots/pi-puck/simulator/pipuck_entity.h>
#include <argos3/core/simulator/entity/embodied_entity.h>

namespace argos {

   /****************************************/
   /****************************************/

   bool CTestLoopFunctions::IsExperimentFinished() {
      if(GetSpace().GetSimulationClock() < 100) {
         return false;
      }
      else {
         CPiPuckEntity* pcPiPuck = nullptr;
         try {
            CEntity& cEntity = GetSpace().GetEntity("pipuck");
            pcPiPuck = dynamic_cast<CPiPuckEntity*>(&cEntity);
         }
         catch(CARGoSException& ex) {
            THROW_ARGOSEXCEPTION_NESTED("Robot was not added to the simulator", ex);
         }
         if(pcPiPuck == nullptr) {
            THROW_ARGOSEXCEPTION("Robot was not a Pi-Puck");
         }
         const CVector3& cPiPuckPosition = pcPiPuck->GetEmbodiedEntity().GetOriginAnchor().Position;
         if(Distance(cPiPuckPosition, TARGET_POSITION) > THRESHOLD) {
            THROW_ARGOSEXCEPTION("Robot did not drive forwards to the target position");
         }
         return true;
      }
   }

   /****************************************/
   /****************************************/

   const CVector3 CTestLoopFunctions::TARGET_POSITION = CVector3(0.5, 0, 0);
   const Real CTestLoopFunctions::THRESHOLD = 0.01;

   /****************************************/
   /****************************************/

   REGISTER_LOOP_FUNCTIONS(CTestLoopFunctions, "test_loop_functions");

}
