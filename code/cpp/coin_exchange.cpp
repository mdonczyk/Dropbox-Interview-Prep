#include <iostream>
#include <vector>
#include <deque>


void print_vec(const std::vector<int>& vec) {
    for (const auto& e : vec) {
        std::cout<< e << " ";
    }
    std::cout<<std::endl;
}

int coinChange(const std::vector<int>& coins, int amount) {
    // bottom up approach with memoization
    std::vector<int> dp(amount+1, amount+1);
    dp[0] = 0;
    for (int a=1; a<amount+1; a++) {
        for (const auto& coin : coins) {
            if (a-coin >= 0) {
                dp[a] = std::min(dp[a], 1 + dp[a - coin]);
            }
        }
    }
    return (dp[amount] != amount+1)? dp[amount] : -1; 
}

int main () {
    std::vector<int> numbers = {1, 2, 5};
    int target = 11;
     std::cout<<"Numbers: ";
    print_vec(numbers);
    std::cout<<"Target: " << target << "\n";
    std::cout<<"Answer: " << coinChange(numbers, target) << std::endl;
    return 0;
}