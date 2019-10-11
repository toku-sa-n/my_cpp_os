; このプログラムが読み込まれる場所の指定
    org     0x7c00

; FAT12のための記述
    jmp     entry
    db      0x90
    db      "HELLOIPL"  ; ブートセクタの名前
    dw      512         ; 1セクタの大きさ
    db      1           ; クラスタの大きさ
    dw      1           ; FATの開始地点
    db      2           ; FATの個数
    dw      224         ; ルートディレクトリ領域の大きさ
    dw      2880        ; このドライブの大きさ
    db      0xf0        ; メディアのタイプ
    dw      9           ; FAT領域の長さ
    dw      18          ; 1トラックあたりのセクタの数
    dw      2           ; ヘッドの数
    dd      0           ; パーティション未使用
    dd      2880        ; このドライブの大きさ
    db      0,0,0x29
    dd      0xffffffff  ; ボリュームシリアル番号
    db      "MY-OS      "
    db      "FAT12   "
    times 18 db 0

; プログラム本体
entry:

; レジスタ初期化
    mov     ax,0
    mov     ss,ax
    mov     sp,0x7c00
    mov     ds,ax
    mov     es,ax

    mov     si,msg

putloop:
    mov     al,[si]
    add     si,1
    cmp     al,0
    je      fin
    mov     ah,0x0e
    mov     bx,15
    int     0x10
    jmp     putloop

fin:
    hlt
    jmp     fin

msg:
    db      0x0a, 0x0a
    db      "hello, world"
    db      0x0a
    db      0
    times   0x7dfe-0x7c00-($-$$) db 0
    db      0x55, 0xaa

; ブートセクタ以外
    db      0xf0, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00
    times 4600 db 0
    db      0xf0, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00
    times 1469432 db 0
