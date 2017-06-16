//
// Copyright (c) 2017, RISUWWV (https://github.com/risuwwv). All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//

//g++ -std=c++1z -O3 -Wall -Wextra -pedantic constexprTransform.cpp -o test.exe

#include <type_traits>
#include <tuple>
#include <array>
#include <experimental/array>

//using variadic template to avoid doing 
//std::array<std::result_of_t<F(T)>, N> res{0};
//and a loop, which would assume zero initializable type

template<typename T, std::size_t N, typename F, std::size_t... Is>
constexpr auto transform(std::array<T, N> array, F f, std::index_sequence<Is...>) 
{
    return std::experimental::make_array(f(std::get<Is>(array))...);
}

template <typename T, std::size_t N, typename F>
constexpr auto transform(std::array<T, N> array, F f) 
{
  return transform(array, f, std::make_index_sequence<N>{}); 
}

template <typename T, std::size_t N>
constexpr bool equals(std::array<T, N> array1, std::array<T, N> array2)
{
  for(std::size_t i = 0; i < N; ++i)
  {
      if(array1[i] != array2[i])
      {
          return false;
      }
  }
  return true;
}

int main()
{
    constexpr std::array<int, 4> ints{{1, 2, 3, 4}};
    constexpr std::array<int, 4> facts = transform(ints, 
											[](int n)
											{
												int res = 1;
												for(int i = 2; i <= n; ++i)
												{
													res *= i;
												}
												return res;        
											});
	
    static_assert(equals(facts, std::array<int, 4>{{1, 2, 6, 24}}));
}