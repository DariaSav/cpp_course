#include<string>
#include<vector>
#include<algorithm>


template<typename Func>
auto Compose(Func f) {
	return f;
}


template<typename Fhead, typename...Ftails> 
auto Compose(Fhead fhead, Ftails ...ftails) {
	return [=](auto x){ return fhead(Compose(ftails ...)(x));};
}

const char* f2(const std::string& str) {
	return str.c_str();
}


int main() {
	std::string s[] = {"1.2", "2.343", "3.2"};
	std::vector<double> d(3);
	auto f1 = atof;
	std::transform(s,
		s + 3,
		d.begin(),
		Compose(f1, f2));
}
