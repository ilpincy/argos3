/**
 * @file <argos3/core/simulator/loop_functions.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "loop_functions.h"
#include <argos3/core/utility/plugins/factory.h>

namespace argos {

   /****************************************/
   /****************************************/
   
   template <class T> T& AddEntityToSpace(TConfigurationNode& t_tree) {
      /* Create an entity of the wanted type */
      try {
         T* pcEntity = dynamic_cast<T*>(CFactory<CEntity>::New(t_tree.Value()));
         /* Initialize the entity */
         pcEntity->Init(t_tree);
         /* Add the entity to the space */
         CSimulator::GetInstance().GetSpace().AddEntity(*pcEntity);
         /* Return the entity */
         return *pcEntity;
      }
      catch(CARGoSException ex) {
         THROW_ARGOSEXCEPTION_NESTED("Loop functions: error creating entity type \"" << t_tree.Value() << "\"", ex);
      }
   }

   /****************************************/
   /****************************************/

   void AddEntityToPhysics(CEntity& c_entity,
                           const std::vector<std::string>& str_engines) {
      /* Add the entity to the physics engine(s) */
      for(size_t i = 0; i < str_engines.size(); ++i) {
         CSimulator::GetInstance().GetPhysicsEngine(str_engines[i]).AddEntity(c_entity);
      }
   }

   /****************************************/
   /****************************************/

   template <class T> void AssignController(T& c_entity) {
      /* Get a reference to the controllable entity */
      CControllableEntity& cControllableEntity = c_entity.GetControllableEntity();
      /* Look for the controller with the right id in the XML */
      TConfigurationNode tControllersTree;
      tControllersTree = GetNode(CSimulator::GetInstance().GetConfigurationRoot(), "controllers");
      bool found = false;
      TConfigurationNodeIterator itControllers;
      std::string strControllerId;
      itControllers = itControllers.begin(&tControllersTree);
      while(!found && itControllers != itControllers.end()) {
         GetNodeAttribute(*itControllers, "id", strControllerId);
         if(strControllerId == cControllableEntity.GetController().GetId()) {
            found = true;
         }
         else {
            ++itControllers;
         }
      }
      /* Did we find the controller? */
      ARGOS_ASSERT(found,
                   "[FATAL] The entity \"" <<
                   c_entity.GetId() << "\" has been associated with a controller with id \"" <<
                   cControllableEntity.GetController().GetId() <<
                   "\", but a controller with this id wasn't found in the <controllers> section of the XML file.");
      /* Now itControllers points to the right controller subtree */
      /* Get the parameters subtree */
      TConfigurationNode tControllerParameters;
      tControllerParameters = GetNode(*itControllers, "parameters");
      /** @todo Create the controller */
      // CCI_Controller* pcController =
      //    CSimulator::GetInstance().
      //    GetDynamicLinkingManager().
      //    NewController(c_entity,
      //                  cControllableEntity.GetControllerId(),
      //                  tControllerParameters);      
                       
      /* Set the controller to the entity */
      // cControllableEntity.SetController(*pcController);
   }

   /****************************************/
   /****************************************/

   void CLoopFunctions::RemoveEntity(CEntity& c_entity) {
      /* Remove the entity from space */
      CallEntityOperation<CSpaceOperationRemoveEntity, CSpace, void>(m_cSpace, c_entity);
   }

   /****************************************/
   /****************************************/

}
