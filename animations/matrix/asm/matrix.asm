include '../../include/ez80.inc'
include '../../include/tiformat.inc'

format ti archived appvar 'Matrix'

;---------------------------------
; Metadata structure:
;  - 4 bytes header "SAVR"
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
    db "Matrix", 0
    db "Merth, v3x3d", 0

exec:
    include 'bin/output.asm'
