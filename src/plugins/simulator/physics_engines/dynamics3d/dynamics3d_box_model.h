/**
 * @file <argos3/plugins/simulator/physics_engines/dynamics3d/dynamics3d_box_model.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef DYNAMICS3D_BOX_MODEL_H
#define DYNAMICS3D_BOX_MODEL_H

namespace argos {
   class CDynamics3DSingleBodyObjectModel;
   class CDynamics3DBoxModel;
   class CBoxEntity;
}

#include <argos3/plugins/simulator/physics_engines/dynamics3d/dynamics3d_single_body_object_model.h>

namespace argos {

   class CDynamics3DBoxModel : public CDynamics3DSingleBodyObjectModel {

   public:     

      CDynamics3DBoxModel(CDynamics3DEngine& c_engine,
                          CBoxEntity& c_box);

      virtual ~CDynamics3DBoxModel() {}

   private:

      std::shared_ptr<CBody> m_ptrBody;

   };
}

#endif
