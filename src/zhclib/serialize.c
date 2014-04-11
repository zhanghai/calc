/**
 * @file serialize.c
 * @author Zhang Hai
 */

#include "serialize.h"

#include "string.h"
#include "Memory.h"


#define LMS_DEFINE_SERIALIZE_FUNCTION(type) \
    bool type##_serialize(type *value, FILE *file) { \
        return fwrite(value, sizeof(type), 1, file) == 1; \
    };

#define LMS_DEFINE_DESERIALIZE_FUNCTION(type) \
    bool type##_deserialize(type *value, FILE *file) { \
        return fread(value, sizeof(type), 1, file) == 1; \
    };


LMS_DEFINE_SERIALIZE_FUNCTION(size_t)

LMS_DEFINE_DESERIALIZE_FUNCTION(size_t)

LMS_DEFINE_SERIALIZE_FUNCTION(bool)

LMS_DEFINE_DESERIALIZE_FUNCTION(bool)

bool string_serialize(string cString, FILE *file) {
    size_t size = string_length(cString) + 1;
    return size_t_serialize(&size, file)
            && fwrite(cString, size, 1, file) == 1;
}

/**
 * Deserialize a string from file.
 * Note that memory will be allocated for the string.
 * @param cString Pointer to the string.
 * @param file The file to deserialize string from.
 * @return Whether the deserialization is successful.
 */
bool string_deserialize(string *cString, FILE *file) {
    size_t size;
    if (!size_t_deserialize(&size, file)) {
        return false;
    }
    *cString = Memory_allocate(size);
    if (fread(*cString, size, 1, file) != 1) {
        Memory_free(*cString);
        *cString = null;
        return false;
    }
    return true;
};

bool string_array_serialize(string array[], size_t size, FILE *file) {
    size_t i;
    for (i = 0; i < size; ++i) {
        if (!string_serialize(array[i], file)) {
            return false;
        }
    }
    return true;
}

bool string_array_deserialize(string array[], size_t size,
        FILE *file) {
    size_t i;
    for (i = 0; i < size; ++i) {
        if (!string_deserialize(&array[i], file)) {
            size = i;
            for (i = 0; i < size; ++i) {
                Memory_free(array[i]);
                array[i] = null;
            }
            return false;
        }
    }
    return true;
}


#undef LMS_DEFINE_SERIALIZE_FUNCTION
#undef LMS_DEFINE_DESERIALIZE_FUNCTION
