/**
 * @file <argos3/plugins/robots/foot-bot/simulator/pointmass3d_cylinder_model.h>
 *
 * @author Carlo Pinciroli - <cpinciro@ulb.ac.be>
 */

#include "pointmass3d_cylinder_model.h"
#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/core/utility/math/cylinder.h>
#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/plugins/simulator/physics_engines/pointmass3d/pointmass3d_engine.h>

namespace argos {

   CPointMass3DCylinderModel::CPointMass3DCylinderModel(CPointMass3DEngine& c_engine,
                                                    CCylinderEntity& c_cylinder) :
      CPointMass3DModel(c_engine, c_cylinder.GetEmbodiedEntity()),
      m_cCylinderEntity(c_cylinder) {
      Reset();
   }

   /****************************************/
   /****************************************/

   bool CPointMass3DCylinderModel::MoveTo(const CVector3& c_position,
                                        const CQuaternion& c_orientation,
                                        bool b_check_only) {
      if(! b_check_only) {
         m_cCylinderEntity.GetEmbodiedEntity().SetPosition(c_position);
         m_cCylinderEntity.GetEmbodiedEntity().SetOrientation(c_orientation);
         CalculateBoundingBox();
      }
      return true;
   }

   /****************************************/
   /****************************************/

   void CPointMass3DCylinderModel::Reset() {
      CalculateBoundingBox();
   }

   /****************************************/
   /****************************************/

   void CPointMass3DCylinderModel::CalculateBoundingBox() {
      GetBoundingBox().MinCorner.Set(
         m_cEmbodiedEntity.GetPosition().GetX() - m_cCylinderEntity.GetRadius(),
         m_cEmbodiedEntity.GetPosition().GetY() - m_cCylinderEntity.GetRadius(),
         m_cEmbodiedEntity.GetPosition().GetZ());
      GetBoundingBox().MaxCorner.Set(
         m_cEmbodiedEntity.GetPosition().GetX() + m_cCylinderEntity.GetRadius(),
         m_cEmbodiedEntity.GetPosition().GetY() + m_cCylinderEntity.GetRadius(),
         m_cEmbodiedEntity.GetPosition().GetZ() + m_cCylinderEntity.GetHeight());
   }

   /****************************************/
   /****************************************/

   bool CPointMass3DCylinderModel::CheckIntersectionWithRay(Real& f_t_on_ray,
                                                          const CRay3& c_ray) const {
      CCylinder m_cShape(m_cCylinderEntity.GetRadius(),
                         m_cCylinderEntity.GetHeight(),
                         m_cEmbodiedEntity.GetPosition(),
                         CVector3::Z);
      return m_cShape.Intersects(f_t_on_ray, c_ray);
   }
   
   /****************************************/
   /****************************************/

   REGISTER_STANDARD_POINTMASS3D_OPERATIONS_ON_ENTITY(CCylinderEntity, CPointMass3DCylinderModel);

   /****************************************/
   /****************************************/

}
