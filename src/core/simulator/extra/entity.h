#ifndef ENTITY_H
#define ENTITY_H

#include <argos3/core/utility/plugins/factory.h>

namespace argos {

   class CEntity {
   public:
      virtual ~CEntity() {}
      virtual void Operation() = 0;
   };


   typedef CFactory<CEntity> TFactoryEntity;

#define REGISTER_ENTITY(CLASSNAME,              \
                        LABEL,                  \
                        AUTHOR,                 \
                        VERSION,                \
                        BRIEF_DESCRIPTION,      \
                        LONG_DESCRIPTION,       \
                        STATUS)                 \
   REGISTER_SYMBOL(CEntity,                     \
                   CLASSNAME,                   \
                   LABEL,                       \
                   AUTHOR,                      \
                   VERSION,                     \
                   BRIEF_DESCRIPTION,           \
                   LONG_DESCRIPTION,            \
                   STATUS)

}

#endif
