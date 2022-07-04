#ifndef RECIPE_H
#define RECIPE_H

#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <map>
#include <stdexcept>
#include <iterator>
#include <cmath>

#include "utility.h"

class Recipe {
 private:
  std::string name_;
  int servings_ = 0;
  std::vector<std::string> ingredientslist;
  std::vector<std::string> ingredientnameslist;
  std::map<std::string, std::pair<std::string, std::string>> ingredientsmap;
  std::string instructions;
  friend class RecipeApp;

 public:
  // constructor
  Recipe(const std::string & n, const int & s){
      name_ = n;
      servings_ = s;
  }

  // members
  void AddIngredient(const std::string & i){
    std::string value, unit, name, temp;
    splitIngredient(i, value, unit, name);
    std::pair<std::string, std::string> amount(value, unit);
    ingredientsmap.insert({name, amount});
    temp = value + " " + unit + " " + name;
    ingredientslist.push_back(temp);
    ingredientnameslist.push_back(name);
  }

  void SetInstructions(const std::string & instruct){
    std::string s, temp = instruct;
    // remove empty lines algorithm referenced from
    // https://stackoverflow.com/questions/22395333/removing-empty-lines-from-a-string-c
    temp.erase(std::unique(temp.begin(), temp.end(),
      [] (char a, char b) {return a == '\n' && b == '\n';}),
      temp.end());
    std::istringstream iss(temp);
    std::ostringstream oss;
    while(getline(iss, s)){
      s = trim(s);
      oss << s << "\n";
    }
    instructions = oss.str();
  }

  std::string IngredientInOneServing(const std::string & ingrename){
    std::string sval;
    std::map<std::string, std::pair<std::string, std::string>>::iterator i = ingredientsmap.find(ingrename);
    if (i != ingredientsmap.end()){ // if ingredient is found in the recipe
      std::string value = (i->second).first, unit = (i->second).second, name = i->first;
      if(servings_ != 1){
        // get fraction value and simplify it
        std::vector<int> frac = reduceToOne(value, servings_);
        value = simplifyFraction(frac);
      }
      sval = value + " " + unit + " " + name;
    }
    else{ // throw exception if not found
      throw std::invalid_argument("");
    }
    return sval;
  }

  void ChangeServings(const int & newServing){
    for(int x = 0; x < ingredientnameslist.size(); x++){
      std::map<std::string, std::pair<std::string, std::string>>::iterator i = ingredientsmap.find(ingredientnameslist.at(x));
      std::string value = (i->second).first, unit = (i->second).second, name = i->first;
      std::vector<int> frac1 = reduceToOne(value, servings_); // scales it to 1
      value = simplifyFraction(frac1); 
      std::vector<int> frac2 = reduceToServings(value, newServing); // scale it to newServing
      value = simplifyFraction(frac2); // modifies the new fraction value
      (i->second).first = value;
      ingredientslist.at(x) = value + " " + unit + " " + name;
    }
    servings_ = newServing; // changes object attribute
  }

  friend std::ostream &operator<<(std::ostream & out, Recipe const & r){
    out << "Recipe for: " << r.name_ << '\n' << "Serves " << r.servings_ 
          << '\n' << "Ingredients:\n";
    for(auto i : r.ingredientslist){ out << i << '\n'; }
    out << '\n' << "Instructions:" << '\n' << r.instructions << '\n';
    return out;
  }

};

#endif
