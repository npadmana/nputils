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
  #   data : a data table/frame with MODELFLUX_[2,3,4] and EXTINCTION_[2,3,4] 
  #      The naming conventions come from STILTS loading an SQL database
  #
  # Returns :
  #   A modified data table/frame with cperp, cpar, and dperp
  #
  # NOTE : This is not necessarily memory efficient. But it's designed to 
  # be easy to read.
  within(data,{
    
    # Model colors
    g <- boss.Flux2Mag(MODELFLUX_2) - EXTINCTION_2
    r <- boss.Flux2Mag(MODELFLUX_3) - EXTINCTION_3
    i <- boss.Flux2Mag(MODELFLUX_4) - EXTINCTION_4
    gr <- g-r
    ri <- r-i
    dperp <- (ri) - (gr)/8.0
    cperp <- (ri) - (gr)/4.0 - 0.18
    cpar <- 0.7*(gr) + 1.2*(ri-0.18)
    
    # Fluxes and magnitudes
    rcmodel <- boss.Flux2Mag(CMODELFLUX_3) - EXTINCTION_3
    icmodel <- boss.Flux2Mag(CMODELFLUX_4) - EXTINCTION_4
  })
}

boss.SelectCMASS <- function(data) {
  # Selects CMASS objects and adds in a new column in the dataframe : is.CMASS
  #
  # Args :
  #    data : A data frame with appropriate fluxes etc calculated. 
  #      Assumed that ComputeAuxColors has been run on this
  #
  # Result : 
  #    A new data frames with is.CMASS appended to it.
  #
  # TODO : ONLY THE SLIDING CMASS CUT HAS BEEN IMPLEMENTED
  data[, is.CMASS := icmodel < (19.86 + 1.6*(dperp-0.8))]
}

boss.SelectLOWZ <- function(data) {
  # Selects LOWZ objects and adds in a new column in the dataframe : is.LOWZ
  #
  # Args :
  #    data : A data frame with appropriate fluxes etc calculated. 
  #      Assumed that ComputeAuxColors has been run on this
  #
  # Result : 
  #    A new data frames with is.LOWZ appended to it.
  #
  # TODO : ONLY THE CPERP AND CPAR CUTS HAVE BEEN IMPLEMENTED
  data[, is.LOWZ := rcmodel < (13.5 + cpar/0.3)]
  data[, is.LOWZ := is.LOWZ & (abs(cperp) < 0.2)]
}

boss.SelectSPARSE <- function(data) {
  # Selects SPARSE objects and adds in a new column in the dataframe : is.SPARSE
  #
  # Args :
  #    data : A data frame with appropriate fluxes etc calculated. 
  #      Assumed that ComputeAuxColors has been run on this
  #
  # Result : 
  #    A new data frames with is.SPARSE appended to it.
  #
  # TODO : ONLY THE SLIDING CUT HAS BEEN IMPLEMENTED
  data[, is.SPARSE := icmodel >= (19.86 + 1.6*(dperp-0.8))]
  data[, is.SPARSE := is.SPARSE & (icmodel < (20.14 + 1.6*(dperp-0.8)))]
}

boss.galTypes <- list()
boss.galTypes[["LOWZ"]] <- list(name="GAL_LOZ", func=boss.SelectLOWZ)
boss.galTypes[["CMASS"]] <- list(name="GAL_CMASS", func=boss.SelectCMASS)
boss.galTypes[["SPARSE"]] <- list(name="GAL_CMASS_SPARSE", func=boss.SelectSPARSE)