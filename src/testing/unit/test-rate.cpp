#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/core/utility/rate.h>
#include <iostream>

using namespace argos;

int main() {
   CRate cRate(10000);
   std::cout << "start" << std::endl;
   for(int i = 0; i < 10; ++i) {
      cRate.Sleep();
      std::cout << "i = " << i << std::endl;
      LOGERR.Flush();
   }
}
