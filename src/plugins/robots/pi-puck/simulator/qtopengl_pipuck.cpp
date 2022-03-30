/**
 * @file <argos3/plugins/robots/pipuck/simulator/qtopengl_pipuck.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "qtopengl_pipuck.h"

#include <argos3/core/utility/datatypes/color.h>
#include <argos3/core/utility/math/vector2.h>
#include <argos3/plugins/robots/pi-puck/simulator/pipuck_entity.h>
#include <argos3/plugins/simulator/entities/directional_led_equipped_entity.h>
#include <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_widget.h>

#include <array>

#define BODY_LED_INDEX 8
#define FRONT_LED_INDEX 9

namespace argos {

   /****************************************/
   /****************************************/

    CQTOpenGLPiPuck::CQTOpenGLPiPuck() :
      /* create the model */
      m_cPiPuckModel("pipuck.obj"),
      m_cPiPuckWheelModel("pipuck-wheel.obj"),
      m_arrRingLEDs {
         &m_cPiPuckModel.GetMaterial("Red-Ring-LED-0"),
         &m_cPiPuckModel.GetMaterial("Red-Ring-LED-1"),
         &m_cPiPuckModel.GetMaterial("Red-Ring-LED-2"),
         &m_cPiPuckModel.GetMaterial("Red-Ring-LED-3"),
         &m_cPiPuckModel.GetMaterial("Red-Ring-LED-4"),
         &m_cPiPuckModel.GetMaterial("Red-Ring-LED-5"),
         &m_cPiPuckModel.GetMaterial("Red-Ring-LED-6"),
         &m_cPiPuckModel.GetMaterial("Red-Ring-LED-7"),
      },
      m_sBodyLED(m_cPiPuckModel.GetMaterial("Green-Body-LEDs")),
      m_sFrontLED(m_cPiPuckModel.GetMaterial("Front-facing-Red-LED")) {
   }

   /****************************************/
   /****************************************/

   CQTOpenGLPiPuck::~CQTOpenGLPiPuck() {}

   /****************************************/
   /****************************************/

   void CQTOpenGLPiPuck::Draw(const CPiPuckEntity& c_entity) {
      /* Update LED materials */
      const CDirectionalLEDEquippedEntity& c_leds = c_entity.GetDirectionalLEDEquippedEntity();
      for(UInt32 un_ring_led_index = 0;
          un_ring_led_index < 8;
          un_ring_led_index++) {
         if(c_leds.GetLED(un_ring_led_index).GetColor() == CColor::BLACK) {
            m_arrRingLEDs[un_ring_led_index]->Diffuse = m_arrRingLedOffAmbientDiffuse;
            m_arrRingLEDs[un_ring_led_index]->Ambient = m_arrRingLedOffAmbientDiffuse;
         }
         else {
            m_arrRingLEDs[un_ring_led_index]->Diffuse = m_arrRingLedOnAmbientDiffuse;
            m_arrRingLEDs[un_ring_led_index]->Ambient = m_arrRingLedOnAmbientDiffuse;
         }
      }
      if(c_leds.GetLED(BODY_LED_INDEX).GetColor() == CColor::BLACK) {
         m_sBodyLED.Diffuse = m_arrBodyLedOffAmbientDiffuse;
         m_sBodyLED.Ambient = m_arrBodyLedOffAmbientDiffuse;
      }
      else {
         m_sBodyLED.Diffuse = m_arrBodyLedOnAmbientDiffuse;
         m_sBodyLED.Ambient = m_arrBodyLedOnAmbientDiffuse;
      }
      if(c_leds.GetLED(FRONT_LED_INDEX).GetColor() == CColor::BLACK) {
         m_sFrontLED.Diffuse = m_arrFrontLedOffAmbientDiffuse;
         m_sFrontLED.Ambient = m_arrFrontLedOffAmbientDiffuse;
      }
      else {
         m_sFrontLED.Diffuse = m_arrFrontLedOnAmbientDiffuse;
         m_sFrontLED.Ambient = m_arrFrontLedOnAmbientDiffuse;
      }
      CRadians cZAngle, cYAngle, cXAngle;
      /* draw the body */
      const SAnchor& sBodyAnchor = c_entity.GetEmbodiedEntity().GetAnchor("body");
      sBodyAnchor.Orientation.ToEulerAngles(cZAngle, cYAngle, cXAngle);
      glPushMatrix();
      glTranslatef(sBodyAnchor.Position.GetX(),
                   sBodyAnchor.Position.GetY(),
                   sBodyAnchor.Position.GetZ());
      glRotatef(ToDegrees(cXAngle).GetValue(), 1.0f, 0.0f, 0.0f);
      glRotatef(ToDegrees(cYAngle).GetValue(), 0.0f, 1.0f, 0.0f);
      glRotatef(ToDegrees(cZAngle).GetValue(), 0.0f, 0.0f, 1.0f);
      m_cPiPuckModel.Draw();
      glPopMatrix();
      /* draw the left wheel */
      const SAnchor& sLeftWheelAnchor = c_entity.GetEmbodiedEntity().GetAnchor("left_wheel");
      sLeftWheelAnchor.Orientation.ToEulerAngles(cZAngle, cYAngle, cXAngle);
      glPushMatrix();
      glTranslatef(sLeftWheelAnchor.Position.GetX(),
                   sLeftWheelAnchor.Position.GetY(),
                   sLeftWheelAnchor.Position.GetZ());
      glRotatef(ToDegrees(cXAngle).GetValue(), 1.0f, 0.0f, 0.0f);
      glRotatef(ToDegrees(cYAngle).GetValue(), 0.0f, 1.0f, 0.0f);
      glRotatef(ToDegrees(cZAngle).GetValue(), 0.0f, 0.0f, 1.0f);
      m_cPiPuckWheelModel.Draw();
      glPopMatrix();
      /* draw the right wheel */
      const SAnchor& sRightWheelAnchor = c_entity.GetEmbodiedEntity().GetAnchor("right_wheel");
      sRightWheelAnchor.Orientation.ToEulerAngles(cZAngle, cYAngle, cXAngle);
      glPushMatrix();
      glTranslatef(sRightWheelAnchor.Position.GetX(),
                   sRightWheelAnchor.Position.GetY(),
                   sRightWheelAnchor.Position.GetZ());
      glRotatef(ToDegrees(cXAngle).GetValue(), 1.0f, 0.0f, 0.0f);
      glRotatef(ToDegrees(cYAngle).GetValue(), 0.0f, 1.0f, 0.0f);
      glRotatef(ToDegrees(cZAngle).GetValue(), 0.0f, 0.0f, 1.0f);
      m_cPiPuckWheelModel.Draw();
      glPopMatrix();
   }

   /****************************************/
   /****************************************/

   class CQTOpenGLOperationDrawPiPuckNormal : public CQTOpenGLOperationDrawNormal {
   public:
      void ApplyTo(CQTOpenGLWidget& c_visualization,
                   CPiPuckEntity& c_entity) {
         static CQTOpenGLPiPuck m_cModel;
         m_cModel.Draw(c_entity);
         c_visualization.DrawRays(c_entity.GetControllableEntity());
      }
   };

   /****************************************/
   /****************************************/

   class CQTOpenGLOperationDrawPiPuckSelected : public CQTOpenGLOperationDrawSelected {
   public:
      void ApplyTo(CQTOpenGLWidget& c_visualization,
                   CPiPuckEntity& c_entity) {
         c_visualization.DrawBoundingBox(c_entity.GetEmbodiedEntity());
      }
   };

   /****************************************/
   /****************************************/

   REGISTER_QTOPENGL_ENTITY_OPERATION(CQTOpenGLOperationDrawNormal, CQTOpenGLOperationDrawPiPuckNormal, CPiPuckEntity);

   REGISTER_QTOPENGL_ENTITY_OPERATION(CQTOpenGLOperationDrawSelected, CQTOpenGLOperationDrawPiPuckSelected, CPiPuckEntity);

   /****************************************/
   /****************************************/

   const std::array<GLfloat, 4> CQTOpenGLPiPuck::m_arrRingLedOffAmbientDiffuse  {0.75f, 0.75f, 0.75f, 0.8f};
   const std::array<GLfloat, 4> CQTOpenGLPiPuck::m_arrRingLedOnAmbientDiffuse   {1.00f, 0.00f, 0.00f, 0.8f};
   const std::array<GLfloat, 4> CQTOpenGLPiPuck::m_arrBodyLedOffAmbientDiffuse  {0.75f, 0.75f, 0.75f, 0.7f};
   const std::array<GLfloat, 4> CQTOpenGLPiPuck::m_arrBodyLedOnAmbientDiffuse   {0.27f, 0.83f, 0.27f, 0.7f};
   const std::array<GLfloat, 4> CQTOpenGLPiPuck::m_arrFrontLedOffAmbientDiffuse {0.75f, 0.75f, 0.75f, 0.8f};
   const std::array<GLfloat, 4> CQTOpenGLPiPuck::m_arrFrontLedOnAmbientDiffuse  {1.00f, 0.00f, 0.00f, 0.8f};

   /****************************************/
   /****************************************/

}
