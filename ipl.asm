; FAT12のための記述
    db      0xeb, 0x4e, 0x90
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
    db      0xb8, 0x00, 0x00, 0x8e, 0xd0, 0xbc, 0x00, 0x7c
    db      0x8e, 0xd8, 0x8e, 0xc0, 0xbe, 0x74, 0x7c, 0x8a
    db      0x04, 0x83, 0xc6, 0x01, 0x3c, 0x00, 0x74, 0x09
    db      0xb4, 0x0e, 0xbb, 0x0f, 0x00, 0xcd, 0x10, 0xeb
    db      0xee, 0xf4, 0xeb, 0xfd

; メッセージ部分
    db      0x0a, 0x0a   ; 改行2つ
    db      "hello, world"
    db      0x0a
    db      0

;  $: その行の最初
; $$: 現在のセクションの最初
    times   0x1fe-($-$$) db 0

    db      0x55, 0xaa

; ブートセクタ以外
    db      0xf0, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00
    times 4600 db 0
    db      0xf0, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00
    times 1469432 db 0
