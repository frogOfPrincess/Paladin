//
//  macro.hpp
//  Paladin
//
//  Created by SATAN_Z on 2019/6/30.
//  Copyright © 2019 Zero. All rights reserved.
//

#ifndef macro_hpp
#define macro_hpp

#include "core/header.h"

#define CONSTEXPR constexpr

//#define OUTPUT_LINENO 1

#ifdef OUTPUT_LINENO
#define COUT std::cout << __FILE__ << " " << __LINE__ << std::endl
#else
#define COUT std::cout
#endif

#define USING_PALADIN using namespace paladin;
#define USING_STD using namespace std;

#define PALADIN_BEGIN namespace paladin {
#define PALADIN_END }

#ifdef FLOAT_AS_DOUBLE
    typedef double Float;
#else
    typedef float Float;
#endif

#define PALADIN_HAVE_POSIX_MEMALIGN

#define PALADIN_HAVE_CONSTEXPR

#ifdef _MSC_VER
    #define PALADIN_NO_INLINE __declspec(noinline)
#else
    #define PALADIN_NO_INLINE __attribute__((noinline))
#endif

#define PALADIN_INLINE __attribute__((always_inline))

#if defined(_MSC_VER)
    #define PALADIN_HAVE_ALIGNED_MALLOC
#endif

#ifndef PALADIN_L1_CACHE_LINE_SIZE
    #define PALADIN_L1_CACHE_LINE_SIZE 64
#endif

#ifndef PALADIN_HAVE_ALIGNOF
    #define PALADIN_HAVE_ALIGNOF 16
#endif

#if defined(_WIN32) || defined(_WIN64)
    #define PALADIN_IS_WINDOWS
#endif

#define PALADIN_HAVE_HEX_FP_CONSTANTS

#ifndef PALADIN_HAVE_HEX_FP_CONSTANTS
static const double DoubleOneMinusEpsilon = 0.99999999999999989;
static const float FloatOneMinusEpsilon = 0.99999994;
#elif _MSC_VER
static const double DoubleOneMinusEpsilon = 0.99999999999999989;
static const float FloatOneMinusEpsilon = 0.99999994;
#else
static const double DoubleOneMinusEpsilon = 0x1.fffffffffffffp-1;
static const float FloatOneMinusEpsilon = 0x1.fffffep-1;
#endif

#ifdef PALADIN_FLOAT_AS_DOUBLE
static const Float OneMinusEpsilon = DoubleOneMinusEpsilon;
#else
static const Float OneMinusEpsilon = FloatOneMinusEpsilon;
#endif

static CONSTEXPR Float MaxFloat = std::numeric_limits<Float>::max();
static CONSTEXPR Float Infinity = std::numeric_limits<Float>::infinity();
static CONSTEXPR Float MachineEpsilon = std::numeric_limits<Float>::epsilon() * 0.5;
static CONSTEXPR Float Epsilon = std::numeric_limits<Float>::epsilon();
static CONSTEXPR Float ShadowEpsilon = 0.0001f;
static CONSTEXPR Float Pi = 3.14159265358979323846;
static CONSTEXPR Float _2Pi = 3.14159265358979323846 * 2;
static CONSTEXPR Float InvPi = 0.31830988618379067154;
static CONSTEXPR Float Inv2Pi = 0.15915494309189533577;
static CONSTEXPR Float Inv4Pi = 0.07957747154594766788;
static CONSTEXPR Float PiOver2 = 1.57079632679489661923;
static CONSTEXPR Float PiOver4 = 0.78539816339744830961;
static CONSTEXPR Float Sqrt2 = 1.41421356237309504880;



#endif /* macro_h */




