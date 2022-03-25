#include <iostream>
#include <vector>
#include <tuple>
#include <utility>
/** 
 * bigs are always **stored** in the flipped format
 * e.g. 123,456 = big{6,5,4,3,2,1}
 * ~~~
 * for sanity, Convert is provided for the translation
 * */
using big = std::vector<unsigned int>;
using signed_big = std::tuple<bool, big>;

namespace unpack {
  bool& sign(signed_big p) {
    return std::get<0>(p);
  }
  big& big(signed_big p) {
    return std::get<1>(p);
  }
};


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
  std::string String(signed_big x) {
    std::string s;
    if (unpack::sign(x)) {
      s+='-';
    };
    big xb = unpack::big(x);
    for (int i=xb.size()-1; i>-1; i--) {
      s+=(xb.at(i) + '0');
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

big add(big _a, big _b) {
  big r, a, b;
  // initial compare
  if (_b.size() > _a.size()) {
    a = _b; b = _a;
  } else {
    a = _a; b = _b;
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

signed_big sub(big _a, big _b) {
  big a, b, rv;
  bool rb = false;
  if (_b.size() > _a.size()) {
    rb = true;
    a = _b; b = _a;
  } else {
    a = _a; b = _b;
  }
  for (int i=0; i<b.size(); i++) {
    int s = (a.at(i) - b.at(i));
    if (s < 0) {
      a.at(i+1)--;
      s += 10;
    }
    rv.push_back(s);
  }
  // fill in
  if (rb) {
    for (int i=b.size(); i < a.size(); i++) {
      rv.push_back(a.at(i));
    }
  }
  // get rid of trailing zeros
  for (int i=rv.size()-1; i>0; i--) {
    if (rv.at(i) == 0) {rv.pop_back();}
  }
  return signed_big(rb, rv);
}


/** fourier */
big fdiv(big x, big y) {
  // fix odd pairing
  // honestly this might be unnessecary
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
  bool bi_done = false;
  int i = 0;
  while (!bi_done) {
    // check for done
    if (b_n.size() == (sizeof(an) / sizeof(*an))) {bi_done = true;}
    int i = (i == 0) ? i-- : i;
    int ls = (r + cn[i+1]);
    int rs = 0;
    if (!b_n.empty()) {
      int j = 2;
      while(j != i) {
        // v should be (i-j+1), but b_n's index starts at 0
        rs -= (b_n.at(i-j) * an[j-1]);
      }
    }
    b_n.push_back((ls-rs)/an[0]);
    r = ((ls-rs)%an[0]);
    i++;
  }
  // interpret b terms
  for (auto b: b_n) {
     
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

/** [possible] FEATURE
 * ~~~~
 * this could very easily take any two bigs
 */
int mod2(big x) {
  int rem = 0;
  for (int i=x.size()-1; i>-1; i--) {
    rem = (rem*10 + x.at(i)) % 2;
  }
  return rem;
}
