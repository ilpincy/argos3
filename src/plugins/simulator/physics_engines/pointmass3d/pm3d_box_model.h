/**
 * @file <argos3/plugins/simulator/physics_engines/pointmass3d/pm3d_box_model.h>
 *
 * @author Carlo Pinciroli - <cpinciro@ulb.ac.be>
 */

#ifndef PM3D_BOX_H
#define PM3D_BOX_H

namespace argos {
   class CPM3DEngine;
   class CPM3DBoxModel;
   class CQuadRotorEntity;
}

#include <argos3/plugins/simulator/physics_engines/pointmass3d/pm3d_model.h>
#include <argos3/plugins/simulator/entities/quadrotor_entity.h>
#include <argos3/plugins/simulator/entities/box_entity.h>

namespace argos {

   class CPM3DBoxModel : public CPM3DModel {

   public:
      
      CPM3DBoxModel(CPM3DEngine& c_engine,
                           CBoxEntity& c_box);
      virtual ~CPM3DBoxModel() {}
      
      virtual void UpdateFromEntityStatus() {}
      virtual void Step() {}
      virtual void UpdateEntityStatus() {}

      virtual void CalculateBoundingBox();

      virtual bool CheckIntersectionWithRay(Real& f_t_on_ray,
                                            const CRay3& c_ray) const;

   private:

      CBoxEntity& m_cBoxEntity;
   };

}

#endif
