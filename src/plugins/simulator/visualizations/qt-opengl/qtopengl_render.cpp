/**
 * @file <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_render.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "qtopengl_render.h"
#include "qtopengl_application.h"

#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/core/utility/string_utilities.h>
#include <cstring>

#include <QPixmap>
#include <QSplashScreen>

namespace argos {

   /****************************************/
   /****************************************/

   void CQTOpenGLRender::Init(TConfigurationNode& t_tree) {
      /* Parse options from the XML */
#ifdef ARGOS_WITH_LUA
      GetNodeAttributeOrDefault(t_tree, "lua_editor", m_bLuaEditor, m_bLuaEditor);
#endif
      /* Save the configuration for later */
      m_tConfTree = t_tree;
      /* Set up dummy arguments for QApplication */
      m_nOptionNum = 1;
      m_ppcOptions = new char*[m_nOptionNum];
      m_ppcOptions[0] = new char[7];
      ::strcpy(m_ppcOptions[0], "argos3");
      /* Create the QT application */
      m_pcApplication = new CQTOpenGLApplication(m_nOptionNum, m_ppcOptions);
      /* Set some data about the application */
      m_pcApplication->setApplicationName("ARGoS");
      m_pcApplication->setApplicationVersion("3.0");
      m_pcApplication->setOrganizationName("Iridia-ULB");
      m_pcApplication->setOrganizationDomain("iridia.ulb.ac.be");
      /* Draw the main window */
      m_pcMainWindow = new CQTOpenGLMainWindow(m_tConfTree);
#ifdef ARGOS_WITH_LUA
      /* Create Lua editor if required */
      if(m_bLuaEditor) {
         m_pcQTOpenGLLuaMainWindow = new CQTOpenGLLuaMainWindow(m_pcMainWindow);
      }
#endif
      LOG.Flush();
      LOGERR.Flush();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLRender::Execute() {
      try {
         /* This effectively starts the experiment */
         m_pcMainWindow->show();
#ifdef ARGOS_WITH_LUA
         if(m_bLuaEditor) {
            m_pcQTOpenGLLuaMainWindow->show();
         }
#endif
         m_pcApplication->exec();
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Error while executing the experiment.", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLRender::Destroy() {
#ifdef ARGOS_WITH_LUA
      /* Destroy the Lua editor */
      if(m_bLuaEditor) {
         delete m_pcQTOpenGLLuaMainWindow;
      }
#endif
      /* Destroy the main window */
      delete m_pcMainWindow;
      /* Destroy the QT application */
      delete m_pcApplication;
      /* Get rid of the factory */
      CFactory<CQTOpenGLUserFunctions>::Destroy();
   }

   /****************************************/
   /****************************************/

   CQTOpenGLMainWindow& CQTOpenGLRender::GetMainWindow() {
      if(m_pcMainWindow == NULL) {
         THROW_ARGOSEXCEPTION("CQTOpenGLRender::GetMainWindow(): no main window created");
      }
      return *m_pcMainWindow;
   }

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   CQTOpenGLLuaMainWindow& CQTOpenGLRender::GetLuaMainWindow() {
      if(m_pcQTOpenGLLuaMainWindow == NULL) {
         THROW_ARGOSEXCEPTION("CQTOpenGLRender::GetLuaMainWindow(): no Lua main window created");
      }
      return *m_pcQTOpenGLLuaMainWindow;
   }
#endif

   /****************************************/
   /****************************************/

   REGISTER_VISUALIZATION(CQTOpenGLRender,
                          "qt-opengl",
                          "Carlo Pinciroli [ilpincy@gmail.com]",
                          "1.0",
                          "An interactive graphical renderer based on QT and OpenGL.",
                          "The QT-OpenGL renderer is a graphical renderer based on QT >= 4.5 and OpenGL.\n"
                          "It allows the user to watch and modify the simulation as it's running in an\n"
                          "intuitive way.\n\n"
                          "REQUIRED XML CONFIGURATION\n\n"
                          "  <visualization>\n"
                          "    <qt-opengl />\n"
                          "  </visualization>\n\n"
                          "OPTIONAL XML CONFIGURATION\n\n"
                          "You can auto-play the simulation at startup by specifying the 'autoplay'\n"
                          "attribute as follows:\n\n"
                          "  <visualization>\n"
                          "    <qt-opengl autoplay=\"true\" />\n"
                          "  </visualization>\n\n"
                          "It is also possible to set some camera parameters. There are 10 available\n"
                          "cameras to use. You can switch from one to the other by clicking on the\n"
                          "graphical view (to give it focus) and then pressing the keys 0-9.\n"
                          "To configure position and orientation of specific cameras, say cameras 0 to 3,\n"
                          "you have to include the following XML code:\n\n"
                          "  <visualization>\n"
                          "    <qt-opengl>\n"
                          "      <camera>\n"
                          "        <placement idx=\"0\" position=\"2,2,2\" look_at=\"1,1,1\" />\n"
                          "        <placement idx=\"1\" position=\"1,0,7\" look_at=\"1,0,0\" />\n"
                          "        <placement idx=\"2\" position=\"3,3,4\" look_at=\"1,6,0\" />\n"
                          "        <placement idx=\"3\" position=\"2,3,2\" look_at=\"0,1,0\" />\n"
                          "      </camera>\n"
                          "    </qt-opengl>\n"
                          "  </visualization>\n\n"
                          "The 'idx' attribute specifies the camera index (and the key to press to switch\n"
                          "to that camera).\n"
                          "The 'position' attribute contains the position of the camera in the arena.\n"
                          "The 'look_at' attribute sets the point the camera is looking at.\n"
                          "Sometimes, specifying a camera positioning with only 'position' and 'look_at'\n"
                          "generates ambiguous configurations, which ARGoS resolves in a default way after\n"
                          "printing a warning message. To place the camera without ambiguities, specify\n"
                          "also the 'up' vector of the camera. If the camera is your head, imagine this\n"
                          "vector as an arrow that stems from the center of your head and extends upwards\n."
                          "The 'up' vector must be perpendicular to the difference between the 'look_at'\n"
                          "and the 'position' vectors."
                          "You can set some optical parameters of real cameras, namely the focal length\n"
                          "and the length of the frame diagonal. For example:\n\n"
                          "  <visualization>\n"
                          "    <qt-opengl>\n"
                          "      <camera>\n"
                          "        ...\n"
                          "        <placement idx=\"4\"\n"
                          "                   position=\"4,1,4\"\n"
                          "                   look_at=\"2,1,0\"\n"
                          "                   lens_focal_length=\"50\"\n"
                          "                   frame_diagonal=\"40\" />\n"
                          "        ...\n"
                          "      </camera>\n"
                          "    </qt-opengl>\n"
                          "  </visualization>\n\n"
                          "The 'lens_focal_length' attribute controls the focal length of the lens of the\n"
                          "simulated camera. The value is in millimeters and it defaults, if not set in\n"
                          "XML, to 20mm.\n"
                          "The 'frame_diagonal' attribute specifies the length of the frame diagonal of\n"
                          "the image film. The value is in millimeters and it defaults, if not set in\n"
                          "XML, to 35mm.\n"
                          "This visualization also allows for user customization. In a similar fashion to\n"
                          "the loop functions, you can set a plug-in that derives from the\n"
                          "CQTOpenGLUserFunctions class. To load it in the system, follow this example:\n\n"
                          "  <visualization>\n"
                          "    <qt-opengl>\n"
                          "      <user_functions library=\"/path/to/libmyuserfunctions.so\"\n"
                          "                      label=\"my_user_functions\" />\n"
                          "    </qt-opengl>\n"
                          "  </visualization>\n\n"
                          "The 'library' attribute points to the library where the user functions are\n"
                          "stored. This library can be the same as the loop functions, or a new one.\n"
                          "There is no limitation to where the code is to be found.\n"
                          "The 'label' attribute identifies the user function class to use. In this way,\n"
                          "in a single library you can have multiple user function implementations, if\n"
                          "you wish.\n"
                          "You can also grab frames and store them into image files, for example to create\n"
                          "videos in a fast way. To do it, you just need to press the red capture button\n"
                          "and frame grabbing will be on. By default, the frames are named\n"
                          "'frame_NNNNN.png' and are stored in the current directory, i.e. the directory\n"
                          "where you run the 'argos' command. If you want to override this behavior, you\n"
                          "can add the optional 'frame_grabbing' section as follows:\n\n"
                          "  <visualization>\n"
                          "    <qt-opengl>\n"
                          "      <frame_grabbing directory=\"frames\"\n"
                          "                      base_name=\"myframe_\"\n"
                          "                      format=\"png\"\n"
                          "                      quality=\"100\" />\n"
                          "    </qt-opengl>\n"
                          "  </visualization>\n\n"
                          "All the attributes in this section are optional. If you don't specify one of\n"
                          "them, the default is taken.\n"
                          "The 'directory' attribute stores the directory where the frames are saved. If\n"
                          "the directory does not exist, a fatal error occurs. The directory must exist\n"
                          "and be writable. Both absolute and relative paths are allowed. The default\n"
                          "value is '.'\n"
                          "The 'base_name' attribute is the string to prepend to the file name. After this\n"
                          "string, the frame number (padded to 5 digits) is added. The default value is\n"
                          "'frame_', so a typical resulting name is 'frame_00165'.\n"
                          "The 'format' attribute specifies the format. The default value is 'png' but you\n"
                          "can put any format supported by Qt>=4.5. Refer to the Qt documentation for the\n"
                          "complete list of supported formats.\n"
                          "The 'quality' attribute dictates the quality of the image. Its value is in the\n"
                          "range [0:100] where 0 means maximum compression and minimum quality, and 100\n"
                          "means maximum quality and no compression at all. The default value is '-1',\n"
                          "which means to use Qt's default quality. For videos, it's best to use 100 to\n"
                          "avoid artifacts due to compression. For a normal screenshot, the default is the\n"
                          "safest choice.\n",
                          "Usable"
      );

}
