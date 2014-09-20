/**
 * @file <argos3/plugins/robots/foot-bot/simulator/physx_footbot_model.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef PHYSX_FOOTBOT_MODEL_H
#define PHYSX_FOOTBOT_MODEL_H

namespace argos {
   class CPhysXEngine;
   class CPhysXFootBotModel;
   class CFootBotEntity;
}

#include <argos3/plugins/simulator/physics_engines/physx/physx_single_body_object_model.h>

namespace argos {

   class CPhysXFootBotModel : public CPhysXSingleBodyObjectModel {

   public:

      CPhysXFootBotModel(CPhysXEngine& c_engine,
                         CFootBotEntity& c_entity);

      virtual void UpdateFromEntityStatus();

   private:

      CFootBotEntity& m_cFootBotEntity;
   };

}

#endif
