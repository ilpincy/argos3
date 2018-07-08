#ifndef TEST_MEDIA_ENABLEDISABLE_LOOPFUNCTIONS_H
#define TEST_MEDIA_ENABLEDISABLE_LOOPFUNCTIONS_H

#include <argos3/core/simulator/loop_functions.h>
#include <argos3/plugins/robots/foot-bot/simulator/footbot_entity.h>

using namespace argos;

class CTestMediaEnableDisableLoopFunctions : public CLoopFunctions {

public:

   CTestMediaEnableDisableLoopFunctions();
   virtual ~CTestMediaEnableDisableLoopFunctions();
   virtual void Init(TConfigurationNode& t_tree);
   virtual void Reset();
   virtual void Destroy();
   virtual void PostStep();

private:

   CFootBotEntity* m_pcFBs[4];

};

#endif
