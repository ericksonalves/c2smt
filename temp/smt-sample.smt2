(declare-const c0 Real)
(declare-const n0 Int)
(assert (= n0 4))
(assert (> n0 0))
(declare-const c1 Real)
(assert (= c1 (/ 10 n0)))
(declare-const n1 Int)
(assert (= n1 (- n0 1)))
(assert (> n1 0))
(declare-const c2 Real)
(assert (= c2 (/ 10 n1)))
(declare-const n2 Int)
(assert (= n2 (- n1 1)))
(assert (> n2 0))
(declare-const c3 Real)
(assert (= c3 (/ 10 n2)))
(declare-const n3 Int)
(assert (= n3 (- n2 1)))
(assert (> n3 0))
(declare-const c4 Real)
(assert (= c4 (/ 10 n3)))
(declare-const n4 Int)
(assert (= n4 (- n3 1)))
;(assert (> n4 0))
(check-sat)
