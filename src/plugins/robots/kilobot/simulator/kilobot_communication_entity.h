/**
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef KILOBOT_COMMUNICATION_ENTITY_H
#define KILOBOT_COMMUNICATION_ENTITY_H

namespace argos {
   class CKilobotCommunicationEntity;
   class CEmbodiedEntity;
   struct SAnchor;
}

#include <argos3/core/utility/datatypes/byte_array.h>
#include <argos3/core/utility/datatypes/set.h>
#include <argos3/core/utility/math/vector3.h>
#include <argos3/core/simulator/entity/positional_entity.h>
#include <argos3/core/simulator/space/positional_indices/space_hash.h>
#include <argos3/core/simulator/space/positional_indices/grid.h>
#include <argos3/plugins/robots/kilobot/control_interface/kilolib.h>

namespace argos {

   class CKilobotCommunicationEntity : public CPositionalEntity {

   public:

      ENABLE_VTABLE();

      typedef std::vector<CKilobotCommunicationEntity*> TVector;
      typedef CSet<CKilobotCommunicationEntity*> TSet;

      enum ETxStatus {
         TX_NONE,
         TX_ATTEMPT,
         TX_SUCCESS
      };

   public:

      CKilobotCommunicationEntity(CComposableEntity* pc_parent,
                                  const std::string& str_id,
                                  size_t un_msg_size,
                                  Real f_range,
                                  SAnchor& s_anchor,
                                  CEmbodiedEntity& c_entity_body);

      virtual ~CKilobotCommunicationEntity() {}

      virtual void Reset();

      virtual void Update();

      virtual void SetEnabled(bool b_enabled);

      inline CEmbodiedEntity& GetEntityBody() {
         return *m_pcEntityBody;
      }

      inline Real GetTxRange() const {
         return m_fTxRange;
      }

      inline void SetTxRange(Real f_range) {
         m_fTxRange = f_range;
      }

      inline ETxStatus GetTxStatus() const {
         return m_eTxStatus;
      }

      inline void SetTxStatus(ETxStatus e_tx_status) {
         m_eTxStatus = e_tx_status;
      }

      inline const message_t* GetTxMessage() const {
         return m_ptMessage;
      }

      inline void SetTxMessage(message_t* pt_msg) {
         m_ptMessage = pt_msg;
      }

      inline const SAnchor& GetAnchor() const {
         return *m_psAnchor;
      }

      virtual std::string GetTypeDescription() const {
         return "kilocomm";
      }

   protected:

      /** Body anchor this entity is attached to */
      SAnchor* m_psAnchor;

      /** Transmission range */
      Real m_fTxRange;

      /** Robot body */
      CEmbodiedEntity* m_pcEntityBody;

      /** Current message transmission status */
      ETxStatus m_eTxStatus;

      /** The message to send */
      message_t* m_ptMessage;
   };

   /****************************************/
   /****************************************/

   class CKilobotCommunicationEntitySpaceHashUpdater : public CSpaceHashUpdater<CKilobotCommunicationEntity> {

   public:

      virtual void operator()(CAbstractSpaceHash<CKilobotCommunicationEntity>& c_space_hash,
                              CKilobotCommunicationEntity& c_element);

   private:

      SInt32 m_nCenterI, m_nCenterJ, m_nCenterK;

   };

   /****************************************/
   /****************************************/

   class CKilobotCommunicationEntityGridCellUpdater : public CGrid<CKilobotCommunicationEntity>::CCellOperation {

   public:

      CKilobotCommunicationEntityGridCellUpdater(CGrid<CKilobotCommunicationEntity>& c_grid);

      virtual bool operator()(SInt32 n_i,
                              SInt32 n_j,
                              SInt32 n_k,
                              CGrid<CKilobotCommunicationEntity>::SCell& s_cell);

      void SetEntity(CKilobotCommunicationEntity& c_entity);

   private:

      CGrid<CKilobotCommunicationEntity>& m_cGrid;
      CKilobotCommunicationEntity* m_pcEntity;
   };

   class CKilobotCommunicationEntityGridEntityUpdater : public CGrid<CKilobotCommunicationEntity>::COperation {

   public:

      CKilobotCommunicationEntityGridEntityUpdater(CGrid<CKilobotCommunicationEntity>& c_grid);
      virtual bool operator()(CKilobotCommunicationEntity& c_entity);

   private:

      CGrid<CKilobotCommunicationEntity>& m_cGrid;
      CKilobotCommunicationEntityGridCellUpdater m_cCellUpdater;
   };

   /****************************************/
   /****************************************/

}

#endif
