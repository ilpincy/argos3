/**
 * @file <argos3/core/simulator/entity/rab_equipped_entity.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef RAB_EQUIPPED_ENTITY_H
#define RAB_EQUIPPED_ENTITY_H

#include <argos3/core/simulator/entity/positional_entity.h>
#include <argos3/core/utility/math/vector3.h>
#include <argos3/core/simulator/space/space_hash.h>

namespace argos {

   class CRABEquippedEntity : public CPositionalEntity {

   public:

      ENABLE_VTABLE();

      typedef std::vector<CRABEquippedEntity*> TVector;
      typedef std::tr1::unordered_set<CRABEquippedEntity*> TSet;

   public:

      CRABEquippedEntity(CComposableEntity* pc_parent,
                         size_t un_msg_size);

      CRABEquippedEntity(CComposableEntity* pc_parent,
                         const std::string& str_id,
                         const CVector3& c_position,
                         const CQuaternion& c_orientation,
                         size_t un_msg_size,
                         Real f_range);

      virtual ~CRABEquippedEntity();

      virtual void Reset();

      inline size_t GetMsgSize() const {
         return m_unMsgSize;
      }

      void GetData(UInt8* pun_data) const;

      void SetData(const UInt8* pun_data);

      inline Real GetRange() const {
         return m_fRange;
      }

      inline void SetRange(Real f_range) {
         m_fRange = f_range;
      }

      void ClearData();

      virtual std::string GetTypeDescription() const {
         return "rab";
      }

   protected:

      size_t m_unMsgSize;
      UInt8* m_punData;
      Real m_fRange;

   };

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
