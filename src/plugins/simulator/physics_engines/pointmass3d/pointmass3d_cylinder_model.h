/**
 * @file <argos3/plugins/simulator/physics_engines/pointmass3d/pointmass3d_cylinder_model.h>
 *
 * @author Carlo Pinciroli - <cpinciro@ulb.ac.be>
 */

#ifndef POINTMASS3D_CYLINDER_H
#define POINTMASS3D_CYLINDER_H

namespace argos {
   class CPointMass3DEngine;
   class CPointMass3DCylinderModel;
   class CQuadRotorEntity;
}

#include <argos3/plugins/simulator/physics_engines/pointmass3d/pointmass3d_model.h>
#include <argos3/plugins/simulator/entities/quadrotor_entity.h>
#include <argos3/plugins/simulator/entities/cylinder_entity.h>

namespace argos {

   class CPointMass3DCylinderModel : public CPointMass3DModel {

   public:
      
      CPointMass3DCylinderModel(CPointMass3DEngine& c_engine,
                                CCylinderEntity& c_cylinder);
      virtual ~CPointMass3DCylinderModel() {}
      
      virtual void UpdateFromEntityStatus() {}
      virtual void Step() {}

      virtual void CalculateBoundingBox();

      virtual bool CheckIntersectionWithRay(Real& f_t_on_ray,
                                            const CRay3& c_ray) const;

   private:

      CCylinderEntity& m_cCylinderEntity;
   };

}

#endif
