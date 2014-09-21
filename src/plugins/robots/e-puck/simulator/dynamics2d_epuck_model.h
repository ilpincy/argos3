/**
 * @file <argos3/plugins/robots/e-puck/simulator/dynamics2d_epuck_model.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef DYNAMICS2D_EPUCK_MODEL_H
#define DYNAMICS2D_EPUCK_MODEL_H

namespace argos {
   class CDynamics2DDifferentialSteeringControl;
   class CDynamics2DGripper;
   class CDynamics2DGrippable;
   class CDynamics2DEPuckModel;
}

#include <argos3/plugins/simulator/physics_engines/dynamics2d/dynamics2d_single_body_object_model.h>
#include <argos3/plugins/simulator/physics_engines/dynamics2d/dynamics2d_differentialsteering_control.h>
#include <argos3/plugins/robots/e-puck/simulator/epuck_entity.h>

namespace argos {

   class CDynamics2DEPuckModel : public CDynamics2DSingleBodyObjectModel {

   public:

      CDynamics2DEPuckModel(CDynamics2DEngine& c_engine,
                              CEPuckEntity& c_entity);
      virtual ~CDynamics2DEPuckModel();

      virtual void Reset();

      virtual void UpdateFromEntityStatus();
      
   private:

      CEPuckEntity& m_cEPuckEntity;
      CWheeledEntity& m_cWheeledEntity;

      CDynamics2DDifferentialSteeringControl m_cDiffSteering;

      const Real* m_fCurrentWheelVelocity;

   };

}

#endif
