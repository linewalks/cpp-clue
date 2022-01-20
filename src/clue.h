
#include <string>

#include <grpcpp/grpcpp.h>
#include "clue.grpc.pb.h"

#include "connection.h"

using std::string;

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

namespace clue {

class CLUE {
  public:
    CLUE(string host, int port, string username, string password);

    Connection Connect();

  protected:
    string host_;
    int port_;
    string username_;
    string password_;
};

} // namespace clue
