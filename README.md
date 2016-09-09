## gettz

Get System Timezone Information

### Motivation

Many date and time conversion need information about the local timezone. It
is common to set the environment variable `TZ`.  However, sometimes it is
unset, and we need a fallback.  Or maybe we just want a fallback to not
always have to set the `TZ` variable by hand.

R has a helper function `Sys.timezone()` which queries the system timezone
information. Sadly, we have seen it fail (possibly because `/etc/localtime`
was not a symbolic link as it expected, but rather a copy of the
corresponding (local) timezone metadata).  Another issue is, once again, that
the code which R uses here is sadly _not_ accessible to users via a callable
API for C or C++.  So there is a need for something different---and Duane
McCully provided just that
[in this StackOverflow answer](http://stackoverflow.com/a/33881726/143305). This
package essentially adapted his code to be callable from R.

### Status

Works on Linux, expected to also work on Solaris and BSD, builds on OS
X but only returns `NULL` due to the apparent lack of any programmatic
interface.

Windows could possibly be supported by querying the registry.

### Author

Dirk Eddelbuettel

### License

GPL (>= 2)
