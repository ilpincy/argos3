/**
 * @file <argos3/plugins/simulator/visualizations/qt-opengl/dialogs/layouts/qtopengl_epuck_entity_dialog_layout.h>
 *
 * @author Ewout Merckx - <ewoutmerckx.pro@gmail.com>
 */

#ifndef QTOPENGL_EPUCK_ENTITY_DIALOG_LAYOUT
#define QTOPENGL_EPUCK_ENTITY_DIALOG_LAYOUT

namespace argos {
    class CQTOpenGLEPuckEntityDialogLayout;
}

#include <argos3/plugins/simulator/visualizations/qt-opengl/dialogs/layouts/qtopengl_bot_entity_dialog_layout.h>

namespace argos {

    class CQTOpenGLEPuckEntityDialogLayout : public CQTOpenGLBotEntityDialogLayout {

        Q_OBJECT

        public:

            /**
             * Constructor.
             * @param pc_parent_widget The pointer to the parent widget
             */
            CQTOpenGLEPuckEntityDialogLayout(QWidget* pc_parent_widget = 0);

            /**
             * Creates and returns the e-puck entity using the specified values
             * @returns The entity using the specified values
             */
            virtual CEntity& GetEntity();

            /**
             * Getter for the range and bearing range
             * @returns The real number containing the specified range and bearing range value
             */
            inline Real GetRabRange() const
            {
                return m_pcEnityRabRangeLineEdit->text().toDouble();
            }

            /**
             * Getter for the range and bearing data size
             * @returns The natural number containing the specified range and bearing data size value
             */
            inline size_t GetRabDataSize() const
            {
                return m_pcEnityRabDataSizeLineEdit->text().toUInt();
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
            // rab input fields
            QLineEdit* m_pcEnityRabRangeLineEdit;
            QLineEdit* m_pcEnityRabDataSizeLineEdit;
    };
}

#endif