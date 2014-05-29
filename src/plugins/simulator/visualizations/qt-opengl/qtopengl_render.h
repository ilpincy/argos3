/**
 * @file <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_render.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef QT_OPENGL_RENDER_H
#define QT_OPENGL_RENDER_H

namespace argos {
   class CQTOpenGLRender;
   class CQTOpenGLApplication;
}

#include <argos3/core/simulator/visualization/visualization.h>
#include <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_main_window.h>

#ifdef ARGOS_WITH_LUA
#include <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_lua_main_window.h>
#endif

namespace argos {

   class CQTOpenGLRender : public CVisualization {

   public:

      CQTOpenGLRender() :
         m_pcApplication(NULL),
         m_pcMainWindow(NULL),
         m_ppcOptions(NULL),
         m_nOptionNum(0)
#ifdef ARGOS_WITH_LUA
         , m_pcQTOpenGLLuaMainWindow(NULL),
         m_bLuaEditor(false)
#endif
      {}

      virtual ~CQTOpenGLRender() {}

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Execute();

      virtual void Reset() {}

      virtual void Destroy();

      CQTOpenGLMainWindow& GetMainWindow();

#ifdef ARGOS_WITH_LUA
      CQTOpenGLLuaMainWindow& GetLuaMainWindow();
#endif

   private:

      CQTOpenGLApplication* m_pcApplication;
      CQTOpenGLMainWindow* m_pcMainWindow;
      char** m_ppcOptions;
      SInt32 m_nOptionNum;
      TConfigurationNode m_tConfTree;
#ifdef ARGOS_WITH_LUA
      CQTOpenGLLuaMainWindow* m_pcQTOpenGLLuaMainWindow;
      bool m_bLuaEditor;
#endif

   };

}

#endif
