#include "loop_functions.h"
#include <argos3/plugins/robots/drone/simulator/drone_entity.h>
#include <argos3/core/simulator/entity/embodied_entity.h>

namespace argos {

   /****************************************/
   /****************************************/

   bool CTestLoopFunctions::IsExperimentFinished() {
      if(GetSpace().GetSimulationClock() < 100) {
         return false;
      }
      else {
         CDroneEntity* pcDrone = nullptr;
         try {
            CEntity& cEntity = GetSpace().GetEntity("drone");
            pcDrone = dynamic_cast<CDroneEntity*>(&cEntity);
         }
         catch(CARGoSException& ex) {
            THROW_ARGOSEXCEPTION_NESTED("Robot was not added to the simulator", ex);
         }
         if(pcDrone == nullptr) {
            THROW_ARGOSEXCEPTION("Robot was not a drone");
         }
         const CVector3& cDronePosition = pcDrone->GetEmbodiedEntity().GetOriginAnchor().Position;
         if(Distance(cDronePosition, TARGET_POSITION) > THRESHOLD) {
            THROW_ARGOSEXCEPTION("Robot did not fly forwards to the target position");
         }
         return true;
      }
   }

   /****************************************/
   /****************************************/

   const CVector3 CTestLoopFunctions::TARGET_POSITION = CVector3(2.0, 0, 1.0);
   const Real CTestLoopFunctions::THRESHOLD = 0.05;

   /****************************************/
   /****************************************/

   REGISTER_LOOP_FUNCTIONS(CTestLoopFunctions, "test_loop_functions");

}
