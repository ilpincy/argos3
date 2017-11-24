
/**
 * @file <argos3/plugins/simulator/entities/proximity_sensor_equipped_entity.cpp>
 *
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 */
#include "proximity_sensor_equipped_entity.h"
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/entity/composable_entity.h>

namespace argos {

   /****************************************/
   /****************************************/

   CProximitySensorEquippedEntity::SSensor::SSensor(const CVector3& c_offset,
                                                    const CVector3& c_direction,
                                                    Real f_range,
                                                    SAnchor& s_anchor) :
      Offset(c_offset),
      Direction(c_direction),
      Anchor(s_anchor) {
      Direction.Normalize();
      Direction *= f_range;
   }

   /****************************************/
   /****************************************/

   CProximitySensorEquippedEntity::CProximitySensorEquippedEntity(CComposableEntity* pc_parent) :
      CEntity(pc_parent) {
      Disable();
   }
   
   /****************************************/
   /****************************************/

   CProximitySensorEquippedEntity::CProximitySensorEquippedEntity(CComposableEntity* pc_parent,
                                                                  const std::string& str_id) :
      CEntity(pc_parent, str_id) {
      Disable();
   }

   /****************************************/
   /****************************************/

   CProximitySensorEquippedEntity::~CProximitySensorEquippedEntity() {
      while(! m_tSensors.empty()) {
         delete m_tSensors.back();
         m_tSensors.pop_back();
      }
   }

   /****************************************/
   /****************************************/

   void CProximitySensorEquippedEntity::Init(TConfigurationNode& t_tree) {
      try {
         /*
          * Parse basic entity stuff
          */
         CEntity::Init(t_tree);
         /*
          * Parse proximity sensors
          */
         /* Not adding any sensor is a fatal error */
         if(t_tree.NoChildren()) {
            THROW_ARGOSEXCEPTION("No sensors defined");
         }
         /* Go through children */
         TConfigurationNodeIterator it;
         for(it = it.begin(&t_tree); it != it.end(); ++it) {
            std::string strAnchorId;
            GetNodeAttribute(*it, "anchor", strAnchorId);
            /*
             * NOTE: here we get a reference to the embodied entity
             * This line works under the assumption that:
             * 1. the ProximitySensorEquippedEntity has a parent;
             * 2. the parent has a child whose id is "body"
             * 3. the "body" is an embodied entity
             * If any of the above is false, this line will bomb out.
             */
            CEmbodiedEntity& cBody = GetParent().GetComponent<CEmbodiedEntity>("body");
            if(it->Value() == "sensor") {
               CVector3 cOff, cDir;
               Real fRange;
               GetNodeAttribute(*it, "offset", cOff);
               GetNodeAttribute(*it, "direction", cDir);
               GetNodeAttribute(*it, "range", fRange);
               AddSensor(cOff, cDir, fRange, cBody.GetAnchor(strAnchorId));
            }
            else if(it->Value() == "ring") {
               CVector3 cRingCenter;
               GetNodeAttributeOrDefault(t_tree, "center", cRingCenter, cRingCenter);
               Real fRadius;
               GetNodeAttribute(t_tree, "radius", fRadius);
               CDegrees cRingStartAngleDegrees;
               GetNodeAttributeOrDefault(t_tree, "start_angle", cRingStartAngleDegrees, cRingStartAngleDegrees);
               CRadians cRingStartAngleRadians = ToRadians(cRingStartAngleDegrees);
               Real fRange;
               GetNodeAttribute(t_tree, "range", fRange);
               UInt32 unNumSensors;
               GetNodeAttribute(t_tree, "num_sensors", unNumSensors);
               AddSensorRing(cRingCenter,
                             fRadius,
                             cRingStartAngleRadians,
                             fRange,
                             unNumSensors,
                             cBody.GetAnchor(strAnchorId));
            }
            else {
               THROW_ARGOSEXCEPTION("Unrecognized tag \"" << it->Value() << "\"");
            }
         }
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Initialization error in proximity sensor equipped entity", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CProximitySensorEquippedEntity::Enable() {
      CEntity::Enable();
      for(size_t i = 0; i < m_tSensors.size(); ++i) {
         m_tSensors[i]->Anchor.Enable();
      }
   }

   /****************************************/
   /****************************************/

   void CProximitySensorEquippedEntity::Disable() {
      CEntity::Disable();
      for(size_t i = 0; i < m_tSensors.size(); ++i) {
         m_tSensors[i]->Anchor.Disable();
      }
   }

   /****************************************/
   /****************************************/

   void CProximitySensorEquippedEntity::AddSensor(const CVector3& c_offset,
                                                  const CVector3& c_direction,
                                                  Real f_range,
                                                  SAnchor& s_anchor) {
      m_tSensors.push_back(new SSensor(c_offset, c_direction, f_range, s_anchor));
   }

   /****************************************/
   /****************************************/
   
   void CProximitySensorEquippedEntity::AddSensorRing(const CVector3& c_center,
                                                      Real f_radius,
                                                      const CRadians& c_start_angle,
                                                      Real f_range,
                                                      UInt32 un_num_sensors,
                                                      SAnchor& s_anchor) {
      CRadians cSensorSpacing = CRadians::TWO_PI / un_num_sensors;
      CRadians cAngle;
      CVector3 cOff, cDir;
      for(UInt32 i = 0; i < un_num_sensors; ++i) {
         cAngle = c_start_angle + i * cSensorSpacing;
         cAngle.SignedNormalize();
         cOff.Set(f_radius, 0.0f, 0.0f);
         cOff.RotateZ(cAngle);
         cOff += c_center;
         cDir.Set(f_range, 0.0f, 0.0f);
         cDir.RotateZ(cAngle);
         AddSensor(cOff, cDir, f_range, s_anchor);
      }
   }

   /****************************************/
   /****************************************/

   void CProximitySensorEquippedEntity::AddSensorFan(const CVector3& c_center,
                                                     Real f_radius,
                                                     const CRadians& c_start_angle,
                                                     const CRadians& c_end_angle,
                                                     Real f_range,
                                                     UInt32 un_num_sensors,
                                                     SAnchor& s_anchor) {
      CRadians cSensorSpacing = (c_end_angle - c_start_angle) / (un_num_sensors - 1);
      CRadians cAngle;
      CVector3 cOff, cDir;
      for(UInt32 i = 0; i < un_num_sensors; ++i) {
         cAngle = c_start_angle + i * cSensorSpacing;
         cAngle.SignedNormalize();
         cOff.Set(f_radius, 0.0f, 0.0f);
         cOff.RotateZ(cAngle);
         cOff += c_center;
         cDir.Set(f_range, 0.0f, 0.0f);
         cDir.RotateZ(cAngle);
         AddSensor(cOff, cDir, f_range, s_anchor);
      }
   }

   /****************************************/
   /****************************************/

   REGISTER_STANDARD_SPACE_OPERATIONS_ON_ENTITY(CProximitySensorEquippedEntity);
   
   /****************************************/
   /****************************************/

}
