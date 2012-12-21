/**
 * @file <argos3/core/simulator/query_plugins.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef QUERY_PLUGINS_H
#define QUERY_PLUGINS_H

#include <argos3/core/utility/plugins/factory.h>
#include <argos3/core/utility/logging/argos_log.h>

#include <map>
#include <vector>
#include <string>

namespace argos {

   /****************************************/
   /****************************************/

   struct SQueryResultItem {
      std::string Label;
      std::string Author;
      std::string Version;
      std::string BriefDescription;
      std::string LongDescription;
      std::string Status;

      SQueryResultItem(std::string str_label,
                       std::string str_author,
                       std::string str_version,
                       std::string str_brief_description,
                       std::string str_long_description,
                       std::string str_status) :
         Label(str_label),
         Author(str_author),
         Version(str_version),
         BriefDescription(str_brief_description),
         LongDescription(str_long_description),
         Status(str_status) {}
   };

   typedef std::vector<SQueryResultItem> TQueryResult;

   /****************************************/
   /****************************************/

   void QueryPlugins(const std::string& str_query);

   /****************************************/
   /****************************************/

   void QueryShowPluginDescription(const std::string& str_query);

   /****************************************/
   /****************************************/

   template <class TYPE>
   void QuerySearchPlugins(const std::string& str_query,
                           TQueryResult& t_result) {
      typename CFactory<TYPE>::TTypeMap& tTypeMap = CFactory<TYPE>::GetTypeMap();
      for(typename CFactory<TYPE>::TTypeMap::const_iterator it = tTypeMap.begin();
          it != tTypeMap.end();
          ++it) {
         /* If the current plugin name contains the passed query */
         if(it->first.find(str_query) != std::string::npos) {
            t_result.push_back(
               SQueryResultItem(
                  it->first,
                  it->second->Author,
                  it->second->Version,
                  it->second->BriefDescription,
                  it->second->LongDescription,
                  it->second->Status));
         }
      }
   }

   /****************************************/
   /****************************************/

   template <class TYPE>
   void QueryShowList(const std::string& str_header) {      
      typename CFactory<TYPE>::TTypeMap& tTypeMap = CFactory<TYPE>::GetTypeMap();
      LOG << str_header << std::endl << std::endl;
      for(typename CFactory<TYPE>::TTypeMap::iterator it = tTypeMap.begin();
          it != tTypeMap.end();
          ++it) {
         LOG << "   [ " << it->first << " ]" << std::endl;
         LOG << "      " << it->second->BriefDescription << std::endl << std::endl;
      }
   }

   /****************************************/
   /****************************************/

}

#endif

