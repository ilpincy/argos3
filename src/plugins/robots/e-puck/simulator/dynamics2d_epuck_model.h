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
}

#include <argos3/plugins/simulator/physics_engines/dynamics2d/dynamics2d_model.h>
#include <argos3/plugins/simulator/physics_engines/dynamics2d/dynamics2d_differentialsteering_control.h>
#include <argos3/plugins/robots/e-puck/simulator/epuck_entity.h>

namespace argos {

   class CDynamics2DEPuckModel : public CDynamics2DModel {

   public:

      CDynamics2DEPuckModel(CDynamics2DEngine& c_engine,
                              CEPuckEntity& c_entity);
      virtual ~CDynamics2DEPuckModel();
      
      virtual bool MoveTo(const CVector3& c_position,
                          const CQuaternion& c_orientation,
                          bool b_check_only = false);

      virtual void Reset();

      virtual void CalculateBoundingBox();

      virtual void UpdateFromEntityStatus();

      virtual bool IsCollidingWithSomething() const;

      void UpdateOriginAnchor(SAnchor& s_anchor);

   private:

      CEPuckEntity& m_cEPuckEntity;
      CWheeledEntity& m_cWheeledEntity;

      CDynamics2DDifferentialSteeringControl m_cDiffSteering;

      cpFloat  m_fMass;
      cpShape* m_ptBaseShape;
      cpBody*  m_ptActualBaseBody;

      const Real* m_fCurrentWheelVelocity;

   };

}

#endif
