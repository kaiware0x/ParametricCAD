#ifndef YES_NO_H
#define YES_NO_H

/**
 * @brief bool型の可読性を強化するヘルパー構造体
 * ref: https://zenn.dev/reputeless/articles/cpp-article-yesno
 */
template <class Tag>
struct YesNo {
private:
    bool m_yesNo;

    struct Helper {
        bool yesNo;
    };

public:
    [[nodiscard]] YesNo() = default;

    [[nodiscard]] explicit constexpr YesNo(bool yesNo) noexcept : m_yesNo{yesNo} {}

    [[nodiscard]] constexpr YesNo(Helper helper) noexcept : m_yesNo{helper.yesNo} {}

    [[nodiscard]] explicit constexpr operator bool() const noexcept { return m_yesNo; }

    [[nodiscard]] constexpr bool getBool() const noexcept { return m_yesNo; }

    /// NOTE: C++20からは三方演算子<=>で、比較演算子を一括定義できる.
    constexpr bool operator==(const YesNo& yesno) const noexcept { return m_yesNo == yesno; }
    constexpr bool operator!=(const YesNo& yesno) const noexcept { return m_yesNo != yesno; }
    constexpr bool operator!() const noexcept { return !m_yesNo; }

    //    constexpr bool operator<=(const YesNo& yesno) const noexcept { return m_yesNo <= yesno; }
    //    constexpr bool operator>=(const YesNo& yesno) const noexcept { return m_yesNo >= yesno; }
    //    constexpr bool operator<(const YesNo& yesno) const noexcept { return m_yesNo < yesno; }
    //    constexpr bool operator>(const YesNo& yesno) const noexcept { return m_yesNo > yesno; }

    static constexpr Helper Yes{true};

    static constexpr Helper No{false};
};

using MakeChildLabel = YesNo<struct MakeChildLabel_tag>;

#endif    // YES_NO_H
