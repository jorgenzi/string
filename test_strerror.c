#include <check.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "string.h"

START_TEST(test_strerror_known_errors) {
  ck_assert_str_eq(strerror(0), "Success");
  ck_assert_str_eq(strerror(1), "Operation not permitted");
  ck_assert_str_eq(strerror(2), "No such file or directory");
  ck_assert_str_eq(strerror(13), "Permission denied");
  ck_assert_str_eq(strerror(22), "Invalid argument");
  ck_assert_str_eq(strerror(40), "Too many levels of symbolic links");
}
END_TEST

START_TEST(test_strerror_boundary_cases) {
  const char *result_neg = strerror(-1);
  ck_assert_ptr_nonnull(result_neg);
  ck_assert(strstr(result_neg, "Unknown error") != NULL);

  const char *result_large = strerror(999);
  ck_assert_ptr_nonnull(result_large);
  ck_assert(strstr(result_large, "Unknown error") != NULL);

  const char *result_very_large = strerror(9999);
  ck_assert_ptr_nonnull(result_very_large);
  ck_assert(strstr(result_very_large, "Unknown error") != NULL);
}
END_TEST

START_TEST(test_strerror_vs_system) {
  int test_errors[] = {0, 1, 2, 13, 22, -1, 999};
  int num_tests = sizeof(test_errors) / sizeof(test_errors[0]);

  for (int i = 0; i < num_tests; i++) {
    int err = test_errors[i];
    const char *result = strerror(err);
    const char *sys_result = strerror(err);

    if (err >= 0 && err <= 40) {
      ck_assert_str_eq(result, sys_result);
    } else {
      ck_assert_ptr_nonnull(result);
      ck_assert(strstr(result, "Unknown error") != NULL);
    }
  }
}
END_TEST

START_TEST(test_strerror_static_buffer) {
  const char *ptr1 = strerror(999);
  const char *ptr2 = strerror(888);
  const char *ptr3 = strerror(777);

  ck_assert_ptr_eq(ptr1, ptr2);
  ck_assert_ptr_eq(ptr2, ptr3);

  ck_assert_str_eq(ptr1, "Unknown error 777");
  ck_assert_str_eq(ptr2, "Unknown error 777");
  ck_assert_str_eq(ptr3, "Unknown error 777");
}
END_TEST

START_TEST(test_strerror_os_specific) {
  const char *unknown_error = strerror(999);

#ifdef __APPLE__
  ck_assert(strstr(unknown_error, "Unknown error:") != NULL);
#elif defined(__linux__)
  ck_assert(strstr(unknown_error, "Unknown error ") != NULL);
  ck_assert(strstr(unknown_error, "Unknown error:") == NULL);
#else
  ck_assert(strstr(unknown_error, "Unknown error") != NULL);
#endif
}
END_TEST

START_TEST(test_strerror_performance) {
  for (int i = 0; i < 1000; i++) {
    int err = i % 100;
    const char *result = strerror(err);
    ck_assert_ptr_nonnull(result);
    ck_assert(strlen(result) > 0);
  }
}
END_TEST

Suite *strerror_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("strerror");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_strerror_known_errors);
  tcase_add_test(tc_core, test_strerror_boundary_cases);
  tcase_add_test(tc_core, test_strerror_vs_system);
  tcase_add_test(tc_core, test_strerror_static_buffer);
  tcase_add_test(tc_core, test_strerror_os_specific);
  tcase_add_test(tc_core, test_strerror_performance);

  suite_add_tcase(s, tc_core);

  return s;
}

int main(void) {
  Suite *s = strerror_suite();
  SRunner *sr = srunner_create(s);
  srunner_run_all(sr, CK_NORMAL);
  int number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}