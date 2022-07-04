#ifndef APP_H
#define APP_H

#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <map>
#include <stdexcept>
#include <iterator>
#include <cmath>

#include "utility.h"
#include "recipe.h"

class RecipeApp {
 private:
  std::vector<class Recipe> recipeVector;
  std::vector<std::string> pantry;
  std::map<std::string, std::pair<std::string, std::string>> pantrymap;

 public:
  // constructor
  RecipeApp() = default;

  // members
  void AddRecipe(const class Recipe & r){
    recipeVector.push_back(r);
    std::sort(recipeVector.begin(), recipeVector.end(), [] (Recipe r1, Recipe r2) {
      return r1.name_ < r2.name_;
    });
  }

  void AddIngredientToPantry(const std::string & i){
    std::string value, unit, name, temp;
    splitIngredient(i, value, unit, name);
    std::pair<std::string, std::string> amount(value, unit);
    pantrymap.insert({name, amount});
    temp = value + " " + unit + " " + name;
    pantry.push_back(temp);
  }
  
  Recipe UseUpIngredient(const std::string & ingredient){
    std::string value, unit, name;
    splitIngredient(ingredient, value, unit, name);
    int notfound = 0, count = 0;
    for(Recipe r : recipeVector){ 
      // looks through each recipe and checks if it uses requested ingredient
      if(std::find(r.ingredientnameslist.begin(), r.ingredientnameslist.end(), name) != r.ingredientnameslist.end()){
        Recipe ans = r;
        auto iter = r.ingredientsmap.find(name); // finds the amount of requested ingredient
        std::vector<int> v = reduceToOne(iter->second.first, r.servings_); // finds how much is in one serving
        for(int serv = 1; serv < 1000; ++serv){ // finds how many servings can be made
          if((1.0 * v[0] * serv) / v[1] <= toDecimal(value)) {count++;}
        }
        ans.ChangeServings(count);
        return ans;
      }
      // if it doesn't then check next recipe
      else {notfound++;}
    }
    // if no recipe used the ingredient then throw exception
    if(notfound == recipeVector.size()) {throw std::invalid_argument("");}
  }
  
  friend std::ostream &operator<<(std::ostream & out, const RecipeApp & ra){
    out << "Recipes in the app (ordered by name):\n";
    for(class Recipe r : ra.recipeVector){
      out << r;
    } // prints the recipe then ingredients in the pantry in order
    out << "Ingredients in pantry (ordered by name):\n";
    for(auto [name, amount] : ra.pantrymap){
      out << amount.first << " " << amount.second << " " << name << "\n";
    }
    return out;
  }
};

#endif
