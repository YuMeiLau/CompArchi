;; Really sets r to
;; <0, if a<b
;;  0, if a==b
;; >0, if a>b

;;  Returned value
;;         |  a<0  |  a>=0 |
;;  ------ +-------+-------+
;;   b<0   |  a-b  |   a   |
;;  ------ +-------+-------+
;;   b>=0  |   a   |  a-b  |
;;  ------ +-------+-------+

       .ORIG   x3000
cint    LD     R3,b15
        LD     R1,a
        LD     R2,b
        AND    R3,R3,R1              ;; R3 contains the sign bit of R1.
        ADD    R3,R3,R2              ;; Negative only if R1 and R2
        BRzp   cmp                   ;;    have different signs.

;; a and b have different signs
retA    ST     R1,r
        BR     leave

;; a and b have same sign
cmp     NOT    R2,R2
        ADD    R2,R2,#1
        ADD    R2,R1,R2
        ST     R2,r

leave   HALT

b15    .FILL   x8000    
a      .FILL   #-20000
b      .FILL   #20000
r      .BLKW   1
       .END