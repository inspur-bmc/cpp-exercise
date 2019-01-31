#include <iostream>
#include <tuple>


template <typename T>
struct type_id_downcast
{
	using type = std::remove_cv_t<std::remove_reference_t<T>>;
};

template <typename T>
using type_id_downcast_t = typename type_id_downcast<T>::type;


template <typename S,typename Enable = void>
struct append_single
{
	template <typename T>
	static void op(T&& t){
		std::cout << t << std::endl;
	}
};

template <>
struct append_single<std::string>
{
	template <typename T>
	static void op(T &&t){
		std::cout <<"string(" << t << ")" << std::endl;
	}
};

template <typename T>
using append_single_t = append_single<type_id_downcast_t<T>>;


template <typename T>
void tuple_append_item(T &&t)
{
	std::cout << "tuple_append_item:" << std::endl;
	using itemType = decltype(t);
	append_single_t<itemType>::op(std::forward<itemType>(t));
}

template <int N>
struct AppendHelper
{
	template <typename Tuple>
	static void op(Tuple &&t){
		auto field = std::get<N-1>(t);
		AppendHelper<N-1>::op(std::forward<Tuple>(t));
		tuple_append_item(field);
	}
};

template <>
struct AppendHelper<1>
{
	template <typename Tuple>
	static void op(Tuple && t){
		tuple_append_item(std::get<0>(t));
	}	
};

template <typename Tuple>
void append_tuple(Tuple&& t)
{
	AppendHelper<std::tuple_size<Tuple>::value>::op(std::forward<Tuple>(t));
}

template <typename Tuple,typename... Args>
void append_grouping(Tuple &&t,Args... args);
template <typename Tuple>
void append_grouping(Tuple &&t);

template <typename Tuple>
void append_grouping(Tuple &&t)
{
	append_tuple(std::forward<Tuple>(t));
}

template <typename Tuple,typename... Args>
void append_grouping(Tuple &&t,Args... args)
{
	append_grouping(std::tuple_cat(std::forward<Tuple>(t) ,std::forward_as_tuple(args... )));
}

inline void append(){}

template <typename... Args>
void append(Args... args)
{
	append_grouping(std::make_tuple(),args...);
}

int main()
{
	append(1,2,4,std::string{"hello world"});
}




