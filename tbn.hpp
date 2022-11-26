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
  std::size_t point = 0;
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

// == fourier ==
// < quotient, remainder >
std::array<big, 2> div(big c, big a) {
  auto& cn = c.num; auto& an = a.num;
  // split into power-modifer complient sections
  unsigned int d = 0; // decimal index
  // we only shift c, because a_1[0] must be non-zero;
  // the way we apply the general form, 
  // a mod 2 doesn't necessarily need to be equal to 0
  while (cn.size() % 2) {
    cn.push_back('0');
    d++;
  }
  // expand form
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

  int rem = fold({cn.at(cn.size()-1), cn.at(cn.size()-2)});
  cn.erase(cn.end()-2, cn.end());

  // forming the c and a terms
  for (int i = cn.size()-1; -1 < i; i--) {
    cterm.push_back(fold({cn.at(i), cn.at(i-1)}));
    i--;
  }
  for (int i = an.size()-1; -1 < i; i--) {
    // we don't equalise the a term; we might have an uneven digit count
    int a_i = (i != 0) ? fold({an.at(i), an.at(i-1)}) : an.at(i);
    aterm.push_back(a_i);
  }
   
  // applying the general term
  // we have to move j back one to compensate for an assumed base index of 1
  int a_0 = aterm.at(0); // reduce complexity
  for (unsigned int i = 0; i < cterm.size(); i++) { 
    // (left) numerator
    int lnum = rem + cterm.at(i);
    std::cout << "ln: " << lnum << '\n'; // DEBUG 
    // apply sum
    int sum = 0;
    for (unsigned int j = 1; -1 < (i - (j+1)); j++) {
      std::cout << "b :" << bterm.at(i-(j+1)) << '\n'; // DEBUG
      std::cout << "a :" << aterm.at(j) << '\n'; // DEBUG
      
      sum -= (bterm.at(i-(j+1)) * aterm.at(j)); 
    }
    std::cout << "s: " << sum << '\n'; // DEBUG
    rem = (lnum + sum) % a_0;
    bterm.push_back((lnum + sum - rem) / a_0);
  }

  // - fold in term
  // - apply power rule
  // - determine overall sign (by applying each term)
  big b;
  for (int i = bterm.size()-1; -1 < i; i--) {
    auto& b_i = bterm.at(i); 
    if (b_i < 0) {
      b = sub(b, big(b_i*pmod(i)));
    } else {
      b = add(b, big(b_i*pmod(i))); 
    }
  }
  print(b); // DEBUG
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
  // order the bigs
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
