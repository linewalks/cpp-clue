#include "clue.grpc.pb.h"
#include "connection.h"

namespace clue {

Connection::Connection(string host, int port, string username, string password) {
  string address = host + ":" + std::to_string(port);
  
  std::shared_ptr<Channel> channel = grpc::CreateChannel(address, grpc::InsecureChannelCredentials());
  // grpc::InterceptChannel(channel, new AuthInterceptor(username, password));

  stub_ = CLUE::NewStub(channel);

  connected_ = Login(username, password);
}

bool Connection::Login(string username, string password) {
  RequestLogin request;
  request.set_email(username);
  request.set_password(password);

  ClientContext context;
  ResponseLogin response;
  
  Status status = stub_->AuthLogin(&context, request, &response);
  if (status.ok()) {
    token_ = response.access_token();
    std::cout << token_ << std::endl;
    return true;
  } else {
    std::cout << status.error_code() << ": " << status.error_message()
              << std::endl;
    return false;
  }
}

void Connection::AuthorizeContext(ClientContext* context) {
  context->AddMetadata("access_token", token_);
}

ResponseCohortList Connection::GetCohortList(int page, int length, string term) {
  RequestCohortList request;
  request.set_page(page);
  request.set_length(length);
  request.set_term(term);

  ClientContext context;
  AuthorizeContext(&context);
  ResponseCohortList response;

  Status status = stub_->GetCohortList(&context, request, &response);
  if (status.ok()) {
    std::cout << response.DebugString() << std::endl;
  } else {
    std::cout << status.error_code() << ": " << status.error_message()
              << std::endl;
  }

  // TODO return type을 어떻게 하지....
  // R과 연결해보고 결정해야할듯
  return response;
}

ResponseCohortList Connection::GetCohortList() {
  return GetCohortList(1, 0, "");
}

}