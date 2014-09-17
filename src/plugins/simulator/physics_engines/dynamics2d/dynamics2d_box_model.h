/**
 * @file <argos3/plugins/simulator/physics_engines/dynamics2d/dynamics2d_box_model.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef DYNAMICS2D_BOX_MODEL_H
#define DYNAMICS2D_BOX_MODEL_H

namespace argos {
   class CDynamics2DBoxModel;
   class CDynamics2DGrippable;
}

#include <argos3/plugins/simulator/physics_engines/dynamics2d/dynamics2d_model.h>
#include <argos3/plugins/simulator/entities/box_entity.h>

namespace argos {

   class CDynamics2DBoxModel : public CDynamics2DModel {

   public:

      CDynamics2DBoxModel(CDynamics2DEngine& c_engine,
                          CBoxEntity& c_entity);
      virtual ~CDynamics2DBoxModel();

      virtual bool MoveTo(const CVector3& c_position,
                          const CQuaternion& c_orientation,
                          bool b_check_only = false);

      virtual void Reset();

      virtual void CalculateBoundingBox();

      virtual void UpdateEntityStatus();
      virtual void UpdateFromEntityStatus() {}

      virtual bool IsCollidingWithSomething() const;

      void UpdateOriginAnchor(SAnchor& s_anchor);

   private:

      bool                  m_bMovable;
      CBoxEntity&           m_cBoxEntity;
      CDynamics2DGrippable* m_pcGrippable;
      cpFloat               m_fMass;
      cpShape*              m_ptShape;
      cpBody*               m_ptBody;
      cpConstraint*         m_ptLinearFriction;
      cpConstraint*         m_ptAngularFriction;

   };

}

#endif
