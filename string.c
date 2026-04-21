#include "string.h"

char *strncat(char *dest, const char *src, size_t n) {
  char *d = dest;
  while (*d != '\0') {
    d++;
  }
  size_t i = 0;
  while (i < n && src[i] != '\0') {
    d[i] = src[i];
    i++;
  }
  d[i] = '\0';
  return dest;
}

char *strchr(const char *s, int c) {
  unsigned char target = (unsigned char)c;
  const char *p = s;
  while (*p != '\0' && (unsigned char)*p != target) {
    p++;
  }
  char *result = NULL;
  if ((unsigned char)*p == target) {
    result = (char *)p;
  }
  return result;
}

char *strrchr(const char *s, int c) {
  unsigned char target = (unsigned char)c;
  const char *last_match = NULL;
  const char *p = s;

  while (*p != '\0') {
    if ((unsigned char)*p == target) {
      last_match = p;
    }
    p++;
  }
  if (target == '\0') {
    last_match = p;
  }
  char *result = NULL;
  if (last_match != NULL) {
    result = (char *)last_match;
  }
  return result;
}

size_t strlen(const char *str) {
  size_t len = 0;
  while (str[len] != '\0') {
    len++;
  }
  return len;
}

void *memset(void *str, int c, size_t n) {
  unsigned char *ptr = (unsigned char *)str;
  for (size_t i = 0; i < n; i++) {
    ptr[i] = (unsigned char)c;
  }
  return str;
}

void *memcpy(void *dest, const void *src, size_t n) {
  unsigned char *d = (unsigned char *)dest;
  const unsigned char *s = (const unsigned char *)src;
  for (size_t i = 0; i < n; i++) {
    d[i] = s[i];
  }
  return dest;
}

void *memchr(const void *str, int c, size_t n) {
  const unsigned char *ptr = (const unsigned char *)str;
  unsigned char uc = (unsigned char)c;
  void *result = NULL;
  size_t i = 0;

  while (i < n && result == NULL) {
    if (ptr[i] == uc) {
      result = (void *)(ptr + i);
    }
    i++;
  }
  return result;
}

int memcmp(const void *str1, const void *str2, size_t n) {
  const unsigned char *p1 = (const unsigned char *)str1;
  const unsigned char *p2 = (const unsigned char *)str2;
  size_t i = 0;
  int result = 0;

  while (i < n && result == 0) {
    if (p1[i] != p2[i]) {
      result = (int)p1[i] - (int)p2[i];
    }
    i++;
  }

  return result;
}

char *strncpy(char *dest, const char *src, size_t n) {
  char *result = dest;
  size_t i = 0;

  while (i < n && src[i] != '\0') {
    dest[i] = src[i];
    i++;
  }

  while (i < n) {
    dest[i] = '\0';
    i++;
  }

  return result;
}

int strncmp(const char *str1, const char *str2, size_t n) {
  int result = 0;

  if (n > 0) {
    size_t i = 0;
    while (i < n && str1[i] != '\0' && str2[i] != '\0' && str1[i] == str2[i]) {
      i++;
    }
    if (i < n) {
      result = (unsigned char)(str1[i]) - (unsigned char)(str2[i]);
    }
  }
  return result;
}

size_t strcspn(const char *str1, const char *str2) {
  size_t count = 0;
  int found = 0;

  while (str1[count] != '\0' && found == 0) {
    int i = 0;
    while (str2[i] != '\0' && !found) {
      if (str1[count] == str2[i]) {
        found = 1;
      }
      i++;
    }
    if (found == 0) {
      count++;
    }
  }

  return count;
}

char *strpbrk(const char *str1, const char *str2) {
  size_t count = 0;
  int found = 0;

  while (str1[count] != '\0' && !found) {
    int i = 0;
    while (str2[i] != '\0' && !found) {
      if (str1[count] == str2[i]) {
        found = 1;
      }
      i++;
    }
    if (found == 0) {
      count++;
    }
  }

  char *result = NULL;
  if (found == 1) {
    result = (char *)(str1 + count);
  }

  return result;
}

static int check_match(const char *haystack, const char *needle,
                       size_t start_pos) {
  size_t j = 0;
  int match = 1;

  while (needle[j] != '\0' && match == 1) {
    if (haystack[start_pos + j] == '\0' ||
        haystack[start_pos + j] != needle[j]) {
      match = 0;
    }
    j++;
  }

  return (match == 1 && needle[j] == '\0') ? 1 : 0;
}

char *strstr(const char *haystack, const char *needle) {
  char *result = NULL;

  if (needle[0] == '\0') {
    result = (char *)haystack;
  } else {
    size_t i = 0;
    int found = 0;

    while (haystack[i] != '\0' && !found) {
      found = check_match(haystack, needle, i);
      i++;
    }

    if (found == 1) {
      result = (char *)(haystack + i - 1);
    }
  }

  return result;
}

int is_delim(char c, const char *delim) {
  int found = 0;
  while (*delim != '\0' && !found) {
    if (c == *delim) found = 1;
    delim++;
  }
  return found;
}

char *strtok(char *str, const char *delim) {
  static char *last_token = NULL;
  char *token_start = NULL;
  char *result = NULL;

  if (str != NULL) {
    last_token = str;
  }

  while (last_token != NULL && *last_token != '\0' &&
         is_delim(*last_token, delim)) {
    last_token++;
  }

  if (last_token == NULL || *last_token == '\0') {
    last_token = NULL;
    result = NULL;
  } else {
    token_start = last_token;

    while (*last_token != '\0' && !is_delim(*last_token, delim)) {
      last_token++;
    }

    if (*last_token != '\0') {
      *last_token = '\0';
      last_token++;
    } else {
      last_token = NULL;
    }

    result = token_start;
  }

  return result;
}

#if defined(__linux__)
#define MAX_ERROR_NUMBER 134
#define MIN_ERROR_NUMBER -1
#define UNKNOWN_ERROR "Unknown error "
static const char *error_arr[] = {
    "Success",
    "Operation not permitted",
    "No such file or directory",
    "No such process",
    "Interrupted system call",
    "Input/output error",
    "No such device or address",
    "Argument list too long",
    "Exec format error",
    "Bad file descriptor",
    "No child processes",
    "Resource temporarily unavailable",
    "Cannot allocate memory",
    "Permission denied",
    "Bad address",
    "Block device required",
    "Device or resource busy",
    "File exists",
    "Invalid cross-device link",
    "No such device",
    "Not a directory",
    "Is a directory",
    "Invalid argument",
    "Too many open files in system",
    "Too many open files",
    "Inappropriate ioctl for device",
    "Text file busy",
    "File too large",
    "No space left on device",
    "Illegal seek",
    "Read-only file system",
    "Too many links",
    "Broken pipe",
    "Numerical argument out of domain",
    "Numerical result out of range",
    "Resource deadlock avoided",
    "File name too long",
    "No locks available",
    "Function not implemented",
    "Directory not empty",
    "Too many levels of symbolic links",
    "Unknown error 41",
    "No message of desired type",
    "Identifier removed",
    "Channel number out of range",
    "Level 2 not synchronized",
    "Level 3 halted",
    "Level 3 reset",
    "Link number out of range",
    "Protocol driver not attached",
    "No CSI structure available",
    "Level 2 halted",
    "Invalid exchange",
    "Invalid request descriptor",
    "Exchange full",
    "No anode",
    "Invalid request code",
    "Invalid slot",
    "Unknown error 58",
    "Bad font file format",
    "Device not a stream",
    "No data available",
    "Timer expired",
    "Out of streams resources",
    "Machine is not on the network",
    "Package not installed",
    "Object is remote",
    "Link has been severed",
    "Advertise error",
    "Srmount error",
    "Communication error on send",
    "Protocol error",
    "Multihop attempted",
    "RFS specific error",
    "Bad message",
    "Value too large for defined data type",
    "Name not unique on network",
    "File descriptor in bad state",
    "Remote address changed",
    "Can not access a needed shared library",
    "Accessing a corrupted shared library",
    ".lib section in a.out corrupted",
    "Attempting to link in too many shared libraries",
    "Cannot exec a shared library directly",
    "Invalid or incomplete multibyte or wide character",
    "Interrupted system call should be restarted",
    "Streams pipe error",
    "Too many users",
    "Socket operation on non-socket",
    "Destination address required",
    "Message too long",
    "Protocol wrong type for socket",
    "Protocol not available",
    "Protocol not supported",
    "Socket type not supported",
    "Operation not supported",
    "Protocol family not supported",
    "Address family not supported by protocol",
    "Address already in use",
    "Cannot assign requested address",
    "Network is down",
    "Network is unreachable",
    "Network dropped connection on reset",
    "Software caused connection abort",
    "Connection reset by peer",
    "No buffer space available",
    "Transport endpoint is already connected",
    "Transport endpoint is not connected",
    "Cannot send after transport endpoint shutdown",
    "Too many references: cannot splice",
    "Connection timed out",
    "Connection refused",
    "Host is down",
    "No route to host",
    "Operation already in progress",
    "Operation now in progress",
    "Stale file handle",
    "Structure needs cleaning",
    "Not a XENIX named type file",
    "No XENIX semaphores available",
    "Is a named type file",
    "Remote I/O error",
    "Disk quota exceeded",
    "No medium found",
    "Wrong medium type",
    "Operation canceled",
    "Required key not available",
    "Key has expired",
    "Key has been revoked",
    "Key was rejected by service",
    "Owner died",
    "State not recoverable",
    "Operation not possible due to RF-kill",
    "Memory page has hardware error"};
#endif
#if defined(__APPLE__) && defined(__MACH__)
#define MAX_ERROR_NUMBER 107
#define MIN_ERROR_NUMBER -1
#define UNKNOWN_ERROR "Unknown error: "
static const char *error_arr[] = {
    "Undefined error: 0",
    "Operation not permitted",
    "No such file or directory",
    "No such process",
    "Interrupted system call",
    "Input/output error",
    "Device not configured",
    "Argument list too long",
    "Exec format error",
    "Bad file descriptor",
    "No child processes",
    "Resource deadlock avoided",
    "Cannot allocate memory",
    "Permission denied",
    "Bad address",
    "Block device required",
    "Resource busy",
    "File exists",
    "Cross-device link",
    "Operation not supported by device",
    "Not a directory",
    "Is a directory",
    "Invalid argument",
    "Too many open files in system",
    "Too many open files",
    "Inappropriate ioctl for device",
    "Text file busy",
    "File too large",
    "No space left on device",
    "Illegal seek",
    "Read-only file system",
    "Too many links",
    "Broken pipe",
    "Numerical argument out of domain",
    "Result too large",
    "Resource temporarily unavailable",
    "Operation now in progress",
    "Operation already in progress",
    "Socket operation on non-socket",
    "Destination address required",
    "Message too long",
    "Protocol wrong type for socket",
    "Protocol not available",
    "Protocol not supported",
    "Socket type not supported",
    "Operation not supported",
    "Protocol family not supported",
    "Address family not supported by protocol family",
    "Address already in use",
    "Can't assign requested address",
    "Network is down",
    "Network is unreachable",
    "Network dropped connection on reset",
    "Software caused connection abort",
    "Connection reset by peer",
    "No buffer space available",
    "Socket is already connected",
    "Socket is not connected",
    "Can't send after socket shutdown",
    "Too many references: can't splice",
    "Operation timed out",
    "Connection refused",
    "Too many levels of symbolic links",
    "File name too long",
    "Host is down",
    "No route to host",
    "Directory not empty",
    "Too many processes",
    "Too many users",
    "Disc quota exceeded",
    "Stale NFS file handle",
    "Too many levels of remote in path",
    "RPC struct is bad",
    "RPC version wrong",
    "RPC prog. not avail",
    "Program version wrong",
    "Bad procedure for program",
    "No locks available",
    "Function not implemented",
    "Inappropriate file type or format",
    "Authentication error",
    "Need authenticator",
    "Device power is off",
    "Device error",
    "Value too large to be stored in data type",
    "Bad executable (or shared library)",
    "Bad CPU type in executable",
    "Shared library version mismatch",
    "Malformed Mach-o file",
    "Operation canceled",
    "Identifier removed",
    "No message of desired type",
    "Illegal byte sequence",
    "Attribute not found",
    "Bad message",
    "EMULTIHOP (Reserved)",
    "No message available on STREAM",
    "ENOLINK (Reserved)",
    "No STREAM resources",
    "Not a STREAM",
    "Protocol error",
    "STREAM ioctl timeout",
    "Operation not supported on socket",
    "Policy not found",
    "State not recoverable",
    "Previous owner died",
    "Interface output queue is full"};
#endif

char *strerror(int errnum) {
  static char result[1024] = {'\0'};

  if (errnum <= MIN_ERROR_NUMBER || errnum >= MAX_ERROR_NUMBER) {
    sprintf(result, "%s%d", UNKNOWN_ERROR, errnum);
  } else {
    strncpy(result, error_arr[errnum], sizeof(result) - 1);
    result[sizeof(result) - 1] = '\0';
  }

  return result;
}

void *to_lower(const char *str) {
  char *res = NULL;
  size_t len = 0;

  if (str) {
    while (str[len] != '\0') len++;

    res = malloc(len + 1);
    if (res) {
      for (size_t i = 0; i < len; i++) {
        char c = str[i];
        if (c >= 'A' && c <= 'Z')
          res[i] = c + ('a' - 'A');
        else
          res[i] = c;
      }
      res[len] = '\0';
    }
  }
  return (void *)res;
}

void *to_upper(const char *str) {
  char *res = NULL;
  size_t len = 0;

  if (str) {
    while (str[len] != '\0') len++;

    res = malloc(len + 1);
    if (res) {
      for (size_t i = 0; i < len; i++) {
        char c = str[i];
        if (c >= 'a' && c <= 'z')
          res[i] = c - ('a' - 'A');
        else
          res[i] = c;
      }
      res[len] = '\0';
    }
  }
  return (void *)res;
}

void *insert(const char *src, const char *str, size_t start_index) {
  char *res = NULL;
  size_t len_src = 0;
  size_t len_ins = 0;
  int error = 0;

  if (!src || !str) error = 1;

  while (!error && src[len_src] != '\0') len_src++;

  if (!error && start_index > len_src) error = 1;

  while (!error && str[len_ins] != '\0') len_ins++;

  if (!error) {
    res = malloc(len_src + len_ins + 1);
    if (!res) error = 1;
  }

  if (!error) {
    size_t i = 0;
    for (; i < start_index; i++) res[i] = src[i];
    for (size_t j = 0; j < len_ins; j++, i++) res[i] = str[j];
    for (size_t k = start_index; k < len_src; k++, i++) res[i] = src[k];
    res[i] = '\0';
  }

  return (void *)(error ? NULL : res);
}

static int is_trim_char(char c, const char *trim_chars) {
  int found = 0;
  size_t i = 0;
  if (trim_chars) {
    while (trim_chars[i] != '\0' && !found) {
      if (trim_chars[i] == c) found = 1;
      i++;
    }
  }
  return found;
}

void *trim(const char *src, const char *trim_chars) {
  char *res = NULL;
  int error = 0;
  size_t len = 0, start = 0, end = 0, new_len = 0;

  if (!src || !trim_chars) error = 1;

  if (!error) {
    while (src[len] != '\0') len++;

    while (start < len && is_trim_char(src[start], trim_chars)) start++;

    end = len;
    while (end > start && is_trim_char(src[end - 1], trim_chars)) end--;

    new_len = end - start;

    res = malloc(new_len + 1);
    if (!res) error = 1;
  }

  if (!error) {
    for (size_t i = 0; i < new_len; i++) res[i] = src[start + i];
    res[new_len] = '\0';
  }

  return (void *)(error ? NULL : res);
}
