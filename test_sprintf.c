#include <check.h>
#include <stdlib.h>
#include <string.h>

#include "sprintf.h"

START_TEST(test_basic_string) {
  char buf[100];
  int len = sprintf(buf, "Hello, world!");
  ck_assert_str_eq(buf, "Hello, world!");
  ck_assert_int_eq(len, (int)strlen("Hello, world!"));
}
END_TEST

START_TEST(test_integer_formats) {
  char buf[100];
  int len = sprintf(buf, "%d %i %+d % d %-5d %05d", 42, -42, 42, 42, 7, 7);
  ck_assert_str_eq(buf, "42 -42 +42  42 7     00007");
  ck_assert_int_eq(len, (int)strlen(buf));
}
END_TEST

START_TEST(test_unsigned_oct_hex) {
  char buf[100];
  int len = sprintf(buf, "%u %#o %#x %#X", 255, 255, 255, 255);
  ck_assert_str_eq(buf, "255 0377 0xff 0XFF");
  ck_assert_int_eq(len, (int)strlen("255 0377 0xff 0XFF"));
}
END_TEST

START_TEST(test_char_string) {
  char buf[100];
  int len = sprintf(buf, "%5c %.3s %-8s", 'A', "Hello", "world");
  ck_assert_str_eq(buf, "    A Hel world   ");
  ck_assert_int_eq(len, (int)strlen("    A Hel world   "));
}
END_TEST

START_TEST(test_float_formats) {
  char buf[100];
  int len = sprintf(buf, "%.2f %.1e %.3g", 3.14159, 123.456, 0.0001234);
  ck_assert_str_eq(buf, "3.14 1.2e+02 0.000123");
  ck_assert_int_eq(len, (int)strlen("3.14 1.2e+02 0.000123"));
}
END_TEST

START_TEST(test_pointer_and_n) {
  char buf[100];
  int written = 0;
  int len = sprintf(buf, "ptr=%p%n", buf, &written);
  ck_assert(written > 0);
  ck_assert_int_eq(len, written);
}
END_TEST

START_TEST(test_width_precision_star) {
  char buf[100];
  int len = sprintf(buf, "%*.*f", 8, 3, 3.14159);
  ck_assert_str_eq(buf, "   3.142");
  ck_assert_int_eq(len, (int)strlen("   3.142"));
}
END_TEST

START_TEST(test_unknown_and_percent) {
  char buf[100];
  int len = sprintf(buf, "%% %q");
  ck_assert_str_eq(buf, "% %q");
  ck_assert_int_eq(len, (int)strlen("% %q"));
}
END_TEST

START_TEST(test_exponential_formats) {
  char buf[100];

  int len1 = sprintf(buf, "%.2e", 123.456);
  ck_assert_str_eq(buf, "1.23e+02");
  ck_assert_int_eq(len1, (int)strlen("1.23e+02"));

  int len2 = sprintf(buf, "%.3E", -0.004567);
  ck_assert_str_eq(buf, "-4.567E-03");
  ck_assert_int_eq(len2, (int)strlen("-4.567E-03"));

  int len3 = sprintf(buf, "%.1e", 0.0);
  ck_assert_str_eq(buf, "0.0e+00");
  ck_assert_int_eq(len3, (int)strlen("0.0e+00"));

  int len4 = sprintf(buf, "%.5e", 0.0000012345);
  ck_assert_str_eq(buf, "1.23450e-06");
  ck_assert_int_eq(len4, (int)strlen("1.23450e-06"));
}
END_TEST

Suite *sprintf_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("sprintf");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_basic_string);
  tcase_add_test(tc_core, test_integer_formats);
  tcase_add_test(tc_core, test_unsigned_oct_hex);
  tcase_add_test(tc_core, test_char_string);
  tcase_add_test(tc_core, test_float_formats);
  tcase_add_test(tc_core, test_pointer_and_n);
  tcase_add_test(tc_core, test_width_precision_star);
  tcase_add_test(tc_core, test_unknown_and_percent);
  tcase_add_test(tc_core, test_exponential_formats);

  suite_add_tcase(s, tc_core);

  return s;
}

int main(void) {
  int number_failed;
  Suite *s;
  SRunner *sr;

  s = sprintf_suite();
  sr = srunner_create(s);

  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}