#ifndef TEST_USER_FUNCTIONS_H
#define TEST_USER_FUNCTIONS_H

#include <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_user_functions.h>
#include <argos3/plugins/robots/foot-bot/simulator/footbot_entity.h>

using namespace argos;

class CTestUserFunctions : public CQTOpenGLUserFunctions {

public:

   CTestUserFunctions();
   virtual ~CTestUserFunctions();

   virtual void DrawInWorld();

   using CQTOpenGLUserFunctions::Draw;
   void Draw(CFootBotEntity& c_footbot);

};

#endif
