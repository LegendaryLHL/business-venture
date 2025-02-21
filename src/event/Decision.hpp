#pragma once

#include <string>
#include <vector>
#include <unordered_map>

enum class EffectType {
    REMOVE_BUILDING,
    RANDOM_BUILDING_PLACEMENT,
    REDUCE_MONEY_RATE,
    INCREASE_MONEY_RATE,
    LOSE_MONEY,
    GAIN_MONEY,
    INFLATION,
    DEFLATION,
    BAD_LUCK,
    GOOD_LUCK,
    NO_EFFECT,
};


enum class DecisionType {
    DOWNSIZING,
    REINVEST_PROFITS,
    SAFETY_INSPECTION,
    MARKET_EXPANSION,
    LIQUIDATE_ASSETS,
    EMPLOYEE_SUGGESTION,
    CUT_EXECUTIVE_SALARIES,
    INVESTOR_CONFIDENCE,
    SUSTAINABLE_GROWTH,
    STRUCTURAL_INSTABILITY,
    FIRST_ORDER,
};

class Decision {
public:
    std::string title;
    std::string description;
    std::vector<EffectType> acceptEffect;
    std::vector<EffectType> declineEffect;

    Decision(std::string title, std::string desc, std::vector<EffectType> accept, std::vector<EffectType> decline);
    Decision() = default;

    static std::unordered_map<DecisionType, Decision> decisionMap;
};
