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
#include <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_main_window.h>
#include <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_lua_main_window.h>

namespace argos {

   class CQTOpenGLRender : public CVisualization {

   protected:

      QApplication* m_pcApplication;
      CQTOpenGLMainWindow* m_pcMainWindow;
      CQTOpenGLLuaMainWindow* m_pcQTOpenGLLuaMainWindow;
      char** m_ppcOptions;
      SInt32 m_nOptionNum;
      TConfigurationNode m_tConfTree;
      bool m_bLuaEditor;

   public:

      CQTOpenGLRender() :
         m_pcApplication(NULL),
         m_pcMainWindow(NULL),
         m_ppcOptions(NULL),
         m_nOptionNum(0),
         m_bLuaEditor(NULL) {}

      virtual ~CQTOpenGLRender() {}

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Execute();

      virtual void Destroy();

   };

}

#endif
