#ifndef UTIL_HPP
#define UTIL_HPP

static inline void ltrim(string &s) {
    s.erase(s.begin(), find_if(s.begin(), s.end(), [](int ch) {
        return !isspace(ch);
    }));
}

static inline void rtrim(string &s) {
    s.erase(find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

static inline void trim(string &s) {
    ltrim(s);
    rtrim(s);
}

static inline string indexed_replace(string str, ...) {
    va_list args;
    va_start(args, str);

    for (int i = 0; ; ++i) {
        regex reg("(\\{" + to_string(i) + "\\})");

        auto it = sregex_iterator(str.begin(), str.end(), reg);
        if (it == sregex_iterator()) {
            break;
        }

        const char * rep = va_arg(args, const char *);
        str = std::regex_replace(str,reg,rep);
    }

    return str;
}

#endif // UTIL_HPP
