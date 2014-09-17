/**
 * @file <argos3/core/simulator/loop_functions.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "loop_functions.h"
#include <argos3/core/utility/plugins/factory.h>
#include <argos3/core/simulator/entity/positional_entity.h>
#include <argos3/core/simulator/entity/embodied_entity.h>

namespace argos {

   /****************************************/
   /****************************************/
   
   void CLoopFunctions::MoveEntity(CPositionalEntity& c_entity,
                                   const CVector3& c_position,
                                   const CQuaternion& c_orientation) {
      c_entity.MoveTo(c_position, c_orientation);
   }

   /****************************************/
   /****************************************/

   bool CLoopFunctions::MoveEntity(CEmbodiedEntity& c_entity,
                                   const CVector3& c_position,
                                   const CQuaternion& c_orientation,
                                   bool b_check_only) {
      return c_entity.MoveTo(c_position, c_orientation, b_check_only);
   }

   /****************************************/
   /****************************************/

   void CLoopFunctions::AddEntity(CEntity& c_entity) {
      CallEntityOperation<CSpaceOperationAddEntity, CSpace, void>(m_cSpace, c_entity);
   }
   
   /****************************************/
   /****************************************/

   void CLoopFunctions::RemoveEntity(const std::string& str_entity_id) {
      CEntity& entity = m_cSpace.GetEntity(str_entity_id);
      RemoveEntity(entity);
   }
   
   /****************************************/
   /****************************************/
   
   void CLoopFunctions::RemoveEntity(CEntity& c_entity) {
      CallEntityOperation<CSpaceOperationRemoveEntity, CSpace, void>(m_cSpace, c_entity);
   }

   /****************************************/
   /****************************************/

}
