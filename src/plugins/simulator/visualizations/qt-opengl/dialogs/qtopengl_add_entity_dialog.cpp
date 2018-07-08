/**
 * @file <argos3/plugins/simulator/visualizations/qt-opengl/dialogs/qtopengl_add_entity_dialog.cpp>
 *
 * @author Ewout Merckx - <ewoutmerckx.pro@gmail.com>
 */

#include "qtopengl_add_entity_dialog.h"

#include <argos3/core/simulator/query_plugins.h>
#include <argos3/core/simulator/loop_functions.h>

#include <QDialogButtonBox>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QString>

#include <argos3/plugins/simulator/visualizations/qt-opengl/dialogs/layouts/qtopengl_box_entity_dialog_layout.h>

namespace argos {

    /****************************************/
    /****************************************/

    CQTOpenGLAddEntityDialog::CQTOpenGLAddEntityDialog(QWidget *pc_parent_widget, CQTOpenGLWidget* pc_openglwidget) 
      : QDialog(pc_parent_widget)
    {
        /* Delete the dialog box when closed */
        this->setAttribute(Qt::WA_DeleteOnClose);

        /* Set the widget */
        m_pcOpenGLWidget = pc_openglwidget;

        /* Initialize the layout */
        Init();  

        /* Get the layout for the current entity type */
        EntitySelectionChanged();
    }

    /****************************************/
    /****************************************/

    void CQTOpenGLAddEntityDialog::Init() 
    {
        /* Initialize dialog layout */
        m_pcDialogLayout = new QVBoxLayout;

        /* Initialize entity type input field */
        std::vector<std::string> vecEntityTypes;
        std::string strQuery = "entities";        
        GetPlugins(strQuery, vecEntityTypes);
        QStringList cEntityTypeStringList;
        for(size_t i=0; i<vecEntityTypes.size(); i++){
            cEntityTypeStringList.append(vecEntityTypes[i].c_str());
        }
        m_pcEntityTypeComboBox = new QComboBox;
        m_pcEntityTypeComboBox->addItems(cEntityTypeStringList);

        connect(m_pcEntityTypeComboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(EntitySelectionChanged()));

        /* Initialize entity settings group box */
        m_pcEntitySettingsGroupBox = new QGroupBox(tr("Entity settings"));;

        /* Initialize mandatory settings label */
        QLabel* m_pqlabelEntityMandatory = new QLabel(
            tr("Fields marked with an asterisk are required information."));

        /* initialize error label */
        m_pcErrorLabel = new QLabel(tr(""));
        m_pcErrorLabel->setStyleSheet("QLabel { color : red; }");

        /* Initialize buttons */
        QDialogButtonBox* pcAddDialogButtonBox;
        pcAddDialogButtonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
        connect(pcAddDialogButtonBox, SIGNAL(accepted()), this, SLOT(Accept()));
        connect(pcAddDialogButtonBox, SIGNAL(rejected()), this, SLOT(Reject()));

        /* Add entity type input field to group box */
        QGroupBox* pcEntityTypeGroupBox = new QGroupBox(tr("Entity type*"));
        QHBoxLayout* pcEntityTypeGroupBoxLayout = new QHBoxLayout;
        pcEntityTypeGroupBoxLayout->addWidget(m_pcEntityTypeComboBox);
        pcEntityTypeGroupBox->setLayout(pcEntityTypeGroupBoxLayout);
        pcEntityTypeGroupBox->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);

        /* Add group boxes to layout */
        m_pcDialogLayout->addWidget(pcEntityTypeGroupBox);
        m_pcDialogLayout->addWidget(m_pcEntitySettingsGroupBox);
        m_pcDialogLayout->addWidget(m_pqlabelEntityMandatory);
        m_pcDialogLayout->addWidget(m_pcErrorLabel);
        m_pcDialogLayout->addWidget(pcAddDialogButtonBox);

        /* Set the title and the layout of the dialog */
        this->setWindowTitle(tr("Add a new entity"));
        this->setLayout(m_pcDialogLayout);
    }

    /****************************************/
    /****************************************/

    void CQTOpenGLAddEntityDialog::EntitySelectionChanged(){
        /* Reset error */
        bool bErrorOccured = false;
        m_pcErrorLabel->setText(QString(""));

        /* Remove the previous entity settings group box*/
        delete m_pcEntitySettingsGroupBox;
        m_pcEntitySettingsGroupBox = new QGroupBox(tr("Entity settings"));

        /* Get the entity from the combobox */
        std::string strEntityType = m_pcEntityTypeComboBox->currentText().toStdString();

        /* Load the specific layout */
        if(strEntityType == "box") {
            m_pcEntityDialogLayout = new CQTOpenGLBoxEntityDialogLayout; 
        }
        else {
            bErrorOccured = true;
        }

        /* If no error occurred, apply the layout */
        if(!bErrorOccured){
            m_pcEntitySettingsGroupBox->setLayout(m_pcEntityDialogLayout);
            m_pcDialogLayout->insertWidget(1, m_pcEntitySettingsGroupBox);
        }
        else {
            m_pcErrorLabel->setText("The dialog layout for that entity hasn't been implemented yet.");
        }
    }

    /****************************************/
    /****************************************/

    void CQTOpenGLAddEntityDialog::Accept(){
        /* Check the currently supported entities */
        if(m_pcEntityTypeComboBox->currentText().toStdString() != "box") {
            return;
        }

        /* Reset error text */
        m_pcErrorLabel->setText(QString(""));

        /* Try to get the entity for the filled in values
         * If succeeded, add it and close the dialog box
         * Else, show error to the user */
        try {

            CEntity* pcEntity = &m_pcEntityDialogLayout->GetEntity();

            /* If type is floor, it was already added in layout class */
            if(pcEntity->GetTypeDescription() != "floor"){
                CSimulator::GetInstance().GetLoopFunctions().AddEntity(*pcEntity);
            }
            else{
                // comment out: will be useful when floor can be changed
                //m_pcOpenGLWidget->ResetFloor();
            }

            /* Draw the scene */
            //m_pcOpenGLWidget->DrawScene();
            /* Close the dialog box */
            QDialog::close();
        }
        catch(CARGoSException& ex) {
            m_pcErrorLabel->setText(QString(ex.what()));
        }
    }

    /****************************************/
    /****************************************/

    void CQTOpenGLAddEntityDialog::Reject(){
        /* Close the dialog box */
        QDialog::close();
    }

    /****************************************/
    /****************************************/

}
