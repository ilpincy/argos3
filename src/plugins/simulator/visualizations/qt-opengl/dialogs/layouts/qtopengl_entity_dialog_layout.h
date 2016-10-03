/**
 * @file <argos3/plugins/simulator/visualizations/qt-opengl/dialogs/layouts/qtopengl_entity_dialog_layout.h>
 *
 * @author Ewout Merckx - <ewoutmerckx.pro@gmail.com>
 */

#ifndef QTOPENGL_ENTITY_DIALOG_LAYOUT
#define QTOPENGL_ENTITY_DIALOG_LAYOUT

namespace argos {
    class CQTOpenGLEntityDialogLayout;
}

#include <argos3/core/simulator/entity/entity.h>

#include <QBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>

namespace argos {

    class CQTOpenGLEntityDialogLayout : public QVBoxLayout {

        Q_OBJECT

        public:

            /**
             * Constructor.
             * @param pc_parent_widget The pointer to the parent widget
             */
            CQTOpenGLEntityDialogLayout(QWidget* pc_parent_widget = 0);

            /**
             * Creates and returns the entity using the specified values
             * @returns The entity using the specified values
             */
            virtual CEntity& GetEntity();

            /**
             * Getter for the id
             * @returns The string containing the specified id value
             */
            inline std::string GetId() const
            {
                return m_pcEnityIdLineEdit->text().toStdString();
            }

        protected:

            /**
             * Sets the text of the id input field to the given string
             */
            void SetEntityIdLineEditText(const std::string& str_entity_id);

            /**
             * Disables the id input field
             */
            void DisableEntityIdLineEdit();

            /**
             * Enables the id input field
             */
            void EnableEntityIdLineEdit();

            /**
             * Validates the filled in values
             */
            virtual void CheckInput();

            /**
             * Creates and returns a horizontal group box
             * The group box gets the given title
             * In this group box, the given label and 
             * the given input field are placed
             * @param str_title The title for the group box
             * @param c_line_edit The input field that will be inserted
             */
            QGroupBox* GetHorizontalGroupBox(std::string& str_title, 
                QLineEdit* c_line_edit) const;


            /**
             * Creates and returns a horizontal group box
             * The group box gets the given title
             * In this group box, the given label and 
             * the given input field are placed
             * @param str_title The title for the group box
             * @param c_label The label for the input field
             * @param c_line_edit The input field that will be inserted
             */
            QGroupBox* GetHorizontalGroupBox(std::string& str_title, 
                QLabel* c_label, QLineEdit* c_line_edit) const;

        private:

            /**
             * Initializes the layout
             */
            virtual void Init();

        public slots:

        private:
            // id input field
            QLineEdit* m_pcEnityIdLineEdit;
    };
}

#endif