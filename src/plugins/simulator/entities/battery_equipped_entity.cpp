
/**
 * @file <argos3/plugins/simulator/entities/battery_equipped_entity.cpp>
 *
 * @author Adhavan Jayabalan <jadhu94@gmail.com>
 */
#include "battery_equipped_entity.h"
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/entity/composable_entity.h>

namespace argos {

    /****************************************/
    /****************************************/

    CBatteryEquippedEntity::CBatteryEquippedEntity(CComposableEntity *pc_parent) :
        CEntity(pc_parent),
        m_unFullCapacity(0),
        m_fIdealDischarge(0),
        m_fMovingDischarge(0) {
        m_fPrevPosition = CVector3::ZERO;
//        m_fPrevOrientation.Set(1,0,0,0);;
        Disable();
    }

    /****************************************/
    /****************************************/

    CBatteryEquippedEntity::CBatteryEquippedEntity(CComposableEntity *pc_parent, const std::string &str_id, UInt16 un_full_capacity, Real f_ideal_discharge, Real f_moving_discharge):
        CEntity(pc_parent,str_id),
        m_unFullCapacity(un_full_capacity),
        m_fIdealDischarge(f_ideal_discharge),
        m_fMovingDischarge(f_moving_discharge) {
        // todo: write properly different starting level for robots
        m_fLevel = 1.0f;
        m_pcEmbodiedEntity = &(pc_parent->GetComponent<CEmbodiedEntity>("body"));
        m_fPrevPosition = m_pcEmbodiedEntity->GetOriginAnchor().Position;
//        m_fPrevOrientation = m_pcEmbodiedEntity->GetOriginAnchor().Orientation;
        Disable();
    }

    /****************************************/
    /****************************************/

    CBatteryEquippedEntity::~CBatteryEquippedEntity() {

    }

    /****************************************/
    /****************************************/

    void CBatteryEquippedEntity::Init(TConfigurationNode& t_tree) {
       try {
          CEntity::Init(t_tree);
       }
       catch(CARGoSException& ex) {
          THROW_ARGOSEXCEPTION_NESTED("Error initializing the battery sensor equipped entity \"" << GetId() << "\"", ex);
       }
    }

    /****************************************/
    /****************************************/

    void CBatteryEquippedEntity::Update() {
        /*
         * Simulate the battery model
         * E_{t+1} = E_{t} - I - (M* Displacement)
         */
        CVector3 PosDisplacement = m_pcEmbodiedEntity->GetOriginAnchor().Position - m_fPrevPosition;
//        CQuaternion OrientDisplacement = m_pcEmbodiedEntity->GetOriginAnchor().Orientation - m_fPrevOrientation;

        Real discharge = (m_fIdealDischarge + (m_fMovingDischarge*PosDisplacement.Length()));
        m_fLevel -= discharge;
        m_unAvailCapacity = m_fLevel * m_unFullCapacity;
        m_unTimeRemaining = m_fLevel/(CPhysicsEngine::GetInverseSimulationClockTick()*discharge);

        m_fPrevPosition = m_pcEmbodiedEntity->GetOriginAnchor().Position ;
//        m_fPrevOrientation = m_pcEmbodiedEntity->GetOriginAnchor().Orientation;
    }

    /****************************************/
    /****************************************/

    void CBatteryEquippedEntity::Enable() {
        CEntity::Enable();
    }

    /****************************************/
    /****************************************/

    void CBatteryEquippedEntity::Disable() {
        CEntity::Disable();
    }

    /****************************************/
    /****************************************/

    REGISTER_STANDARD_SPACE_OPERATIONS_ON_ENTITY(CBatteryEquippedEntity);

    /****************************************/
    /****************************************/
}
