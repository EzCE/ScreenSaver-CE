    assume adl=1

    section .text

include 'include/ti84pceg.inc'

    public _installHook
    public _isHookInstalled

    extern hookStart

_installHook:
    ld iy, ti.flags
    ld hl, hookStart
    jp ti.SetGetCSCHook

_isHookInstalled:
    ld iy, ti.flags
    xor a, a
    bit ti.getCSCHookActive, (iy + ti.hookflags2)
    ret z
    ld hl, hookStart
    ld de, (ti.getKeyHookPtr)
    xor a, a
    sbc hl, de
    ret nz
    inc a
    ret
