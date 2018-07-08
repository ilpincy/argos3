/**
 * @file <argos3/plugins/simulator/visualizations/qt-opengl/dialogs/layouts/qtopengl_cylinder_entity_dialog_layout.cpp>
 *
 * @author Ewout Merckx - <ewoutmerckx.pro@gmail.com>
 */

#include "qtopengl_cylinder_entity_dialog_layout.h"

#include <argos3/plugins/simulator/entities/cylinder_entity.h>

#include <QGroupBox>
#include <QLabel>

namespace argos {

    /****************************************/
    /****************************************/

    CQTOpenGLCylinderEntityDialogLayout::CQTOpenGLCylinderEntityDialogLayout(QWidget* pc_parent_widget) 
        : CQTOpenGLEmbodiedEntityDialogLayout(pc_parent_widget)
    { 
        this->Init();
    }

    /****************************************/
    /****************************************/

    void CQTOpenGLCylinderEntityDialogLayout::Init() 
    {
        // initialize size input fields
        QLabel* pcEnitySizeRadiusLabel = new QLabel(tr("Radius"));
        QLabel* pcEnitySizeHeightLabel = new QLabel(tr("Height"));
        m_pcEnitySizeRadiusLineEdit = new QLineEdit;
        m_pcEnitySizeHeightLineEdit = new QLineEdit;
        m_pcEnitySizeRadiusLineEdit->setPlaceholderText("real number");
        m_pcEnitySizeHeightLineEdit->setPlaceholderText("real number");

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
        pcEntitySizeGroupBoxLayout->addWidget(pcEnitySizeRadiusLabel);
        pcEntitySizeGroupBoxLayout->addWidget(m_pcEnitySizeRadiusLineEdit);
        pcEntitySizeGroupBoxLayout->addWidget(pcEnitySizeHeightLabel);
        pcEntitySizeGroupBoxLayout->addWidget(m_pcEnitySizeHeightLineEdit);
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

    void CQTOpenGLCylinderEntityDialogLayout::CheckInput() 
    {
        CQTOpenGLEmbodiedEntityDialogLayout::CheckInput();

        // check size
        if(this->GetRadius() <= 0.0f) {
            THROW_ARGOSEXCEPTION("Please fill in a strict positive radius of the entity.");
        }
        if(this->GetHeight() <= 0.0f) {
            THROW_ARGOSEXCEPTION("Please fill in a strict positive height of the entity.");
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

    CEntity& CQTOpenGLCylinderEntityDialogLayout::GetEntity()
    {
        this->CheckInput();

        // calculate mass
        Real fEntityMass = 1.0f;
        if(this->IsMovable()){
            fEntityMass = this->GetMass();
        }

        // create new entity
        CCylinderEntity* cCylinderEnt = new CCylinderEntity(
            this->GetId(),
            this->GetPositition(),
            this->GetOrientation(),
            this->IsMovable(),
            this->GetRadius(),
            this->GetHeight(),
            fEntityMass
            );

        return *cCylinderEnt;
    }

    /****************************************/
    /****************************************/

    void CQTOpenGLCylinderEntityDialogLayout::MovableCheckboxToggled(bool b_is_checked)
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