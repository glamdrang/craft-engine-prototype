
/* T:
See these tutorials:
* http://eli.thegreenplace.net/2014/variadic-templates-in-c/
*/

/******************************************************************************
** Detection Templates
******************************************************************************/

// From http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/n4502.pdf
// The *_v methods appear to crash MSVC
// Also:
//   * http://stackoverflow.com/a/30860338
//   * http://stackoverflow.com/a/30848101

inline namespace _t
{
	template< class... >
	using
		void_t = void;


	// `detect`

	template < class, template<class> class, class = void_t< > >
	struct
		detect
		: std::false_type { };
	template < class T, template<class> class Op>
	struct
		detect< T, Op, void_t<Op<T>> >
		: std::true_type { };

	// `detector`

	template 
		< class Default
		, class // Always void
		, template<class...> class Op
		, class... Args
		>
	struct
		detector
	{
		using value_t = std::false_type;
		using type = Default;
	};

	template
	< class Default
		, template<class...> class Op
		, class... Args
	>
	struct
		detector< Default, void_t< Op<Args...> >, Op, Args...>
	{
		using value_t = std::true_type;
		using type = Op<Args...>;
	};


	// `nonesuch`

	struct
		nonesuch
	{
		nonesuch() = delete;
		~nonesuch() = delete;
		nonesuch(nonesuch const&) = delete;
		void 
			operator = (nonesuch const&) = delete;
	};


	// `is_detected`

	template< template<class...> class Op, class... Args >
	using
		is_detected = typename detector<nonesuch, void, Op, Args...>::value_t;

	/*
	template< template<class...> class Op, class... Args >
	constexpr bool
		is_detected_v = is_detected<Op, Args...>::value;
	*/


	// `detected_t`

	template< template<class...> class Op, class... Args >
	using
		detected_t = typename detector<nonesuch, void, Op, Args...>::type;


	// `detected_or`

	template< class Default, template<class...> class Op, class... Args >
	using
		detected_or = detector<Default, void, Op, Args...>;

	template< class Default, template<class...> class Op, class... Args >
	using
		detected_or_t = typename detected_or<Default, Op, Args...>::type;


	// `is_detected_exact`

	template< class Expected, template<class...> class Op, class... Args >
	using
		is_detected_exact = std::is_same< Expected, detected_t<Op, Args...> >;

	/*
	template< class Expected, template<class...> class Op, class... Args >
	constexpr bool
		is_detected_exact_v = is_detected_exact<Expected, Op, Args...>::value;
	*/


	// `is_detected_convertable`

	template< class To, template<class...> class Op, class... Args >
	using
		is_detected_convertable = std::is_convertible< detected_t<Op, Args...>, To >;

	/*
	template< class To, template<class...> class Op, class... Args >
	constexpr bool
	is_detected_convertable_v = is_detected_convertable<To, Op, Args...>::value;
	*/
}

/******************************************************************************
** Template Aserrt Helpers
******************************************************************************/

// See http://stackoverflow.com/a/13366183

template <typename Assertion>
struct _T_assertValue
{
	static_assert(Assertion::value, "Assertion failed <see below for more information>");
	static bool const value = Assertion::value;
};

template <typename BadType>
struct _T_assertBad : std::false_type
{
	static_assert(std::is_same<BadType, _t::nonesuch>::value, "Assertion failed bad type <see below for type>");
};


/******************************************************************************
** _T_elem_index
******************************************************************************/

/* T: template class _T_elem_index
This is some template magic for accessing arbitrary type arguments from a template using a number.
*/

template <
	size_t k,
	typename... Ts>
class _T_elemIndex;


template <
	typename T,
	typename... Ts>
class _T_elemIndex<0, T, Ts...> {
public:
	typedef T type;
};

template <
	size_t k,
	typename T,
	typename... Ts>
class _T_elemIndex<k, T, Ts...> {
public:
	typedef typename _T_elemIndex<k - 1, Ts...>::type type;
};

template <
	typename T,
	typename... Ts,
	template <typename...> typename M>
class _T_elemIndex<0, M<T, Ts...>> {
public:
	typedef T type;
};

template <
	size_t k,
	typename T,
	typename... Ts,
	template <typename...> typename M>
class _T_elemIndex<k, M<T, Ts...>> {
public:
	typedef typename _T_elemIndex<k - 1, M<Ts...>>::type type;
};
