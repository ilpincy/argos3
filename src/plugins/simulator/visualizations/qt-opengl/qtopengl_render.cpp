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
      m_pcApplication->setOrganizationName("ARGoS");
      m_pcApplication->setOrganizationDomain("argos-sim.info");
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
      if(m_pcMainWindow == nullptr) {
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
                          "An interactive graphical renderer based on Qt and OpenGL.",
                          "The QT-OpenGL renderer is a graphical renderer based on Qt >= 4.5 and OpenGL.\n"
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
                          "It is also possible to set the camera parameters. There are 12 available slots\n"
                          "in which cameras can be configured. These cameras can be selected in the user\n"
                          "interface by pressing F1-F12 or by clicking on the corresponding camera icon.\n"
                          "By default, and if no configuration is given, each of these slots defaults to a\n"
                          "camera position calculated from the size of the arena and the arena's center.\n"
                          "The following example demonstrates overriding four of these camera\n"
                          "configurations:\n\n"
                          "  <visualization>\n"
                          "    <qt-opengl>\n"
                          "      <camera>\n"
                          "        <placements>\n"
                          "          <placement index=\"0\" position=\"2,2,2\" look_at=\"1,1,1\" />\n"
                          "          <placement index=\"1\" position=\"1,0,7\" look_at=\"1,0,0\" />\n"
                          "          <placement index=\"2\" position=\"3,3,4\" look_at=\"1,6,0\" />\n"
                          "          <placement index=\"3\" position=\"2,3,2\" look_at=\"0,1,0\" />\n"
                          "        </placements>\n"
                          "      </camera>\n"
                          "    </qt-opengl>\n"
                          "  </visualization>\n\n"
                          "The 'index' attribute specifies the camera to override, with the zeroth index\n"
                          "corresponding to the camera bound to the F1 key and so on.\n"
                          "The 'position' attribute contains the position of the camera in the arena.\n"
                          "The 'look_at' attribute sets the point the camera is looking at.\n"
                          "Sometimes, specifying a camera positioning with only 'position' and 'look_at'\n"
                          "generates ambiguous configurations, which ARGoS resolves in a default way after\n"
                          "printing a warning message. To place the camera without ambiguities, specify\n"
                          "also the 'up' vector of the camera. If the camera is your head, imagine this\n"
                          "vector as an arrow that stems from the center of your head and extends upwards\n."
                          "The 'up' vector must be perpendicular to the difference between the 'look_at'\n"
                          "and the 'position' vectors.\n"
                          "You can also set the focal length of the camera. For example:\n\n"
                          "  <visualization>\n"
                          "    <qt-opengl>\n"
                          "      <camera>\n"
                          "        <placements>\n"
                          "          ...\n"
                          "          <placement index=\"4\"\n"
                          "                     position=\"4,1,4\"\n"
                          "                     look_at=\"2,1,0\"\n"
                          "                     lens_focal_length=\"50\" />\n"
                          "          ...\n"
                          "        <placements>\n"
                          "      </camera>\n"
                          "    </qt-opengl>\n"
                          "  </visualization>\n\n"
                          "The 'lens_focal_length' attribute controls the focal length of the lens of the\n"
                          "simulated camera. The value is in millimeters and defaults to 20 mm, if it is\n"
                          "not set from the XML.\n"
                          "You can also configure the camera to switch placement automatically according\n"
                          "to a timeline and even interpolate between different placements as shown below:\n"
                          "  <visualization>\n"
                          "    <qt-opengl>\n"
                          "      <camera>\n"
                          "        <placements>\n"
                          "          ...\n"
                          "          <placement index=\"0\" ... />\n"
                          "          <placement index=\"1\" ... />\n"
                          "          <placement index=\"2\" ... />\n"
                          "          ...\n"
                          "        </placements>\n"
                          "        <timeline loop=\"400\">\n"
                          "          <keyframe placement=\"0\" step=\"0\" />\n"
                          "          <interpolate />\n"
                          "          <keyframe placement=\"1\" step=\"100\" />\n"
                          "          <keyframe placement=\"2\" step=\"200\" />\n"
                          "          <keyframe placement=\"1\" step=\"300\" />\n"
                          "          <interpolate />\n"
                          "        </timeline>\n"
                          "      </camera>\n"
                          "    </qt-opengl>\n"
                          "  </visualization>\n\n"
                          "This feature is enabled by adding a <timeline> node which consists of\n"
                          "<keyframe> nodes. These keyframe nodes indicate the step number on which a\n"
                          "given camera placement will be used. It is possible to interpolate between two\n"
                          "keyframes by adding a <interpolate> node between them. The keyframes must be\n"
                          "specified in order. The timeline can be set to loop by setting the loop\n"
                          "attribute on the timeline node to the value at which the timeline should start\n"
                          "over from the beginning.\n"
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
                          "videos in a fast way. This can be done from within the ARGoS window and when\n"
                          "running ARGoS without visualizations (headless). To do it from within the ARGoS\n"
                          "window, you just need to press the red capture button and frame grabbing will be\n"
                          "on.\n"
                          "You can also grab frames when running without visualizations by running ARGoS\n"
                          "under Xvfb to give it a virtual framebuffer to render into. The command\n\n"
                          "  xvfb-run -s \"-screen 0 1600x1200x24\" argos3 -c example.argos\n\n"
                          "will run spawn a new Xvfb server with a virtual window size of 1600x1200, 8-bit\n"
                          "color per channel and run ARGoS under it (ARGoS can also be made to attach to an\n"
                          "existing Xvfb server via the DISPLAY environment variable). Refer to the\n"
                          "documentation on Xvfb for the precise meaning of the arguments.\n"
                          "By default, the frames are named 'frame_NNNNNNNNNN.png' and are stored in the current\n"
                          "directory, i.e. the directory where you run the 'argos' command. If you want to\n"
                          "override these defaults, you can add the optional 'frame_grabbing' section as\n"
                          "follows:\n\n"
                          "  <visualization>\n"
                          "    <qt-opengl>\n"
                          "      <frame_grabbing directory=\"frames\"\n"
                          "                      base_name=\"myframe_\"\n"
                          "                      format=\"png\"\n"
                          "                      quality=\"100\"\n"
                          "                      headless_grabbing=\"false\"\n"
                          "                      headless_frame_size=\"1600x1200\"\n"
                          "                      headless_frame_rate=\"1\"/>\n"
                          "    </qt-opengl>\n"
                          "  </visualization>\n\n"
                          "All the attributes in this section are optional. If you don't specify one of\n"
                          "them, the default is taken.\n"
                          "The 'directory' attribute stores the directory where the frames are saved. If\n"
                          "the directory does not exist, a fatal error occurs. The directory must exist\n"
                          "and be writable. Both absolute and relative paths are allowed. The default\n"
                          "value is '.'\n"
                          "The 'base_name' attribute is the string to prepend to the file name. After this\n"
                          "string, the frame number (padded to 10 digits) is added. The default value is\n"
                          "'frame_', so a typical resulting name is 'frame_0000000165'.\n"
                          "The 'format' attribute specifies the format. The default value is 'png' but you\n"
                          "can put any format supported by Qt>=4.5. Refer to the Qt documentation for the\n"
                          "complete list of supported formats.\n"
                          "The 'quality' attribute dictates the quality of the image. Its value is in the\n"
                          "range [0:100] where 0 means maximum compression and minimum quality, and 100\n"
                          "means maximum quality and no compression at all. The default value is '-1',\n"
                          "which means to use Qt's default quality. For videos, it's best to use 100 to\n"
                          "avoid artifacts due to compression. For a normal screenshot, the default is the\n"
                          "safest choice.\n"
                          "The 'headless_grabbing' attribute defaults to 'false'.\n"
                          "The 'headless_frame_size' attribute is the size of the main QTWidget in ARGoS,\n"
                          "*not* the size of the converted frames (actual images will be somewhat smaller).\n"
                          "The 'headless_frame_rate' attribute specifes the frame skip rate (i.e. grab\n"
                          "every n-th frame). The default value is '1'.\n",
                          "Usable"
      );

}
