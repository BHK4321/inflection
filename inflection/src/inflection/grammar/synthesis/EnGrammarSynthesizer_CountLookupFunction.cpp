/*
 * Copyright 2017-2024 Apple Inc. All rights reserved.
 */
#include <inflection/grammar/synthesis/EnGrammarSynthesizer_CountLookupFunction.hpp>

#include <inflection/grammar/synthesis/GrammemeConstants.hpp>
#include <inflection/util/Validate.hpp>
#include <inflection/util/LocaleUtils.hpp>
#include <inflection/tokenizer/Token_Word.hpp>
#include <inflection/tokenizer/Token.hpp>
#include <inflection/tokenizer/TokenChain.hpp>
#include <inflection/tokenizer/Tokenizer.hpp>
#include <inflection/tokenizer/TokenizerFactory.hpp>
#include <inflection/npc.hpp>
#include <memory>

namespace inflection::grammar::synthesis {

EnGrammarSynthesizer_CountLookupFunction::EnGrammarSynthesizer_CountLookupFunction()
    : super(::inflection::util::LocaleUtils::ENGLISH(), {GrammemeConstants::NUMBER_SINGULAR(), GrammemeConstants::NUMBER_PLURAL()}, {GrammemeConstants::POS_NOUN(), GrammemeConstants::POS_VERB()})
    , tokenizer(::inflection::tokenizer::TokenizerFactory::createTokenizer(::inflection::util::LocaleUtils::ENGLISH()))
    , dictionary(getDictionary())
{
    ::inflection::util::Validate::notNull(dictionary.getBinaryProperties(&prepositionProperty, {u"adposition"}));
}

EnGrammarSynthesizer_CountLookupFunction::~EnGrammarSynthesizer_CountLookupFunction()
{

}

::std::u16string EnGrammarSynthesizer_CountLookupFunction::determine(const ::std::u16string& word) const
{
    auto out = super::determine(word);
    if (!out.empty() || word.empty()) {
        return out;
    }
    ::std::unique_ptr<::inflection::tokenizer::TokenChain> tokenChain(npc(npc(tokenizer.get())->createTokenChain(word)));
    const ::inflection::tokenizer::Token* lastWordToken = nullptr;
    for (const auto& token : *tokenChain) {
        if (dynamic_cast<const ::inflection::tokenizer::Token_Word*>(&token) != nullptr) {
            if (lastWordToken != nullptr && dictionary.hasAllProperties(token.getCleanValue(), prepositionProperty)) {
                // Handle "to the red light on the front porch" where "light" is the noun to check.
                out = super::determine(npc(lastWordToken)->getValue());
                if (!out.empty()) {
                    return out;
                }
                break;
            }
            lastWordToken = &token;
        }
    }
    return super::determine(npc(npc(tokenChain->getEnd())->getPrevious())->getValue());
}

} // namespace inflection::grammar::synthesis
