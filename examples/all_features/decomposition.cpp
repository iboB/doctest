#include <doctest/doctest.h>

class MoveOnly {
    public:
        MoveOnly(int iIn) : i(iIn) { }
        MoveOnly(MoveOnly&&) = default;
        MoveOnly(const MoveOnly&) = delete;
        MoveOnly& operator=(MoveOnly&&) = default;
        MoveOnly& operator=(const MoveOnly&) = default;
        operator bool() { // NOT const!
            return i == 42;
        }

    private:
        int i;
};

static MoveOnly genType(bool b) {
    return MoveOnly(b ? 42 : 0);
}

TEST_CASE("Move Only Type") {
    CHECK(genType(true));
    CHECK(genType(false));
}


struct int_pointer {
    int* p = nullptr;
    int_pointer() = default;

    // non-const ref used in constructor
    // we don't want to accidentally construct a dangling pointer from a temporary
    int_pointer(int& i) : p(&i) { }

    explicit operator bool() { return !!p; }
    int val() const { return *p; }
};

struct int_holder {
    int i = 0;
    bool operator==(int_pointer p) const {
        if (!p) return false;
        return i == p.val();
    }
};

DOCTEST_MSVC_SUPPRESS_WARNING_WITH_PUSH(4866) // eval order
TEST_CASE("Impl cast from non-const value") {
    int_holder h{ 8 };
    int i = 8;

    // this comparison is safe
    // int_pointer can be implicitly constructed from this int here
    CHECK(h == i);
    CHECK((h == i));
}
DOCTEST_MSVC_SUPPRESS_WARNING_POP
