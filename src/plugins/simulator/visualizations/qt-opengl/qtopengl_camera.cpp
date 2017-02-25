/**
 * @file <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_camera.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "qtopengl_camera.h"
#include <QPoint>
#include <argos3/core/utility/math/quaternion.h>
#include <argos3/core/utility/logging/argos_log.h>

namespace argos {

   static const Real MOVE_GAIN = 0.005f / ::exp(0.02f);
   static const Real ROTATE_GAIN = 0.01f / ::exp(-0.02f);

   /****************************************/
   /****************************************/

   void CQTOpenGLCamera::Init(TConfigurationNode& t_tree) {
      if(NodeExists(t_tree, "camera")) {
         try {
            TConfigurationNode tCameraNode;
            tCameraNode = GetNode(t_tree, "camera");
            TConfigurationNodeIterator itSettingss;
            SInt32 nIdx;
            for(itSettingss = itSettingss.begin(&tCameraNode);
                itSettingss != itSettingss.end();
                ++itSettingss) {
               GetNodeAttribute(*itSettingss, "idx", nIdx);
               if(nIdx >=0 && nIdx <= 11) {
                  m_sSettings[nIdx].Init(*itSettingss);
               }
               else {
                  THROW_ARGOSEXCEPTION("Error initializing QTOpenGL camera settings: value given for 'idx' is out of bounds. Value = \"" << nIdx << "\", allowed [0-9].");
               }
            }
         }
         catch(CARGoSException& ex) {
            THROW_ARGOSEXCEPTION_NESTED("Error initializing QTOpenGL camera settings", ex);
         }
      }
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLCamera::SSettings::Init(TConfigurationNode& t_tree) {
      /* Get positional attributes */
      GetNodeAttribute(t_tree, "position", Position);
      GetNodeAttribute(t_tree, "look_at", Target);
      /* Calculate the Forward vector */
      Forward = (Target - Position).Normalize();
      /* Was the up vector specified? */
      if(!NodeAttributeExists(t_tree, "up")) {
         /* Calculate the Left vector
            It is located on a plane parallel to XY
            It is perpendicular to the projection of Forward on that plane
         */
         if(Forward.GetX() != 0 || Forward.GetY() != 0) {
            CVector2 cLeftXY(Forward.GetX(), Forward.GetY());
            cLeftXY.Perpendicularize();
            Left.Set(cLeftXY.GetX(), cLeftXY.GetY(), 0.0f);
            Left.Normalize();
         }
         else {
            LOGERR << "[WARNING] The given camera position is ambiguous, "
                   << "and a standard attitude has been used to calculate it. "
                   << "Consider specifying the \"up\" vector in the .argos file "
                   << "for more precise placement."
                   << std::endl;
            Left.Set(0.0f, 1.0f, 0.0f);
         }
         /* Calculate the Up vector with a cross product */
         Up = Forward;
         Up.CrossProduct(Left).Normalize();
      }
      else {
         /* Get Up vector */
         GetNodeAttribute(t_tree, "up", Up);
         Up.Normalize();
         /* Calculate the Left vector with a cross product */
         Left = Up;
         Left.CrossProduct(Forward).Normalize();
      }
      /* Get optional optics parameters */
      Real fValue;
      GetNodeAttributeOrDefault<Real>(t_tree, "lens_focal_length", fValue, 20.0f);
      LensFocalLength = fValue * 0.001f;
      CalculateYFieldOfView();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLCamera::SSettings::RotateUpDown(const CRadians& c_angle) {
      /* Rotate around the local Y axis (Left)
         The rotation matrix corresponding to this rotation is:
         
         | Fcos(a)-Usin(a)   L   Ucos(a)+Fsin(a) |
         
         where a is c_angle
         L is the Left vector (local Y)
         U is the Up vector (local Z)
         F is the Forward vector (local X)
      */
      /* Calculate the new Up vector, given by:
         Ucos(a)+Fsin(a)
      */
      /* Same, but faster than
         cNewUp = Up * Cos(c_angle) + Forward * Sin(c_angle);
      */
      CVector3 cNewUp(Up);
      cNewUp *= Cos(c_angle);
      cNewUp += Forward * Sin(c_angle);
      /* Check whether the rotation exceeds the limits */
      if(cNewUp.GetAngleWith(CVector3::Z) > CRadians::PI_OVER_TWO) {
         /*
          * The rotation exceeds the limits
          * The camera Up vector would form an angle bigger than 90 degrees with
          * the global Z axis
          */
         /* We force the Up vector to lie on the XY plane */
         Up.SetZ(0.0f);
         Up.Normalize();
         if(Forward.GetZ() < 0.0f) {
            /* Forward was looking down, set it to -Z */
            Forward = -CVector3::Z;
         }
         else {
            /* Forward was looking up, set it to Z */
            Forward = CVector3::Z;
         }
      }
      else {
         /* The rotation is OK */
         Up = cNewUp;
         Up.Normalize();
         /* Now calculate the new Forward vector with a cross product
            NOTE: the Left vector, being the rotation axis, remains
            unchanged!
         */
         Forward = Left;
         Forward.CrossProduct(Up).Normalize();
      }
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLCamera::SSettings::RotateLeftRight(const CRadians& c_angle) {
      /* Rotate around the local Z axis (Up)
         The rotation matrix corresponding to this rotation is:
         
         | Fcos(a)+Lsin(a)   -Fsin(a)+Lcos(a)   U |
         
         where a is c_angle
         L is the Left vector (local Y)
         U is the Up vector (local Z)
         F is the Forward vector (local X)
      */

      /* Calculate the new Forward vector, given by:
         Fcos(a)+Lsin(a)
      */
      /* Same, but faster than
         Forward = Forward * Cos(c_angle) + Left * Sin(c_angle);
      */
      Forward *= Cos(c_angle);
      Forward += Left * Sin(c_angle);
      Forward.Normalize();
      /* Now calculate the new Left vector with a cross product
         NOTE: the Up vector, being the rotation axis, remains
         unchanged!
      */
      Left = Up;
      Left.CrossProduct(Forward).Normalize();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLCamera::SSettings::RotateLeftRight2(const CRadians& c_angle) {
      /* This rotation is performed around the global Z axis.
         To this aim, along with the global Z axis we project the
         Forward and Left axes onto the XY plane and use these as
         additional axis to build up the rotation matrix.
         With the matrix, we rotate the projected Forward and Left vectors.
         We then transform the projected vectors into the final ones.
         Finally, we cross-product the two vectors to obtain the new Up vector.
      */
      if(Forward.GetX() != 0 || Forward.GetY() != 0) {
         /* Project Forward onto the XY axis */
         CVector3 cForwardXY(Forward.GetX(), Forward.GetY(), 0.0f);
         /* Save its length: it will be used to restore the Z coordinate correctly */
         Real cForwardXYLength = cForwardXY.Length();
         /* Normalize the projection */
         cForwardXY.Normalize();
         
         /* Project Left onto the XY axis and normalize the result */
         CVector3 cLeftXY = CVector3::Z;
         cLeftXY.CrossProduct(cForwardXY).Normalize();
         
         /* The rotation matrix corresponding to this rotation is:
            
            | Fcos(a)+Lsin(a)   -Fsin(a)+Lcos(a)   Z |
            
            where a is c_angle
            L is the Left vector projected on XY
            F is the Forward vector projected on XY
            Z is the global Z vector
         */
         
         /* Calculate the new cForwardXY vector, given by:
            Fcos(a)+Lsin(a)
            We keep the unrotated cForwardXY vector, because we will
            need it for rotating Left too.
         */
         /* Same, but faster than
            CVector3 cNewForwardXY = cForwardXY * Cos(c_angle) + cLeftXY * Sin(c_angle);
         */
         CVector3 cNewForwardXY(cForwardXY);
         cNewForwardXY *= Cos(c_angle);
         cNewForwardXY += cLeftXY * Sin(c_angle);
         cNewForwardXY.Normalize();
         /* Update Forward from cNewForwardXY: we want to keep the Z coordinate
            of Forward unchanged cause we rotated around the global Z, but we
            need to update the X and Y coordinates. Right now, cNewForwardXY has
            zero Z coordinate and length 1, which means that its X and Y are wrong.
            To get the right values, we need to scale it back to the length of the
            projection of Forward onto the XY plane. Once this is done, the X and Y
            coordinates are right.
         */
         /* Scale the vector back to the right length */
         cNewForwardXY *= cForwardXYLength;
         /* Finally, update Forward */
         Forward.SetX(cNewForwardXY.GetX());
         Forward.SetY(cNewForwardXY.GetY());
         Forward.Normalize();
         
         /* Calculate the new Left vector, given by:
            -Fsin(a)+Lcos(a)
         */
         /* Same, but faster than
            Left = cLeftXY * Cos(c_angle) - cForwardXY * Sin(c_angle);
         */
         Left = cLeftXY;
         Left *= Cos(c_angle);
         Left -= cForwardXY * Sin(c_angle);
         Left.Normalize();
         
         /* To calculate the new Up vector, a cross-product is enough */
         Up = Forward;
         Up.CrossProduct(Left).Normalize();
      }
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLCamera::SSettings::Translate(const CVector3& c_delta) {
      Position += Forward * c_delta.GetX() + Left * c_delta.GetY() + Up * c_delta.GetZ();
      Target = Position;
      Target += Forward;
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLCamera::SSettings::Do() {
      gluLookAt(
         Position.GetX(),
         Position.GetY(),
         Position.GetZ(),
         Target.GetX(),
         Target.GetY(),
         Target.GetZ(),
         Up.GetX(),
         Up.GetY(),
         Up.GetZ());
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLCamera::SSettings::CalculateYFieldOfView() {
      YFieldOfView = ToDegrees(2.0f * ATan2(0.027f * 0.5f, LensFocalLength));
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLCamera::SSettings::CalculateSensitivity() {
      MotionSensitivity = MOVE_GAIN * ::exp(LensFocalLength);
      RotationSensitivity = ROTATE_GAIN * ::exp(-LensFocalLength);
   }

   /****************************************/
   /****************************************/

   CQTOpenGLCamera::CQTOpenGLCamera() :
      m_unActiveSettings(0) {
   }

   /****************************************/
   /****************************************/

   CQTOpenGLCamera::~CQTOpenGLCamera() {
   }
   
   /****************************************/
   /****************************************/

   void CQTOpenGLCamera::Rotate(const QPoint& c_delta) {
      m_sSettings[m_unActiveSettings]
         .RotateUpDown(CRadians(m_sSettings[m_unActiveSettings].RotationSensitivity * c_delta.y()));
      m_sSettings[m_unActiveSettings]
         .RotateLeftRight2(CRadians(-m_sSettings[m_unActiveSettings].RotationSensitivity * c_delta.x()));
      m_sSettings[m_unActiveSettings]
         .Target = m_sSettings[m_unActiveSettings].Position;
      m_sSettings[m_unActiveSettings]
         .Target += m_sSettings[m_unActiveSettings].Forward;
   }
   
   /****************************************/
   /****************************************/

   void CQTOpenGLCamera::Move(SInt32 n_forwards_backwards,
                              SInt32 n_sideways,
                              SInt32 n_up_down) {
      m_sSettings[m_unActiveSettings].Translate(
         CVector3(m_sSettings[m_unActiveSettings].MotionSensitivity * n_forwards_backwards,
                  m_sSettings[m_unActiveSettings].MotionSensitivity * n_sideways,
                  m_sSettings[m_unActiveSettings].MotionSensitivity * n_up_down));
   }

   /****************************************/
   /****************************************/

}
