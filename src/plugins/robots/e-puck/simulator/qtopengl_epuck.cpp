/**
 * @file <argos3/plugins/robots/e-puck/simulator/qtopengl_epuck.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "qtopengl_epuck.h"
#include "epuck_entity.h"
#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/plugins/simulator/entities/led_equipped_entity.h>
#include <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_widget.h>

namespace argos {

   /****************************************/
   /****************************************/

   CQTOpenGLEPuck::CQTOpenGLEPuck() {
      /* create the model */
      m_pcEPuckModel = new CQTOpenGLObjModel("epuck.obj");
      /* get pointers to the LED materials */
      m_vecLEDs = {
         &m_pcEPuckModel->GetMaterial("led_0"),
         &m_pcEPuckModel->GetMaterial("led_1"),
         &m_pcEPuckModel->GetMaterial("led_2"),
         &m_pcEPuckModel->GetMaterial("led_3"),
         &m_pcEPuckModel->GetMaterial("led_4"),
         &m_pcEPuckModel->GetMaterial("led_5"),
         &m_pcEPuckModel->GetMaterial("led_6"),
         &m_pcEPuckModel->GetMaterial("led_7")
      };
   }

   /****************************************/
   /****************************************/

   CQTOpenGLEPuck::~CQTOpenGLEPuck() {
      delete m_pcEPuckModel;
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLEPuck::Draw(CEPuckEntity& c_entity) {
      glPushMatrix();
      /* update the LED materials */
      for(UInt32 un_material_idx = 0;
          un_material_idx < m_vecLEDs.size();
          un_material_idx++) {
         const CColor& cColor = c_entity.GetLEDEquippedEntity().GetLED(un_material_idx).GetColor();
         std::array<GLfloat, 4> arrColor = {
            cColor.GetRed() / 255.0f,
            cColor.GetGreen() / 255.0f,
            cColor.GetBlue() / 255.0f,
            1.0f
         };
         m_vecLEDs[un_material_idx]->Emission = arrColor;
      }
      m_pcEPuckModel->Draw();
      glPopMatrix();
   }

   /****************************************/
   /****************************************/

   class CQTOpenGLOperationDrawEPuckNormal : public CQTOpenGLOperationDrawNormal {
   public:
      void ApplyTo(CQTOpenGLWidget& c_visualization,
                   CEPuckEntity& c_entity) {
         static CQTOpenGLEPuck m_cModel;
         c_visualization.DrawRays(c_entity.GetControllableEntity());
         c_visualization.DrawEntity(c_entity.GetEmbodiedEntity());
         m_cModel.Draw(c_entity);
      }
   };

   class CQTOpenGLOperationDrawEPuckSelected : public CQTOpenGLOperationDrawSelected {
   public:
      void ApplyTo(CQTOpenGLWidget& c_visualization,
                   CEPuckEntity& c_entity) {
         c_visualization.DrawBoundingBox(c_entity.GetEmbodiedEntity());
      }
   };

   REGISTER_QTOPENGL_ENTITY_OPERATION(CQTOpenGLOperationDrawNormal, CQTOpenGLOperationDrawEPuckNormal, CEPuckEntity);

   REGISTER_QTOPENGL_ENTITY_OPERATION(CQTOpenGLOperationDrawSelected, CQTOpenGLOperationDrawEPuckSelected, CEPuckEntity);

   /****************************************/
   /****************************************/

}
