      INTEGER*4 FUNCTION WBSTATE_ADDRESS() 
      IMPLICIT NONE

      DOUBLE PRECISION RATIOU,RATIOD,RATIOS 
      INTEGER NBOUND

      COMMON/WBSTATE/RATIOU,RATIOD,RATIOS,NBOUND

      INTEGER*4 GETADDR

      WBSTATE_ADDRESS = GETADDR(RATIOU)
      RETURN
      END
