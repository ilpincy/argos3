/**
 * @file <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_camera.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "qtopengl_camera.h"
#include <QPoint>
#include <argos3/core/utility/math/quaternion.h>
#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/space/space.h>

namespace argos {

   static const Real MOVE_GAIN = 0.005f / ::exp(0.02f);
   static const Real ROTATE_GAIN = 0.01f / ::exp(-0.02f);

   /****************************************/
   /****************************************/

   void CQTOpenGLCamera::SPlacement::Init(TConfigurationNode& t_tree) {
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

   void CQTOpenGLCamera::SPlacement::RotateUpDown(const CRadians& c_angle) {
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

   void CQTOpenGLCamera::SPlacement::RotateLeftRight(const CRadians& c_angle) {
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

   void CQTOpenGLCamera::SPlacement::RotateLeftRight2(const CRadians& c_angle) {
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

   void CQTOpenGLCamera::SPlacement::Translate(const CVector3& c_delta) {
      Position += Forward * c_delta.GetX() + Left * c_delta.GetY() + Up * c_delta.GetZ();
      Target = Position;
      Target += Forward;
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLCamera::SPlacement::Do() {
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

   void CQTOpenGLCamera::SPlacement::CalculateYFieldOfView() {
      YFieldOfView = ToDegrees(2.0f * ATan2(0.027f * 0.5f, LensFocalLength));
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLCamera::SPlacement::CalculateSensitivity() {
      MotionSensitivity = MOVE_GAIN * ::exp(LensFocalLength);
      RotationSensitivity = ROTATE_GAIN * ::exp(-LensFocalLength);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLCamera::STimelineItem::Init(TConfigurationNode& t_tree) {
      GetNodeAttribute(t_tree, "idx", Idx);
      if(Idx > 11) {
         THROW_ARGOSEXCEPTION("Error initializing QTOpenGL camera timeline: value given for 'idx' is out of bounds. Value = \"" << Idx << "\", allowed [0-11].");
      }
      GetNodeAttribute(t_tree, "step", Step);
   }

   /****************************************/
   /****************************************/

   CQTOpenGLCamera::CQTOpenGLCamera() :
      m_unActivePlacement(0) {
   }

   /****************************************/
   /****************************************/

   CQTOpenGLCamera::~CQTOpenGLCamera() {
      while(!m_listTimeline.empty()) {
         delete m_listTimeline.back();
         m_listTimeline.pop_back();
      }
   }
   
   /****************************************/
   /****************************************/

   void CQTOpenGLCamera::Init(TConfigurationNode& t_tree) {
      if(NodeExists(t_tree, "camera")) {
         try {
            TConfigurationNode tCameraNode;
            tCameraNode = GetNode(t_tree, "camera");
            /* Parse "placement" nodes */
            TConfigurationNodeIterator itPlacement("placement");
            SInt32 nIdx;
            for(itPlacement = itPlacement.begin(&tCameraNode);
                itPlacement != itPlacement.end();
                ++itPlacement) {
               GetNodeAttribute(*itPlacement, "idx", nIdx);
               if(nIdx >=0 && nIdx <= 11) {
                  m_sPlacement[nIdx].Init(*itPlacement);
               }
               else {
                  THROW_ARGOSEXCEPTION("Error initializing QTOpenGL camera placement: value given for 'idx' is out of bounds. Value = \"" << nIdx << "\", allowed [0-11].");
               }
            }
            /* Parse "timeline" nodes */
            TConfigurationNodeIterator itTimeline("timeline");
            for(itTimeline = itTimeline.begin(&tCameraNode);
                itTimeline != itTimeline.end();
                ++itTimeline) {
               /* Create timeline item */
               STimelineItem* psTimelineItem = new STimelineItem();
               psTimelineItem->Init(*itTimeline);
               /* Sorted insert */
               if(m_listTimeline.empty())
                  m_listTimeline.push_back(psTimelineItem);
               else {
                  std::list<STimelineItem*>::iterator it = m_listTimeline.begin();
                  while(it != m_listTimeline.end() && (*it)->Step < psTimelineItem->Step) ++it;
                  if(it == m_listTimeline.end())
                     m_listTimeline.push_back(psTimelineItem);
                  else
                     m_listTimeline.insert(it, psTimelineItem);
               }
            }
            for(std::list<STimelineItem*>::iterator it = m_listTimeline.begin();
                it != m_listTimeline.end();
                ++it) {
            }
         }
         catch(CARGoSException& ex) {
            THROW_ARGOSEXCEPTION_NESTED("Error initializing QTOpenGL camera placement", ex);
         }
      }
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLCamera::Reset() {
      m_unActivePlacement = 0;
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLCamera::Rotate(const QPoint& c_delta) {
      m_sPlacement[m_unActivePlacement]
         .RotateUpDown(CRadians(m_sPlacement[m_unActivePlacement].RotationSensitivity * c_delta.y()));
      m_sPlacement[m_unActivePlacement]
         .RotateLeftRight2(CRadians(-m_sPlacement[m_unActivePlacement].RotationSensitivity * c_delta.x()));
      m_sPlacement[m_unActivePlacement]
         .Target = m_sPlacement[m_unActivePlacement].Position;
      m_sPlacement[m_unActivePlacement]
         .Target += m_sPlacement[m_unActivePlacement].Forward;
   }
   
   /****************************************/
   /****************************************/

   void CQTOpenGLCamera::Move(SInt32 n_forwards_backwards,
                              SInt32 n_sideways,
                              SInt32 n_up_down) {
      m_sPlacement[m_unActivePlacement].Translate(
         CVector3(m_sPlacement[m_unActivePlacement].MotionSensitivity * n_forwards_backwards,
                  m_sPlacement[m_unActivePlacement].MotionSensitivity * n_sideways,
                  m_sPlacement[m_unActivePlacement].MotionSensitivity * n_up_down));
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLCamera::UpdateTimeline() {
      /* If no timeline, nothing to do */
      if(m_listTimeline.empty()) return;
      /* Get current timestep */
      UInt32 unStep = CSimulator::GetInstance().GetSpace().GetSimulationClock();
      /* Search for active timeline item */
      std::list<STimelineItem*>::iterator it = m_listTimeline.begin();
      /* Is the first item potentially active? */
      if((*it)->Step <= unStep) {
         /* Yes, save it and search for subsequent items */
         STimelineItem* psTimelineItem = *it;
         ++it;
         while(it != m_listTimeline.end() && (*it)->Step <= unStep) {
            psTimelineItem = *it;
            ++it;
         }
         /* Set active placement according to currently active timeline item */
         m_unActivePlacement = psTimelineItem->Idx;
      }
   }

   /****************************************/
   /****************************************/

}
