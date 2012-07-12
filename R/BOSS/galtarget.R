# Specify the BOSS_TARGET1 galaxy flags; these are all in the lower 32 bits
# so we don't need to worry about the lack of a native 64 bit int in R
bosstarget1 <- list()
bosstarget1[["GAL_LOZ"]] <- 0L
bosstarget1[["GAL_CMASS"]] <- 1L
bosstarget1[["GAL_CMASS_COMM"]] <- 2L
bosstarget1[["GAL_CMASS_SPARSE"]] <- 3L
bosstarget1[["SDSS_GAL_KNOWN"]] <- 6L
bosstarget1[["GAL_CMASS_ALL"]] <- 7L

boss_flagname2val <- function (names, flagdefs) {
  tmpfun <- function(name1) return(flagdefs[[name1]])
  sum(2L^sapply(names, tmpfun))
}