#ifndef OPERATION_H
#define OPERATION_H

#include <argos3/core/utility/plugins/factory.h>

namespace argos {

   class CEntity;

   class COperation {
   public:
      virtual ~COperation() {}
      virtual void Execute(CEntity* e) = 0;
   };

   typedef CFactory<COperation> TFactoryOperation;
#define REGISTER_OPERATION(CLASSNAME,           \
                           LABEL,               \
                           AUTHOR,              \
                           VERSION,             \
                           BRIEF_DESCRIPTION,   \
                           LONG_DESCRIPTION,    \
                           STATUS)              \
   REGISTER_SYMBOL(COperation,                  \
                   CLASSNAME,                   \
                   LABEL,                       \
                   AUTHOR,                      \
                   VERSION,                     \
                   BRIEF_DESCRIPTION,           \
                   LONG_DESCRIPTION,            \
                   STATUS)

}

#endif
