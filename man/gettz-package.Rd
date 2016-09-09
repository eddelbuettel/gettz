\name{gettz-package}
\alias{gettz-package}
\alias{gettz}
\docType{package}
\title{\packageTitle{gettz}}
\description{\packageDescription{gettz}}
\details{
  R has a function \code{\link{Sys.timezone}} which tries hard to
  retrieve the same information.  However, we have found instances
  where it failed to provide an answer---whereas the simple code in
  this package does.

  This package provides a single function \code{gettz} which looks
  at \code{/etc/timezone} (on Debian/Ubuntu), \code{/etc/sysconfig/clock}
  (on RedHat/CentOS/Fedora) or \code{/etc/TIMEZONE} (on Solaris).  
  In case no information can be provided, \code{NULL} is returned.

  Windows is not currently supported, but could probably deploy
  custom code to query the registry.  
}
\references{\url{http://stackoverflow.com/a/33881726/143305}}
\author{\packageAuthor{gettz}}
\keyword{package}
\examples{
  gettz()
}
