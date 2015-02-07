/**
 * @file <argos3/plugins/simulator/physics_engines/dynamics2d/dynamics2d_box_model.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef DYNAMICS2D_BOX_MODEL_H
#define DYNAMICS2D_BOX_MODEL_H

namespace argos {
   class CDynamics2DStretchableObjectModel;
   class CDynamics2DBoxModel;
}

#include <argos3/plugins/simulator/physics_engines/dynamics2d/dynamics2d_stretchable_object_model.h>
#include <argos3/plugins/simulator/entities/box_entity.h>

namespace argos {

   class CDynamics2DBoxModel : public CDynamics2DStretchableObjectModel {

   public:

      CDynamics2DBoxModel(CDynamics2DEngine& c_engine,
                          CBoxEntity& c_entity);
      virtual ~CDynamics2DBoxModel() {}
   };

}

#endif
