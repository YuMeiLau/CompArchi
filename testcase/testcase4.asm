;; Really sets r to
;; <0, if a<b
;;  0, if a==b
;; >0, if a>b


;;  Returned value
;;         |  a<0  |  a?=0 |
;;  ------ +-------+-------+
;;   b<0   |  a-b  |   a   |
;;  ------ +-------+-------+
;;   b>=0  |   a   |  a-b  |
;;  ------ +-------+-------+

       .ORIG    x3000
cint    LD      R1,a
        BRn     aNeg

;; a>=0, if here
        LD      R2,b
        BRn     retA
        BR      cmp

;; a<0, if here
aNeg    LD      R2,b
        BRn     cmp
;;      BR      retA

;; a and b have different signs
retA    ST      R1,r
        BR     leave

;; a and b have same sign
cmp     NOT    R2,R2
        ADD    R2,R2,#1
        ADD    R2,R1,R2
        ST     R2,r

leave   HALT

a      .FILL   #-20000
b      .FILL   #20000
r      .BLKW   1
       .END