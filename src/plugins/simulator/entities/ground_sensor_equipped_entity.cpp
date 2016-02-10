
/**
 * @file <argos3/plugins/simulator/entities/ground_sensor_equipped_entity.cpp>
 *
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 */
#include "ground_sensor_equipped_entity.h"
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/entity/composable_entity.h>

namespace argos {

   /****************************************/
   /****************************************/

   CGroundSensorEquippedEntity::CGroundSensorEquippedEntity(CComposableEntity* pc_parent) :
      CEntity(pc_parent) {
      Disable();
   }
   
   /****************************************/
   /****************************************/

   CGroundSensorEquippedEntity::CGroundSensorEquippedEntity(CComposableEntity* pc_parent,
                                                                  const std::string& str_id) :
      CEntity(pc_parent, str_id) {
      Disable();
   }

   /****************************************/
   /****************************************/

   CGroundSensorEquippedEntity::~CGroundSensorEquippedEntity() {
      while(! m_tSensors.empty()) {
         delete m_tSensors.back();
         m_tSensors.pop_back();
      }
   }

   /****************************************/
   /****************************************/

   void CGroundSensorEquippedEntity::Init(TConfigurationNode& t_tree) {
      try {
         /*
          * Parse basic entity stuff
          */
         CEntity::Init(t_tree);
         /*
          * Parse ground sensors
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
             * 1. the GroundSensorEquippedEntity has a parent;
             * 2. the parent has a child whose id is "body"
             * 3. the "body" is an embodied entity
             * If any of the above is false, this line will bomb out.
             */
            CEmbodiedEntity& cBody = GetParent().GetComponent<CEmbodiedEntity>("body");
            if(it->Value() == "sensor") {
               CVector2 cOffset;
               GetNodeAttribute(*it, "offset", cOffset);
               std::string strType;
               GetNodeAttribute(*it, "type", strType);
               AddSensor(cOffset, ParseType(strType), cBody.GetAnchor(strAnchorId));
            }
            else if(it->Value() == "ring") {
               CVector2 cRingCenter;
               GetNodeAttributeOrDefault(t_tree, "center", cRingCenter, cRingCenter);
               Real fRadius;
               GetNodeAttribute(t_tree, "radius", fRadius);
               CDegrees cRingStartAngleDegrees;
               GetNodeAttributeOrDefault(t_tree, "start_angle", cRingStartAngleDegrees, cRingStartAngleDegrees);
               CRadians cRingStartAngleRadians = ToRadians(cRingStartAngleDegrees);
               std::string strType;
               GetNodeAttribute(*it, "type", strType);
               ESensorType eType = ParseType(strType);
               UInt32 unNumSensors;
               GetNodeAttribute(t_tree, "num_sensors", unNumSensors);
               AddSensorRing(cRingCenter,
                             fRadius,
                             cRingStartAngleRadians,
                             eType,
                             unNumSensors,
                             cBody.GetAnchor(strAnchorId));
            }
            else {
               THROW_ARGOSEXCEPTION("Unrecognized tag \"" << it->Value() << "\"");
            }
         }
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Initialization error in ground sensor equipped entity", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CGroundSensorEquippedEntity::Enable() {
      CEntity::Enable();
      for(size_t i = 0; i < m_tSensors.size(); ++i) {
         m_tSensors[i]->Anchor.Enable();
      }
   }

   /****************************************/
   /****************************************/

   void CGroundSensorEquippedEntity::Disable() {
      CEntity::Disable();
      for(size_t i = 0; i < m_tSensors.size(); ++i) {
         m_tSensors[i]->Anchor.Disable();
      }
   }

   /****************************************/
   /****************************************/

   void CGroundSensorEquippedEntity::AddSensor(const CVector2& c_offset,
                                               ESensorType e_type,
                                               SAnchor& s_anchor) {
      m_tSensors.push_back(new SSensor(c_offset, e_type, s_anchor));
   }

   /****************************************/
   /****************************************/
   
   void CGroundSensorEquippedEntity::AddSensorRing(const CVector2& c_center,
                                                      Real f_radius,
                                                      const CRadians& c_start_angle,
                                                      ESensorType e_type,
                                                      UInt32 un_num_sensors,
                                                      SAnchor& s_anchor) {
      CRadians cSensorSpacing = CRadians::TWO_PI / un_num_sensors;
      CRadians cAngle;
      CVector2 cOffset;
      for(UInt32 i = 0; i < un_num_sensors; ++i) {
         cAngle = c_start_angle + i * cSensorSpacing;
         cAngle.SignedNormalize();
         cOffset.Set(f_radius, 0.0f);
         cOffset.Rotate(cAngle);
         cOffset += c_center;
         AddSensor(cOffset, e_type, s_anchor);
      }
   }

   /****************************************/
   /****************************************/

   CGroundSensorEquippedEntity::ESensorType CGroundSensorEquippedEntity::ParseType(const std::string& str_type) const {
      if(str_type == "bw")    return TYPE_BLACK_WHITE;
      if(str_type == "gray") return TYPE_GRAYSCALE;
      THROW_ARGOSEXCEPTION("Unrecognized ground sensor type \"" << str_type << "\"");
   }

   /****************************************/
   /****************************************/

   REGISTER_STANDARD_SPACE_OPERATIONS_ON_ENTITY(CGroundSensorEquippedEntity);
   
   /****************************************/
   /****************************************/

}
