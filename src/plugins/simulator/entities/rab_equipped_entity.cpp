/**
 * @file <argos3/core/simulator/entity/rab_equipped_entity.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "rab_equipped_entity.h"
#include <argos3/core/utility/string_utilities.h>
#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/space/space.h>

namespace argos {

   /****************************************/
   /****************************************/

   CRABEquippedEntity::CRABEquippedEntity(CComposableEntity* pc_parent) :
      CPositionalEntity(pc_parent),
      m_pcReference(NULL),
      m_fRange(0.0f) {
      Disable();
      SetCanBeEnabledIfDisabled(false);
   }

   /****************************************/
   /****************************************/

   CRABEquippedEntity::CRABEquippedEntity(CComposableEntity* pc_parent,
                                          const std::string& str_id,
                                          size_t un_msg_size,
                                          Real f_range,
                                          const CPositionalEntity& c_reference,
                                          const CVector3& c_pos_offset,
                                          const CQuaternion& c_rot_offset) :
      CPositionalEntity(pc_parent,
                        str_id,
                        c_reference.GetInitPosition() + c_pos_offset,
                        c_reference.GetInitOrientation() * c_rot_offset),
      m_pcReference(&c_reference),
      m_cPosOffset(c_pos_offset),
      m_cRotOffset(c_rot_offset),
      m_cData(un_msg_size),
      m_fRange(f_range) {
      Disable();
      SetCanBeEnabledIfDisabled(false);
   }

   /****************************************/
   /****************************************/

   void CRABEquippedEntity::Init(TConfigurationNode& t_tree) {
      try {
         /*
          * Init entity.
          * Here we explicitly avoid to call CPositionalEntity::Init() because that
          * would also initialize position and orientation, which, instead, must
          * be calculated from reference entity and offsets.
          */
         CEntity::Init(t_tree);
         /* Get reference entity */
         std::string strReference;
         GetNodeAttribute(t_tree, "reference", strReference);
         m_pcReference = dynamic_cast<CPositionalEntity*>(&CSimulator::GetInstance().GetSpace().GetEntity(strReference));
         if(m_pcReference == NULL) {
            THROW_ARGOSEXCEPTION("Entity \"" << strReference << "\" can't be used as a reference for range and bearing entity \"" << GetId() << "\"");
         }
         /* Get offsets */
         GetNodeAttributeOrDefault(t_tree, "pos_offset", m_cPosOffset, m_cPosOffset);
         std::string strRotOffset;
         GetNodeAttributeOrDefault(t_tree, "rot_offset", strRotOffset, strRotOffset);
         if(strRotOffset != "") {
            CDegrees cRotOffsetEuler[3];
            ParseValues(strRotOffset, 3, cRotOffsetEuler, ',');
            m_cRotOffset.FromEulerAngles(ToRadians(cRotOffsetEuler[0]),
                                         ToRadians(cRotOffsetEuler[1]),
                                         ToRadians(cRotOffsetEuler[2]));
         }
         /* Set init position and orientation */
         SetInitPosition(m_pcReference->GetInitPosition() + m_cPosOffset);
         SetInitOrientation(m_pcReference->GetInitOrientation() * m_cRotOffset);
         SetPosition(GetInitPosition());
         SetOrientation(GetInitOrientation());
         /* Get message size */
         size_t unMsgSize;
         GetNodeAttribute(t_tree, "msg_size", unMsgSize);
         m_cData.Resize(unMsgSize);
         /* Get transmission range */
         GetNodeAttribute(t_tree, "range", m_fRange);
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Error initializing a range and bearing entity \"" << GetId() << "\"", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CRABEquippedEntity::Update() {
      SetPosition(m_pcReference->GetPosition() + m_cPosOffset);
      SetOrientation(m_pcReference->GetOrientation() * m_cRotOffset);
   }

   /****************************************/
   /****************************************/

   void CRABEquippedEntity::Reset() {
      m_cData.Zero();
   }

   /****************************************/
   /****************************************/

   void CRABEquippedEntity::SetData(const CByteArray& c_data) {
      if(m_cData.Size() == c_data.Size()) {
         m_cData = c_data;
      }
      else {
         THROW_ARGOSEXCEPTION("CRABEquippedEntity::SetData() : data size does not match, expected " << m_cData.Size() << ", got " << c_data.Size());
      }
   }

   /****************************************/
   /****************************************/

   void CRABEquippedEntity::ClearData() {
      m_cData.Zero();
   }

   /****************************************/
   /****************************************/

   void CRABEquippedEntitySpaceHashUpdater::operator()(CAbstractSpaceHash<CRABEquippedEntity>& c_space_hash,
                                                       CRABEquippedEntity& c_element) {
      /* Calculate the position of the center of the RAB equipped entity in the space hash */
      c_space_hash.SpaceToHashTable(m_nCenterI,
                                    m_nCenterJ,
                                    m_nCenterK,
                                    c_element.GetPosition());
      /* Update the cells in a sphere around it */
      SInt32 nRangeI = c_space_hash.SpaceToHashTable(c_element.GetRange(), 0);
      SInt32 nRangeJ;
      SInt32 nRangeK;
      for(SInt32 i = 0; i <= nRangeI; ++i) {
         nRangeJ =
            c_space_hash.SpaceToHashTable(
               ::sqrt(
                  Square(c_element.GetRange()) -
                  Square(c_space_hash.HashTableToSpace(i, 0))
                  ),
               1);
         for(SInt32 j = 0; j <= nRangeJ; ++j) {
            nRangeK =
               c_space_hash.SpaceToHashTable(
                  ::sqrt(
                     Square(c_element.GetRange()) -
                     Square(c_space_hash.HashTableToSpace(j, 1))
                     ),
                  2);
            for(SInt32 k = 0; k <= nRangeK; ++k) {
               if(i > 0) {
                  /*
                   * i > 0
                   */
                  if(j > 0) {
                     /*
                      * i > 0
                      * j > 0
                      */
                     if(k > 0) {
                        /*
                         * i > 0
                         * j > 0
                         * k > 0
                         */
                        c_space_hash.UpdateCell(m_nCenterI + i, m_nCenterJ + j, m_nCenterK + k, c_element);
                        c_space_hash.UpdateCell(m_nCenterI + i, m_nCenterJ + j, m_nCenterK - k, c_element);
                        c_space_hash.UpdateCell(m_nCenterI + i, m_nCenterJ - j, m_nCenterK + k, c_element);
                        c_space_hash.UpdateCell(m_nCenterI + i, m_nCenterJ - j, m_nCenterK - k, c_element);
                        c_space_hash.UpdateCell(m_nCenterI - i, m_nCenterJ + j, m_nCenterK + k, c_element);
                        c_space_hash.UpdateCell(m_nCenterI - i, m_nCenterJ + j, m_nCenterK - k, c_element);
                        c_space_hash.UpdateCell(m_nCenterI - i, m_nCenterJ - j, m_nCenterK + k, c_element);
                        c_space_hash.UpdateCell(m_nCenterI - i, m_nCenterJ - j, m_nCenterK - k, c_element);
                     }
                     else {
                        /*
                         * i > 0
                         * j > 0
                         * k == 0
                         */
                        c_space_hash.UpdateCell(m_nCenterI + i, m_nCenterJ + j, m_nCenterK, c_element);
                        c_space_hash.UpdateCell(m_nCenterI + i, m_nCenterJ - j, m_nCenterK, c_element);
                        c_space_hash.UpdateCell(m_nCenterI - i, m_nCenterJ + j, m_nCenterK, c_element);
                        c_space_hash.UpdateCell(m_nCenterI - i, m_nCenterJ - j, m_nCenterK, c_element);
                     }
                  }
                  else {
                     /*
                      * i > 0
                      * j == 0
                      */
                     if(k > 0) {
                        /*
                         * i > 0
                         * j == 0
                         * k > 0
                         */
                        c_space_hash.UpdateCell(m_nCenterI + i, m_nCenterJ, m_nCenterK + k, c_element);
                        c_space_hash.UpdateCell(m_nCenterI + i, m_nCenterJ, m_nCenterK - k, c_element);
                        c_space_hash.UpdateCell(m_nCenterI - i, m_nCenterJ, m_nCenterK + k, c_element);
                        c_space_hash.UpdateCell(m_nCenterI - i, m_nCenterJ, m_nCenterK - k, c_element);
                     }
                     else {
                        /*
                         * i > 0
                         * j == 0
                         * k == 0
                         */
                        c_space_hash.UpdateCell(m_nCenterI + i, m_nCenterJ, m_nCenterK, c_element);
                        c_space_hash.UpdateCell(m_nCenterI - i, m_nCenterJ, m_nCenterK, c_element);
                     }
                  }
               }
               else {
                  /*
                   * i == 0
                   */
                  if(j > 0) {
                     /*
                      * i == 0
                      * j > 0
                      */
                     if(k > 0) {
                        /*
                         * i == 0
                         * j > 0
                         * k > 0
                         */
                        c_space_hash.UpdateCell(m_nCenterI, m_nCenterJ + j, m_nCenterK + k, c_element);
                        c_space_hash.UpdateCell(m_nCenterI, m_nCenterJ + j, m_nCenterK - k, c_element);
                        c_space_hash.UpdateCell(m_nCenterI, m_nCenterJ - j, m_nCenterK + k, c_element);
                        c_space_hash.UpdateCell(m_nCenterI, m_nCenterJ - j, m_nCenterK - k, c_element);
                     }
                     else {
                        /*
                         * i == 0
                         * j > 0
                         * k == 0
                         */
                        c_space_hash.UpdateCell(m_nCenterI, m_nCenterJ + j, m_nCenterK, c_element);
                        c_space_hash.UpdateCell(m_nCenterI, m_nCenterJ - j, m_nCenterK, c_element);
                     }
                  }
                  else {                     
                     /*
                      * i == 0
                      * j == 0
                      */
                     if(k > 0) {
                        /*
                         * i == 0
                         * j == 0
                         * k > 0
                         */
                        c_space_hash.UpdateCell(m_nCenterI, m_nCenterJ, m_nCenterK + k, c_element);
                        c_space_hash.UpdateCell(m_nCenterI, m_nCenterJ, m_nCenterK - k, c_element);
                     }
                     else {
                        /*
                         * i == 0
                         * j == 0
                         * k == 0
                         */
                        c_space_hash.UpdateCell(m_nCenterI, m_nCenterJ, m_nCenterK, c_element);
                     }
                  }
               }
            }
         }
      }
   }

   /****************************************/
   /****************************************/

   class CSpaceOperationAddRABEquippedEntity : public CSpaceOperationAddEntity {
   public:
      void ApplyTo(CSpace& c_space, CRABEquippedEntity& c_entity) {
         c_space.AddEntity(c_entity);
      }
   };
   REGISTER_SPACE_OPERATION(CSpaceOperationAddEntity, CSpaceOperationAddRABEquippedEntity, CRABEquippedEntity);
   
   class CSpaceOperationRemoveRABEquippedEntity : public CSpaceOperationRemoveEntity {
   public:
      void ApplyTo(CSpace& c_space, CRABEquippedEntity& c_entity) {
         c_space.RemoveEntity(c_entity);
      }
   };
   REGISTER_SPACE_OPERATION(CSpaceOperationRemoveEntity, CSpaceOperationRemoveRABEquippedEntity, CRABEquippedEntity);

   /****************************************/
   /****************************************/

}
