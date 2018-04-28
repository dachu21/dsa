cmake_minimum_required(VERSION 3.8)
project(DigitalSignatureDSA)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        main.cpp
        BigInt.cpp
        BigInt.hpp
        Primes.hpp
        constants.hpp
        AlgorithmParameters.hpp
        SecretKey.hpp
        PublicKey.hpp
        DigitalSignature.hpp
        Message.hpp
        lib/sha-2/sha2.h
        lib/sha-2/sha2.cpp
        lib/sha-2/hash.h
        )
add_executable(DigitalSignatureDSA ${SOURCE_FILES})