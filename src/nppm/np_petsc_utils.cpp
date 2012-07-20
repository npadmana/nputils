#include "np_petsc_utils.h"

void safeCall(PetscErrorCode n, const char * mess) {
	if (n) {
		PetscError(PETSC_COMM_WORLD, __LINE__,__FUNCT__,__FILE__,__SDIR__,n,PETSC_ERROR_IN_CXX,mess);
		// We should have thrown by now, but if not, die.
		MPI_Abort(PETSC_COMM_WORLD, 99);
	}
}
