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

	template<RealNumber Type, std::size_t N>
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
		[[nodiscard]] constexpr ValueType& get(SizeType index) noexcept
		{
			return this->data_[index];
		}

		[[nodiscard]] constexpr const ValueType& get(SizeType index) const noexcept
		{
			return this->data_[index];
		}

		[[nodiscard]] constexpr ValueType& x() noexcept
			requires (N > 0)
		{
			return this->data_[0];
		}

		[[nodiscard]] constexpr ValueType& y() noexcept
			requires (N > 1)
		{
			return this->data_[1];
		}

		[[nodiscard]] constexpr ValueType& z() noexcept
			requires (N > 2)
		{
			return this->data_[2];
		}

		[[nodiscard]] constexpr ValueType& w() noexcept
			requires (N > 3)
		{
			return this->data_[3];
		}

		[[nodiscard]] constexpr const ValueType& x() const noexcept
			requires (N > 0)
		{
			return this->data_[0];
		}

		[[nodiscard]] constexpr const ValueType& y() const noexcept
			requires (N > 1)
		{
			return this->data_[1];
		}

		[[nodiscard]] constexpr const ValueType& z() const noexcept
			requires (N > 2)
		{
			return this->data_[2];
		}

		[[nodiscard]] constexpr const ValueType& w() const noexcept
			requires (N > 3)
		{
			return this->data_[3];
		}

	public:
		[[nodiscard]] constexpr ValueType& operator [] (SizeType index) noexcept
		{
			return this->data_[index];
		}

		[[nodiscard]] constexpr const ValueType& operator [] (SizeType index) const noexcept
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
			for (SizeType index{}; index < N; ++index)
			{
				this->data_[index] /= num;
			}

			return *this;
		}

		[[nodiscard]] constexpr bool operator == (const Vec& other) const noexcept
		{
			return this->data_ == other.data_;
		}

	private:


	private:
		std::array<ValueType, N> data_{};
	};

	template <RealNumber T, std::size_t N>
	[[nodiscard]] constexpr Vec<T, N> operator + (Vec<T, N> lhs, const Vec<T, N>& rhs) noexcept
	{
		lhs += rhs;

		return lhs;
	}

	template <RealNumber T, std::size_t N>
	[[nodiscard]] constexpr Vec<T, N> operator - (Vec<T, N> lhs, const Vec<T, N>& rhs) noexcept
	{
		lhs -= rhs;

		return lhs;
	}

	template <RealNumber T, std::size_t N>
	[[nodiscard]] constexpr Vec<T, N> operator - (const Vec<T, N>& vec) noexcept
	{
		Vec<T, N, P> temp{};

		temp -= vec;

		return temp;
	}

	template <RealNumber T, std::size_t N, RealNumber Num>
	[[nodiscard]] constexpr Vec<T, N> operator * (Vec<T, N> vec, Num num)
	{
		vec *= num;

		return vec;
	}

	template <RealNumber T, std::size_t N, RealNumber Num>
	[[nodiscard]] constexpr Vec<T, N> operator * (Num num, Vec<T, N> vec)
	{
		vec *= num;

		return vec;
	}

	template <RealNumber T, std::size_t N, RealNumber Num>
	[[nodiscard]] constexpr Vec<T, N> operator / (Vec<T, N> vec, Num num)
	{
		vec /= num;

		return vec;
	}

	template <RealNumber T, std::size_t N>
	std::ostream& operator << (std::ostream& os, const Vec<T, N>& vec)
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