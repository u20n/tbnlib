#include "tbn.hpp"
#include <cassert>
std::vector<int> t {1,2,3,4,5,6,7,8,9,0};

int main(void) {
  /** Convert Testing */
  big ca = Convert::Big("12345");
  int ia = 1; int ib = 2;
  assert(Convert::Concat(ia, ib) == 12);
  assert(Convert::String(ca) == "12345");
  assert(t.back() != t.at(t.size()-2));

  /** Add */
  big aa = Convert::Big("12345"); big ab = Convert::Big("12345");
  assert(Convert::String(add(aa, ab)) == "24690");
 
  /** Subtract */
  big sa = Convert::Big("12345"); big sb = Convert::Big("12345");
  big sc = Convert::Big("9"); big sd = Convert::Big("12345");
  assert(Convert::String(sub(sa, sb)) == "0");
  assert(Convert::String(sub(sc, sd)) == "-12336");
  
  /** Mod */
  big ma = Convert::Big("12345"); big mb = Convert::Big("2");
  // assert(Convert::String(mod(ma, mb)) == "1");
  
  /** Div */
  big da = Convert::Big("12345"); big db = Convert::Big("12345");
  // assert(Convert::String(fdiv(da, db)) == "1");
  return 0;
}
