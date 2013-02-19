
/**
 * @file <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_lua_find_dialog.h>
 *
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 */
#ifndef QTOPENGL_LUA_FIND_DIALOG_H
#define QTOPENGL_LUA_FIND_DIALOG_H

namespace argos {
   class CQTOpenGLLuaFindDialog;
}

#include <QDialog>

class QCheckBox;
class QDialogButtonBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QPushButton;

namespace argos {

   class CQTOpenGLLuaFindDialog : public QDialog {

      Q_OBJECT

   public:

      CQTOpenGLLuaFindDialog(QWidget *parent = 0);

   private:

      QLabel* m_pcLabel;
      QLineEdit* m_pcLineEdit;
      QCheckBox* m_pcCaseCheckBox;
      QCheckBox* m_pcFromStartCheckBox;
      QCheckBox* m_pcWholeWordsCheckBox;
      QCheckBox* m_pcSearchSelectionCheckBox;
      QCheckBox* m_pcBackwardCheckBox;
      QDialogButtonBox* m_pcButtonBox;
      QPushButton* m_pcFindButton;
      QPushButton* m_pcMoreButton;
      QWidget* m_pcExtension;

   };

}

#endif
