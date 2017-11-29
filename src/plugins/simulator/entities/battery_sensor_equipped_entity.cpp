
/**
 * @file <argos3/plugins/simulator/entities/battery_sensor_equipped_entity.cpp>
 *
 * @author
 */
#include "battery_sensor_equipped_entity.h"
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/entity/composable_entity.h>

namespace argos {

    /****************************************/
    /****************************************/

    CBatterySensorEquippedEntity::CBatterySensorEquippedEntity(CComposableEntity *pc_parent) :
        CEntity(pc_parent){
        Disable();
    }

    /****************************************/
    /****************************************/

    CBatterySensorEquippedEntity::CBatterySensorEquippedEntity(CComposableEntity *pc_parent, const std::string &str_id):
        CEntity(pc_parent,str_id) {
        Disable();
    }

    /****************************************/
    /****************************************/

    CBatterySensorEquippedEntity::~CBatterySensorEquippedEntity() {

    }

    /****************************************/
    /****************************************/

    void CBatterySensorEquippedEntity::Init(TConfigurationNode& t_tree) {
       try {
          CEntity::Init(t_tree);
          std::string strCapacity;
          GetNodeAttribute(t_tree, "capacity", strCapacity);
          std::cout<<"capacity"<<strCapacity;
       }
       catch(CARGoSException& ex) {
          THROW_ARGOSEXCEPTION_NESTED("Error initializing the battery sensor equipped entity \"" << GetId() << "\"", ex);
       }
    }

    /****************************************/
    /****************************************/

    void CBatterySensorEquippedEntity::Enable() {
        CEntity::Enable();
    }

    /****************************************/
    /****************************************/

    void CBatterySensorEquippedEntity::Disable() {
        CEntity::Disable();
    }

    /****************************************/
    /****************************************/

    REGISTER_STANDARD_SPACE_OPERATIONS_ON_ENTITY(CBatterySensorEquippedEntity);

    /****************************************/
    /****************************************/
}
