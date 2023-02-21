#include "util.h"

#include <sstream>
#include <iomanip>
#include <stdexcept>


std::string to_string_prec(double d, int precision)
{
    std::stringstream ss;
    ss << std::setprecision(precision) << d;
    return ss.str();
}

bool IsUint(const std::string& str)
{
	if (str.empty()) return false;
	if (str.length() > 1 and str[0] == '0') return false;
	for (auto i = str.begin(); i != str.end(); ++i)
	{
		if (!std::isdigit(*i)) return false;
	}
	return true;
}

bool IsInt(const std::string& str)
{
	auto start = str.begin() + (!str.empty() and str[0] == '-');
	if (start == str.end()) return false;

	if (str.length() > 1 and *start == '0') return false;
	for (auto i = start; i != str.end(); ++i)
	{
		if (!std::isdigit(*i)) return false;
	}
	return true;
}

bool IsUfloat(const std::string& str)
{
	bool dot_used = false;
	if (str.empty()) return false;
	if (str[0] == '0' and str.length() > 1) return false;
	for (auto i = str.begin(); i != str.end(); ++i)
	{
		if (!std::isdigit(*i))
		{
			if (*i == '.')
			{
				if (dot_used or i == str.begin() or i == str.end()) return false;
				else dot_used = true;
			}
			else return false;
		}
	}
	return true;
}

bool IsFloat(const std::string& str)
{
	bool dot_used = false;
	auto start = str.begin() + (!str.empty() and str[0] == '-');
	if (start == str.end()) return false;

	if (*start == '0' and str.length() > 1) return false;
	for (auto i = start; i != str.end(); ++i)
	{
		if (!std::isdigit(*i))
		{
			if (*i == '.')
			{
				if (dot_used or i == start or i == str.end()) return false;
				else dot_used = true;
			}
			else return false;
		}
	}
	return true;
}

std::string map_safe_at(std::map<std::string, std::string>& m, std::string key, bool is_number)
{
	if (is_number)
	{
		if (m.find(key) == m.end()) return "0";

		auto result = m.at(key);
		if (IsFloat(result)) return result;
		else return "0";
	}
	else
	{
		if (m.find(key) != m.end()) return m.at(key);
		else return "";
	}

}

//strtod.c from here https://github.com/gagern/gnulib/blob/master/lib/strtod.c
//edited by me (Givikp120)
//normal strtod or std::stod just doesn't work super strange reasons



/* Return true if C is a space in the current locale, avoiding
   problems with signed char and isspace.  */
static bool
locale_isspace(char c)
{
    unsigned char uc = c;
    return isspace(uc) != 0;
}

#if !HAVE_LDEXP_IN_LIBC
#define ldexp dummy_ldexp
/* A dummy definition that will never be invoked.  */
static double ldexp(double x, int exponent)
{
    abort();
    return 0.0;
}
#endif

/* Return X * BASE**EXPONENT.  Return an extreme value and set errno
   to ERANGE if underflow or overflow occurs.  */
static double
scale_radix_exp(double x, int radix, long int exponent)
{
    /* If RADIX == 10, this code is neither precise nor fast; it is
       merely a straightforward and relatively portable approximation.
       If N == 2, this code is precise on a radix-2 implementation,
       albeit perhaps not fast if ldexp is not in libc.  */

    long int e = exponent;

    if (radix == 2)
        return ldexp(x, e < INT_MIN ? INT_MIN : INT_MAX < e ? INT_MAX : e);
    else
    {
        double r = x;

        if (r != 0)
        {
            if (e < 0)
            {
                while (e++ != 0)
                {
                    r /= radix;
                    if (r == 0 && x != 0)
                    {
                        errno = ERANGE;
                        break;
                    }
                }
            }
            else
            {
                while (e-- != 0)
                {
                    if (r < -DBL_MAX / radix)
                    {
                        errno = ERANGE;
                        return -HUGE_VAL;
                    }
                    else if (DBL_MAX / radix < r)
                    {
                        errno = ERANGE;
                        return HUGE_VAL;
                    }
                    else
                        r *= radix;
                }
            }
        }

        return r;
    }
}


static double
parse_number(const char* nptr,
    int base, int radix, int radix_multiplier, char expchar)
{
    const char* s = nptr;
    bool got_dot = false;
    long int exponent = 0;
    double num = 0;

    for (;; ++s)
    {
        int digit;
        if (isdigit(*s))
            digit = *s - '0';
        else if (base == 16 && isxdigit(*s))
            digit = tolower(*s) - ('a' - 10);
        else if (!got_dot && *s == '.')
        {
            /* Record that we have found the decimal point.  */
            got_dot = true;
            continue;
        }
        else
            /* Any other character terminates the number.  */
            break;

        /* Make sure that multiplication by base will not overflow.  */
        if (num <= DBL_MAX / base)
            num = num * base + digit;
        else
        {
            /* The value of the digit doesn't matter, since we have already
               gotten as many digits as can be represented in a 'double'.
               This doesn't necessarily mean the result will overflow.
               The exponent may reduce it to within range.
               We just need to record that there was another
               digit so that we can multiply by 10 later.  */
            exponent += radix_multiplier;
        }

        /* Keep track of the number of digits after the decimal point.
           If we just divided by base here, we might lose precision.  */
        if (got_dot)
            exponent -= radix_multiplier;
    }

    if (tolower(*s) == expchar && !locale_isspace(s[1]))
    {
        /* Add any given exponent to the implicit one.  */
        int save = errno;
        char* end;
        long int value = strtol(s + 1, &end, 10);
        errno = save;

        if (s + 1 != end)
        {
            /* Skip past the exponent, and add in the implicit exponent,
               resulting in an extreme value on overflow.  */
            s = end;
            exponent =
                (exponent < 0
                    ? (value < LONG_MIN - exponent ? LONG_MIN : exponent + value)
                    : (LONG_MAX - exponent < value ? LONG_MAX : exponent + value));
        }
    }

    return scale_radix_exp(num, radix, exponent);
}

double strtod_edit(const char* nptr)
{
    bool negative = false;

    /* The number so far.  */
    double num = 0;

    const char* s = nptr;

    /* Eat whitespace.  */
    while (locale_isspace(*s))
        ++s;

    /* Get the sign.  */
    negative = *s == '-';
    if (*s == '-' || *s == '+')
        ++s;

    num = parse_number(s, 10, 10, 1, 'e');


    return negative ? -num : num;
}