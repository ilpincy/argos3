#include <argos3/core/utility/plugins/factory.h>
#include <argos3/core/utility/plugins/dynamic_loading.h>
#include <argos3/core/simulator/entity.h>
#include <argos3/core/simulator/operation.h>

int main() {
   
}

// int main() {
//    /*
//     * INIT
//     */
//    argos::CDynamicLoading cDL;
//    cDL.LoadLibrary("./libplugin.so");
//    argos::TFactoryEntity::Print(std::cout);
//    argos::CEntity* pcEA = argos::TFactoryEntity::New("entity_a");
//    argos::CEntity* pcEB = argos::TFactoryEntity::New("entity_b");
//    argos::COperation* pcOA = argos::TFactoryOperation::New("operation_a");
//    argos::COperation* pcOB = argos::TFactoryOperation::New("operation_b");
//    pcOA->Execute(pcEA);
//    pcOA->Execute(pcEB);
//    pcOB->Execute(pcEA);
//    pcOB->Execute(pcEB);
//    /*
//     * CLEANUP
//     */
//    delete pcOA;
//    delete pcOB;
//    delete pcEA;
//    delete pcEB;
//    return 0;
// }
