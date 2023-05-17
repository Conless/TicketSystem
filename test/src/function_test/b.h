class B {
 public:
  explicit B(int x) { val_ = x; }
  friend auto Haha(int x) -> B;
  friend auto operator<(const B &lhs, const B &rhs) -> bool;
  int val_;
};

auto Haha(int x) -> B;