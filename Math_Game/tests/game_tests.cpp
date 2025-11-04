#include "../catch2/catch.hpp"
#include "../Game.h"
#include <string>
#include <cstdlib>

// Helper: extract numeric value after a key like "answer":
static int extract_answer_from_json(const std::string& json, const std::string& key = "answer") {
    auto pos = json.find('"' + key + '"');
    if (pos == std::string::npos) return -999999;
    pos = json.find(':', pos);
    if (pos == std::string::npos) return -999999;
    ++pos;
    // skip spaces
    while (pos < json.size() && isspace((unsigned char)json[pos])) ++pos;
    // read optional minus
    size_t end = pos;
    if (json[end] == '-') ++end;
    while (end < json.size() && (json[end] == '+' || json[end] == '-' || isdigit((unsigned char)json[end]))) ++end;
    std::string num = json.substr(pos, end - pos);
    return std::atoi(num.c_str());
}

TEST_CASE("Game question and answer", "[game]") {
    Game::initialize();

    auto q = Game::get_question();
    std::string qstr = q.dump();
    REQUIRE(qstr.find("answer") != std::string::npos);

    int correct = extract_answer_from_json(qstr, "answer");
    REQUIRE(correct != -999999);

    auto r1 = Game::check_answer(correct, correct);
    std::string r1s = r1.dump();
    REQUIRE(r1s.find("\"correct\":true") != std::string::npos);

    auto r2 = Game::check_answer(correct + 1, correct);
    std::string r2s = r2.dump();
    REQUIRE(r2s.find("\"correct\":false") != std::string::npos);
}
