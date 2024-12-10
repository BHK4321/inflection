/*
 * Copyright 2017-2024 Apple Inc. All rights reserved.
 */
#include <morphuntion/grammar/synthesis/PtGrammarSynthesizer.hpp>

#include <morphuntion/dialog/ArticleDetectionFunction.hpp>
#include <morphuntion/dialog/SemanticFeatureModel.hpp>
#include <morphuntion/grammar/synthesis/GrammemeConstants.hpp>
#include <morphuntion/grammar/synthesis/PtGrammarSynthesizer_ArticleLookupFunction.hpp>
#include <morphuntion/grammar/synthesis/PtGrammarSynthesizer_GenericLookupFunction.hpp>
#include <morphuntion/grammar/synthesis/PtGrammarSynthesizer_PtDisplayFunction.hpp>
#include <morphuntion/util/LocaleUtils.hpp>
#include <morphuntion/npc.hpp>

namespace morphuntion::grammar::synthesis {

void PtGrammarSynthesizer::addSemanticFeatures(::morphuntion::dialog::SemanticFeatureModel& featureModel)
{
    featureModel.putDefaultFeatureFunctionByName(ARTICLE_DEFINITE, new PtGrammarSynthesizer_ArticleLookupFunction(featureModel, u"", u"", u"o(a)", u"os(as)", u"o", u"a", u"os", u"as"));
    featureModel.putDefaultFeatureFunctionByName(ARTICLE_INDEFINITE, new PtGrammarSynthesizer_ArticleLookupFunction(featureModel, u"", u"", u"um(a)", u"uns(umas)", u"um", u"uma", u"uns", u"umas"));
    featureModel.putDefaultFeatureFunctionByName(ARTICLE_INDEFINITE_EM_PREPOSITION, new PtGrammarSynthesizer_ArticleLookupFunction(featureModel, u"", u"em", u"num(a)", u"nuns(umas)", u"num", u"numa", u"nuns", u"numas"));
    featureModel.putDefaultFeatureFunctionByName(WITH_ARTICLE_INDEFINITE_EM_PREPOSITION, new PtGrammarSynthesizer_ArticleLookupFunction(featureModel, ARTICLE_INDEFINITE_EM_PREPOSITION, u"em", u"num(a)", u"nuns(umas)", u"num", u"numa", u"nuns", u"numas"));
    featureModel.putDefaultFeatureFunctionByName(ARTICLE_A_PREPOSITION, new PtGrammarSynthesizer_ArticleLookupFunction(featureModel, u"", u"", u"ao(à)", u"aos(às)", u"ao", u"à", u"aos", u"às"));
    featureModel.putDefaultFeatureFunctionByName(WITH_ARTICLE_A_PREPOSITION, new PtGrammarSynthesizer_ArticleLookupFunction(featureModel, ARTICLE_A_PREPOSITION, u"", u"ao(à)", u"aos(às)", u"ao", u"à", u"aos", u"às"));
    featureModel.putDefaultFeatureFunctionByName(ARTICLE_DE_PREPOSITION, new PtGrammarSynthesizer_ArticleLookupFunction(featureModel, u"", u"de", u"do(a)", u"dos(as)", u"do", u"da", u"dos", u"das"));
    featureModel.putDefaultFeatureFunctionByName(WITH_ARTICLE_DE_PREPOSITION, new PtGrammarSynthesizer_ArticleLookupFunction(featureModel, ARTICLE_DE_PREPOSITION, u"de", u"do(a)", u"dos(as)", u"do", u"da", u"dos", u"das"));
    featureModel.putDefaultFeatureFunctionByName(ARTICLE_EM_PREPOSITION, new PtGrammarSynthesizer_ArticleLookupFunction(featureModel, u"", u"em", u"no(a)", u"nos(as)", u"no", u"na", u"nos", u"nas"));
    featureModel.putDefaultFeatureFunctionByName(WITH_ARTICLE_EM_PREPOSITION, new PtGrammarSynthesizer_ArticleLookupFunction(featureModel, ARTICLE_EM_PREPOSITION, u"em", u"no(a)", u"nos(as)", u"no", u"na", u"nos", u"nas"));
    featureModel.putDefaultFeatureFunctionByName(ARTICLE_POR_PREPOSITION, new PtGrammarSynthesizer_ArticleLookupFunction(featureModel, u"", u"por", u"pelo(a)", u"pelos(as)", u"pelo", u"pela", u"pelos", u"pelas"));
    featureModel.putDefaultFeatureFunctionByName(WITH_ARTICLE_POR_PREPOSITION, new PtGrammarSynthesizer_ArticleLookupFunction(featureModel, ARTICLE_POR_PREPOSITION, u"por", u"pelo(a)", u"pelos(as)", u"pelo", u"pela", u"pelos", u"pelas"));
    featureModel.putDefaultFeatureFunctionByName(DEMONSTRATIVE_ADJECTIVE, new PtGrammarSynthesizer_ArticleLookupFunction(featureModel, u"", u"", u"este(a)", u"estes(as)", u"este", u"esta", u"estes", u"estas"));
    featureModel.putDefaultFeatureFunctionByName(WITH_DEMONSTRATIVE_ADJECTIVE, new PtGrammarSynthesizer_ArticleLookupFunction(featureModel, DEMONSTRATIVE_ADJECTIVE, u"", u"este(a)", u"estes(as)", u"este", u"esta", u"estes", u"estas"));
    featureModel.putDefaultFeatureFunctionByName(DEMONSTRATIVE_IN_ADJECTIVE, new PtGrammarSynthesizer_ArticleLookupFunction(featureModel, u"", u"", u"neste(a)", u"nestes(as)", u"neste", u"nesta", u"nestes", u"nestas"));
    featureModel.putDefaultFeatureFunctionByName(WITH_DEMONSTRATIVE_IN_ADJECTIVE, new PtGrammarSynthesizer_ArticleLookupFunction(featureModel, DEMONSTRATIVE_IN_ADJECTIVE, u"", u"neste(a)", u"nestes(as)", u"neste", u"nesta", u"nestes", u"nestas"));
    featureModel.putDefaultFeatureFunctionByName(DEMONSTRATIVE_OF_ADJECTIVE, new PtGrammarSynthesizer_ArticleLookupFunction(featureModel, u"", u"", u"deste(a)", u"destes(as)", u"deste", u"desta", u"destes", u"destas"));
    featureModel.putDefaultFeatureFunctionByName(WITH_DEMONSTRATIVE_OF_ADJECTIVE, new PtGrammarSynthesizer_ArticleLookupFunction(featureModel, DEMONSTRATIVE_OF_ADJECTIVE, u"", u"deste(a)", u"destes(as)", u"deste", u"desta", u"destes", u"destas"));
    featureModel.putDefaultFeatureFunctionByName(DEMONSTRATIVE_THIS_ADJECTIVE, new PtGrammarSynthesizer_ArticleLookupFunction(featureModel, u"", u"", u"esse(a)", u"esses(as)", u"esse", u"essa", u"esses", u"essas"));
    featureModel.putDefaultFeatureFunctionByName(WITH_DEMONSTRATIVE_THIS_ADJECTIVE, new PtGrammarSynthesizer_ArticleLookupFunction(featureModel, DEMONSTRATIVE_THIS_ADJECTIVE, u"", u"esse(a)", u"esses(as)", u"esse", u"essa", u"esses", u"essas"));
    featureModel.putDefaultFeatureFunctionByName(DEMONSTRATIVE_IN_THIS_ADJECTIVE, new PtGrammarSynthesizer_ArticleLookupFunction(featureModel, u"", u"", u"nesse(a)", u"nesses(as)", u"nesse", u"nessa", u"nesses", u"nessas"));
    featureModel.putDefaultFeatureFunctionByName(WITH_DEMONSTRATIVE_IN_THIS_ADJECTIVE, new PtGrammarSynthesizer_ArticleLookupFunction(featureModel, DEMONSTRATIVE_IN_THIS_ADJECTIVE, u"", u"nesse(a)", u"nesses(as)", u"nesse", u"nessa", u"nesses", u"nessas"));
    featureModel.putDefaultFeatureFunctionByName(DEMONSTRATIVE_OF_THIS_ADJECTIVE, new PtGrammarSynthesizer_ArticleLookupFunction(featureModel, u"", u"", u"desse(a)", u"desses(as)", u"desse", u"dessa", u"desses", u"dessas"));
    featureModel.putDefaultFeatureFunctionByName(WITH_DEMONSTRATIVE_OF_THIS_ADJECTIVE, new PtGrammarSynthesizer_ArticleLookupFunction(featureModel, DEMONSTRATIVE_OF_THIS_ADJECTIVE, u"", u"desse(a)", u"desses(as)", u"desse", u"dessa", u"desses", u"dessas"));
    featureModel.putDefaultFeatureFunctionByName(DEMONSTRATIVE_THAT_ADJECTIVE, new PtGrammarSynthesizer_ArticleLookupFunction(featureModel, u"", u"", u"aquele(a)", u"aqueles(as)", u"aquele", u"aquela", u"aqueles", u"aquelas"));
    featureModel.putDefaultFeatureFunctionByName(WITH_DEMONSTRATIVE_THAT_ADJECTIVE, new PtGrammarSynthesizer_ArticleLookupFunction(featureModel, DEMONSTRATIVE_THAT_ADJECTIVE, u"", u"aquele(a)", u"aqueles(as)", u"aquele", u"aquela", u"aqueles", u"aquelas"));
    featureModel.putDefaultFeatureFunctionByName(DEMONSTRATIVE_IN_THAT_ADJECTIVE, new PtGrammarSynthesizer_ArticleLookupFunction(featureModel, u"", u"", u"naquele(a)", u"naqueles(as)", u"naquele", u"naquela", u"naqueles", u"naquelas"));
    featureModel.putDefaultFeatureFunctionByName(WITH_DEMONSTRATIVE_IN_THAT_ADJECTIVE, new PtGrammarSynthesizer_ArticleLookupFunction(featureModel, DEMONSTRATIVE_IN_THAT_ADJECTIVE, u"", u"naquele(a)", u"naqueles(as)", u"naquele", u"naquela", u"naqueles", u"naquelas"));
    featureModel.putDefaultFeatureFunctionByName(DEMONSTRATIVE_OF_THAT_ADJECTIVE, new PtGrammarSynthesizer_ArticleLookupFunction(featureModel, u"", u"", u"daquele(a)", u"daqueles(as)", u"daquele", u"daquela", u"daqueles", u"daquelas"));
    featureModel.putDefaultFeatureFunctionByName(WITH_DEMONSTRATIVE_OF_THAT_ADJECTIVE, new PtGrammarSynthesizer_ArticleLookupFunction(featureModel, DEMONSTRATIVE_OF_THAT_ADJECTIVE, u"", u"daquele(a)", u"daqueles(as)", u"daquele", u"daquela", u"daqueles", u"daquelas"));
    featureModel.putDefaultFeatureFunctionByName(ARTICLE_POSSESSIVE, new PtGrammarSynthesizer_ArticleLookupFunction(featureModel, u"", u"", u"seu(sua)", u"seus(suas)", u"seu", u"sua", u"seus", u"suas"));
    featureModel.putDefaultFeatureFunctionByName(WITH_ARTICLE_POSSESSIVE, new PtGrammarSynthesizer_ArticleLookupFunction(featureModel, ARTICLE_POSSESSIVE, u"", u"seu(sua)", u"seus(suas)", u"seu", u"sua", u"seus", u"suas"));

    featureModel.putDefaultFeatureFunctionByName(GrammemeConstants::NUMBER, new PtGrammarSynthesizer_GenericLookupFunction({GrammemeConstants::NUMBER_SINGULAR(), GrammemeConstants::NUMBER_PLURAL()}));
    featureModel.putDefaultFeatureFunctionByName(GrammemeConstants::GENDER, new PtGrammarSynthesizer_GenericLookupFunction({GrammemeConstants::GENDER_MASCULINE(), GrammemeConstants::GENDER_FEMININE()}));
    featureModel.putDefaultFeatureFunctionByName(GrammemeConstants::DEFINITENESS, new morphuntion::dialog::ArticleDetectionFunction(morphuntion::util::LocaleUtils::PORTUGUESE(),
         {ARTICLE_DEFINITE, ARTICLE_DE_PREPOSITION, ARTICLE_A_PREPOSITION, ARTICLE_EM_PREPOSITION, WITH_ARTICLE_POR_PREPOSITION}, {u"de", u"em", u"por"},
         {ARTICLE_INDEFINITE}, {}));

    featureModel.setDefaultDisplayFunction(new PtGrammarSynthesizer_PtDisplayFunction(featureModel));
}

PtGrammarSynthesizer::Count PtGrammarSynthesizer::getCount(const ::std::u16string* value) {
    static auto valueMap = new ::std::map<::std::u16string, PtGrammarSynthesizer::Count>({
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

PtGrammarSynthesizer::Gender PtGrammarSynthesizer::getGender(const ::std::u16string* value) {
    static auto valueMap = new ::std::map<::std::u16string, PtGrammarSynthesizer::Gender>({
        {GrammemeConstants::GENDER_MASCULINE(), Gender::masculine},
        {GrammemeConstants::GENDER_FEMININE(), Gender::feminine},
    });
    if (value != nullptr) {
        auto result = npc(valueMap)->find(*npc(value));
        if (result != npc(valueMap)->end()) {
            return result->second;
        }
    }
    return Gender::undefined;
}

} // namespace morphuntion::grammar::synthesis
