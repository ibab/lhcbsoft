C $Id: gfuntrans.F,v 1.1.1.1 2006-04-24 21:45:50 robbep Exp $
      INTEGER*4 FUNCTION FUNTRANS_ADDRESS() 
      IMPLICIT NONE

      INTEGER NQ2,NPDFU

      COMMON/FUNTRANS/NQ2,NPDFU
 
      INTEGER*4 GETADDR

      FUNTRANS_ADDRESS = GETADDR(NQ2)
      RETURN
      END
