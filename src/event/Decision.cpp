#include "Decision.hpp"

Decision::Decision(std::string title, std::string desc, std::vector<EffectType> accept, std::vector<EffectType> decline)
    : title(title), description(desc), acceptEffect(accept), declineEffect(decline) {}

std::unordered_map<DecisionType, Decision> Decision::decisionMap = {
    {DecisionType::DOWNSIZING, Decision(
        "Strategic Downsizing",
        "Efficiency is everything! By removing 'redundant' structures, we can free up resources and cut costs. Sure, we might lose some operational capacity... but who needs that anyway?",
        {EffectType::REMOVE_BUILDING, EffectType::GAIN_MONEY},
        {EffectType::NO_EFFECT}
    )},

    {DecisionType::REINVEST_PROFITS, Decision(
        "Reinvest or Hoard Cash?",
        "The board is debating whether to reinvest excess profits into growth or hoard them for a rainy day. Investing will surely boost future earnings... assuming, of course, nothing terrible happens.",
        {EffectType::INCREASE_MONEY_RATE},
        {EffectType::DEFLATION, EffectType::LOSE_MONEY}
    )},

    {DecisionType::SAFETY_INSPECTION, Decision(
        "Government Safety Inspection",
        "Surprise! Regulators have decided to check if our buildings are 'structurally sound.' We could cooperate and risk fines or delay the inspection and hope for the best. What's the worst that could happen?",
        {EffectType::LOSE_MONEY},
        {EffectType::REDUCE_MONEY_RATE}
    )},

    {DecisionType::MARKET_EXPANSION, Decision(
        "Expand into New Markets?",
        "An exciting new market opportunity has emerged! We can expand our reach and dominate—provided we ignore minor concerns like cultural differences, logistics, and pesky legal barriers. Easy money, right?",
        {EffectType::INCREASE_MONEY_RATE, EffectType::LOSE_MONEY},
        {EffectType::NO_EFFECT}
    )},

    {DecisionType::LIQUIDATE_ASSETS, Decision(
        "Sell Off Assets?",
        "Liquidity is tight, so why not sell some assets? Sure, those buildings were essential for long-term growth, but we need cash right now. It's not like we'll regret this later…",
        {EffectType::GAIN_MONEY, EffectType::REDUCE_MONEY_RATE},
        {EffectType::NO_EFFECT}
    )},

    {DecisionType::EMPLOYEE_SUGGESTION, Decision(
        "Trust an Employee's Idea?",
        "Some ambitious intern thinks they've discovered a way to optimize profits. Should we humor them? Worst case scenario: they're wrong, and we wasted time. Best case? We take the credit.",
        {EffectType::INCREASE_MONEY_RATE},
        {EffectType::NO_EFFECT}
    )},

    {DecisionType::CUT_EXECUTIVE_SALARIES, Decision(
        "Cut Executive Salaries?",
        "Some people think our executives are 'overpaid.' Cutting their salaries could free up money for other areas... though we risk making our most powerful employees very unhappy. What could go wrong?",
        {EffectType::GAIN_MONEY},
        {EffectType::NO_EFFECT}
    )},

    {DecisionType::INVESTOR_CONFIDENCE, Decision(
        "Investor Panic!",
        "Stockholders are getting nervous. We could inject emergency funds to stabilize the market... or just hope they don't start selling. Investors are known for being rational, right?",
        {EffectType::LOSE_MONEY},
        {EffectType::REDUCE_MONEY_RATE}
    )},

    {DecisionType::SUSTAINABLE_GROWTH, Decision(
        "Go Slow for Stability?",
        "We could scale back aggressive expansion and focus on sustainability. Of course, this means we won't see immediate profits, but playing the long game is always the smart move. Right?",
        {EffectType::INCREASE_MONEY_RATE, EffectType::DEFLATION},
        {EffectType::NO_EFFECT}
    )},

    {DecisionType::STRUCTURAL_INSTABILITY, Decision(
        "Minor Structural Issues Detected",
        "Engineers have reported 'minor' weaknesses in some older structures. Fixing them will cost money, but ignoring the problem probably won't result in a catastrophic collapse. Probably.",
        {EffectType::REMOVE_BUILDING, EffectType::LOSE_MONEY},
        {EffectType::INFLATION}
    )},

    {DecisionType::AUTOMATION_UPGRADE, Decision(
        "Replace Workers with Robots?",
        "Automation is the future! Machines don't need breaks, salaries, or dignity. Let's replace employees with AI. It's not like they'll unionize and riot over this... right?",
        {EffectType::INCREASE_MONEY_RATE, EffectType::LOSE_MONEY},
        {EffectType::NO_EFFECT}
    )},

    {DecisionType::HIRING_FREEZE, Decision(
        "No More Hiring?",
        "Hiring new employees costs money, so why not just... stop? Sure, we'll overwork the current staff, but hey, grit builds character.",
        {EffectType::GAIN_MONEY},
        {EffectType::REDUCE_MONEY_RATE}
    )},

    {DecisionType::ENVIRONMENTAL_REGULATION, Decision(
        "New Environmental Laws",
        "The government wants us to reduce emissions and waste. Compliance is expensive, but imagine the PR disaster if we get caught not caring about the environment!",
        {EffectType::LOSE_MONEY, EffectType::INCREASE_MONEY_RATE},
        {EffectType::NO_EFFECT}
    )},

    {DecisionType::TAX_EVASION_SCANDAL, Decision(
        "Creative Accounting?",
        "Some 'accounting irregularities' have come to light. We could admit to everything and take the hit, or creatively reinterpret the numbers. The IRS has a great sense of humor... right?",
        {EffectType::LOSE_MONEY},
        {EffectType::REDUCE_MONEY_RATE, EffectType::INFLATION}
    )},

    {DecisionType::NEW_PRODUCT_LAUNCH, Decision(
        "Launch a Bold New Product?",
        "We have an exciting new product ready for market! Sure, we skipped testing, but consumers love surprises. What's the worst that could happen?",
        {EffectType::INCREASE_MONEY_RATE, EffectType::LOSE_MONEY},
        {EffectType::NO_EFFECT}
    )},

    {DecisionType::UNION_DEMANDS, Decision(
        "Give In to Union Demands?",
        "The workers' union wants higher wages and better conditions. We could give in... or remind them who's really in charge. It's not like they'll go on strike or anything!",
        {EffectType::LOSE_MONEY, EffectType::INCREASE_MONEY_RATE},
        {EffectType::REDUCE_MONEY_RATE}
    )},

    {DecisionType::STOCK_REPURCHASE, Decision(
        "Buy Back Shares?",
        "Repurchasing stock makes investors happy and boosts share value. Sure, it drains capital, but when has corporate short-term thinking ever gone wrong?",
        {EffectType::LOSE_MONEY, EffectType::INFLATION},
        {EffectType::NO_EFFECT}
    )},

    {DecisionType::COSTCUTTING_MEASURES, Decision(
        "Extreme Cost Cutting?",
        "Slash expenses across the board! Layoffs, budget cuts, selling furniture—nothing is off-limits. This definitely won't lead to long-term ruin.",
        {EffectType::GAIN_MONEY, EffectType::REDUCE_MONEY_RATE},
        {EffectType::NO_EFFECT}
    )},

    {DecisionType::DEFENSIVE_MERGER, Decision(
        "Merge with a Competitor?",
        "A struggling competitor is offering a merger. This could stabilize our market position... or result in a messy power struggle and cultural clashes. Business as usual!",
        {EffectType::INCREASE_MONEY_RATE, EffectType::LOSE_MONEY},
        {EffectType::NO_EFFECT}
    )},

    {DecisionType::FIRST_ORDER, Decision(
        "Mandatory HORIZONTAL Expansion",
        "The board has issued a directive: BUILD SIDEWAY. All new structures must be stacked to meet a strict WIDTH requirement. This will surely improve efficiency and not result in instability. You may acknowledge the directive... or politely acknowledge the directive. Either way, compliance is expected.",
        {EffectType::NO_EFFECT},
        {EffectType::BAD_LUCK}
    )},
};

