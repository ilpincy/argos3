#include <argos3/core/utility/networking/tcp_socket.h>

using namespace argos;

int main() {
   try {
      CTCPSocket s;
      std::cout << "[INFO] Connecting to localhost:12345" << std::endl;
      s.Connect("localhost", 12345);
      std::cout << "[INFO] Connected to server" << std::endl;
      CByteArray b;
      s.ReceiveByteArray(b);
      std::cout << "[INFO] Received " << b.Size() << " bytes" << std::endl;
      SInt32 v[4];
      b >> v[0];
      b >> v[1];
      b >> v[2];
      b >> v[3];
      std::cout << "[INFO]   "
                << " " << v[0]
                << " " << v[1]
                << " " << v[2]
                << " " << v[3]
                << std::endl;
   }
   catch(CARGoSException& ex) {
      std::cerr << ex.what() << std::endl;
      return 1;
   }
   return 0;
}
