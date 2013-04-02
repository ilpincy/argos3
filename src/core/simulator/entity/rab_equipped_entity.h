/**
 * @file <argos3/core/simulator/entity/rab_equipped_entity.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef RAB_EQUIPPED_ENTITY_H
#define RAB_EQUIPPED_ENTITY_H

#include <argos3/core/utility/datatypes/byte_array.h>
#include <argos3/core/utility/datatypes/set.h>
#include <argos3/core/simulator/entity/positional_entity.h>
#include <argos3/core/utility/math/vector3.h>
#include <argos3/core/simulator/space/positional_indices/space_hash.h>

namespace argos {

   class CRABEquippedEntity : public CPositionalEntity {

   public:

      ENABLE_VTABLE();

      typedef std::vector<CRABEquippedEntity*> TVector;
      typedef CSet<CRABEquippedEntity*> TSet;

   public:

      CRABEquippedEntity(CComposableEntity* pc_parent);

      CRABEquippedEntity(CComposableEntity* pc_parent,
                         const std::string& str_id,
                         size_t un_msg_size,
                         Real f_range,
                         const CPositionalEntity& c_reference,
                         const CVector3& c_position = CVector3(),
                         const CQuaternion& c_orientation = CQuaternion());

      virtual void Init(TConfigurationNode& t_tree);

      virtual ~CRABEquippedEntity() {}

      virtual void Reset();

      virtual void Update();

      inline size_t GetMsgSize() const {
         return m_cData.Size();
      }

      inline CByteArray& GetData() {
         return m_cData;
      }

      void SetData(const CByteArray& c_data);

      void ClearData();

      inline Real GetRange() const {
         return m_fRange;
      }

      virtual std::string GetTypeDescription() const {
         return "rab";
      }

   protected:

      const CPositionalEntity* m_pcReference;
      CVector3 m_cPosOffset;
      CQuaternion m_cRotOffset;
      CByteArray m_cData;
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
