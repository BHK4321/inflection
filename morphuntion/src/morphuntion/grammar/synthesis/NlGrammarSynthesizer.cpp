/*
 * Copyright 2017-2024 Apple Inc. All rights reserved.
 */
#include <morphuntion/grammar/synthesis/NlGrammarSynthesizer.hpp>

#include <morphuntion/dialog/DictionaryLookupFunction.hpp>
#include <morphuntion/dialog/SemanticFeatureModel.hpp>
#include <morphuntion/grammar/synthesis/GrammemeConstants.hpp>
#include <morphuntion/grammar/synthesis/NlGrammarSynthesizer_ArticleLookupFunction.hpp>
#include <morphuntion/grammar/synthesis/NlGrammarSynthesizer_DefinitenessLookupFunction.hpp>
#include <morphuntion/grammar/synthesis/NlGrammarSynthesizer_NlDisplayFunction.hpp>
#include <morphuntion/util/LocaleUtils.hpp>
#include <morphuntion/npc.hpp>

namespace morphuntion::grammar::synthesis {

const ::std::u16string& NlGrammarSynthesizer::SIZENESS()
{
    static auto SIZENESS_ = new ::std::u16string(u"sizeness");
    return *npc(SIZENESS_);
}

const ::std::u16string& NlGrammarSynthesizer::DECLENSION()
{
    static auto DECLENSION_ = new ::std::u16string(u"declension");
    return *npc(DECLENSION_);
}

const ::std::u16string& NlGrammarSynthesizer::DECLENSION_UNDECLINED()
{
    static auto DECLENSION_UNDECLINED_ = new ::std::u16string(u"undeclined");
    return *npc(DECLENSION_UNDECLINED_);
}

const ::std::u16string& NlGrammarSynthesizer::DECLENSION_DECLINED()
{
    static auto DECLENSION_DECLINED_ = new ::std::u16string(u"declined");
    return *npc(DECLENSION_DECLINED_);
}

const ::std::u16string& NlGrammarSynthesizer::SIZENESS_DIMINUTIVE()
{
    static auto SIZENESS_DIMINUTIVE_ = new ::std::u16string(u"diminutive");
    return *npc(SIZENESS_DIMINUTIVE_);
}
void NlGrammarSynthesizer::addSemanticFeatures(::morphuntion::dialog::SemanticFeatureModel& featureModel)
{
    featureModel.putDefaultFeatureFunctionByName(ARTICLE_DEFINITE, new NlGrammarSynthesizer_ArticleLookupFunction(featureModel, false, u"de", u"het"));
    featureModel.putDefaultFeatureFunctionByName(GrammemeConstants::NUMBER, new morphuntion::dialog::DictionaryLookupFunction(::morphuntion::util::LocaleUtils::DUTCH(), {GrammemeConstants::NUMBER_SINGULAR(), GrammemeConstants::NUMBER_PLURAL()}));
    featureModel.putDefaultFeatureFunctionByName(GrammemeConstants::GENDER, new morphuntion::dialog::DictionaryLookupFunction(::morphuntion::util::LocaleUtils::DUTCH(), {GrammemeConstants::GENDER_MASCULINE(), GrammemeConstants::GENDER_FEMININE(), GrammemeConstants::GENDER_NEUTER()}));
    featureModel.putDefaultFeatureFunctionByName(GrammemeConstants::DEFINITENESS, new NlGrammarSynthesizer_DefinitenessLookupFunction());
    featureModel.setDefaultDisplayFunction(new NlGrammarSynthesizer_NlDisplayFunction(featureModel));
}

static std::u16string_view AEIOU_SET()
{
    static auto AEIOU_SET_ = new std::u16string_view(u"aeiou");
    return *npc(AEIOU_SET_);
}

static std::u16string_view BLNMRP_SET()
{
    static auto BLNMRP_SET_ = new std::u16string_view(u"blnmrpg");
    return *npc(BLNMRP_SET_);
}

static std::u16string_view LRN_SET()
{
    static auto LRN_SET_ = new std::u16string_view(u"lrn");
    return *npc(LRN_SET_);
}

bool NlGrammarSynthesizer::isVowelAtIndex(std::u16string_view str, int32_t index)
{
    return (AEIOU_SET().find(str[index])) != std::u16string_view::npos || (index > 0 && str[index] == u'j' && str[index - 1] == u'i');
}

void NlGrammarSynthesizer::guessDiminutive(::std::u16string* result, std::u16string_view word) {
    auto guess(word);
    if (::morphuntion::util::StringViewUtils::endsWith(guess, u"jes")) {
        guess.remove_suffix(1);
    }
    int32_t len = int32_t(guess.length());
    if (len > 2 && ::morphuntion::util::StringViewUtils::endsWith(guess, u"je")) {
        if (len > 3 && guess[len - 3] == u'p' && guess[len - 4] == u'm') {
            npc(result)->assign(guess, 0, len - 3);
        } else if (len > 4 && guess[len - 3] == u'k' && guess[len - 4] == u'n' && guess[len - 5] == u'i') {
            npc(result)->assign(guess, 0, len - 3).append(u"g");
        } else if (len > 3 && guess[len - 3] == u't') {
            if (guess[len - 4] == u'\'') {
                npc(result)->assign(guess, 0, len - 4);
            } else if (len > 7 && guess[len - 4] == u'e' && (BLNMRP_SET().find(guess[len - 5]) != std::u16string_view::npos || (guess[len - 5] == u'g' && guess[len - 6] == u'n'))) {
                if (guess[len - 6] == guess[len - 5] && isVowelAtIndex(guess, len - 7)) {
                    npc(result)->assign(guess, 0, len - 5);
                } else {
                    npc(result)->assign(guess, 0, len - 4);
                }
            } else if (isVowelAtIndex(guess, len - 4)) {
                if (len > 4 && guess[len - 4] == guess[len - 5]) {
                    npc(result)->assign(guess, 0, len - 4);
                } else {
                    npc(result)->assign(guess, 0, len - 3);
                }
            } else if (LRN_SET().find(guess[len - 4]) != std::u16string_view::npos) {
                npc(result)->assign(guess, 0, len - 3);
            }
        } else if (guess[len - 3] == u'\'') {
            npc(result)->assign(guess, 0, len - 3);
        }
        else {
            npc(result)->assign(guess, 0, len - 2);
        }
    }
}


NlGrammarSynthesizer::Count NlGrammarSynthesizer::getCount(const ::std::u16string* value) {
    static auto valueMap = new ::std::map<::std::u16string, NlGrammarSynthesizer::Count>({
        {GrammemeConstants::NUMBER_SINGULAR(), Count::singular},
        {GrammemeConstants::NUMBER_PLURAL(), Count::plural}
    });
    if (value != nullptr) {
        auto result = npc(valueMap)->find(*npc(value));
        if (result != npc(valueMap)->end()) {
            return result->second;
        }
    }
    return Count::undefined;
}

NlGrammarSynthesizer::Gender NlGrammarSynthesizer::getGender(const ::std::u16string* value) {
    static auto valueMap = new ::std::map<::std::u16string, NlGrammarSynthesizer::Gender>({
        {GrammemeConstants::GENDER_MASCULINE(), Gender::masculine},
        {GrammemeConstants::GENDER_FEMININE(), Gender::feminine},
        {GrammemeConstants::GENDER_NEUTER(), Gender::neuter}
    });
    if (value != nullptr) {
        auto result = npc(valueMap)->find(*npc(value));
        if (result != npc(valueMap)->end()) {
            return result->second;
        }
    }
    return Gender::undefined;
}

NlGrammarSynthesizer::Declension NlGrammarSynthesizer::getDeclension(const ::std::u16string* value) {
    static auto valueMap = new ::std::map<::std::u16string, NlGrammarSynthesizer::Declension>({
        {DECLENSION_DECLINED(), Declension::declined},
        {DECLENSION_UNDECLINED(), Declension::undeclined},
    });
    if (value != nullptr) {
        auto result = npc(valueMap)->find(*npc(value));
        if (result != npc(valueMap)->end()) {
            return result->second;
        }
    }
    return Declension::undefined;
}

NlGrammarSynthesizer::Case NlGrammarSynthesizer::getCase(const ::std::u16string* value) {
    static auto valueMap = new ::std::map<::std::u16string, NlGrammarSynthesizer::Case>({
        {GrammemeConstants::CASE_GENITIVE(), Case::genitive},
        {GrammemeConstants::CASE_NOMINATIVE(), Case::nominative},
    });
    if (value != nullptr) {
        auto result = npc(valueMap)->find(*npc(value));
        if (result != npc(valueMap)->end()) {
            return result->second;
        }
    }
    return Case::undefined;
}

} // namespace morphuntion::grammar::synthesis
