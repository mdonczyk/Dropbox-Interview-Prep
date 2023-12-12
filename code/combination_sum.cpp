/*
Dropbox

Bottles of Soda / Combination Sum / Coin Change

This problem is given in the context of soda bottles or coins of different values.

This problem is usually for college interns.

Given a collection of soda bottles of different sizes, find all unique combinations of bottles that 
result in a given amount of soda.
*/
#include <iostream>
#include <vector>

void print_vec(const std::vector<int>& vec) {
    for (const auto& e : vec) {
        std::cout<< e << " ";
    }
    std::cout<<std::endl;
}

void dfs(const std::vector<int>& candidates, std::vector<int>& current_combo, std::vector<std::vector<int>>& result,  int currIndex, int currSum, int target) {
        print_vec(current_combo);
        if (currIndex >= candidates.size() || currSum > target) return;
        if (currSum == target) {
            result.push_back(current_combo);
            return;
        }
        // first we want to explore the whole branch all the way and only then get back up and try update the index
        current_combo.push_back(candidates[currIndex]);
        dfs(candidates, current_combo, result, currIndex, currSum + candidates[currIndex], target);

        current_combo.pop_back();
        dfs(candidates, current_combo, result, currIndex + 1, currSum, target);
}

std::vector<std::vector<int>> combinationSum(const std::vector<int>& candidates, int target) {
    std::vector<std::vector<int>> result;
    std::vector<int> currCombo;
    dfs(candidates, currCombo, result, 0, 0, target);
    return result;
}

int main () {
    std::vector<int> numbers = {2, 3, 6, 7};
    int target = 7;
     std::cout<<"Numbers: ";
    print_vec(numbers);
    std::cout<<"Target: " << target << "\n\n";

    std::cout<<"DFS - first it goes all the way of one branch and after it gets all the options\n"
    <<"it goes back to the most recent one and tries again\n";
    std::vector<std::vector<int>> answer = combinationSum(numbers, target);
   
    std::cout<<"Answer:\n";
    for (const auto& vec : answer) {
        for (const auto& e : vec) {
            std::cout<< e << " ";
        }
        std::cout<<std::endl;
    }
    return 0;
}

#if 0
class CombinationSum:

    def __init__(self, bottles, target):
        self.bottles = bottles
        self.target = target
        self.result = []
        self.bottles.sort()
        self.num_bottles = len(bottles)

    def backtrack(self, remainder, combo, start_idx):
        if remainder == 0: //You've found a combination!
            self.results.append(combo[:]) //Add a deep copy of the combo to the results
            return
        elif remainder < 0:
            return
        for bottle_idx in range(start_idx, self.num_bottles):
            bottle_size = self.bottles[bottle_idx]
            if bottle_size > remainder:
                break //this bottle is too big, and all the rest of the bottles are also too big
            combo.append(bottle_size)
            self.backtrack(remainder - bottle_size, combo, bottle_idx)
            combo.pop()

    def process(self):
        self.backtrack(self.target, [], 0)
        return self.result
#endif