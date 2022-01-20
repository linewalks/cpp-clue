#include "clue.h"

namespace clue {

CLUE::CLUE(string host, int port, string username, string password)
    : host_(host),
      port_(port),
      username_(username),
      password_(password) {
}

Connection CLUE::Connect() {
  return Connection(
      host_,
      port_,
      username_,
      password_);
};

} // namespace clue
