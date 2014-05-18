PHP_ARG_ENABLE(highlight, Enable test support)

if test "$PHP_HIGHLIGHT" = "yes"; then
	PHP_REQUIRE_CXX()
	PHP_ADD_LIBRARY(stdc++,, HIGHLIGHT_SHARED_LIBADD)
	PHP_SUBST(HIGHLIGHT_SHARED_LIBADD)
	PHP_NEW_EXTENSION(highlight, main.cpp data.cpp module/php.cpp module/html.cpp module/css.cpp module/js.cpp module/sql.cpp, $ext_shared, ,"-std=c++0x")
fi
