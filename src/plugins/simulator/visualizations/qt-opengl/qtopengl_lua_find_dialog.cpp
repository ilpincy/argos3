
/**
 * @file <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_lua_find_dialog.cpp>
 *
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 */
#include "qtopengl_lua_find_dialog.h"

#include <QCheckBox>
#include <QDialogButtonBox>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

namespace argos {

   /****************************************/
   /****************************************/

   CQTOpenGLLuaFindDialog::CQTOpenGLLuaFindDialog(QWidget *parent) :
      QDialog(parent) {
      m_pcLabel = new QLabel(tr("&Find text:"));
      m_pcLineEdit = new QLineEdit();
      m_pcLabel->setBuddy(m_pcLineEdit);

      m_pcCaseCheckBox = new QCheckBox(tr("Match &case"));
      m_pcFromStartCheckBox = new QCheckBox(tr("Search from &start"));
      m_pcFromStartCheckBox->setChecked(true);

      m_pcFindButton = new QPushButton(tr("&Find"));
      m_pcFindButton->setDefault(true);

      m_pcMoreButton = new QPushButton(tr("&More"));
      m_pcMoreButton->setCheckable(true);
      m_pcMoreButton->setAutoDefault(false);

      m_pcButtonBox = new QDialogButtonBox(Qt::Vertical);
      m_pcButtonBox->addButton(m_pcFindButton, QDialogButtonBox::ActionRole);
      m_pcButtonBox->addButton(m_pcMoreButton, QDialogButtonBox::ActionRole);

      m_pcExtension = new QWidget();

      m_pcWholeWordsCheckBox = new QCheckBox(tr("&Whole words"));
      m_pcBackwardCheckBox = new QCheckBox(tr("Search &backward"));
      m_pcSearchSelectionCheckBox = new QCheckBox(tr("Search se&lection"));

      connect(m_pcMoreButton, SIGNAL(toggled(bool)),
              m_pcExtension, SLOT(setVisible(bool)));

      QVBoxLayout* m_pcExtensionLayout = new QVBoxLayout();
      m_pcExtensionLayout->setMargin(0);
      m_pcExtensionLayout->addWidget(m_pcWholeWordsCheckBox);
      m_pcExtensionLayout->addWidget(m_pcBackwardCheckBox);
      m_pcExtensionLayout->addWidget(m_pcSearchSelectionCheckBox);
      m_pcExtension->setLayout(m_pcExtensionLayout);

      QHBoxLayout* m_pcTopLeftLayout = new QHBoxLayout();
      m_pcTopLeftLayout->addWidget(m_pcLabel);
      m_pcTopLeftLayout->addWidget(m_pcLineEdit);

      QVBoxLayout* m_pcLeftLayout = new QVBoxLayout();
      m_pcLeftLayout->addLayout(m_pcTopLeftLayout);
      m_pcLeftLayout->addWidget(m_pcCaseCheckBox);
      m_pcLeftLayout->addWidget(m_pcFromStartCheckBox);
      m_pcLeftLayout->addStretch(1);

      QGridLayout* m_pcMainLayout = new QGridLayout();
      m_pcMainLayout->setSizeConstraint(QLayout::SetFixedSize);
      m_pcMainLayout->addLayout(m_pcLeftLayout, 0, 0);
      m_pcMainLayout->addWidget(m_pcButtonBox, 0, 1);
      m_pcMainLayout->addWidget(m_pcExtension, 1, 0, 1, 2);
      setLayout(m_pcMainLayout);

      setWindowTitle(tr("Find/replace"));
      m_pcExtension->hide();
   }

   /****************************************/
   /****************************************/

}
