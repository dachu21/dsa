#include <istream>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <chrono>
#include <iostream>

#include "BigInt.hpp"

BigInt::BigInt() : sign(1) {
}

BigInt::BigInt(long long v) {
  *this = v;
}

BigInt::BigInt(const std::string &s, int radix) {
  if (radix == 10)
    read(s);
  else
    *this = parseBase(s, radix);
}

BigInt &BigInt::operator=(const BigInt &v) {
  sign = v.sign;
  mag = v.mag;
  return *this;
}

BigInt &BigInt::operator=(long long v) {
  sign = 1;
  if (v < 0)
    sign = -1, v = -v;

  for (; v > 0; v = v / base)
    mag.push_back((int) (v % base));

  return *this;
}

BigInt BigInt::operator+(const BigInt &v) const {
  if (sign == v.sign) {
    BigInt res = v;

    for (int i = 0, carry = 0; i < (int) std::max(mag.size(), v.mag.size()) || carry; ++i) {
      if (i == (int) res.mag.size()) res.mag.push_back(0);
      res.mag[i] += carry + (i < (int) mag.size() ? mag[i] : 0);
      carry = res.mag[i] >= base;
      if (carry) res.mag[i] -= base;
    }

    return res;
  }

  return *this - (-v);
}

void BigInt::operator+=(const BigInt &v) {
  *this = *this + v;
}

BigInt BigInt::operator-(const BigInt &v) const {
  if (sign == v.sign) {
    if (abs() >= v.abs()) {
      BigInt res = *this;

      for (int i = 0, carry = 0; i < (int) v.mag.size() || carry; ++i) {
        res.mag[i] -= carry + (i < (int) v.mag.size() ? v.mag[i] : 0);
        carry = res.mag[i] < 0;
        if (carry) res.mag[i] += base;
      }

      res.trim();
      return res;
    }
    return -(v - *this);
  }

  return *this + (-v);
}

void BigInt::operator-=(const BigInt &v) {
  *this = *this - v;
}

BigInt BigInt::operator*(const BigInt &v) const {
  if (mag.size() < 30)
    return mulSimple(*this, v);

  std::vector<int> a6 = convertBase(this->mag, baseDigits, 6);
  std::vector<int> b6 = convertBase(v.mag, baseDigits, 6);
  std::vector<long long> a(a6.begin(), a6.end());
  std::vector<long long> b(b6.begin(), b6.end());

  while (a.size() < b.size())
    a.push_back(0);
  while (b.size() < a.size())
    b.push_back(0);
  while (a.size() & (a.size() - 1))
    a.push_back(0), b.push_back(0);

  std::vector<long long> c = karatsubaMultiply(a, b);
  BigInt res;
  res.sign = sign * v.sign;

  for (int i = 0, carry = 0; i < (int) c.size(); i++) {
    long long cur = c[i] + carry;
    res.mag.push_back((int) (cur % 1000000));
    carry = (int) (cur / 1000000);
  }

  res.mag = convertBase(res.mag, 6, baseDigits);
  res.trim();
  return res;
}

void BigInt::operator*=(const BigInt &v) {
  *this = *this * v;
}

BigInt BigInt::operator*(int v) const {
  BigInt res = *this;
  res *= v;
  return res;
}

void BigInt::operator*=(int v) {
  if (v < 0)
    sign = -sign, v = -v;

  for (int i = 0, carry = 0; i < (int) mag.size() || carry; ++i) {
    if (i == (int) mag.size())
      mag.push_back(0);
    long long cur = mag[i] * (long long) v + carry;
    carry = (int) (cur / base);
    mag[i] = (int) (cur % base);
  }

  trim();
}

BigInt BigInt::operator>>(int n) const {
  return *this / fastPow(2, n);
}

BigInt BigInt::operator<<(int n) const {
  return *this * fastPow(2, n);
}

BigInt BigInt::operator/(const BigInt &v) const {
  return divmod(*this, v).first;
}

void BigInt::operator/=(const BigInt &v) {
  *this = *this / v;
}

BigInt BigInt::operator/(int v) const {
  BigInt res = *this;
  res /= v;
  return res;
}

void BigInt::operator/=(int v) {
  if (v < 0)
    sign = -sign, v = -v;

  for (int i = (int) mag.size() - 1, rem = 0; i >= 0; --i) {
    long long cur = mag[i] + rem * (long long) base;
    mag[i] = (int) (cur / v);
    rem = (int) (cur % v);
  }

  trim();
}

BigInt BigInt::operator%(const BigInt &v) const {
  return divmod(*this, v).second;
}

int BigInt::operator%(int v) const {
  if (v < 0)
    v = -v;

  int m = 0;
  for (int i = (int) mag.size() - 1; i >= 0; --i)
    m = (int) ((mag[i] + m * (long long) base) % v);

  return m * sign;
}

BigInt BigInt::operator-() const {
  BigInt res = *this;
  res.sign = -sign;
  return res;
}

bool BigInt::operator<(const BigInt &v) const {
  if (sign != v.sign)
    return sign < v.sign;

  if (mag.size() != v.mag.size())
    return mag.size() * sign < v.mag.size() * v.sign;

  for (int i = (int) mag.size() - 1; i >= 0; i--) {
    if (mag[i] != v.mag[i])
      return mag[i] * sign < v.mag[i] * sign;
  }

  return false;
}

bool BigInt::operator>(const BigInt &v) const {
  return v < *this;
}

bool BigInt::operator<=(const BigInt &v) const {
  return !(v < *this);
}

bool BigInt::operator>=(const BigInt &v) const {
  return !(*this < v);
}

bool BigInt::operator==(const BigInt &v) const {
  return !(*this < v) && !(v < *this);
}

bool BigInt::operator!=(const BigInt &v) const {
  return *this < v || v < *this;
}

std::istream &operator>>(std::istream &stream, BigInt &v) {
  std::string s;
  stream >> s;
  v.read(s);
  return stream;
}

std::ostream &operator<<(std::ostream &stream, const BigInt &v) {
  stream << v.toString();
  return stream;
}

BigInt BigInt::abs() const {
  BigInt res = *this;
  res.sign *= res.sign;
  return res;
}

bool BigInt::isZero() const {
  return mag.empty() || (mag.size() == 1 && !mag[0]);
}

long long BigInt::longValue() const {
  long long res = 0;
  for (int i = (int) mag.size() - 1; i >= 0; i--)
    res = res * base + mag[i];

  return res * sign;
}

std::string BigInt::toString(int radix) const {
  if (radix != 10)
    return toStringBase(*this, radix);

  std::ostringstream stream;

  if (sign == -1)
    stream << '-';

  stream << (mag.empty() ? 0 : mag.back());
  for (int i = (int) mag.size() - 2; i >= 0; --i)
    stream << std::setw(baseDigits) << std::setfill('0') << mag[i];

  return stream.str();
}

int BigInt::len() const {
  int sz = (int) mag.size();
  int len = (int) log10(mag[sz - 1]) + 1;
  if (sz > 1) len += baseDigits * (sz - 1);

  return len;
}

void BigInt::trim() {
  while (!mag.empty() && !mag.back())
    mag.pop_back();
  if (mag.empty())
    sign = 1;
}

void BigInt::read(const std::string &s) {
  sign = 1;
  mag.clear();
  int pos = 0;

  while (pos < (int) s.size() && (s[pos] == '-' || s[pos] == '+')) {
    if (s[pos] == '-')
      sign = -sign;
    ++pos;
  }

  for (int i = (int) s.size() - 1; i >= pos; i -= baseDigits) {
    int x = 0;
    for (int j = std::max(pos, i - baseDigits + 1); j <= i; j++)
      x = x * 10 + s[j] - '0';
    mag.push_back(x);
  }

  trim();
}

std::pair<BigInt, BigInt> BigInt::divmod(const BigInt &a1, const BigInt &b1) {
  int norm = base / (b1.mag.back() + 1);
  BigInt a = a1.abs() * norm;
  BigInt b = b1.abs() * norm;
  BigInt q, r;
  q.mag.resize(a.mag.size());

  for (int i = (int) a.mag.size() - 1; i >= 0; i--) {
    r *= base;
    r += a.mag[i];
    int s1 = r.mag.size() <= b.mag.size() ? 0 : r.mag[b.mag.size()];
    int s2 = r.mag.size() <= b.mag.size() - 1 ? 0 : r.mag[b.mag.size() - 1];
    int d = (int) (((long long) base * s1 + s2) / b.mag.back());
    r -= b * d;
    while (r < 0)
      r += b, --d;
    q.mag[i] = d;
  }

  q.sign = a1.sign * b1.sign;
  r.sign = a1.sign;
  q.trim();
  r.trim();

  return std::make_pair(q, r / norm);
}

std::vector<int> BigInt::convertBase(const std::vector<int> &a, int oldDigits, int newDigits) {
  std::vector<int> res;
  long long cur = 0;
  int cur_digits = 0;
  std::vector<long long> p((unsigned long) (std::max(oldDigits, newDigits) + 1));

  p[0] = 1;
  for (int i = 1; i < (int) p.size(); i++)
    p[i] = p[i - 1] * 10;

  for (int i : a) {
    cur += i * p[cur_digits];
    cur_digits += oldDigits;
    while (cur_digits >= newDigits) {
      res.push_back(int(cur % p[newDigits]));
      cur /= p[newDigits];
      cur_digits -= newDigits;
    }
  }

  res.push_back((int) cur);
  while (!res.empty() && !res.back())
    res.pop_back();

  return res;
}

BigInt BigInt::mulSimple(const BigInt &a, const BigInt &b) {
  BigInt res = 0, tmp;

  for (int i = 0; i < b.mag.size(); i++) {
    tmp = a * b.mag[i];
    for (int j = i - 1; j >= 0; j--) tmp *= base;
    res += tmp;
  }

  res.trim();
  return res;
}

std::vector<long long>
BigInt::karatsubaMultiply(const std::vector<long long> &a, const std::vector<long long> &b) {
  int n = (int) a.size();
  std::vector<long long> res(a.size() * 2);
  if (n <= 32) {
    for (int i = 0; i < n; i++)
      for (int j = 0; j < n; j++)
        res[i + j] += a[i] * b[j];
    return res;
  }

  int k = n >> 1;
  std::vector<long long> a1(a.begin(), a.begin() + k);
  std::vector<long long> a2(a.begin() + k, a.end());
  std::vector<long long> b1(b.begin(), b.begin() + k);
  std::vector<long long> b2(b.begin() + k, b.end());

  std::vector<long long> a1b1 = karatsubaMultiply(a1, b1);
  std::vector<long long> a2b2 = karatsubaMultiply(a2, b2);

  for (int i = 0; i < k; i++)
    a2[i] += a1[i];
  for (int i = 0; i < k; i++)
    b2[i] += b1[i];

  std::vector<long long> r = karatsubaMultiply(a2, b2);

  for (int i = 0; i < (int) a1b1.size(); i++)
    r[i] -= a1b1[i];
  for (int i = 0; i < (int) a2b2.size(); i++)
    r[i] -= a2b2[i];
  for (int i = 0; i < (int) r.size(); i++)
    res[i + k] += r[i];
  for (int i = 0; i < (int) a1b1.size(); i++)
    res[i] += a1b1[i];
  for (int i = 0; i < (int) a2b2.size(); i++)
    res[i + n] += a2b2[i];

  return res;
}

BigInt BigInt::parseBase(const std::string &val, int radix) {
  int len = (int) val.size();
  BigInt base = 1;
  BigInt res = 0;

  for (int i = len - 1; i >= 0; i--) {
    if (val[i] >= '0' && val[i] <= '9') {
      res += base * (val[i] - '0');
      base = base * radix;
    } else {
      // a - 87 = 10, A - 55 = 10
      res += base * (val[i] - 87);
      base = base * radix;
    }
  }

  return res;
}

std::string BigInt::toStringBase(BigInt val, int radix) {
  static std::string base36 = "0123456789abcdefghijklmnopqrstuvwxyz";
  std::string result;

  if (val.sign == -1)
    result.push_back('-');

  do {
    result = base36[val % radix] + result;
    val /= radix;
  } while (val >= 1);
  return result;
}

BigInt modPow(BigInt base, BigInt exp, const BigInt &mod) {
  // Record start time
  auto start = std::chrono::high_resolution_clock::now();

  if (base == 0) return 0;
  if (exp == 0) return 1;

  BigInt result = 1;
  base = base % mod;
  while (exp > 0) {
    if (exp.mag[0] & 1)
      result = (result * base) % mod;
    exp /= 2;
    base = (base * base) % mod;
  }

  auto finish = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = finish - start;
  //std::cout << elapsed.count() << "\n";

  return result;
}

BigInt gcd(BigInt a, BigInt b) {
  BigInt c;
  while (!b.isZero()) {
    c = a % b;
    a = b;
    b = c;
  }

  return a;
}

BigInt fastPow(BigInt base, int exp) {
  BigInt result = 1;
  while (exp != 0) {
    if (exp & 1) result *= base;
    exp >>= 1;
    base *= base;
  }

  return result;
}

BigInt xrand(const BigInt &min, const BigInt &max) {
  int b = min.len();
  int a = max.len() - b + 1;
  int length;

  std::string nums0 = "123456789";
  std::string nums1 = "0123456789";
  std::string nums2 = "13579";
  std::string strval;
  BigInt res;

  do {
    strval = "";
    length = rand() % (a) + b;
    strval.push_back(nums0[rand() % 9]);
    for (int i = 1; i < length - 1; ++i)
      strval.push_back(nums1[rand() % 10]);
    strval.push_back(nums2[rand() % 5]);
    res.read(strval);
  } while (res >= max || res <= min);

  return res;
}