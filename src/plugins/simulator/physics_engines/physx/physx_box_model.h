/**
 * @file <argos3/plugins/simulator/physics_engines/physx/physx_box_model.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef PHYSX_BOX_MODEL_H
#define PHYSX_BOX_MODEL_H

namespace argos {
   class CPhysXBoxModel;
}

#include <argos3/plugins/simulator/physics_engines/physx/physx_stretchable_object_model.h>
#include <argos3/plugins/simulator/entities/box_entity.h>

namespace argos {

   class CPhysXBoxModel : public CPhysXStretchableObjectModel<CBoxEntity> {

   public:

      CPhysXBoxModel(CPhysXEngine& c_engine,
                     CBoxEntity& c_entity);

      virtual ~CPhysXBoxModel() {}

   };

}

#endif
