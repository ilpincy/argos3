/**
 * @file <argos3/plugins/simulator/physics_engines/pointmass3d/pm3d_box_model.cpp>
 *
 * @author Carlo Pinciroli - <cpinciro@ulb.ac.be>
 */

#include "pm3d_box_model.h"
#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/core/utility/math/box.h>
#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/plugins/simulator/physics_engines/pointmass3d/pm3d_engine.h>

namespace argos {

   CPM3DBoxModel::CPM3DBoxModel(CPM3DEngine& c_engine,
                                              CBoxEntity& c_box) :
      CPM3DModel(c_engine, c_box.GetEmbodiedEntity()),
      m_cBoxEntity(c_box) {}

   /****************************************/
   /****************************************/

   void CPM3DBoxModel::CalculateBoundingBox() {
      CVector3 cHalfSize = 0.5f * m_cBoxEntity.GetSize();
      GetBoundingBox().MinCorner.Set(
         GetEmbodiedEntity().GetOriginAnchor().Position.GetX() - cHalfSize.GetX(),
         GetEmbodiedEntity().GetOriginAnchor().Position.GetY() - cHalfSize.GetY(),
         GetEmbodiedEntity().GetOriginAnchor().Position.GetZ());
      GetBoundingBox().MaxCorner.Set(
         GetEmbodiedEntity().GetOriginAnchor().Position.GetX() + cHalfSize.GetX(),
         GetEmbodiedEntity().GetOriginAnchor().Position.GetY() + cHalfSize.GetY(),
         GetEmbodiedEntity().GetOriginAnchor().Position.GetZ() + m_cBoxEntity.GetSize().GetZ());
   }

   /****************************************/
   /****************************************/

   bool CPM3DBoxModel::CheckIntersectionWithRay(Real& f_t_on_ray,
                                                       const CRay3& c_ray) const {
      CBox m_cShape(m_cBoxEntity.GetSize(),
                    GetEmbodiedEntity().GetOriginAnchor().Position,
                    CVector3::Z);
      return m_cShape.Intersects(f_t_on_ray, c_ray);
   }
   
   /****************************************/
   /****************************************/

   REGISTER_STANDARD_PM3D_OPERATIONS_ON_ENTITY(CBoxEntity, CPM3DBoxModel);

   /****************************************/
   /****************************************/

}
