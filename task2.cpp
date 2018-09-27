class ComplexNumber {
    int re;
    int im;
public:
    constexpr ComplexNumber(int _re=0, int _im=0): re(_re), im(_im) {};
    constexpr void SetRe(int _re) {re=_re;}
    constexpr int GetRe() const {return re;}
    constexpr int GetIm() const {return im;}
    constexpr void SetIm(int _im) {im=_im;}
    friend constexpr bool operator==(const ComplexNumber &a, const ComplexNumber &b);
};


constexpr bool operator==(const ComplexNumber &a, const ComplexNumber &b) {
    return (a.re == b.re) and (b.im == a.im);
}

constexpr ComplexNumber Conjugate(const ComplexNumber& x) {
    ComplexNumber res;
    res.SetRe(x.GetRe());
    res.SetIm(-x.GetIm());
    return res;
}


int main() {
    constexpr ComplexNumber a(1, 2);
    constexpr ComplexNumber b(1, -2);
    constexpr auto c = Conjugate(a);
    static_assert(b == c, "failed");
}
