/*
 * Copyright 2017-2024 Apple Inc. All rights reserved.
 */
#pragma once

#include <inflection/dialog/language/fwd.hpp>
#include <inflection/dialog/CommonConceptFactoryImpl.hpp>

class inflection::dialog::language::KoCommonConceptFactory
    : public inflection::dialog::CommonConceptFactoryImpl
{
public:
    typedef CommonConceptFactoryImpl super;
public:
    SemanticConceptList* createOrList(const ::std::vector<const ::inflection::dialog::SemanticFeatureConceptBase*>& concepts) const override;
    SemanticConceptList* createAndList(const ::std::vector<const ::inflection::dialog::SemanticFeatureConceptBase*>& concepts) const override;

protected: /* package */
    ::inflection::dialog::SpeakableString quantifiedJoin(const ::inflection::dialog::SpeakableString& formattedNumber, const ::inflection::dialog::SpeakableString& nounPhrase, const ::std::u16string& measureWord, Plurality::Rule countType) const override;

public:
    explicit KoCommonConceptFactory(const ::inflection::util::ULocale& language);
};
