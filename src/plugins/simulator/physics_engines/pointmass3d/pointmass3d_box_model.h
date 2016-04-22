/**
 * @file <argos3/plugins/simulator/physics_engines/pointmass3d/pointmass3d_box_model.h>
 *
 * @author Carlo Pinciroli - <cpinciro@ulb.ac.be>
 */

#ifndef POINTMASS3D_BOX_H
#define POINTMASS3D_BOX_H

namespace argos {
   class CPointMass3DEngine;
   class CPointMass3DBoxModel;
   class CQuadRotorEntity;
}

#include <argos3/plugins/simulator/physics_engines/pointmass3d/pointmass3d_model.h>
#include <argos3/plugins/simulator/entities/quadrotor_entity.h>
#include <argos3/plugins/simulator/entities/box_entity.h>

namespace argos {

   class CPointMass3DBoxModel : public CPointMass3DModel {

   public:
      
      CPointMass3DBoxModel(CPointMass3DEngine& c_engine,
                           CBoxEntity& c_box);
      virtual ~CPointMass3DBoxModel() {}
      
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
