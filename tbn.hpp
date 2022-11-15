#include <iostream>
#include <deque>
#include <array>
#include <utility>
#include <cmath>
#include <string>

namespace tbn { // == begin namespace ==
struct big {
  // states //
  bool sign = true;
  std::size_t point = 0;
  std::deque<std::size_t> num; // bigs are always stored flipped; make sure to translate!
  
  // translation //
  std::string string() {
    std::string s = (this->sign) ? "" : "-";
    for (int i=this->num.size()-1; -1 < i; i--) {
      s += (this->num.at(i) + '0');
    } return s;
  }

  big(std::string s) {
    if (s.at(0) == '-') {
      this->sign = false;
      s.erase(0,1);
    }
    this->num.resize(s.length());
    std::size_t bi = 0;
    for (int i = s.length()-1; -1 < i; i--) {
      this->num.at(bi) = (s[i] - '0'); bi++;
    } 
  }
  big(){}
};

void print(big n) {
  for (const auto& e: n.num) {std::cout << e;}
  std::cout  << '\n';
}

/**
 * This will break for negative _a or _b;
 * This is intentional: use sub(tract)!
 * */
big add(big _a, big _b) {
  big r, a, b;
  auto& an = a.num; auto& bn = b.num; auto& rn = r.num;
  
  // initial compare
  if (_a.num.size() < _b.num.size()) {
    a = _b; b = _a;
  } else {
    a = _a; b = _b;
  }

  // addition
  for (int i=0; i<bn.size(); i++) {
    unsigned int s = (an.at(i) + bn.at(i));
    if (9<s) {
      an.at(i+1)+=1;
      s = (s-10);
    }
    rn.push_back(s);
  }
  // fix leftover digits
  if (rn.size() < an.size()) {
    int d = an.size() - (an.size()-rn.size());
    for (int i=d; i<an.size(); i++) {
      rn.push_back(an.at(i));
    } 
  }
  
  return r;
}

big sub(big _a, big _b) {
  big a, b, r;
  auto& an = a.num; auto& bn = b.num; auto& rn = r.num;

  if(_b.num.size() <= _a.num.size()) {
    for (int i = _a.num.size()-1; -1 < i; i--) {
      if (_a.num.at(i) < _b.num.at(i)) r.sign = 0;
    }
  } else r.sign = 0;
  
  switch (r.sign) {
    case 0:
      a = _b; b = _a;
      break;
    case 1:
      a = _a; b = _b;
      break;
  }
  
  while(bn.size() < an.size()) {bn.push_back(0);}
  
  for (int i=0; i<bn.size(); i++) {
    int s = (an.at(i) - bn.at(i));
    if (s < 0) {
      an.at(i+1)--;
      s += 10;
    }
    rn.push_back(s);
  }

  // get rid of trailing zeros
  for (int i=rn.size()-1; i>0; i--) {
    if (rn.at(i) == 0) {rn.pop_back();}
  } 
  return r;
}

/** 
 * == fourier ==
 * < quotient, remainder >
 * 
 *
 * */
std::array<big, 2> div(big c, big a) { 

}


/** x mod 2 */
std::size_t mod2(big x) {
  auto& xn = x.num;
  int rem = 0;
  for (int i=xn.size()-1; i>-1; i--) {
    rem = (rem*10 + xn.at(i)) % 2;
  }

  return rem;
}

/** a mod b */
// [TODO]
int mod(big a, big b) { 
}

} // == end namespace ==
