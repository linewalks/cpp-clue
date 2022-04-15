#ifndef CLUE_STREAM_H
#define CLUE_STREAM_H
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <vector>
#include <grpcpp/grpcpp.h>
#include "clue.grpc.pb.h"

using grpc::Status;
using grpc::ClientContext;
using grpc::ClientReaderWriter;

namespace clue {

template <class RequestCreator, class Request, class Response>
class Stream : public grpc::ClientBidiReactor<Request, Response> {
  public:
    Stream(CLUE::Stub* stub, RequestCreator creator)
        : request_creator_(creator),
          stop_(false),
          wait_count_(0) {}

    void Start() {
      this->AddHold();
      this->StartCall();
    }

    std::shared_ptr<Response> FetchOne() {
      AddFetchNum(1);
      return ReadNext();
    }

    void FetchMany(int num, std::vector<std::shared_ptr<Response>>* result) {
      AddFetchNum(num);
      for (int i = 0; i < num; ++i) {
        std::shared_ptr<Response> response = ReadNext();
        result->push_back(response);
      }
    }

    void FetchAll(std::vector<std::shared_ptr<Response>>* result) {
      AddFetchNum(0);
      while (true) {
        std::shared_ptr<Response> response = ReadNext();
        if (response == NULL) {
          break;
        }
        result->push_back(response);
      }
    }

    void OnDone(const Status& s) override {
      std::unique_lock<std::mutex> lock(mutex_);
      stop_ = true;
      response_condition_.notify_one();
    }

    void OnReadDone(bool ok) override {
      std::unique_lock<std::mutex> lock(mutex_);
      --wait_count_;
      if (!ok) {
        stop_ = true;
      }
      response_condition_.notify_one();
    }

    void Close() {
      this->RemoveHold();
      if (!stop_) {
        this->StartWritesDone();
      }
    }

  protected:
    Response PopResponse() {
      Response response = response_queue_.front();
      response_queue_.pop();
      return response;
    }

    std::shared_ptr<Response> ReadNext() {
      std::unique_lock<std::mutex> lock(mutex_);

      std::shared_ptr<Response> response(new Response());
      ++wait_count_;
      this->StartRead(response.get());
      response_condition_.wait(lock, [this] { return wait_count_ <= 0 || stop_; });

      return stop_ ? NULL : response;
    }

    void AddFetchNum(int fetch_num) {
      std::shared_ptr<Request> request = request_creator_.get(fetch_num);
      this->StartWrite(request.get());
    }

    RequestCreator request_creator_;

    std::mutex mutex_;
    std::condition_variable response_condition_;

    std::queue<std::shared_ptr<Response>> send_response_queue_;
    std::queue<Response> response_queue_;
    Response temp_response_;

    bool stop_;
    int wait_count_;
};

}
#endif
