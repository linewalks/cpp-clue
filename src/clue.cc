#include "clue.h"

namespace clue {

CLUE::CLUE(string host, int port, string username, string password)
    : host_(host),
      port_(port),
      username_(username),
      password_(password) {
}

std::shared_ptr<Connection> CLUE::Connect() {
  return std::shared_ptr<Connection>(
      new Connection(
          host_,
          port_,
          username_,
          password_));
};

} // namespace clue
