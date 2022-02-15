/*
 *
 * Copyright 2015 gRPC authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>
#include <google/protobuf/message.h>
#include "clue.grpc.pb.h"
#include "clue.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

int main(int argc, char** argv) {
  // Instantiate the client. It requires a channel, out of which the actual RPCs
  // are created. This channel models a connection to an endpoint specified by
  // the argument "--target=" which is the only expected argument.
  // We indicate that the channel isn't authenticated (use of
  // InsecureChannelCredentials()).
  std::string target_str;
  std::string arg_str("--target");
  if (argc > 1) {
    std::string arg_val = argv[1];
    size_t start_pos = arg_val.find(arg_str);
    if (start_pos != std::string::npos) {
      start_pos += arg_str.size();
      if (arg_val[start_pos] == '=') {
        target_str = arg_val.substr(start_pos + 1);
      } else {
        std::cout << "The only correct argument syntax is --target="
                  << std::endl;
        return 0;
      }
    } else {
      std::cout << "The only acceptable argument is --target=" << std::endl;
      return 0;
    }
  } else {
    target_str = "localhost:50051";
  }

  clue::CLUE clue_client(
      std::string("192.168.0.69"),
      8889,
      std::string("test@linewalks.com"),
      std::string("q1w2e3r4!"));

  std::shared_ptr<clue::Connection> conn = clue_client.Connect();

  ResponseCohortList response = conn->GetCohortList(1, 10, "");
  int cohort_size = response.cohort_list_size();
  std::cout << "cohort size " << cohort_size << std::endl;

  if (cohort_size > 0) {
    CohortInfo cohort_info = response.cohort_list()[0];
    std::cout << "cohort id " << cohort_info.id() << std::endl;
    std::cout << "cohort name " << cohort_info.name() << std::endl;
  }

  std::cout << "GetCohortPersonTable" << std::endl;

  std::shared_ptr<clue::Stream<RequestCohortStream, PersonInfo>> person_stream = conn->GetCohortPersonTable(527);

  std::cout << "Outside" << std::endl;
  std::cout << "person_stream " << person_stream << std::endl;
  PersonInfo person = person_stream->FetchOne();
  std::cout << "Person : " << person.person_id() << std::endl;

  sleep(1);
  PersonInfo person2 = person_stream->FetchOne();
  std::cout << "Person2 : " << person2.person_id() << std::endl;

  sleep(0.5);
  std::vector<PersonInfo> person_vector = person_stream->FetchMany(10);
  std::cout << "Person vector size : " << person_vector.size() << std::endl;

  std::cout << "GetCohortPersonTableDone" << std::endl;
  person_stream->Close();
  sleep(1);

  return 0;
}
