/**
 * @file <argos3/core/simulator/entity/rab_equipped_entity.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef RAB_EQUIPPED_ENTITY_H
#define RAB_EQUIPPED_ENTITY_H

#include <argos3/core/simulator/entity/entity.h>
#include <argos3/core/utility/math/vector3.h>
#include <argos3/core/simulator/space/space_hash.h>

namespace argos {

   class CRABEquippedEntity : public CEntity {

   public:

      CRABEquippedEntity(CEntity* pc_parent, size_t un_msg_size) :
         CEntity(pc_parent),
         m_unMsgSize(un_msg_size),
         m_punData(new UInt8[un_msg_size]),
         m_fRange(0.0f) {
         bool bFound = false;
         size_t i = 0;
         while(!bFound && i < AVAILABLE_IDS.size()) {
            if(AVAILABLE_IDS[i]) {
               m_unNumericId = i;
               AVAILABLE_IDS[i] = 0;
               bFound = true;
            }
            else {
               ++i;
            }
         }         
         if(!bFound) {
            THROW_ARGOSEXCEPTION("The total amount of range and bearing devices cannot be more than 65535.");
         }
         ::memset(m_punData, 0, m_unMsgSize);
      }
      virtual ~CRABEquippedEntity() {
         /* Mark the ID as available */
         AVAILABLE_IDS[m_unNumericId] = 1;
         delete[] m_punData;
      }

      virtual void Reset() {
         ::memset(m_punData, 0, m_unMsgSize);
      }

      virtual void Update() {}

      inline const CVector3& GetPosition() const {
         return m_cPosition;
      }

      inline void SetPosition(const CVector3& c_position) {
         m_cPosition = c_position;
      }

      inline size_t GetMsgSize() const {
         return m_unMsgSize;
      }

      inline void GetData(UInt8* pun_data) const {
         ::memcpy(pun_data, m_punData, m_unMsgSize);
      }

      inline void SetData(const UInt8* pun_data) {
         ::memcpy(m_punData, pun_data, m_unMsgSize);
      }

      inline Real GetRange() const {
         return m_fRange;
      }

      inline void SetRange(Real f_range) {
         m_fRange = f_range;
      }

      inline UInt16 GetNumericId() const {
         return m_unNumericId;
      }

      inline void ClearData() {
         ::memset(m_punData, 0, m_unMsgSize);
      }

      virtual std::string GetTypeDescription() const {
         return "rab_equipped_entity";
      }

   protected:

      CVector3 m_cPosition;
      size_t m_unMsgSize;
      UInt8* m_punData;
      Real m_fRange;
      UInt16 m_unNumericId;

   private:

      static std::vector<UInt16> AVAILABLE_IDS;

   };

   typedef std::vector<CRABEquippedEntity*> TRABEquippedEntityList;
   typedef std::tr1::unordered_set<CRABEquippedEntity*> TRABEquippedEntitySet;

   /****************************************/
   /****************************************/

   class CRABEquippedEntitySpaceHashUpdater : public CSpaceHashUpdater<CRABEquippedEntity> {

   public:

      virtual void operator()(CAbstractSpaceHash<CRABEquippedEntity>& c_space_hash,
                              CRABEquippedEntity& c_element);

   private:

      SInt32 m_nCenterI, m_nCenterJ, m_nCenterK;

   };

   /****************************************/
   /****************************************/

}

#endif
