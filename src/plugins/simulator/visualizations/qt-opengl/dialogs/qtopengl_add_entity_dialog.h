/**
 * @file <argos3/plugins/simulator/visualizations/qt-opengl/dialogs/qtopengl_add_entity_dialog.h>
 *
 * @author Ewout Merckx - <ewoutmerckx.pro@gmail.com>
 */

#ifndef QTOPENGL_ADD_ENTITY_DIALOG_H
#define QTOPENGL_ADD_ENTITY_DIALOG_H

#include <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_widget.h>
#include <argos3/plugins/simulator/visualizations/qt-opengl/dialogs/layouts/qtopengl_entity_dialog_layout.h>

#include <QComboBox>
#include <QDialog>
#include <QGroupBox>
#include <QLabel>
#include <QVBoxLayout>

namespace argos {

    class CQTOpenGLAddEntityDialog : public QDialog {

        Q_OBJECT

        public:

            /**
             * Constructor
             * @param pc_parent_widget Pointer to the parent widget
             * @param pc_openglwidget Pointer to the OpenGL widget
             */
            CQTOpenGLAddEntityDialog(QWidget* pc_parent_widget, CQTOpenGLWidget* pc_openglwidget);
            ~CQTOpenGLAddEntityDialog() { }

        private: 

            /**
             * Initialize the dialog box, add buttons, set the title
             */
            void Init();

        public slots:

            /**
             * Invoked when the user selects an other entity 
             * Load the form layout for the current selected entity type
             */
            void EntitySelectionChanged();

            /**
             * Invoked when the user presses OK
             * Add the entity to the arena, redraws the scene
             * Closes the dialog box afterwards
             */
            void Accept();

            /**
             * Invoked when the user presses cancel
             * Closes the dialog box
             */
            void Reject();

        private:

            /* Layout of the dialog box */
            QVBoxLayout* m_pcDialogLayout;

            /* Drop down for all entity types */
            QComboBox* m_pcEntityTypeComboBox;

            /* Group box for the form */
            QGroupBox* m_pcEntitySettingsGroupBox;

            /* Layout of the form for the entity */
            CQTOpenGLEntityDialogLayout* m_pcEntityDialogLayout;

            /* Red error label */
            QLabel* m_pcErrorLabel;

            /* Widget for drawing */
            CQTOpenGLWidget* m_pcOpenGLWidget;
    };
}

#endif
