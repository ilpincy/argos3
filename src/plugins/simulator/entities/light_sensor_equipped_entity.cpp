
/**
 * @file <argos3/plugins/simulator/entities/light_sensor_equipped_entity.cpp>
 *
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 */
#include "light_sensor_equipped_entity.h"
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/entity/composable_entity.h>

namespace argos {

   /****************************************/
   /****************************************/

   CLightSensorEquippedEntity::SSensor::SSensor(const CVector3& c_position,
                                                const CVector3& c_direction,
                                                Real f_range,
                                                SAnchor& s_anchor) :
      Position(c_position),
      Direction(c_direction),
      Anchor(s_anchor) {
      Direction.Normalize();
      Direction *= f_range;
   }

   /****************************************/
   /****************************************/

   CLightSensorEquippedEntity::CLightSensorEquippedEntity(CComposableEntity* pc_parent) :
      CEntity(pc_parent) {
      Disable();
   }
   
   /****************************************/
   /****************************************/

   CLightSensorEquippedEntity::CLightSensorEquippedEntity(CComposableEntity* pc_parent,
                                                          const std::string& str_id) :
      CEntity(pc_parent, str_id) {
      Disable();
   }

   /****************************************/
   /****************************************/

   CLightSensorEquippedEntity::~CLightSensorEquippedEntity() {
      while(! m_tSensors.empty()) {
         delete m_tSensors.back();
         m_tSensors.pop_back();
      }
   }

   /****************************************/
   /****************************************/

   void CLightSensorEquippedEntity::Init(TConfigurationNode& t_tree) {
      try {
         /*
          * Parse basic entity stuff
          */
         CEntity::Init(t_tree);
         /*
          * Parse light sensors
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
             * 1. the entity has a parent;
             * 2. the parent has a child whose id is "body"
             * 3. the "body" is an embodied entity
             * If any of the above is false, this line will bomb out.
             */
            CEmbodiedEntity& cBody = GetParent().GetComponent<CEmbodiedEntity>("body");
            if(it->Value() == "sensor") {
               CVector3 cPos, cDir;
               Real fRange;
               GetNodeAttribute(*it, "position", cPos);
               GetNodeAttribute(*it, "direction", cDir);
               GetNodeAttribute(*it, "range", fRange);
               AddSensor(cPos, cDir, fRange, cBody.GetAnchor(strAnchorId));
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
         THROW_ARGOSEXCEPTION_NESTED("Initialization error in light sensor equipped entity", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CLightSensorEquippedEntity::Enable() {
      CEntity::Enable();
      for(size_t i = 0; i < m_tSensors.size(); ++i) {
         m_tSensors[i]->Anchor.Enable();
      }
   }

   /****************************************/
   /****************************************/

   void CLightSensorEquippedEntity::Disable() {
      CEntity::Disable();
      for(size_t i = 0; i < m_tSensors.size(); ++i) {
         m_tSensors[i]->Anchor.Disable();
      }
   }

   /****************************************/
   /****************************************/

   void CLightSensorEquippedEntity::AddSensor(const CVector3& c_position,
                                              const CVector3& c_direction,
                                              Real f_range,
                                              SAnchor& s_anchor) {
      m_tSensors.push_back(new SSensor(c_position, c_direction, f_range, s_anchor));
   }

   /****************************************/
   /****************************************/
   
   void CLightSensorEquippedEntity::AddSensorRing(const CVector3& c_center,
                                                  Real f_radius,
                                                  const CRadians& c_start_angle,
                                                  Real f_range,
                                                  UInt32 un_num_sensors,
                                                  SAnchor& s_anchor) {
      CRadians cSensorSpacing = CRadians::TWO_PI / un_num_sensors;
      CRadians cAngle;
      CVector3 cPos, cDir;
      for(UInt32 i = 0; i < un_num_sensors; ++i) {
         cAngle = c_start_angle + i * cSensorSpacing;
         cAngle.SignedNormalize();
         cPos.Set(f_radius, 0.0f, 0.0f);
         cPos.RotateZ(cAngle);
         cPos += c_center;
         cDir.Set(f_range, 0.0f, 0.0f);
         cDir.RotateZ(cAngle);
         AddSensor(cPos, cDir, f_range, s_anchor);
      }
   }

   /****************************************/
   /****************************************/

   REGISTER_STANDARD_SPACE_OPERATIONS_ON_ENTITY(CLightSensorEquippedEntity);
   
   /****************************************/
   /****************************************/

}
