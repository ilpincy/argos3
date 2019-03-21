/**
 * @file <argos3/plugins/simulator/entities/rotor_equipped_entity.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "rotor_equipped_entity.h"
#include <argos3/core/simulator/space/space.h>

namespace argos {

   /****************************************/
   /****************************************/

   CRotorEquippedEntity::CRotorEquippedEntity(CComposableEntity* pc_parent,
                                              size_t un_num_rotors) :
      CEntity(pc_parent),
      m_unNumRotors(un_num_rotors) {
      m_pcRotorPositions = new CVector3[m_unNumRotors];
      m_pfRotorVelocities = new Real[m_unNumRotors];
      ::memset(m_pfRotorVelocities, 0, m_unNumRotors * sizeof(Real));
      Disable();
   }

   /****************************************/
   /****************************************/

   CRotorEquippedEntity::CRotorEquippedEntity(CComposableEntity* pc_parent,
                                              const std::string& str_id,
                                              size_t un_num_rotors) :
      CEntity(pc_parent, str_id),
      m_unNumRotors(un_num_rotors) {
      m_pcRotorPositions = new CVector3[m_unNumRotors];
      m_pfRotorVelocities = new Real[m_unNumRotors];
      ::memset(m_pfRotorVelocities, 0, m_unNumRotors * sizeof(Real));
      Disable();
   }
   
   /****************************************/
   /****************************************/

   CRotorEquippedEntity::~CRotorEquippedEntity() {
      delete[] m_pcRotorPositions;
      delete[] m_pfRotorVelocities;
   }
   
   /****************************************/
   /****************************************/
   
   void CRotorEquippedEntity::Reset() {
      ::memset(m_pfRotorVelocities, 0, m_unNumRotors * sizeof(Real));
   }
   
   /****************************************/
   /****************************************/
   
   void CRotorEquippedEntity::SetRotor(UInt32 un_index,
                                       const CVector3& c_position) {
      if(un_index < m_unNumRotors) {
         m_pcRotorPositions[un_index] = c_position;
      }
      else {
         THROW_ARGOSEXCEPTION("CRotorEquippedEntity::SetRotor() : index " << un_index << " out of bounds (allowed [0:" << m_unNumRotors << "])");
      }
   }

   /****************************************/
   /****************************************/

   const CVector3& CRotorEquippedEntity::GetRotorPosition(size_t un_index) const {
      if(un_index < m_unNumRotors) {
         return m_pcRotorPositions[un_index];
      }
      else {
         THROW_ARGOSEXCEPTION("CRotorEquippedEntity::GetRotorPosition() : index " << un_index << " out of bounds (allowed [0:" << m_unNumRotors << "])");
      }
   }
   
   /****************************************/
   /****************************************/
   
   Real CRotorEquippedEntity::GetRotorVelocity(size_t un_index) const {
      if(un_index < m_unNumRotors) {
         return m_pfRotorVelocities[un_index];
      }
      else {
         THROW_ARGOSEXCEPTION("CRotorEquippedEntity::GetRotorVelocity() : index " << un_index << " out of bounds (allowed [0:" << m_unNumRotors << "])");
      }
   }
   
   /****************************************/
   /****************************************/
   
   void CRotorEquippedEntity::SetVelocities(Real* pf_velocities) {
      ::memcpy(m_pfRotorVelocities, pf_velocities, m_unNumRotors * sizeof(Real));
   }

   /****************************************/
   /****************************************/

   REGISTER_STANDARD_SPACE_OPERATIONS_ON_ENTITY(CRotorEquippedEntity);

   /****************************************/
   /****************************************/

}
