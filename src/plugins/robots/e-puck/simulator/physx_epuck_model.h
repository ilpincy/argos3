/**
 * @file <argos3/plugins/ropucks/e-puck/simulator/physx_epuck_model.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef PHYSX_EPUCK_MODEL_H
#define PHYSX_EPUCK_MODEL_H

namespace argos {
   class CPhysXEngine;
   class CPhysXEPuckModel;
   class CEPuckEntity;
}

#include <argos3/plugins/simulator/physics_engines/physx/physx_single_body_object_model.h>

namespace argos {

   class CPhysXEPuckModel : public CPhysXSingleBodyObjectModel {

   public:

      CPhysXEPuckModel(CPhysXEngine& c_engine,
                       CEPuckEntity& c_entity);

      virtual void UpdateFromEntityStatus();

   private:

      CEPuckEntity& m_cEPuckEntity;
      const Real* m_fCurrentWheelVelocity;

   };

}

#endif
