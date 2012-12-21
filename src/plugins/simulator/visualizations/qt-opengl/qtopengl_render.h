/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/**
 * @file <simulator/visualizations/qtopengl_render.h>
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
