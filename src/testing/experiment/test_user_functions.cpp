#include "test_user_functions.h"
#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/plugins/simulator/entities/led_equipped_entity.h>
#include <argos3/plugins/simulator/entities/led_entity.h>

/********************************************************************************/
/********************************************************************************/

CTestUserFunctions::CTestUserFunctions() {
   RegisterUserFunction<CTestUserFunctions,CFootBotEntity>(&CTestUserFunctions::Draw);
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
      for(CLEDEquippedEntity::SActuator::TList::iterator it = cLEDs.GetLEDs().begin();
          it != cLEDs.GetLEDs().end();
          ++it) {
         DrawPoint((*it)->LED.GetPosition(), (*it)->LED.GetColor(), 50.0f);
      }
   }
}

/********************************************************************************/
/********************************************************************************/

void CTestUserFunctions::Draw(CFootBotEntity& c_footbot) {
   fprintf(stderr, "[DEBUG] FOOTBOT HERE\n");
}

/********************************************************************************/
/********************************************************************************/

//REGISTER_QTOPENGL_USER_FUNCTIONS(CTestUserFunctions, "test_user_functions");
