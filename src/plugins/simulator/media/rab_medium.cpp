#include "rab_medium.h"
#include <argos3/core/utility/configuration/argos_exception.h>
#include <argos3/core/utility/logging/argos_log.h>

namespace argos {

   /****************************************/
   /****************************************/

   CRABMedium::CRABMedium() {
   }

   /****************************************/
   /****************************************/

   CRABMedium::~CRABMedium() {
   }

   /****************************************/
   /****************************************/

   void CRABMedium::Init(TConfigurationNode& t_tree) {
      try {
         CMedium::Init(t_tree);
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Error in initialization of the range-and-bearing medium", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CRABMedium::Reset() {
   }

   /****************************************/
   /****************************************/

   void CRABMedium::Destroy() {
   }

   /****************************************/
   /****************************************/

   void CRABMedium::Update() {
      LOGERR << "[DEBUG] CRABMedium::Update()" << std::endl;
   }

   /****************************************/
   /****************************************/

   REGISTER_MEDIUM(CRABMedium,
                   "range_and_bearing",
                   "Carlo Pinciroli [ilpincy@gmail.com]",
                   "1.0",
                   "It simulates the communication across range-and-bearing-equipped robots.",
                   "This entity is required to simulate communication across range-and-bearing-\n"
                   "equipped robots. You need to add it to the <media> section every time you add\n"
                   "a range-and-bearing-equipped entity whose controller has a range-and-bearing\n"
                   "device activated.\n\n"
                   "REQUIRED XML CONFIGURATION\n\n"
                   "<range_and_bearing id=\"rab\" />\n\n"
                   "OPTIONAL XML CONFIGURATION\n\n"
                   "None for the time being\n",
                   "Under development"
      );

   /****************************************/
   /****************************************/

}
