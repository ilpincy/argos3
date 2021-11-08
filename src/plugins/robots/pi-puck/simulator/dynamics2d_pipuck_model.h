/**
 * @file <argos3/plugins/robots/pi-puck/simulator/dynamics2d_pipuck_model.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef DYNAMICS2D_PIPUCK_MODEL_H
#define DYNAMICS2D_PIPUCK_MODEL_H

namespace argos {
   class CDynamics2DDifferentialSteeringControl;
   class CPiPuckDifferentialDriveEntity;
   class CDynamics2DPiPuckModel;
}

#include <argos3/plugins/simulator/physics_engines/dynamics2d/dynamics2d_single_body_object_model.h>
#include <argos3/plugins/simulator/physics_engines/dynamics2d/dynamics2d_differentialsteering_control.h>
#include <argos3/plugins/robots/pi-puck/simulator/pipuck_entity.h>

namespace argos {

   class CDynamics2DPiPuckModel : public CDynamics2DSingleBodyObjectModel {

   public:

      CDynamics2DPiPuckModel(CDynamics2DEngine& c_engine,
                             CPiPuckEntity& c_entity);
      
      virtual ~CDynamics2DPiPuckModel();

      virtual void Reset();

      virtual void UpdateFromEntityStatus();
      
   private:

      enum PIPUCK_WHEELS {
         PIPUCK_LEFT_WHEEL = 0,
         PIPUCK_RIGHT_WHEEL = 1
      };

   private:

      void UpdateAuxiliaryAnchor(SAnchor& s_anchor);
      
      CPiPuckEntity& m_cPiPuckEntity;
      CPiPuckDifferentialDriveEntity& m_cDifferentialDriveEntity;
      CDynamics2DDifferentialSteeringControl m_cDiffSteering;

   private:
      static const Real PIPUCK_RADIUS;
      static const Real PIPUCK_INTERWHEEL_DISTANCE;
      static const Real PIPUCK_HEIGHT;
      static const Real PIPUCK_MASS;
      static const Real PIPUCK_MAX_FORCE;
      static const Real PIPUCK_MAX_TORQUE;

   };

}

#endif
