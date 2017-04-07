#include "pm3d_kinematicmodel.h"
#include <argos3/core/simulator/entity/embodied_entity.h>

namespace argos {

   /****************************************/
   /****************************************/

   CPM3DKinematicModel::CPM3DKinematicModel(CPM3DEngine& c_engine,
                                            CEmbodiedEntity& c_entity) :
      CPM3DModel(c_engine,
                 c_entity) {}
   
   /****************************************/
   /****************************************/

   void CPM3DKinematicModel::Step() {
      /*
       * Calculate new rotation
       */
      CVector3 cAVN = m_cAngularVelocity;
      cAVN.Normalize();
      m_cOrientation *= CQuaternion(CRadians(m_cAngularVelocity.Length() *
                                             GetEngine().GetSimulationClockTick()),
                                    cAVN);
      /* Calculate new position */
      m_cPosition += m_cLinearVelocity * GetEngine().GetSimulationClockTick();
   }

   /****************************************/
   /****************************************/

}
