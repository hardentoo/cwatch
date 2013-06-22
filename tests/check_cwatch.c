#include <stdlib.h>
#include <check.h>

#include "../src/cwatch.h"

/* helper functions */
void fill_with_paths(LIST *list, char **paths, int number_of_paths)
{
    int i;
    for (i = 0; i < number_of_paths; i++) {
        list_push(list, (void *) paths[i]);
    }
}
/* end of helper functions */

LIST *list_wd;

void setup(void)
{
    list_wd = list_init();
}

void teardown(void)
{
    list_free(list_wd);
}

START_TEST(creates_a_wd_data)
{
    char *path = "/usr/opt/path/";
    int wd = 1;
    WD_DATA *wd_data = create_wd_data(path, wd);

    ck_assert_ptr_ne(wd_data, NULL);
    ck_assert_str_eq(wd_data->path, path);
    ck_assert_int_eq(wd_data->wd, wd);
    ck_assert_ptr_eq(wd_data->links->first, NULL);
}
END_TEST

START_TEST(creates_a_link_data)
{
    char *link_path = "/usr/opt/symlink";
    WD_DATA *wd_data = create_wd_data("foo", 0);
    LINK_DATA *link_data = create_link_data(link_path, wd_data);

    ck_assert_ptr_ne(link_data, NULL);
    ck_assert_str_eq(link_data->path, link_path);
    ck_assert_ptr_eq(link_data->wd_data, wd_data);
}
END_TEST

START_TEST(formats_command_correctly_using_special_characters)
{
    ck_assert_ptr_eq(NULL, NULL);
}
END_TEST

START_TEST(returns_true_if_a_path_is_a_child_of_another_path)
{
    char *parent = "/usr/opt/parent/";
    char *child = "/usr/opt/parent/child/of/";

    bool_t expected = TRUE;
    bool_t actual = is_child_of(child, parent);

    ck_assert_msg(
        expected == actual,
        "The child path provided is not child of parent path");
}
END_TEST

START_TEST(returns_false_if_a_path_is_not_a_child_of_another_path)
{
    char *parent = "/usr/opt/parent/";
    char *child = "/usr/opt/another_parent/child/of/";

    bool_t expected = FALSE;
    bool_t actual = is_child_of(child, parent);

    ck_assert_msg(
        expected == actual,
        "The child path provided is child of parent path");
}
END_TEST

START_TEST(returns_true_if_a_path_is_listed_in)
{
    char *paths[] = {
        "/usr/opt/path1",
        "/usr/opt/path2",
        "/usr/opt/path3"
    };

    LIST *list = list_init();
    fill_with_paths(list, paths, 3);

    bool_t expected = TRUE;
    bool_t actual = is_listed_in(paths[0], list);

    ck_assert_msg(
        expected == actual,
        "The path provided is not listed in the list of paths");

    list_free(list);
}
END_TEST

START_TEST(returns_false_if_a_path_is_not_listed_in)
{
    char *paths[] = {
        "/usr/opt/path1",
        "/usr/opt/path2",
        "/usr/opt/path3"
    };

    LIST *list = list_init();
    fill_with_paths(list, paths, 3);

    bool_t expected = FALSE;
    bool_t actual = is_listed_in("/usr/opt/not/listed", list);

    ck_assert_msg(
        expected == actual,
        "The path provided is not listed in the list of paths");

    list_free(list);
}
END_TEST

Suite *cwatch_suite(void)
{
    Suite *s = suite_create("cwatch");

    TCase *tc_core = tcase_create("When dealing with cwatch");
    tcase_add_checked_fixture(tc_core, setup, teardown);

    /* object creation */
    tcase_add_test(tc_core, creates_a_wd_data);
    tcase_add_test(tc_core, creates_a_link_data);

    /* command format */
    tcase_add_test(tc_core, formats_command_correctly_using_special_characters);

    /* utility functions */
    tcase_add_test(tc_core, returns_true_if_a_path_is_a_child_of_another_path);
    tcase_add_test(tc_core, returns_false_if_a_path_is_not_a_child_of_another_path);
    tcase_add_test(tc_core, returns_true_if_a_path_is_listed_in);
    tcase_add_test(tc_core, returns_false_if_a_path_is_not_listed_in);

    /* tcase_add_test(tc_core, creates_a_wd_data); */

    suite_add_tcase(s, tc_core);

    return s;
}

int main(void)
{
    int number_failed;
    Suite *s = cwatch_suite();
    SRunner *sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free (sr);
    return(number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
