#include <check.h>
#include <stdlib.h>
#include <string.h>

#include "string.h"

START_TEST(test_strncat_basic) {
  char dest1[20] = "Hello";
  char dest2[20] = "Hello";
  ck_assert_str_eq(strncat(dest1, " World", 3),
                   strncat(dest2, " World", 3));
  ck_assert_str_eq(dest1, dest2);
}
END_TEST

START_TEST(test_strncat_empty_src) {
  char dest1[20] = "Hello";
  char dest2[20] = "Hello";
  ck_assert_str_eq(strncat(dest1, "", 5), strncat(dest2, "", 5));
}
END_TEST

START_TEST(test_strncat_zero_n) {
  char dest1[20] = "Hello";
  char dest2[20] = "Hello";
  ck_assert_str_eq(strncat(dest1, "World", 0), strncat(dest2, "World", 0));
}
END_TEST

START_TEST(test_strchr_basic) {
  const char *str = "Test string";
  int c = 's';
  ck_assert_ptr_eq(strchr(str, c), strchr(str, c));
}
END_TEST

START_TEST(test_strchr_char_not_found) {
  const char *str = "No match here";
  int c = 'z';
  ck_assert_ptr_eq(strchr(str, c), strchr(str, c));
}
END_TEST

START_TEST(test_strchr_null_char) {
  const char *str = "abc\0def";
  int c = '\0';
  ck_assert_ptr_eq(strchr(str, c), strchr(str, c));
}
END_TEST

START_TEST(test_strrchr_basic) {
  const char *str = "banana";
  int c = 'a';
  ck_assert_ptr_eq(strrchr(str, c), strrchr(str, c));
}
END_TEST

START_TEST(test_strrchr_char_not_found) {
  const char *str = "hello";
  int c = 'z';
  ck_assert_ptr_eq(strrchr(str, c), strrchr(str, c));
}
END_TEST

START_TEST(test_strlen_basic) {
  const char *str = "Check length";
  ck_assert_uint_eq(strlen(str), strlen(str));
}
END_TEST

START_TEST(test_strlen_empty_string) {
  const char *str = "";
  ck_assert_uint_eq(strlen(str), strlen(str));
}
END_TEST

START_TEST(test_memset_basic) {
  char buf1[20];
  char buf2[20];
  memset(buf1, 'A', 20);
  memset(buf2, 'A', 20);
  ck_assert_mem_eq(buf1, buf2, 20);
}
END_TEST

START_TEST(test_memcpy_basic) {
  char src[] = "abcdef";
  char dest1[10] = {0};
  char dest2[10] = {0};
  memcpy(dest1, src, 6);
  memcpy(dest2, src, 6);
  ck_assert_mem_eq(dest1, dest2, 6);
}
END_TEST

START_TEST(test_memchr_basic) {
  const char *str = "example";
  int c = 'a';
  size_t n = 7;
  ck_assert_ptr_eq(memchr(str, c, n), memchr(str, c, n));
}
END_TEST

START_TEST(test_memchr_char_not_found) {
  const char *str = "example";
  int c = 'z';
  size_t n = 7;
  ck_assert_ptr_eq(memchr(str, c, n), memchr(str, c, n));
}
END_TEST

START_TEST(test_memcmp_equal) {
  const char *a = "abcdef";
  const char *b = "abcdef";
  ck_assert_int_eq(memcmp(a, b, 6), memcmp(a, b, 6));
}
END_TEST

START_TEST(test_memcmp_diff_first) {
  const char *a = "abcdef";
  const char *b = "bbcdef";
  ck_assert_int_eq(memcmp(a, b, 6), memcmp(a, b, 6));
}
END_TEST

START_TEST(test_memcmp_diff_middle) {
  const char *a = "abcXef";
  const char *b = "abcdef";
  ck_assert_int_eq(memcmp(a, b, 6), memcmp(a, b, 6));
}
END_TEST

START_TEST(test_memcmp_zero_len) {
  const char *a = "abc";
  const char *b = "xyz";
  ck_assert_int_eq(memcmp(a, b, 0), memcmp(a, b, 0));
}
END_TEST

START_TEST(test_memcmp_partial) {
  const char *a = "abcde";
  const char *b = "abCde";
  ck_assert_int_eq(memcmp(a, b, 3), memcmp(a, b, 3));
}
END_TEST

START_TEST(test_strncpy_basic) {
  char dest1[20];
  char dest2[20];
  const char *src = "Hello";
  ck_assert_str_eq(strncpy(dest1, src, 5), strncpy(dest2, src, 5));
}
END_TEST

START_TEST(test_strncmp_basic) {
  const char *s1 = "abcdef";
  const char *s2 = "abcdeg";
  ck_assert_int_eq(strncmp(s1, s2, 6), strncmp(s1, s2, 6));
}
END_TEST

START_TEST(test_strncmp_equal) {
  const char *s1 = "abcdef";
  const char *s2 = "abcdef";
  ck_assert_int_eq(strncmp(s1, s2, 6), strncmp(s1, s2, 6));
}
END_TEST

START_TEST(test_strcspn_edge_cases) {
  ck_assert_uint_eq(strcspn("", ""), 0);
  ck_assert_uint_eq(strcspn("", "abc"), 0);
  ck_assert_uint_eq(strcspn("abc", ""), 3);

  ck_assert_uint_eq(strcspn("abc", "a"), 0);

  ck_assert_uint_eq(strcspn("abcdef", "xyz"), 6);

  ck_assert_uint_eq(strcspn("abcdef", "f"), 5);
}
END_TEST

START_TEST(test_strpbrk_edge_cases) {
  ck_assert_ptr_eq(strpbrk("", ""), NULL);
  ck_assert_ptr_eq(strpbrk("", "abc"), NULL);
  ck_assert_ptr_eq(strpbrk("abc", ""), NULL);

  ck_assert_ptr_eq(strpbrk("abc", "a"), &"abc"[0]);

  ck_assert_ptr_eq(strpbrk("abc", "c"), &"abc"[2]);

  ck_assert_ptr_eq(strpbrk("abc", "x"), NULL);
}
END_TEST

START_TEST(test_strstr_edge_cases) {
  ck_assert_ptr_eq(strstr("", ""), &""[0]);
  ck_assert_ptr_eq(strstr("abc", ""), &"abc"[0]);
  ck_assert_ptr_eq(strstr("", "a"), NULL);

  ck_assert_ptr_eq(strstr("abc", "abcdef"), NULL);

  ck_assert_ptr_eq(strstr("abc", "abc"), &"abc"[0]);

  ck_assert_ptr_eq(strstr("abcabcabc", "abc"), &"abcabcabc"[0]);
}
END_TEST

START_TEST(test_to_upper_null_input) {
  char *result = to_upper(NULL);
  ck_assert_ptr_null(result);
}
END_TEST

START_TEST(test_to_upper_empty_string) {
  char *result = to_upper("");
  ck_assert_ptr_nonnull(result);
  ck_assert_str_eq(result, "");
  free(result);
}
END_TEST

START_TEST(test_to_upper_all_lowercase) {
  const char *input = "hello world";
  char *result = to_upper(input);
  ck_assert_ptr_nonnull(result);
  ck_assert_str_eq(result, "HELLO WORLD");
  free(result);
}
END_TEST

START_TEST(test_to_upper_mixed_characters) {
  const char *input = "TeSt 123!";
  char *result = to_upper(input);
  ck_assert_ptr_nonnull(result);
  ck_assert_str_eq(result, "TEST 123!");
  free(result);
}
END_TEST

START_TEST(test_to_upper_no_lowercase) {
  const char *input = "1234!@#$";
  char *result = to_upper(input);
  ck_assert_ptr_nonnull(result);
  ck_assert_str_eq(result, "1234!@#$");
  free(result);
}
END_TEST

START_TEST(test_to_lower_null_input) {
  char *result = to_lower(NULL);
  ck_assert_ptr_null(result);
}
END_TEST

START_TEST(test_to_lower_empty_string) {
  char *result = to_lower("");
  ck_assert_ptr_nonnull(result);
  ck_assert_str_eq(result, "");
  free(result);
}
END_TEST

START_TEST(test_to_lower_all_lowercase) {
  const char *input = "HELLO WORLD";
  char *result = to_lower(input);
  ck_assert_ptr_nonnull(result);
  ck_assert_str_eq(result, "hello world");
  free(result);
}
END_TEST

START_TEST(test_to_lower_mixed_characters) {
  const char *input = "TeSt 123!";
  char *result = to_lower(input);
  ck_assert_ptr_nonnull(result);
  ck_assert_str_eq(result, "test 123!");
  free(result);
}
END_TEST

START_TEST(test_to_lower_no_lowercase) {
  const char *input = "already lowercase";
  char *result = to_lower(input);
  ck_assert_ptr_nonnull(result);
  ck_assert_str_eq(result, "already lowercase");
  free(result);
}
END_TEST

START_TEST(test_to_insert_null_src) {
  char *result = insert(NULL, "test", 0);
  ck_assert_ptr_null(result);
}
END_TEST

START_TEST(test_to_insert_null_str) {
  char *result = insert("hello", NULL, 0);
  ck_assert_ptr_null(result);
}
END_TEST

START_TEST(test_to_insert_index_out_of_bounds) {
  char *result = insert("hello", "test", 10);
  ck_assert_ptr_null(result);
}
END_TEST

START_TEST(test_to_insert_insert_beginning) {
  const char *src = "world";
  const char *ins = "hello ";
  char *result = insert(src, ins, 0);
  ck_assert_ptr_nonnull(result);
  ck_assert_str_eq(result, "hello world");
  free(result);
}
END_TEST

START_TEST(test_to_insert_insert_middle) {
  const char *src = "hwld";
  const char *ins = "or";
  char *result = insert(src, ins, 2);
  ck_assert_ptr_nonnull(result);
  ck_assert_str_eq(result, "hworld");
  free(result);
}
END_TEST

START_TEST(test_to_insert_insert_end) {
  const char *src = "hello";
  const char *ins = " world";
  char *result = insert(src, ins, 5);
  ck_assert_ptr_nonnull(result);
  ck_assert_str_eq(result, "hello world");
  free(result);
}
END_TEST

START_TEST(test_to_insert_empty_insert) {
  const char *src = "hello";
  const char *ins = "";
  char *result = insert(src, ins, 3);
  ck_assert_ptr_nonnull(result);
  ck_assert_str_eq(result, "hello");
  free(result);
}
END_TEST

START_TEST(test_to_trim_null_src) {
  char *result = trim(NULL, " ");
  ck_assert_ptr_null(result);
}
END_TEST

START_TEST(test_to_trim_null_trim_chars) {
  char *result = trim(" hello ", NULL);
  ck_assert_ptr_null(result);
}
END_TEST

START_TEST(test_to_trim_no_trim_chars_in_src) {
  const char *input = "hello";
  char *result = trim(input, " ");
  ck_assert_ptr_nonnull(result);
  ck_assert_str_eq(result, "hello");
  free(result);
}
END_TEST

START_TEST(test_to_trim_trim_spaces) {
  const char *input = "  hello  ";
  char *result = trim(input, " ");
  ck_assert_ptr_nonnull(result);
  ck_assert_str_eq(result, "hello");
  free(result);
}
END_TEST

START_TEST(test_to_trim_trim_multiple_chars) {
  const char *input = "\t\nhello\n\t";
  char *result = trim(input, "\t\n");
  ck_assert_ptr_nonnull(result);
  ck_assert_str_eq(result, "hello");
  free(result);
}
END_TEST

START_TEST(test_to_trim_all_trim_chars) {
  const char *input = " \n\t ";
  char *result = trim(input, " \n\t");
  ck_assert_ptr_nonnull(result);
  ck_assert_str_eq(result, "");
  free(result);
}
END_TEST

Suite *string_suite(void) {
  Suite *s = suite_create("String");

  TCase *tc_strncat = tcase_create("strncat");
  tcase_add_test(tc_strncat, test_strncat_basic);
  tcase_add_test(tc_strncat, test_strncat_empty_src);
  tcase_add_test(tc_strncat, test_strncat_zero_n);
  suite_add_tcase(s, tc_strncat);

  TCase *tc_strchr = tcase_create("strchr");
  tcase_add_test(tc_strchr, test_strchr_basic);
  tcase_add_test(tc_strchr, test_strchr_char_not_found);
  tcase_add_test(tc_strchr, test_strchr_null_char);
  suite_add_tcase(s, tc_strchr);

  TCase *tc_strrchr = tcase_create("strrchr");
  tcase_add_test(tc_strrchr, test_strrchr_basic);
  tcase_add_test(tc_strrchr, test_strrchr_char_not_found);
  suite_add_tcase(s, tc_strrchr);

  TCase *tc_strlen = tcase_create("strlen");
  tcase_add_test(tc_strlen, test_strlen_basic);
  tcase_add_test(tc_strlen, test_strlen_empty_string);
  suite_add_tcase(s, tc_strlen);

  TCase *tc_memset = tcase_create("memset");
  tcase_add_test(tc_memset, test_memset_basic);
  suite_add_tcase(s, tc_memset);

  TCase *tc_memcpy = tcase_create("memcpy");
  tcase_add_test(tc_memcpy, test_memcpy_basic);
  suite_add_tcase(s, tc_memcpy);

  TCase *tc_memchr = tcase_create("memchr");
  tcase_add_test(tc_memchr, test_memchr_basic);
  tcase_add_test(tc_memchr, test_memchr_char_not_found);
  suite_add_tcase(s, tc_memchr);

  TCase *tc_memcmp = tcase_create("memcmp");
  tcase_add_test(tc_memcmp, test_memcmp_equal);
  tcase_add_test(tc_memcmp, test_memcmp_diff_first);
  tcase_add_test(tc_memcmp, test_memcmp_diff_middle);
  tcase_add_test(tc_memcmp, test_memcmp_zero_len);
  tcase_add_test(tc_memcmp, test_memcmp_partial);
  suite_add_tcase(s, tc_memcmp);

  TCase *tc_strncpy = tcase_create("strncpy");
  tcase_add_test(tc_strncpy, test_strncpy_basic);
  suite_add_tcase(s, tc_strncpy);

  TCase *tc_strncmp = tcase_create("strncmp");
  tcase_add_test(tc_strncmp, test_strncmp_basic);
  tcase_add_test(tc_strncmp, test_strncmp_equal);
  suite_add_tcase(s, tc_strncmp);

  TCase *tc = tcase_create("Edge");
  tcase_add_test(tc, test_strcspn_edge_cases);
  tcase_add_test(tc, test_strpbrk_edge_cases);
  tcase_add_test(tc, test_strstr_edge_cases);
  suite_add_tcase(s, tc);

  TCase *tc_to_upper = tcase_create("to_upper");
  tcase_add_test(tc_to_upper, test_to_upper_null_input);
  tcase_add_test(tc_to_upper, test_to_upper_empty_string);
  tcase_add_test(tc_to_upper, test_to_upper_all_lowercase);
  tcase_add_test(tc_to_upper, test_to_upper_mixed_characters);
  tcase_add_test(tc_to_upper, test_to_upper_no_lowercase);
  suite_add_tcase(s, tc_to_upper);

  TCase *tc_to_lower = tcase_create("to_lower");
  tcase_add_test(tc_to_lower, test_to_lower_null_input);
  tcase_add_test(tc_to_lower, test_to_lower_empty_string);
  tcase_add_test(tc_to_lower, test_to_lower_all_lowercase);
  tcase_add_test(tc_to_lower, test_to_lower_mixed_characters);
  tcase_add_test(tc_to_lower, test_to_lower_no_lowercase);
  suite_add_tcase(s, tc_to_lower);

  TCase *tc_to_insert = tcase_create("to_insert");
  tcase_add_test(tc_to_insert, test_to_insert_null_src);
  tcase_add_test(tc_to_insert, test_to_insert_null_str);
  tcase_add_test(tc_to_insert, test_to_insert_index_out_of_bounds);
  tcase_add_test(tc_to_insert, test_to_insert_insert_beginning);
  tcase_add_test(tc_to_insert, test_to_insert_insert_middle);
  tcase_add_test(tc_to_insert, test_to_insert_insert_end);
  tcase_add_test(tc_to_insert, test_to_insert_empty_insert);
  suite_add_tcase(s, tc_to_insert);

  TCase *tc_to_trim = tcase_create("to_trim");
  tcase_add_test(tc_to_trim, test_to_trim_null_src);
  tcase_add_test(tc_to_trim, test_to_trim_null_trim_chars);
  tcase_add_test(tc_to_trim, test_to_trim_no_trim_chars_in_src);
  tcase_add_test(tc_to_trim, test_to_trim_trim_spaces);
  tcase_add_test(tc_to_trim, test_to_trim_trim_multiple_chars);
  tcase_add_test(tc_to_trim, test_to_trim_all_trim_chars);
  suite_add_tcase(s, tc_to_trim);

  return s;
}

int main(void) {
  Suite *s = string_suite();
  SRunner *sr = srunner_create(s);
  srunner_run_all(sr, CK_NORMAL);
  int failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}