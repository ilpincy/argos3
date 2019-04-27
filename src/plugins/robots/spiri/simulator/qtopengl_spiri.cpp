/**
 * @file <argos3/plugins/robots/spiri/simulator/qtopengl_spiri.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "qtopengl_spiri.h"
#include "spiri_entity.h"
#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_obj_model.h>
#include <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_widget.h>

namespace argos {

   /****************************************/
   /****************************************/

   class CQTOpenGLOperationDrawSpiriNormal : public CQTOpenGLOperationDrawNormal {
   public:
      void ApplyTo(CQTOpenGLWidget& c_visualization,
                   CSpiriEntity& c_entity) {
         static CQTOpenGLObjModel m_cSpiriModel("spiri.obj");
         c_visualization.DrawRays(c_entity.GetControllableEntity());
         c_visualization.DrawEntity(c_entity.GetEmbodiedEntity());
         m_cSpiriModel.Draw();
      }
   };

   /****************************************/
   /****************************************/

   class CQTOpenGLOperationDrawSpiriSelected : public CQTOpenGLOperationDrawSelected {
   public:
      void ApplyTo(CQTOpenGLWidget& c_visualization,
                   CSpiriEntity& c_entity) {
         c_visualization.DrawBoundingBox(c_entity.GetEmbodiedEntity());
      }
   };

   REGISTER_QTOPENGL_ENTITY_OPERATION(CQTOpenGLOperationDrawNormal, CQTOpenGLOperationDrawSpiriNormal, CSpiriEntity);

   REGISTER_QTOPENGL_ENTITY_OPERATION(CQTOpenGLOperationDrawSelected, CQTOpenGLOperationDrawSpiriSelected, CSpiriEntity);

   /****************************************/
   /****************************************/

}
