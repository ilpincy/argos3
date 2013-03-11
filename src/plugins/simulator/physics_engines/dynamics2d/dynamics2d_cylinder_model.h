/**
 * @file <argos3/plugins/simulator/physics_engines/dynamics2d/dynamics2d_cylinder_model.h>
 *
 * @author Carlo Pinciroli - <cpinciro@ulb.ac.be>
 */

#ifndef DYNAMICS2D_CYLINDER_MODEL_H
#define DYNAMICS2D_CYLINDER_MODEL_H

namespace argos {
   class CDynamics2DCylinderModel;
}

#include <argos3/plugins/simulator/physics_engines/dynamics2d/dynamics2d_model.h>
#include <argos3/plugins/simulator/entities/cylinder_entity.h>

namespace argos {

   class CDynamics2DCylinderModel : public CDynamics2DModel {

   public:

      CDynamics2DCylinderModel(CDynamics2DEngine& c_engine,
                                CCylinderEntity& c_entity);
      virtual ~CDynamics2DCylinderModel();
      
      virtual bool CheckIntersectionWithRay(Real& f_t_on_ray,
                                            const CRay3& c_ray) const;

      virtual bool MoveTo(const CVector3& c_position,
                          const CQuaternion& c_orientation,
                          bool b_check_only = false);

      virtual void Reset();

      virtual void CalculateBoundingBox();

      virtual void UpdateEntityStatus();
      virtual void UpdateFromEntityStatus() {}

      virtual bool IsCollidingWithSomething() const;

   private:

      CCylinderEntity& m_cCylinderEntity;
      cpFloat  m_fMass;
      cpShape* m_ptShape;
      cpBody*  m_ptBody;
      cpConstraint* m_ptLinearFriction;
      cpConstraint* m_ptAngularFriction;
      
   };

}

#endif
