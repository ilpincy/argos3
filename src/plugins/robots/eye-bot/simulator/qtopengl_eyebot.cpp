/**
 * @file <argos3/plugins/robots/eye-bot/simulator/qtopengl_eyebot.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "qtopengl_eyebot.h"
#include "eyebot_entity.h"
#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/core/utility/math/vector2.h>
#include <argos3/core/utility/math/vector3.h>
#include <argos3/plugins/simulator/entities/led_equipped_entity.h>
#include <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_widget.h>

namespace argos {

   /****************************************/
   /****************************************/

   /* All measures are in meters */

   /* Legs */
   static const Real LEG_HEIGHT         = 0.166f;
   static const Real LEG_WIDTH          = 0.02f;
   static const Real LEG_HALF_WIDTH     = LEG_WIDTH * 0.5f;
   static const Real LEG_DEPTH          = 0.005f;
   static const Real LEG_HALF_DEPTH     = LEG_DEPTH * 0.5f;

   /* Ring */
   static const Real RING_OUTER_RADIUS  = 0.25f;
   static const Real RING_INNER_RADIUS  = 0.237f;
   static const Real RING_HEIGHT        = 0.015f;
   static const Real RING_ELEVATION     = LEG_HEIGHT - RING_HEIGHT;

   /* Led */
   static const Real LED_HEIGHT               = 0.01f;
   static const Real LED_SIDE                 = RING_HEIGHT;
   static const Real LED_HALF_SIDE            = LED_SIDE * 0.5f;
   static const Real LED_SIDE_RING_ELEVATION = RING_ELEVATION + LED_HALF_SIDE;
   static const Real LED_BOTTOM_RING_ELEVATION = RING_ELEVATION - LED_HEIGHT;
   static const Real LED_BOTTOM_RING_DISTANCE  = RING_OUTER_RADIUS - LEG_HALF_WIDTH;

   /* Main rod */
   static const Real ROD_SIDE           = 0.07f;
   static const Real ROD_HALF_SIDE      = ROD_SIDE * 0.5f;
   static const Real ROD_HEIGHT         = 0.4f;
   static const Real ROD_ELEVATION      = LEG_HEIGHT;

   /****************************************/
   /****************************************/

   CQTOpenGLEyeBot::CQTOpenGLEyeBot() :
      m_unVertices(40),
      m_fLEDAngleSlice(360.0f / 16.0f) {
      /* Reserve the needed display lists */
      m_unLists = glGenLists(4);

      /* Assign indices for better referencing (later) */
      m_unLegList      = m_unLists;
      m_unBodyList     = m_unLists + 1;
      m_unSideLEDList = m_unLists + 2;
      m_unBottomLEDList = m_unLists + 3;

      /* Create the led display list */
      glNewList(m_unLegList, GL_COMPILE);
      MakeLeg();
      glEndList();

      /* Create the body display list */
      glNewList(m_unBodyList, GL_COMPILE);
      DrawBody();
      glEndList();

      /* Create the side LED display list */
      glNewList(m_unSideLEDList, GL_COMPILE);
      DrawSideLED();
      glEndList();

      /* Create the bottom LED display list */
      glNewList(m_unBottomLEDList, GL_COMPILE);
      DrawBottomLED();
      glEndList();
   }

   /****************************************/
   /****************************************/

   CQTOpenGLEyeBot::~CQTOpenGLEyeBot() {
      glDeleteLists(m_unLists, 4);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLEyeBot::Draw(CEyeBotEntity& c_entity) {
      SetBodyMaterial();
      /* Place the legs */
      glPushMatrix();
      glRotatef(45.0f, 0.0f, 0.0f, 1.0f);
      glTranslatef(RING_OUTER_RADIUS, 0.0f, 0.0f);
      glCallList(m_unLegList);
      glPopMatrix();
      glPushMatrix();
      glRotatef(135.0f, 0.0f, 0.0f, 1.0f);
      glTranslatef(RING_OUTER_RADIUS, 0.0f, 0.0f);
      glCallList(m_unLegList);
      glPopMatrix();
      glPushMatrix();
      glRotatef(225.0f, 0.0f, 0.0f, 1.0f);
      glTranslatef(RING_OUTER_RADIUS, 0.0f, 0.0f);
      glCallList(m_unLegList);
      glPopMatrix();
      glPushMatrix();
      glRotatef(315.0f, 0.0f, 0.0f, 1.0f);
      glTranslatef(RING_OUTER_RADIUS, 0.0f, 0.0f);
      glCallList(m_unLegList);
      glPopMatrix();
      /* Place the body */
      glCallList(m_unBodyList);
      /* Place the LEDs */
      glPushMatrix();
      CLEDEquippedEntity& cLEDEquippedEntity = c_entity.GetLEDEquippedEntity();
      glRotatef(m_fLEDAngleSlice * 0.5f, 0.0f, 0.0f, 1.0f);
      const CColor& cUpColor = cLEDEquippedEntity.GetLED(0).GetColor();
      const CColor& cLowColor = cLEDEquippedEntity.GetLED(16).GetColor();
      SetLEDMaterial(cUpColor.GetRed(),
                     cUpColor.GetGreen(),
                     cUpColor.GetBlue());
      glCallList(m_unSideLEDList);
      SetLEDMaterial(cLowColor.GetRed(),
                     cLowColor.GetGreen(),
                     cLowColor.GetBlue());
      glCallList(m_unBottomLEDList);
      for(UInt32 i = 1; i < 16; i++) {
         glRotatef(m_fLEDAngleSlice, 0.0f, 0.0f, 1.0f);
         const CColor& cUpColor = cLEDEquippedEntity.GetLED(i).GetColor();
         const CColor& cLowColor = cLEDEquippedEntity.GetLED(i+16).GetColor();
         SetLEDMaterial(cUpColor.GetRed(),
                        cUpColor.GetGreen(),
                        cUpColor.GetBlue());
         glCallList(m_unSideLEDList);
         SetLEDMaterial(cLowColor.GetRed(),
                        cLowColor.GetGreen(),
                        cLowColor.GetBlue());
         glCallList(m_unBottomLEDList);
      }
      glRotatef(135.0f, 0.0f, 0.0f, 1.0f);
      glTranslatef(-0.3*RING_OUTER_RADIUS, 0.0f, 0.0f);
      const CColor& cBottomLedColor = cLEDEquippedEntity.GetLED(32).GetColor();
      SetLEDMaterial(cBottomLedColor.GetRed(),
                     cBottomLedColor.GetGreen(),
                     cBottomLedColor.GetBlue());
      glCallList(m_unBottomLEDList);
      glPopMatrix();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLEyeBot::SetLEDMaterial(GLfloat f_red, GLfloat f_green, GLfloat f_blue) {
      const GLfloat pfColor[]     = { f_red, f_green, f_blue, 1.0f };
      const GLfloat pfSpecular[]  = {  0.0f,    0.0f,   0.0f, 1.0f };
      const GLfloat pfShininess[] = {  0.0f                        };
      const GLfloat pfEmission[]  = { f_red, f_green, f_blue, 1.0f };
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, pfColor);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,            pfSpecular);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS,           pfShininess);
      glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION,            pfEmission);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLEyeBot::SetBodyMaterial() {
      const GLfloat pfColor[]     = {   0.25f, 0.25f, 0.25f, 1.0f };
      const GLfloat pfSpecular[]  = {   0.9f,  0.9f,  0.9f,  1.0f };
      const GLfloat pfShininess[] = { 100.0f                   };
      const GLfloat pfEmission[]  = {   0.0f, 0.0f, 0.0f, 1.0f };
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, pfColor);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,            pfSpecular);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS,           pfShininess);
      glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION,            pfEmission);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLEyeBot::MakeLeg() {
      /* Top face */
      glBegin(GL_QUADS);
      glNormal3f(0.0f, 0.0f, 1.0f);
      glVertex3f(-LEG_HALF_WIDTH, -LEG_HALF_DEPTH, LEG_HEIGHT);
      glVertex3f( LEG_HALF_WIDTH, -LEG_HALF_DEPTH, LEG_HEIGHT);
      glVertex3f( LEG_HALF_WIDTH,  LEG_HALF_DEPTH, LEG_HEIGHT);
      glVertex3f(-LEG_HALF_WIDTH,  LEG_HALF_DEPTH, LEG_HEIGHT);
      glEnd();
      /* Bottom face */
      glBegin(GL_QUADS);
      glNormal3f(0.0f, 0.0f, -1.0f);
      glVertex3f(-LEG_HALF_WIDTH, -LEG_HALF_DEPTH, 0.0f);
      glVertex3f(-LEG_HALF_WIDTH,  LEG_HALF_DEPTH, 0.0f);
      glVertex3f( LEG_HALF_WIDTH,  LEG_HALF_DEPTH, 0.0f);
      glVertex3f( LEG_HALF_WIDTH, -LEG_HALF_DEPTH, 0.0f);
      glEnd();
      /* This part covers the faces (South, East, North, West) */
      glBegin(GL_QUAD_STRIP);
      /* Starting vertex */
      glNormal3f(0.0f, -1.0f, 0.0f);
      glVertex3f(-LEG_HALF_WIDTH, -LEG_HALF_DEPTH, LEG_HEIGHT);
      glVertex3f(-LEG_HALF_WIDTH, -LEG_HALF_DEPTH, 0.0f);
      /* South face */
      glVertex3f( LEG_HALF_WIDTH, -LEG_HALF_DEPTH, LEG_HEIGHT);
      glVertex3f( LEG_HALF_WIDTH, -LEG_HALF_DEPTH, 0.0f);
      /* East face */
      glNormal3f(1.0f, 0.0f, 0.0f);
      glVertex3f( LEG_HALF_WIDTH,  LEG_HALF_DEPTH, LEG_HEIGHT);
      glVertex3f( LEG_HALF_WIDTH,  LEG_HALF_DEPTH, 0.0f);
      /* North face */
      glNormal3f(0.0f, 1.0f, 0.0f);
      glVertex3f(-LEG_HALF_WIDTH,  LEG_HALF_DEPTH, LEG_HEIGHT);
      glVertex3f(-LEG_HALF_WIDTH,  LEG_HALF_DEPTH, 0.0f);
      /* West face */
      glNormal3f(-1.0f, 0.0f, 0.0f);
      glVertex3f(-LEG_HALF_WIDTH, -LEG_HALF_DEPTH, LEG_HEIGHT);
      glVertex3f(-LEG_HALF_WIDTH, -LEG_HALF_DEPTH, 0.0f);
      glEnd();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLEyeBot::DrawSideLED() {
      /* Draw it as a tiny pyramid pointing to X
         (with no base, cause it's not visible anyway) */
      glBegin(GL_TRIANGLES);
      /* Top */
      glVertex3f(LED_HEIGHT + RING_OUTER_RADIUS,           0.0f, LED_SIDE_RING_ELEVATION                );
      glVertex3f(             RING_OUTER_RADIUS,  LED_HALF_SIDE, LED_SIDE_RING_ELEVATION + LED_HALF_SIDE);
      glVertex3f(             RING_OUTER_RADIUS, -LED_HALF_SIDE, LED_SIDE_RING_ELEVATION + LED_HALF_SIDE);
      /* Bottom */
      glVertex3f(LED_HEIGHT + RING_OUTER_RADIUS,           0.0f, LED_SIDE_RING_ELEVATION                );
      glVertex3f(             RING_OUTER_RADIUS, -LED_HALF_SIDE, LED_SIDE_RING_ELEVATION - LED_HALF_SIDE);
      glVertex3f(             RING_OUTER_RADIUS,  LED_HALF_SIDE, LED_SIDE_RING_ELEVATION - LED_HALF_SIDE);
      /* Left */
      glVertex3f(LED_HEIGHT + RING_OUTER_RADIUS,           0.0f, LED_SIDE_RING_ELEVATION                );
      glVertex3f(             RING_OUTER_RADIUS,  LED_HALF_SIDE, LED_SIDE_RING_ELEVATION - LED_HALF_SIDE);
      glVertex3f(             RING_OUTER_RADIUS,  LED_HALF_SIDE, LED_SIDE_RING_ELEVATION + LED_HALF_SIDE);
      /* Right */
      glVertex3f(LED_HEIGHT + RING_OUTER_RADIUS,           0.0f, LED_SIDE_RING_ELEVATION                );
      glVertex3f(             RING_OUTER_RADIUS, -LED_HALF_SIDE, LED_SIDE_RING_ELEVATION + LED_HALF_SIDE);
      glVertex3f(             RING_OUTER_RADIUS, -LED_HALF_SIDE, LED_SIDE_RING_ELEVATION - LED_HALF_SIDE);
      glEnd();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLEyeBot::DrawBottomLED() {
      /* Draw it as a tiny pyramid pointing to Z downwards
         (with no base, cause it's not visible anyway) */
      glBegin(GL_TRIANGLES);
      /* North */
      glVertex3f(LED_BOTTOM_RING_DISTANCE, 0.0f, LED_BOTTOM_RING_ELEVATION);
      glVertex3f(RING_OUTER_RADIUS,  LED_HALF_SIDE, RING_ELEVATION);
      glVertex3f(RING_OUTER_RADIUS, -LED_HALF_SIDE, RING_ELEVATION);
      /* South */
      glVertex3f(LED_BOTTOM_RING_DISTANCE, 0.0f, LED_BOTTOM_RING_ELEVATION);
      glVertex3f(RING_OUTER_RADIUS - LED_SIDE, -LED_HALF_SIDE, RING_ELEVATION);
      glVertex3f(RING_OUTER_RADIUS - LED_SIDE,  LED_HALF_SIDE, RING_ELEVATION);
      /* West */
      glVertex3f(LED_BOTTOM_RING_DISTANCE, 0.0f, LED_BOTTOM_RING_ELEVATION);
      glVertex3f(RING_OUTER_RADIUS - LED_SIDE, LED_HALF_SIDE, RING_ELEVATION);
      glVertex3f(RING_OUTER_RADIUS           , LED_HALF_SIDE, RING_ELEVATION);
      /* East */
      glVertex3f(LED_BOTTOM_RING_DISTANCE, 0.0f, LED_BOTTOM_RING_ELEVATION);
      glVertex3f(RING_OUTER_RADIUS           , -LED_HALF_SIDE, RING_ELEVATION);
      glVertex3f(RING_OUTER_RADIUS - LED_SIDE, -LED_HALF_SIDE, RING_ELEVATION);
      glEnd();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLEyeBot::DrawBody() {
      /* Draw the ring */
      CVector2 cVertex(RING_OUTER_RADIUS, 0.0f);
      CRadians cAngle(-CRadians::TWO_PI / m_unVertices);
      /* Bottom part */
      glBegin(GL_POLYGON);
      glNormal3f(0.0f, 0.0f, -1.0f);
      for(GLuint i = 0; i <= m_unVertices; i++) {
         glVertex3f(cVertex.GetX(), cVertex.GetY(), RING_ELEVATION);
         cVertex.Rotate(cAngle);
      }
      glEnd();
      /* Side surface */
      cAngle = -cAngle;
      CVector2 cNormal(1.0f, 0.0f);
      cVertex.Set(RING_OUTER_RADIUS, 0.0f);
      glBegin(GL_QUAD_STRIP);
      for(GLuint i = 0; i <= m_unVertices; i++) {
         glNormal3f(cNormal.GetX(), cNormal.GetY(), 0.0f);
         glVertex3f(cVertex.GetX(), cVertex.GetY(), RING_ELEVATION + RING_HEIGHT);
         glVertex3f(cVertex.GetX(), cVertex.GetY(), RING_ELEVATION);
         cVertex.Rotate(cAngle);
         cNormal.Rotate(cAngle);
      }
      glEnd();
      /* Top part */
      glBegin(GL_POLYGON);
      glNormal3f(0.0f, 0.0f, 1.0f);
      cVertex.Set(RING_OUTER_RADIUS, 0.0f);
      for(GLuint i = 0; i <= m_unVertices; i++) {
         glVertex3f(cVertex.GetX(), cVertex.GetY(), RING_ELEVATION + RING_HEIGHT);
         cVertex.Rotate(cAngle);
      }
      glEnd();
      /* Draw the rod */
      /* Top face */
      glBegin(GL_QUADS);
      glNormal3f(0.0f, 0.0f, 1.0f);
      glVertex3f(-ROD_HALF_SIDE, -ROD_HALF_SIDE, ROD_ELEVATION+ROD_HEIGHT);
      glVertex3f( ROD_HALF_SIDE, -ROD_HALF_SIDE, ROD_ELEVATION+ROD_HEIGHT);
      glVertex3f( ROD_HALF_SIDE,  ROD_HALF_SIDE, ROD_ELEVATION+ROD_HEIGHT);
      glVertex3f(-ROD_HALF_SIDE,  ROD_HALF_SIDE, ROD_ELEVATION+ROD_HEIGHT);
      glEnd();
      /* This part covers the faces (South, East, North, West) */
      glBegin(GL_QUAD_STRIP);
      /* Starting vertex */
      glNormal3f(0.0f, -1.0f, 0.0f);
      glVertex3f(-ROD_HALF_SIDE, -ROD_HALF_SIDE, ROD_ELEVATION+ROD_HEIGHT);
      glVertex3f(-ROD_HALF_SIDE, -ROD_HALF_SIDE, ROD_ELEVATION);
      /* South face */
      glVertex3f( ROD_HALF_SIDE, -ROD_HALF_SIDE, ROD_ELEVATION+ROD_HEIGHT);
      glVertex3f( ROD_HALF_SIDE, -ROD_HALF_SIDE, ROD_ELEVATION);
      /* East face */
      glNormal3f(1.0f, 0.0f, 0.0f);
      glVertex3f( ROD_HALF_SIDE,  ROD_HALF_SIDE, ROD_ELEVATION+ROD_HEIGHT);
      glVertex3f( ROD_HALF_SIDE,  ROD_HALF_SIDE, ROD_ELEVATION);
      /* North face */
      glNormal3f(0.0f, 1.0f, 0.0f);
      glVertex3f(-ROD_HALF_SIDE,  ROD_HALF_SIDE, ROD_ELEVATION+ROD_HEIGHT);
      glVertex3f(-ROD_HALF_SIDE,  ROD_HALF_SIDE, ROD_ELEVATION);
      /* West face */
      glNormal3f(-1.0f, 0.0f, 0.0f);
      glVertex3f(-ROD_HALF_SIDE, -ROD_HALF_SIDE, ROD_ELEVATION+ROD_HEIGHT);
      glVertex3f(-ROD_HALF_SIDE, -ROD_HALF_SIDE, ROD_ELEVATION);
      glEnd();
   }

   /****************************************/
   /****************************************/

   class CQTOpenGLOperationDrawEyeBotNormal : public CQTOpenGLOperationDrawNormal {
   public:
      void ApplyTo(CQTOpenGLWidget& c_visualization,
                   CEyeBotEntity& c_entity) {
         static CQTOpenGLEyeBot m_cModel;
         c_visualization.DrawRays(c_entity.GetControllableEntity());
         c_visualization.DrawEntity(c_entity.GetEmbodiedEntity());
         m_cModel.Draw(c_entity);
      }
   };

   class CQTOpenGLOperationDrawEyeBotSelected : public CQTOpenGLOperationDrawSelected {
   public:
      void ApplyTo(CQTOpenGLWidget& c_visualization,
                   CEyeBotEntity& c_entity) {
         c_visualization.DrawBoundingBox(c_entity.GetEmbodiedEntity());
      }
   };

   REGISTER_QTOPENGL_ENTITY_OPERATION(CQTOpenGLOperationDrawNormal, CQTOpenGLOperationDrawEyeBotNormal, CEyeBotEntity);

   REGISTER_QTOPENGL_ENTITY_OPERATION(CQTOpenGLOperationDrawSelected, CQTOpenGLOperationDrawEyeBotSelected, CEyeBotEntity);

   /****************************************/
   /****************************************/

}
