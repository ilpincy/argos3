#include "test_user_functions.h"
#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/plugins/robots/foot-bot/simulator/footbot_entity.h>
#include <argos3/plugins/simulator/entities/led_equipped_entity.h>
#include <argos3/plugins/simulator/entities/led_entity.h>

/********************************************************************************/
/********************************************************************************/

CTestUserFunctions::CTestUserFunctions() {
}

/********************************************************************************/
/********************************************************************************/

CTestUserFunctions::~CTestUserFunctions() {
}

/********************************************************************************/
/********************************************************************************/

void CTestUserFunctions::DrawInWorld() {
   CSpace& cSpace = CSimulator::GetInstance().GetSpace();
   CSpace::TMapPerType& tFBMap = cSpace.GetEntitiesByType("foot-bot");
   for(CSpace::TMapPerType::iterator it = tFBMap.begin();
       it != tFBMap.end();
       ++it) {
      CFootBotEntity& cFB = *any_cast<CFootBotEntity*>(it->second);
      CLEDEquippedEntity& cLEDs = cFB.GetLEDEquippedEntity();
      for(CLEDEntity::TList::iterator it = cLEDs.GetAllLEDs().begin();
          it != cLEDs.GetAllLEDs().end();
          ++it) {
         DrawPoint((*it)->GetPosition(), (*it)->GetColor(), 50.0f);
      }
   }
}

/********************************************************************************/
/********************************************************************************/

REGISTER_QTOPENGL_USER_FUNCTIONS(CTestUserFunctions, "test_user_functions");
