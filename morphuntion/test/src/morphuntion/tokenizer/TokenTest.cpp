/*
 * Copyright 2017-2024 Apple Inc. All rights reserved.
 */
#include "catch2/catch_test_macros.hpp"

#include <morphuntion/tokenizer/Token_Head.hpp>
#include <morphuntion/tokenizer/Token_Word.hpp>
#include <morphuntion/tokenizer/TokenChain.hpp>
#include <morphuntion/tokenizer/Tokenizer.hpp>
#include <morphuntion/tokenizer/TokenizerFactory.hpp>
#include <morphuntion/util/ULocale.hpp>
#include <morphuntion/util/LocaleUtils.hpp>
#include <morphuntion/npc.hpp>
#include <memory>

TEST_CASE("TokenTest#testTokenEquality")
{
    ::std::unique_ptr<::morphuntion::tokenizer::Tokenizer> tok(::morphuntion::tokenizer::TokenizerFactory::createTokenizer(::morphuntion::util::LocaleUtils::US()));
    ::std::unique_ptr<::morphuntion::tokenizer::TokenChain> tc(tok->createTokenChain(u"Hello, my name is Fred."));
    auto a = tc->getHead();
    auto b = npc(a)->getNext();
    auto c = npc(b)->getNext();
    auto d = npc(c)->getNext();
    auto e = npc(d)->getNext();
    auto f = npc(e)->getNext();
    auto g = npc(f)->getNext();
    auto h = npc(g)->getNext();
    auto i = npc(h)->getNext();
    auto j = npc(i)->getNext();
    auto k = npc(j)->getNext();
    auto l = npc(k)->getNext();
    auto m = npc(l)->getNext();
    REQUIRE(npc(m)->getNext() == nullptr);
    ::std::unique_ptr<::morphuntion::tokenizer::TokenChain> otherTokChain(tok->createTokenChain(u"Hello fred"));
    auto head = otherTokChain->getHead();
    auto hello = otherTokChain->getToken(0);
    auto fred = otherTokChain->getToken(6);
    REQUIRE(hello != nullptr);
    REQUIRE(fred != nullptr);

    REQUIRE(*b == *hello);
    REQUIRE(*d == *f);
    REQUIRE(*a == *head);

    REQUIRE(*a != *b);
    REQUIRE(*a != *d);
    REQUIRE(*a != *m);
    REQUIRE(*b != *e);
    REQUIRE(*c != *l);
    REQUIRE(a != nullptr);

    REQUIRE(*k != *fred);

    morphuntion::tokenizer::Token_Word is(0, 2, u"is", u"is", true);
    REQUIRE(*i == is);
    morphuntion::tokenizer::Token_Word isInsignificant(0, 2, u"is", u"is", false);
    REQUIRE(*i != isInsignificant);
    REQUIRE(morphuntion::tokenizer::Token_Word(0, 2, u"is", u"is", false) == isInsignificant);
}

TEST_CASE("TokenTest#testCollation")
{
    morphuntion::tokenizer::Token_Word g1(0, 2, u"oe", u"oe", true);
    morphuntion::tokenizer::Token_Word g2(0, 1, u"ö", u"ö", true);
    REQUIRE(g1 != g2);
}
