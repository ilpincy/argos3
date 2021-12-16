#include "loop_functions.h"
#include <argos3/plugins/robots/builderbot/simulator/builderbot_entity.h>
#include <argos3/plugins/robots/block/simulator/block_entity.h>
#include <argos3/plugins/robots/generic/control_interface/ci_simple_radios_sensor.h>
#include <argos3/core/simulator/entity/embodied_entity.h>

namespace argos {

   /****************************************/
   /****************************************/

   bool CTestLoopFunctions::IsExperimentFinished() {
      /* wait five ticks before evaluating the test */
      if(GetSpace().GetSimulationClock() < 5) {
         return false;
      }
      else {
         CBuilderBotEntity* pcRobot = nullptr;
         CBlockEntity* pcBlock = nullptr;
         try {
            pcRobot = dynamic_cast<CBuilderBotEntity*>(&GetSpace().GetEntity("robot"));
            pcBlock = dynamic_cast<CBlockEntity*>(&GetSpace().GetEntity("block"));
         }
         catch(CARGoSException& ex) {
            THROW_ARGOSEXCEPTION_NESTED("Entity was not added to the simulator", ex);
         }
         /* Get a reference to the first message in the robot's nfc interface */
         if(pcRobot == nullptr) {
            THROW_ARGOSEXCEPTION("Entity with identifier \"robot\" was not a BuilderBot");
         }
         CCI_SimpleRadiosSensor* pcSimpleRadioSensorRobot =
            pcRobot->GetControllableEntity().GetController().GetSensor<CCI_SimpleRadiosSensor>("simple_radios");
         auto itRobotInterface = std::find_if(
            std::begin(pcSimpleRadioSensorRobot->GetInterfaces()),
            std::end(pcSimpleRadioSensorRobot->GetInterfaces()),
            [] (auto& interface) {
               return interface.Id == "nfc";
            });
         if(itRobotInterface == std::end(pcSimpleRadioSensorRobot->GetInterfaces())) {
            THROW_ARGOSEXCEPTION("BuilderBot did not have a radio with the identifier: \"nfc\"");
         }
         if(itRobotInterface->Messages.size() != 1) {
            THROW_ARGOSEXCEPTION("Robot's \"nfc\" radio did not have exactly one message");
         }
         const CByteArray& cRobotMessage = itRobotInterface->Messages[0];
         /* Get a reference to the first message in the block's south interface */
         if(pcBlock == nullptr) {
            THROW_ARGOSEXCEPTION("Entity with identifier \"block\" was not a block");
         }
         CCI_SimpleRadiosSensor* pcSimpleRadioSensorBlock =
            pcBlock->GetControllableEntity().GetController().GetSensor<CCI_SimpleRadiosSensor>("simple_radios");
         auto itBlockInterface = std::find_if(
            std::begin(pcSimpleRadioSensorBlock->GetInterfaces()),
            std::end(pcSimpleRadioSensorBlock->GetInterfaces()),
            [] (auto& interface) {
               return interface.Id == "south";
            });
         if(itBlockInterface == std::end(pcSimpleRadioSensorBlock->GetInterfaces())) {
            THROW_ARGOSEXCEPTION("Block did not have a radio with the identifier: \"south\"");
         }
         if(itBlockInterface->Messages.size() != 1) {
            THROW_ARGOSEXCEPTION("Block's \"south\" radio did not have exactly one message");
         }
         const CByteArray& cBlockMessage = itBlockInterface->Messages[0];

         /* the robot and block are constantly sending a Lua table containing the string 'ping' or
            'pong' at index 1. At any timestep, these messages should be the same */
         if(!(cBlockMessage == cRobotMessage)) {
            THROW_ARGOSEXCEPTION("The message in the block did not match the message in the robot");
         }
         return true;
      }
   }

   /****************************************/
   /****************************************/

   REGISTER_LOOP_FUNCTIONS(CTestLoopFunctions, "test_loop_functions");

}
