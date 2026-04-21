#include <check.h>
#include <stdio.h>
#include <string.h>

#include "string.h"

static void compare_tokenization(const char *input, const char *delim) {
  char buf1[256], buf2[256];
  snprintf(buf1, sizeof(buf1), "%s", input ? input : "");
  snprintf(buf2, sizeof(buf2), "%s", input ? input : "");

  char *tok = strtok(input ? buf1 : NULL, delim);
  char *sys_tok = strtok(input ? buf2 : NULL, delim);

  while (tok != NULL && sys_tok != NULL) {
    ck_assert_str_eq(tok, sys_tok);
    tok = strtok(NULL, delim);
    sys_tok = strtok(NULL, delim);
  }
  ck_assert_ptr_eq(tok, sys_tok);
}

START_TEST(test_strtok_basic) { compare_tokenization("a,b,c", ","); }
END_TEST

START_TEST(test_strtok_multi_delims) {
  compare_tokenization("a,,b;c::d", ",;:");
}
END_TEST

START_TEST(test_strtok_leading_delims) { compare_tokenization(",,a,b", ","); }
END_TEST

START_TEST(test_strtok_trailing_delims) { compare_tokenization("a,b,", ","); }
END_TEST

START_TEST(test_strtok_only_delims) { compare_tokenization(",,,,", ","); }
END_TEST

START_TEST(test_strtok_single_token) { compare_tokenization("abc", ","); }
END_TEST

START_TEST(test_strtok_spaces) { compare_tokenization("  a  b   c  ", " "); }
END_TEST

START_TEST(test_strtok_tabs_spaces) {
  compare_tokenization("\t\tfoo\t bar\tbaz ", " \t");
}
END_TEST

START_TEST(test_strtok_empty_string) { compare_tokenization("", ","); }
END_TEST

START_TEST(test_strtok_null_after_init) {
  char buf1[] = "hello";
  char buf2[] = "hello";
  strtok(buf1, " ");
  strtok(buf2, " ");

  char *res = strtok(NULL, " ");
  char *sys_res = strtok(NULL, " ");

  ck_assert_ptr_eq(res, sys_res);
}
END_TEST

START_TEST(test_strtok_delim_not_present) { compare_tokenization("abc", ";"); }
END_TEST

START_TEST(test_strtok_empty_delim) {
  char input[] = "a b c";
  char sys_input[] = "a b c";

  char *res = strtok(input, "");
  char *sys_res = strtok(sys_input, "");

  ck_assert_str_eq(res, sys_res);
  ck_assert_ptr_eq(strtok(NULL, ""), strtok(NULL, ""));
}
END_TEST

Suite *strtok_suite(void) {
  Suite *s = suite_create("strtok");
  TCase *tc = tcase_create("Core");

  tcase_add_test(tc, test_strtok_basic);
  tcase_add_test(tc, test_strtok_multi_delims);
  tcase_add_test(tc, test_strtok_leading_delims);
  tcase_add_test(tc, test_strtok_trailing_delims);
  tcase_add_test(tc, test_strtok_only_delims);
  tcase_add_test(tc, test_strtok_single_token);
  tcase_add_test(tc, test_strtok_spaces);
  tcase_add_test(tc, test_strtok_tabs_spaces);
  tcase_add_test(tc, test_strtok_empty_string);
  tcase_add_test(tc, test_strtok_null_after_init);
  tcase_add_test(tc, test_strtok_delim_not_present);
  tcase_add_test(tc, test_strtok_empty_delim);

  suite_add_tcase(s, tc);
  return s;
}

int main(void) {
  Suite *s = strtok_suite();
  SRunner *sr = srunner_create(s);
  srunner_run_all(sr, CK_NORMAL);
  int failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}