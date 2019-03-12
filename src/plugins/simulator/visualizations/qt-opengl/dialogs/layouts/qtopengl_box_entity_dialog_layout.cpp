/**
 * @file <argos3/plugins/simulator/visualizations/qt-opengl/dialogs/layouts/qtopengl_box_entity_dialog_layout.cpp>
 *
 * @author Ewout Merckx - <ewoutmerckx.pro@gmail.com>
 */

#include "qtopengl_box_entity_dialog_layout.h"

#include <argos3/plugins/simulator/entities/box_entity.h>

#include <QGroupBox>
#include <QLabel>

namespace argos {

    /****************************************/
    /****************************************/

    CQTOpenGLBoxEntityDialogLayout::CQTOpenGLBoxEntityDialogLayout(QWidget* pc_parent_widget) 
        : CQTOpenGLEmbodiedEntityDialogLayout(pc_parent_widget)
    { 
        this->Init();
    }

    /****************************************/
    /****************************************/

    void CQTOpenGLBoxEntityDialogLayout::Init() 
    {
        // initialize size input fields
        QLabel* pcEnitySizeXLabel = new QLabel(tr("x"));
        QLabel* pcEnitySizeYLabel = new QLabel(tr("y"));
        QLabel* pcEnitySizeZLabel = new QLabel(tr("z"));
        m_pcEnitySizeXLineEdit = new QLineEdit;
        m_pcEnitySizeYLineEdit = new QLineEdit;
        m_pcEnitySizeZLineEdit = new QLineEdit;
        m_pcEnitySizeXLineEdit->setPlaceholderText("real number");
        m_pcEnitySizeZLineEdit->setPlaceholderText("real number");
        m_pcEnitySizeYLineEdit->setPlaceholderText("real number");

        // initialize movable input field
        m_pcEnityIsMovableCheckBox = new QCheckBox;
        connect(m_pcEnityIsMovableCheckBox, SIGNAL(clicked(bool)), 
            this, SLOT(MovableCheckboxToggled(bool)));

        // initialize mass input field
        QLabel* pcEnityMassLabel = new QLabel(tr("Mass"));
        m_pcEnityMassLineEdit = new QLineEdit;
        m_pcEnityMassLineEdit->setPlaceholderText("real number");
        m_pcEnityMassLineEdit->setDisabled(1);

        // add size input fields to group box
        QGroupBox* pcEntitySizeGroupBox = new QGroupBox(tr("Size*"));
        QHBoxLayout *pcEntitySizeGroupBoxLayout = new QHBoxLayout;
        pcEntitySizeGroupBoxLayout->addWidget(pcEnitySizeXLabel);
        pcEntitySizeGroupBoxLayout->addWidget(m_pcEnitySizeXLineEdit);
        pcEntitySizeGroupBoxLayout->addWidget(pcEnitySizeYLabel);
        pcEntitySizeGroupBoxLayout->addWidget(m_pcEnitySizeYLineEdit);
        pcEntitySizeGroupBoxLayout->addWidget(pcEnitySizeZLabel);
        pcEntitySizeGroupBoxLayout->addWidget(m_pcEnitySizeZLineEdit);
        pcEntitySizeGroupBox->setLayout(pcEntitySizeGroupBoxLayout);

        // add movable and mass input fields to group box
        QGroupBox* pcEntityMovableGroupBox = new QGroupBox(tr("Movable"));
        QHBoxLayout *pcEntityMovableGroupBoxLayout = new QHBoxLayout;
        pcEntityMovableGroupBoxLayout->addWidget(m_pcEnityIsMovableCheckBox);
        pcEntityMovableGroupBoxLayout->addWidget(pcEnityMassLabel);
        pcEntityMovableGroupBoxLayout->addWidget(m_pcEnityMassLineEdit);
        pcEntityMovableGroupBox->setLayout(pcEntityMovableGroupBoxLayout);

        // add group boxes to layout
        this->addWidget(pcEntitySizeGroupBox);
        this->addWidget(pcEntityMovableGroupBox);
    }

    /****************************************/
    /****************************************/

    void CQTOpenGLBoxEntityDialogLayout::CheckInput() 
    {
        CQTOpenGLEmbodiedEntityDialogLayout::CheckInput();

        // check size
        CVector3 cSizeVector = this->GetSize();
        if(cSizeVector.GetX() <= 0.0f || cSizeVector.GetY() <= 0.0f || cSizeVector.GetZ() <= 0.0f) {
            THROW_ARGOSEXCEPTION("Please fill in a strict positive size of the entity.");
        }

        // check mass if movable
        if(this->IsMovable()){
            if(this->GetMass() <= 0.0f) {
                THROW_ARGOSEXCEPTION("Please fill in a strict positive mass of the entity.");
            }
        }
    }

    /****************************************/
    /****************************************/

    CEntity& CQTOpenGLBoxEntityDialogLayout::GetEntity()
    {
        this->CheckInput();

        // calculate mass
        Real fEntityMass = 1.0f;
        if(this->IsMovable()){
            fEntityMass = this->GetMass();
        }

        // create new entity
        CBoxEntity* cBoxEnt = new CBoxEntity(
            this->GetId(),
            this->GetPositition(),
            this->GetOrientation(),
            this->IsMovable(),
            this->GetSize(),
            fEntityMass
            );

        return *cBoxEnt;
    }

    /****************************************/
    /****************************************/

    void CQTOpenGLBoxEntityDialogLayout::MovableCheckboxToggled(bool b_is_checked)
    {
        if(b_is_checked) {
            m_pcEnityMassLineEdit->setEnabled(1);
        }
        else {
            m_pcEnityMassLineEdit->setDisabled(1);
        }
    }

    /****************************************/
    /****************************************/

}