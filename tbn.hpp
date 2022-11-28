#include <iostream>
#include <deque>
#include <array>
#include <utility>
#include <cmath>
#include <string>
#include <vector>

namespace tbn { // == begin namespace ==
struct big {
  // states //
  bool sign = true;
  unsigned int point = 0;
  std::deque<unsigned int> num; // bigs are always stored flipped; make sure to translate!
  
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
  big(int n) : big(std::to_string(n)) {} 
  big(){}
};

void print(big n) {
  std::cout << "Sign: " << n.sign << '\n' << "Value: ";
  for (int i = n.num.size()-1; -1 < i; i--) {std::cout << n.num.at(i);}
  std::cout << '\n' << "Decimal Place: " << n.point << '\n';
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

big add(big _a, big _b) {
  big r, a, b;
  if (_a.sign == _b.sign) {
    r.sign = _a.sign; // ... or _b.sign 
  } else {
    return sub(_a, _b); 
  }
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

// == fourier ==
// < quotient, remainder >
// <big> c / <big> a -> <big> q, <big> r
std::array<big, 2> div(big c, big a) {
  // TODO
  
  std::deque<int> bterm; // we'll have to balance this later
  
  // pull out initial c_0
  int rem = (c.num.at(c.num.size()-1 + c.num.at(c.num.size()-2) * 100);
  // save the operation...
  int a_0 = (a.num.at(a.size()-1) + a.num.at(a.num.size()-2) * 100);
  // it's slightly more performant to calculate the (actual) index than to flip the a and c terms...
  // step size is 2
  for (int i = c.num.size()-1; 2/** saves the erase call for initial rem */ < i); i-=2) {
    int sum = 0;
    for (unsigned int j=2; -1 < i - (j + 1); j+=2) {
      // a_j can be a two digit or one digit integer; this checks if a two digit a_j is possible
      int a_j_len = ( 2 <= (a.size()-1) - j ) ? 100 : 0;
      sum -= (bterm.at(i - (j+1)) * (a.num.at(j) + a.num.at(j+1)*a_j_len);
    }
    std::cout << "sum: " << sum << '\n'; // DEBUG
    // fold in r and c_{i+1}, apply sum
    int num = (rem*100 + (c.num.at(i) + c.num.at(i+1)*100)) + sum;
    // calculate (new) rem
    rem = (num % a_0);
    // push to bterm
    bterm.push_front((num - rem)/a_0);
  }

  // balance b terms
}


// <big> mod <unsigned int>
unsigned int mod(big x, unsigned int a) {
  auto& xn = x.num;
  unsigned int rem = 0;
  for (int i=xn.size()-1; i>-1; i--) {
    rem = (rem*10 + xn.at(i)) % a;
  }
  return rem;
}

// <big> mod 2
unsigned int mod2(big x) {return mod(x, 2);}

// <big> mod <big>
big mod(big x, big y) {return div(x, y)[1];}

// multi: <big> * <big>
big multi(big x, big y) {
  // order
  big a, b, f;
  if (x.num.size() <= y.num.size()) {
    a = y; b = x;
  } else {
    a = x; b = y;
  }

  // signs
  if (
    (x.sign && !y.sign) ||
    (!x.sign && y.sign)
    ) f.sign = false;
  
  const auto& an = a.num; const auto& bn = b.num;
  std::deque<unsigned int>r(an.size()+bn.size());
  // compute 
  for(unsigned int i = 0; i < bn.size(); i++) {
    for(unsigned int j = 0; j < an.size(); j++) { 
      r.at(i+j) += an.at(j) * bn.at(i);
      r.at(i+j+1) = r.at(i+j+1) + (r.at(i+j) / 10);
      r.at(i+j) %= 10;
    }
  }
  
  // clean trailing zeros
  while(r.back() == 0) {
    r.pop_back();
  }
  // done
  f.num = r; 
  return f;
}

} // == end namespace ==
