/**
 * @file <argos3/core/simulator/entity/rab_equipped_entity.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef RAB_EQUIPPED_ENTITY_H
#define RAB_EQUIPPED_ENTITY_H

namespace argos {
   class CRABEquippedEntity;
   class CEmbodiedEntity;
   struct SAnchor;
}

#include <argos3/core/utility/datatypes/byte_array.h>
#include <argos3/core/utility/datatypes/set.h>
#include <argos3/core/simulator/entity/positional_entity.h>
#include <argos3/core/utility/math/vector3.h>
#include <argos3/core/simulator/space/positional_indices/space_hash.h>
#include <argos3/core/simulator/space/positional_indices/grid.h>

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
                         SAnchor& s_anchor,
                         CEmbodiedEntity& c_entity_body,
                         const CVector3& c_position = CVector3(),
                         const CQuaternion& c_orientation = CQuaternion());

      virtual void Init(TConfigurationNode& t_tree);

      virtual ~CRABEquippedEntity() {}

      virtual void Reset();

      virtual void Enable();

      virtual void Disable();

      virtual void Update();

      inline CEmbodiedEntity& GetEntityBody() {
         return *m_pcEntityBody;
      }

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

      inline void SetRange(Real f_range) {
         m_fRange = f_range;
      }

      inline const SAnchor& GetAnchor() const {
         return *m_psAnchor;
      }

      virtual std::string GetTypeDescription() const {
         return "rab";
      }

   protected:

      SAnchor* m_psAnchor;
      CVector3 m_cPosOffset;
      CQuaternion m_cRotOffset;
      CByteArray m_cData;
      Real m_fRange;
      CEmbodiedEntity* m_pcEntityBody;

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

   class CRABEquippedEntityGridCellUpdater : public CGrid<CRABEquippedEntity>::CCellOperation {

   public:

      CRABEquippedEntityGridCellUpdater(CGrid<CRABEquippedEntity>& c_grid);

      virtual bool operator()(SInt32 n_i,
                              SInt32 n_j,
                              SInt32 n_k,
                              CGrid<CRABEquippedEntity>::SCell& s_cell);

      void SetEntity(CRABEquippedEntity& c_entity);

   private:

      CGrid<CRABEquippedEntity>& m_cGrid;
      CRABEquippedEntity* m_pcEntity;
   };

   class CRABEquippedEntityGridEntityUpdater : public CGrid<CRABEquippedEntity>::COperation {

   public:

      CRABEquippedEntityGridEntityUpdater(CGrid<CRABEquippedEntity>& c_grid);
      virtual bool operator()(CRABEquippedEntity& c_entity);

   private:

      CGrid<CRABEquippedEntity>& m_cGrid;
      CRABEquippedEntityGridCellUpdater m_cCellUpdater;
   };

   /****************************************/
   /****************************************/

}

#endif
