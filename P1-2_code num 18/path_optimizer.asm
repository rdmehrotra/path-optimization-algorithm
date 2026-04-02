.data
Diamond: .alloc 121

.text
PathOut:
             addi a7, x0, 514
             addi a0, gp, Diamond
             ecall

             # start at ring2 (ring1 is unchanged because center weight is 0)
             # pre-increment loop starts from ring1:
             # top=49, right=61 then advances to ring2 each iteration.
             addi t0, a0, 196
             add gp, a0, a0
             addi gp, gp, 480        # mirror constant: top+bottom (in bytes)
             addi t3, a0, 244

RING_LOOP:
             # advance to next ring (ring2..ring5)
             addi t0, t0, -44
             addi t3, t3, 4

             # top axis
             lw t5, 0(t0)
             lw t6, 44(t0)
             add t5, t5, t6
             sw t5, 0(t0)

             # bottom axis
             sub t5, gp, t0
             lw t6, 0(t5)
             lw a0, -44(t5)
             add t6, t6, a0
             sw t6, 0(t5)

             # left axis: left = mirror - right
             sub t6, gp, t3
             lw t5, 0(t6)
             lw a0, 4(t6)
             add t5, t5, a0
             sw t5, 0(t6)

             # right axis
             lw t5, 0(t3)
             lw t6, -4(t3)
             add t5, t5, t6
             sw t5, 0(t3)

             addi a0, t0, 48
             addi a7, t0, 40

DIAG_LOOP:
             # UR: preds -4 and +44
             lw t6, -4(a0)
             lw t5, 44(a0)
             bge t6, t5, UR_MIN_OK
             add t5, t6, x0
UR_MIN_OK:
             lw t6, 0(a0)
             add t6, t6, t5
             sw t6, 0(a0)

             # UL: preds +4 and +44
             lw t6, 4(a7)
             lw t5, 44(a7)
             bge t6, t5, UL_MIN_OK
             add t5, t6, x0
UL_MIN_OK:
             lw t6, 0(a7)
             add t6, t6, t5
             sw t6, 0(a7)

             # DL: mirror of UR, preds +4 and -44
             sub t6, gp, a0
             lw a1, 4(t6)
             lw t5, -44(t6)
             bge a1, t5, DL_MIN_OK
             add t5, a1, x0
DL_MIN_OK:
             lw a1, 0(t6)
             add a1, a1, t5
             sw a1, 0(t6)

             # DR: mirror of UL, preds -4 and -44
             sub t6, gp, a7
             lw a1, -4(t6)
             lw t5, -44(t6)
             bge a1, t5, DR_MIN_OK
             add t5, a1, x0
DR_MIN_OK:
             lw a1, 0(t6)
             add a1, a1, t5
             sw a1, 0(t6)

             addi a0, a0, 48
             addi a7, a7, 40
             bne a0, t3, DIAG_LOOP

             addi t5, t0, 240        # equals right ptr only on ring5
             bne t3, t5, RING_LOOP

SCAN_MIN:
             # ring5 edge minimum via 5 symmetric samples of (UR, UL, DL, DR)
             # coverage is exact when starting UR=top and UL=55.
             # a7 already equals UL index 55 when ring5 DIAG_LOOP exits.
             # a1 already holds edge index 75 from the final DR update.

MIN_LOOP:
             lw t5, 0(t0)            # UR
             bge t5, a1, MIN_UL
             add a1, t5, x0
MIN_UL:
             lw t5, 0(a7)            # UL
             bge t5, a1, MIN_DL
             add a1, t5, x0
MIN_DL:
             sub t5, gp, t0          # DL ptr
             lw t5, 0(t5)
             bge t5, a1, MIN_DR
             add a1, t5, x0
MIN_DR:
             sub t5, gp, a7          # DR ptr
             lw t5, 0(t5)
             bge t5, a1, MIN_STEP
             add a1, t5, x0
MIN_STEP:
             addi t0, t0, 48
             addi a7, a7, -40
             bne t0, t3, MIN_LOOP

DONE:
             addi a7, x0, 591
             ecall
             jalr x0, ra, 0
