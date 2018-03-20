#include <iostream>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <set>
#include <assert.h>

constexpr int TARGET_SUM = 210;

using available_t = std::set<int>; // { 3, 5, 9 }
using pair_t = int; // small number x 100 + large number - for one specific pair only
using combination_t = std::set<pair_t>; // for one combination (might not be complete)
using combinations_t = std::vector<combination_t>;
// [0] - decide 1 pair, [1] - decide 2 pairs, ...
using all_combinations_t = combinations_t[3];

void createCombinations(
	const available_t&   available1,
	const combination_t& prev_combination,
	all_combinations_t&  all_combinations,
	int                  nthPair)
{
	if (available1.empty()) { return; }

	combination_t curr_pairs;

	for (auto i1 : available1) { // 1st number
		auto available2 = available1;
		available2.erase(i1);

		for (auto i2 : available2) { // 2nd number
			auto new_available = available2;
			new_available.erase(i2);
			int new_pair = (i1 < i2) ? i1 * 100 + i2 : i2 * 100 + i1;
			// this is new_pair is not new!
			if (curr_pairs.find(new_pair) != curr_pairs.cend()) { continue; }
			curr_pairs.insert(new_pair);

			if (prev_combination.find(new_pair) != prev_combination.cend()) {
				assert(false);
				continue;
			}

			auto combination = prev_combination;
			combination.insert(new_pair);
			int sum = 0;
			for (const auto pair : combination) {
				sum += (pair / 100) * (pair % 100);
			}
			if (sum > TARGET_SUM) { continue; }

			bool same_combination = false;
			auto& curr_combinations = all_combinations[nthPair - 1];
			for (const auto& curr_combination : curr_combinations) {
				if (combination == curr_combination) {
					same_combination = true;
					break;
				}
			}
			if (same_combination) { continue; }

			if (combination.size() != nthPair) {
				assert(false);
				break;
			}
			curr_combinations.push_back(combination);
			createCombinations(new_available, combination, all_combinations, nthPair + 1);
		}
	}
}

int main()
{
	// all elements should be > 0
	auto available = available_t{ 13, 11, 9, 7, 5, 4 };
	all_combinations_t allCombinations;
	assert(available.size() / 2 <= _countof(allCombinations));

	createCombinations(available, std::set<pair_t>{}, allCombinations, 1);

	const auto& combinations = allCombinations[available.size() / 2 - 1];

	for (const auto& combination : combinations) {
		int sum = 0;
		for (const auto pair : combination) {
			sum += (pair / 100) * (pair % 100);
		}
		if (sum != TARGET_SUM) { continue; }

		bool first = true;
		for (const auto pair : combination) {
			if (!first) {
				std::cout << " + ";
			}
			first = false;
			std::cout << (pair / 100) << " x " << std::setw(2) << (pair % 100);
		}

		std::cout << std::endl;
	}

	std::cout << "all done" << std::endl;
	return 0;
}
