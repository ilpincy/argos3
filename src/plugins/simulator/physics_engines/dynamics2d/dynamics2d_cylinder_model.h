/**
 * @file <argos3/plugins/simulator/physics_engines/dynamics2d/dynamics2d_cylinder_model.h>
 *
 * @author Carlo Pinciroli - <cpinciro@ulb.ac.be>
 */

#ifndef DYNAMICS2D_CYLINDER_MODEL_H
#define DYNAMICS2D_CYLINDER_MODEL_H

namespace argos {
   class CDynamics2DGrippable;
   class CDynamics2DCylinderModel;
   class CPhysicsCylinderModel;
}

#include <argos3/plugins/simulator/physics_engines/dynamics2d/dynamics2d_stretchable_object_model.h>
#include <argos3/plugins/simulator/physics_engines/physics_cylinder_model.h>
#include <argos3/plugins/simulator/entities/cylinder_entity.h>

namespace argos {

   class CDynamics2DCylinderModel : public CDynamics2DStretchableObjectModel {

   public:

      CDynamics2DCylinderModel(CDynamics2DEngine& c_engine,
                               CCylinderEntity& c_entity);
      virtual ~CDynamics2DCylinderModel() {}

      void Resize(Real f_radius, Real f_height);
   };

}

#endif
