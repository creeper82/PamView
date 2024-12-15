#pragma once
#include <exception>
#include <string>

class bad_dimensions_exception : public std::exception
{
private:
    std::string message;

public:
    bad_dimensions_exception(const char *msg) : message(msg) {}
    const char *what() const throw()
    {
        return message.c_str();
    }
};

class too_large_exception : public std::exception
{
private:
    std::string message;

public:
    too_large_exception(const char *msg) : message(msg) {}
    const char *what() const throw()
    {
        return message.c_str();
    }
};

class unsupported_format_exception : public std::exception
{
private:
    std::string message;

public:
    unsupported_format_exception(const char *msg) : message(msg) {}
    const char *what() const throw()
    {
        return message.c_str();
    }
};

class unsupported_maxvalue_exception : public std::exception
{
private:
    std::string message;

public:
    unsupported_maxvalue_exception(const char *msg) : message(msg) {}
    const char *what() const throw()
    {
        return message.c_str();
    }
};

class no_bitmap_open_exception : public std::exception
{
private:
    std::string message;

public:
    no_bitmap_open_exception(const char *msg) : message(msg) {}
    const char *what() const throw()
    {
        return message.c_str();
    }
};

class stream_corrupt_exception : public std::exception
{
private:
    std::string message;

public:
    stream_corrupt_exception(const char *msg, bool _isEof = false) : message(msg), isEof(_isEof) {}
    const char *what() const throw()
    {
        return message.c_str();
    }

    bool isEof;
};