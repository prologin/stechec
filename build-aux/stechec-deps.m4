# -*- shell-script -*-

#
# STECHEC_ADDMODULE([module-name])
#
AC_DEFUN([STECHEC_ADDMODULE],
[
    existing_modules="$existing_modules,$1"
    stechec_cond_$1_set() {
        AM_CONDITIONAL([BUILD_$1], [true])
    }
    stechec_cond_$1_unset() {
        AM_CONDITIONAL([BUILD_$1], [false])
    }
])

#
# STECHEC_PUSHDEP([dependency])
#
AC_DEFUN([STECHEC_PUSHDEP],
[
    case $DEPENDS in
        *$1*) ;; *) DEPENDS="$DEPENDS $1" ;;
    esac
])

#
# STECHEC_CONFIG_DEPENDENCY()
#
# Yes, I do not know m4... And do not like it, either.
#
AC_DEFUN([STECHEC_CONFIG_DEPENDENCY],
[

stechec_adddep() 
{
    dep=[$]1 # hum.
    case $dep in
        sdl) STECHEC_PUSHDEP([sdl]) ;;
        qt) STECHEC_PUSHDEP([qt]) ;;
        mysql) STECHEC_PUSHDEP([mysql]) ;;
        readline) STECHEC_PUSHDEP([readline]) ;;
        *)
            sub_deps=$(eval echo "\$DEPENDS_$dep")
            if test "x$sub_deps" != x; then
                stechec_getdeps $dep
            else
                AC_MSG_ERROR([unknown dependency: $dep])
            fi
            ;;
    esac
}

stechec_getdeps()
{
    for mod in [$]1; do
        used_modules="$used_modules,$mod"
        deps=$(eval echo "\$DEPENDS_$mod")
        for dep in $deps; do
            stechec_adddep "$dep"
        done
    done
}

save_IFS=$IFS
IFS=",$IFS"
stechec_getdeps "$tbt_cv_with_modules"
for mod in $existing_modules; do
    case $used_modules in
        *${mod}*) stechec_cond_${mod}_set ;;
        *) stechec_cond_${mod}_unset ;;
    esac
done 2> /dev/null
IFS=$save_IFS

])
