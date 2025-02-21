#include "Decision.hpp"

Decision::Decision(std::string title, std::string desc, std::vector<EffectType> accept, std::vector<EffectType> decline)
    : title(title), description(desc), acceptEffect(accept), declineEffect(decline) {}

std::unordered_map<DecisionType, Decision> Decision::decisionMap = {
    {DecisionType::DOWNSIZING, Decision(
        "Strategic Downsizing",
        "Our analysts suggest removing inefficient structures to optimize costs. Should we proceed?",
        {EffectType::REMOVE_BUILDING, EffectType::BAD_LUCK},
        {EffectType::NO_EFFECT}
    )},

    {DecisionType::REINVEST_PROFITS, Decision(
        "Reinvest or Hold Reserves?",
        "The board is considering reinvesting profits to accelerate growth. Should we proceed?",
        {EffectType::INCREASE_MONEY_RATE, EffectType::GOOD_LUCK},
        {EffectType::LOSE_MONEY}
    )},

    {DecisionType::SAFETY_INSPECTION, Decision(
        "Government Safety Inspection",
        "A safety audit has been requested. Cooperate and risk fines, or delay?",
        {EffectType::LOSE_MONEY, EffectType::BAD_LUCK},
        {EffectType::BAD_LUCK, EffectType::REDUCE_MONEY_RATE}
    )},

    {DecisionType::LIQUIDATE_ASSETS, Decision(
        "Liquidity Crisis: Sell Assets?",
        "Market instability has reduced cash flow. Should we sell off assets for quick funds?",
        {EffectType::GAIN_MONEY, EffectType::REDUCE_MONEY_RATE},
        {EffectType::NO_EFFECT}
    )},

    {DecisionType::EMPLOYEE_SUGGESTION, Decision(
        "Profit Optimization Strategy",
        "A junior analyst suggests restructuring for profit efficiency. Implement or ignore?",
        {EffectType::INCREASE_MONEY_RATE, EffectType::GOOD_LUCK},
        {EffectType::NO_EFFECT}
    )},

    {DecisionType::CUT_EXECUTIVE_SALARIES, Decision(
        "Reduce Executive Salaries?",
        "Reducing leadership salaries could reinvest funds but may impact morale. Proceed?",
        {EffectType::GAIN_MONEY, EffectType::BAD_LUCK},
        {EffectType::NO_EFFECT}
    )},

    {DecisionType::INVESTOR_CONFIDENCE, Decision(
        "Investor Confidence Crisis",
        "Injecting emergency funds could stabilize investor trust. Should we intervene?",
        {EffectType::LOSE_MONEY, EffectType::REDUCE_MONEY_RATE},
        {EffectType::REDUCE_MONEY_RATE, EffectType::BAD_LUCK}
    )},

    {DecisionType::SUSTAINABLE_GROWTH, Decision(
        "Sustainable Growth Initiative",
        "PR advises we slow down expansion for a better corporate image. Implement?",
        {EffectType::INCREASE_MONEY_RATE, EffectType::GOOD_LUCK},
        {EffectType::NO_EFFECT}
    )},

    {DecisionType::STRUCTURAL_INSTABILITY, Decision(
        "Structural Instability Detected",
        "Reports indicate minor weaknesses in older structures. Should we fix them now?",
        {EffectType::REMOVE_BUILDING, EffectType::BAD_LUCK},
        {EffectType::INFLATION}
    )},

    {DecisionType::FIRST_ORDER, Decision(
        "Executive Directive: Vertical Growth Mandate",
        "The board has issued a mandatory directive: all new construction must reach vertically form in a set amount of building. Failure to comply may result in inefficiencies. Do you acknowledge?",
        {EffectType::NO_EFFECT},
        {EffectType::BAD_LUCK, EffectType::REDUCE_MONEY_RATE}
    )}
};
