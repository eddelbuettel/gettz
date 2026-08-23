## gettz: Get System Timezone Information

[![CI](https://github.com/eddelbuettel/gettz/workflows/ci/badge.svg)](https://github.com/eddelbuettel/gettz/actions?query=workflow%3Aci)
[![License](https://img.shields.io/badge/license-GPL%20%28%3E=%202%29-brightgreen.svg?style=flat)](https://www.gnu.org/licenses/gpl-2.0.html) 
[![CRAN](https://www.r-pkg.org/badges/version/gettz)](https://cran.r-project.org/package=gettz) 
[![Dependencies](https://tinyverse.netlify.app/badge/gettz)](https://cran.r-project.org/package=gettz)
[![r-universe](https://eddelbuettel.r-universe.dev/badges/gettz)](https://eddelbuettel.r-universe.dev/gettz)
[![Downloads](https://cranlogs.r-pkg.org/badges/gettz?color=brightgreen)](https://www.r-pkg.org:443/pkg/gettz)
[![Last Commit](https://img.shields.io/github/last-commit/eddelbuettel/gettz)](https://github.com/eddelbuettel/gettz)

### Original Motivation

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
[in this StackOverflow answer](https://stackoverflow.com/a/33881726/143305). This
package essentially adapted his code to be callable from R.

In the years since 2016 (when this was originally written), R itself became more robust
so the packages for which this has been used no longer require it though 
[anytime](https://github.com/eddelbuettel/anytime) still suggests it.

### Status

Works on Linux, expected to also work on Solaris and BSD, builds on OS X and
Windows but only returns `NULL` due to the apparent lack of any programmatic
interface. As noted in the preceding section, it should no longer be needed.

Windows could possibly be supported better by querying the
registry. Contributions / pull requests would be welcomed on this.

### Installation

The package is now on [CRAN](https://cran.r-project.org) and can be installed
via a standard

```r
install.packages("gettz")
```

command.

### Author

Dirk Eddelbuettel

### License

GPL (>= 2)
