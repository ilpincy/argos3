/**
 * @file <argos3/plugins/simulator/entities/tag_equipped_entity.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "tag_equipped_entity.h"
#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/plugins/simulator/media/tag_medium.h>

namespace argos {

   /****************************************/
   /****************************************/

   CTagEquippedEntity::SInstance::SInstance(CTagEntity& c_tag,
                                            SAnchor& s_anchor,
                                            const CVector3& c_position_offset,
                                            const CQuaternion& c_orientation_offset) :
      Tag(c_tag),
      Anchor(s_anchor),
      PositionOffset(c_position_offset),
      OrientationOffset(c_orientation_offset) {}

   /****************************************/
   /****************************************/

   CTagEquippedEntity::CTagEquippedEntity(CComposableEntity* pc_parent) :
      CComposableEntity(pc_parent) {
      Disable();
   }

   /****************************************/
   /****************************************/

   CTagEquippedEntity::CTagEquippedEntity(CComposableEntity* pc_parent,
                                          const std::string& str_id) :
      CComposableEntity(pc_parent, str_id) {
      Disable();
   }

   /****************************************/
   /****************************************/

   void CTagEquippedEntity::Init(TConfigurationNode& t_tree) {
      try {
         /* Init parent */
         CComposableEntity::Init(t_tree);
         /* Go through the tag entries */
         TConfigurationNodeIterator itTag("tag");
         for(itTag = itTag.begin(&t_tree);
             itTag != itTag.end();
             ++itTag) {
            /* Initialise the Tag using the XML */
            CTagEntity* pcTag = new CTagEntity(this);
            pcTag->Init(*itTag);
            CVector3 cPositionOffset;
            GetNodeAttribute(*itTag, "position", cPositionOffset);
            CQuaternion cOrientationOffset;
            GetNodeAttribute(*itTag, "orientation", cOrientationOffset);
            /* Parse and look up the anchor */
            std::string strAnchorId;
            GetNodeAttribute(*itTag, "anchor", strAnchorId);
            /*
             * NOTE: here we get a reference to the embodied entity
             * This line works under the assumption that:
             * 1. the TagEquippedEntity has a parent;
             * 2. the parent has a child whose id is "body"
             * 3. the "body" is an embodied entity
             * If any of the above is false, this line will bomb out.
             */
            CEmbodiedEntity& cBody = GetParent().GetComponent<CEmbodiedEntity>("body");
            /* Add the tag to this container */
            m_vecInstances.emplace_back(*pcTag,
                                        cBody.GetAnchor(strAnchorId),
                                        cPositionOffset,
                                        cOrientationOffset);
            AddComponent(*pcTag);
         }
         UpdateComponents();
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Failed to initialize tag equipped entity \"" <<
                                     GetContext() + GetId() << "\".", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CTagEquippedEntity::Enable() {
      /* Perform generic enable behavior */
      CComposableEntity::Enable();
      /* Enable anchors */
      for(SInstance& s_instance : m_vecInstances) {
         s_instance.Anchor.Enable();
      }
   }

   /****************************************/
   /****************************************/

   void CTagEquippedEntity::Disable() {
      /* Perform generic disable behavior */
      CComposableEntity::Disable();
      /* Disable anchors */
      for(SInstance& s_instance : m_vecInstances) {
         s_instance.Anchor.Disable();
      }
   }

   /****************************************/
   /****************************************/

   void CTagEquippedEntity::AddTag(const std::string& str_id,
                                   const CVector3& c_position,
                                   const CQuaternion& c_orientation,
                                   SAnchor& s_anchor,
                                   const CRadians& c_observable_angle,
                                   Real f_side_length,
                                   const std::string& str_payload) {
      /* create the new tag entity */
      CTagEntity* pcTag =
         new CTagEntity(this,
                        str_id,
                        c_position,
                        c_orientation,
                        c_observable_angle,
                        f_side_length,
                        str_payload);
      /* add it to the instances vector */
      m_vecInstances.emplace_back(*pcTag,
                                  s_anchor,
                                  c_position,
                                  c_orientation);
      /* inform the base class about the new entity */
      AddComponent(*pcTag);
      UpdateComponents();
   }

   /****************************************/
   /****************************************/

   CTagEntity& CTagEquippedEntity::GetTag(UInt32 un_index) {
      ARGOS_ASSERT(un_index < m_vecInstances.size(),
                   "CTagEquippedEntity::GetTag(), id=\"" <<
                   GetContext() << GetId() <<
                   "\": index out of bounds: un_index = " <<
                   un_index <<
                   ", m_vecInstances.size() = " <<
                   m_vecInstances.size());
      return m_vecInstances[un_index].Tag;
   }

   /****************************************/
   /****************************************/

   void CTagEquippedEntity::SetTagPayload(UInt32 un_index,
                                          const std::string& str_payload) {
      ARGOS_ASSERT(un_index < m_vecInstances.size(),
                   "CTagEquippedEntity::SetTagPayload(), id=\"" <<
                   GetContext() << GetId() <<
                   "\": index out of bounds: un_index = " <<
                   un_index <<
                   ", m_vecInstances.size() = " <<
                   m_vecInstances.size());
      m_vecInstances[un_index].Tag.SetPayload(str_payload);
   }

   /****************************************/
   /****************************************/

   void CTagEquippedEntity::SetTagPayloads(const std::string& str_payload) {
      for(SInstance& s_instance : m_vecInstances) {
         s_instance.Tag.SetPayload(str_payload);
      }
   }

   /****************************************/
   /****************************************/

   void CTagEquippedEntity::SetTagPayloads(const std::vector<std::string>& vec_payloads) {
      if(vec_payloads.size() == m_vecInstances.size()) {
         for(UInt32 i = 0; i < vec_payloads.size(); ++i) {
            m_vecInstances[i].Tag.SetPayload(vec_payloads[i]);
         }
      }
      else {
         THROW_ARGOSEXCEPTION(
            "CTagEquippedEntity::SetTagPayloads(), id=\"" <<
            GetContext() << GetId() <<
            "\": number of tags (" <<
            m_vecInstances.size() <<
            ") does not equal the passed payload vector size (" <<
            vec_payloads.size() <<
            ")");
      }
   }

   /****************************************/
   /****************************************/

   void CTagEquippedEntity::UpdateComponents() {
      /* Tag position wrt global reference frame */
      CVector3 cTagPosition;
      CQuaternion cTagOrientation;
      for(SInstance& s_instance : m_vecInstances) {
         if(s_instance.Tag.IsEnabled()) {
            cTagPosition = s_instance.PositionOffset;
            cTagPosition.Rotate(s_instance.Anchor.Orientation);
            cTagPosition += s_instance.Anchor.Position;
            cTagOrientation = s_instance.Anchor.Orientation *
               s_instance.OrientationOffset;
            s_instance.Tag.MoveTo(cTagPosition, cTagOrientation);
         }
      }
   }

   /****************************************/
   /****************************************/

   void CTagEquippedEntity::SetMedium(CTagMedium& c_medium) {
      for(SInstance& s_instance : m_vecInstances) {
         s_instance.Tag.SetMedium(c_medium);
      }
   }

   /****************************************/
   /****************************************/

   REGISTER_STANDARD_SPACE_OPERATIONS_ON_COMPOSABLE(CTagEquippedEntity);

   /****************************************/
   /****************************************/

}
