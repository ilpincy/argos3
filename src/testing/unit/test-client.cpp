#include <argos3/core/utility/networking/tcp_socket.h>

using namespace argos;

int main() {
   try {
      CTCPSocket s;
      std::cout << "[INFO] Connecting to localhost:12345" << std::endl;
      s.Connect("localhost", 12345);
      std::cout << "[INFO] Connected to server" << std::endl;
      CByteArray b;
      b << 1;
      b << 2;
      b << 3;
      b << 4;
      std::cout << "[INFO] Sending 1 2 3 4 (" << b.Size() << " bytes)" << std::endl;
      s.SendByteArray(b);
   }
   catch(CARGoSException& ex) {
      std::cerr << ex.what() << std::endl;
      return 1;
   }
   return 0;
}
