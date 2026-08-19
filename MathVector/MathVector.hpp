#pragma once

#include <iostream>
#include <concepts>
#include <type_traits>
#include <cstddef>
#include <array>
#include <cassert>
#include <stdexcept>
#include <utility>

namespace Math {
	template <typename T>
	concept RealNumber =
		(std::integral<T> || std::floating_point<T>) &&
		!std::same_as<T, bool> &&
		!std::same_as<T, char> &&
		!std::same_as<T, wchar_t>;

	struct GamePolicy
	{
		template <typename T>
		static void DivisionByZero(T denominator)
		{
			assert(denominator != T{});
		}
	};

	struct PrecisionPolicy
	{
		template <typename T>
		static void DivisionByZero(T denominator)
		{
			if (denominator == T{})
			{
				throw std::domain_error("Division by zero.");
			}
		}
	};

	template<RealNumber Type, std::size_t N, typename Policy = GamePolicy>
	class Vec
	{
	public:
		using ValueType = Type;
		using SizeType = std::size_t;

	public:
		constexpr Vec() noexcept
			: data_{}
		{}

		template <typename... Args>
		requires (sizeof...(Args) == N && (std::same_as<std::remove_cvref_t<Args>, ValueType> && ...))
		constexpr Vec(Args&&... args) noexcept
			: data_{std::forward<Args>(args)...}
		{}

	public:
		constexpr ValueType& operator [] (SizeType index)
		{
			return this->data_[index];
		}

		constexpr const ValueType& operator [] (SizeType index) const
		{
			return this->data_[index];
		}

		constexpr Vec& operator += (const Vec& other) noexcept
		{
			for (SizeType index{}; index < N; ++index)
			{
				this->data_[index] += other.data_[index];
			}

			return *this;
		}

		constexpr Vec& operator -= (const Vec& other) noexcept
		{
			for (SizeType index{}; index < N; ++index)
			{
				this->data_[index] -= other.data_[index];
			}

			return *this;
		}

		template <RealNumber Num>
		constexpr Vec& operator *= (Num num)
		{
			for (SizeType index{}; index < N; ++index)
			{
				this->data_[index] *= num;
			}

			return *this;
		}

		template <RealNumber Num>
		constexpr Vec& operator /= (Num num)
		{
			Policy::DivisionByZero(num);

			for (SizeType index{}; index < N; ++index)
			{
				this->data_[index] /= num;
			}

			return *this;
		}

		constexpr bool operator == (const Vec& other) const noexcept
		{
			return this->data_ == other.data_;
		}

	private:
		std::array<ValueType, N> data_{};
	};

	template <RealNumber T, std::size_t N, typename P>
	constexpr Vec<T, N, P> operator + (Vec<T, N, P> lhs, const Vec<T, N, P>& rhs) noexcept
	{
		lhs += rhs;

		return lhs;
	}

	template <RealNumber T, std::size_t N, typename P>
	constexpr Vec<T, N, P> operator - (Vec<T, N, P> lhs, const Vec<T, N, P>& rhs) noexcept
	{
		lhs -= rhs;

		return lhs;
	}

	template <RealNumber T, std::size_t N, typename P>
	constexpr Vec<T, N, P> operator - (const Vec<T, N, P>& vec) noexcept
	{
		Vec<T, N, P> temp{};

		temp -= vec;

		return temp;
	}

	template <RealNumber T, std::size_t N, typename P>
	constexpr Vec<T, N, P> operator * (Vec<T, N, P> vec, T num)
	{
		vec *= num;

		return vec;
	}

	template <RealNumber T, std::size_t N, typename P>
	constexpr Vec<T, N, P> operator * (T num, Vec<T, N, P> vec)
	{
		vec *= num;

		return vec;
	}

	template <RealNumber T, std::size_t N, typename P>
	constexpr Vec<T, N, P> operator / (Vec<T, N, P> vec, T num)
	{
		vec /= num;

		return vec;
	}

	template <RealNumber T, std::size_t N, typename Policy>
	std::ostream& operator << (std::ostream& os, const Vec<T, N, Policy>& vec)
	{
		os << "[";

		for (std::size_t index{}; index < N; ++index)
		{
			os << vec[index];

			if (index + 1 < N) {
				os << ", ";
			}
		}

		os << "]";

		return os;
	}
}