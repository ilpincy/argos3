#include "pm3d_dynamicmodel.h"
#include <argos3/core/simulator/entity/embodied_entity.h>

namespace argos {

   /****************************************/
   /****************************************/

   CPM3DDynamicModel::CPM3DDynamicModel(CPM3DEngine& c_engine,
                                        CEmbodiedEntity& c_entity) :
      CPM3DKinematicModel(c_engine,
                          c_entity) {}
   
   /****************************************/
   /****************************************/

   void CPM3DDynamicModel::Step() {
      /* Use old settings to step */
      CPM3DKinematicModel::Step();
      /* Calculate the new angular velocity */
      m_cAngularVelocity += m_cTorque * GetEngine().GetSimulationClockTick();
      /* Calculate the new linear velocity */
      m_cLinearVelocity += m_cAcceleration * GetEngine().GetSimulationClockTick();
   }

   /****************************************/
   /****************************************/

}
