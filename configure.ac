dnl Initialise autoconf

AC_INIT([cyphesis],[0.5.26],[alriddoch@googlemail.com])
AC_CONFIG_SRCDIR([server/server.cpp])
AC_CONFIG_HEADERS([config.h])

dnl Detect the canonical host environment

AC_CANONICAL_HOST

dnl Initialise automake

AM_INIT_AUTOMAKE([nostdinc dist-bzip2 check-news color-tests parallel-tests])

AM_BINRELOC

dnl Do not need libtool

dnl AC_PROG_LIBTOOL

COMMON_LIBS=
TERRAIN_LIBS=
NETWORK_LIBS=
MATH_LIBS=
MDNS_LIBS=
PGSQL_LIBS=

PYTHON_LINKER_FLAGS=
TAR_PERM_FLAGS=

dnl Test for C++ compiler options

AC_PROG_CXX
AC_PROG_CPP
AC_PROG_RANLIB

AC_PATH_PROG([XMLLINT], [xmllint], [true])

python_prefix=/usr
AC_ARG_WITH(python,
[  --with-python=DIR       Prefix where python is installed in [default=/usr]],
[
    if test $withval != yes; then
        python_prefix=$withval
    fi
])

dnl allow the user to specify that most libraries should be statically linked

STATIC_LIBSTDCPP=
STATIC_LIBGCC=

AC_ARG_ENABLE(static_link,
  [  --enable-static-link    enable static link of some libs [default=no]],
  [
    if test "$enableval" = "yes"; then
      link_static="true"
      STATIC_LIBSTDCPP=`$CXX -print-file-name=libstdc++.a`
      STATIC_LIBGCC=`$CXX -print-file-name=libgcc.a`
    else
      link_static="false"
    fi
  ],[
    link_static="false"
  ]
)
AM_CONDITIONAL(LINK_STATIC, test x"$link_static" = xtrue)

AC_MSG_CHECKING(whether gcc needs -fno-strict-aliasing)
AC_PREPROC_IFELSE([
#ifdef __GNUC__
# if defined(__GNUC_MINOR__) && __GNUC__ == 3 && (__GNUC_MINOR__ >= 3)
#   error -fno-strict-aliasing needed
# elif __GNUC__ > 3
#   error -fno-strict-aliasing needed
# endif
#endif
],[
  GCC_EXTRA_OPT=
  AC_MSG_RESULT(no)
],[
  GCC_EXTRA_OPT=-fno-strict-aliasing
  AC_MSG_RESULT(yes)
])

AC_ARG_ENABLE(debug,
    [  --enable-debug          enable debug information [default=no]],
    [
        if test "$enableval" = "yes"; then
            CXXFLAGS="$CXXFLAGS $GCC_EXTRA_OPT -Wall -DDEBUG"
        else
            CXXFLAGS="$CXXFLAGS $GCC_EXTRA_OPT -Wall -DNDEBUG"
        fi
    ],[
        CXXFLAGS="$CXXFLAGS $GCC_EXTRA_OPT -Wall -DNDEBUG"
    ]
)

dnl allow the user to provide the directory where python is installed in

AC_LANG(C++)

AC_CHECK_FUNCS([sigset sigaction epoll_create poll localtime_r syslog uname])
AC_CHECK_FUNCS([getuid nice fork setsid wait4 getppid kill inet_ntop])
AC_CHECK_FUNCS([unlink usleep Sleep gettimeofday getnameinfo])

AC_SEARCH_LIBS([cos], [m])

AC_MSG_CHECKING([whether we have res_init() available])
AC_LINK_IFELSE(
    [
      AC_LANG_PROGRAM(
      [[
        #include <resolv.h>
      ]],
      [[
        res_init();
      ]]
     )
    ],
    [
        AC_MSG_RESULT([yes])
        AC_DEFINE(HAVE_RES_INIT, 1, [Define to 1 if you have res_init libs.])
    ],
    [
        AC_MSG_RESULT([no])
    ]
)

AC_MSG_CHECKING([whether we should link to MacOS-X frameworks])
AC_COMPILE_IFELSE(
    [
      AC_LANG_PROGRAM(
        [[
          #include <CoreFoundation/CFBundle.h>
        ]],
        [[
          CFBundleRef appBundle = CFBundleGetMainBundle();
        ]]
      )
    ],
    [
        AC_MSG_RESULT([yes])
        COMMON_LIBS="$COMMON_LIBS -Wl,-framework,CoreFoundation"
    ],
    [
        AC_MSG_RESULT([no])
    ]
)

dnl General header checks
AC_CHECK_HEADERS(termios.h signal.h syslog.h sys/utsname.h sys/wait.h)
AC_CHECK_HEADERS(winsock.h arpa/inet.h sys/un.h dirent.h io.h sys/ucred.h)
AC_CHECK_HEADERS(windows.h getopt.h)

dnl then check for the header file Python.h and set
dnl python_include_path and python_version
dnl appropriately to what we have found
dnl
dnl if found define HAVE_PYTHON_H
dnl

AC_CHECK_HEADER(python2.7/Python.h,
[
    python_include_path=-I${python_prefix}/include/python2.7
    python_version=2.7
],[
    AC_CHECK_HEADER(python2.6/Python.h,
    [
        python_include_path=-I${python_prefix}/include/python2.6
        python_version=2.6
    ],[
        AC_CHECK_HEADER(python2.5/Python.h,
        [
            python_include_path=-I${python_prefix}/include/python2.5
            python_version=2.5
        ],[
            AC_CHECK_HEADER(python2.4/Python.h,
            [
                python_include_path=-I${python_prefix}/include/python2.4
                python_version=2.4
            ],[
                AC_CHECK_HEADER(python2.3/Python.h,
                [
                    python_include_path=-I${python_prefix}/include/python2.3
                    python_version=2.3
                ],[
                    AC_CHECK_HEADER(python2.2/Python.h,
                    [
                        python_include_path=-I${python_prefix}/include/python2.2
                        python_version=2.2
                    ],[
                        AC_MSG_ERROR([
Cannot find python headers for Python 2.2 or later.
Please see http://www.python.org/ for details of how to download and install
Python. If Python is installed somewhere other than in /usr the please use the
--with-python=DIR option to point to the prefix where Python is installed.])
                    ])
                ])
            ])
        ])
    ])
])

PYTHON_VERSION=${python_version}
PYTHON_INCLUDES=${python_include_path}
CPPFLAGS="$CPPFLAGS $PYTHON_INCLUDES"

dnl build the library path from the found version
python_lib_path=${python_prefix}/lib/python${python_version}/config

PYTHON_UTIL_LIBS=

AC_CHECK_LIB(dl,dlopen,
[
    PYTHON_UTIL_LIBS="$PYTHON_UTIL_LIBS -ldl"
])

AC_CHECK_LIB(util,openpty,
[
    PYTHON_UTIL_LIBS="$PYTHON_UTIL_LIBS -lutil"
])

AC_CHECK_FUNC(pthread_mutex_trylock, ,
[
    ac_save_CXXFLAGS="$CXXFLAGS"
    CXXFLAGS="$CXXFLAGS -pthread"
    AC_CHECK_FUNC(pthread_create, ,
    [
        CXXFLAGS="$ac_save_CXXFLAGS"
        AC_CHECK_LIB(pthread, pthread_create, ,
        [
            AC_MSG_WARN([Cannot find pthread library. Python lib check may fail])
        ])
    ])
])

if test x"$link_static" = "xfalse"; then

AC_CHECK_LIB(python${python_version}, Py_Initialize,
[
    python_libs="-lpython${python_version}"
    PYTHON_UTIL_LIBS=
],
[
    AC_CHECK_LIB(python${python_version}, Py_Initialize,
    [
        python_libs="-L${python_lib_path} -lpython${python_version}"
        PYTHON_LINKER_FLAGS="-export-dynamic"
    ],AC_MSG_ERROR([
Cannot find python libraries. Do you have python development installed? Please
see http://www.python.org/ for details of how to download and install Python.]),
    [-L${python_lib_path} ${PYTHON_UTIL_LIBS}])
])

else

AC_CHECK_LIB(python${python_version}, Py_Initialize,
[
    python_libs="-L${python_lib_path} -lpython${python_version}"
    PYTHON_LINKER_FLAGS="-export-dynamic"
],AC_MSG_ERROR([
Cannot find python libraries. Do you have python development installed?
Please see http://www.python.org/ for details of how to download and install
Python.]),
[-L${python_lib_path} ${PYTHON_UTIL_LIBS}])

fi

PYTHON_LIBS=${python_libs}

AM_PATH_LIBGCRYPT(1.2.0,
[
    CPPFLAGS="$CPPFLAGS $LIBGCRYPT_CFLAGS"
    COMMON_LIBS="$COMMON_LIBS $LIBGCRYPT_LIBS"
],
[
    AC_MSG_ERROR([
Cannot find libgcrypt for encrypted passwords. Please see
http://directory.fsf.org/security/libgcrypt.html for details of how to
download and install libgcrypt development libraries. If it is installed
please ensure libgcrypt-config is on the PATH.])
])

PKG_PROG_PKG_CONFIG

dnl PKG_CHECK_MODULES(MONO, mono,
dnl     [
dnl         CPPFLAGS="$CPPFLAGS $MONO_CFLAGS"
dnl         COMMON_LIBS="$COMMON_LIBS $MONO_LIBS"
dnl     ])

PKG_CHECK_MODULES(DEPS, atlascpp-0.6 varconf-1.0 >= 0.6.4,
    [
        CPPFLAGS="$CPPFLAGS $DEPS_CFLAGS"
        COMMON_LIBS="$COMMON_LIBS $DEPS_LIBS"
    ],
    [
        AC_MSG_RESULT([no])
        AC_MSG_ERROR([
Cannot find valid versions of required WorldForge libraries:
$DEPS_PKG_ERRORS
Perhaps you should add the directory containing the missing libraries to the
PKG_CONFIG_PATH environment variable.
Please see http://worldforge.org/dev/eng/servers/cyphesis for details of the
libraries required and where to obtain them.])
    ])

PKG_CHECK_MODULES(WFMATH, wfmath-0.3 >= 0.3.11,
    [
        MATH_LIBS="$MATH_LIBS $WFMATH_LIBS"
    ],
    [
        AC_MSG_RESULT([no])
        AC_MSG_ERROR([
Cannot find valid versions of required WorldForge maths library, wfmath:
$WFMATH_PKG_ERRORS
Perhaps you should add the directory containing the missing libraries to the
PKG_CONFIG_PATH environment variable.
Please see http://worldforge.org/dev/eng/servers/cyphesis for details of the
libraries required and where to obtain them.])
    ])

PKG_CHECK_MODULES(MERCATOR, mercator-0.3 >= 0.3.0,
    [
        CPPFLAGS="$CPPFLAGS $MERCATOR_CFLAGS"
        TERRAIN_LIBS="$TERRAIN_LIBS $MERCATOR_LIBS"
    ],
    [
        AC_MSG_RESULT([no])
        AC_MSG_ERROR([
Cannot find valid versions of required WorldForge terrain library, mercator:
$MERCATOR_PKG_ERRORS
Perhaps you should add the directory containing the missing libraries to the
PKG_CONFIG_PATH environment variable.
Please see http://worldforge.org/dev/eng/servers/cyphesis for details of the
libraries required and where to obtain them.])
    ])

PKG_CHECK_MODULES(SKSTREAM, skstream-0.3 >= 0.3.6,
    [
        CPPFLAGS="$CPPFLAGS $SKSTREAM_CFLAGS"
        NETWORK_LIBS="$NETWORK_LIBS $SKSTREAM_LIBS"
    ],
    [
        AC_MSG_RESULT([no])
        AC_MSG_ERROR([
Cannot find valid versions of required WorldForge network library, skstream:
$SKSTREAM_PKG_ERRORS
Perhaps you should add the directory containing the missing libraries to the
PKG_CONFIG_PATH environment variable.
Please see http://worldforge.org/dev/eng/servers/cyphesis for details of the
libraries required and where to obtain them.])
    ])

PKG_CHECK_MODULES(AVAHI, avahi-client,
    [
        CPPFLAGS="$CPPFLAGS $AVAHI_CFLAGS"
        MDNS_LIBS="$MDNS_LIBS $AVAHI_LIBS"
        AC_DEFINE(HAVE_AVAHI, 1, [Define to 1 if you have avahi libs.])
    ],
    [
        AC_MSG_RESULT([no])
        PKG_CHECK_MODULES(HOWL, howl,
            [
                CPPFLAGS="$CPPFLAGS $HOWL_CFLAGS"
                MDNS_LIBS="$MDNS_LIBS $HOWL_LIBS"
                AC_DEFINE(HAVE_LIBHOWL, 1, [Define to 1 if you have howl libs.])
            ],
            [
                AC_MSG_RESULT([no])
                AC_MSG_NOTICE([Ommiting DNS-SD functionality.])
            ])
    ])

AM_PATH_PSQL(7.1.0,
    [
        if test "$PG_CFLAGS" != "-I/usr/include"; then
            CPPFLAGS="$CPPFLAGS $PG_CFLAGS"
        fi
        if test "$PG_LIBS" != "-L/usr/lib"; then
            PGSQL_LIBS="$PGSQL_LIBS $PG_LIBS"
        fi
    ],
    AC_MSG_ERROR([
Cannot find PostgreSQL config or config failed. Please install PostgreSQL and
ensure than pg_config is on the PATH. See http://www.postgresql.org/ for
details of how to download the software.])
)

ac_save_LIBS="$LIBS"
LIBS="$LIBS $PG_LIBS"
AC_CHECK_LIB(pq,PQconnectdb,
[
    PGSQL_LIBS="$PGSQL_LIBS -lpq"
],
[
    AC_CHECK_LIB(pq,PQconnectStart,
    [
        PGSQL_LIBS="$PGSQL_LIBS -lpq -lws2_32 -lwsock32 -lsecur32"
    ],
    [
        AC_MSG_ERROR([
Cannot find PostgreSQL bindings! Have you installed the PostgreSQL development
software?])
    ],[-lws2_32 -lwsock32 -lsecur32])
])
LIBS="$ac_save_LIBS"

READLINE_LIBS=

AC_CHECK_LIB(termcap,tgetent, 
    [READLINE_LIBS="$READLINE_LIBS -ltermcap"],
    AC_CHECK_LIB(ncurses, tgetent, 
        [READLINE_LIBS="$READLINE_LIBS -lncurses"],
        AC_MSG_WARN([
Cannot find tgetent in termcap or ncurses libraries!
This may be a problem for readline.])
    )
)

AC_CHECK_LIB(readline,readline,
    [
        READLINE_LIBS="$READLINE_LIBS -lreadline"
        AC_COMPILE_IFELSE(
          [
            AC_LANG_PROGRAM(
              [[
                #include <stdio.h>
                #include <readline/readline.h>
              ]],
              [[
                readline("test > ")
              ]]
            )
          ],
          [
            AC_DEFINE(READLINE_CXX_SANE, 1, Define if readline headers are C++ aware)
          ],
          [
            AC_MSG_WARN(Readline C++ workaround enabled)
          ]
        )
    ],
    [
        AC_MSG_WARN([
Cannot find readline! Please see http://directory.fsf.org/readline.html for
details of how to download and install the missing library.])
    ],[ $READLINE_LIBS ]
)

AC_MSG_CHECKING([whether tar allows permissions to be overriden])
if tar cf /dev/null --mode o+rX /dev/null 2> /dev/null ; then
    TAR_PERM_FLAGS="--mode o+rX"
    AC_MSG_RESULT([yes])
else
    AC_MSG_RESULT([no])
fi

AC_SUBST(VARCONF_LIBS)
AC_SUBST(READLINE_LIBS)
AC_SUBST(COMMON_LIBS)
AC_SUBST(TERRAIN_LIBS)
AC_SUBST(NETWORK_LIBS)
AC_SUBST(MATH_LIBS)
AC_SUBST(MDNS_LIBS)
AC_SUBST(PYTHON_VERSION)
AC_SUBST(PYTHON_LIBS)
AC_SUBST(PYTHON_UTIL_LIBS)
AC_SUBST(python_version)
AC_SUBST(PGSQL_LIBS)

AC_SUBST(STATIC_LIBSTDCPP)
AC_SUBST(STATIC_LIBGCC)

AC_SUBST(PYTHON_LINKER_FLAGS)
AC_SUBST(TAR_PERM_FLAGS)

dnl Generate files
AC_CONFIG_FILES([
    cyphesis.spec
    cyphesis.iss
    Makefile
    libraries/Makefile
    physics/Makefile
    common/Makefile
    modules/Makefile
    rulesets/Makefile
    server/Makefile
    client/Makefile
    tools/Makefile
    data/Makefile
    tests/Makefile
    man/Makefile
    dox/Makefile
    autopackage/default.apspec
])
AC_OUTPUT
