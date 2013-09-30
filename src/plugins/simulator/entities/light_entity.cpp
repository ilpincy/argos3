/**
 * @file <argos3/plugins/simulator/entities/light_entity.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "light_entity.h"
#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/plugins/simulator/media/led_medium.h>

namespace argos {

   /****************************************/
   /****************************************/

   CLightEntity::CLightEntity() :
      CLEDEntity(NULL),
      m_fIntensity(1.0f) {}
      
   /****************************************/
   /****************************************/

   CLightEntity::CLightEntity(const std::string& str_id,
                              const CVector3& c_position,
                              const CColor& c_color,
                              Real f_intensity) :
      CLEDEntity(NULL,
                 str_id,
                 c_position,
                 c_color),
      m_fIntensity(f_intensity) {}

   /****************************************/
   /****************************************/

   void CLightEntity::Init(TConfigurationNode& t_tree) {
      try {
         /* Init parent */
         CLEDEntity::Init(t_tree);
         /* Parse XML */
         GetNodeAttribute(t_tree, "intensity", m_fIntensity);
         std::string strMedium;
         GetNodeAttribute(t_tree, "medium", strMedium);
         CLEDMedium& cLEDMedium = CSimulator::GetInstance().GetMedium<CLEDMedium>(strMedium);
         cLEDMedium.AddEntity(*this);
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Error while initializing light entity", ex);
      }
   }

   /****************************************/
   /****************************************/

   REGISTER_ENTITY(CLightEntity,
                   "light",
                   "Carlo Pinciroli [ilpincy@gmail.com]",
                   "1.0",
                   "A colored light.",
                   "The light entity is an entity that emits a light detectable by a robot camera\n"
                   "(as a normal LED) or by light sensors. A light is bodyless, therefore it must\n"
                   "not be added to physics engines.\n"
                   "A light is characterized by a color and an intensity. The color of the light\n"
                   "is perceived by cameras. The intensity modifies the reading of the light\n"
                   "sensors. The higher the intensity, the closer the light is perceived.\n\n"
                   "REQUIRED XML CONFIGURATION\n\n"
                   "  <arena ...>\n"
                   "    ...\n"
                   "    <light id=\"light0\"\n"
                   "           position=\"0.4,2.3,0.25\"\n"
                   "           orientation=\"0,0,0\"\n"
                   "           color=\"yellow\"\n"
                   "           intensity=\"1.0\"\n"
                   "           medium=\"leds\"/>\n"
                   "    ...\n"
                   "  </arena>\n\n"
                   "The 'id' attribute is necessary and must be unique among the entities. If two\n"
                   "entities share the same id, initialization aborts.\n"
                   "The 'position' attribute specifies the position of the center of the light.\n"
                   "The attribute values are in the X,Y,Z order.\n"
                   "The 'orientation' attribute specifies the orientation of the light. At the\n"
                   "moment this attribute is mandatory but its value is ignored. In the future,\n"
                   "it will be used to implement a directional light.\n"
                   "The 'color' attribute specifies the color of the light.\n"
                   "The 'intensity' attribute sets the intensity of the light. When the value is\n"
                   "1.0, the light emits a normal amount of light. When it is 0.5 the amount of\n"
                   "light is half, and when the value is 2.0 the emission is doubled. The\n"
                   "intensity of the light affects the readings of the light sensors but not\n"
                   "those of the cameras.\n"
                   "The 'medium' attribute is used to add the light the corresponding LED medium.\n\n"
                   "OPTIONAL XML CONFIGURATION\n\n"
                   "None.\n",
                   "Usable"
      );

   /****************************************/
   /****************************************/

   REGISTER_STANDARD_SPACE_OPERATIONS_ON_ENTITY(CLightEntity);

   /****************************************/
   /****************************************/

}
