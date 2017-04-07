/**
 * @file <argos3/plugins/simulator/physics_engines/pointmass3d/pm3d_cylinder_model.cpp>
 *
 * @author Carlo Pinciroli - <cpinciro@ulb.ac.be>
 */

#include "pm3d_cylinder_model.h"
#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/core/utility/math/cylinder.h>
#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/plugins/simulator/physics_engines/pointmass3d/pm3d_engine.h>

namespace argos {

   CPM3DCylinderModel::CPM3DCylinderModel(CPM3DEngine& c_engine,
                                                        CCylinderEntity& c_cylinder) :
      CPM3DModel(c_engine, c_cylinder.GetEmbodiedEntity()),
      m_cCylinderEntity(c_cylinder) {}

   /****************************************/
   /****************************************/

   void CPM3DCylinderModel::CalculateBoundingBox() {
      GetBoundingBox().MinCorner.Set(
         GetEmbodiedEntity().GetOriginAnchor().Position.GetX() - m_cCylinderEntity.GetRadius(),
         GetEmbodiedEntity().GetOriginAnchor().Position.GetY() - m_cCylinderEntity.GetRadius(),
         GetEmbodiedEntity().GetOriginAnchor().Position.GetZ());
      GetBoundingBox().MaxCorner.Set(
         GetEmbodiedEntity().GetOriginAnchor().Position.GetX() + m_cCylinderEntity.GetRadius(),
         GetEmbodiedEntity().GetOriginAnchor().Position.GetY() + m_cCylinderEntity.GetRadius(),
         GetEmbodiedEntity().GetOriginAnchor().Position.GetZ() + m_cCylinderEntity.GetHeight());
   }

   /****************************************/
   /****************************************/

   bool CPM3DCylinderModel::CheckIntersectionWithRay(Real& f_t_on_ray,
                                                            const CRay3& c_ray) const {
      CCylinder m_cShape(m_cCylinderEntity.GetRadius(),
                         m_cCylinderEntity.GetHeight(),
                         GetEmbodiedEntity().GetOriginAnchor().Position,
                         CVector3::Z);
      return m_cShape.Intersects(f_t_on_ray, c_ray);
   }
   
   /****************************************/
   /****************************************/

   REGISTER_STANDARD_PM3D_OPERATIONS_ON_ENTITY(CCylinderEntity, CPM3DCylinderModel);

   /****************************************/
   /****************************************/

}
