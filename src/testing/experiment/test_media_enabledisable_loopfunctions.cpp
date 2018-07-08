#include "test_media_enabledisable_loopfunctions.h"
#include <argos3/core/utility/logging/argos_log.h>

/****************************************/
/****************************************/

CTestMediaEnableDisableLoopFunctions::CTestMediaEnableDisableLoopFunctions() {
}

/****************************************/
/****************************************/

CTestMediaEnableDisableLoopFunctions::~CTestMediaEnableDisableLoopFunctions() {
}

/****************************************/
/****************************************/

void CTestMediaEnableDisableLoopFunctions::Init(TConfigurationNode& t_tree) {
   DEBUG_FUNCTION_ENTER;
   m_pcFBs[0] = &dynamic_cast<CFootBotEntity&>(GetSpace().GetEntity("fb0"));
   m_pcFBs[1] = &dynamic_cast<CFootBotEntity&>(GetSpace().GetEntity("fb1"));
   m_pcFBs[2] = new CFootBotEntity("fb2", "fsc", CVector3(0, -1, 0));
   m_pcFBs[3] = new CFootBotEntity("fb3", "fsc", CVector3(0,  1, 0));
   AddEntity(*m_pcFBs[2]);
   AddEntity(*m_pcFBs[3]);
   DEBUG_FUNCTION_EXIT;
}

/****************************************/
/****************************************/

void CTestMediaEnableDisableLoopFunctions::Reset() {
   DEBUG_FUNCTION_ENTER;
   DEBUG_FUNCTION_EXIT;
}

/****************************************/
/****************************************/

void CTestMediaEnableDisableLoopFunctions::Destroy() {
   DEBUG_FUNCTION_ENTER;
   DEBUG_FUNCTION_EXIT;
}

/****************************************/
/****************************************/

void CTestMediaEnableDisableLoopFunctions::PostStep() {
   DEBUG_FUNCTION_ENTER;
   if(GetSpace().GetSimulationClock() == 5) {
      DEBUG("**********\n");
      DEBUG("DISABLING ENTITIES\n");
      DEBUG("**********\n");
      m_pcFBs[0]->Disable();
      m_pcFBs[2]->Disable();
   }
   DEBUG_FUNCTION_EXIT;
}

/****************************************/
/****************************************/

REGISTER_LOOP_FUNCTIONS(CTestMediaEnableDisableLoopFunctions, "test_media_enabledisable_loopfunctions")
