include '../../include/ez80.inc'
include '../../include/tiformat.inc'

format ti archived appvar 'Toasters'

;---------------------------------
; Metadata structure:
;  - 2 bytes header "SAVR"
;  - Executable data offset
;    (Automatically calculated)
;  - Animation name <= 9 bytes:
;     - Name data <= 8 chars
;     - Null byte
;  - Author info <= 15 bytes:
;     - Author data <= 14 chars
;     - Null byte
;---------------------------------

metadata:
    db "SAVR"
    dl exec - metadata
    db "Toasters", 0
    db "TIny, RoccoLox", 0

exec:
    include 'bin/output.asm'
