#ifndef TPL_INCLUDE_TPL_BASE_HPP
#define TPL_INCLUDE_TPL_BASE_HPP

#include <stdexcept>
#include <tuple>
#include <typeinfo>

namespace tpl {

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float f32;
typedef double f64;

typedef bool b8;
typedef uint32_t b32;

typedef const char *cString;

// template <typename Type> //
// cString typenameStr() noexcept {
//   return typeid(Type).name();
// }

// template <u64 Index, typename... Args> //
// void iterateTuple(std::tuple<Args...> &tp, auto func) {
//   func(std::get<Index>(tp));
//   if constexpr (Index + 1 != sizeof...(Args)) {
//     iterateTuple<Index + 1>(tp, func);
//   }
// }

// template <typename> //
// struct First2ArgTypes;

// template <typename FuncType, typename Arg1Type, typename Arg2Type,
//           typename... Args> //
// struct First2ArgTypes<FuncType(Arg1Type, Arg2Type, Args...)> {
//   using type1 = Arg1Type;
//   using type2 = Arg2Type;
// };

} // namespace tpl

#endif // TPL_INCLUDE_TPL_BASE_HPP