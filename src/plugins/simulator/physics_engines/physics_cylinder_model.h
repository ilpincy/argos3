/**
 * @file <argos3/plugins/simulator/physics_engines/physics_cylinder_model.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef PHYSICS_CYLINDER_MODEL_H
#define PHYSICS_CYLINDER_MODEL_H

namespace argos {

   class CPhysicsCylinderModel {

   public:

      virtual void Resize(Real f_radius, Real f_height) = 0;

   };

}

#endif
