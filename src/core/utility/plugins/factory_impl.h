
/**
 * @file <argos3/core/utility/plugins/factory_impl.h>
 *
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 */
/**
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef FACTORY_IMPL_H
#define FACTORY_IMPL_H

/****************************************/
/****************************************/

template<typename TYPE>
typename CFactory<TYPE>::TTypeMap& CFactory<TYPE>::GetTypeMap() {
   static typename CFactory<TYPE>::TTypeMap tTypeMap;
   return tTypeMap;
}

/****************************************/
/****************************************/

template<typename TYPE>
void CFactory<TYPE>::Register(const std::string& str_label,
                              const std::string& str_author,
                              const std::string& str_version,
                              const std::string& str_brief_desc,
                              const std::string& str_long_desc,
                              const std::string& str_status,
                              TCreator* pc_creator) {
   typename CFactory<TYPE>::STypeInfo* psTypeInfo = new typename CFactory<TYPE>::STypeInfo;
   psTypeInfo->Author = str_author;
   psTypeInfo->Version = str_version;
   psTypeInfo->BriefDescription = str_brief_desc;
   psTypeInfo->LongDescription = str_long_desc;
   psTypeInfo->Status = str_status;
   psTypeInfo->Creator = pc_creator;
   GetTypeMap()[str_label] = psTypeInfo;
}

/****************************************/
/****************************************/

template<typename TYPE>
TYPE* CFactory<TYPE>::New(const std::string& str_label) {
   typename TTypeMap::iterator it = GetTypeMap().find(str_label);
   if(it != GetTypeMap().end()) {
      return it->second->Creator();
   }
   else {
      THROW_ARGOSEXCEPTION("Symbol \"" << str_label << "\" not found");
   }
}

/****************************************/
/****************************************/

template<typename TYPE>
bool CFactory<TYPE>::Exists(const std::string& str_label) {
   typename TTypeMap::iterator it = GetTypeMap().find(str_label);
   return(it != GetTypeMap().end());
}

/****************************************/
/****************************************/

template<typename TYPE>
void CFactory<TYPE>::Print(std::ostream& c_os) {
   typename TTypeMap::iterator it;
   c_os << "Symbols:" << std::endl;
   for(it = GetTypeMap().begin();
       it != GetTypeMap().end();
       ++it) {
      c_os << it->first << " (" << it->second->BriefDescription << ")" << std::endl;
   }
}

/****************************************/
/****************************************/

template<typename TYPE>
void CFactory<TYPE>::Destroy() {
   typename TTypeMap::iterator it;
   for(it = GetTypeMap().begin();
       it != GetTypeMap().end();
       ++it) {
      delete it->second;
   }
   GetTypeMap().clear();
}

/****************************************/
/****************************************/

#endif
