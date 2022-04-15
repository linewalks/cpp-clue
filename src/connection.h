#ifndef CLUE_CONNECTION_H
#define CLUE_CONNECTION_H
#include <string>

#include <grpcpp/grpcpp.h>

#include "clue.grpc.pb.h"
#include "stream.h"
#include "stream_request.h"

using std::string;
using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

#define INFO(t) t##Info
#define FUNC(t) GetCohort##t##Table
#define GET_COHORT_TABLE(t) std::shared_ptr<Stream<RequestCohortCreator, RequestCohortStream, INFO(t)>> FUNC(t)(int cohort_id) { \
  RequestCohortCreator creator(cohort_id); \
  std::shared_ptr<ClientContext> context(new ClientContext()); \
  AuthorizeContext(context.get()); \
  std::shared_ptr<Stream<RequestCohortCreator, RequestCohortStream, INFO(t)>> stream( \
      new Stream<RequestCohortCreator, RequestCohortStream, INFO(t)>(stub_.get(), creator)); \
  stub_->async()->FUNC(t)(context.get(), stream.get()); \
  stream->Start(); \
  return stream; \
};

namespace clue {

class Connection {
  public:
    Connection(string host, int port, string username, string password);

    ResponseCohortList GetCohortList(int page, int length, string term);
    ResponseCohortList GetCohortList();

    GET_COHORT_TABLE(Person)
    GET_COHORT_TABLE(ConditionOccurrence)
    GET_COHORT_TABLE(Death)
    GET_COHORT_TABLE(DeviceExposure)
    GET_COHORT_TABLE(DrugExposure)
    GET_COHORT_TABLE(Measurement)
    GET_COHORT_TABLE(Observation)
    GET_COHORT_TABLE(ObservationPeriod)
    GET_COHORT_TABLE(ProcedureOccurrence)
    GET_COHORT_TABLE(VisitOccurrence)

    ResponseComparison GetCohortComparison(int comparison_id);

    ResponseIncidenceRateResult GetIncidenceRateResult(int incidence_rate_id);
    std::shared_ptr<Stream<RequestIncidenceRateCreator, RequestIncidenceRateStream, IncidenceRateRawInfo>> GetIncidenceRateRaw(int incidence_rate_id);
  protected:
    bool Login(string username, string password);
    void AuthorizeContext(ClientContext* context);

    std::unique_ptr<CLUE::Stub> stub_;
    bool connected_;
    string token_;
};

}
#endif
