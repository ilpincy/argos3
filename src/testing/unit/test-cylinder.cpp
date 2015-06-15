#include <argos3/core/utility/math/cylinder.h>
#include <argos3/core/utility/math/ray3.h>
#include <iostream>

using namespace argos;

int main(int argc, char* argv[]) {
   CCylinder c(1.0, 1.0);
   CRay3 r(CVector3(0, 0, -2), CVector3(0, 0, 2));
   Real t;
   bool result = c.Intersects(t, r);
   std::cout << "result = " << result
             << ", t = " << t
             << std::endl;
   return 0;
}
