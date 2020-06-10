#ifndef NOON_MACROS_HPP
#define NOON_MACROS_HPP

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
    TypeName(const TypeName&) = delete;   \
    TypeName& operator=(const TypeName&) = delete;

#define UNUSED(V) (void)(V)

#endif // NOON_MACROS_HPP