/**
 * @file <argos3/plugins/simulator/physics_engines/physx/physx_cylinder_model.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef PHYSX_CYLINDER_MODEL_H
#define PHYSX_CYLINDER_MODEL_H

namespace argos {
   class CPhysXCylinderModel;
}

#include <argos3/plugins/simulator/physics_engines/physx/physx_stretchable_object_model.h>
#include <argos3/plugins/simulator/entities/cylinder_entity.h>

namespace argos {

   class CPhysXCylinderModel : public CPhysXStretchableObjectModel<CCylinderEntity> {

   public:

      CPhysXCylinderModel(CPhysXEngine& c_engine,
                          CCylinderEntity& c_entity);

      virtual ~CPhysXCylinderModel() {}
   };

}

#endif
