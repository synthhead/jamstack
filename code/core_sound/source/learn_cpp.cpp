// // * constexpr if
// // Write code that is instantiated depending on a compile-time condition.
// template <typename T>
// constexpr bool isIntegral()
// {
//   if constexpr(std::is_integral<T>::value)
//     return true;
//   else
//     return false;
// }
// static_assert(isIntegral<int>() == true);

// //* constexpr lambda
// // Compile-time lambdas using `constexpr`.
// constexpr auto add = [](int x, int y) {
//   auto L = [=] { return x; };
//   auto R = [=] { return y; };
//   return [=] { return L() + R(); };
// };
// static_assert(add(1, 2)() == 3);
