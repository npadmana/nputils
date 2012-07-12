# Specify the BOSS_TARGET1 galaxy flags; these are all in the lower 32 bits
# so we don't need to worry about the lack of a native 64 bit int in R
boss.target1 <- list()
boss.target1[["GAL_LOZ"]] <- 0L
boss.target1[["GAL_CMASS"]] <- 1L
boss.target1[["GAL_CMASS_COMM"]] <- 2L
boss.target1[["GAL_CMASS_SPARSE"]] <- 3L
boss.target1[["SDSS_GAL_KNOWN"]] <- 6L
boss.target1[["GAL_CMASS_ALL"]] <- 7L


boss.ComputeAuxColors <- function(data) {
  # Computes the BOSS galaxy TS auxiliary colors cperp, cpar and dperp
  #
  # Args :
  #   data : a data frame with MODELFLUX_[2,3,4] and EXTINCTION_[2,3,4] 
  #      The naming conventions come from STILTS loading an SQL database
  #
  # Returns :
  #   A modified data frame with cperp, cpar, and dperp. 
  g <- boss.Flux2Mag(data$MODELFLUX_2) - data$EXTINCTION_2
  r <- boss.Flux2Mag(data$MODELFLUX_3) - data$EXTINCTION_3
  i <- boss.Flux2Mag(data$MODELFLUX_4) - data$EXTINCTION_4
  dperp <- (r-i) - (g-r)/8.0
  cperp <- (r-i) - (g-r)/4.0 - 0.18
  cpar <- 0.7*(g-r) + 1.2*(r-i-0.18)
  return(cbind(data, g, r, i))
}