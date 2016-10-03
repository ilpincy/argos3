/**
 * @file <argos3/plugins/simulator/visualizations/qt-opengl/dialogs/layouts/qtopengl_embodied_entity_dialog_layout.cpp>
 *
 * @author Ewout Merckx - <ewoutmerckx.pro@gmail.com>
 */

#include "qtopengl_embodied_entity_dialog_layout.h"

#include <QGroupBox>
#include <QLabel>

namespace argos {

    /****************************************/
    /****************************************/

    CQTOpenGLEmbodiedEntityDialogLayout::CQTOpenGLEmbodiedEntityDialogLayout(QWidget* pc_parent_widget) 
        : CQTOpenGLEntityDialogLayout(pc_parent_widget)
    { 
        this->Init();
    }

    /****************************************/
    /****************************************/

    void CQTOpenGLEmbodiedEntityDialogLayout::Init() 
    {
        // initialize position input fields
        QLabel* pcEnityPositionXLabel = new QLabel(tr("x"));
        QLabel* pcEnityPositionYLabel = new QLabel(tr("y"));
        QLabel* pcEnityPositionZLabel = new QLabel(tr("z"));
        m_pcEnityPositionXLineEdit = new QLineEdit;
        m_pcEnityPositionYLineEdit = new QLineEdit;
        m_pcEnityPositionZLineEdit = new QLineEdit;
        m_pcEnityPositionXLineEdit->setPlaceholderText("real number");
        m_pcEnityPositionYLineEdit->setPlaceholderText("real number");
        m_pcEnityPositionZLineEdit->setPlaceholderText("real number");

        // initialize orientation input fields
        QLabel* pcEnityOrientationWLabel = new QLabel(tr("w"));
        QLabel* pcEnityOrientationXLabel = new QLabel(tr("x"));
        QLabel* pcEnityOrientationYLabel = new QLabel(tr("y"));
        QLabel* pcEnityOrientationZLabel = new QLabel(tr("z"));
        m_pcEnityOrientationWLineEdit = new QLineEdit;
        m_pcEnityOrientationXLineEdit = new QLineEdit;
        m_pcEnityOrientationYLineEdit = new QLineEdit;
        m_pcEnityOrientationZLineEdit = new QLineEdit;
        m_pcEnityOrientationWLineEdit->setPlaceholderText("real number");
        m_pcEnityOrientationXLineEdit->setPlaceholderText("real number");
        m_pcEnityOrientationYLineEdit->setPlaceholderText("real number");
        m_pcEnityOrientationZLineEdit->setPlaceholderText("real number");

        // add position input fields to group box
        QGroupBox* pcEntityPositionGroupBox = new QGroupBox(tr("Position"));
        QHBoxLayout *pcEntityPositionGroupBoxLayout = new QHBoxLayout;
        pcEntityPositionGroupBoxLayout->addWidget(pcEnityPositionXLabel);
        pcEntityPositionGroupBoxLayout->addWidget(m_pcEnityPositionXLineEdit);
        pcEntityPositionGroupBoxLayout->addWidget(pcEnityPositionYLabel);
        pcEntityPositionGroupBoxLayout->addWidget(m_pcEnityPositionYLineEdit);
        pcEntityPositionGroupBoxLayout->addWidget(pcEnityPositionZLabel);
        pcEntityPositionGroupBoxLayout->addWidget(m_pcEnityPositionZLineEdit);
        pcEntityPositionGroupBox->setLayout(pcEntityPositionGroupBoxLayout);

        // add orientation input fields to group box
        QGroupBox* pcEntityOrientationGroupBox = new QGroupBox(tr("Orientation"));
        QHBoxLayout *pcEntityOrientationGroupBoxLayout = new QHBoxLayout;
        pcEntityOrientationGroupBoxLayout->addWidget(pcEnityOrientationWLabel);
        pcEntityOrientationGroupBoxLayout->addWidget(m_pcEnityOrientationWLineEdit);
        pcEntityOrientationGroupBoxLayout->addWidget(pcEnityOrientationXLabel);
        pcEntityOrientationGroupBoxLayout->addWidget(m_pcEnityOrientationXLineEdit);
        pcEntityOrientationGroupBoxLayout->addWidget(pcEnityOrientationYLabel);
        pcEntityOrientationGroupBoxLayout->addWidget(m_pcEnityOrientationYLineEdit);
        pcEntityOrientationGroupBoxLayout->addWidget(pcEnityOrientationZLabel);
        pcEntityOrientationGroupBoxLayout->addWidget(m_pcEnityOrientationZLineEdit);
        pcEntityOrientationGroupBox->setLayout(pcEntityOrientationGroupBoxLayout);

        // add group boxes to layout
        this->addWidget(pcEntityPositionGroupBox);
        this->addWidget(pcEntityOrientationGroupBox);
    }

    /****************************************/
    /****************************************/

    void CQTOpenGLEmbodiedEntityDialogLayout::CheckInput() 
    {
        CQTOpenGLEntityDialogLayout::CheckInput();
    }

    /****************************************/
    /****************************************/

    CEntity& CQTOpenGLEmbodiedEntityDialogLayout::GetEntity()
    {
        THROW_ARGOSEXCEPTION("In CQTOpenGLEmbodiedEntityDialogLayout::GetEntity(), " 
            << "there is no entity to get.");
    }

    /****************************************/
    /****************************************/

}