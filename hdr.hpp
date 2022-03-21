#include <iostream>
#include <vector>
#include <utility>
/** 
 * bigs are always **stored** in the flipped format
 * e.g. 123,456 = big{6,5,4,3,2,1}
 * ~~~
 * for sanity, Convert is provided for the translation
 * */
using big = std::vector<unsigned int>;

void print(big n) {
  for (auto e:n) {std::cout << e;}
  std::cout  << '\n';
}

namespace Convert {
  int Concat(int _a, int _b) {
    std::string a = std::to_string(_a);
    std::string b = std::to_string(_b);
    std::string p = (a+b);
    return stoi(p);
  }

  std::string String(big x) {
    std::string s;
    for (int i=x.size()-1; i>-1; i--) {
      s+=(x.at(i) + '0');
    };
    return s;
  }
  
  big Big(std::string s) {
    big v;
    v.resize(s.length());
    v.reserve(s.length());
    /**
     * OPTIMIZATION
     * ~~~~
     * this could be contained in a single for loop but i'm lazy
     */
    // map 2 vector
    for (int i=0; i<s.length(); i++) {
      v.at(i) = s[i] - '0';
    }
    // flip vector
    for (int l=0, h=v.size()-1; l<h; l++, h--) {
      std::swap(v.at(l), v.at(h));
    }
    return v;
  }
}

/** positives only */
big add(big _a, big _b) {
  big r, a, b;
  // initial compare
  if (_b.size() > _a.size()) {
    a = _b;
    b = _a;
  } else {
    a = _a;
    b = _b;
  }
  // addition
  for (int i=0; i<b.size(); i++) {
    unsigned int s = (a.at(i) + b.at(i));
    if (s>9) {
      a.at(i+1)+=1;
      s = (s-10);
    }
    r.push_back(s);
  }
  // fix leftover digits
  if (r.size() < a.size()) {
    int d = a.size() - (a.size()-r.size());
    for (int i=d; i<a.size(); i++) {
      r.push_back(a.at(i));
    } 
  }
  
  return r;
}

/** fourier */
big fdiv(big a, big b) {
  // split into 2x
  std::cout << "size: " << a.size() << '\n';

  // fix odd pairing
  if (a.size()%2) {
    a.push_back(0);
  }

  int tp = (a.size()/2)-1; // total pairs n
  int cn[tp]; // c_n array
  int j = 0; // index for c_n
  // loop each 2x
  for (int i=a.size()-1; i>-1; i-=2) {
    int p = Convert::Concat(a.at(i), a.at(i-1));
    std::cout << "p: " << p << '\n';
    std::cout << a.at(i) << '|' << a.at(i-1) << '\n'; 
    // forming c_n
    int c = p*(tp*100);
    cn[j] = c;
    tp--; j++;
  }
  for (int i=0; i>-1; i++) {
    int r; // remainder
    int f = (cn[i]+cn[i+1]) / b.end() /** a_1 */;
  }
}

big multi(big a, big b) {
  big r;
  size_t len = (a.size()+b.size());
  r.resize(len);
  r.reserve(len);

  for (int i=0; i<a.size(); i++) {
    for (int j=0; j<b.size(); j++) {
      r.at(i+j) += a.at(i)*b.at(j);
      r.at(i+j+1) = r.at(i+j+1) + r.at(i+j)/10;
      r.at(i+j) %= 10;
    }
  }
  if (r.back() == 0) {r.pop_back();}

  return r;
}

int mod2(big x) {
  int rem = 0;
  for (int i=x.size()-1; i>-1; i--) {
    rem = (rem*10 + x.at(i)) % 2;
  }
  return rem;
}
