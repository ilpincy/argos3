/**
 * @file <argos3/plugins/robots/e-puck/simulator/qtopengl_epuck.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef QTOPENGL_EPUCK_H
#define QTOPENGL_EPUCK_H

namespace argos {
   class CQTOpenGLEPuck;
   class CEPuckEntity;
}

#ifdef __APPLE__
#include <gl.h>
#else
#include <GL/gl.h>
#endif

#include <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_obj_model.h>

namespace argos {

   class CQTOpenGLEPuck {

   public:

      CQTOpenGLEPuck();

      virtual ~CQTOpenGLEPuck();

      virtual void Draw(CEPuckEntity& c_entity);

   private:

      CQTOpenGLObjModel* m_pcEPuckModel;
      std::vector<CQTOpenGLObjModel::SMaterial*> m_vecLEDs;

   };

}

#endif
