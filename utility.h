#ifndef UTILITY_H
#define UTILITY_H

#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <map>
#include <stdexcept>
#include <iterator>
#include <cmath>
#include <numeric>


// referenced trim function from:
// https://www.techiedelight.com/trim-string-cpp-remove-leading-trailing-spaces/#:~:text=We%20can%20use%20a%20combination,functions%20to%20trim%20the%20string.
std::string trim(const std::string &s) {
  std::string temp, ans, WHITESPACE = " \n\r\t\f\v";;
  size_t start = s.find_first_not_of(WHITESPACE);
  temp = (start == std::string::npos) ? "" : s.substr(start);
  size_t end = temp.find_last_not_of(WHITESPACE);
  ans = (end == std::string::npos) ? "" : temp.substr(0, end + 1);
  return ans;
}

// makes a fraction out of a string
void makeFraction(const std::string & value, int & num, int & den, int & whole){
  if(value.find("/") != std::string::npos && value.find("-") != std::string::npos){
    whole = std::stoi(value.substr(0, value.find("-")));
    num = std::stoi(value.substr(value.find("-")+1, value.find("/")));
    den = std::stoi(value.substr(value.find("/")+1));
    num = den*whole + num;  
  }           
  else if(value.find("/") != std::string::npos) {
    num = std::stoi(value.substr(0, value.find("/")));
    den = std::stoi(value.substr(value.find("/")+1));
  }
  else {
    num = std::stoi(value);
    den = 1;
  }
}

// returns a fraction stored in a vector according to 1 serving
std::vector<int> reduceToOne(const std::string & value, const int & s) {
  std::vector<int> ans;
  int num = 0, den = 0, whole = 0;
  makeFraction(value, num, den, whole);
  den = den*s;
  ans.push_back(num);
  ans.push_back(den);
  return ans;
}

// returns a fraction stored in a vector according to n servings
std::vector<int> reduceToServings(const std::string & value, const int & s) {
  std::vector<int> ans;
  int num = 0, den = 0, whole = 0;
  makeFraction(value, num, den, whole);
  num = num*s;
  ans.push_back(num);
  ans.push_back(den);
  return ans;
}

// takes a fraction vector and returns a string representation of the fraction
std::string simplifyFraction(const std::vector<int> & fraction){
  int num = fraction[0];
  int den = fraction[1];
  int whole = 0;
  std::string value;
  if(num == den) {value = std::to_string(1);}
  else if(num > den){
    int gcd = std::__gcd(num, den);
    num = num / gcd;
    den = den / gcd;
    whole = std::floor(num / den);
    num = num % den;
    if(num == 0)
      value = std::to_string(whole);
    else
      value = std::to_string(whole) + "-" + std::to_string(num) + "/" + std::to_string(den);
  }
  else if(num < den) {
    int gcd = std::__gcd(num, den);
    num = num / gcd;
    den = den / gcd;
    value = std::to_string(num) + "/" + std::to_string(den);
  }
  return value;
}

// takes a value and simplifies it if needed
void simplify(std::string & value) {
  std::vector<int> frac;
  int num = 0, den = 0, whole = 0;
  makeFraction(value, num, den, whole);
  if(num != 0 && den != 0){
    frac.push_back(num);
    frac.push_back(den);
    value = simplifyFraction(frac);
  } 
}

// takes a value and turns it into a decimal
double toDecimal(const std::string & value){
  double ans = 0.0;
  int num = 0, den = 0, whole = 0;
  makeFraction(value, num, den, whole);
  ans = 1.0 * num/den;
  return ans;
}

// takes an ingredient string and splits up the ingredient into its respective strings
void splitIngredient(const std::string & i, std::string & value, std::string & unit, std::string & name){
  std::stringstream ss(i);
  ss >> value >> unit;
  getline(ss, name);
  name = trim(name);
  simplify(value);
}


#endif
