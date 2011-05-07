AC_DEFUN(AEGIS_INIT,[
AC_ARG_WITH(aegis, [  --with-aegis=PROJECT    Use the aegis view path when building ],
[
if test "$withval" != "no" ; then 
AC_CHECK_PROG(HAVE_AEGIS,aegis,yes)
if test "$HAVE_AEGIS"x != "yesx" ; then 
  AC_MSG_ERROR("Aegis is not installed") ; 
fi ;

if test "$withval" = "yes" ; then 
  AC_MSG_ERROR([ "When building with aegis, you must specify the project name" ]) 
fi ;
for p in  `aegis -list -terse projects` ; do
	if test "$p" = "$withval" ; then 
	  aegisProject=$withval;
	  break ;
	fi
done ;
if test "$aegisProject" = "" ; then 
  AC_MSG_ERROR([ "$withval is not an aegis project" ]) ;
fi ;
   AEPATH='VPATH=$(shell aesub '"'"'$$search_path'"'"') # : '
fi
],
AEPATH='##';
)
])



dnl A macro to test if the compiler (preprocessor) recognises __FUNCTION__     
AC_DEFUN(AC_C_FUNCTION, [
 AC_CACHE_CHECK([whether the C compiler knows about __FUNCTION__],ac_cv_c_function,[
 AC_LANG_C
 AC_TRY_COMPILE( , [ 
int myfunc(x)
{
	char X[]=__FUNCTION__;
	return 0 ; 
}
],ac_cv_c_function=yes,ac_cv_c_function=no)
])
 if test ac_cv_c_function = yes ; then 
	AC_DEFINE(HAVE_FUNCTION)
 fi
])
