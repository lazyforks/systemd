/* SPDX-License-Identifier: LGPL-2.1-or-later */

#include "hostname-util.h"
#include "string-util.h"
#include "tests.h"

TEST(hostname_is_valid) {
        assert_se(hostname_is_valid("foobar", 0));
        assert_se(hostname_is_valid("foobar.com", 0));
        assert_se(!hostname_is_valid("foobar.com.", 0));
        assert_se(hostname_is_valid("fooBAR", 0));
        assert_se(hostname_is_valid("fooBAR.com", 0));
        assert_se(!hostname_is_valid("fooBAR.", 0));
        assert_se(!hostname_is_valid("fooBAR.com.", 0));
        assert_se(!hostname_is_valid("fööbar", 0));
        assert_se(!hostname_is_valid("", 0));
        assert_se(!hostname_is_valid(".", 0));
        assert_se(!hostname_is_valid("..", 0));
        assert_se(!hostname_is_valid("foobar.", 0));
        assert_se(!hostname_is_valid(".foobar", 0));
        assert_se(!hostname_is_valid("foo..bar", 0));
        assert_se(!hostname_is_valid("foo.bar..", 0));
        assert_se(!hostname_is_valid("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", 0));
        assert_se(!hostname_is_valid("au-xph5-rvgrdsb5hcxc-47et3a5vvkrc-server-wyoz4elpdpe3.openstack.local", 0));

        assert_se(hostname_is_valid("foobar", VALID_HOSTNAME_TRAILING_DOT));
        assert_se(hostname_is_valid("foobar.com", VALID_HOSTNAME_TRAILING_DOT));
        assert_se(hostname_is_valid("foobar.com.", VALID_HOSTNAME_TRAILING_DOT));
        assert_se(hostname_is_valid("fooBAR", VALID_HOSTNAME_TRAILING_DOT));
        assert_se(hostname_is_valid("fooBAR.com", VALID_HOSTNAME_TRAILING_DOT));
        assert_se(!hostname_is_valid("fooBAR.", VALID_HOSTNAME_TRAILING_DOT));
        assert_se(hostname_is_valid("fooBAR.com.", VALID_HOSTNAME_TRAILING_DOT));
        assert_se(!hostname_is_valid("fööbar", VALID_HOSTNAME_TRAILING_DOT));
        assert_se(!hostname_is_valid("", VALID_HOSTNAME_TRAILING_DOT));
        assert_se(!hostname_is_valid(".", VALID_HOSTNAME_TRAILING_DOT));
        assert_se(!hostname_is_valid("..", VALID_HOSTNAME_TRAILING_DOT));
        assert_se(!hostname_is_valid("foobar.", VALID_HOSTNAME_TRAILING_DOT));
        assert_se(!hostname_is_valid(".foobar", VALID_HOSTNAME_TRAILING_DOT));
        assert_se(!hostname_is_valid("foo..bar", VALID_HOSTNAME_TRAILING_DOT));
        assert_se(!hostname_is_valid("foo.bar..", VALID_HOSTNAME_TRAILING_DOT));
        assert_se(!hostname_is_valid("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", VALID_HOSTNAME_TRAILING_DOT));

        ASSERT_FALSE(hostname_is_valid("foo??bar", 0));
        ASSERT_TRUE(hostname_is_valid("foo??bar", VALID_HOSTNAME_QUESTION_MARK));
}

TEST(hostname_cleanup) {
        char *s;

        s = strdupa_safe("foobar");
        ASSERT_STREQ(hostname_cleanup(s), "foobar");
        s = strdupa_safe("foobar.com");
        ASSERT_STREQ(hostname_cleanup(s), "foobar.com");
        s = strdupa_safe("foobar.com.");
        ASSERT_STREQ(hostname_cleanup(s), "foobar.com");
        s = strdupa_safe("foo-bar.-com-.");
        ASSERT_STREQ(hostname_cleanup(s), "foo-bar.com");
        s = strdupa_safe("foo-bar-.-com-.");
        ASSERT_STREQ(hostname_cleanup(s), "foo-bar--com");
        s = strdupa_safe("--foo-bar.-com");
        ASSERT_STREQ(hostname_cleanup(s), "foo-bar.com");
        s = strdupa_safe("fooBAR");
        ASSERT_STREQ(hostname_cleanup(s), "fooBAR");
        s = strdupa_safe("fooBAR.com");
        ASSERT_STREQ(hostname_cleanup(s), "fooBAR.com");
        s = strdupa_safe("fooBAR.");
        ASSERT_STREQ(hostname_cleanup(s), "fooBAR");
        s = strdupa_safe("fooBAR.com.");
        ASSERT_STREQ(hostname_cleanup(s), "fooBAR.com");
        s = strdupa_safe("fööbar");
        ASSERT_STREQ(hostname_cleanup(s), "fbar");
        s = strdupa_safe("");
        assert_se(isempty(hostname_cleanup(s)));
        s = strdupa_safe(".");
        assert_se(isempty(hostname_cleanup(s)));
        s = strdupa_safe("..");
        assert_se(isempty(hostname_cleanup(s)));
        s = strdupa_safe("foobar.");
        ASSERT_STREQ(hostname_cleanup(s), "foobar");
        s = strdupa_safe(".foobar");
        ASSERT_STREQ(hostname_cleanup(s), "foobar");
        s = strdupa_safe("foo..bar");
        ASSERT_STREQ(hostname_cleanup(s), "foo.bar");
        s = strdupa_safe("foo.bar..");
        ASSERT_STREQ(hostname_cleanup(s), "foo.bar");
        s = strdupa_safe("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
        ASSERT_STREQ(hostname_cleanup(s), "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
        s = strdupa_safe("xxxx........xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
        ASSERT_STREQ(hostname_cleanup(s), "xxxx.xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
}

DEFINE_TEST_MAIN(LOG_DEBUG);
