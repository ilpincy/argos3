
/**
 * @file <argos3/testing/unit/test-range.cpp>
 *
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 */
#include <argos3/core/utility/math/rng.h>
#include <argos3/core/utility/string_utilities.h>

#include <cstdlib>
#include <iostream>
#include <fstream>

using namespace argos;

int main(int argc, char** argv) {
   if(argc != 2) {
      std::cerr << "Usage: test-range <seed>" << std::endl;
      abort();
   }
   UInt32 unSeed = FromString<UInt32>(argv[1]);
   CRandom::CreateCategory("argos", unSeed);
   CRandom::CRNG* pcRNG = CRandom::CreateRNG("argos");
   UInt32 unMax = 2;
   CRange<UInt32> cRange(0, unMax);
   std::string strFileName = "range_log_" + ToString(unSeed);
   std::ofstream outStream(strFileName.c_str());
   outStream << "[min,max) " << cRange << std::endl;
   UInt32 unSelected = pcRNG->Uniform(cRange);
   UInt64 i;
   for(i = 0; unSelected != unMax; ++i) {
      unSelected = pcRNG->Uniform(cRange);
      outStream << "seed: " << unSeed << ", iteration: " << i << ", selected: " << unSelected << std::endl;
   }
   outStream << "DONE! seed: " << unSeed << ", iteration: " << i << std::endl;
   return 0;
}
