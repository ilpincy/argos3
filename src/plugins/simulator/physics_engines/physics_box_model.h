/**
 * @file <argos3/plugins/simulator/physics_engines/physics_box_model.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef PHYSICS_BOX_MODEL_H
#define PHYSICS_BOX_MODEL_H

namespace argos {

   class CPhysicsBoxModel {

   public:

      virtual void Resize(const CVector3& c_size) = 0;

   };

}

#endif
