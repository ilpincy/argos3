/**
 * @file <argos3/plugins/simulator/physics_engines/dynamics2d/dynamics2d_box_entity.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef DYNAMICS2D_BOX_ENTITY_H
#define DYNAMICS2D_BOX_ENTITY_H

#include <argos3/plugins/simulator/physics_engines/dynamics2d/dynamics2d_entity.h>
#include <argos3/plugins/simulator/entities/box_entity.h>

namespace argos {

   class CDynamics2DBoxEntity : public CDynamics2DEntity {

   public:

      CDynamics2DBoxEntity(CDynamics2DEngine& c_engine,
                           CBoxEntity& c_entity);
      virtual ~CDynamics2DBoxEntity();

      virtual bool CheckIntersectionWithRay(Real& f_t_on_ray,
                                            const CRay3& c_ray) const;

      virtual bool MoveTo(const CVector3& c_position,
                          const CQuaternion& c_orientation,
                          bool b_check_only = false);

      virtual void Reset();

      virtual void UpdateEntityStatus();
      virtual void UpdateFromEntityStatus() {}

   private:

      CBoxEntity&   m_cBoxEntity;
      Real          m_fHalfHeight;
      cpFloat       m_fMass;
      cpShape*      m_ptShape;
      cpBody*       m_ptBody;
      cpConstraint* m_ptLinearFriction;
      cpConstraint* m_ptAngularFriction;

   };

}

#endif
