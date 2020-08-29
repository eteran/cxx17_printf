
#include "Printf.h"

#include <chrono>
#include <iostream>

template <class R, int Count, class F>
R time_code(F func) {

	// test the timing of our printf
	auto then = std::chrono::system_clock::now();

	for (int i = 0; i < Count; ++i) {
		func();
	}

	auto now = std::chrono::system_clock::now();
	auto dur = now - then;

	return std::chrono::duration_cast<R>(dur);
}

#ifdef CXX17_PRINTF_EXTENSIONS
class Test {};

std::string to_string(Test) {
	return "Test!";
}
#endif

int main() {
	int Foo = 1234;

	// first test correctness
	cxx17::printf("hello %*s, %c, %d, %08x %p %016u %02x %016o\n", 10, "world", 0x41, -123, 0x1234, static_cast<void *>(&Foo), -4, -1, 64);
	       printf("hello %*s, %c, %d, %08x %p %016u %02x %016o\n", 10, "world", 0x41, -123, 0x1234, static_cast<void *>(&Foo), -4, -1, 64);

	typedef std::chrono::microseconds ms;

	constexpr int count = 800000;

	auto time1 = time_code<ms, count>([&Foo]() {
		char buf[128];
		cxx17::sprintf(buf, "hello %*s, %c, %d, %08x %p %016u %02x %016o\n", 10, "world", 0x41, -123, 0x1234, static_cast<void *>(&Foo), -4, -1, 1234);
	});

	auto time2 = time_code<ms, count>([&Foo]() {
		char buf[128];
		snprintf(buf, sizeof(buf), "hello %*s, %c, %d, %08x %p %016u %02x %016o\n", 10, "world", 0x41, -123, 0x1234, static_cast<void *>(&Foo), -4, -1, 1234);
	});

	std::cerr << "cxx17 Took:  " << time1.count() << " \xC2\xB5s to execute." << std::endl;
	std::cerr << "printf Took: " << time2.count() << " \xC2\xB5s to execute." << std::endl;

#ifdef CXX17_PRINTF_EXTENSIONS
	{
		std::string s = "[std::string]!";
		cxx17::sprintf(std::cout, "hello %10? %?\n", s, Test());

		cxx17::printf("%032b\n", 5ul);
		cxx17::printf("%032b\n", 1234ul);
	}
#endif
}
