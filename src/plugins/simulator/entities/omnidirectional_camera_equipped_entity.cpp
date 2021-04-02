#include "omnidirectional_camera_equipped_entity.h"
#include <argos3/core/simulator/space/space.h>

namespace argos {

   /****************************************/
   /****************************************/

   COmnidirectionalCameraEquippedEntity::COmnidirectionalCameraEquippedEntity(CComposableEntity* pc_parent) :
      CEntity(pc_parent) {
      Disable();
   }

   /****************************************/
   /****************************************/

   COmnidirectionalCameraEquippedEntity::COmnidirectionalCameraEquippedEntity(CComposableEntity* pc_parent,
                                                                              const std::string& str_id,
                                                                              const CRadians& c_aperture,
                                                                              const CVector3& c_offset) :
      CEntity(pc_parent, str_id),
      m_cAperture(c_aperture),
      m_cOffset(c_offset) {
      Disable();
   }

   /****************************************/
   /****************************************/

   void COmnidirectionalCameraEquippedEntity::Init(TConfigurationNode& t_tree) {
      try {
         CEntity::Init(t_tree);
         GetNodeAttributeOrDefault(t_tree, "aperture", m_cAperture, m_cAperture);
         GetNodeAttributeOrDefault(t_tree, "offset", m_cOffset, m_cOffset);
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Error initializing the omnidirectional camera equipped entity \"" << GetId() << "\"", ex);
      }
   }

   /****************************************/
   /****************************************/

   std::string COmnidirectionalCameraEquippedEntity::GetQueryDocumentation(const SDocumentationQuerySpec& c_spec) {
     std::string strDoc =
         "You can configure the parameters of the omnidirectional camera. The following\n"
         "parameters are available:\n\n"

         "- 'aperture' - The aperture of the omndirectional camera. Defaults to " + std::to_string(c_spec.cApertureDefault.GetValue()) + "\n"
         "               if omitted. Units of degrees.\n\n"
         "- 'offset' - The offset in Z from the center of the robot to place the tip\n"
         "             of the omnidirectional camera (i.e., at (0.0, 0.0, 'offset')\n"
         "             from the center of the " + c_spec.strEntityName + "). Defaults to " + std::to_string(c_spec.fOffsetDefault) + " if omitted.\n"
         "             Units of meters.\n\n"

         "The camera range on the ground is calculated according to r=offset*tan(aperture);\n"
         "with the default values this gives a range of " + std::to_string(c_spec.fOffsetDefault * Tan(argos::ToRadians(c_spec.cApertureDefault))) + " "
         "meters.\n\n";

     std::string strExamples =
         "Example XML Omndirectional camera configurations (default values shown):\n\n"

         "  <arena>\n"
         "    ...\n"
         "    <" + c_spec.strEntityName + " ...>\n"
         "      ...\n"
         "      <omnidirectional_camera aperture=\"" + std::to_string(c_spec.cApertureDefault.GetValue()) + "\"\n"
         "                              offset=\""+ std::to_string(c_spec.fOffsetDefault)  + "\"/>\n"
         "      ...\n"
         "    </" + c_spec.strEntityName + ">\n"
         "    ...\n"
         "  </arena>\n\n";
     return strDoc + strExamples;
   }

   /****************************************/
   /****************************************/

   REGISTER_STANDARD_SPACE_OPERATIONS_ON_ENTITY(COmnidirectionalCameraEquippedEntity);

   /****************************************/
   /****************************************/

}
