#ifndef DEALS_H
#define DEALS_H

// EECS 281 LAB 10: DYNAMIC PROGRAMMING
// Identifier: D7E20F91029D0CB08715A2C54A782E0E8DF829BF
// Authors: Manuel Rodriguez - mrrodrig, Calvin Tran - cktran

// Your favorite restaurant has a customer loyalty program. It works like this: whenever you buy a
// meal, you can add one holepunch to your punchcard. Once you have 5 punches, you can turn in the
// card for a free meal (and a new, unmarked punchcard).

// For example, if your meals cost [3, 3, 3, 3, 3, 3, 3, 120] then you should earn hole punches
// from the first 5 meals ($15), pay normally for the next two, and then turn in the punchcard
// so that the $120 meal is free! The total cost would be $19 (or with coupons [see below] $17).

// However, you ALSO have a lot of coupons to this restaurant. In fact, you have enough coupons
// that you can apply one to any and every meal! If you apply a coupon, you get a 25% discount
// on that meal. HOWEVER, you don't get to add a holepunch to your card if you use a coupon!

// For example, if your meals cost [2, 2, 2, 2, 1000, 100], then if you use the first 5 meals to
// earn holepunches, you'll need to spend $1008. Then you'll get $100 free. It would be much better
// to just apply the 25% discount to each item, so that you pay a total of $829.

// There are, however, many cases where it makes sense to use a mixture of punchcard discounts and
// discounting coupons. This is where your program comes in!

// You will be given a list of meal prices. For the given meal prices, find the LEAST POSSIBLE
// cost needed to pay for all the meals using the holepunch loyalty program and coupons.

// Notes/Clarifications/Hints:
// * use the "discounted" function below to compute discount
//    (so that you round the same way as the instructor solution).
// * you should always EITHER use a coupon, or apply the punchcard
// * you have an unlimited number of coupons
// * your program should be able to run in linear time.
// * greedy solutions will not work
// * use dynamic programming

#include <iostream>
#include <vector>
#include <functional>   
#include <limits>

// TODO: import whatever you want

// This is a type synonym.
using cost = long long;

// How much you pay for a discounted (25% off) meal.
cost discounted(cost full_price) {
    return full_price * 3 / 4;
}

// TODO: add any helpers you want here.
/*
            _______________Days->__________________________
            |10 |10 |10  |10 |10  |100|10  |1000|10   |10 |       FULL PRICES
            |7.5|7.5|7.5 |7.5|7.5 |75 |7.5 |750 |7.5  |7.5|       DISCOUNTED PRICES
            |7.5|15 |22.5|30 |37.5|50 |57.5|160 |167.5|175|       BEST PRICES
            -----------------------------------------------

*/

cost min(const cost cost1, const cost cost2) { //include the count variable
    return (cost1 < cost2) ? cost1 : cost2;
}

void solve(const std::vector<cost>& prices, std::vector<std::vector<cost>>& memo, size_t stamps, size_t day)
{
    switch (stamps)
    {
    case 0:
        if (day >= 5)
        {
            memo[stamps][day] = min(memo[5][day - 1], memo[stamps][day - 1] + discounted(prices[day]));
        }
        else
            memo[stamps][day] = memo[stamps][day - 1] + discounted(prices[day]);  // prev + disc
        break;
    default:
        memo[stamps][day] = min(memo[stamps - 1][day - 1] + prices[day], memo[stamps][day - 1] + discounted(prices[day]));
        break;
    }
}
cost best_price(const std::vector<cost>& prices) {
    // TODO: put your code here
    // NOTE: if you use a bottom-up approach, initialize your table with
    // std::numeric_limits<cost>::max()/2 ... you MUST divide by 2!
    const size_t rows = 6;
    //memo[stamps][day]
    std::vector<std::vector<cost>> memo(rows, std::vector<cost>(prices.size(), std::numeric_limits<cost>::max() / 2));
    memo[0][0] = discounted(prices[0]);
    memo[1][0] = prices[0];
    for (size_t day = 1; day < prices.size(); ++day)
    {
        for (size_t stamp = 0; stamp < rows; ++stamp)
        {
            if (day == prices.size() - 1 && stamp == 5)
                memo[0][0] = memo[0][0];
            solve(prices, memo, stamp, day);
        }
    }

    return memo[0][prices.size() - 1];
}



#endif