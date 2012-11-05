#include <argos3/core/utility/plugins/vtable.h>
#include <argos3/core/simulator/entity/entity.h>

using namespace argos;

struct Testing {};

class CTestEntity1 : public CEntity {
public:
   ENABLE_VTABLE();
public:
   CTestEntity1() : CEntity(NULL) {}
   virtual void Reset() {}
   virtual void Destroy() {}
   virtual void SaveState(CByteArray& c_buffer) {}
   virtual void LoadState(CByteArray& c_buffer) {}
   virtual std::string GetTypeDescription() const {
      return "test_entity";
   }
};

class CTestEntity2 : public CEntity {
public:
   ENABLE_VTABLE();
public:
   CTestEntity2() : CEntity(NULL) {}
   virtual void Reset() {}
   virtual void Destroy() {}
   virtual void SaveState(CByteArray& c_buffer) {}
   virtual void LoadState(CByteArray& c_buffer) {}
   virtual std::string GetTypeDescription() const {
      return "test_entity";
   }
};

class CTestOperationOnTestEntity1 : public COperation<Testing, CEntity, void> {
public:
   void ApplyTo(CTestEntity1& c_entity) {
      std::cout << "CTestOperationOnTestEntity1::ApplyTo("
                << c_entity.GetId()
                << ")" << std::endl;
   }
};
REGISTER_OPERATION(Testing, CEntity, CTestOperationOnTestEntity1, void, CTestEntity1);

class CTestOperationOnTestEntity2 : public COperation<Testing, CEntity, void> {
public:
   void ApplyTo(CTestEntity2& c_entity) {
      std::cout << "CTestOperationOnTestEntity2::ApplyTo("
                << c_entity.GetId()
                << ")" << std::endl;
   }
};
REGISTER_OPERATION(Testing, CEntity, CTestOperationOnTestEntity2, void, CTestEntity2);

int main() {
   CEntity* e1 = new CTestEntity1;
   CEntity* e2 = new CTestEntity2;
   e1->SetId("CTestEntity1");
   e2->SetId("CTestEntity2");

   CALL_OPERATION(Testing, CEntity, void, *e1);
   CALL_OPERATION(Testing, CEntity, void, *e2);

}
