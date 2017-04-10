#include "perspective_camera_equipped_entity.h"
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/entity/composable_entity.h>

namespace argos {

   /****************************************/
   /****************************************/

   CPerspectiveCameraEquippedEntity::CPerspectiveCameraEquippedEntity(CComposableEntity* pc_parent) :
      CEntity(pc_parent),
      m_fFocalLength(0.0f),
      m_fRange(0.0f),
      m_nImagePxWidth(0),
      m_nImagePxHeight(0),
      m_psAnchor(NULL) {
      Disable();
   }

   /****************************************/
   /****************************************/

   CPerspectiveCameraEquippedEntity::CPerspectiveCameraEquippedEntity(CComposableEntity* pc_parent,
                                                                      const std::string& str_id,
                                                                      const CRadians& c_aperture,
                                                                      Real f_focal_length,
                                                                      Real f_range,
                                                                      SInt32 n_width,
                                                                      SInt32 n_height,
                                                                      SAnchor& s_anchor) :
      CEntity(pc_parent, str_id),
      m_cAperture(c_aperture),
      m_fFocalLength(f_focal_length),
      m_fRange(f_range),
      m_psAnchor(&s_anchor) {
      SetImagePxSize(n_width, n_height);
      Disable();
   }

   /****************************************/
   /****************************************/

   void CPerspectiveCameraEquippedEntity::Init(TConfigurationNode& t_tree) {
      try {
         CEntity::Init(t_tree);
         CDegrees cApertureDeg;
         GetNodeAttribute(t_tree, "aperture", cApertureDeg);
         m_cAperture = ToRadians(cApertureDeg);
         GetNodeAttribute(t_tree, "focal_length", m_fFocalLength);
         GetNodeAttribute(t_tree, "range", m_fRange);
         GetNodeAttribute(t_tree, "width", m_nImagePxWidth);
         GetNodeAttribute(t_tree, "height", m_nImagePxHeight);
         SetImagePxSize(m_nImagePxWidth, m_nImagePxHeight);
         std::string strAnchorId;
         GetNodeAttribute(t_tree, "anchor", strAnchorId);
         /*
          * NOTE: here we get a reference to the embodied entity
          * This line works under the assumption that:
          * 1. the PerspectiveCameraEquippedEntity has a parent;
          * 2. the parent has a child whose id is "body"
          * 3. the "body" is an embodied entity
          * If any of the above is false, this line will bomb out.
          */
         CEmbodiedEntity& cBody = GetParent().GetComponent<CEmbodiedEntity>("body");
         m_psAnchor = &cBody.GetAnchor(strAnchorId);
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Error initializing the perspective camera equipped entity \"" << GetId() << "\"", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CPerspectiveCameraEquippedEntity::Enable() {
      if (m_psAnchor) {
         m_psAnchor->Enable();
      }
   }

   /****************************************/
   /****************************************/

   void CPerspectiveCameraEquippedEntity::Disable() {
      if (m_psAnchor) {
         m_psAnchor->Disable();
      }
   }

   /****************************************/
   /****************************************/

   REGISTER_STANDARD_SPACE_OPERATIONS_ON_ENTITY(CPerspectiveCameraEquippedEntity);

   /****************************************/
   /****************************************/

}
