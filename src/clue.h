
#include <string>

#include <grpcpp/grpcpp.h>

#include "connection.h"

using std::string;

namespace clue {

class CLUE {
  public:
    CLUE(string host, int port, string username, string password);
    std::shared_ptr<Connection> Connect();

  protected:
    string host_;
    int port_;
    string username_;
    string password_;
};

} // namespace clue
