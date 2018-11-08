#include <type_traits>
#include <iostream>


template<class ... TTypes>
struct are_same: public std::false_type {
};

template <class T, class ... TTails>
struct are_same<T, T, TTails ...>: public are_same<T, TTails ...> {
};

template<class T, class U>
struct are_same<T, U>: std::false_type  {
};

template<class T>
struct are_same<T, T>: std::true_type  {
};

template<class T>
struct are_same<T>: std::true_type  {
};

template<class ... TTypes>
constexpr bool are_same_v = are_same<TTypes ...>::value;


int main(int argc, char **argv) {
    are_same<int, int> same2;
    are_same<int, float> notsame2;

    are_same<int, int, int> same3;
    are_same<int, float, int> notsame3;
    are_same<int, int, float> notsame3_2;
    are_same<float, int, int> notsame3_3;

    are_same<int, int, int, int> same4;
    are_same<int, int, int, float> notsame4;

    std::cout << same2 << " " << same3 << " " << same4 << std::endl;
    std::cout << notsame2 << " " << notsame3 << " " << notsame3_2 << " " << notsame3_3 << " "
              << notsame4 << std::endl;

    static_assert(are_same_v<int, int32_t, signed int>, "compile assert");

}