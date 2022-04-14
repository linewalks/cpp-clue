#include "clue.grpc.pb.h"
#include "connection.h"

namespace clue {

Connection::Connection(string host, int port, string username, string password) {
  string address = host + ":" + std::to_string(port);
  std::shared_ptr<Channel> channel = grpc::CreateChannel(address, grpc::InsecureChannelCredentials());

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
  stub_->GetCohortList(&context, request, &response);
  return response;
}

ResponseCohortList Connection::GetCohortList() {
  return GetCohortList(1, 0, "");
}

ResponseComparison Connection::GetCohortComparison(int comparison_id) {
  RequestComparison request;
  request.set_comparison_id(comparison_id);

  ClientContext context;
  AuthorizeContext(&context);

  ResponseComparison response;
  stub_->GetCohortComparison(&context, request, &response);
  return response;
}

ResponseIncidenceRateResult Connection::GetIncidenceRateResult(int incidence_rate_id) {
  RequestIncidenceRate request;
  request.set_incidence_rate_id(incidence_rate_id);

  ClientContext context;
  AuthorizeContext(&context);

  ResponseIncidenceRateResult response;
  stub_->GetIncidenceRateResult(&context, request, &response);

  return response;
}

std::shared_ptr<Stream<RequestIncidenceRateCreator, RequestIncidenceRateStream, IncidenceRateRawInfo>> Connection::GetIncidenceRateRaw(int incidence_rate_id) {
  std::shared_ptr<ClientContext> context(new ClientContext());
  AuthorizeContext(context.get());
  std::shared_ptr<Stream<RequestIncidenceRateCreator, RequestIncidenceRateStream, IncidenceRateRawInfo>> stream(
      new Stream<RequestIncidenceRateCreator, RequestIncidenceRateStream, IncidenceRateRawInfo>(stub_.get(), incidence_rate_id));
  stub_->async()->GetIncidenceRateRaw(context.get(), stream.get());
  stream->Start();
  return stream;
}

}
