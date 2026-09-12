    .assume adl=1

    .include "src/asm/include/ti84pceg.inc"

    .global _installHook
    .type   _installHook, @function
    .global _isHookInstalled
    .type   _isHookInstalled, @function
    .global _previewAnimation
    .type   _previewAnimation, @function

    .extern hookStart
    .extern _exit.sp

    .section .text

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

_previewAnimation:
    call _exit_sp
    pop de
    pop hl
    push de
    dec hl
    call ti.Mov9ToOP1
    ld a, ti.AppVarObj
    ld (ti.OP1), a
    call ti.ChkFindSym
    call ti.ChkInRam
    jr z, _previewAnimation.inRam
    ld hl, 10
    add hl, de
    ld a, c
    ld bc, 0
    ld c, a
    add hl, bc
    ex de, hl

_previewAnimation.inRam:
    ex de, hl
    ld bc, 0
    ld c, (hl)
    inc hl
    ld b, (hl)
    inc hl
    inc hl
    inc hl
    inc hl
    inc hl
    ld de, (hl)
    add hl, de
    dec hl
    dec hl
    push bc
    push hl
    ld hl, 128
    add hl, bc
    call ti.EnoughMem
    pop hl
    pop bc
    jr c, _previewAnimation.error
    push bc
    push hl
    ld de, (ti.asm_prgm_size)
    ld hl, ti.userMem
    push hl
    push bc
    call ti.DelMem
    pop hl
    ld (ti.asm_prgm_size), hl
    pop de
    push de
    call ti.InsertMem
    pop de
    pop hl
    pop bc
    ldir
    ld hl, _previewAnimation.error
    call ti.PushErrorHandler
    call ti.userMem
    call ti.PopErrorHandler

_previewAnimation.error:
    ld hl, _appName
    call ti.FindAppStart
    push hl
    push hl

_previewAnimation.debounce:
    call ti.GetCSC
    or a, a
    jr nz, _previewAnimation.debounce
    ld a, $25
    ld ($D02687), a
    xor a, a
    ld (ti.menuCurrent), a
    ld (ti.appErr1), a
    ld (ti.kbdGetKy), a
    ld de, ti.userMem
    ld hl, (ti.asm_prgm_size)
    ex de, hl
    call ti.DelMem
    or a, a
    sbc hl, hl
    ld (ti.asm_prgm_size), hl
    res ti.useTokensInString, (iy + ti.clockFlags)
    res ti.onInterrupt, (iy + ti.onFlags)
    set ti.graphDraw, (iy + ti.graphFlags)
    call ti.ReloadAppEntryVecs
    call ti.AppSetup
    set ti.appRunning, (iy + ti.APIFlg)
    set 6, (iy + $28)
    res 0, (iy + $2C)
    set ti.appAllowContext, (iy + ti.APIFlg)
    ld hl, $D1787C 
    ld bc, $FFF
    call ti.MemClear
    ; get proper stuff for the app
    ; call _asm_utils_clrScrnAndUsedRAM
    ; call ti.DrawStatusBar
    pop de
    ex (sp), hl
    push de
    ld sp, (ti.onSP) ; Don't bork the stack (safety first)
    push hl
    call ti.ResetStacks
    pop hl
    ld de, $100
    add hl, de
    push hl
    ex de, hl ; code pointer in de
    ld hl, $18
    add hl, de
    ld hl, (hl)
    add hl, de
    or a, a
    sbc hl, de
    jr z, _previewAnimation.noBss
    push hl
    pop bc
    ld hl, $15
    add hl, de
    ld hl, (hl)
    add hl, de
    ld de, $D1787C
    ldir

_previewAnimation.noBss:
    pop hl
    push hl
    pop de
    ld bc, $1B
    add hl, bc
    ld hl, (hl)
    add hl, de
    jp (hl)

_exit_sp:
    push hl
    ld iy, ti.flags
    ld a, (_exit.sp + 3)
    ld (0xE00305), a
    call ti.usb_ResetTimer
    ld a, 1
    ld (0xF00008), a ; clear on interrupt
    res 4, (iy + 0x09) ; onInterrupt,(iy+onFlags)
    set 0, (iy + 0x03) ; graphDraw,(iy+graphFlags)
    set 1, (iy + 0x0D) ; use text buffer
    res 3, (iy + 0x4A) ; use first shadow buffer
    res 5, (iy + 0x4C) ; use shadow buffer
    call ti.ClrLCDFull
    call ti.HomeUp
    call ti.DrawStatusBar
    pop hl ; hl = exit code
    ret

_appName:
    db "ScrnSavr", 0
