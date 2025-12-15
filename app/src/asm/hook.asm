    assume adl=1

    section .text

include 'include/ti84pceg.inc'

    public hookStart

;---------------------------------

screenBackupSize := (ti.lcdWidth * (ti.lcdHeight - 30)) / 2
statusBarBackupSize := (300 * 10) * 2
hookBackup := ti.pixelShadow2
returnVal := hookBackup + 3

macro spi cmd, params&
    ld a, cmd
    call spiCmd
    match any, params
        iterate param, any
            ld a, param
            call spiParam
        end iterate
    end match
end macro

;---------------------------------

hookStart:
    db $83
    set ti.apdAble, (iy + ti.apdFlags)
    cp a, $1B
    ret nz
    bit ti.progExecuting, (iy + ti.newDispF)
    jr nz, abortHook
    ld a, b
    or a, a
    jr z, continueHook
    bit 7, (iy + ti.asm_Flag2)
    res 7, (iy + ti.asm_Flag2)
    jr z, abortHook
    ld b, 0

abortHook:
    or a, 1
    ld a, b
    ret

continueHook:
    ld de, (ti.apdTimer)
    ld hl, $25
    or a, a
    sbc hl, de
    jr c, abortHook
    ;---------------
    ld hl, -1
    ld (hl), 2
    ;---------------
    push bc
    ld hl, cProgSize + screenBackupSize + statusBarBackupSize + 128
    call ti.EnoughMem
    pop bc
    jr c, abortHook
    ld hl, (ti.curRow)
    push hl
    ; I don't know what these do but I need to preserve them since HomeUp messes with them
    ld.sis hl, ($2685)
    push hl
    ld.sis hl, ($059A)
    push hl
    push ix
    call ti.CleanAll
    call ti.DeleteTempPrograms
    ld hl, tempProg
    call ti.Mov9ToOP1
    ld hl, screenBackupSize + statusBarBackupSize
    ld a, ti.TempProgObj
    call ti.CreateVar
    inc de
    inc de
    ld hl, ti.vRam + ((ti.lcdWidth * 2) * 30)
    ld bc, screenBackupSize
    call storeScreen
    ld hl, cProgSize
    push hl
    ld (ti.asm_prgm_size), hl
    ld de, ti.userMem
    call ti.InsertMem
    ld hl, cProgExec + 2
    ld de, ti.userMem
    pop bc
    ldir
    ld hl, (ti.getKeyHookPtr)
    push hl
    ld hl, errorHandler
    call ti.PushErrorHandler
    call ti.ClrGetKeyHook
    call ti.userMem
    ld (returnVal), hl
    call ti.PopErrorHandler

errorHandler:
    pop hl
    call ti.SetGetCSCHook
    ld hl, ti.mpLcdCtrl
    ld de, (hl)
    ex de, hl
    ld bc, ti.lcdIntFront
    add hl, bc
    ex de, hl
    ld (hl), de
    ; Initialize the SPI interface
    ld hl, $2000B
    ld (ti.mpSpiRange + ti.spiCtrl1), hl
    ld hl, $1828
    ld (ti.mpSpiRange), hl
    ld hl, $0C
    ld (ti.mpSpiRange + ti.spiCtrl2), hl
    nop
    ld hl, $40
    ld (ti.mpSpiRange + ti.spiCtrl2), hl
    call ti.Delay10ms
    ld hl, $182B
    ld (ti.mpSpiRange), hl
    ld hl, $0C
    ld (ti.mpSpiRange + ti.spiCtrl2), hl
    nop
    ld hl, $40
    ld (ti.mpSpiRange + ti.spiCtrl2), hl
    call ti.Delay10ms
    ld hl, $21
    ld (ti.mpSpiRange + ti.spiIntCtrl), hl
    ld hl, $100
    ld (ti.mpSpiRange + ti.spiCtrl2), hl
    res ti.graphDraw, (iy + ti.graphFlags)
    ld de, (ti.asm_prgm_size)
    or a, a
    sbc hl, hl
    ld (ti.asm_prgm_size), hl
    ld hl, ti.userMem
    call ti.DelMem
    call findRestoreProgram
    ld hl, ti.mpLcdCtrl
    ld de, ti.lcdNormalMode
    ld (hl), de
    call ti.ApdSetup
    pop ix
    pop hl
    ; make sure these get restored after running the C program
    ld.sis ($059A), hl
    pop hl
    ld.sis ($2685), hl
    pop hl
    ld (ti.curRow), hl
    or a, a
    sbc hl, hl
    ld (hookBackup), hl
    bit ti.rawKeyHookActive, (iy + ti.hookflags2)
    jr z, setHook
    ld hl, (ti.rawKeyHookPtr)
    ld (hookBackup), hl

setHook:
    ld hl, rawKeyHook
    call ti.SetGetKeyHook
    set 7, (iy + ti.asm_Flag2)
    ld hl, (returnVal)
    bit 0, l
    jr z, .exit
    di
    call ti.EnableAPD
    ld a, 1
    ld hl, ti.apdSubTimer
    ld (hl), a
    inc hl
    ld (hl), a
    set ti.apdRunning, (iy + ti.apdFlags)
    ei

.exit:
    xor a, a
    inc a
    ret

rawKeyHook:
    db $83
    call ti.ClrRawKeyHook
    ld hl, (hookBackup)
    call ti.ChkHLIs0
    call nz, ti.SetGetKeyHook
    xor a, a
    ret

tempProg:
    db ti.TempProgObj, "Screen", 0

cProgExec:
    include 'ANIMATE.asm'
cProgSize := $ - cProgExec

storeScreen:
    push de
    ld de, 0
    ld e, (hl)
    inc hl
    ld a, (hl)
    inc hl
    add a, e
    push hl
    ld hl, colors
    ld e, a
    add hl, de
    ld a, (hl)
    pop hl
    add a, a
    add a, a
    add a, a
    add a, a
    push af
    ld e, (hl)
    inc hl
    ld a, (hl)
    inc hl
    add a, e
    push hl
    ld hl, colors
    ld e, a
    add hl, de
    ld e, (hl)
    pop hl
    pop af
    add a, e
    pop de
    ld (de), a
    inc de
    dec bc
    ld a, b
    or a, c
    jr nz, storeScreen
    ld hl, ti.vRam + (16 * ti.lcdWidth * 2) + (2 * 2)
    ld b, 10

loopBackupStatusBar:
    push bc
    push hl
    ld bc, 300 * 2
    ldir
    pop hl
    ld bc, ti.lcdWidth * 2
    add hl, bc
    pop bc
    djnz loopBackupStatusBar
    ret

findRestoreProgram:
    ld a, (ti.mpLcdRis)
    and a, ti.lcdIntVcomp
    jr z, findRestoreProgram
    ld (ti.mpLcdIcr), a
    spi $B0, $01 ; disable framebuffer copies
    spi $2C
    ld hl, tempProg
    call ti.Mov9ToOP1
    call ti.ChkFindSym
    inc de
    inc de
    ld hl, ti.vRam + ((ti.lcdWidth * 2) * 30)
    ld bc, screenBackupSize
    ld a, (ti.cxCurApp)
    cp a, ti.cxGraph
    jr nz, restoreScreen
    push de
    push bc
    call ti.DrawGraphBackground
    call ti.GrBufCpy
    pop hl
    pop de
    add hl, de
    jr redrawStatusBarLoop - 6

restoreScreen:
    push de
    ld a, (de)
    srl a
    srl a
    srl a
    srl a
    call getColorValue
    pop de
    push de
    ld a, (de)
    and a, $0F
    call getColorValue
    pop de
    inc de
    dec bc
    ld a, b
    or a, c
    jr nz, restoreScreen
    ex de, hl
    ld de, ti.vRam + (16 * ti.lcdWidth * 2) + (2 * 2)
    ld b, 10

redrawStatusBarLoop:
    push bc
    push de
    ld bc, 300 * 2
    ldir
    pop de
    ex de, hl
    ld bc, ti.lcdWidth * 2
    add hl, bc
    ex de, hl
    pop bc
    djnz redrawStatusBarLoop

lcdCurrLoop:
    ld a, (ti.mpLcdCurr + 2) ; a = *mpLcdCurr >> 16
    ld hl, (ti.mpLcdCurr + 1) ; hl = *mpLcdCurr >> 8
    sub a, h
    jr nz, lcdCurrLoop ; nz ==> lcdCurr may have updated mid-read; retry read
    ld de, ti.vRamEnd
    or a, a
    sbc hl, de
    jr z, resetVcomp
    ld a, ti.lcdIntVcomp
    ld (ti.mpLcdIcr), a

loop:
    ld a, (ti.mpLcdRis)
    bit ti.bLcdIntVcomp, a
    jr z, loop

resetVcomp:
    ld a, ti.lcdIntVcomp
    ld (ti.mpLcdIcr), a
    spi $B0, $11 ; enable framebuffer copies
    call ti.CleanAll
    jp ti.DeleteTempPrograms

spiParam:
    scf
    virtual
        jr nc, $
        load .jr_nc : byte from $$
    end virtual
    db .jr_nc

spiCmd:
    or a, a
    ld hl, ti.mpSpiData or 8 shl 8
    ld b, 3

.loop:	
    rla
    rla
    rla
    ld (hl), a
    djnz .loop
    ld l, h
    ld (hl), 1

.wait:
    ld l, ti.spiStatus + 1

.wait1:
    ld a, (hl)
    and a, $f0
    jr nz, .wait1
    dec l

.wait2:
    bit 2, (hl)
    jr nz, .wait2
    ld l, h
    ld (hl), a
    ret

getColorValue:
    push hl
    ld de, osColors
    or a, a
    sbc hl, hl
    ld l, a
    add hl, hl
    add hl, de
    pop de
    ldi
    ldi
    ex de, hl
    inc bc
    inc bc
    ret

osColors:
    dw $7FFE ; highlight
    dw $001F ; blue
    dw $F800 ; red
    dw $0000 ; black
    dw $F81F ; magenta
    dw $04E0 ; green
    dw $FC64 ; orange
    dw $B100 ; brown
    dw $0010 ; navy
    dw $049F ; light blue
    dw $FFE0 ; yellow
    dw $FFFF ; white
    dw $E71C ; light gray
    dw $C618 ; medium gray
    dw $8C51 ; gray
    dw $52AA ; dark gray

colors:
    db 3 ; black
    db 2 dup 0
    db 12 ; light gray
    db 12 dup 0
    db 8 ; navy
    db 6 dup 0
    db 4 ; magenta
    db 7 dup 0
    db 1 ; blue
    db 64 dup 0
    db 6 ; orange
    db 28 dup 0
    db 0 ; highlight color
    db 37 dup 0
    db 9 ; light blue
    db 13 dup 0
    db 7 ; brown
    db 43 dup 0
    db 14 ; gray
    db 13 ; medium gray
    db 10 ; yellow
    db 4 dup 0
    db 5 ; green
    db 19 dup 0
    db 2 ; red
    db 3 dup 0
    db 15 ; dark gray
    db 0 ; null
    db 11 ; white
    db 0 ; null
hookEnd:
