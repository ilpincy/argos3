/**
 * @file <argos3/plugins/simulator/visualizations/qt-opengl/dialogs/layouts/qtopengl_light_entity_dialog_layout.cpp>
 *
 * @author Ewout Merckx - <ewoutmerckx.pro@gmail.com>
 */

#include "qtopengl_light_entity_dialog_layout.h"

#include <argos3/plugins/simulator/entities/light_entity.h>

#include <QGroupBox>
#include <QLabel>

namespace argos {

    /****************************************/
    /****************************************/

    CQTOpenGLLightEntityDialogLayout::CQTOpenGLLightEntityDialogLayout(QWidget* pc_parent_widget) 
        : CQTOpenGLEntityDialogLayout(pc_parent_widget)
    { 
        this->Init();
    }

    /****************************************/
    /****************************************/

    void CQTOpenGLLightEntityDialogLayout::Init() 
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

        // initialize color input field
        m_pcEnityColorComboBox = new QComboBox;
        std::vector<std::string> vecColors = CColor::GetDefaultColors();
        QStringList cColorStringList;
        for(size_t i=0; i<vecColors.size(); i++) {
            cColorStringList.append(vecColors[i].c_str());
        }
        m_pcEnityColorComboBox->addItems(cColorStringList);

        // initialize intensity input fields
        m_pcEnityIntensityLineEdit = new QLineEdit;
        m_pcEnityIntensityLineEdit->setPlaceholderText("real number");

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

        // add color input field to group box
        QGroupBox* pcEntityColorGroupBox = new QGroupBox(tr("Color*"));
        QHBoxLayout *pcEntityColorGroupBoxLayout = new QHBoxLayout;
        pcEntityColorGroupBoxLayout->addWidget(m_pcEnityColorComboBox);
        pcEntityColorGroupBox->setLayout(pcEntityColorGroupBoxLayout);

        // add color input field to group box
        QGroupBox* pcEntityIntensityGroupBox = new QGroupBox(tr("Intensity*"));
        QHBoxLayout *pcEntityIntensityGroupBoxLayout = new QHBoxLayout;
        pcEntityIntensityGroupBoxLayout->addWidget(m_pcEnityIntensityLineEdit);
        pcEntityIntensityGroupBox->setLayout(pcEntityIntensityGroupBoxLayout);

        // add group boxes to layout
        this->addWidget(pcEntityPositionGroupBox);
        this->addWidget(pcEntityColorGroupBox);
        this->addWidget(pcEntityIntensityGroupBox);
    }

    /****************************************/
    /****************************************/

    void CQTOpenGLLightEntityDialogLayout::CheckInput() 
    {
        CQTOpenGLEntityDialogLayout::CheckInput();

        // check color
        if(m_pcEnityColorComboBox->currentText().toStdString() == "") {
            THROW_ARGOSEXCEPTION("Please fill in a color for the entity.");
        }

        // check intensity
        if(m_pcEnityIntensityLineEdit->text().toStdString() == "" 
            || this->GetIntensity() < 0) {

            THROW_ARGOSEXCEPTION("Please fill in a positive intensity of the entity.");
        }
    }

    /****************************************/
    /****************************************/

    CEntity& CQTOpenGLLightEntityDialogLayout::GetEntity()
    {
        this->CheckInput();

        // create new entity
        CLightEntity* cLightEnt = new CLightEntity(
            this->GetId(),
            this->GetPositition(),
            this->GetColor(),
            this->GetIntensity()
            );

        return *cLightEnt;
    }

    /****************************************/
    /****************************************/

}