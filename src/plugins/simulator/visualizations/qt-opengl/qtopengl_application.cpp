
/**
 * @file <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_application.cpp>
 *
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 */
#include "qtopengl_application.h"

#include <argos3/core/utility/logging/argos_log.h>

#include <typeinfo>

namespace argos {

   /****************************************/
   /****************************************/

   bool CQTOpenGLApplication::notify(QObject* pc_receiver,
                                     QEvent* pc_event) {
      try {
         return QApplication::notify(pc_receiver, pc_event);
      } catch (std::exception& ex) {
         fprintf(stderr, "%s\n", ex.what());
         QApplication::exit(1);
      } catch (...) {
         qFatal("Error <unknown> sending event %s to object %s (%s)", 
                typeid(*pc_event).name(),
                qPrintable(pc_receiver->objectName()),
                typeid(*pc_receiver).name());
      }
      /* Should never get here */
      return false;
   }

   /****************************************/
   /****************************************/

}
