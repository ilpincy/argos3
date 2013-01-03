#include "composable_entity.h"

#include <argos3/core/utility/string_utilities.h>

namespace argos {

   /****************************************/
   /****************************************/

   void CComposableEntity::AddComponent(CEntity& c_component) {
      m_mapComponents.insert(
         std::pair<std::string, CEntity*>(
            c_component.GetTypeDescription(),
            &c_component));
   }

   /****************************************/
   /****************************************/

   void CComposableEntity::RemoveComponent(const std::string& str_component) {
      m_mapComponents.erase(FindComponent(str_component));
   }

   /****************************************/
   /****************************************/

   CEntity& CComposableEntity::GetComponent(const std::string& str_component) {
      return *(FindComponent(str_component)->second);
   }

   /****************************************/
   /****************************************/

   bool CComposableEntity::HasComponent(const std::string& str_component) {
      return m_mapComponents.count(str_component) > 0;
   }

   /****************************************/
   /****************************************/

   void CComposableEntity::Update() {
      UpdateComponents();
   }

   /****************************************/
   /****************************************/

   void CComposableEntity::UpdateComponents() {
      for(CEntity::TMap::iterator it = m_mapComponents.begin();
          it != m_mapComponents.end();
          ++it) {
         it->second->Update();
      }
   }

   /****************************************/
   /****************************************/

   CEntity::TMultiMap::iterator CComposableEntity::FindComponent(const std::string& str_component) {      
      /* Check for the presence of [ */
      std::string::size_type unIndexStart = str_component.find('[');
      if(unIndexStart != std::string::npos) {
         /* Found, now check for the presence of ] after [ */
         std::string::size_type unIndexEnd = str_component.rfind(']');
         if(unIndexEnd != std::string::npos &&
            unIndexEnd < unIndexStart) {
            /* Use the string between [ and ] as an index and whatever comes before as base id */
            /* Count how many components there are for the base type */
            std::string strBaseType = str_component.substr(0,unIndexStart);
            size_t unCount = m_mapComponents.count(strBaseType);
            if(unCount == 0) {
               /* No components -> error */
               THROW_ARGOSEXCEPTION("No component of type \"" << strBaseType << "\" found for entity \"" << GetId() << "\"");
            }
            else {
               /* Components found */
               size_t unIndex = FromString<size_t>(str_component.substr(unIndexStart, unIndexEnd - unIndexStart));
               /* Is index valid? */
               if(unIndex < unCount) {
                  /* All OK, return the wanted component */
                  /* Get the range of matching components */
                  std::pair<CEntity::TMultiMap::iterator,
                            CEntity::TMultiMap::iterator> cRange = m_mapComponents.equal_range(strBaseType);
                  /* Start from the first */
                  CEntity::TMultiMap::iterator it = cRange.first;
                  size_t i = 0;
                  /* Go through elements until we hit the element */
                  while(1) {
                     ++i;
                     if(i < unIndex) {
                        ++it;
                     }
                     else {
                        break;
                     }
                  }
                  return it;
               }
               else {
                  /* Index out of bounds -> error */
                  THROW_ARGOSEXCEPTION("Index out of bound for component of type \"" << str_component.substr(0,unIndexStart) << "\" in entity \"" << GetId() << "\"; max = " << unCount << ", index = " << unIndex);
               }
            }
         }
         else {
            THROW_ARGOSEXCEPTION("Syntax error in entity id \"" << str_component << "\"");
         }
      }
      else {
         /* No [ found in the string, consider the entire string as a type id */
         /* Count how many elements match the given type id */
         size_t unCount = m_mapComponents.count(str_component);
         if(unCount == 0) {
            /* No components -> error */
            THROW_ARGOSEXCEPTION("No component of type \"" << str_component << "\" found for entity \"" << GetId() << "\"");
         }
         else if(unCount > 1) {
            /* Not used array syntax, but needed to -> error */
            THROW_ARGOSEXCEPTION("You need to provide an index for component of type \"" << str_component.substr(0,unIndexStart) << "\" in entity \"" << GetId() << "\": " << unCount << " matching elements are present");
         }
         else {
            /* All OK, return the element */
            return m_mapComponents.find(str_component);
         }
      }
   }

   /****************************************/
   /****************************************/

}

