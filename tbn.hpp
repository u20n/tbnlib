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
// QoL might be the addition of a big construction

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

    int mi = 0;
    for (int i=s.length()-1; i>-1; i--) {
      v.at(mi) = (s[i] - '0');
      mi++;
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

/**
** fourier *
big fdiv(big x, big y) {
  // split into 2x
  std::cout << "size: " << x.size() << '\n';

  // fix odd pairing
  if (x.size()%2) {
    x.push_back(0);
  }
  int an[(y.size()/2)+(y.size()%2)]; // a_n array w/o leading zero
  int tp = (x.size()/2)-1; // total pairs n
  int cn[tp]; // c_n array
  int j = 0; // index for c_n
  // translate to 2 digit pairs
  for (int i=x.size()-1; i>-1; i-=2) {
    int p = Convert::Concat(x.at(i), x.at(i-1));
    std::cout << "p: " << p << '\n';
    std::cout << x.at(i) << '|' << x.at(i-1) << '\n'; 
    // forming c_n
    int c = p*(tp*100);
    cn[j] = c;
    tp--; j++;
  }
  int r = cn[0];
  std::vector<int> b_n;
  int b_i(int i) {
    // TODO: check for finish
    int ls = (r + cn[i+1]);
    int rs = 0;
    if (!b_n.empty()) {
      int j = 2;
      while(j != i) {
        // v should be (i-j+1), but b_n's index starts at 0
        rs -= (b_n.at(i-j) * an.at(j-1));
      }
    }
    b_n.push_back((ls-rs)/an[0]);
    r = ((ls-rs)%an[0]);
    b_i(i++);
  }
}
*/
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
