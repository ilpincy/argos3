#include <argos3/core/utility/math/box.h>
#include <argos3/core/utility/math/ray3.h>
#include <iostream>

using namespace argos;

int main(int argc, char* argv[]) {
   CVector3 cCenter(1.0, 1.0, 0.0);
   CBox b(CVector3(2.0,2.0,1.0), cCenter, CQuaternion(CRadians::PI_OVER_TWO, CVector3::Z));
   Real t;
   bool result;
   for(size_t i = 0; i < 11; ++i) {
      CRay3 r1(cCenter + CVector3(-2, -1.25 + i * 0.25, .5),
               cCenter + CVector3( 2, -1.25 + i * 0.25, .5));
      result = b.Intersects(t, r1);
      std::cout << "ray = [" << r1.GetStart() << "],[" << r1.GetEnd() << "]"
                << ", result = " << result
                << ", t = " << t
                << std::endl;
      if(result != (i > 0 && i < 10)) {
         std::cerr << "ERROR" << std::endl;
         return 1;
      }
      CRay3 r2(cCenter + CVector3(-1.25 + i * 0.25, -2, .5),
               cCenter + CVector3(-1.25 + i * 0.25,  2, .5));
      result = b.Intersects(t, r2);
      std::cout << "ray = [" << r2.GetStart() << "],[" << r2.GetEnd() << "]"
                << ", result = " << result
                << ", t = " << t
                << std::endl;
      if(result != (i > 0 && i < 10)) {
         std::cerr << "ERROR" << std::endl;
         return 1;
      }
   }
   return 0;
}
