/**
 * @file <argos3/plugins/simulator/entities/wheeled_entity.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "wheeled_entity.h"
#include <argos3/core/simulator/space/space.h>

namespace argos {

   /****************************************/
   /****************************************/

   CWheeledEntity::CWheeledEntity(CComposableEntity* pc_parent,
                                  UInt32 un_num_wheels) :
      CEntity(pc_parent),
      m_unNumWheels(un_num_wheels) {
      m_pcWheelPositions = new CVector3[m_unNumWheels];
      ::memset(m_pcWheelPositions, 0, m_unNumWheels * sizeof(CVector3));
      m_pfWheelSpeeds = new Real[m_unNumWheels];
      ::memset(m_pfWheelSpeeds, 0, m_unNumWheels * sizeof(Real));
   }

   /****************************************/
   /****************************************/

   CWheeledEntity::CWheeledEntity(CComposableEntity* pc_parent,
                                  const std::string& str_id,
                                  UInt32 un_num_wheels) :
      CEntity(pc_parent, str_id),
      m_unNumWheels(un_num_wheels) {
      m_pcWheelPositions = new CVector3[m_unNumWheels];
      ::memset(m_pcWheelPositions, 0, m_unNumWheels * sizeof(CVector3));
      m_pfWheelSpeeds = new Real[m_unNumWheels];
      ::memset(m_pfWheelSpeeds, 0, m_unNumWheels * sizeof(Real));
   }

   /****************************************/
   /****************************************/

   CWheeledEntity::~CWheeledEntity() {
      delete[] m_pcWheelPositions;
      delete[] m_pfWheelSpeeds;
   }

   /****************************************/
   /****************************************/

   void CWheeledEntity::Reset() {
      ::memset(m_pfWheelSpeeds, 0, m_unNumWheels * sizeof(Real));
   }

   /****************************************/
   /****************************************/

   void CWheeledEntity::GetSpeed(Real* pf_speeds) {
      ::memcpy(pf_speeds, m_pfWheelSpeeds, m_unNumWheels * sizeof(Real));
   }

   /****************************************/
   /****************************************/

   void CWheeledEntity::SetSpeed(Real* pf_speeds) {
      ::memcpy(m_pfWheelSpeeds, pf_speeds, m_unNumWheels * sizeof(Real));
   }

   /****************************************/
   /****************************************/

   REGISTER_STANDARD_SPACE_OPERATIONS_ON_ENTITY(CWheeledEntity);

   /****************************************/
   /****************************************/

}
