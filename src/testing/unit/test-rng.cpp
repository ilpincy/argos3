#include <argos3/core/utility/math/rng.h>
#include <fstream>
#include <sstream>
#include <cerrno>
#include <cstring>

using namespace argos;

static const CRange<UInt32>   URANGE(0, 100);
static const CRange<SInt32>   SRANGE(-100, 100);
static const CRange<Real>     FRANGE(-10.0, 10.0);
static UInt32 N = 10000;

template<typename T>
void GenerateU(const std::string& str_fname,
               CRange<T> c_range) {
   CRandom::CRNG* m_pcRNG = CRandom::CreateRNG("testing");
   std::ofstream cFile(str_fname.c_str(), std::ofstream::out | std::ofstream::trunc);
   if(cFile.fail())
      THROW_ARGOSEXCEPTION("Error opening \"" << str_fname << "\": " << strerror(errno));
   for(UInt32 i = 0; i < N; ++i) {
      cFile << m_pcRNG->Uniform(c_range) << std::endl;
   }
   cFile.close();
}

void GenerateB(Real f_true) {
   CRandom::CRNG* m_pcRNG = CRandom::CreateRNG("testing");
   UInt32 unTrue = 0, unFalse = 0;
   for(UInt32 i = 0; i < N; ++i) {
      if(m_pcRNG->Bernoulli(f_true)) ++unTrue;
      else ++unFalse;
   }
   std::cout << "Bernoulli(" << f_true << "): "
             << unTrue << " true (" << static_cast<Real>(unTrue) / static_cast<Real>(N) << "), "
             << unFalse << " false (" << static_cast<Real>(unFalse) / static_cast<Real>(N) << ")"
             << std::endl;
}

void GenerateG(Real f_std_dev, Real f_mean) {
   CRandom::CRNG* m_pcRNG = CRandom::CreateRNG("testing");
   std::ostringstream ssFName;
   ssFName << "gfile_" << f_std_dev << "_" << f_mean << ".dat";
   std::ofstream cFile(ssFName.str().c_str(), std::ofstream::out | std::ofstream::trunc);
   if(cFile.fail())
      THROW_ARGOSEXCEPTION("Error opening \"" << ssFName.str() << "\": " << strerror(errno));
   for(UInt32 i = 0; i < N; ++i) {
      cFile << m_pcRNG->Gaussian(f_std_dev, f_mean) << std::endl;
   }
   cFile.close();
}

int main() {
   CRandom::CreateCategory("testing", 12345);
   GenerateU("ufile.dat", URANGE);
   GenerateU("sfile.dat", SRANGE);
   GenerateU("ffile.dat", FRANGE);
   for(UInt32 i = 1; i < 4; ++i) {
      GenerateB(0.25f * i);
   }
   for(UInt32 s = 1; s < 4; ++s) {
      for(UInt32 m = 0; m < 3; ++m) {
         GenerateG(s, m);
      }
   }
   CRandom::RemoveCategory("testing");
}
