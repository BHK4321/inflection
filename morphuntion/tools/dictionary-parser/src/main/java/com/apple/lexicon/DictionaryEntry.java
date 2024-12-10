/*
 * Copyright 2023-2024 Apple Inc. All rights reserved.
 */
package com.apple.lexicon;

import java.util.List;
import java.util.Set;
import java.util.TreeSet;
import java.util.TreeMap;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import javax.annotation.Nonnull;

public class DictionaryEntry implements Comparable<DictionaryEntry> {

    final String phrase;
    private final TreeSet<String> lemmas;
    private final TreeSet<Enum<?>> grammemes;
    private final List<InflectionPattern> inflectionalPatterns;
    private final List<InflectionPattern> rareInflectionalPatterns = new ArrayList<>();

    public static final Comparator<InflectionPattern> INFLECTION_PATTERN_COMPARATOR = Comparator
            .comparing(InflectionPattern::getCount)
            .reversed();

    public DictionaryEntry(String phrase, boolean isLemmaRare, TreeSet<Enum<?>> grammemes, InflectionPattern inflectionalPattern) {
        this(phrase, phrase, isLemmaRare, grammemes, inflectionalPattern);
    }

    public DictionaryEntry(String phrase, String lemmas, boolean isLemmaRare, TreeSet<Enum<?>> grammemes, InflectionPattern inflectionalPattern) {
        this.phrase = phrase;
        this.lemmas = new TreeSet<>(Collections.singleton(lemmas));
        this.grammemes = new TreeSet<>(grammemes.comparator());
        this.grammemes.addAll(grammemes);
        if (inflectionalPattern != null) {
            this.inflectionalPatterns = new ArrayList<>(2);
            this.inflectionalPatterns.add(inflectionalPattern);
            if (isLemmaRare) {
                rareInflectionalPatterns.add(inflectionalPattern);
            }
        }
        else {
            this.inflectionalPatterns = new ArrayList<>();
        }
    }

    public void merge(DictionaryEntry entry) {
        this.grammemes.addAll(entry.grammemes);
        for (InflectionPattern inflectionalPattern : entry.inflectionalPatterns) {
            if (!this.inflectionalPatterns.contains(inflectionalPattern)) {
                this.inflectionalPatterns.add(inflectionalPattern);
            }
        }
        for (InflectionPattern inflectionalPattern : entry.rareInflectionalPatterns) {
            if (!this.rareInflectionalPatterns.contains(inflectionalPattern)) {
                this.rareInflectionalPatterns.add(inflectionalPattern);
            }
        }
        this.lemmas.addAll(entry.lemmas);
    }

    public Set<Enum<?>> getGrammemes() {
        return grammemes;
    }

    @Override
    public String toString() {
        return this.toString(true, false);
    }

    public String toString(boolean isInflectional, boolean withLemmaForm) {
        StringBuilder sb = new StringBuilder(256);
        sb.append(phrase).append(':');
        for (Enum<?> enumVal : grammemes) {
            sb.append(' ').append(enumVal);
        }
        if (isInflectional) {
            if (inflectionalPatterns.size() > 1) {
                inflectionalPatterns.sort(INFLECTION_PATTERN_COMPARATOR);
                if (rareInflectionalPatterns.size() > 1) {
                    rareInflectionalPatterns.sort(INFLECTION_PATTERN_COMPARATOR);
                }
            }
            for (InflectionPattern inflectionPattern : inflectionalPatterns) {
                if (!rareInflectionalPatterns.contains(inflectionPattern)) {
                    sb.append(" inflection=").append(inflectionPattern.getID());
                }
            }
            for (InflectionPattern inflectionPattern : rareInflectionalPatterns) {
                sb.append(" inflection=").append(inflectionPattern.getID());
            }
        }
        if (withLemmaForm) {
            for (String lemmaForm : lemmas) {
                if (!lemmaForm.equals(this.phrase)) {
                    sb.append(" lemma=").append(lemmaForm);
                }
            }
        }
        return sb.toString();
    }

    @Override
    public int compareTo(@Nonnull DictionaryEntry o) {
        return phrase.compareTo(o.phrase);
    }
}