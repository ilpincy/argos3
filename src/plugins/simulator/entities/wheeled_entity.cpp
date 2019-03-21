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
                                  size_t un_num_wheels) :
      CEntity(pc_parent),
      m_unNumWheels(un_num_wheels) {
      m_pcWheelPositions = new CVector3[m_unNumWheels];
      m_pfWheelRadia = new Real[m_unNumWheels];
      ::memset(m_pfWheelRadia, 0, m_unNumWheels * sizeof(Real));
      m_pfWheelVelocities = new Real[m_unNumWheels];
      ::memset(m_pfWheelVelocities, 0, m_unNumWheels * sizeof(Real));
      Disable();
   }

   /****************************************/
   /****************************************/

   CWheeledEntity::CWheeledEntity(CComposableEntity* pc_parent,
                                  const std::string& str_id,
                                  size_t un_num_wheels) :
      CEntity(pc_parent, str_id),
      m_unNumWheels(un_num_wheels) {
      m_pcWheelPositions = new CVector3[m_unNumWheels];
      m_pfWheelRadia = new Real[m_unNumWheels];
      ::memset(m_pfWheelRadia, 0, m_unNumWheels * sizeof(Real));
      m_pfWheelVelocities = new Real[m_unNumWheels];
      ::memset(m_pfWheelVelocities, 0, m_unNumWheels * sizeof(Real));
      Disable();
   }
   
   /****************************************/
   /****************************************/

   CWheeledEntity::~CWheeledEntity() {
      delete[] m_pcWheelPositions;
      delete[] m_pfWheelRadia;
      delete[] m_pfWheelVelocities;
   }
   
   /****************************************/
   /****************************************/
   
   void CWheeledEntity::Reset() {
      ::memset(m_pfWheelVelocities, 0, m_unNumWheels * sizeof(Real));
   }
   
   /****************************************/
   /****************************************/
   
   void CWheeledEntity::SetWheel(UInt32 un_index,
                                 const CVector3& c_position,
                                 Real f_radius) {
      if(un_index < m_unNumWheels) {
         m_pcWheelPositions[un_index] = c_position;
         m_pfWheelRadia[un_index] = f_radius;
      }
      else {
         THROW_ARGOSEXCEPTION("CWheeledEntity::SetWheel() : index " << un_index << " out of bounds (allowed [0:" << m_unNumWheels << "])");
      }
   }

   /****************************************/
   /****************************************/

   const CVector3& CWheeledEntity::GetWheelPosition(size_t un_index) const {
      if(un_index < m_unNumWheels) {
         return m_pcWheelPositions[un_index];
      }
      else {
         THROW_ARGOSEXCEPTION("CWheeledEntity::GetWheelPosition() : index " << un_index << " out of bounds (allowed [0:" << m_unNumWheels << "])");
      }
   }
   
   /****************************************/
   /****************************************/
   
   Real CWheeledEntity::GetWheelRadius(size_t un_index) const {
      if(un_index < m_unNumWheels) {
         return m_pfWheelRadia[un_index];
      }
      else {
         THROW_ARGOSEXCEPTION("CWheeledEntity::GetWheelRadius() : index " << un_index << " out of bounds (allowed [0:" << m_unNumWheels << "])");
      }
   }
   
   /****************************************/
   /****************************************/
   
   Real CWheeledEntity::GetWheelVelocity(size_t un_index) const {
      if(un_index < m_unNumWheels) {
         return m_pfWheelVelocities[un_index];
      }
      else {
         THROW_ARGOSEXCEPTION("CWheeledEntity::GetWheelVelocity() : index " << un_index << " out of bounds (allowed [0:" << m_unNumWheels << "])");
      }
   }
   
   /****************************************/
   /****************************************/
   
   void CWheeledEntity::SetVelocities(Real* pf_velocities) {
      ::memcpy(m_pfWheelVelocities, pf_velocities, m_unNumWheels * sizeof(Real));
   }

   /****************************************/
   /****************************************/

   REGISTER_STANDARD_SPACE_OPERATIONS_ON_ENTITY(CWheeledEntity);

   /****************************************/
   /****************************************/

}
