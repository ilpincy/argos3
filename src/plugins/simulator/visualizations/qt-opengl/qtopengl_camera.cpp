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

   static const Real MOVE_GAIN = 0.005 / Exp(0.02);
   static const Real ROTATE_GAIN = 0.01 / Exp(-0.02);

   /****************************************/
   /****************************************/

   void CQTOpenGLCamera::SPlacement::Init(TConfigurationNode& t_tree) {
      /* Get positional attributes */
      GetNodeAttribute(t_tree, "position", Position);
      GetNodeAttribute(t_tree, "look_at", Target);
      /* Was the up vector specified? */
      if(!NodeAttributeExists(t_tree, "up")) {
         /* Calculate the Left vector
            It is located on a plane parallel to XY
            It is perpendicular to the projection of Forward on that plane
         */
         CVector3 cForward((Target - Position).Normalize());
         CVector3 cLeft;
         if(cForward.GetX() != 0 || cForward.GetY() != 0) {
            CVector2 cLeftXY(cForward.GetX(), cForward.GetY());
            cLeftXY.Perpendicularize();
            cLeft.Set(cLeftXY.GetX(), cLeftXY.GetY(), 0.0f);
            cLeft.Normalize();
         }
         else {
            LOGERR << "[WARNING] The given camera position is ambiguous, "
                   << "and a standard attitude has been used to calculate it. "
                   << "Consider specifying the \"up\" vector in the .argos file "
                   << "for more precise placement."
                   << std::endl;
            cLeft = CVector3::Y;
         }
         /* Calculate the Up vector with a cross product */
         Up = cForward;
         Up.CrossProduct(cLeft).Normalize();
      }
      else {
         /* Get Up vector */
         GetNodeAttribute(t_tree, "up", Up);
         Up.Normalize();
      }
      /* Get focal length */
      Real fValue;
      GetNodeAttributeOrDefault<Real>(t_tree, "lens_focal_length", fValue, 20.0f);
      LensFocalLength = fValue * 0.001f;
      CalculateYFieldOfView();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLCamera::SPlacement::CalculateYFieldOfView() {
      YFieldOfView = ToDegrees(2.0f * ATan2(0.027f * 0.5f, LensFocalLength));
   }

   /****************************************/
   /****************************************/

   CQTOpenGLCamera::CQTOpenGLCamera() :
      m_bEnableTimeline(false),
      m_bHasTimeline(false),
      m_unLoop(0) {
      /* automatically place the cameras at 30 degree increments
         around the arena */
      const CVector3& cArenaSize = 
         CSimulator::GetInstance().GetSpace().GetArenaSize();
      const CVector3& cArenaCenter = 
         CSimulator::GetInstance().GetSpace().GetArenaCenter(); 
      for(UInt32 un_index = 0; 
            un_index < m_arrPlacements.size();
            ++un_index) {
         CRadians cAngle(CRadians::PI_OVER_SIX);
         cAngle *= static_cast<Real>(un_index);
         m_arrPlacements[un_index].Position = 
            CVector3::X * cArenaSize.Length();
         m_arrPlacements[un_index].Position.RotateZ(cAngle);
         m_arrPlacements[un_index].Position += cArenaCenter;
         m_arrPlacements[un_index].Target = cArenaCenter;
         m_arrPlacements[un_index].Target.SetZ(0);
         /* (position - target).Normalize() ? */
         m_arrPlacements[un_index].Up = CVector3::Z;
         m_arrPlacements[un_index].LensFocalLength = 0.08;
         m_arrPlacements[un_index].CalculateYFieldOfView();
      }
   }

   /****************************************/
   /****************************************/

   CQTOpenGLCamera::~CQTOpenGLCamera() {}
   
   /****************************************/
   /****************************************/

   void CQTOpenGLCamera::Init(TConfigurationNode& t_tree) {
      try {
         if(NodeExists(t_tree, "placements")) {
            TConfigurationNode& tPlacementsNode = GetNode(t_tree, "placements");
            /* Parse the placement nodes */
            TConfigurationNodeIterator itPlacement("placement");
            for(itPlacement = itPlacement.begin(&tPlacementsNode);
                  itPlacement != itPlacement.end();
                  ++itPlacement) {
               UInt32 unIndex = 0;
               GetNodeAttribute(*itPlacement, "index", unIndex);
               if(unIndex < m_arrPlacements.size()) {
                  m_arrPlacements[unIndex].Init(*itPlacement);
               }
               else {
                  THROW_ARGOSEXCEPTION("Placement index is out of bounds");
               }
            }
         }
         /* Parse the timeline nodes */
         if(NodeExists(t_tree, "timeline")) {
            m_bHasTimeline = true;
            TConfigurationNode& tTimelineNode = GetNode(t_tree, "timeline");
            /* When does the timeline loop */
            GetNodeAttributeOrDefault(tTimelineNode, "loop", m_unLoop, m_unLoop);
            /* Parse the placement nodes */
            TConfigurationNodeIterator itTimelineElement;
            UInt32 unLastStep = 0;
            UInt32 unKeyframeStep = 0;
            UInt32 unKeyframePlacement = 0;
            for(itTimelineElement = itTimelineElement.begin(&tTimelineNode);
               itTimelineElement != itTimelineElement.end();
               ++itTimelineElement) {
               /* Create timeline item */
               if(itTimelineElement->Value() == "keyframe") {
                  GetNodeAttribute(*itTimelineElement, "placement", unKeyframePlacement);
                  GetNodeAttribute(*itTimelineElement, "step", unKeyframeStep);
                  if(unKeyframeStep < unLastStep) {
                     THROW_ARGOSEXCEPTION("Keyframes must be in increasing order");
                  }
                  if(m_unLoop && unKeyframeStep > m_unLoop) {
                     THROW_ARGOSEXCEPTION("Keyframe step must be less than the loop period");
                  }
                  else {
                     unLastStep = unKeyframeStep;
                     m_vecKeyframes.emplace_back(unKeyframeStep,
                                                 unKeyframePlacement,
                                                 false);
                  }
               }
               else if(itTimelineElement->Value() == "interpolate") {
                  if(!m_vecKeyframes.empty()) {
                     m_vecKeyframes.back().InterpolateToNext = true;
                  }
               }
            }
         }
         /* Reset the camera */
         Reset();
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Error initializing QTOpenGL camera", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLCamera::Reset() {
      if(m_bHasTimeline) {
         m_bEnableTimeline = true;
         UpdateTimeline();
      }
      else {
         SetActivePlacement(0);
      }
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLCamera::Look() {
      const CVector3& cPosition = GetActivePlacement().Position;
      const CVector3& cTarget = GetActivePlacement().Target;
      const CVector3& cUp = GetActivePlacement().Up;
      gluLookAt(cPosition.GetX(),
                cPosition.GetY(),
                cPosition.GetZ(),
                cTarget.GetX(),
                cTarget.GetY(),
                cTarget.GetZ(),
                cUp.GetX(),
                cUp.GetY(),
                cUp.GetZ());
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLCamera::Rotate(const QPoint& c_delta) {
      /* disable timeline on rotate */
      m_bEnableTimeline = false;
      /* Calculate cForward and cLeft */
      CVector3 cForward((m_sActivePlacement.Target - m_sActivePlacement.Position).Normalize());
      CVector3 cLeft = m_sActivePlacement.Up;
      cLeft.CrossProduct(cForward).Normalize();
      /* Calculate fRotationSensitivity */
      Real fRotationSensitivity = 
         ROTATE_GAIN * Exp(-m_sActivePlacement.LensFocalLength);
      /* rotate the camera */
      CRadians cUpDownRotation(fRotationSensitivity * c_delta.y());
      CVector3 cNewUp(m_sActivePlacement.Up);
      cNewUp *= Cos(cUpDownRotation);
      cNewUp += cForward * Sin(cUpDownRotation);
      /* Check whether the rotation exceeds the limits */
      if(cNewUp.GetAngleWith(CVector3::Z) > CRadians::PI_OVER_TWO) {
         m_sActivePlacement.Up.SetZ(0.0f);
         m_sActivePlacement.Up.Normalize();
         if(cForward.GetZ() < 0.0f) {
            cForward = -CVector3::Z;
         }
         else {
            cForward = CVector3::Z;
         }
      }
      else {
         /* The rotation is OK */
         m_sActivePlacement.Up = cNewUp;
         m_sActivePlacement.Up.Normalize();
         cForward = cLeft;
         cForward.CrossProduct(m_sActivePlacement.Up).Normalize();
      }
      if(cForward.GetX() != 0 || cForward.GetY() != 0) {
         CRadians cLeftRightRotation(-fRotationSensitivity * c_delta.x());
         /* Project Forward onto the XY axis */
         CVector3 cForwardXY(cForward.GetX(), cForward.GetY(), 0.0f);
         /* Save its length: it will be used to restore the Z coordinate correctly */
         Real cForwardXYLength = cForwardXY.Length();
         /* Normalize the projection */
         cForwardXY.Normalize();
         /* Project Left onto the XY axis and normalize the result */
         CVector3 cLeftXY = CVector3::Z;
         cLeftXY.CrossProduct(cForwardXY).Normalize();
         CVector3 cNewForwardXY(cForwardXY);
         cNewForwardXY *= Cos(cLeftRightRotation);
         cNewForwardXY += cLeftXY * Sin(cLeftRightRotation);
         cNewForwardXY.Normalize();
         /* Scale the vector back to the right length */
         cNewForwardXY *= cForwardXYLength;
         /* Finally, update Forward */
         cForward.SetX(cNewForwardXY.GetX());
         cForward.SetY(cNewForwardXY.GetY());
         cForward.Normalize();
         cLeft = cLeftXY;
         cLeft *= Cos(cLeftRightRotation);
         cLeft -= cForwardXY * Sin(cLeftRightRotation);
         cLeft.Normalize();
         /* To calculate the new Up vector, a cross-product is enough */
         m_sActivePlacement.Up = cForward;
         m_sActivePlacement.Up.CrossProduct(cLeft).Normalize();
      }
      m_sActivePlacement.Target = m_sActivePlacement.Position;
      m_sActivePlacement.Target += cForward;
   }
   
   /****************************************/
   /****************************************/

   void CQTOpenGLCamera::Move(SInt32 n_forwards_backwards,
                              SInt32 n_sideways,
                              SInt32 n_up_down) {
      /* disable timeline on move */
      m_bEnableTimeline = false;
      /* Get cUp and calculate cForward and cLeft */
      const CVector3& cUp = m_sActivePlacement.Up;
      CVector3 cForward((m_sActivePlacement.Target - m_sActivePlacement.Position).Normalize());
      CVector3 cLeft = m_sActivePlacement.Up;
      cLeft.CrossProduct(cForward).Normalize();
      /* Calculate motion sensitivity */
      Real fMotionSensitivity = MOVE_GAIN * Exp(m_sActivePlacement.LensFocalLength);
      /* Apply translation */
      m_sActivePlacement.Position += 
         cForward * (fMotionSensitivity * n_forwards_backwards) +
         cLeft * (fMotionSensitivity * n_sideways) +
         cUp * (fMotionSensitivity * n_up_down);
      m_sActivePlacement.Target = m_sActivePlacement.Position;
      m_sActivePlacement.Target += cForward;
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLCamera::Interpolate(UInt32 un_start_placement,
                                     UInt32 un_end_placement,
                                     Real f_time_fraction) {
      const SPlacement& sStart = m_arrPlacements[un_start_placement];
      const SPlacement& sEnd = m_arrPlacements[un_end_placement];
      /* Linear interpolate position vector */
      m_sActivePlacement.Position = (sEnd.Position - sStart.Position);
      m_sActivePlacement.Position *= f_time_fraction;
      m_sActivePlacement.Position += sStart.Position;
      /* Linear interpolate look at vector */
      m_sActivePlacement.Target = (sEnd.Target - sStart.Target);
      m_sActivePlacement.Target *= f_time_fraction;
      m_sActivePlacement.Target += sStart.Target;
      /* Linear interpolate up vector */
      m_sActivePlacement.Up = (sEnd.Up - sStart.Up);
      m_sActivePlacement.Up *= f_time_fraction;
      m_sActivePlacement.Up += sStart.Up;
      /* Linear interpolate the focal length */
      m_sActivePlacement.LensFocalLength = (sEnd.LensFocalLength - sStart.LensFocalLength);
      m_sActivePlacement.LensFocalLength *= f_time_fraction;
      m_sActivePlacement.LensFocalLength += sStart.LensFocalLength;
      /* Calculate the Y field-of-view */
      m_sActivePlacement.CalculateYFieldOfView();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLCamera::UpdateTimeline() {
      if(!m_bEnableTimeline)
         return;
      /* initialize two iterators to the first keyframe */
      std::vector<SKeyframe>::const_iterator itCurrent = 
         std::cbegin(m_vecKeyframes);
      std::vector<SKeyframe>::const_iterator itNext = 
         std::cbegin(m_vecKeyframes);
      if(itCurrent == std::cend(m_vecKeyframes)) {
         /* no keyframes defined: nothing to do */
         return;
      }
      UInt32 unStep = 
         CSimulator::GetInstance().GetSpace().GetSimulationClock();
      /* wrap unStep if we are looping */
      if(m_unLoop) {
         unStep %= m_unLoop;
      }
      /* search for the current and next keyframes */
      while(itNext != std::cend(m_vecKeyframes) &&
         itNext->Step < unStep) {
         itCurrent = itNext++;
      }
      if(m_unLoop == 0) {
         if(itCurrent == itNext) {
            /* no keyframes have been defined yet */
            return;
         }
         else if(itNext == std::cend(m_vecKeyframes)) {
            if(itCurrent->Step == unStep - 1) {
               SetActivePlacement(itCurrent->PlacementIndex);
            }
         }
         else {
            /* general case */
            if(itCurrent->InterpolateToNext) {
               Real fSegmentTimeFraction =
                  (unStep - itCurrent->Step) / 
                  static_cast<Real>(itNext->Step - itCurrent->Step);
               Interpolate(itCurrent->PlacementIndex,
                           itNext->PlacementIndex,
                           fSegmentTimeFraction);
            }
            else {
               if(itCurrent->Step == unStep - 1) {
                  SetActivePlacement(itCurrent->PlacementIndex);
               }
            }
         }
      }
      else /* m_unLoop != 0 */ {
         if(itCurrent == itNext) {
            /* no keyframes have been defined yet, since we are looping
               wrap itCurrent around to the last keyframe */
            itCurrent = std::prev(std::cend(m_vecKeyframes));
            if(itCurrent == itNext) {
               /* we are looping with a single keyframe, apply it once
                  and disable the timeline */
               SetActivePlacement(itCurrent->PlacementIndex);
               m_bEnableTimeline = false;
            }
            else if(itCurrent->InterpolateToNext) {
               Real fSegmentTimeFraction =
                  (m_unLoop - itCurrent->Step + unStep) / 
                  static_cast<Real>(m_unLoop - itCurrent->Step + itNext->Step);
               Interpolate(itCurrent->PlacementIndex,
                           itNext->PlacementIndex,
                           fSegmentTimeFraction);
            }
            else /* itCurrent->InterpolateToNext == false */ {
               SetActivePlacement(itCurrent->PlacementIndex);
            }
         }
         else if(itNext == std::cend(m_vecKeyframes)) {
            itNext = std::cbegin(m_vecKeyframes);
            if(itCurrent == itNext) {
               /* we are looping with a single keyframe, apply it once
                  and disable the timeline */
               SetActivePlacement(itCurrent->PlacementIndex);
               m_bEnableTimeline = false;
            }
            else if(itCurrent->InterpolateToNext) {
               Real fSegmentTimeFraction =
                  (unStep - itCurrent->Step) /
                  static_cast<Real>(m_unLoop - itCurrent->Step + itNext->Step);
               Interpolate(itCurrent->PlacementIndex,
                           itNext->PlacementIndex,
                           fSegmentTimeFraction);
            }
            else /* itCurrent->InterpolateToNext == false */ {
               SetActivePlacement(itCurrent->PlacementIndex);
            }
         }
         else {
            if(itCurrent->InterpolateToNext) {
               Real fSegmentTimeFraction =
                  (unStep - itCurrent->Step) / 
                  static_cast<Real>(itNext->Step - itCurrent->Step);
               Interpolate(itCurrent->PlacementIndex,
                           itNext->PlacementIndex,
                           fSegmentTimeFraction);
            }
            else {
               SetActivePlacement(itCurrent->PlacementIndex);
            }
         }
      }
   }

   /****************************************/
   /****************************************/

}
