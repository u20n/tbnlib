#include "tbn.hpp"
#include <cassert>

using namespace tbn;
int main(void) {
  /** Construct */
  big ca("12345");
  assert(ca.string() == "12345");
  std::cout << "Postive Construct OK\n";
  
  big cb("-12345");
  assert(cb.string() == "-12345");
  std::cout << "Negative Construct OK\n";

  /** Add */
  big aa("12345"); big ab("12345");
  assert(add(aa, ab).string() == "24690");
  std::cout << "Add OK\n";

  /** Subtract */
  big sa("12345"); big sb("12345");
  big sc("9"); big sd("12345");

  assert(sub(sa, sb).string() == "0");
  std::cout << "Positive Sub OK\n";

  assert(sub(sc, sd).string() == "-12336");
  std::cout << "Negative Sub OK\n";

  /** Mod */
  big ma("12345"); big mb("2");
  // assert(Convert::String(mod(ma, mb)) == "1");
  // std::cout << "Construct OK\n";

  /** Div */
  big da("12345"); big db("12345");
  // assert(Convert::String(fdiv(da, db)) == "1");
  // std::cout << "Construct OK\n";

  return 0;
}
