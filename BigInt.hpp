#ifndef DIGITALSIGNATUREDES_BIGINT_HPP
#define DIGITALSIGNATUREDES_BIGINT_HPP

#include <vector>
#include <string>

class BigInt {
 public:

  BigInt();
  BigInt(long long v);
  BigInt(const std::string &s, int radix = 10);

  BigInt &operator=(const BigInt &v);
  BigInt &operator=(long long v);

  BigInt operator+(const BigInt &v) const;
  void operator+=(const BigInt &v);

  BigInt operator-(const BigInt &v) const;
  void operator-=(const BigInt &v);

  BigInt operator*(const BigInt &v) const;
  void operator*=(const BigInt &v);

  BigInt operator*(int v) const;
  void operator*=(int v);

  BigInt operator>>(int n) const;
  BigInt operator<<(int n) const;

  BigInt operator/(const BigInt &v) const;
  void operator/=(const BigInt &v);

  BigInt operator/(int v) const;
  void operator/=(int v);

  BigInt operator%(const BigInt &v) const;
  int operator%(int v) const;

  BigInt operator-() const;

  bool operator<(const BigInt &v) const;
  bool operator>(const BigInt &v) const;
  bool operator<=(const BigInt &v) const;
  bool operator>=(const BigInt &v) const;
  bool operator==(const BigInt &v) const;
  bool operator!=(const BigInt &v) const;

  friend std::istream &operator>>(std::istream &stream, BigInt &v);
  friend std::ostream &operator<<(std::ostream &stream, const BigInt &v);

  BigInt abs() const;
  bool isZero() const;
  long long longValue() const;
  std::string toString(int radix = 10) const;
  int len() const;

  friend BigInt modPow(BigInt base, BigInt exp, const BigInt &mod);
  friend BigInt gcd(BigInt a, BigInt b);
  friend BigInt xrand(const BigInt &min, const BigInt &max);
  friend BigInt fastPow(BigInt base, int exp);

 private:
  static const int base = 1000000000;
  static const int baseDigits = 9;

  std::vector<int> mag;
  int sign;

  void trim();
  void read(const std::string &s);
  static std::pair<BigInt, BigInt> divmod(const BigInt &a1, const BigInt &b1);
  static std::vector<int> convertBase(const std::vector<int> &a, int oldDigits, int newDigits);
  static std::vector<long long> karatsubaMultiply(const std::vector<long long> &a,
                                                  const std::vector<long long> &b);
  static BigInt mulSimple(const BigInt &a, const BigInt &b);
  static BigInt parseBase(const std::string &val, int radix);
  static std::string toStringBase(BigInt val, int radix);
};

BigInt modPow(BigInt base, BigInt exp, const BigInt &mod);
BigInt gcd(BigInt a, BigInt b);
BigInt lcm(const BigInt &a, const BigInt &b);
BigInt xrand(const BigInt &min, const BigInt &max);
BigInt fastPow(BigInt base, int exp);

#endif //DIGITALSIGNATUREDES_BIGINT_HPP