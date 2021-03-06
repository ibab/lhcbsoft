C
C
C The next three subroutines are for Monte Carlo generation 
C according to a given function FHB. One calls first HIFUN 
C with assigned channel number I, low and up limits. Then to 
C generate the distribution one can call HIRND(I) which gives 
C you a random number generated according to the given function.
C 
	SUBROUTINE HIFUN(I,XMIN,XMAX,FHB)
	IMPLICIT REAL*8(A-H,O-Z)
	COMMON/HIJHB/RR(10,201),XX(10,201)
	SAVE  /HIJHB/
	EXTERNAL FHB
	FNORM=GAUSS1(FHB,XMIN,XMAX,0.001D0)
	DO 100 J=1,201
		XX(I,J)=XMIN+(XMAX-XMIN)*(J-1)/200.0d0
		XDD=XX(I,J)
		RR(I,J)=GAUSS1(FHB,XMIN,XDD,0.001D0)/FNORM
100	CONTINUE
	RETURN
	END
