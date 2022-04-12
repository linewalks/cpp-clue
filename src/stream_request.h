#ifndef CLUE_STREAM_REQUEST_H
#define CLUE_STREAM_REQUEST_H

#include <grpcpp/grpcpp.h>

namespace clue {

class RequestCohortCreator {
  public:
    RequestCohortCreator(int cohort_id) {
      cohort_id_ = cohort_id;
    }

    std::shared_ptr<RequestCohortStream> get(int fetch_num) {
      std::shared_ptr<RequestCohortStream> request(new RequestCohortStream());
      request->set_cohort_id(cohort_id_);
      request->set_fetch_num(fetch_num);
      return request;
    }

  protected:
    int cohort_id_;
};

class RequestIncidenceRateCreator {
  public:
    RequestIncidenceRateCreator(int incidence_rate_id) {
      incidence_rate_id_ = incidence_rate_id;
    }

    std::shared_ptr<RequestIncidenceRateStream> get(int fetch_num) {
      std::shared_ptr<RequestIncidenceRateStream> request(new RequestIncidenceRateStream());
      request->set_incidence_rate_id(incidence_rate_id_);
      request->set_fetch_num(fetch_num);
      return request;
    }

  protected:
    int incidence_rate_id_;
};

}

#endif
