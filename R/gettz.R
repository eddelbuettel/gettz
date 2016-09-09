
gettz <- function() {
    val <- .Call("gettz_impl");
    return(val)                         #return visibly
}
