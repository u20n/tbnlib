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
  auto& cn = c.num; auto& an = a.num;
  // split into power-modifer complient sections
  unsigned int d = 0; // decimal index
  // only [c|2]
  while (cn.size() % 2) {
    cn.push_back('0');
    d++;
  }

  // 'comma'(?) form
  std::vector<int> aterm, bterm, cterm;
  
  // QoL power rule
  auto pmod = [](int i) {
    return (0 < i) ? round(pow(10, i+1)) : 1;
  };
  // QoL folding
  auto fold = [pmod](std::vector<int> n) {
    int f = 0;
    for (unsigned int i=0; i<n.size(); i++) {
      f += (n.at(i) * pmod(i));
    } 
    return f;
  }; 

  // [BREAKING]: in the mod2 test, a_0 isn't a two digit number...
  // forming the a and c terms 
  for (int i = an.size()-1; -1 < i; i--) {
    // we don't equalise the a term; we might have an uneven digit count
    int a_i = (i != 0) ? fold({an.at(i), an.at(i-1)}) : an.at(i);
    aterm.push_back(a_i);
  }
  for (int i = cn.size()-1; -1 < i; i--) {
    // [BREAKING] i'm pretty sure that folding in this direction is opposite to correct...
    cterm.push_back(fold({cn.at(i), cn.at(i-1)}));
    i--;
  } 
  
  // applying the general term
  int rem = cterm.at(0);
  int a_0 = aterm.at(0); // reduce complexity
  for (unsigned int i = 0; i < cterm.size()-1; i++) { 
    // (left) numerator
    int lnum = (rem*pmod(1)) + cterm.at(i+1);
    std::cout << "lnum: " << lnum << '\n'; // DEBUG 
    // apply sum
    int sum = 0;
    for (unsigned int j = 2; -1 < (i - (j+1)); j++) {
      std::cout << "b :" << bterm.at(i-(j+1)) << '\n'; // DEBUG
      std::cout << "a :" << aterm.at(j) << '\n'; // DEBUG
      
      sum -= (bterm.at(i-(j+1)) * aterm.at(j)); 
    }
    std::cout << "sum: " << sum << '\n'; // DEBUG
    rem = (lnum + sum) % a_0;
    std::cout << "a_0: " << a_0 << '\n'; // DEBUG
    std::cout << "rem: " << rem << '\n'; // DEBUG
    std::cout << "bterm: " << (lnum + sum - rem) / a_0 << '\n'; // DEBUG
    std::cout << "--------------------------------\n"; // DEBUG 
    bterm.push_back((lnum + sum - rem) / a_0);
  }

  // - fold in term
  // - apply power rule
  // - applying each term
  big b;
  for (int i = bterm.size()-1; -1 < i; i--) { 
    b = add(b, big(bterm.at(i) * pmod(i)));
  } 
  b.point = d; // set decimal point  
  return {b, big(rem)};
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
