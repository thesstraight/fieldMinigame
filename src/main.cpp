#include <iostream>
#include <cstdint>
#include <array>
#include <algorithm>
#include <unordered_map>
#include <limits>

#include "random.h"

namespace enums
{
    enum class rarities
    {
        none,
        bronze,
        silver,
        gold,
    };
}

namespace structs
{
    struct coin
    {
        enums::rarities rarity{};
        bool picked{};
    };

    struct prize
    {
        std::uint16_t value{};
        enums::rarities rarity{};
    };
}

int main()
{
    structs::prize prize{};

    std::array<structs::coin, 15> coins{};

    std::fill(coins.begin(), coins.begin() + 5, structs::coin{ enums::rarities::bronze });
    std::fill(coins.begin() + 5, coins.begin() + 10, structs::coin{ enums::rarities::silver });
    std::fill(coins.begin() + 10, coins.end(), structs::coin{ enums::rarities::gold });

    std::shuffle(coins.begin(), coins.end(), Random::mt);

    std::uint16_t pickedCoinIndex{};

    bool gameInProgress{ true };
    do
    {
        for (const structs::coin& coin : coins)
        {
            if (coin.picked)
            {
                std::cout << static_cast<std::uint16_t>(coin.rarity) << ' ';
            }
            else
            {
                std::cout << "X ";
            }
        }

        std::cin >> pickedCoinIndex;

        if (!std::cin)
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            continue;
        }

        --pickedCoinIndex;

        if (pickedCoinIndex < 0 || pickedCoinIndex >= 15)
        {
            continue;
        }

        if (coins[pickedCoinIndex].picked)
        {
            continue;
        }

        coins[pickedCoinIndex].picked = true;

        std::unordered_map<enums::rarities, std::uint16_t> coinsAndCount{};
        for (const structs::coin& coin : coins)
        {
            if (coin.picked)
            {
                if (++coinsAndCount[coin.rarity] >= 3)
                {
                    prize.rarity = coin.rarity;

                    gameInProgress = false;
                }
            }
        }

    } while (gameInProgress);

    std::cout << "\n-------------------------------------\n";

    for (const structs::coin& coin : coins)
    {
        std::cout << static_cast<std::uint16_t>(coin.rarity) << ' ';
    }

    {
        const std::unordered_map<enums::rarities, std::uint16_t> prizeMap
        {
            { enums::rarities::bronze, 50 },
            { enums::rarities::silver, 100 },
            { enums::rarities::gold, 2500 }
        };

        prize.value = prizeMap.at(prize.rarity);
    }

    std::cout << "\nYOU WON " << prize.value << "$\n";

    return 0;
}