/**
 * @file <argos3/plugins/simulator/physics_engines/pointmass3d/pm3d_cylinder_model.h>
 *
 * @author Carlo Pinciroli - <cpinciro@ulb.ac.be>
 */

#ifndef PM3D_CYLINDER_H
#define PM3D_CYLINDER_H

namespace argos {
   class CPM3DEngine;
   class CPM3DCylinderModel;
   class CQuadRotorEntity;
}

#include <argos3/plugins/simulator/physics_engines/pointmass3d/pm3d_model.h>
#include <argos3/plugins/simulator/entities/quadrotor_entity.h>
#include <argos3/plugins/simulator/entities/cylinder_entity.h>

namespace argos {

   class CPM3DCylinderModel : public CPM3DModel {

   public:
      
      CPM3DCylinderModel(CPM3DEngine& c_engine,
                                CCylinderEntity& c_cylinder);
      virtual ~CPM3DCylinderModel() {}
      
      virtual void UpdateFromEntityStatus() {}
      virtual void Step() {}
      virtual void UpdateEntityStatus() {}

      virtual void CalculateBoundingBox();

      virtual bool CheckIntersectionWithRay(Real& f_t_on_ray,
                                            const CRay3& c_ray) const;

   private:

      CCylinderEntity& m_cCylinderEntity;
   };

}

#endif
