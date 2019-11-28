#ifndef ANY_H
#define ANY_H

#include <stdlib.h>
#include <stdbool.h>

typedef struct any any;

enum _any_type {
    TYPE_CHAR,
    TYPE_UCHAR,
    TYPE_INT,
    TYPE_UINT,
    TYPE_SHORT,
    TYPE_USHORT,
    TYPE_LONG,
    TYPE_ULONG,
    TYPE_LLONG,
    TYPE_ULLONG,
    TYPE_FLOAT,
    TYPE_DOUBLE,
    TYPE_LDOUBLE,
    TYPE_POINTER,
    TYPE_NONE
};

struct any {
    enum _any_type type;
    union {
        char _char;
        unsigned char _uchar;
        int _int;
        unsigned int _uint;
        short _short;
        unsigned short _ushort;
        long _long;
        unsigned long _ulong;
        long long _llong;
        unsigned long long _ullong;
        float _float;
        double _double;
        long double _ldouble;
        void * _pointer;
    };
};

static inline bool any_is_none(const any * _any) {
    return _any->type == TYPE_NONE;
}

/*
 * getters
 */

static inline char any_get_char(const any * _any) {
    return _any->_char;
}

static inline unsigned char any_get_uchar(const any * _any) {
    return _any->_uchar;
}

static inline int any_get_int(const any * _any) {
    return _any->_int;
}

static inline unsigned int any_get_uint(const any * _any) {
    return _any->_uint;
}

static inline short any_get_short(const any * _any) {
    return _any->_short;
}

static inline unsigned short any_get_ushort(const any * _any) {
    return _any->_ushort;
}

static inline long any_get_long(const any * _any) {
    return _any->_long;
}

static inline unsigned long any_get_ulong(const any * _any) {
    return _any->_ulong;
}

static inline long long any_get_llong(const any * _any) {
    return _any->_llong;
}

static inline unsigned long long any_get_ullong(const any * _any) {
    return _any->_ullong;
}

static inline float any_get_float(const any * _any) {
    return _any->_float;
}

static inline double any_get_double(const any * _any) {
    return _any->_double;
}

static inline long double any_get_ldouble(const any * _any) {
    return _any->_ldouble;
}

static inline void * any_get_pointer(const any * _any) {
    return _any->_pointer;
}

/*
 * setters
 */
static inline void any_set_char(any * _any, char _char) {
    _any->type = TYPE_CHAR;
    _any->_char = _char;
}

static inline void any_set_uchar(any * _any, unsigned char _uchar) {
    _any->type = TYPE_UCHAR;
    _any->_uchar = _uchar;
}

static inline void any_set_int(any * _any, int _int) {
    _any->type = TYPE_INT;
    _any->_int = _int;
}

static inline void any_set_uint(any * _any, unsigned int _uint) {
    _any->type = TYPE_UINT;
    _any->_uint = _uint;
}

static inline void any_set_short(any * _any, short _short) {
    _any->type = TYPE_SHORT;
    _any->_uint = _short;
}

static inline void any_set_ushort(any * _any, unsigned short _ushort) {
    _any->type = TYPE_USHORT;
    _any->_uint = _ushort;
}

static inline void any_set_long(any * _any, long _long) {
    _any->type = TYPE_LONG;
    _any->_long = _long;
}

static inline void any_set_ulong(any * _any, unsigned long _ulong) {
    _any->type = TYPE_ULONG;
    _any->_ulong = _ulong;
}

static inline void any_set_llong(any * _any, long long _llong) {
    _any->type = TYPE_LLONG;
    _any->_llong = _llong;
}

static inline void any_set_ullong(any * _any, unsigned long long _ullong) {
    _any->type = TYPE_ULLONG;
    _any->_ullong = _ullong;
}

static inline void any_set_float(any * _any, float _float) {
    _any->type = TYPE_FLOAT;
    _any->_float = _float;
}

static inline void any_set_double(any * _any, double _double) {
    _any->type = TYPE_DOUBLE;
    _any->_double = _double;
}

static inline void any_set_ldouble(any * _any, long double _ldouble) {
    _any->type = TYPE_LDOUBLE;
    _any->_ldouble = _ldouble;
}

static inline void any_set_pointer(any * _any, void * _pointer) {
    _any->type = TYPE_POINTER;
    _any->_pointer = _pointer;
}
/*
static inline void any_set_object(any * _any, const void * src, size_t size) {
    _any->type = TYPE_OBJECT;
    any_object * obj = (any_object*)malloc(sizeof(any_object) + size);
    obj->size = size;
    memcpy(obj->data, src, size);
    _any->data._pointer = obj;
}

static inline void any_clear_object(any * _any) {
    if (_any->type == TYPE_OBJECT) {
        free(_any->data._pointer);
    }
*/

static inline void any_get(const any * _any, void * val) {
    switch (_any->type) {
        case TYPE_CHAR:    *(char*)val = _any->_char;                 break;
        case TYPE_UCHAR:   *(unsigned char*)val = _any->_uchar;       break;
        case TYPE_INT:     *(int*)val = _any->_int;                   break;
        case TYPE_UINT:    *(unsigned int*)val = _any->_uint;         break;
        case TYPE_SHORT:   *(short*)val = _any->_short;               break;
        case TYPE_USHORT:  *(unsigned short*)val = _any->_ushort;     break;
        case TYPE_LONG:    *(long*)val = _any->_long;                 break;
        case TYPE_ULONG:   *(unsigned long*)val = _any->_ulong;       break;
        case TYPE_LLONG:   *(long long*)val = _any->_llong;           break;
        case TYPE_ULLONG:  *(unsigned long long*)val = _any->_ullong; break;
        case TYPE_FLOAT:   *(float*)val = _any->_float;               break;
        case TYPE_DOUBLE:  *(double*) val = _any->_double;            break;
        case TYPE_LDOUBLE: *(long double*)val = _any->_ldouble;       break;
        case TYPE_POINTER: *(void**)val = _any->_pointer;             break;
        default:           return;
    }
}

static inline void * any_get_ref(const any * _any) {
    switch (_any->type) {
        case TYPE_CHAR:    return (void*)&_any->_char;
        case TYPE_UCHAR:   return (void*)&_any->_uchar;
        case TYPE_INT:     return (void*)&_any->_int;
        case TYPE_UINT:    return (void*)&_any->_uint;
        case TYPE_SHORT:   return (void*)&_any->_short;
        case TYPE_USHORT:  return (void*)&_any->_ushort;
        case TYPE_LONG:    return (void*)&_any->_long;
        case TYPE_ULONG:   return (void*)&_any->_ulong;
        case TYPE_LLONG:   return (void*)&_any->_llong;
        case TYPE_ULLONG:  return (void*)&_any->_ullong;
        case TYPE_FLOAT:   return (void*)&_any->_float;
        case TYPE_DOUBLE:  return (void*)&_any->_double;
        case TYPE_LDOUBLE: return (void*)&_any->_ldouble;
        case TYPE_POINTER: return (void*)&_any->_pointer;
        default:           return NULL;
    }
}

static inline size_t any_size(const any * _any) {
    switch (_any->type) {
        case TYPE_CHAR:    return sizeof(char);
        case TYPE_UCHAR:   return sizeof(unsigned char);
        case TYPE_INT:     return sizeof(int);
        case TYPE_UINT:    return sizeof(unsigned int);
        case TYPE_SHORT:   return sizeof(short);
        case TYPE_USHORT:  return sizeof(unsigned short);
        case TYPE_LONG:    return sizeof(long);
        case TYPE_ULONG:   return sizeof(unsigned long);
        case TYPE_LLONG:   return sizeof(long long);
        case TYPE_ULLONG:  return sizeof(unsigned long long);
        case TYPE_FLOAT:   return sizeof(float);
        case TYPE_DOUBLE:  return sizeof(double);
        case TYPE_LDOUBLE: return sizeof(long double);
        case TYPE_POINTER: return sizeof(void*);
        default:           return 0;
    }
}

#define any_set(A, X) _Generic((X), \
    char:               any_set_char, \
    unsigned char:      any_set_uchar, \
    int:                any_set_int, \
    unsigned int:       any_set_uint, \
    short:              any_set_short, \
    unsigned short:     any_set_ushort, \
    long:               any_set_long, \
    unsigned long:      any_set_ulong, \
    long long:          any_set_llong, \
    unsigned long long: any_set_ullong, \
    float:              any_set_float, \
    double:             any_set_double, \
    long double:        any_set_ldouble, \
    default:            any_set_pointer)(A, X)

#define ANY_CHAR(X)    (any){.type = TYPE_CHAR,    ._char = X}
#define ANY_UCHAR(X)   (any){.type = TYPE_UCHAR,   ._uchar = X}
#define ANY_INT(X)     (any){.type = TYPE_INT,     ._int = X}
#define ANY_UINT(X)    (any){.type = TYPE_UINT,    ._uint = X}
#define ANY_SHORT(X)   (any){.type = TYPE_INT,     ._short = X}
#define ANY_USHORT(X)  (any){.type = TYPE_USHORT,  ._ushort = X}
#define ANY_LONG(X)    (any){.type = TYPE_LONG,    ._long = X}
#define ANY_ULONG(X)   (any){.type = TYPE_ULONG,   ._ulong = X}
#define ANY_LLONG(X)   (any){.type = TYPE_LLONG,   ._llong = X}
#define ANY_ULLONG(X)  (any){.type = TYPE_ULLONG,  ._ullong = X}
#define ANY_FLOAT(X)   (any){.type = TYPE_FLOAT,   ._float = X}
#define ANY_DOUBLE(X)  (any){.type = TYPE_DOUBLE,  ._double = X}
#define ANY_LDOUBLE(X) (any){.type = TYPE_LDOUBLE, ._ldouble = X}
#define ANY_POINTER(X) (any){.type = TYPE_POINTER, ._pointer = X}
#define ANY_NONE       (any){.type = TYPE_NONE}

#endif /* end of include guard: ANY_H */
