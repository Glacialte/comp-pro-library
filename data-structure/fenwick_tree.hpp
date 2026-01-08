#include <vector>
#include <concepts>
#include <format>
#include <cassert>

namespace gcl
{
    template <typename T>
    concept ModuleLike =
        std::default_initializable<T> &&
        std::copyable<T> &&
        requires(T a, T b) {
            { a + b } -> std::same_as<T>;
            { a - b } -> std::same_as<T>;
            { a += b } -> std::same_as<T &>;
            { a -= b } -> std::same_as<T &>;
        };

    template <typename T>
        requires ModuleLike<T>
    class FenwickTree
    {
    public:
        FenwickTree(std::size_t size) : n_vals_(size), data_(size + std::size_t{1}) {}

        template <class U>
            requires std::convertible_to<U, T>
        explicit FenwickTree(const std::vector<U> &vec) : FenwickTree(vec.size())
        {
            for (auto i = std::size_t{0}; i < n_vals_; i++)
                add(i, static_cast<T>(vec[i]));
        }

        // add value to idx-th element (0-indexed)
        template <class U>
            requires std::convertible_to<U, T>
        void add(std::size_t idx, U val)
        {
            assert(idx < n_vals_);
            for (auto i = std::size_t{idx + 1}; i <= n_vals_; i += i & -i)
                data_[i] += static_cast<T>(val);
        }

        // return idx-th element
        T get(std::size_t idx)
        {
            assert(idx < n_vals_);
            return sum_prefix(idx + 1) - sum_prefix(idx);
        }

        // return sum [l, r)
        T sum(std::size_t l, std::size_t r)
        {
            assert(l <= r && r <= n_vals_);
            return sum_prefix(r) - sum_prefix(l);
        }

        std::size_t size() const { return n_vals_; }

    private:
        std::vector<T> data_;
        const std::size_t n_vals_;

        // return sum [0, r)
        inline T sum_prefix(std::size_t r)
        {
            T sum{};
            for (auto i = std::size_t{r}; i > 0; i -= i & -i)
                sum += data_[i];
            return sum;
        }
    };
}