#include "tbn.hpp"
#include <cassert>

using namespace tbn;
int main(void) {
  /** Construct */
  big ca("12345");
  assert(ca.string() == "12345");
  std::cout << "Postive Construct        OK\n";
  
  big cb("-12345");
  assert(cb.string() == "-12345");
  std::cout << "Negative Construct       OK\n";
  
  big cc(12345);
  assert(cc.string() == "12345");
  std::cout << "Positive Int Construct   OK\n";

  big cd(-12345);
  assert(cd.string() == "-12345");
  std::cout << "Negative Int Construct   OK\n";

  /** Add */
  big aa("12345"); big ab("12345");
  assert(add(aa, ab).string() == "24690");
  std::cout << "Add                      OK\n";

  /** Subtract */
  big sa("12345"); big sb("12345");
  big sc("9"); big sd("12345");

  assert(sub(sa, sb).string() == "0");
  std::cout << "Positive Sub             OK\n";

  assert(sub(sc, sd).string() == "-12336");
  std::cout << "Negative Sub             OK\n";

  /** Mod */
  big ma("12345"); big mb("2");
  assert(mod(ma, mb).string() == "1");
  std::cout << "<big> mod <big>          OK\n";
  
  assert(mod(ma, 2) == 1);
  std::cout << "<big> mod <unsigned int> OK\n";

  assert(mod2(ma) == 1);
  std::cout << "<big> mod2               OK\n";
  
  std::cout << "Modulo                   OK\n";

  /** Div */
  big da("12345"); big db("12345");
  assert(div(da, db)[0].string() == "1");
  std::cout << "Division                 OK\n";

  return 0;
}
