/**
 * @file <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_render.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef QT_OPENGL_RENDER_H
#define QT_OPENGL_RENDER_H

namespace argos {
   class CQTOpenGLRender;
}

class QApplication;

#include <argos3/core/simulator/visualization/visualization.h>
#include "qtopengl_main_window.h"

namespace argos {

   class CQTOpenGLRender : public CVisualization {

   protected:

      QApplication* m_pcApplication;
      CQTOpenGLMainWindow* m_pcMainWindow;
      char** m_ppcOptions;
      SInt32 m_nOptionNum;
      TConfigurationNode m_tConfTree;

   public:

      CQTOpenGLRender() :
         m_pcApplication(NULL),
         m_pcMainWindow(NULL),
         m_ppcOptions(NULL),
         m_nOptionNum(0) {}

      virtual ~CQTOpenGLRender() {}

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Execute();

      virtual void Destroy();

   };

}

#endif
