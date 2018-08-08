
#ifndef FORMATTERS_20160922_H_
#define FORMATTERS_20160922_H_

#include <cstddef>
#include <cstring>
#include <iterator>

namespace cxx17 {

// This context writes to a buffer
struct buffer_writer  {

	buffer_writer(char *buffer, size_t size) : ptr_(buffer), size_(size) {
	}

	void write(char ch) noexcept {
		if(size_ > 1) {
			*ptr_++ = ch;
			--size_;
		}
		++written;
	}
	
	void write(const char *p, size_t n) {
		size_t count = std::min(size_, n);
		std::memcpy(ptr_, p, count);
		size_   -= count;
		written += count;
	}

	void done() noexcept {
		if(size_ != 0) {
			*ptr_ = '\0';
		}
	}

	char  *ptr_;
	size_t size_;
	size_t written = 0;
};

// This context writes to a container using a std::back_inserter
struct ostream_writer {

	ostream_writer(std::ostream &os) : os_(os) {
	}

	void write(char ch) {
		os_.put(ch);
		++written;
	}
	
	void write(const char *p, size_t n) {
		os_.write(p, n);
		written+= n;
	}
	
	void done() noexcept {}

	std::ostream &os_;
	size_t written = 0;
};

// This context writes to a container using a std::back_inserter
template <class C>
struct container_writer {

	container_writer(C &s) : it_(std::back_inserter(s)) {
	}

	void write(char ch) {
		*it_++ = ch;
		++written;
	}
	
	void write(const char *p, size_t n) {
		while(n--) {
			write(*p++);
		}
	}
	
	void done() noexcept {}

	std::back_insert_iterator<C> it_;
	size_t written = 0;
};

// this context writes to an STDIO stream
struct stdio_writer {

	stdio_writer(FILE *stream) : stream_(stream) {
	}

	void write(char ch) noexcept {
		putc(ch, stream_);
		++written;
	}
	
	void write(const char *p, size_t n) {
		written += fwrite(p, n, 1, stream_);
	}
	
	void done() noexcept {}

	FILE *stream_;
	size_t written = 0;
};

// this context writes to the stdout stream
struct stdout_writer {

	void write(char ch) noexcept {
		putchar(ch);
		++written;
	}
	
	void write(const char *p, size_t n) {
		written += fwrite(p, n, 1, stdout);
	}
	
	void done() noexcept {}
	
	size_t written = 0;
};

}

#endif
