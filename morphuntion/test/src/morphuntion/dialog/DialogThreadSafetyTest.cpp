/*
 * Copyright 2020-2024 Apple Inc. All rights reserved.
 */
//
// Created by George Rhoten on 2020-7-14.
//
#include "catch2/catch_test_macros.hpp"

#include <morphuntion/dialog/InflectableStringConcept.hpp>
#include <morphuntion/dialog/SemanticFeature.hpp>
#include <morphuntion/dialog/SemanticFeatureModel.hpp>
#include <morphuntion/dictionary/DictionaryMetaData.hpp>
#include <morphuntion/lang/features/LanguageGrammarFeatures.hpp>
#include <morphuntion/util/LocaleUtils.hpp>
#include <morphuntion/util/StringUtils.hpp>
#include <morphuntion/npc.hpp>
#include <thread>

void testInflection(const morphuntion::dialog::SemanticFeatureModel* model, const ::std::vector<::std::u16string>& phrases, const ::std::vector<::morphuntion::dialog::SemanticFeature>& constraints, const ::std::vector<::morphuntion::dialog::SemanticFeature>& semanticFeatures, int32_t threadID, ::std::vector<::morphuntion::dialog::SpeakableString*>* results)
{
    try {
        auto hasDynamicWordInflection = npc(model)->getDefaultDisplayFunction() != nullptr;
        for (const auto& phrase : phrases) {
            morphuntion::dialog::InflectableStringConcept stringConcept(model, morphuntion::dialog::SpeakableString(phrase));
            auto result = stringConcept.toSpeakableString();
            if (result != nullptr) {
                npc(results)->push_back(result);
            }
            if (hasDynamicWordInflection) {
                for (const auto& constraint : constraints) {
                    for (const auto& semanticValue : constraint.getBoundedValues()) {
                        stringConcept.putConstraint(constraint, semanticValue);
                        result = stringConcept.toSpeakableString();
                        if (result != nullptr) {
                            npc(results)->push_back(result);
                        }
                        stringConcept.clearConstraint(constraint);
                    }
                }
            }
            for (const auto& semanticFeature : semanticFeatures) {
                result = stringConcept.getFeatureValue(semanticFeature);
                if (result != nullptr) {
                    npc(results)->push_back(result);
                }
            }
        }
    }
    catch (const std::exception& e) {
        npc(results)->push_back(new ::morphuntion::dialog::SpeakableString(u"Thread exception " + morphuntion::util::StringUtils::to_u16string(threadID) + u": " + morphuntion::util::StringUtils::to_u16string(e.what())));
    }
}

TEST_CASE("DialogThreadSafetyTest#testInflect", "[multithreaded]")
{
    const int32_t processorCount = (int32_t)std::thread::hardware_concurrency();
    REQUIRE(processorCount > 1); // This test requires at least 2 threads.
    ::std::vector<::std::thread> threads;
    threads.reserve(processorCount);
    ::morphuntion::util::ULocale previousLocale("");
    ::std::vector<::std::u16string> phrases;
    ::std::vector<::morphuntion::dialog::SpeakableString*>* results = new ::std::vector<::morphuntion::dialog::SpeakableString*>[processorCount];
    int32_t minimumNumberOfWords = 10;
    if (getenv("MORPHUN_TEST_EXHAUSTIVE") != nullptr) {
        // We do a quick test for development and Rio, but we can test more exhaustively if we have plenty of time to test.
        minimumNumberOfWords *= 100;
    }

    for (const auto& locale : ::morphuntion::util::LocaleUtils::getSupportedLocaleList()) {
        if (locale.getLanguage() == previousLocale.getLanguage()) {
            // This isn't very interesting. Let's move along.
            continue;
        }
        previousLocale = locale;
        INFO(locale.getName());
        threads.clear();
        phrases.clear();
        morphuntion::dialog::SemanticFeatureModel model(locale);

        auto features = ::morphuntion::lang::features::LanguageGrammarFeatures::getLanguageGrammarFeatures(locale);
        ::std::vector<morphuntion::dialog::SemanticFeature> constraints;
        ::std::vector<morphuntion::dialog::SemanticFeature> semanticFeatures;
        bool hasDisplayFunction = model.getDefaultDisplayFunction() != nullptr;
        for (const auto& [featureName, grammarCategory] : features.getCategories()) {
            if (featureName != u"pos" && featureName != u"sound" && grammarCategory.isUniqueValues()) {
                if (hasDisplayFunction) {
                    constraints.push_back(*npc(model.getFeature(featureName)));
                }
                // Can we query the value for gender, number, case or something like that?
                auto semanticFeature = model.getFeature(featureName);
                if (model.getDefaultFeatureFunction(*npc(semanticFeature)) != nullptr) {
                    semanticFeatures.push_back(*npc(semanticFeature));
                }
            }
            // else we don't support inflecting these other types, or they aren't interesting.
        }
        for (const auto& feature : features.getFeatures()) {
            auto semanticFeature = model.getFeature(feature.getName());
            if (model.getDefaultFeatureFunction(*npc(semanticFeature)) != nullptr) {
                semanticFeatures.push_back(*npc(semanticFeature));
            }
            // else we don't support inflecting with this semantic feature.
        }
        if (!hasDisplayFunction && semanticFeatures.empty()) {
            // Don't bother. There is nothing to inflect.
            continue;
        }

        // Random data to execute stuff outside of the dictionary.
        phrases.emplace_back(u"AAHHHHHH");
        phrases.emplace_back(u"🙂s");

        auto dictionary = ::morphuntion::dictionary::DictionaryMetaData::createDictionary(locale);
        int32_t wordCount = npc(dictionary)->getKnownWordsSize();
        auto dictionaryEnd = npc(dictionary)->getKnownWords().end();
        int32_t skipAmount = wordCount / minimumNumberOfWords; // Every nth entry in the dictionary
        wordCount = 0;
        for (auto dictionaryIter = npc(dictionary)->getKnownWords(); dictionaryIter != dictionaryEnd; ++dictionaryIter) {
            wordCount++;
            if (skipAmount > 0 && wordCount % skipAmount != 0) {
                continue;
            }
            phrases.emplace_back(*dictionaryIter);
        }

        for (int32_t count = 0; count < processorCount; count++) {
            results[count].reserve(phrases.size());
            threads.emplace_back(testInflection, &model, phrases, constraints, semanticFeatures, count, results + count);
        }

        // Wait for all the threads to finish.
        for (auto& thread : threads) {
            thread.join();
            // Wait for other threads to finish.
            std::this_thread::yield();
        }

        // In all the threads that just finished, all the results should be precisely the same.
        for (int32_t threadIdx = 1; threadIdx < processorCount; threadIdx++) {
            INFO("Thread=" + std::to_string(threadIdx));
            int32_t resultsSize = (int32_t)results[0].size();
            REQUIRE(resultsSize > 0);
            REQUIRE((int32_t)results[threadIdx].size() == resultsSize);
            for (int32_t resultIdx = 0; resultIdx < resultsSize; resultIdx++) {
                const auto result = results[threadIdx].at(resultIdx);
                const auto expected = results[0].at(resultIdx);
                if (*npc(result) != *npc(expected)) {
                    INFO(std::string("Failure at index ") + std::to_string(resultIdx));
                    FAIL(morphuntion::util::StringUtils::to_string(result->toString()) + "!=" + morphuntion::util::StringUtils::to_string(expected->toString()));
                }
            }
        }

        for (int32_t threadIdx = 0; threadIdx < processorCount; threadIdx++) {
            for (auto result : results[threadIdx]) {
                delete result;
            }
            results[threadIdx].clear();
        }
    }
    delete[] results;
}
