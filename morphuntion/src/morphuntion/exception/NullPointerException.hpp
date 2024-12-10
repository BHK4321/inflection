/*
 * Copyright 2016-2024 Apple Inc. All rights reserved.
 */
#pragma once

#include <morphuntion/exception/fwd.hpp>
#include <morphuntion/exception/RuntimeException.hpp>

/**
 * @brief Used to indicate a pointer is null after a null pointer check.
 * @details This exception can be thrown in **many** places in Morphuntion. Usually this exception is thrown when Morphun
 * gets into a state where it's expecting a non-null pointer and it has a null pointer instead.
 */
class MORPHUNTION_CLASS_API morphuntion::exception::NullPointerException
    : public morphuntion::exception::RuntimeException
{
public:
    typedef ::morphuntion::exception::RuntimeException super;

public:
    /**
     * Default constructor
     */
    NullPointerException();
    /**
     * Construct an exception with an informative message for debugging purposes.
     */
    explicit NullPointerException(const std::u16string& message);
    /**
     * Destructor
     */
    ~NullPointerException() override;
};
