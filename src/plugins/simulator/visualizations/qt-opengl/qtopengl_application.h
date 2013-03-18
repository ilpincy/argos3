#ifndef QTOPENGL_APPLICATION_H
#define QTOPENGL_APPLICATION_H

namespace argos {
   class CQTOpenGLApplication;
}

#include <QApplication>

namespace argos {

   class CQTOpenGLApplication : public QApplication {

      Q_OBJECT

   public:

      CQTOpenGLApplication(int& n_option_num,
                           char** ppc_options) :
         QApplication(n_option_num,
                      ppc_options) {}

      virtual ~CQTOpenGLApplication() {}

      virtual bool notify(QObject* pc_receiver,
                          QEvent* pc_event);

   };

}

#endif
