#ifndef TYPES_H_INCLUDED
#define TYPES_H_INCLUDED

#include <cstdint>

//
//  Integer types
//

typedef std::int8_t   i8;
typedef std::int16_t  i16;
typedef std::int32_t  i32;
typedef std::int64_t  i64;
typedef std::uint8_t  ui8;
typedef std::uint16_t ui16;
typedef std::uint32_t ui32;
typedef std::uint64_t ui64;

//
// Chess types
//

using PositionHash = int;
using Direction    = int;
using Location     = int;
using U64          = unsigned long long;

#endif