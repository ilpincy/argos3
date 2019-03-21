/**
 * @file <argos3/plugins/simulator/physics_engines/dynamics3d/dynamics3d_cylinder_model.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef DYNAMICS3D_CYLINDER_MODEL_H
#define DYNAMICS3D_CYLINDER_MODEL_H

namespace argos {
   class CDynamics3DSingleBodyObjectModel;
   class CDynamics3DCylinderModel;
   class CCylinderEntity;
}

#include <argos3/plugins/simulator/physics_engines/dynamics3d/dynamics3d_single_body_object_model.h>

namespace argos {

   class CDynamics3DCylinderModel : public CDynamics3DSingleBodyObjectModel {

   public:     

      CDynamics3DCylinderModel(CDynamics3DEngine& c_engine,
                          CCylinderEntity& c_cylinder);

      virtual ~CDynamics3DCylinderModel() {}

   private:

      std::shared_ptr<CBody> m_ptrBody;

   };
}

#endif
