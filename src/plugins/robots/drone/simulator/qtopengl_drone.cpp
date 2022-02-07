/**
 * @file <argos3/plugins/robots/drone/simulator/qtopengl_drone.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "qtopengl_drone.h"

#include <argos3/core/simulator/simulator.h>
#include <argos3/core/utility/datatypes/color.h>
#include <argos3/core/utility/math/vector2.h>
#include <argos3/plugins/robots/drone/simulator/drone_entity.h>
#include <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_widget.h>
#include <argos3/plugins/simulator/entities/directional_led_equipped_entity.h>

#include <array>

namespace argos {

   /****************************************/
   /****************************************/

    CQTOpenGLDrone::CQTOpenGLDrone() :
      /* create the model */
      m_cDroneModel("drone.obj"),
      m_cPropellerModel("propeller.obj"),
      /* get pointers to the LED materials */
      m_arrLEDs {
         &m_cDroneModel.GetMaterial("led_0"),
         &m_cDroneModel.GetMaterial("led_1"),
         &m_cDroneModel.GetMaterial("led_2"),
         &m_cDroneModel.GetMaterial("led_3")        
      } {}

   /****************************************/
   /****************************************/

   CQTOpenGLDrone::~CQTOpenGLDrone() {}

   /****************************************/
   /****************************************/

   void CQTOpenGLDrone::Draw(const CDroneEntity& c_entity) {
      /* Update LED materials */
      const CDirectionalLEDEquippedEntity& c_leds = c_entity.GetDirectionalLEDEquippedEntity();
      for(UInt32 un_material_idx = 0;
          un_material_idx < m_arrLEDs.size();
          un_material_idx++) {
         const CColor& cColor = c_leds.GetLED(un_material_idx).GetColor();
         std::array<GLfloat, 4> arrColor = {
            cColor.GetRed() / 255.0f,
            cColor.GetGreen() / 255.0f,
            cColor.GetBlue() / 255.0f,
            1.0f
         };
         m_arrLEDs[un_material_idx]->Emission = arrColor;
      }
      /* buffers for converting from quaternions to euler angles */
      CRadians cZAngle, cYAngle, cXAngle;
      /* draw the robot */
      const CVector3& cPosition = c_entity.GetEmbodiedEntity().GetOriginAnchor().Position;
      const CQuaternion& cOrientation = c_entity.GetEmbodiedEntity().GetOriginAnchor().Orientation;
      cOrientation.ToEulerAngles(cZAngle, cYAngle, cXAngle);
      glPushMatrix();
      glTranslatef(cPosition.GetX(), cPosition.GetY(), cPosition.GetZ());
      glRotatef(ToDegrees(cXAngle).GetValue(), 1.0f, 0.0f, 0.0f);
      glRotatef(ToDegrees(cYAngle).GetValue(), 0.0f, 1.0f, 0.0f);
      glRotatef(ToDegrees(cZAngle).GetValue(), 0.0f, 0.0f, 1.0f);
      m_cDroneModel.Draw();
      /* start of draw propellers */
      UInt32 unPropellerAngle = 
         (CSimulator::GetInstance().GetSpace().GetSimulationClock() * 10) % 360;
      /* (1, 1) */
      glPushMatrix();
      glTranslatef(m_cPropellerOffset.GetX(), m_cPropellerOffset.GetY(), m_cPropellerOffset.GetZ());
      glRotatef(unPropellerAngle, 0.0f, 0.0f, -1.0f);
      m_cPropellerModel.Draw();
      glPopMatrix();
      /* (-1, 1) */
      glPushMatrix();
      glTranslatef(-m_cPropellerOffset.GetX(), m_cPropellerOffset.GetY(), m_cPropellerOffset.GetZ());
      glRotatef(unPropellerAngle, 0.0f, 0.0f, 1.0f);
      m_cPropellerModel.Draw();
      glPopMatrix();
      /* (-1, -1) */
      glPushMatrix();
      glTranslatef(-m_cPropellerOffset.GetX(), -m_cPropellerOffset.GetY(), m_cPropellerOffset.GetZ());
      glRotatef(unPropellerAngle, 0.0f, 0.0f, -1.0f);
      m_cPropellerModel.Draw();
      glPopMatrix();
      /* (1, -1) */
      glPushMatrix();
      glTranslatef(m_cPropellerOffset.GetX(), -m_cPropellerOffset.GetY(), m_cPropellerOffset.GetZ());
      glRotatef(unPropellerAngle, 0.0f, 0.0f, 1.0f);
      m_cPropellerModel.Draw();
      glPopMatrix();
      /* end of draw propellers */
      glPopMatrix();
   }

   /****************************************/
   /****************************************/

   class CQTOpenGLOperationDrawDroneNormal : public CQTOpenGLOperationDrawNormal {
   public:
      void ApplyTo(CQTOpenGLWidget& c_visualization,
                   CDroneEntity& c_entity) {
         static CQTOpenGLDrone m_cModel;
         m_cModel.Draw(c_entity);
         c_visualization.DrawRays(c_entity.GetControllableEntity());
      }
   };

   /****************************************/
   /****************************************/

   class CQTOpenGLOperationDrawDroneSelected : public CQTOpenGLOperationDrawSelected {
   public:
      void ApplyTo(CQTOpenGLWidget& c_visualization,
                   CDroneEntity& c_entity) {
         c_visualization.DrawBoundingBox(c_entity.GetEmbodiedEntity());
      }
   };

   /****************************************/
   /****************************************/

   REGISTER_QTOPENGL_ENTITY_OPERATION(CQTOpenGLOperationDrawNormal, CQTOpenGLOperationDrawDroneNormal, CDroneEntity);

   REGISTER_QTOPENGL_ENTITY_OPERATION(CQTOpenGLOperationDrawSelected, CQTOpenGLOperationDrawDroneSelected, CDroneEntity);

   /****************************************/
   /****************************************/

   const CVector3 CQTOpenGLDrone::m_cPropellerOffset {0.159, 0.159, 0.271};

   /****************************************/
   /****************************************/

}
