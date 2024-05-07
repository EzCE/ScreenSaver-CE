    assume adl=1

    section .text

include 'include/ti84pceg.inc'

    public _installHook
    public _isHookInstalled

_installHook:
    ld hl, -1
    ld (hl), 2
    ld iy, ti.flags
    ld hl, _appvarName
    call ti.Mov9ToOP1
    call ti.ChkFindSym
    call ti.ChkInRam
    jr nz, $ + 8
    call ti.Arc_Unarc
    jr _installHook
    ld hl, 21
    add hl, de
    jp ti.SetGetCSCHook

_isHookInstalled:
    ld hl, -1
    ld (hl), 2
    ld iy, ti.flags
    xor a, a
    bit ti.getCSCHookActive, (iy + ti.hookflags2)
    ret z
    ld hl, _appvarName
    call ti.Mov9ToOP1
    call ti.ChkFindSym
    call ti.ChkInRam
    ld a, 0
    ret z
    ld hl, 21
    add hl, de
    ld de, (ti.getKeyHookPtr)
    xor a, a
    sbc hl, de
    ret nz
    inc a
    ret

    section .rodata

    public _appvarName

_appvarName:
    db ti.AppVarObj, "ScrnSavr", 0
