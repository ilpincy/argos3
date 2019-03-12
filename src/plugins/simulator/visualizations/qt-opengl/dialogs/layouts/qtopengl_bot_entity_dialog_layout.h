/**
 * @file <argos3/plugins/simulator/visualizations/qt-opengl/dialogs/layouts/qtopengl_bot_entity_dialog_layout.h>
 *
 * @author Ewout Merckx - <ewoutmerckx.pro@gmail.com>
 */

#ifndef QTOPENGL_BOT_ENTITY_DIALOG_LAYOUT
#define QTOPENGL_BOT_ENTITY_DIALOG_LAYOUT

namespace argos {
    class CQTOpenGLBotEntityDialogLayout;
}

#include <argos3/plugins/simulator/visualizations/qt-opengl/dialogs/layouts/qtopengl_embodied_entity_dialog_layout.h>

#include <argos3/core/simulator/simulator.h>

#include <QComboBox>

namespace argos {

    class CQTOpenGLBotEntityDialogLayout : public CQTOpenGLEmbodiedEntityDialogLayout {

        Q_OBJECT

        public:
            
            /**
             * Constructor.
             * @param pc_parent_widget The pointer to the parent widget
             */
            CQTOpenGLBotEntityDialogLayout(QWidget* pc_parent_widget = 0);

            /**
             * Creates and returns the entity using the specified values
             * @returns The entity using the specified values
             */
            virtual CEntity& GetEntity();

            /**
             * Getter for the controller id
             * @returns The controller id specified in the combo box
             */
            inline std::string GetControllerId() const
            {
                return m_pcEnityControllerIdComboBox->currentText().toStdString();
            }

        protected:

            /**
             * Validates the filled in values
             */
            virtual void CheckInput();

        private:

            /**
             * Initializes the layout
             */
            virtual void Init();

        public slots:

        private:
            QComboBox* m_pcEnityControllerIdComboBox;
            
    };
}

#endif