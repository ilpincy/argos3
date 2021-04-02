#include "perspective_camera_equipped_entity.h"
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/entity/composable_entity.h>

namespace argos {

   /****************************************/
   /****************************************/

   CPerspectiveCameraEquippedEntity::CPerspectiveCameraEquippedEntity(CComposableEntity* pc_parent) :
      CEntity(pc_parent),
      m_eLookDir(ELookDirection::LOOKS_FRONT),
      m_fFocalLength(0.0f),
      m_fRange(0.0f),
      m_nImagePxWidth(0),
      m_nImagePxHeight(0),
      m_psAnchor(nullptr) {
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
                                                                      SAnchor* s_anchor) :
      CEntity(pc_parent, str_id),
      m_eLookDir(ELookDirection::LOOKS_FRONT),
      m_cAperture(c_aperture),
      m_fFocalLength(f_focal_length),
      m_fRange(f_range),
      m_psAnchor(s_anchor) {
      SetImagePxSize(n_width, n_height);
      Disable();
   }

   /****************************************/
   /****************************************/

   void CPerspectiveCameraEquippedEntity::Init(TConfigurationNode& t_tree) {
      try {
         CEntity::Init(t_tree);
         std::string strOrientationDir = "front";
         GetNodeAttributeOrDefault(t_tree, "looks", strOrientationDir, strOrientationDir);
         if ("front" == strOrientationDir) {
           m_eLookDir = LOOKS_FRONT;
         } else if ("up" == strOrientationDir) {
           m_eLookDir = LOOKS_UP;
         } else if ("down" == strOrientationDir) {
           m_eLookDir = LOOKS_DOWN;
         } else {
           THROW_ARGOSEXCEPTION("Bad look direction specified for perspective camera");
         }
         CDegrees cApertureDeg;
         GetNodeAttribute(t_tree, "aperture", cApertureDeg);
         m_cAperture = ToRadians(cApertureDeg);
         GetNodeAttributeOrDefault(t_tree, "focal_length", m_fFocalLength, m_fFocalLength);
         GetNodeAttributeOrDefault(t_tree, "range", m_fRange, m_fRange);
         GetNodeAttributeOrDefault(t_tree, "image_width", m_nImagePxWidth, m_nImagePxWidth);
         GetNodeAttributeOrDefault(t_tree, "image_height", m_nImagePxHeight, m_nImagePxHeight);
         SetImagePxSize(m_nImagePxWidth, m_nImagePxHeight);
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
   std::string CPerspectiveCameraEquippedEntity::GetQueryDocumentation(const SDocumentationQuerySpec& c_spec) {
        std::string strDoc =
            "You can configure the parameters of the " + c_spec.strEntityName + " perspective camera. "
            "The following\nparameters are available:\n\n";

            if (c_spec.bCanSetLooksAt) {
              strDoc += "- 'looks' - Sets the direction the camera looks at. Can be 'front', 'up', 'down'.\n"
                        "            Which of these make sense depends on your robot; invalid configurations\n"
                        "            (such as setting 'down' on a ground robot) may be ignored. Looking 'up' on ground\n"
                        "            robots can be useful to see the eye-bot LEDs. Defaults to '"  +
                        c_spec.strLooksAtDefault + "' if omitted.\n\n";
            }

            strDoc += "- 'aperture' - The camera aperture (units of degrees). Defaults to " +
            std::to_string(c_spec.cApertureDefault.GetValue()) + " if omitted.\n\n"

            "- 'focal_length' - The camera focal length (used to determine height of the image in meters).\n"
            "                   Units of meters. Defaults to " + std::to_string(c_spec.fFocalLengthDefault) + " if omitted.\n\n"

            "- 'range' - The camera range (objects further away than this will not be detected\n"
            "            by the camera). Units of meters. Defaults to " + std::to_string(c_spec.fRangeDefault) + " if omitted.\n\n"

            "- 'image_height' - The height of the resulting camera image (units of pixels).\n" +
            "                   Defaults to " + std::to_string(c_spec.nImageHeightDefault) + " if omitted.\n\n"

            "- 'image_width' - The width of the resulting camera image (units of pixels).\n"
            "                  Defaults to " + std::to_string(c_spec.nImageWidthDefault) + " if omitted.\n\n";

            if (!c_spec.bCanSetLooksAt) {
              strDoc += "The 'looks' parameter cannot be configured for the " + c_spec.strEntityName +
                        " and is set to '" + c_spec.strLooksAtDefault + "'.\n\n";
            }

            std::string strExamples =
            "Example XML Perspective camera configurations (default values shown):\n\n"

            "  <arena>\n"
            "    ...\n"
            "    <" + c_spec.strEntityName + " ...>\n"
            "      ...\n"
                "      <perspective_camera looks='" + c_spec.strLooksAtDefault + "'\n"
            "                          aperture=\"" + std::to_string(c_spec.cApertureDefault.GetValue()) + "\"\n"
            "                          focal_length=\""+ std::to_string(c_spec.fFocalLengthDefault)  + "\"\n"
            "                          range=\""+ std::to_string(c_spec.fRangeDefault)  + "\"\n"
            "                          image_height=\""+ std::to_string(c_spec.nImageHeightDefault)  + "\"\n"
            "                          image_width=\""+ std::to_string(c_spec.nImageWidthDefault)  + "\"/>\n"
            "      ...\n"
            "    </" + c_spec.strEntityName + ">\n"
            "    ...\n"
            "  </arena>\n\n";
            return strDoc + strExamples;
      }

   /****************************************/
   /****************************************/

   REGISTER_STANDARD_SPACE_OPERATIONS_ON_ENTITY(CPerspectiveCameraEquippedEntity);

   /****************************************/
   /****************************************/

}
