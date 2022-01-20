#include <string>

#include <grpcpp/grpcpp.h>

#include "clue.grpc.pb.h"

using std::string;
using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

namespace clue {
  class Connection {
    public:
      Connection(string host, int port, string username, string password);

      ResponseCohortList GetCohortList(int page, int length, string term);
      ResponseCohortList GetCohortList();

    protected:
      bool Login(string username, string password);
      void AuthorizeContext(ClientContext* context);

      std::unique_ptr<CLUE::Stub> stub_;
      bool connected_;
      string token_;
  };
}
