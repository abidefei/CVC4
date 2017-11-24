; COMMAND-LINE: --uf-ho
; EXPECT: unsat
(set-logic ALL)
(set-info :status unsat)
(declare-sort U 0)
(declare-fun f (U) U)
(declare-fun a () U)
(declare-fun b () U)
(assert (forall ((x (-> U U))) (= (x a) b)))
(assert (not (= (f a) b)))
(check-sat)
