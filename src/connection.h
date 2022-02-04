#ifndef CLUE_CONNECTION_H
#define CLUE_CONNECTION_H
#include <string>

#include <grpcpp/grpcpp.h>

#include "clue.grpc.pb.h"
#include "stream.h"

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

    std::shared_ptr<Stream<RequestCohortStream, PersonInfo>> GetCohortPersonTable(int cohort_id);

  protected:
    bool Login(string username, string password);
    void AuthorizeContext(ClientContext* context);

    std::unique_ptr<CLUE::Stub> stub_;
    bool connected_;
    string token_;
};

}
#endif
