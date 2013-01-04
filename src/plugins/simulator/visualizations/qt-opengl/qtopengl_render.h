/**
 * @file <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_render.h>
 *
 * @brief This file contains the definition of the CRender interface.
 *
 * This file contains the definition of the CRender interface. Such interface
 * is the base for all the visualisations (OpenGL, OGRE, gnuplot, blender,
 * etc.) in ARGOS.
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
      bool m_bShowSplashScreen;
      TConfigurationNode m_tConfTree;

   public:

      CQTOpenGLRender() :
         m_pcApplication(NULL),
         m_pcMainWindow(NULL),
         m_ppcOptions(NULL),
         m_nOptionNum(0),
         m_bShowSplashScreen(true) {}

      virtual ~CQTOpenGLRender() {}

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Execute();

      virtual void Destroy();

   };

}

#endif
