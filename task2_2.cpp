template<int n> 
constexpr int Size(int (&a)[n]) {
  return n;
}

int main() {
    int a[5];
    double b[Size(a)];
}
