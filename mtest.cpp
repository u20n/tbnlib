#include "tbn.hpp"
#include <cassert>
std::vector<int> t {1,2,3,4,5,6,7,8,9,0};

int main(void) {
  big b0 = Convert::Big("12345");
  std::cout << "b0: "; print(b0);
  std::cout << "cS-b0: " << Convert::String(b0) << '\n';
  assert(Convert::String(b0) == "12345");
  assert(t.back() != t.at(t.size()-2));
  return 0;
}
