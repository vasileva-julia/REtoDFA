#include<gtest/gtest.h>
#include "dfa.h"
#include "syntaxtree.h"
#include "alphabet.h"


TEST(eq_test, eq_test_1) {
    std::map<std::string, std::map<char, std::string>> t1;
    t1["0"] = { {'a', "2"}, {'b', "1"} };
    t1["1"] = { {'a', "1"}, {'b', "1"} };
    t1["2"] = { {'a', "2"}, {'b', "3"} };
    t1["3"] = { {'a', "2"}, {'b', "3"} };
    DFA dfa1("ab", {"0", "1", "2", "3"}, "0", {"3"}, t1);

    std::map<std::string, std::map<char, std::string>> t2;
    t2["0"] = { {'a', "2"}, {'b', "1"} };
    t2["1"] = { {'a', "4"}, {'b', "1"} };
    t2["2"] = { {'a', "2"}, {'b', "5"} };
    t2["3"] = { {'a', "2"}, {'b', "3"} };
    t2["4"] = { {'a', "1"}, {'b', "4"} };
    t2["5"] = { {'a', "2"}, {'b', "3"} };
    DFA dfa2("ab", {"0", "1", "2", "3", "4", "5"}, "0", {"3", "5"}, t2);

    EXPECT_EQ(dfa1, dfa2);
}

TEST(eq_test, eq_test_2) {
    std::map<std::string, std::map<char, std::string>> t1;
    t1["0"] = { {'a', "1"} };
    t1["1"] = { {'a', "2"}, {'b', "1"} };
    DFA dfa1("ab", {"0", "1", "2"}, "0", {"2"}, t1);


    std::map<std::string, std::map<char, std::string>> t2;
    t2["0"] = { {'a', "1"} };
    t2["1"] = { {'a', "2"}, {'b', "0"} };
    DFA dfa2("ab", {"0", "1", "2"}, "0", {"2"}, t2);

    EXPECT_NE(dfa1, dfa2);
}

TEST(eq_test, eq_test_3) {
    std::map<std::string, std::map<char, std::string>> t1;
    t1["1"] = { {'a', "3"}, {'b', "2"} };
    DFA dfa1("ab", {"1", "2", "3"}, "1", {"3"}, t1);

    std::map<std::string, std::map<char, std::string>> t2;
    t2["1"] = { {'a', "3"} };
    DFA dfa2("ab", {"1", "3"}, "1", {"3"}, t2);

    EXPECT_EQ(dfa1, dfa2);
}

TEST(eq_test, eq_test_4) {
    std::map<std::string, std::map<char, std::string>> t1;
    t1["1"] = { {'a', "2"} };
    DFA dfa1("ab", {"1", "2"}, "1", {"2"}, t1);

    std::map<std::string, std::map<char, std::string>> t2;
    t2["1"] = { {'a', "2"} };
    DFA dfa2("a", {"1", "2"}, "1", {"2"}, t2);

    EXPECT_NE(dfa1, dfa2);
}

TEST(eq_test, eq_test_5) {
    std::map<std::string, std::map<char, std::string>> t1;
    t1["1"] = { {'a', "2"} };
    DFA dfa1("ab", {"1", "2"}, "1", {"2"}, t1);

    std::map<std::string, std::map<char, std::string>> t2;
    t2["one"] = { {'a', "two"} };
    DFA dfa2("ab", {"one", "two"}, "one", {"two"}, t2);

    EXPECT_EQ(dfa1, dfa2);
}

TEST(eq_test, eq_test_6) {
    std::map<std::string, std::map<char, std::string>> t1;
    t1["1"] = { {'a', "3"}, {'b', "2"} };
    DFA dfa1("ab", {"1", "2", "3"}, "1", {}, t1);

    std::map<std::string, std::map<char, std::string>> t2;
    t2["1"] = { {'a', "3"} };
    DFA dfa2("ab", {"1", "3"}, "1", {}, t2);

    EXPECT_EQ(dfa1, dfa2);
}

TEST(eq_test, eq_test_7) {
    std::map<std::string, std::map<char, std::string>> t1;
    DFA dfa1("ab", {"0", "1"}, "0", {"0"}, t1);

    std::map<std::string, std::map<char, std::string>> t2;
    t2["0"] = { {'a', "1"} };
    DFA dfa2("ab", {"0", "1"}, "0", {"1"}, t2);

    EXPECT_NE(dfa1, dfa2);
}

////////////////////////


TEST(tree_test, tree_test_1) {
    std::string str = "a";
    SyntaxTree tree(str);
    std::vector<std::set<size_t>> ans = {{2}, {}};
    std::vector<std::set<size_t>> res = {tree.getFollowpos(1), tree.getFollowpos(2)};

    EXPECT_EQ(ans, res);
}

TEST(tree_test, tree_test_2) {
    std::string str = "ab";
    SyntaxTree tree(str);
    std::vector<std::set<size_t>> ans = {{2}, {3}, {}};
    std::vector<std::set<size_t>> res = {tree.getFollowpos(1), tree.getFollowpos(2), tree.getFollowpos(3)};

    EXPECT_EQ(ans, res);
}

TEST(tree_test, tree_test_3) {
    std::string str = "a+b";
    SyntaxTree tree(str);
    std::vector<std::set<size_t>> ans = {{2}, {3}, {}};
    std::vector<std::set<size_t>> res = {tree.getFollowpos(1), tree.getFollowpos(2), tree.getFollowpos(3)};

    EXPECT_EQ(ans, res);
}

TEST(tree_test, tree_test_4) {
    std::string str = "a|b";
    SyntaxTree tree(str);
    std::vector<std::set<size_t>> ans = {{3}, {3}, {}};
    std::vector<std::set<size_t>> res = {tree.getFollowpos(1), tree.getFollowpos(2), tree.getFollowpos(3)};

    EXPECT_EQ(ans, res);
}

TEST(tree_test, tree_test_5) {
    std::string str = "a*";
    SyntaxTree tree(str);
    std::vector<std::set<size_t>> ans = {{1, 2}, {}};
    std::vector<std::set<size_t>> res = {tree.getFollowpos(1), tree.getFollowpos(2)};

    EXPECT_EQ(ans, res);
}

TEST(tree_test, tree_test_6) {
    std::string str = "|a";
    SyntaxTree tree(str);
    std::vector<std::set<size_t>> ans = {{2}, {}};
    std::vector<std::set<size_t>> res = {tree.getFollowpos(1), tree.getFollowpos(2)};

    EXPECT_EQ(res, ans);
}

TEST(tree_test, tree_test_7) {
    std::string str = "(a|b)*abb";
    SyntaxTree tree(str);

    std::vector<std::set<size_t>> res(5);
    for (size_t pos = 1; pos < 6; ++pos)
        res[pos - 1] = (tree.getFollowpos(pos));

    std::vector<std::set<size_t>> ans = {
        {1, 2, 3}, {1, 2, 3}, {4}, {5}, {6}
    };

    EXPECT_EQ(res, ans);
}

TEST(tree_test, tree_test_8) {
    std::string str = "(a(b|c))*c";
    SyntaxTree tree(str);

    std::vector<std::set<size_t>> res(4);
    for (size_t pos = 1; pos < 5; ++pos)
        res[pos - 1] = tree.getFollowpos(pos);

    std::vector<std::set<size_t>> ans = {
        {2, 3}, {1, 4}, {1, 4}, {5}
    };

    EXPECT_EQ(res, ans);
}

TEST(tree_test, tree_test_9) {
    std::string str = "()";
    SyntaxTree tree(str);

    std::set<size_t> ans = {};
    std::set<size_t> res = tree.getFollowpos(1);

    EXPECT_EQ(res, ans);
}

////////////////////////


TEST(dfa_test, dfa_1) {
    DFA dfa_res(SyntaxTree("a"));

    std::map<std::string, std::map<char, std::string>> t;
    t["0"] = { {'a', "1"} };
    DFA dfa_ans("a", {"0", "1"}, "0", {"1"}, t);

    EXPECT_EQ(dfa_res, dfa_ans);
}

TEST(dfa_test, dfa_2) {
    DFA dfa_res(SyntaxTree("ab"));

    std::map<std::string, std::map<char, std::string>> t;
    t["0"] = { {'a', "1"} };
    t["1"] = { {'b', "2"} };
    DFA dfa_ans("ab", {"0", "1", "2"}, "0", {"2"}, t);

    EXPECT_EQ(dfa_res, dfa_ans);
}

TEST(dfa_test, dfa_3) {
    DFA dfa_res(SyntaxTree("a+b"));

    std::map<std::string, std::map<char, std::string>> t;
    t["0"] = { {'a', "1"} };
    t["1"] = { {'b', "2"} };
    DFA dfa_ans("ab", {"0", "1", "2"}, "0", {"2"}, t);

    EXPECT_EQ(dfa_res, dfa_ans);
}

TEST(dfa_test, dfa_4) {
    DFA dfa_res(SyntaxTree("a|b"));

    std::map<std::string, std::map<char, std::string>> t;
    t["0"] = { {'a', "1"}, {'b', "1"} };
    DFA dfa_ans("ab", {"0", "1"}, "0", {"1"}, t);

    EXPECT_EQ(dfa_res, dfa_ans);
}

TEST(dfa_test, dfa_5) {
    DFA dfa_res(SyntaxTree("a*"));

    std::map<std::string, std::map<char, std::string>> t;
    t["0"] = { {'a', "0"},};
    DFA dfa_ans("a", {"0"}, "0", {"0"}, t);

    EXPECT_EQ(dfa_res, dfa_ans);
}

TEST(dfa_test, dfa_6) {
    DFA dfa_res(SyntaxTree("|a"));

    std::map<std::string, std::map<char, std::string>> t;
    t["0"] = { {'a', "1"} };
    DFA dfa_ans("a", {"0", "1"}, "0", {"0", "1"}, t);

    EXPECT_EQ(dfa_res, dfa_ans);
}

TEST(dfa_test, dfa_7) {
    DFA dfa_res(SyntaxTree("(a|b)*abb"));

    std::map<std::string, std::map<char, std::string>> t;
    t["0"] = { {'a', "1"}, {'b', "0"} };
    t["1"] = { {'a', "1"}, {'b', "2"} };
    t["2"] = { {'a', "1"}, {'b', "3"} };
    t["3"] = { {'a', "1"}, {'b', "0"} };
    DFA dfa_ans("ab", {"0", "1", "2", "3"}, "0", {"3"}, t);

    EXPECT_EQ(dfa_res, dfa_ans);
}

TEST(dfa_test, dfa_8) {
    std::string str = "(a(b|c))*c";
    SyntaxTree tree(str);

    DFA dfa_res(tree);

    std::map<std::string, std::map<char, std::string>> t;
    t["A"] = { {'a', "B"}, {'c', "C"} };
    t["B"] = { {'b', "A"}, {'c', "A"} };
    DFA dfa_ans("abc", {"A", "B", "C"}, "A", {"C"}, t);

    EXPECT_EQ(dfa_res, dfa_ans);
}

////////////////////////


int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
