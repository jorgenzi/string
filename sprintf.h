#ifndef SPRINTF_H
#define SPRINTF_H

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

typedef struct {
  int left;
  int plus;
  int space;
  int hash;
  int zero;
  int width;
  int precision;
  int length;
  int width_star;
  int precision_star;
} format_spec;

typedef struct {
  char *dst;
  int produced;
  int written;
} Outcontext;

int sprintf(char *str, const char *format, ...);

void out_putc(Outcontext *out, char c);
void out_puts(Outcontext *out, const char *s);
void out_putn(Outcontext *out, char ch, int n);

void pad_before(Outcontext *out, const format_spec *opt, int len);
void pad_before_smart(Outcontext *out, const format_spec *opt, int len,
                      int is_numeric);
void pad_after(Outcontext *out, const format_spec *opt, int len);

int is_digit(char c);

void init_format_spec(format_spec *spec);
const char *parse_flags(const char *p, format_spec *spec);
const char *parse_width(const char *p, format_spec *spec);
const char *parse_precision(const char *p, format_spec *spec);
const char *parse_length(const char *p, format_spec *spec);
const char *parse_format(const char *fmt, format_spec *spec);
void get_star_args(format_spec *spec, va_list *ap);

void unsigned_dec(Outcontext *out, unsigned long long v);
void integer_dec(Outcontext *out, long long x, const format_spec *spec);
void unsigned_oct(Outcontext *out, unsigned long long v,
                  const format_spec *spec);
void unsigned_hex(Outcontext *out, unsigned long long v,
                  const format_spec *spec, int uppercase);

void out_float(Outcontext *out, double f, const format_spec *spec);
int calculate_exponent(double *abs_f);
void trim_trailing_zeros(char *str);
void output_mantissa(Outcontext *out, double abs_f, int precision);
void output_exponent(Outcontext *out, int exp, int uppercase);
void out_exp(Outcontext *out, double f, const format_spec *spec, int uppercase);
void output_exponential_form(Outcontext *out, double abs_f, int precision,
                             int exp, int uppercase);
void output_fixed_form(Outcontext *out, double abs_f, int precision);
void tochka_g(Outcontext *out, double f, const format_spec *spec,
              int uppercase);

int len_int_dec(long long x);
int len_uint_oct(unsigned long long v);
int len_uint_hex(unsigned long long v);
int len_uint_dec(unsigned long long v);
int len_float(double f, const format_spec *spec);

int handle_integer_specs(Outcontext *out, char spec_char, format_spec *spec,
                         va_list *ap);
unsigned long long get_unsigned_value(format_spec *spec, va_list *ap);
int handle_oct_spec(Outcontext *out, format_spec *spec, va_list *ap);
int handle_unsigned_dec_spec(Outcontext *out, format_spec *spec, va_list *ap);
int handle_hex_spec(Outcontext *out, char spec_char, format_spec *spec,
                    va_list *ap);
int handle_unsigned_specs(Outcontext *out, char spec_char, format_spec *spec,
                          va_list *ap);
int handle_float_specs(Outcontext *out, char spec_char, format_spec *spec,
                       va_list *ap);
int handle_string_specs(Outcontext *out, char spec_char, format_spec *spec,
                        va_list *ap);
const char *handle_spec(Outcontext *out, const char *pf, va_list *ap);

#endif