#include "sprintf.h"

#include <float.h>
#include <math.h>
#include <stdio.h>

#include "string.h"

void out_putc(Outcontext *out, char c) {
  *out->dst++ = c;
  out->produced++;
  out->written++;
}

void out_puts(Outcontext *out, const char *s) {
  const char *p = s ? s : "(null)";
  while (*p != '\0') out_putc(out, *p++);
}

void out_putn(Outcontext *out, char ch, int n) {
  while (n-- > 0) out_putc(out, ch);
}

void pad_before(Outcontext *out, const format_spec *opt, int len) {
  if (!opt->left && opt->width > len) {
    char fill = opt->zero ? '0' : ' ';
    out_putn(out, fill, opt->width - len);
  }
}

void pad_before_smart(Outcontext *out, const format_spec *opt, int len,
                      int is_numeric) {
  if (!opt->left && opt->width > len) {
    char fill = ' ';
    if (is_numeric && opt->zero && opt->precision < 0) {
      fill = '0';
    }
    out_putn(out, fill, opt->width - len);
  }
}

void pad_after(Outcontext *out, const format_spec *opt, int len) {
  if (opt->left && opt->width > len) {
    out_putn(out, ' ', opt->width - len);
  }
}

int is_digit(char c) { return (c >= '0' && c <= '9'); }

void init_format_spec(format_spec *spec) {
  spec->left = 0;
  spec->plus = 0;
  spec->space = 0;
  spec->hash = 0;
  spec->zero = 0;
  spec->width = 0;
  spec->precision = -1;
  spec->length = 0;
  spec->width_star = 0;
  spec->precision_star = 0;
}

const char *parse_flags(const char *p, format_spec *spec) {
  int continue_parsing = 1;
  while (*p && continue_parsing) {
    if (*p == '-') {
      spec->left = 1;
      p++;
    } else if (*p == '+') {
      spec->plus = 1;
      p++;
    } else if (*p == ' ') {
      spec->space = 1;
      p++;
    } else if (*p == '#') {
      spec->hash = 1;
      p++;
    } else if (*p == '0') {
      spec->zero = 1;
      p++;
    } else {
      continue_parsing = 0;
    }
  }

  if (spec->left) spec->zero = 0;
  if (spec->plus) spec->space = 0;

  return p;
}

const char *parse_width(const char *p, format_spec *spec) {
  if (is_digit(*p)) {
    while (is_digit(*p)) {
      spec->width = spec->width * 10 + (*p - '0');
      p++;
    }
  } else if (*p == '*') {
    spec->width_star = 1;
    p++;
  }
  return p;
}

const char *parse_precision(const char *p, format_spec *spec) {
  if (*p == '.') {
    p++;
    if (is_digit(*p)) {
      spec->precision = 0;
      while (is_digit(*p)) {
        spec->precision = spec->precision * 10 + (*p - '0');
        p++;
      }
    } else if (*p == '*') {
      spec->precision_star = 1;
      p++;
    } else {
      spec->precision = 0;
    }
  }
  return p;
}

const char *parse_length(const char *p, format_spec *spec) {
  if (*p == 'h') {
    spec->length = 1;
    p++;
  } else if (*p == 'l') {
    spec->length = 2;
    p++;
  } else if (*p == 'L') {
    spec->length = 3;
    p++;
  }
  return p;
}

const char *parse_format(const char *fmt, format_spec *spec) {
  init_format_spec(spec);
  const char *p = fmt;

  p = parse_flags(p, spec);
  p = parse_width(p, spec);
  p = parse_precision(p, spec);
  p = parse_length(p, spec);

  return p;
}

void unsigned_dec(Outcontext *out, unsigned long long v) {
  char tmp[32];
  int n = 0;
  if (v == 0) {
    tmp[n++] = '0';
  } else {
    while (v > 0) {
      tmp[n++] = (char)('0' + (v % 10));
      v /= 10;
    }
  }
  for (int i = n - 1; i >= 0; i--) {
    out_putc(out, tmp[i]);
  }
}

void integer_dec(Outcontext *out, long long x, const format_spec *spec) {
  unsigned long long u;
  int sign = 0;

  if (x < 0) {
    u = (unsigned long long)(-(long long)x);
    sign = 1;
  } else {
    u = (unsigned long long)x;
    if (spec->plus && x > 0) {
      out_putc(out, '+');
    } else if (spec->space && x > 0) {
      out_putc(out, ' ');
    }
  }

  if (sign) out_putc(out, '-');
  unsigned_dec(out, u);
}

void unsigned_oct(Outcontext *out, unsigned long long v,
                  const format_spec *spec) {
  char tmp[32];
  int n = 0;
  if (v == 0) {
    tmp[n++] = '0';
  } else {
    while (v > 0) {
      tmp[n++] = (char)('0' + (v % 8));
      v /= 8;
    }
  }

  if (spec->hash) {
    out_putc(out, '0');
  }

  for (int i = n - 1; i >= 0; i--) {
    out_putc(out, tmp[i]);
  }
}

void unsigned_hex(Outcontext *out, unsigned long long v,
                  const format_spec *spec, int uppercase) {
  char tmp[32];
  int n = 0;
  const char *digits = uppercase ? "0123456789ABCDEF" : "0123456789abcdef";
  int sign = 0;

  if (v == 0) {
    tmp[n++] = '0';
    sign = 1;
  } else {
    while (v > 0) {
      tmp[n++] = digits[v % 16];
      v /= 16;
    }
  }

  if (spec->hash && !sign) {
    out_putc(out, '0');
    out_putc(out, uppercase ? 'X' : 'x');
  }

  for (int i = n - 1; i >= 0; i--) {
    out_putc(out, tmp[i]);
  }
}

void output_rounded_fraction(Outcontext *out, double frac_part, int precision) {
  int digits[20] = {0};
  for (int i = 0; i <= precision; i++) {
    frac_part *= 10;
    digits[i] = (int)frac_part;
    frac_part -= digits[i];
  }

  if (digits[precision] >= 5) {
    int pos = precision - 1;
    digits[pos]++;
    while (pos > 0 && digits[pos] == 10) {
      digits[pos] = 0;
      pos--;
      digits[pos]++;
    }
    if (digits[0] == 10) {
      digits[0] = 0;
    }
  }

  for (int i = 0; i < precision; i++) {
    out_putc(out, '0' + digits[i]);
  }
}

void out_float(Outcontext *out, double f, const format_spec *spec) {
  int print_done = 0;

  int precision = spec->precision < 0 ? 6 : spec->precision;
  int is_negative = (f < 0);
  double abs_f = is_negative ? -f : f;

  if (isnan(f)) {
    out_puts(out, "nan");
    print_done = 1;
  } else if (isinf(f)) {
    if (is_negative) out_putc(out, '-');
    out_puts(out, "inf");
    print_done = 1;
  }

  if (!print_done) {
    if (is_negative) out_putc(out, '-');

    unsigned long long int_part = (unsigned long long)abs_f;
    unsigned_dec(out, int_part);

    if (precision > 0) {
      out_putc(out, '.');
      double frac_part = abs_f - int_part;
      output_rounded_fraction(out, frac_part, precision);
    } else if (spec->hash) {
      out_putc(out, '.');
    }
  }
}

int calculate_exponent(double *abs_f) {
  int exp = 0;
  if (*abs_f != 0) {
    while (*abs_f >= 10) {
      *abs_f /= 10;
      exp++;
    }
    while (*abs_f < 1) {
      *abs_f *= 10;
      exp--;
    }
  }
  return exp;
}

void trim_trailing_zeros(char *str) {
  int len = strlen(str);
  int i = len - 1;

  while (i >= 0 && str[i] == '0') {
    str[i] = '\0';
    i--;
  }
  if (i >= 0 && str[i] == '.') {
    str[i] = '\0';
  }
}

void output_mantissa(Outcontext *out, double abs_f, int precision) {
  int int_part = (int)abs_f;
  out_putc(out, '0' + int_part);

  if (precision > 0) {
    out_putc(out, '.');
    double frac = abs_f - int_part;
    output_rounded_fraction(out, frac, precision);
  }
}

void output_exponent(Outcontext *out, int exp, int uppercase) {
  out_putc(out, uppercase ? 'E' : 'e');
  if (exp < 0) {
    out_putc(out, '-');
    exp = -exp;
  } else {
    out_putc(out, '+');
  }

  if (exp < 10) {
    out_putc(out, '0');
  }
  unsigned_dec(out, exp);
}

void out_exp(Outcontext *out, double f, const format_spec *spec,
             int uppercase) {
  if (isnan(f)) {
    out_puts(out, uppercase ? "NAN" : "nan");
  } else if (isinf(f)) {
    out_puts(out, f < 0 ? (uppercase ? "-INF" : "-inf")
                        : (uppercase ? "INF" : "inf"));
  } else {
    int precision = spec->precision < 0 ? 6 : spec->precision;
    int is_negative = (f < 0);
    double abs_f = is_negative ? -f : f;

    if (is_negative) {
      out_putc(out, '-');
    }

    int exp = calculate_exponent(&abs_f);
    output_mantissa(out, abs_f, precision);
    output_exponent(out, exp, uppercase);
  }
}

void output_exponential_form(Outcontext *out, double abs_f, int precision,
                             int exp, int uppercase) {
  int int_part = (int)abs_f;
  char buf[100];
  int pos = 0;
  buf[pos++] = '0' + int_part;
  if (precision > 0) {
    buf[pos++] = '.';
    double frac = abs_f - int_part;
    int digits[20] = {0};
    for (int i = 0; i < precision; i++) {
      frac *= 10;
      digits[i] = (int)frac;
      frac -= digits[i];
    }
    frac *= 10;
    if ((int)frac >= 5) {
      int carry = 1;
      for (int i = precision - 1; i >= 0 && carry; i--) {
        digits[i]++;
        if (digits[i] == 10)
          digits[i] = 0;
        else
          carry = 0;
      }
      if (carry) {
        int_part++;
        buf[0] = '0' + int_part;
        for (int i = 0; i < precision; i++) digits[i] = 0;
      }
    }
    for (int i = 0; i < precision; i++) buf[pos++] = '0' + digits[i];
  }
  buf[pos] = '\0';

  trim_trailing_zeros(buf);
  for (int i = 0; buf[i]; i++) out_putc(out, buf[i]);

  out_putc(out, uppercase ? 'E' : 'e');
  if (exp < 0) {
    out_putc(out, '-');
    exp = -exp;
  } else {
    out_putc(out, '+');
  }
  if (exp < 10) out_putc(out, '0');
  unsigned_dec(out, exp);
}

void output_fixed_form(Outcontext *out, double abs_f, int precision) {
  unsigned long long int_part = (unsigned long long)abs_f;
  unsigned_dec(out, int_part);

  if (precision > 0) {
    out_putc(out, '.');
    double frac_part = abs_f - int_part;
    output_rounded_fraction(out, frac_part, precision);
  }
}

void tochka_g(Outcontext *out, double f, const format_spec *spec,
              int uppercase) {
  int precision = spec->precision < 0 ? 6 : spec->precision;
  if (precision == 0) precision = 1;

  int is_negative = (f < 0);
  double abs_f = is_negative ? -f : f;

  if (is_negative) {
    out_putc(out, '-');
  }

  int exp = 0;
  double temp = abs_f;

  if (temp != 0.0) {
    while (temp >= 10.0) {
      temp /= 10.0;
      exp++;
    }
    while (temp < 1.0) {
      temp *= 10.0;
      exp--;
    }
  }

  int use_exp = (exp < -4 || exp >= precision);

  if (use_exp) {
    output_exponential_form(out, temp, precision - 1, exp, uppercase);
  } else {
    int fixed_precision = precision - (exp + 1);
    if (fixed_precision < 0) fixed_precision = 0;
    output_fixed_form(out, abs_f, fixed_precision);
  }
}

int len_int_dec(long long x) {
  unsigned long long u =
      (x < 0) ? (unsigned long long)(-(long long)x) : (unsigned long long)x;
  int n = (x < 0) ? 1 : 0;
  if (u == 0) {
    n += 1;
  } else {
    while (u) {
      n++;
      u /= 10;
    }
  }
  return n;
}

int len_uint_oct(unsigned long long v) {
  int n = 0;
  if (v == 0) {
    n = 1;
  } else {
    while (v) {
      n++;
      v /= 8;
    }
  }
  return n;
}

int len_uint_hex(unsigned long long v) {
  int n = 0;
  if (v == 0) {
    n = 1;
  } else {
    while (v) {
      n++;
      v /= 16;
    }
  }
  return n;
}

int len_uint_dec(unsigned long long v) {
  int n = 0;
  if (v == 0) {
    n = 1;
  } else {
    while (v) {
      n++;
      v /= 10;
    }
  }
  return n;
}

int len_float(double f, const format_spec *spec) {
  int precision = spec->precision < 0 ? 6 : spec->precision;
  int len = 0;

  if (isnan(f) || isinf(f)) {
    len = 3;
    if (f < 0) len++;
  } else {
    int is_negative = (f < 0);
    double abs_f = is_negative ? -f : f;

    if (is_negative) len++;
    unsigned long long int_part = (unsigned long long)abs_f;
    if (int_part == 0) {
      len++;
    } else {
      while (int_part > 0) {
        len++;
        int_part /= 10;
      }
    }

    if (precision > 0) {
      len++;
      len += precision;
    }
  }

  return len;
}

void get_star_args(format_spec *spec, va_list *ap) {
  if (spec->width_star) {
    spec->width = va_arg(*ap, int);
    if (spec->width < 0) {
      spec->width = -spec->width;
      spec->left = 1;
    }
  }
  if (spec->precision_star) {
    int prec = va_arg(*ap, int);
    spec->precision = (prec < 0) ? -1 : prec;
  }
}

int handle_integer_specs(Outcontext *out, char spec_char, format_spec *spec,
                         va_list *ap) {
  int result = 0;

  if (spec_char == 'd' || spec_char == 'i') {
    long long v;
    if (spec->length == 1) {
      v = (short)va_arg(*ap, int);
    } else if (spec->length == 2) {
      v = va_arg(*ap, long);
    } else if (spec->length == 3) {
      v = va_arg(*ap, long double);
    } else {
      v = va_arg(*ap, int);
    }

    int len = len_int_dec(v);
    if (spec->plus && v >= 0)
      len++;
    else if (spec->space && v >= 0)
      len++;

    if (spec->zero && !spec->left && spec->precision < 0) {
      if (v < 0) {
        out_putc(out, '-');
      } else if (spec->plus && v > 0) {
        out_putc(out, '+');
      } else if (spec->space && v > 0) {
        out_putc(out, ' ');
      }
      if (spec->width > len) {
        out_putn(out, '0', spec->width - len);
      }
      unsigned_dec(out, (v < 0) ? (unsigned long long)(-(long long)v)
                                : (unsigned long long)v);
    } else {
      pad_before(out, spec, len);
      integer_dec(out, v, spec);
    }
    pad_after(out, spec, len);
    result = 1;
  }

  return result;
}

unsigned long long get_unsigned_value(format_spec *spec, va_list *ap) {
  unsigned long long v;
  if (spec->length == 1) {
    v = (unsigned short)va_arg(*ap, unsigned int);
  } else if (spec->length == 2) {
    v = va_arg(*ap, unsigned long);
  } else if (spec->length == 3) {
    v = va_arg(*ap, unsigned long long);
  } else {
    v = va_arg(*ap, unsigned int);
  }
  return v;
}

int handle_oct_spec(Outcontext *out, format_spec *spec, va_list *ap) {
  unsigned long long v = get_unsigned_value(spec, ap);
  int len = len_uint_oct(v);
  if (spec->hash) len++;

  pad_before_smart(out, spec, len, 1);
  unsigned_oct(out, v, spec);
  pad_after(out, spec, len);
  return 1;
}

int handle_unsigned_dec_spec(Outcontext *out, format_spec *spec, va_list *ap) {
  unsigned long long v = get_unsigned_value(spec, ap);
  int len = len_uint_dec(v);
  pad_before_smart(out, spec, len, 1);
  unsigned_dec(out, v);
  pad_after(out, spec, len);
  return 1;
}

int handle_hex_spec(Outcontext *out, char spec_char, format_spec *spec,
                    va_list *ap) {
  unsigned long long v = get_unsigned_value(spec, ap);
  int len = len_uint_hex(v);
  if (spec->hash && v != 0) len += 2;

  pad_before_smart(out, spec, len, 1);
  unsigned_hex(out, v, spec, (spec_char == 'X'));
  pad_after(out, spec, len);
  return 1;
}

int handle_unsigned_specs(Outcontext *out, char spec_char, format_spec *spec,
                          va_list *ap) {
  int result = 0;

  if (spec_char == 'o') {
    result = handle_oct_spec(out, spec, ap);
  } else if (spec_char == 'u') {
    result = handle_unsigned_dec_spec(out, spec, ap);
  } else if (spec_char == 'x' || spec_char == 'X') {
    result = handle_hex_spec(out, spec_char, spec, ap);
  }

  return result;
}

int handle_float_specs(Outcontext *out, char spec_char, format_spec *spec,
                       va_list *ap) {
  int result = 0;

  if (spec_char == 'f' || spec_char == 'F') {
    double f = va_arg(*ap, double);
    int len = len_float(f, spec);

    pad_before(out, spec, len);
    out_float(out, f, spec);
    pad_after(out, spec, len);
    result = 1;
  } else if (spec_char == 'e' || spec_char == 'E') {
    double f = va_arg(*ap, double);
    int len = 20;

    pad_before(out, spec, len);
    out_exp(out, f, spec, (spec_char == 'E'));
    pad_after(out, spec, len);
    result = 1;
  } else if (spec_char == 'g' || spec_char == 'G') {
    double f = va_arg(*ap, double);
    int len = 20;

    pad_before(out, spec, len);
    tochka_g(out, f, spec, (spec_char == 'G'));
    pad_after(out, spec, len);
    result = 1;
  }

  return result;
}

int handle_string_specs(Outcontext *out, char spec_char, format_spec *spec,
                        va_list *ap) {
  int result = 0;
  if (spec_char == 'c') {
    int ch = va_arg(*ap, int);
    pad_before_smart(out, spec, 1, 0);
    out_putc(out, (char)ch);
    pad_after(out, spec, 1);
    result = 1;
  } else if (spec_char == 's') {
    const char *s = va_arg(*ap, const char *);
    int len = 0;
    const char *t = s ? s : "(null)";
    while (t[len]) len++;
    if (spec->precision >= 0 && len > spec->precision) {
      len = spec->precision;
    }
    pad_before_smart(out, spec, len, 0);
    for (int i = 0; i < len; i++) {
      out_putc(out, t[i]);
    }
    pad_after(out, spec, len);
    result = 1;
  } else if (spec_char == 'p') {
    void *ptr = va_arg(*ap, void *);
    unsigned long long addr = (unsigned long long)ptr;
    int len = 2 + len_uint_hex(addr);
    pad_before_smart(out, spec, len, 0);
    out_putc(out, '0');
    out_putc(out, 'x');
    unsigned_hex(out, addr, spec, 0);
    pad_after(out, spec, len);
    result = 1;
  } else if (spec_char == 'n') {
    int *count = va_arg(*ap, int *);
    *count = out->produced;
    result = 1;
  }

  return result;
}

const char *handle_spec(Outcontext *out, const char *pf, va_list *ap) {
  format_spec spec;
  const char *next = parse_format(pf, &spec);
  char spec_char = *next;
  const char *result = pf;

  get_star_args(&spec, ap);

  if (handle_integer_specs(out, spec_char, &spec, ap)) {
    result = next + 1;
  } else if (handle_unsigned_specs(out, spec_char, &spec, ap)) {
    result = next + 1;
  } else if (handle_float_specs(out, spec_char, &spec, ap)) {
    result = next + 1;
  } else if (handle_string_specs(out, spec_char, &spec, ap)) {
    result = next + 1;
  } else if (spec_char == '%') {
    out_putc(out, '%');
    result = next + 1;
  } else {
    out_putc(out, '%');
  }

  return result;
}

int sprintf(char *str, const char *format, ...) {
  va_list ap;
  va_start(ap, format);

  Outcontext out;
  out.dst = str;
  out.produced = 0;
  out.written = 0;

  const char *f = format;
  while (*f) {
    if (*f != '%') {
      out_putc(&out, *f++);
    } else {
      f++;
      f = handle_spec(&out, f, &ap);
    }
  }

  *out.dst = '\0';
  va_end(ap);
  return out.produced;
}