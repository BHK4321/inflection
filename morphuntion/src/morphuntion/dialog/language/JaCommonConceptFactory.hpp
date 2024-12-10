/*
 * Copyright 2017-2024 Apple Inc. All rights reserved.
 */
#pragma once

#include <morphuntion/dialog/language/fwd.hpp>
#include <morphuntion/dialog/CommonConceptFactoryImpl.hpp>

class morphuntion::dialog::language::JaCommonConceptFactory
    : public morphuntion::dialog::CommonConceptFactoryImpl
{
public:
    typedef CommonConceptFactoryImpl super;
public:
    SemanticConceptList* createOrList(const ::std::vector<const ::morphuntion::dialog::SemanticFeatureConceptBase*>& concepts) const override;
    SemanticConceptList* createAndList(const ::std::vector<const ::morphuntion::dialog::SemanticFeatureConceptBase*>& concepts) const override;
    ::morphuntion::dialog::SpeakableString quote(const ::morphuntion::dialog::SpeakableString& str) const override;

protected: /* package */
    ::morphuntion::dialog::SpeakableString quantifiedJoin(const ::morphuntion::dialog::SpeakableString& formattedNumber, const ::morphuntion::dialog::SpeakableString& nounPhrase, const ::std::u16string& measureWord, Plurality::Rule countType) const override;

public:
    explicit JaCommonConceptFactory(const ::morphuntion::util::ULocale& language);
    ~JaCommonConceptFactory() override;
};
